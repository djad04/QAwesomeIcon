#ifndef QAWESOMEICON_H
#define QAWESOMEICON_H

#include "qawesomeicon_global.h"


#include <QObject>
#include <QIcon>
#include <QPointer>
#include <QImage>
#include <QSize>
#include <QByteArray>
#include <QUrl>

class QWidget;
class QWindow;

class QAWESOMEICON_EXPORT QAwesomeIcon : public QObject
{
    Q_OBJECT
public:
    explicit QAwesomeIcon(QObject* parent = nullptr);
    ~QAwesomeIcon() override;
    // Loading
    bool loadFromFile(const QString& path, QAwesomeAnimationType type = QAwesomeAnimationType::None);
    bool loadFromData(const QByteArray& data, QAwesomeAnimationType type);
    bool loadSpriteSheet(const QString& imagePath, const QSize& frameSize, int frameCount, int framesPerRow = 0);
    bool loadSvgSequence(const QStringList& svgPaths);
    // Targets
    void attachToWidget(QWidget* widget, QAwesomeIconTargets targets = QAwesomeIconTarget::All);
    void attachToWindow(QWindow* window, QAwesomeIconTargets targets = QAwesomeIconTarget::All);
    void detach();

    // Control
    void play(QAwesomeLoopMode loopMode = QAwesomeLoopMode::InfiniteLoop);
    void pause();
    void resume();
    void stop();
    void seek(int frameIndex);

    // Settings
    void setFrameRate(int fps);
    void setSpeedFactor(qreal factor); // 1.0 normal
    void setScaleMode(QAwesomeScaleMode mode);
    void setIconSizes(const QList<QSize>& sizes); // for high DPI: 16, 24, 32, 48, 64, ...
    void setDpr(qreal dpr);
    void setPriority(QAwesomeAnimationPriority priority);

signals:
    void frameChanged(int index);
    void finished();
    void errorOccurred(const QString& message);

private:
    class Impl;
    Impl* d;
};

#endif // QAWESOMEICON_H
