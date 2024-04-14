#include "qawesomeanimationmanager.h"
#include "backends/qawesomegifbackend.h"
#include "backends/qawesomespritebackend.h"
#include "backends/qawesomesvgbackend.h"


QAwesomeAnimationManager::QAwesomeAnimationManager(QObject *parent)
{
    m_timer.setTimerType(Qt::PreciseTimer);
    m_timer.setSingleShot(true);
    connect(&m_timer, &QTimer::timeout, this, [this]() {



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



void QAwesomeAnimationManager::scheduleNextFrame(int lastDelayMs)
{
    qDebug() << "QAwesomeAnimationManager: Scheduling next frame in" << lastDelayMs << "ms";
    if (lastDelayMs <= 0) lastDelayMs = 0;
    m_timer.start(lastDelayMs);
}

