#ifndef QAWESOMEGIFBACKEND_H
#define QAWESOMEGIFBACKEND_H

#include "../qawesomeanimationbackend.h"
#include <QMovie>
#include <QBuffer>
#include <QImageReader>

class QAwesomeGifBackend : public QAwesomeAnimationBackend
{
    Q_OBJECT
public:
    explicit QAwesomeGifBackend(QObject* parent = nullptr);

    bool loadFromFile(const QString& path) override;
    bool loadFromData(const QByteArray& data) override;

    int frameCount() const override { return m_frames.size(); }
    QSize frameSize() const override { return m_frameSize; }
    int defaultFrameDelayMs(int frameIndex) const override;

    QImage renderFrame(int frameIndex, const QSize& targetLogicalSize, qreal dpr, QAwesomeScaleMode scaleMode) override;

    bool isValid() const override { return !m_frames.isEmpty(); }

private:
    QVector<QImage> m_frames;
    QVector<int> m_delays;
    QSize m_frameSize;

};

#endif // QAWESOMEGIFBACKEND_H
