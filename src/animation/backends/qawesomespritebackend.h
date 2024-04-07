#ifndef QAWESOMESPRITEBACKEND_H
#define QAWESOMESPRITEBACKEND_H

#include "../qawesomeanimationbackend.h"
#include <QImage>

class QAwesomeSpriteBackend : public QAwesomeAnimationBackend
{
    Q_OBJECT
public:
    explicit QAwesomeSpriteBackend(QObject* parent = nullptr);



    int frameCount() const override { return m_frames.size(); }
    QSize frameSize() const override { return m_frameSize; }
    int defaultFrameDelayMs(int) const override { return 33; }


    bool isValid() const override { return !m_frames.isEmpty(); }

private:
    QVector<QImage> m_frames;
    QSize m_frameSize;

};

#endif // QAWESOMESPRITEBACKEND_H
