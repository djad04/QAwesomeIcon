#ifndef QAWESOMEANIMATIONWORKER_H
#define QAWESOMEANIMATIONWORKER_H

#include <QObject>
#include <QImage>
#include "../animation/qawesomeanimationbackend.h"

class QAwesomeAnimationWorker : public QObject
{
    Q_OBJECT
public:
    explicit QAwesomeAnimationWorker(QObject* parent = nullptr) : QObject(parent) {}

signals:
    void frameProduced(const QImage& image, int index);
    void error(const QString& message);

public slots:
    void renderFrame(QAwesomeAnimationBackend* backend, int index, const QSize& size, qreal dpr, QAwesomeScaleMode scaleMode)
    {
        if (!backend) {
            emit error(QStringLiteral("Invalid backend"));
            return;
        }
        QImage img = backend->renderFrame(index, size, dpr, scaleMode);
        emit frameProduced(img, index);
    }
};

#endif // QAWESOMEANIMATIONWORKER_H