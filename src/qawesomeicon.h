#ifndef QAWESOMEICON_H
#define QAWESOMEICON_H

#include "qawesomeicon_global.h"

#include <QObject>
#include <QIcon>
#include <QPointer>
#include <QImage>
#include <QSize>
#include <QByteArray>
#include <QUrl>

class QWidget;
class QWindow;

class QAWESOMEICON_EXPORT QAwesomeIcon : public QObject
{
    Q_OBJECT
public:
    explicit QAwesomeIcon(QObject* parent = nullptr);
    ~QAwesomeIcon() override;

};

#endif // QAWESOMEICON_H
