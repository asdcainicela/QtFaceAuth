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

    // Determine Path (Dev Mode: currently in project root/db)
    // We try to find the file we created with python
    QDir dir(QDir::currentPath());
    if (!dir.cd("db")) {
        qWarning() << "Could not find 'db' folder in" << QDir::currentPath();
        // Fallback or create?
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

    QSqlQuery query;
    query.prepare("SELECT id, username, full_name, role FROM users WHERE uuid = :uuid AND is_active = 1");
    query.bindValue(":uuid", uuid);

    if (query.exec() && query.next()) {
        user["id"] = query.value("id");
        user["username"] = query.value("username");
        user["full_name"] = query.value("full_name");
        user["role"] = query.value("role"); // Need to join roles table properly in real impl
    } else {
        qWarning() << "User not found or query failed:" << query.lastError().text();
    }
    return user;
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
