#ifndef QAWESOMEANIMATIONMANAGER_H
#define QAWESOMEANIMATIONMANAGER_H

#include <QObject>
#include <QScopedPointer>
#include <QMutex>
#include <QWaitCondition>
#include <QAtomicInteger>
#include <QTimer>
#include "qawesomeanimationbackend.h"
#include "../qawesomeicon_global.h"

/*
 *  TODO: m_targetSize has to be calculated later
 */

class QThread;

class QAwesomeAnimationManager : public QObject
{
    Q_OBJECT
public:
    explicit QAwesomeAnimationManager(QObject* parent = nullptr);
    ~QAwesomeAnimationManager() override;

    //loaders
    bool loadFromFile(const QString& path, QAwesomeAnimationType type = QAwesomeAnimationType::None);
    bool loadFromData(const QByteArray& data, QAwesomeAnimationType type);
    bool loadSpriteSheet(const QString &imagePath, const QSize &frameSize, int frameCount, int framesPerRow);
    bool loadSvgSequence(const QStringList &svgPaths);

    //controls
    void play(QAwesomeLoopMode loopMode = QAwesomeLoopMode::InfiniteLoop);
    void pause();
    void resume();
    void stop();
    void seek(int frameIndex);

    void setFrameRate(int fps);
signals:
    void frameReady(const QImage& image);
    void frameChanged(int index);
    void finished();
    //error handeling:
    void errorOccurred(const QString& message);

private:
    QScopedPointer<QAwesomeAnimationBackend> m_backend;
    QAtomicInteger<int> m_currentFrame {0};
    QThread* m_workerThread = nullptr;

    int m_fps = 0;
    qreal m_speedFactor = 1.0;

    QAwesomeAnimationState m_state = QAwesomeAnimationState::Stopped;
    QAwesomeLoopMode m_loopMode = QAwesomeLoopMode::InfiniteLoop;
    QAwesomeScaleMode m_scaleMode = QAwesomeScaleMode::KeepAspectRatio;
    QSize m_targetSize ; //TODO

    QTimer m_timer;

private:
    void scheduleNextFrame(int lastDelayMs);

};

#endif // QAWESOMEANIMATIONMANAGER_H
