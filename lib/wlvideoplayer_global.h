#ifndef WLVIDEOPLAYER_GLOBAL_H
#define WLVIDEOPLAYER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(WLVIDEOPLAYER_LIBRARY)
#  define WLVIDEOPLAYERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define WLVIDEOPLAYERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // WLVIDEOPLAYER_GLOBAL_H
