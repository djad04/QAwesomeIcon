#include "qawesomegifbackend.h"
#include <QFile>
#include <QDebug>

QAwesomeGifBackend::QAwesomeGifBackend(QObject* parent)
    : QAwesomeAnimationBackend(parent)
{
}

bool QAwesomeGifBackend::loadFromFile(const QString& path)
{
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly)) return false;
    return loadWithImageReader(&f);
}

bool QAwesomeGifBackend::loadFromData(const QByteArray& data)
{
    QBuffer buf;
    buf.setData(data);
    buf.open(QIODevice::ReadOnly);
    return loadWithImageReader(&buf);
}

bool QAwesomeGifBackend::loadWithImageReader(QIODevice* dev)
{
    // First try QMovie approach for better GIF support
    QMovie movie;
    movie.setDevice(dev);
    if (movie.isValid() && movie.frameCount() > 0) {
        return loadWithQMovie(dev);
    }

    // Fallback to QImageReader
    dev->seek(0); // Reset device position
    QImageReader reader;
    reader.setDecideFormatFromContent(true);
    reader.setDevice(dev);
    if (!reader.canRead()) return false;
    if (reader.format().toLower() != "gif") {
        // Allow GIF only for this backend
    }

    m_frames.clear();
    m_delays.clear();

    int frameIndex = 0;
    while (true) {
        QImage frame = reader.read();
        if (frame.isNull()) break;
        if (m_frameSize.isEmpty()) m_frameSize = frame.size();
        QImage converted = frame.convertToFormat(QImage::Format_ARGB32_Premultiplied);
        m_frames.push_back(converted);

        int delay = reader.nextImageDelay();
        if (delay <= 0) delay = 33;
        m_delays.push_back(delay);
        qDebug() << "QAwesomeGifBackend: Loaded frame" << frameIndex << "size:" << converted.size() << "delay:" << delay << "ms";
        frameIndex++;

        if (!reader.jumpToNextImage()) break;
    }
    qDebug() << "QAwesomeGifBackend: Total frames:" << m_frames.size() << "frameSize:" << m_frameSize;
    return !m_frames.isEmpty();
}
