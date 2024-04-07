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
