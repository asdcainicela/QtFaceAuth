#include "UserManager.h"
#include "../database/DatabaseManager.h"
#include "../engine/FaceEngine.h" // Include Engine
#include <QDebug>
#include <QUrl>

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

bool UserManager::registerFace(int userId, const QString &imagePath)
{
    qDebug() << "Registering face for User" << userId << "from" << imagePath;
    
    // 1. Load Image (Remove file:// prefix if present)
    QString localPath = QUrl(imagePath).toLocalFile();
    if (localPath.isEmpty()) localPath = imagePath;
    
    QImage img(localPath);
    if (img.isNull()) {
        qCritical() << "Failed to load image at" << localPath;
        return false;
    }
    
    // 2. Extract Features
    QByteArray faceEmbedding = FaceEngine::instance().extractFeatures(img);
    if (faceEmbedding.isEmpty()) {
        qWarning() << "No face detected in image.";
        return false;
    }
    
    // 3. Save to Database (We need to add this method to DatabaseManager)
    QSqlQuery query;
    query.prepare("INSERT INTO biometrics (user_id, face_vector) VALUES (:uid, :vec)");
    query.bindValue(":uid", userId);
    query.bindValue(":vec", faceEmbedding);
    
    if (!query.exec()) {
         qCritical() << "DB Error saving biometrics:" << query.lastError().text();
         return false;
    }
    
    return true;
}

bool UserManager::isLoggedIn() const
{
    return m_loggedIn;
}

QVariantMap UserManager::currentUser() const
{
    return m_currentUser;
}
