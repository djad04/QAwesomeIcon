#include "qawesomeiconupdater.h"
#include <QWidget>
#include <QWindow>
#include <QGuiApplication>
#include <QScreen>


QAwesomeIconUpdater::QAwesomeIconUpdater(QObject* parent)
    : QObject(parent)
{
}

void QAwesomeIconUpdater::attachToWidget(QWidget* widget, QAwesomeIconTargets targets)
{
    m_widget = widget;
    m_window = nullptr;
    m_targets = targets;
}

void QAwesomeIconUpdater::attachToWindow(QWindow* window, QAwesomeIconTargets targets)
{
    m_window = window;
    m_widget = nullptr;
    m_targets = targets;
}

void QAwesomeIconUpdater::detach()
{
    m_widget = nullptr;
    m_window = nullptr;
}

void QAwesomeIconUpdater::setIconSizes(const QList<QSize>& sizes)
{
    m_sizes = sizes;
}

void QAwesomeIconUpdater::setDpr(qreal dpr)
{
    m_dpr = dpr;
}

QSize QAwesomeIconUpdater::targetSize() const
{
    if (!m_sizes.isEmpty()) return m_sizes.first();
    return QSize(32, 32);
}

void QAwesomeIconUpdater::applyIcon(const QIcon& icon)
{
    if (m_widget) {
        m_widget->setWindowIcon(icon);
    }
    if (m_window) {
        m_window->setIcon(icon);
    }
    // TODO: Platform-specific taskbar icon updates or overlays
}

void QAwesomeIconUpdater::onFrameReady(const QImage& image)
{
    if (image.isNull()) return;

    QList<QSize> sizes = m_sizes;
    if (sizes.isEmpty()) {
        sizes << QSize(16,16) << QSize(24,24) << QSize(32,32) << QSize(48,48) << QSize(64,64) << QSize(128,128);
    }

    qreal dpr = m_dpr;
    if (dpr <= 0.0) {
        QScreen* scr = nullptr;
        if (m_widget && m_widget->windowHandle()) scr = m_widget->windowHandle()->screen();
        if (!scr && m_window) scr = m_window->screen();
        if (!scr) scr = QGuiApplication::primaryScreen();
        dpr = scr ? scr->devicePixelRatio() : qApp->devicePixelRatio();
        if (dpr <= 0.0) dpr = 1.0;
    }

    QIcon icon;
    for (const QSize& s : sizes) {
        QImage scaled = image.scaled(s * dpr, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        scaled.setDevicePixelRatio(dpr);
        icon.addPixmap(QPixmap::fromImage(scaled));
    }

    applyIcon(icon);
}
