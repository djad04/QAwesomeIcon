#include "qawesomeutils.h"
#include <QGuiApplication>
#include <QScreen>

namespace QAwesomeUtils {

QSize bestIconLogicalSize()
{
    // Use 32x32 by default
    return QSize(32, 32);
}

QIcon iconFromImage(const QImage& img, const QList<QSize>& sizes, qreal dpr)
{
    QIcon icon;
    QList<QSize> s = sizes;
    if (s.isEmpty()) s << QSize(16,16) << QSize(24,24) << QSize(32,32) << QSize(48,48) << QSize(64,64);
    for (const QSize& sz : s) {
        QImage scaled = img.scaled(sz * dpr, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        scaled.setDevicePixelRatio(dpr);
        icon.addPixmap(QPixmap::fromImage(scaled));
    }
    return icon;
}

} // namespace