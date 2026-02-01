#ifndef FACEENGINE_H
#define FACEENGINE_H

#include <QObject>
#include <QImage>
#include <QVector>

#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>

class FaceEngine : public QObject
{
    Q_OBJECT
public:
    static FaceEngine& instance();

    // Returns extracted features (raw grayscale pixels of resized face, 128x128 = 16KB)
    Q_INVOKABLE QByteArray extractFeatures(const QImage &img);
    
    // Compare two vectors (returns similarity 0.0 - 1.0)
    // Uses Mean Squared Error (MSE) logic for V1
    Q_INVOKABLE float compare(const QByteArray &face1, const QByteArray &face2);

private:
    explicit FaceEngine(QObject *parent = nullptr);
    
    cv::CascadeClassifier m_faceCascade;
    bool m_modelLoaded;
    
    // Helper
    cv::Mat qImageToCvMat(const QImage &inImage);
};

#endif // FACEENGINE_H
