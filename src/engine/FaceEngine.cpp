#include "FaceEngine.h"
#include <QDebug>
#include <QThread>

#include "FaceEngine.h"
#include <QDebug>
#include <QCoreApplication>
#include <QDir>

FaceEngine& FaceEngine::instance()
{
    static FaceEngine instance;
    return instance;
}

FaceEngine::FaceEngine(QObject *parent) : QObject(parent), m_modelLoaded(false)
{
    // Try to load Haar Cascade
    // Standard path in vcpkg
    QString cascadePath = "C:/vcpkg/installed/x64-windows/share/opencv4/haarcascades/haarcascade_frontalface_default.xml";
    
    if (!QFileInfo::exists(cascadePath)) {
        // Fallback to local execution dir
        cascadePath = QCoreApplication::applicationDirPath() + "/haarcascade_frontalface_default.xml";
    }

    if (m_faceCascade.load(cascadePath.toStdString())) {
        qDebug() << "FaceEngine: Loaded face model from" << cascadePath;
        m_modelLoaded = true;
    } else {
        qCritical() << "FaceEngine: FAILED to load face model from" << cascadePath;
    }
}

QByteArray FaceEngine::extractFeatures(const QImage &img)
{
    if (!m_modelLoaded) {
        qWarning() << "FaceEngine: Model not loaded! Cannot extract features.";
        return QByteArray();
    }

    // 1. Convert to cv::Mat
    cv::Mat mat = qImageToCvMat(img);
    if (mat.empty()) return QByteArray();

    // 2. Convert to Grayscale
    cv::Mat gray;
    if (mat.channels() == 3) cv::cvtColor(mat, gray, cv::COLOR_BGR2GRAY);
    else if (mat.channels() == 4) cv::cvtColor(mat, gray, cv::COLOR_BGRA2GRAY);
    else gray = mat;

    // 3. Detect Face
    std::vector<cv::Rect> faces;
    m_faceCascade.detectMultiScale(gray, faces, 1.1, 5, 0, cv::Size(100, 100));

    if (faces.empty()) {
        qDebug() << "FaceEngine: No face detected.";
        return QByteArray();
    }

    // 4. Extract largest face
    cv::Rect largestFace = faces[0];
    for (const auto &f : faces) {
        if (f.area() > largestFace.area()) largestFace = f;
    }

    cv::Mat faceROI = gray(largestFace);

    // 5. Resize to standard (128x128)
    cv::Mat resized;
    cv::resize(faceROI, resized, cv::Size(128, 128));

    // 6. Serialize (Raw Bytes)
    // 128*128 = 16384 bytes
    QByteArray features((const char*)resized.data, resized.total() * resized.elemSize());
    
    qDebug() << "FaceEngine: Face extracted! Size:" << features.size();
    return features;
}

float FaceEngine::compare(const QByteArray &face1, const QByteArray &face2)
{
    if (face1.isEmpty() || face2.isEmpty()) return 0.0f;
    if (face1.size() != face2.size()) return 0.0f;

    // Simple MSE (Mean Squared Error) Comparison
    // V1 Logic: Lower difference = Higher similarity
    // This is NOT production grade recognition, but robust enough for a demo
    
    const unsigned char* d1 = (const unsigned char*)face1.constData();
    const unsigned char* d2 = (const unsigned char*)face2.constData();
    
    double mse = 0.0;
    for (int i = 0; i < face1.size(); ++i) {
        int diff = d1[i] - d2[i];
        mse += diff * diff;
    }
    mse /= face1.size();
    
    // Normalize logic: 
    // MSE 0 => 1.0 (Perfect)
    // MSE > 10000 => 0.0 (Different)
    
    float similarity = 1.0f - (float)(mse / 10000.0);
    if (similarity < 0) similarity = 0;
    
    qDebug() << "FaceEngine: MSE:" << mse << "Similarity:" << similarity;
    return similarity;
}

cv::Mat FaceEngine::qImageToCvMat(const QImage &inImage)
{
    // Handle formats
    switch (inImage.format()) {
        case QImage::Format_RGB32:
        case QImage::Format_ARGB32:
        case QImage::Format_ARGB32_Premultiplied: {
            cv::Mat mat(inImage.height(), inImage.width(), CV_8UC4, 
                        (void*)inImage.constBits(), inImage.bytesPerLine());
            return mat.clone(); // Clone to avoid memory issues if QImage dies
        }
        case QImage::Format_RGB888: {
            cv::Mat mat(inImage.height(), inImage.width(), CV_8UC3, 
                        (void*)inImage.constBits(), inImage.bytesPerLine());
            cv::Mat matSwap;
            cv::cvtColor(mat, matSwap, cv::COLOR_RGB2BGR);
            return matSwap;
        }
        default:
            qWarning() << "FaceEngine: Unsupported QImage format:" << inImage.format();
            break;
    }
    return cv::Mat();
}
