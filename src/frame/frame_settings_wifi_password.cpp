#include "frame_settings_wifi_password.h"


void key_passwordclear_cb(epdgui_args_vector_t &args) {
    ((EPDGUI_Textbox*)(args[0]))->SetText("");
}

Frame_Settings_Wifi_Password::Frame_Settings_Wifi_Password() : Frame_Base() {
    _frame_name = "Frame_Settings_Wifi_Password";
    const uint16_t kKeyBaseY = 176;
    inputbox = new EPDGUI_Textbox(4, 100, 532, 60);
    key_textclear = new EPDGUI_Button("CLR", 4, kKeyBaseY, 260, 52);
    
    inputbox->SetTextMargin(8, 15, 8, 8);
    inputbox->SetState(EPDGUI_Textbox::EVENT_PRESSED);

    keyboard = new EPDGUI_Keyboard(EPDGUI_Keyboard::STYLE_INPUTMODE_NEEDCONFIRM);
    
    key_textclear->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void*)inputbox);
    key_textclear->Bind(EPDGUI_Button::EVENT_RELEASED, key_passwordclear_cb);

    exitbtn("Wi-Fi");
    _canvas_title->drawString("Password", 270, 34);
    
    _key_exit->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void*)(&_is_run));
    _key_exit->Bind(EPDGUI_Button::EVENT_RELEASED, &Frame_Base::exit_cb);
}

Frame_Settings_Wifi_Password::~Frame_Settings_Wifi_Password() {
    delete inputbox;
    delete keyboard;
    delete key_textclear;
}

int Frame_Settings_Wifi_Password::init(epdgui_args_vector_t &args) {
    _is_run = 1;
    M5.EPD.Clear();
    _canvas_title->pushCanvas(0, 8, UPDATE_MODE_NONE);
    EPDGUI_AddObject(inputbox);
    EPDGUI_AddObject(keyboard);
    EPDGUI_AddObject(_key_exit);
    EPDGUI_AddObject(key_textclear);
    return 6;
}

int Frame_Settings_Wifi_Password::run(void) {
    String data = keyboard->getData();
    if (data.indexOf("\n") >= 0) {
        String *pswd = new String(inputbox->GetText());
        EPDGUI_AddFrameArg("Frame_Settings_Wifi", 0, pswd);        
        inputbox->SetText("");
        EPDGUI_PopFrame();
        _is_run = 0;
        return 0;
    }
    inputbox->AddText(data);
    return 1;
}
