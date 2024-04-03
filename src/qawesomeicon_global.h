#ifndef QAWESOMEICON_GLOBAL_H
#define QAWESOMEICON_GLOBAL_H

#include <QtCore/qglobal.h>
#include <QtCore/qmetatype.h>


#if defined(QAWESOMEICON_LIBRARY)
#  define QAWESOMEICON_EXPORT Q_DECL_EXPORT
#else
#  define QAWESOMEICON_EXPORT Q_DECL_IMPORT
#endif

// Animation types supported
enum class QAwesomeAnimationType {
    None = 0,
    GIF,
    SpriteSheet,
    SVGSequence,
    SVGMorph,
    Lottie  // Future implementation
};

// Animation states
enum class QAwesomeAnimationState {
    Stopped = 0,
    Playing,
    Paused,
    Finished,
    Error
};

// Loop modes
enum class QAwesomeLoopMode {
    NoLoop = 0,
    InfiniteLoop,
    FiniteLoop,
    PingPong
};

// Animation scaling modes
enum class QAwesomeScaleMode {
    KeepAspectRatio = 0,
    IgnoreAspectRatio,
    KeepAspectRatioByExpanding
};

// Priority levels for animations
enum class QAwesomeAnimationPriority {
    Low = 0,
    Normal,
    High,
    Critical
};

// Platform-specific icon types
enum class QAwesomeIconTarget {
    WindowIcon = 0x01,
    TaskbarIcon = 0x02,
    SystemTrayIcon = 0x04,
    All = WindowIcon | TaskbarIcon | SystemTrayIcon
};

Q_DECLARE_FLAGS(QAwesomeIconTargets, QAwesomeIconTarget)
Q_DECLARE_OPERATORS_FOR_FLAGS(QAwesomeIconTargets)

// Declare metatypes for enums used in Qt signals/slots
Q_DECLARE_METATYPE(QAwesomeScaleMode)
Q_DECLARE_METATYPE(QAwesomeAnimationType)
Q_DECLARE_METATYPE(QAwesomeAnimationState)
Q_DECLARE_METATYPE(QAwesomeLoopMode)
Q_DECLARE_METATYPE(QAwesomeAnimationPriority)
Q_DECLARE_METATYPE(QAwesomeIconTarget)

#ifndef QT_NO_DATASTREAM
// Provide QDataStream operators for QAwesomeScaleMode to satisfy moc streaming
inline QDataStream& operator<<(QDataStream& out, QAwesomeScaleMode mode)
{
    out << static_cast<qint32>(static_cast<int>(mode));
    return out;
}

inline QDataStream& operator>>(QDataStream& in, QAwesomeScaleMode& mode)
{
    qint32 v = 0;
    in >> v;
    mode = static_cast<QAwesomeScaleMode>(static_cast<int>(v));
    return in;
}
#endif

// Version information
#define QAWESOMEICON_VERSION_MAJOR 1
#define QAWESOMEICON_VERSION_MINOR 0
#define QAWESOMEICON_VERSION_PATCH 0
#define QAWESOMEICON_VERSION_STR "1.0.0"

#endif // QAWESOMEICON_GLOBAL_H
