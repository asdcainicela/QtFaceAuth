#include "UserManager.h"
#include "../database/DatabaseManager.h"
#include <QDebug>

UserManager::UserManager(QObject *parent) : QObject(parent), m_loggedIn(false)
{
}

bool UserManager::login(const QString &username, const QString &password)
{
    // 1. Verify credentials against DB
    QVariantMap result = DatabaseManager::instance().verifyUserPassword(username, password);

    if (result["valid"].toBool()) {
        m_currentUser = result;
        m_loggedIn = true;
        
        qDebug() << "Login Success:" << username << "Role:" << result["role"];
        
        // Log the event
        int userId = result["id"].toInt();
        DatabaseManager::instance().logAccess(userId, "PASSWORD", 1.0, "SUCCESS");

        emit sessionChanged();
        emit loginSuccess(username, result["role"].toString());
        return true;
    }

    qWarning() << "Login Failed for:" << username;
    emit loginFailed("Invalid username or password");
    return false;
}

void UserManager::logout()
{
    m_loggedIn = false;
    m_currentUser.clear();
    emit sessionChanged();
}

bool UserManager::hasBiometrics(int userId)
{
    return DatabaseManager::instance().hasBiometrics(userId);
}

bool UserManager::isLoggedIn() const
{
    return m_loggedIn;
}

QVariantMap UserManager::currentUser() const
{
    return m_currentUser;
}
