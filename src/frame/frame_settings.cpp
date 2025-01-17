#include "frame_settings.h"
#include "frame_settings_wifi.h"
#include "frame_settings_wallpaper.h"
#include "WiFi.h"

#define KEY_W 92
#define KEY_H 92
const uint16_t kTimeZoneY = 500;

void key_shutdown_cb(epdgui_args_vector_t &args) {
    M5.EPD.WriteFullGram4bpp(GetWallpaper());
    M5.EPD.UpdateFull(UPDATE_MODE_GC16);
    M5.EPD.UpdateFull(UPDATE_MODE_GC16);
    SaveSetting();
    delay(600);
    M5.disableEPDPower();
    M5.disableEXTPower();
    M5.disableMainPower();
    esp_deep_sleep_start();
    while (true);
}

void key_restart_cb(epdgui_args_vector_t &args) {
    M5.EPD.WriteFullGram4bpp(GetWallpaper());
    M5.EPD.UpdateFull(UPDATE_MODE_GC16);
    SaveSetting();
    esp_restart();
}

void key_wifi_cb(epdgui_args_vector_t &args) {
    Frame_Base *frame = EPDGUI_GetFrame("Frame_Settings_Wifi");
    if (frame == NULL) {
        frame = new Frame_Settings_Wifi();
        EPDGUI_AddFrame("Frame_Settings_Wifi", frame);
    }
    EPDGUI_PushFrame(frame);
    *((int*)(args[0])) = 0;
}

void key_wallpaper_cb(epdgui_args_vector_t &args) {
    Frame_Base *frame = EPDGUI_GetFrame("Frame_Settings_Wallpaper");
    if (frame == NULL) {
        frame = new Frame_Settings_Wallpaper();
        EPDGUI_AddFrame("Frame_Settings_Wallpaper", frame);
    }
    EPDGUI_PushFrame(frame);
    *((int*)(args[0])) = 0;
}

void key_synctime_cb(epdgui_args_vector_t &args) {
    // SaveSetting();
    M5EPD_Canvas info(&M5.EPD);
    M5EPD_Canvas *title = (M5EPD_Canvas*)(args[0]);
    M5EPD_Canvas *tzone = (M5EPD_Canvas*)(args[1]);
    info.createCanvas(300, 100);
    info.fillCanvas(15);
    info.setTextSize(26);
    info.setTextColor(0);
    info.setTextDatum(CC_DATUM);
    if (WiFi.status() != WL_CONNECTED) {
        info.drawString("Wi-Fi not connected", 150, 55);
        info.pushCanvas(120, 430, UPDATE_MODE_GL16);
        M5.EPD.WriteFullGram4bpp(GetWallpaper());
        title->pushCanvas(0, 8, UPDATE_MODE_NONE);
        tzone->pushCanvas(4, kTimeZoneY, UPDATE_MODE_NONE);
        EPDGUI_Draw(UPDATE_MODE_NONE);
        while (!M5.TP.avaliable());
        M5.EPD.UpdateFull(UPDATE_MODE_GL16);
        return;
    }
    LoadingAnime_32x32_Start(532 - 15 - 32, 220 + 14);
    bool ret = SyncNTPTime();
    LoadingAnime_32x32_Stop();

    if (ret == 0) {
        info.drawString("Time sync failed", 150, 55);
        info.pushCanvas(120, 430, UPDATE_MODE_GL16);
     } else {
        info.drawString("Success", 150, 55);
        info.pushCanvas(120, 430, UPDATE_MODE_GL16);
    }
    M5.EPD.WriteFullGram4bpp(GetWallpaper());
    title->pushCanvas(0, 8, UPDATE_MODE_NONE);
    tzone->pushCanvas(4, kTimeZoneY, UPDATE_MODE_NONE);
    EPDGUI_Draw(UPDATE_MODE_NONE);
    while (!M5.TP.avaliable());
    M5.EPD.UpdateFull(UPDATE_MODE_GL16);
}

void key_timezone_plus_cb(epdgui_args_vector_t &args) {
    int *tz = (int*)(args[0]);
    (*tz)++;
    if ((*tz) > 12) {
        (*tz) = 12;
    }
    String str = String(*tz);
    if ((*tz) > 0) {
        str = "+" + str;
    }
    ((EPDGUI_Button*)(args[1]))->setLabel(str);
    ((EPDGUI_Button*)(args[1]))->Draw(UPDATE_MODE_GL16);
    SetTimeZone(*tz);
}

void key_timezone_minus_cb(epdgui_args_vector_t &args) {
    int *tz = (int*)(args[0]);
    (*tz)--;
    if ((*tz) < -11) {
        (*tz) = -11;
    }
    String str = String(*tz);
    if ((*tz) > 0) {
        str = "+" + str;
    }
    ((EPDGUI_Button*)(args[1]))->setLabel(str);
    ((EPDGUI_Button*)(args[1]))->Draw(UPDATE_MODE_GL16);
    SetTimeZone(*tz);
}

void key_timezone_reset_cb(epdgui_args_vector_t &args) {
    int *tz = (int*)(args[0]);
    (*tz) = 0;
    ((EPDGUI_Button*)(args[1]))->setLabel(String(*tz));
    ((EPDGUI_Button*)(args[1]))->Draw(UPDATE_MODE_GL16);
    SetTimeZone(*tz);
}

Frame_Settings::Frame_Settings(void) {
    _frame_name = "Frame_Settings";

    _timezone_canvas = new M5EPD_Canvas(&M5.EPD);
    _timezone_canvas->createCanvas(540, 60);
    _timezone_canvas->fillCanvas(0);
    _timezone_canvas->setTextSize(26);
    _timezone_canvas->setTextColor(15);
    _timezone_canvas->setTextDatum(CL_DATUM);
    _timezone_canvas->drawRect(0, 0, 540, 60, 15);
 
    _key_wifi = new EPDGUI_Button(4, 100, 532, 61);
    _key_wallpaper = new EPDGUI_Button(4, 160, 532, 61);
    _key_syncntp = new EPDGUI_Button(4, 220, 532, 61);
    _key_restart = new EPDGUI_Button(4, 320, 532, 61);
    _key_shutdown = new EPDGUI_Button(4, 380, 532, 61);

    key_timezone_plus = new EPDGUI_Button("+", 448, kTimeZoneY + 2, 88, 52);
    String str = String(GetTimeZone());
    if (GetTimeZone() > 0) {
        str = "+" + str;
    }
    key_timezone_reset = new EPDGUI_Button(str, 360, kTimeZoneY + 2, 88, 52);
    key_timezone_minus = new EPDGUI_Button("–", 272, kTimeZoneY + 2, 88, 52);
    
    key_timezone_plus->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, &_timezone);
    key_timezone_plus->AddArgs(EPDGUI_Button::EVENT_RELEASED, 1, key_timezone_reset);
    key_timezone_plus->Bind(EPDGUI_Button::EVENT_RELEASED, key_timezone_plus_cb);

    key_timezone_reset->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, &_timezone);
    key_timezone_reset->AddArgs(EPDGUI_Button::EVENT_RELEASED, 1, key_timezone_reset);
    key_timezone_reset->Bind(EPDGUI_Button::EVENT_RELEASED, key_timezone_reset_cb);

    key_timezone_minus->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, &_timezone);
    key_timezone_minus->AddArgs(EPDGUI_Button::EVENT_RELEASED, 1, key_timezone_reset);
    key_timezone_minus->Bind(EPDGUI_Button::EVENT_RELEASED, key_timezone_minus_cb);

    _key_wifi->setBMPButton("  Wi-Fi", "\u25B6", ImageResource_item_icon_wifi_3_32x32);
    _key_wallpaper->setBMPButton("  Wallpaper", "\u25B6", ImageResource_item_icon_wallpaper_32x32);
    _key_syncntp->setBMPButton("  Sync Time", "", ImageResource_item_icon_ntptime_32x32);
    _key_restart->setBMPButton("  Restart", "", ImageResource_item_icon_restart_32x32);
    _key_shutdown->setBMPButton("  Shutdown", "", ImageResource_item_icon_shutdown_32x32);
    _timezone_canvas->drawString("Time Zone (UTC)", 15, 35);
    exitbtn("Home");
    _canvas_title->drawString("Settings", 270, 34);

    _key_exit->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void*)(&_is_run));
    _key_exit->Bind(EPDGUI_Button::EVENT_RELEASED, &Frame_Base::exit_cb);

    _key_wifi->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void*)(&_is_run));
    _key_wifi->Bind(EPDGUI_Button::EVENT_RELEASED, &key_wifi_cb);

    _key_wallpaper->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void*)(&_is_run));
    _key_wallpaper->Bind(EPDGUI_Button::EVENT_RELEASED, &key_wallpaper_cb);

    _key_shutdown->Bind(EPDGUI_Button::EVENT_RELEASED, &key_shutdown_cb);
    _key_restart->Bind(EPDGUI_Button::EVENT_RELEASED, &key_restart_cb);
    _key_syncntp->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, _canvas_title);
    _key_syncntp->AddArgs(EPDGUI_Button::EVENT_RELEASED, 1, _timezone_canvas);
    _key_syncntp->Bind(EPDGUI_Button::EVENT_RELEASED, &key_synctime_cb);

    _timezone = GetTimeZone();
}

Frame_Settings::~Frame_Settings(void) {
    delete _key_wifi;
    delete _key_wallpaper;
    delete _key_shutdown;
    delete _key_restart;
    delete _key_syncntp;
}

int Frame_Settings::init(epdgui_args_vector_t &args) {
    _is_run = 1;
    M5.EPD.WriteFullGram4bpp(GetWallpaper());
    _canvas_title->pushCanvas(0, 8, UPDATE_MODE_NONE);
    _timezone_canvas->pushCanvas(0, kTimeZoneY, UPDATE_MODE_NONE);
    EPDGUI_AddObject(_key_wifi);
    EPDGUI_AddObject(_key_wallpaper);
    EPDGUI_AddObject(_key_shutdown);
    EPDGUI_AddObject(_key_restart);
    EPDGUI_AddObject(_key_syncntp);
    EPDGUI_AddObject(_key_exit);
    EPDGUI_AddObject(key_timezone_plus);
    EPDGUI_AddObject(key_timezone_reset);
    EPDGUI_AddObject(key_timezone_minus);
    return 3;
}