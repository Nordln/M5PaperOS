#ifndef _FRAME_WALLPAPER_H_
#define _FRAME_WALLPAPER_H_

#include "frame_base.h"
#include "../epdgui/epdgui.h"

class Frame_Settings_Wallpaper : public Frame_Base {
public:
    Frame_Settings_Wallpaper();
    ~Frame_Settings_Wallpaper();
    int init(epdgui_args_vector_t &args);

private:
    EPDGUI_Switch* _sw_wallpapers[WALLPAPER_NUM];
    EPDGUI_MutexSwitch* _sw_mutex_group;
};

#endif //_FRAME_WALLPAPER_H_