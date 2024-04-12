#ifndef QAWESOMEANIMATIONMANAGER_H
#define QAWESOMEANIMATIONMANAGER_H

#include <QObject>
#include <QScopedPointer>
#include <QMutex>
#include <QWaitCondition>
#include <QAtomicInteger>
#include <QTimer>
#include "qawesomeanimationbackend.h"
#include "../qawesomeicon_global.h"

class QThread;

class QAwesomeAnimationManager : public QObject
{
    Q_OBJECT
public:
    explicit QAwesomeAnimationManager(QObject* parent = nullptr);
    ~QAwesomeAnimationManager() override;

    //loaders
    bool loadFromFile(const QString& path, QAwesomeAnimationType type = QAwesomeAnimationType::None);
    bool loadFromData(const QByteArray& data, QAwesomeAnimationType type);

signals:
    //error handeling:
    void errorOccurred(const QString& message);

private:
    QScopedPointer<QAwesomeAnimationBackend> m_backend;
     QAtomicInteger<int> m_currentFrame {0};

};

#endif // QAWESOMEANIMATIONMANAGER_H
