#ifndef QAWESOMESPRITEBACKEND_H
#define QAWESOMESPRITEBACKEND_H

#include "../qawesomeanimationbackend.h"
#include <QImage>

class QAwesomeSpriteBackend : public QAwesomeAnimationBackend
{
    Q_OBJECT
public:
    explicit QAwesomeSpriteBackend(QObject* parent = nullptr);

    // unsupported here but has to be declared (QAwesomeAnimationBackend is abstract)
    bool loadFromFile(const QString& path) override; // unsupported
    bool loadFromData(const QByteArray& data) override; // unsupported

    bool loadSpriteSheet(const QString& imagePath, const QSize& frameSize, int frameCount, int framesPerRow = 0);

    int frameCount() const override { return m_frames.size(); }
    QSize frameSize() const override { return m_frameSize; }
    int defaultFrameDelayMs(int) const override { return 33; }

    QImage renderFrame(int frameIndex, const QSize& targetLogicalSize, qreal dpr, QAwesomeScaleMode scaleMode) override;

    bool isValid() const override { return !m_frames.isEmpty(); }

private:
    QVector<QImage> m_frames;
    QSize m_frameSize;

    QImage scaled(const QImage& src, const QSize& targetLogicalSize, qreal dpr, QAwesomeScaleMode scaleMode) const;
};

#endif // QAWESOMESPRITEBACKEND_H
