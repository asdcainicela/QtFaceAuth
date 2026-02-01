#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>
#include <QStandardPaths>
#include <QDir>

/**
 * @brief Handles all convenient access to the SQLite database.
 * Follows the Singleton pattern for global access.
 */
class DatabaseManager : public QObject
{
    Q_OBJECT
    // Expose connectivity status to QML
    Q_PROPERTY(bool isConnected READ isConnected NOTIFY connectionStatusChanged)

public:
    static DatabaseManager& instance();

    // Core Setup
    bool connectToDatabase();
    void close();
    bool isConnected() const;

    // User Operations
    Q_INVOKABLE QVariantMap getUserByUuid(const QString &uuid);
    Q_INVOKABLE QVariantMap verifyUserPassword(const QString &username, const QString &password);
    Q_INVOKABLE bool hasBiometrics(int userId);
    Q_INVOKABLE bool hasAnyBiometrics();
    Q_INVOKABLE bool logAccess(int userId, const QString &method, double confidence, const QString &eventType);
    
    // Face Auth
    // Returns List of Maps: {user_id, username, uuid, role_name, features (QByteArray)}
    QList<QVariantMap> getAllUsersWithBiometrics();
    Q_INVOKABLE bool registerBiometrics(int userId, const QByteArray &features);

    // Management
    Q_INVOKABLE QList<QVariantMap> getAllUsers();

signals:
    void connectionStatusChanged(bool connected);
    void databaseError(const QString &errorMsg);

private:
    explicit DatabaseManager(QObject *parent = nullptr);
    ~DatabaseManager();
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    QSqlDatabase m_db;
    bool m_connected;
    QString m_dbPath;
};

#endif // DATABASEMANAGER_H
