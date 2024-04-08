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
