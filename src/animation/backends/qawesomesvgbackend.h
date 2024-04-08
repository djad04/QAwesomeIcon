#ifndef QAWESOMESVGBACKEND_H
#define QAWESOMESVGBACKEND_H

#include "../qawesomeanimationbackend.h"
#include <QSvgRenderer>
#include <QSharedPointer>
#include <QVector>

class QAwesomeSvgBackend : public QAwesomeAnimationBackend
{
    Q_OBJECT
public:
    explicit QAwesomeSvgBackend(QObject* parent = nullptr);

    bool loadFromFile(const QString& path) override;
    bool loadFromData(const QByteArray& data) override;

    bool loadSvgSequence(const QStringList& svgPaths);

    int frameCount() const override { return qMax(1, m_renderers.size()); }
    QSize frameSize() const override { return m_defaultSize; }
    int defaultFrameDelayMs(int) const override { return 33; }



    bool isValid() const override { return !m_renderers.isEmpty() || (!m_single.isNull() && m_single->isValid()); }

private:
    QVector<QSharedPointer<QSvgRenderer>> m_renderers;
    QSharedPointer<QSvgRenderer> m_single;
    QSize m_defaultSize;

    QImage renderSvg(QSvgRenderer* renderer, const QSize& targetLogicalSize, qreal dpr, QAwesomeScaleMode scaleMode);
};

#endif // QAWESOMESVGBACKEND_H
