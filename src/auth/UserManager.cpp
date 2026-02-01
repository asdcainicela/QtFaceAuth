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
    
    // 3. Save to Database
    if (DatabaseManager::instance().registerBiometrics(userId, faceEmbedding)) {
        return DatabaseManager::instance().logAccess(userId, "face_enroll", 1.0, "enrollment");
    } else {
        return false;
    }
}

bool UserManager::authenticateWithFace(const QString &imagePath)
{
    QImage img(imagePath);
    if (img.isNull()) {
        emit loginFailed("Invalid image capture.");
        return false;
    }

    // 1. Extract Features from input
    QByteArray inputFeatures = FaceEngine::instance().extractFeatures(img);
    if (inputFeatures.isEmpty()) {
        emit loginFailed("No face detected in camera feed.");
        return false;
    }

    // 2. Get All Users
    QList<QVariantMap> candidates = DatabaseManager::instance().getAllUsersWithBiometrics();
    
    // 3. Find Best Match
    float bestScore = 0.0f;
    QVariantMap bestUser;

    for (const auto &user : candidates) {
        QByteArray storedFeatures = user["features"].toByteArray();
        float score = FaceEngine::instance().compare(inputFeatures, storedFeatures);
        
        qDebug() << "Comparing with" << user["username"] << "Score:" << score;

        if (score > bestScore) {
            bestScore = score;
            bestUser = user;
        }
    }

    // 4. Threshold (0.85 seems like a safe bet for MSE logic)
    // Adjust this threshold based on testing
    if (bestScore > 0.85f) {
        // Success!
        m_currentUser = bestUser;
        m_loggedIn = true;
        
        emit sessionChanged();
        emit loginSuccess(bestUser["username"].toString(), bestUser["role"].toString());
        
        DatabaseManager::instance().logAccess(bestUser["id"].toInt(), "face", bestScore, "login");
        qDebug() << "Face Login Success:" << bestUser["username"] << "Confidence:" << bestScore;
        return true;
    }

    qWarning() << "Face Login Failed. Best Score:" << bestScore;
    emit loginFailed("Face not recognized.");
    return false;
}

bool UserManager::isLoggedIn() const
{
    return m_loggedIn;
}

QVariantMap UserManager::currentUser() const
{
    return m_currentUser;
}
