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
