#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QObject>
#include <QVariantMap>

class UserManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isLoggedIn READ isLoggedIn NOTIFY sessionChanged)
    Q_PROPERTY(QVariantMap currentUser READ currentUser NOTIFY sessionChanged)

public:
    explicit UserManager(QObject *parent = nullptr);

    Q_INVOKABLE bool login(const QString &username, const QString &password);
    Q_INVOKABLE void logout();
    Q_INVOKABLE bool hasBiometrics(int userId);
    
    // Future CRUD methods
    // Q_INVOKABLE bool createUser(...)

    bool isLoggedIn() const;
    QVariantMap currentUser() const;

signals:
    void sessionChanged();
    void loginFailed(const QString &reason);
    void loginSuccess(const QString &username, const QString &role);

private:
    QVariantMap m_currentUser;
    bool m_loggedIn;
};

#endif // USERMANAGER_H
