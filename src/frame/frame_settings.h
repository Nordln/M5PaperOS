#ifndef _FRAME_SETTINGS_H_
#define _FRAME_SETTINGS_H_

#include "frame_base.h"
#include "../epdgui/epdgui.h"

class Frame_Settings : public Frame_Base {
public:
    Frame_Settings();
    ~Frame_Settings();
    int init(epdgui_args_vector_t &args);


private:
    EPDGUI_Button *_key_wifi;
    EPDGUI_Button *_key_wallpaper;
    EPDGUI_Button *_key_shutdown;
    EPDGUI_Button *_key_restart;
    EPDGUI_Button *_key_syncntp;

    EPDGUI_Button *key_timezone_plus;
    EPDGUI_Button *key_timezone_reset;
    EPDGUI_Button *key_timezone_minus;
    int _timezone;
    M5EPD_Canvas *_timezone_canvas;
};

#endif //_FRAME_SETTINGS_H_