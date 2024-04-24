#ifndef QAWESOMEUTILS_H
#define QAWESOMEUTILS_H

#include <QSize>
#include <QImage>
#include <QIcon>

namespace QAwesomeUtils {
QSize bestIconLogicalSize();
QIcon iconFromImage(const QImage& img, const QList<QSize>& sizes, qreal dpr);
}

#endif // QAWESOMEUTILS_H