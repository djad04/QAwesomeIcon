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

class QThread;

class QAwesomeAnimationManager : public QObject
{
    Q_OBJECT
public:
    explicit QAwesomeAnimationManager(QObject* parent = nullptr);
    ~QAwesomeAnimationManager() override;

    // Loaders
    bool loadFromFile(const QString& path, QAwesomeAnimationType type = QAwesomeAnimationType::None);
    bool loadFromData(const QByteArray& data, QAwesomeAnimationType type);
    bool loadSpriteSheet(const QString& imagePath, const QSize& frameSize, int frameCount, int framesPerRow = 0);
    bool loadSvgSequence(const QStringList& svgPaths);

    // Controls
    void play(QAwesomeLoopMode loopMode = QAwesomeLoopMode::InfiniteLoop);
    void pause();
    void resume();
    void stop();
    void seek(int frameIndex);

    void setFrameRate(int fps); // 0 to use backend delays
    void setSpeedFactor(qreal factor);
    void setScaleMode(QAwesomeScaleMode mode);
    void setPriority(QAwesomeAnimationPriority priority);

    // Info
    int frameCount() const;
    int currentFrame() const;
    QSize frameSize() const;
    QAwesomeAnimationState state() const;
    qreal speedFactor() const { return m_speedFactor; }
    int frameRate() const { return m_fps; }

signals:
    void frameReady(const QImage& image); // Emitted from worker via queued connection
    void frameChanged(int index);
    void finished();
    void errorOccurred(const QString& message);

private slots:
    void onWorkerFrame(const QImage& image, int index);
    void onWorkerError(const QString& message);

private:
    void ensureWorker();
    void destroyWorker();
    void scheduleNextFrame(int lastDelayMs);

    QScopedPointer<QAwesomeAnimationBackend> m_backend;
    QThread* m_workerThread = nullptr;

    int m_fps = 0;
    qreal m_speedFactor = 1.0;
    QAwesomeScaleMode m_scaleMode = QAwesomeScaleMode::KeepAspectRatio;
    QAwesomeLoopMode m_loopMode = QAwesomeLoopMode::InfiniteLoop;

    QAtomicInteger<int> m_currentFrame {0};
    QAwesomeAnimationState m_state = QAwesomeAnimationState::Stopped;

    QTimer m_timer; // Main-thread scheduler

    QSize m_targetSize; // computed from icon updater later

public:
    void requestTargetSize(const QSize& size) { m_targetSize = size; }
};

#endif // QAWESOMEANIMATIONMANAGER_H