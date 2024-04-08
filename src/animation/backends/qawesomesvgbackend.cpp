#include "qawesomesvgbackend.h"
#include <QPainter>
#include <QFile>

QAwesomeSvgBackend::QAwesomeSvgBackend(QObject* parent)
    : QAwesomeAnimationBackend(parent)
{
}

bool QAwesomeSvgBackend::loadFromFile(const QString& path)
{
    m_renderers.clear();
    m_single.reset(new QSvgRenderer(path));
    if (m_single.isNull() || !m_single->isValid()) return false;
    m_defaultSize = m_single->defaultSize();
    return true;
}

bool QAwesomeSvgBackend::loadFromData(const QByteArray& data)
{
    m_renderers.clear();
    m_single.reset(new QSvgRenderer(data));
    if (m_single.isNull() || !m_single->isValid()) return false;
    m_defaultSize = m_single->defaultSize();
    return true;
}

bool QAwesomeSvgBackend::loadSvgSequence(const QStringList& svgPaths)
{
    m_renderers.clear();
    for (const QString& p : svgPaths) {
        QSharedPointer<QSvgRenderer> r(new QSvgRenderer(p));
        if (!r->isValid()) return false;
        m_renderers.push_back(r);
    }
    if (!m_renderers.isEmpty()) m_defaultSize = m_renderers.first()->defaultSize();
    return !m_renderers.isEmpty();
}

QImage QAwesomeSvgBackend::renderSvg(QSvgRenderer* renderer, const QSize& targetLogicalSize, qreal dpr, QAwesomeScaleMode scaleMode)
{
    QSize logical = targetLogicalSize.isEmpty() ? renderer->defaultSize() : targetLogicalSize;
    QSize pixel = logical * dpr;

    QImage img(pixel, QImage::Format_ARGB32_Premultiplied);
    img.setDevicePixelRatio(dpr);
    img.fill(Qt::transparent);
    QPainter p(&img);
    QRectF target(QPointF(0, 0), logical);
    if (scaleMode == QAwesomeScaleMode::KeepAspectRatioByExpanding) {
        QSizeF s = renderer->defaultSize();
        target = QRectF(QPointF(0, 0), logical);
    }
    renderer->render(&p, target);
    p.end();
    return img;
}
