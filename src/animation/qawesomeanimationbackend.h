#ifndef QAWESOMEANIMATIONBACKEND_H
#define QAWESOMEANIMATIONBACKEND_H

#include <QObject>
#include <QImage>
#include <QSize>
#include <QByteArray>
#include <QStringList>
#include "../qawesomeicon_global.h"

class QAwesomeAnimationBackend : public QObject
{
    Q_OBJECT
public:
    explicit QAwesomeAnimationBackend(QObject* parent = nullptr) : QObject(parent) {}
    ~QAwesomeAnimationBackend() override = default;

    virtual bool loadFromFile(const QString& path) = 0;
    virtual bool loadFromData(const QByteArray& data) = 0;

    virtual int frameCount() const = 0;
    virtual QSize frameSize() const = 0;

    virtual int defaultFrameDelayMs(int frameIndex) const = 0; // 0 if unknown

    // Render a frame at requested size and DPR
    virtual QImage renderFrame(int frameIndex, const QSize& targetLogicalSize, qreal dpr, QAwesomeScaleMode scaleMode) = 0;

    virtual bool supportsSeeking() const { return true; }
    virtual bool isValid() const = 0;

    virtual void setSpeedFactor(qreal factor) { Q_UNUSED(factor) }
};

#endif // QAWESOMEANIMATIONBACKEND_H
