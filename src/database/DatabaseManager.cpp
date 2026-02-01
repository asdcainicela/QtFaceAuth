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
    QDir dir(QDir::currentPath());
    
    // Try current, then parent, then parent's parent
    for (int i = 0; i < 3; ++i) {
        if (dir.exists("db")) {
            if (dir.cd("db")) break;
        }
        if (!dir.cdUp()) break;
    }

    if (!QFileInfo::exists(dir.filePath("faceauth.db"))) {
        qWarning() << "Could not find 'db/faceauth.db' starting from" << QDir::currentPath();
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
