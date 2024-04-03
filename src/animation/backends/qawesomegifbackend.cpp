#include "qawesomegifbackend.h"
#include <QFile>
#include <QDebug>

QAwesomeGifBackend::QAwesomeGifBackend(QObject* parent)
    : QAwesomeAnimationBackend(parent)
{
}

bool QAwesomeGifBackend::loadFromFile(const QString& path)
{
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly)) return false;
    return loadWithImageReader(&f);
}

bool QAwesomeGifBackend::loadFromData(const QByteArray& data)
{
    QBuffer buf;
    buf.setData(data);
    buf.open(QIODevice::ReadOnly);
    return loadWithImageReader(&buf);
}
