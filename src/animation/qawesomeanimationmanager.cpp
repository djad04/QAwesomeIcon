#include "qawesomeanimationmanager.h"
#include "backends/qawesomegifbackend.h"
#include "backends/qawesomespritebackend.h"
#include "backends/qawesomesvgbackend.h"


QAwesomeAnimationManager::QAwesomeAnimationManager(QObject *parent)
{

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



