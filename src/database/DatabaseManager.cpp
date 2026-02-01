#include "DatabaseManager.h"

DatabaseManager& DatabaseManager::instance()
{
    static DatabaseManager instance;
    return instance;
}

DatabaseManager::DatabaseManager(QObject *parent)
    : QObject(parent), m_connected(false)
{
    // Define path in AppData/QtFaceAuth/db/faceauth.db to match our Python script
    // Or user local folder for dev
    QString dataLocation = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    // In dev, we might want to point to the local project "db" folder for testing
    // For now, let's look for the one we just created in the project root relative to execution
    // PROD: Use dataLocation
}

DatabaseManager::~DatabaseManager()
{
    close();
}

bool DatabaseManager::connectToDatabase()
{
    if (m_connected) return true;

    // Determine Path (Search up the tree for 'db' folder if in shadow build)
    // Determine Path (Search up the tree for 'db' folder)
    // We want to find the source 'db' folder, not a shadow copy if possible.
    QDir dir(QDir::currentPath());
    bool found = false;
    
    // Try current, then parent, up to 4 levels
    for (int i = 0; i < 4; ++i) {
        if (dir.exists("db") && QFileInfo::exists(dir.filePath("db/faceauth.db"))) {
             if (dir.cd("db")) {
                 found = true;
                 break;
             }
        }
        if (!dir.cdUp()) break;
    }

    if (!found) {
        qWarning() << "Could not find 'db/faceauth.db' searching up from" << QDir::currentPath();
        // Fallback to creating one in current/db if not found?
        // For now, let's keep it clean and fail or use AppData (but code below uses dir.filePath)
    }
    
    QString dbFile = dir.filePath("faceauth.db");
    qDebug() << "Connecting to database at:" << dbFile;

    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(dbFile);

    if (!m_db.open()) {
        qCritical() << "Error opening database:" << m_db.lastError().text();
        emit databaseError(m_db.lastError().text());
        return false;
    }

    // Enable Foreign Keys
    QSqlQuery query;
    query.exec("PRAGMA foreign_keys = ON");

    m_connected = true;
    emit connectionStatusChanged(true);
    qDebug() << "Database connected successfully.";
    return true;
}

void DatabaseManager::close()
{
    if (m_db.isOpen()) {
        m_db.close();
    }
    m_connected = false;
    emit connectionStatusChanged(false);
}

bool DatabaseManager::isConnected() const
{
    return m_connected;
}

QVariantMap DatabaseManager::getUserByUuid(const QString &uuid)
{
    QVariantMap user;
    if (!m_connected) return user;

    if (!m_connected) return user;

    // Optimized query to fetch user and joined role name
    QSqlQuery query;
    query.prepare(R"(
        SELECT u.id, u.username, u.full_name, u.uuid, r.name as role_name 
        FROM users u 
        LEFT JOIN user_roles ur ON u.id = ur.user_id 
        LEFT JOIN roles r ON ur.role_id = r.id 
        WHERE u.uuid = :uuid AND u.is_active = 1
    )");
    query.bindValue(":uuid", uuid);

    if (query.exec() && query.next()) {
        user["id"] = query.value("id");
        user["username"] = query.value("username");
        user["full_name"] = query.value("full_name");
        user["uuid"] = query.value("uuid");
        user["role"] = query.value("role_name");
    } else {
        qWarning() << "User not found or query failed:" << query.lastError().text();
    }
    return user;
}

QVariantMap DatabaseManager::verifyUserPassword(const QString &username, const QString &password)
{
    QVariantMap result;
    if (!m_connected) return result;

    QSqlQuery query;
    // In production, use bcrypt verify. Here we compare plain/simple hash.
    query.prepare(R"(
        SELECT u.id, u.uuid, u.full_name, r.name as role_name
        FROM users u
        LEFT JOIN user_roles ur ON u.id = ur.user_id
        LEFT JOIN roles r ON ur.role_id = r.id
        WHERE u.username = :username AND u.password_hash = :pass AND u.is_active = 1
    )");
    query.bindValue(":username", username);
    query.bindValue(":pass", password);

    if (query.exec() && query.next()) {
        result["valid"] = true;
        result["id"] = query.value("id");
        result["uuid"] = query.value("uuid");
        result["full_name"] = query.value("full_name");
        result["role"] = query.value("role_name");
    } else {
        result["valid"] = false;
    }
    return result;
}

bool DatabaseManager::hasBiometrics(int userId)
{
    if (!m_connected) return false;
    QSqlQuery query;
    query.prepare("SELECT count(*) FROM biometrics WHERE user_id = :uid");
    query.bindValue(":uid", userId);
    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;
    }
    return false;
}

bool DatabaseManager::hasAnyBiometrics()
{
    if (!m_connected) return false;
    QSqlQuery query;
    if (query.exec("SELECT count(*) FROM biometrics") && query.next()) {
        return query.value(0).toInt() > 0;
    }
    return false;
}

bool DatabaseManager::logAccess(int userId, const QString &method, double confidence, const QString &eventType)
{
    if (!m_connected) return false;

    QSqlQuery query;
    query.prepare("INSERT INTO sessions (user_id, login_method, confidence, event_type) VALUES (:uid, :method, :conf, :event)");
    query.bindValue(":uid", userId);
    query.bindValue(":method", method);
    query.bindValue(":conf", confidence);
    query.bindValue(":event", eventType);

    if (!query.exec()) {
        qCritical() << "Failed to log access:" << query.lastError().text();
        return false;
    }
    return true;
}

QList<QVariantMap> DatabaseManager::getAllUsersWithBiometrics()
{
    QList<QVariantMap> results;
    if (!m_connected) return results;

    QSqlQuery query;
    // Join users, roles, and biometrics
    // We only care about users who HAVE biometrics
    query.prepare(R"(
        SELECT u.id, u.username, u.full_name, u.uuid, r.name as role_name, b.features
        FROM users u
        JOIN biometrics b ON u.id = b.user_id
        LEFT JOIN user_roles ur ON u.id = ur.user_id
        LEFT JOIN roles r ON ur.role_id = r.id
        WHERE u.is_active = 1
    )");

    if (query.exec()) {
        while (query.next()) {
            QVariantMap user;
            user["id"] = query.value("id");
            user["username"] = query.value("username");
            user["full_name"] = query.value("full_name");
            user["uuid"] = query.value("uuid");
            user["role"] = query.value("role_name");
            user["features"] = query.value("features");
            results.append(user);
        }
    } else {
        qWarning() << "Failed to fetch biometric users:" << query.lastError().text();
    }
    return results;
}

bool DatabaseManager::registerBiometrics(int userId, const QByteArray &features)
{
    if (!m_connected) return false;

    // Check if user already has biometrics, if so, update? Or just insert new row.
    // For now, let's assume one face per user or multiple.
    // Let's just insert.
    QSqlQuery query;
    query.prepare("INSERT INTO biometrics (user_id, features, created_at) VALUES (:uid, :vec, DATETIME('now'))");
    query.bindValue(":uid", userId);
    query.bindValue(":vec", features);

    if (!query.exec()) {
        qCritical() << "Failed to register biometrics:" << query.lastError().text();
        return false;
    }
    return true;
}

QList<QVariantMap> DatabaseManager::getAllUsers()
{
    QList<QVariantMap> results;
    if (!m_connected) return results;

    QSqlQuery query;
    query.prepare(R"(
        SELECT u.id, u.username, u.full_name, u.uuid, u.is_active, r.name as role_name, 
               (SELECT COUNT(*) FROM biometrics b WHERE b.user_id = u.id) as output_biometrics
        FROM users u
        LEFT JOIN user_roles ur ON u.id = ur.user_id
        LEFT JOIN roles r ON ur.role_id = r.id
        ORDER BY u.username ASC
    )");

    if (query.exec()) {
        while (query.next()) {
            QVariantMap user;
            user["id"] = query.value("id");
            user["username"] = query.value("username");
            user["full_name"] = query.value("full_name");
            user["uuid"] = query.value("uuid");
            user["role"] = query.value("role_name");
            user["is_active"] = query.value("is_active");
            user["has_biometrics"] = query.value("output_biometrics").toInt() > 0;
            results.append(user);
        }
    } else {
        qWarning() << "Failed to fetch users:" << query.lastError().text();
    }
    return results;
}
