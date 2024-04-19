#include "qawesomeanimationmanager.h"
#include "backends/qawesomegifbackend.h"
#include "backends/qawesomespritebackend.h"
#include "backends/qawesomesvgbackend.h"



QAwesomeAnimationManager::QAwesomeAnimationManager(QObject* parent)
    : QObject(parent)
{
    m_timer.setTimerType(Qt::PreciseTimer);
    m_timer.setSingleShot(true);
    connect(&m_timer, &QTimer::timeout, this, [this]() {
        qDebug() << "QAwesomeAnimationManager: Timer timeout triggered";
        if (!m_backend || m_state != QAwesomeAnimationState::Playing) {
            qDebug() << "QAwesomeAnimationManager: Skipping - backend:" << (m_backend ? "exists" : "null")
            << "state:" << (int)m_state;
            return;
        }

        int idx = m_currentFrame.loadRelaxed();
        int frameCount = m_backend->frameCount();
        qDebug() << "QAwesomeAnimationManager: Processing frame" << idx << "of" << frameCount;

        // Handle frame bounds
        if (idx >= frameCount) {
            qDebug() << "QAwesomeAnimationManager: Frame overflow, handling loop mode:" << (int)m_loopMode;
            if (m_loopMode == QAwesomeLoopMode::InfiniteLoop || m_loopMode == QAwesomeLoopMode::FiniteLoop) {
                idx = 0;
            } else if (m_loopMode == QAwesomeLoopMode::NoLoop) {
                m_state = QAwesomeAnimationState::Finished;
                emit finished();
                return;
            } else if (m_loopMode == QAwesomeLoopMode::PingPong) {
                idx = 0; // Simple wrap for now
            }
        }

        qDebug() << "QAwesomeAnimationManager: Rendering frame" << idx << "with target size" << m_targetSize;
        QImage img = m_backend->renderFrame(idx, m_targetSize, 1.0, m_scaleMode);
        qDebug() << "QAwesomeAnimationManager: Rendered image size:" << img.size() << "isNull:" << img.isNull();

        emit frameReady(img);
        emit frameChanged(idx);

        int delay = m_fps > 0 ? int(1000.0 / m_fps) : m_backend->defaultFrameDelayMs(idx);
        if (delay <= 0) delay = 33; // ~30fps fallback
        delay = int(delay / m_speedFactor);
        int effectiveFps = delay > 0 ? int(1000.0 / delay) : 0;
        qDebug() << "QAwesomeAnimationManager: Frame delay:" << delay << "ms (effective fps:" << effectiveFps << ", speed:" << m_speedFactor << ")";

        // Advance to next frame
        int nextFrame = idx + 1;
        if (nextFrame >= frameCount) {
            if (m_loopMode == QAwesomeLoopMode::InfiniteLoop || m_loopMode == QAwesomeLoopMode::FiniteLoop) {
                nextFrame = 0;
            } else if (m_loopMode == QAwesomeLoopMode::NoLoop) {
                // Don't advance, will finish on next timeout
                nextFrame = frameCount;
            }
        }
        m_currentFrame = nextFrame;

        scheduleNextFrame(delay);
    });
}



QAwesomeAnimationManager::~QAwesomeAnimationManager()
{

}



bool QAwesomeAnimationManager::loadFromFile(const QString& path, QAwesomeAnimationType type)
{
    qDebug() << "QAwesomeAnimationManager: Loading file:" << path << "type:" << (int)type;
    QScopedPointer<QAwesomeAnimationBackend> backend;
    if (type == QAwesomeAnimationType::GIF || (type == QAwesomeAnimationType::None && path.endsWith(".gif", Qt::CaseInsensitive))) {
        backend.reset(new QAwesomeGifBackend);
    } else if (type == QAwesomeAnimationType::SpriteSheet) {
        backend.reset(new QAwesomeSpriteBackend);
    } else if (type == QAwesomeAnimationType::SVGSequence || path.endsWith(".svg", Qt::CaseInsensitive)) {
        backend.reset(new QAwesomeSvgBackend);
    } else {
        emit errorOccurred(QStringLiteral("unsupported format or type for file: %1").arg(path));
        return false;
    }
    if (!backend->loadFromFile(path)) {
        emit errorOccurred(QStringLiteral("Failed to load animation: %1").arg(path));
        return false;
    }
    m_backend.swap(backend);
    m_currentFrame = 0;
    qDebug() << "QAwesomeAnimationManager: Loaded successfully , frame count:" << m_backend->frameCount()
             << "frame size:" << m_backend->frameSize();
    return true;
}


bool QAwesomeAnimationManager::loadFromData(const QByteArray& data, QAwesomeAnimationType type)
{
    QScopedPointer<QAwesomeAnimationBackend> backend;
    if (type == QAwesomeAnimationType::GIF) {
        backend.reset(new QAwesomeGifBackend);
    } else if (type == QAwesomeAnimationType::SpriteSheet) {
        backend.reset(new QAwesomeSpriteBackend);
    } else if (type == QAwesomeAnimationType::SVGSequence) {
        backend.reset(new QAwesomeSvgBackend);
    } else {
        emit errorOccurred(QStringLiteral("Unsupported data type"));
        return false;
    }

    if (!backend->loadFromData(data)) {
        emit errorOccurred(QStringLiteral("Failed to load animation from data"));
        return false;
    }
    m_backend.swap(backend);
    m_currentFrame = 0;
    return true;
}

bool QAwesomeAnimationManager::loadSpriteSheet(const QString& imagePath, const QSize& frameSize, int frameCount, int framesPerRow)
{
    QScopedPointer<QAwesomeSpriteBackend> backend(new QAwesomeSpriteBackend);
    if (!backend->loadSpriteSheet(imagePath, frameSize, frameCount, framesPerRow)) {
        emit errorOccurred(QStringLiteral("Failed to load sprite sheet"));
        return false;
    }
    m_backend.reset(backend.take());
    m_currentFrame = 0;
    return true;
}

bool QAwesomeAnimationManager::loadSvgSequence(const QStringList& svgPaths)
{
    QScopedPointer<QAwesomeSvgBackend> backend(new QAwesomeSvgBackend);
    if (!backend->loadSvgSequence(svgPaths)) {
        emit errorOccurred(QStringLiteral("Failed to load SVG sequence"));
        return false;
    }
    m_backend.reset(backend.take());
    m_currentFrame = 0;
    return true;
}


void QAwesomeAnimationManager::play(QAwesomeLoopMode loopMode)
{
    qDebug() << "QAwesomeAnimationManager: Play called with loop mode:" << (int)loopMode;
    if (!m_backend) {
        qDebug() << "QAwesomeAnimationManager: No backend loaded!";
        emit errorOccurred(QStringLiteral("No backend loaded"));
        return;
    }
    qDebug() << "QAwesomeAnimationManager: Backend exists, frame count:" << m_backend->frameCount()
             << "target size:" << m_targetSize;
    m_loopMode = loopMode;
    m_state = QAwesomeAnimationState::Playing;
    m_currentFrame = 0;
    scheduleNextFrame(0);
}

void QAwesomeAnimationManager::pause()
{
    qDebug() << "QAwesomeAnimationManager: Pause called";
    if (m_state == QAwesomeAnimationState::Playing) {
        m_state = QAwesomeAnimationState::Paused;
        m_timer.stop();
    }
}

void QAwesomeAnimationManager::resume()
{
    qDebug() << "QAwesomeAnimationManager: Resume called";
    if (m_state == QAwesomeAnimationState::Paused) {
        m_state = QAwesomeAnimationState::Playing;
        scheduleNextFrame(0);
    }
}

void QAwesomeAnimationManager::stop()
{
    qDebug() << "QAwesomeAnimationManager: Stop called";
    m_state = QAwesomeAnimationState::Stopped;
    m_timer.stop();
    m_currentFrame = 0;
}
void QAwesomeAnimationManager::seek(int frameIndex)
{
    if (!m_backend) return;
    if (frameIndex < 0) frameIndex = 0;
    if (frameIndex >= m_backend->frameCount()) frameIndex = m_backend->frameCount() - 1;
    m_currentFrame = frameIndex;
}


void QAwesomeAnimationManager::setFrameRate(int fps)
{
    qDebug() << "QAwesomeAnimationManager: Setting frame rate to:" << fps;
    m_fps = fps;
}

void QAwesomeAnimationManager::setSpeedFactor(qreal factor)
{
    if (factor <= 0) factor = 0.1;
    m_speedFactor = factor;
    if (m_backend)
        m_backend->setSpeedFactor(factor);
}

void QAwesomeAnimationManager::setScaleMode(QAwesomeScaleMode mode)
{
    m_scaleMode = mode;
}

void QAwesomeAnimationManager::setPriority(QAwesomeAnimationPriority priority)
{
    Q_UNUSED(priority)
}


int QAwesomeAnimationManager::frameCount() const
{
    return m_backend ? m_backend->frameCount() : 0;
}

int QAwesomeAnimationManager::currentFrame() const
{
    return m_currentFrame.loadRelaxed();
}

QSize QAwesomeAnimationManager::frameSize() const
{
    return m_backend ? m_backend->frameSize() : QSize();
}

QAwesomeAnimationState QAwesomeAnimationManager::state() const
{
    return m_state;
}


void QAwesomeAnimationManager::scheduleNextFrame(int lastDelayMs)
{
    qDebug() << "QAwesomeAnimationManager: Scheduling next frame in" << lastDelayMs << "ms";
    if (lastDelayMs <= 0) lastDelayMs = 0;
    m_timer.start(lastDelayMs);
}

