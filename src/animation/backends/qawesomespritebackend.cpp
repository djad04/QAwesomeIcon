#include "qawesomespritebackend.h"
#include <QImageReader>
#include <QFile>

QAwesomeSpriteBackend::QAwesomeSpriteBackend(QObject* parent)
    : QAwesomeAnimationBackend(parent)
{
}


bool QAwesomeSpriteBackend::loadSpriteSheet(const QString& imagePath, const QSize& frameSize, int frameCount, int framesPerRow)
{
    QImage sheet(imagePath);
    if (sheet.isNull()) return false;
    m_frameSize = frameSize;

    if (framesPerRow <= 0) framesPerRow = sheet.width() / frameSize.width();
    int rows = (frameCount + framesPerRow - 1) / framesPerRow;

    m_frames.clear();
    m_frames.reserve(frameCount);

    for (int i = 0; i < frameCount; ++i) {
        int row = i / framesPerRow;
        int col = i % framesPerRow;
        QRect r(col * frameSize.width(), row * frameSize.height(), frameSize.width(), frameSize.height());
        QImage frame = sheet.copy(r).convertToFormat(QImage::Format_ARGB32_Premultiplied);
        m_frames.push_back(frame);
    }
    return !m_frames.isEmpty();
}

QImage QAwesomeSpriteBackend::scaled(const QImage& src, const QSize& targetLogicalSize, qreal dpr, QAwesomeScaleMode scaleMode) const
{
    if (targetLogicalSize.isEmpty()) return src;
    QSize targetPx = targetLogicalSize * dpr;
    Qt::AspectRatioMode arm = Qt::KeepAspectRatio;
    if (scaleMode == QAwesomeScaleMode::IgnoreAspectRatio) arm = Qt::IgnoreAspectRatio;
    else if (scaleMode == QAwesomeScaleMode::KeepAspectRatioByExpanding) arm = Qt::KeepAspectRatioByExpanding;
    return src.scaled(targetPx, arm, Qt::SmoothTransformation);
}

QImage QAwesomeSpriteBackend::renderFrame(int frameIndex, const QSize& targetLogicalSize, qreal dpr, QAwesomeScaleMode scaleMode)
{
    if (m_frames.isEmpty()) return QImage();
    if (frameIndex < 0) frameIndex = 0;
    frameIndex %= m_frames.size();
    return scaled(m_frames.at(frameIndex), targetLogicalSize, dpr, scaleMode);
}


bool QAwesomeSpriteBackend::loadFromFile(const QString &path)
{
    return false; //unsuported
}



bool QAwesomeSpriteBackend::loadFromData(const QByteArray &data)
{
    return false; // unsuported
}

