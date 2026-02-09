#ifndef BK_AROS_SETTINGS_H
#define BK_AROS_SETTINGS_H

#include "core/save.h"

typedef struct BKSettings {
    float master_volume;
    float music_volume;
    float sfx_volume;
    int fullscreen;
    int vsync;
    int blood;
    int ui_sfx;
    int minimap;
    int cursor;
    int rotate_cursor;
    float freeze_frames;
    float flash_frames;
    int show_fps;
    int flashes;
    int low_quality;
    int vignette;
    float floor_darkness;
    float game_scale;
    int speedrun_mode;
    int speedrun_timer;
    float screenshake;
    int vegan;
    int autopause;
    int autosave;
    char gamepad[64];
    int vibrate;
    float sensivity;
    float cursor_radius;
    char language[16];
    int hide_ui;
    int hide_cursor;
} BKSettings;

void bk_settings_setup(BKSettings *settings);
void bk_settings_load(BKSettings *settings, BKSaveStore *store);
void bk_settings_save(const BKSettings *settings, BKSaveStore *store);

#endif
