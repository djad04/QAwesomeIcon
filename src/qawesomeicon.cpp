#include "qawesomeicon.h"
#include "animation/qawesomeanimationmanager.h"
#include "icon/qawesomeiconupdater.h"
#include "utils/qawesomeutils.h"
#include <qguiapplication.h>
#include <QWidget>
#include <QWindow>
#include <QGuiApplication>
#include <QScreen>

class QAwesomeIcon::Impl : public QObject {
    Q_OBJECT
public:
    QAwesomeAnimationManager manager;
    QAwesomeIconUpdater updater;
    QPointer<QWidget> targetWidget;
    QPointer<QWindow> targetWindow;
    QAwesomeIconTargets targets = QAwesomeIconTarget::All;

    Impl()
    {
        QObject::connect(&manager, &QAwesomeAnimationManager::frameReady, &updater, &QAwesomeIconUpdater::onFrameReady, Qt::QueuedConnection);
        QObject::connect(&manager, &QAwesomeAnimationManager::frameChanged, qApp, [this](int index){ emit q->frameChanged(index); }, Qt::QueuedConnection);
        QObject::connect(&manager, &QAwesomeAnimationManager::finished, qApp, [this](){ emit q->finished(); }, Qt::QueuedConnection);
        QObject::connect(&manager, &QAwesomeAnimationManager::errorOccurred, qApp, [this](const QString& m){ emit q->errorOccurred(m); }, Qt::QueuedConnection);

        // Initialize target size for animation rendering
        manager.requestTargetSize(QSize(32, 32)); // Default icon size
    }

    QAwesomeIcon* q = nullptr;
};

QAwesomeIcon::QAwesomeIcon(QObject* parent) : QObject(parent), d(new Impl) {
    d->q = this;
}

QAwesomeIcon::~QAwesomeIcon() { delete d; }


bool QAwesomeIcon::loadFromFile(const QString& path, QAwesomeAnimationType type) {
    return d->manager.loadFromFile(path, type);
}

bool QAwesomeIcon::loadFromData(const QByteArray& data, QAwesomeAnimationType type) {
    return d->manager.loadFromData(data, type);
}

bool QAwesomeIcon::loadSpriteSheet(const QString& imagePath, const QSize& frameSize, int frameCount, int framesPerRow) {
    return d->manager.loadSpriteSheet(imagePath, frameSize, frameCount, framesPerRow);
}

bool QAwesomeIcon::loadSvgSequence(const QStringList& svgPaths) {
    return d->manager.loadSvgSequence(svgPaths);
}
