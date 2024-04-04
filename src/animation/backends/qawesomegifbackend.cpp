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


QImage QAwesomeGifBackend::renderFrame(int frameIndex, const QSize& targetLogicalSize, qreal dpr, QAwesomeScaleMode scaleMode)
{
    if (m_frames.isEmpty()) return QImage();
    if (frameIndex < 0) frameIndex = 0;
    frameIndex %= m_frames.size();
    QImage out = scaled(m_frames.at(frameIndex), targetLogicalSize, dpr, scaleMode);
    qDebug() << "QAwesomeGifBackend: renderFrame index:" << frameIndex << "out size:" << out.size() << "targetLogicalSize:" << targetLogicalSize;
    return out;
}

bool QAwesomeGifBackend::loadWithQMovie(QIODevice* dev)
{
    QMovie movie;
    movie.setDevice(dev);
    movie.setCacheMode(QMovie::CacheAll);

    if (!movie.isValid()) {
        qDebug() << "QAwesomeGifBackend: QMovie invalid";
        return false;
    }

    movie.start();
    movie.stop();

    m_frames.clear();
    m_delays.clear();

    int count = movie.frameCount();
    if (count <= 0) {
        int guard = 0;
        while (guard < 10000) {
            QImage img = movie.currentImage();
            if (img.isNull()) break;
            if (m_frameSize.isEmpty()) m_frameSize = img.size();
            m_frames.push_back(img.convertToFormat(QImage::Format_ARGB32_Premultiplied));
            m_delays.push_back(movie.nextFrameDelay() > 0 ? movie.nextFrameDelay() : 33);
            if (!movie.jumpToNextFrame()) break;
            guard++;
        }
    } else {
        for (int i = 0; i < count; ++i) {
            movie.jumpToFrame(i);
            QImage img = movie.currentImage();
            if (i == 0 && !img.isNull() && m_frameSize.isEmpty()) m_frameSize = img.size();
            if (!img.isNull()) {
                m_frames.push_back(img.convertToFormat(QImage::Format_ARGB32_Premultiplied));
                int delay = movie.nextFrameDelay();
                if (delay <= 0) delay = 33;
                m_delays.push_back(delay);
                qDebug() << "QAwesomeGifBackend: QMovie frame" << i << "size:" << img.size() << "delay:" << delay << "ms";
            }
        }
    }

    qDebug() << "QAwesomeGifBackend: QMovie total frames:" << m_frames.size() << "frameSize:" << m_frameSize;
    return !m_frames.isEmpty();
}
