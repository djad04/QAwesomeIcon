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
    // Loading
    bool loadFromFile(const QString& path, QAwesomeAnimationType type = QAwesomeAnimationType::None);
    bool loadFromData(const QByteArray& data, QAwesomeAnimationType type);
    bool loadSpriteSheet(const QString& imagePath, const QSize& frameSize, int frameCount, int framesPerRow = 0);
    bool loadSvgSequence(const QStringList& svgPaths);
    // Targets
    void attachToWidget(QWidget* widget, QAwesomeIconTargets targets = QAwesomeIconTarget::All);
    void attachToWindow(QWindow* window, QAwesomeIconTargets targets = QAwesomeIconTarget::All);
    void detach();

signals:
    void frameChanged(int index);
    void finished();
    void errorOccurred(const QString& message);

private:
    class Impl;
    Impl* d;
};

#endif // QAWESOMEICON_H
