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

private:
    QPointer<QWidget> m_widget;
    QPointer<QWindow> m_window;

};

#endif // QAWESOMEICONUPDATER_H
