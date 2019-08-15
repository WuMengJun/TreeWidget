#ifndef WLVIDEOPLAYER_H
#define WLVIDEOPLAYER_H

#include "wlvideoplayer_global.h"
#include <QWidget>

class WLVIDEOPLAYERSHARED_EXPORT WLVideoPlayer
{
public:
    explicit WLVideoPlayer(QWidget* parent = NULL);
    void startPlay(const QString& filePath);
    void setVolume(int volume);
    void setVideoTitle(const QString title);
    bool pause();
    QWidget* videoPlayer;
    void initStyle();
    void setContextMenuEnabled(bool value);
};

#endif // WLVIDEOPLAYER_H
