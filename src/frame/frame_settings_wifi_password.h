#ifndef _FRAME_SETTINGS_WIFI_PASSWORD_H_
#define _FRAME_SETTINGS_WIFI_PASSWORD_H_

#include "frame_base.h"
#include "../epdgui/epdgui.h"

class Frame_Settings_Wifi_Password : public Frame_Base {
public:
    Frame_Settings_Wifi_Password();
    ~Frame_Settings_Wifi_Password();
    int run();
    int init(epdgui_args_vector_t &args);

private:
    EPDGUI_Textbox *inputbox;
    EPDGUI_Keyboard *keyboard;
    EPDGUI_Button *key_textclear;
};

#endif //_FRAME_SETTINGS_WIFI_PASSWORD_H_