#ifndef QAWESOMEICONUPDATER_H
#define QAWESOMEICONUPDATER_H

#include <QObject>
#include <QIcon>
#include <QPointer>
#include <QList>
#include <QSize>
#include "../qawesomeicon_global.h"

class QWidget;
class QWindow;

class QAwesomeIconUpdater : public QObject
{
    Q_OBJECT
public:
    explicit QAwesomeIconUpdater(QObject* parent = nullptr);

    void attachToWidget(QWidget* widget, QAwesomeIconTargets targets);
    void attachToWindow(QWindow* window, QAwesomeIconTargets targets);
    void detach();

    void setIconSizes(const QList<QSize>& sizes);
    void setDpr(qreal dpr);

    QSize targetSize() const;

public slots:
    void onFrameReady(const QImage& image);

private:
    QPointer<QWidget> m_widget;
    QPointer<QWindow> m_window;
    QAwesomeIconTargets m_targets = QAwesomeIconTarget::All;
    QList<QSize> m_sizes;
    qreal m_dpr = 0.0;

    void applyIcon(const QIcon& icon);
};

#endif // QAWESOMEICONUPDATER_H
