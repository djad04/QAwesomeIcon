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


QAwesomeIcon::QAwesomeIcon(QObject *parent)
{

}



QAwesomeIcon::~QAwesomeIcon()
{

}

