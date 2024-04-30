#include "qawesomeicon.h"
#include "animation/qawesomeanimationmanager.h"
#include "icon/qawesomeiconupdater.h"
#include "utils/qawesomeutils.h"

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

void QAwesomeIcon::attachToWidget(QWidget* widget, QAwesomeIconTargets targets) {
    d->targetWidget = widget;
    d->targetWindow = nullptr;
    d->targets = targets;
    d->updater.attachToWidget(widget, targets);

    // Update target size based on widget's icon requirements
    QSize targetSize = d->updater.targetSize();
    if (!targetSize.isEmpty()) {
        d->manager.requestTargetSize(targetSize);
    }
}

void QAwesomeIcon::attachToWindow(QWindow* window, QAwesomeIconTargets targets) {
    d->targetWindow = window;
    d->targetWidget = nullptr;
    d->targets = targets;
    d->updater.attachToWindow(window, targets);

    // Update target size based on window's icon requirements
    QSize targetSize = d->updater.targetSize();
    if (!targetSize.isEmpty()) {
        d->manager.requestTargetSize(targetSize);
    }
}

void QAwesomeIcon::detach() {
    d->updater.detach();
    d->targetWidget = nullptr;
    d->targetWindow = nullptr;
}

void QAwesomeIcon::play(QAwesomeLoopMode loopMode) {
    d->manager.play(loopMode);
}

void QAwesomeIcon::pause() {
    d->manager.pause();
}

void QAwesomeIcon::resume() {
    d->manager.resume();
}

void QAwesomeIcon::stop() {
    d->manager.stop();
}

void QAwesomeIcon::seek(int frameIndex) {
    d->manager.seek(frameIndex);
}

void QAwesomeIcon::setFrameRate(int fps) {
    d->manager.setFrameRate(fps);
}

void QAwesomeIcon::setSpeedFactor(qreal factor) {
    d->manager.setSpeedFactor(factor);
}

void QAwesomeIcon::setScaleMode(QAwesomeScaleMode mode) {
    d->manager.setScaleMode(mode);
}

void QAwesomeIcon::setIconSizes(const QList<QSize>& sizes) {
    d->updater.setIconSizes(sizes);
    // Update target size for rendering
    QSize targetSize = d->updater.targetSize();
    if (!targetSize.isEmpty()) {
        d->manager.requestTargetSize(targetSize);
    }
}
void QAwesomeIcon::setDpr(qreal dpr) {
    d->updater.setDpr(dpr);
}

void QAwesomeIcon::setPriority(QAwesomeAnimationPriority priority) {
    d->manager.setPriority(priority);
}


int QAwesomeIcon::frameCount() const {
    return d->manager.frameCount();
}

int QAwesomeIcon::currentFrame() const {
    return d->manager.currentFrame();
}

QSize QAwesomeIcon::frameSize() const {
    return d->manager.frameSize();
}


QAwesomeAnimationState QAwesomeIcon::state() const {
    return d->manager.state();
}

qreal QAwesomeIcon::speedFactor() const {
    return d->manager.speedFactor();
}

int QAwesomeIcon::frameRate() const {
    return d->manager.frameRate();
}


#include "qawesomeicon.moc"
