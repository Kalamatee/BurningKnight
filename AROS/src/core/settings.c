#include "core/settings.h"

#include <string.h>

static void bk_settings_set_string(char *dest, size_t dest_size, const char *value) {
    if (!value) {
        dest[0] = '\0';
        return;
    }

    strncpy(dest, value, dest_size - 1);
    dest[dest_size - 1] = '\0';
}

void bk_settings_setup(BKSettings *settings) {
    settings->fullscreen = 1;
    settings->show_fps = 0;
    settings->blood = 1;
    settings->ui_sfx = 1;
    settings->vsync = 1;
    settings->screenshake = 0.35f;
    settings->speedrun_mode = 0;
    settings->speedrun_timer = 0;
    settings->freeze_frames = 0.5f;
    settings->flash_frames = 0.5f;
    settings->sfx_volume = 0.6f;
    settings->music_volume = 0.4f;
    settings->master_volume = 1.0f;
    settings->cursor = 0;
    settings->rotate_cursor = 0;
    settings->vegan = 0;
    settings->vignette = 1;
    settings->autopause = 0;
    settings->autosave = 1;
    settings->vibrate = 1;
    settings->sensivity = 1.5f;
    settings->game_scale = 1.0f;
    settings->floor_darkness = 1.0f;
    settings->cursor_radius = 1.0f;
    settings->minimap = 1;
    settings->flashes = 1;
    settings->low_quality = 0;
    settings->hide_ui = 0;
    settings->hide_cursor = 0;

    bk_settings_set_string(settings->gamepad, sizeof(settings->gamepad), "");
    bk_settings_set_string(settings->language, sizeof(settings->language), "en");
}

void bk_settings_load(BKSettings *settings, BKSaveStore *store) {
    settings->fullscreen = bk_save_get_bool(store, "s_fullscreen", settings->fullscreen);
    settings->show_fps = bk_save_get_bool(store, "s_fps", settings->show_fps);
    settings->blood = bk_save_get_bool(store, "s_blood", settings->blood);
    settings->ui_sfx = bk_save_get_bool(store, "s_uisfx", settings->ui_sfx);
    settings->vsync = bk_save_get_bool(store, "s_vsync", settings->vsync);
    settings->speedrun_mode = bk_save_get_bool(store, "s_sm", settings->speedrun_mode);
    settings->speedrun_timer = bk_save_get_bool(store, "s_stmr", settings->speedrun_timer);
    settings->screenshake = bk_save_get_float(store, "s_screenshake", settings->screenshake);
    settings->sfx_volume = bk_save_get_float(store, "s_sfx", settings->sfx_volume);
    settings->freeze_frames = bk_save_get_float(store, "s_frf", settings->freeze_frames);
    settings->flash_frames = bk_save_get_float(store, "s_ff", settings->flash_frames);
    settings->music_volume = bk_save_get_float(store, "s_music", settings->music_volume);
    settings->master_volume = bk_save_get_float(store, "s_master", settings->master_volume);
    settings->cursor = bk_save_get_int(store, "s_cursor", settings->cursor);
    settings->rotate_cursor = bk_save_get_bool(store, "s_rotate_cursor", settings->rotate_cursor);
    settings->vegan = bk_save_get_bool(store, "s_v", settings->vegan);
    settings->autosave = bk_save_get_bool(store, "s_as", settings->autosave);
    settings->autopause = bk_save_get_bool(store, "s_ap", settings->autopause);
    settings->vibrate = bk_save_get_bool(store, "s_vb", settings->vibrate);
    settings->minimap = bk_save_get_bool(store, "s_mm", settings->minimap);
    settings->flashes = bk_save_get_bool(store, "s_fl", settings->flashes);
    settings->vignette = bk_save_get_bool(store, "s_vgn", settings->vignette);
    settings->low_quality = bk_save_get_bool(store, "s_lq", settings->low_quality);
    settings->sensivity = bk_save_get_float(store, "s_ss", settings->sensivity);
    settings->game_scale = bk_save_get_float(store, "s_gs", settings->game_scale);
    settings->floor_darkness = bk_save_get_float(store, "s_fd", settings->floor_darkness);
    settings->cursor_radius = bk_save_get_float(store, "s_cr", settings->cursor_radius);

    const char *gamepad = bk_save_get(store, "s_gp");
    if (gamepad) {
        bk_settings_set_string(settings->gamepad, sizeof(settings->gamepad), gamepad);
    }

    const char *language = bk_save_get(store, "s_ln");
    if (language) {
        bk_settings_set_string(settings->language, sizeof(settings->language), language);
    }
}

void bk_settings_save(const BKSettings *settings, BKSaveStore *store) {
    bk_save_put_bool(store, "s_fullscreen", settings->fullscreen);
    bk_save_put_bool(store, "s_fps", settings->show_fps);
    bk_save_put_bool(store, "s_blood", settings->blood);
    bk_save_put_bool(store, "s_uisfx", settings->ui_sfx);
    bk_save_put_bool(store, "s_vsync", settings->vsync);
    bk_save_put_bool(store, "s_sm", settings->speedrun_mode);
    bk_save_put_bool(store, "s_stmr", settings->speedrun_timer);
    bk_save_put_float(store, "s_frf", settings->freeze_frames);
    bk_save_put_float(store, "s_ff", settings->flash_frames);
    bk_save_put_float(store, "s_screenshake", settings->screenshake);
    bk_save_put_float(store, "s_sfx", settings->sfx_volume);
    bk_save_put_float(store, "s_music", settings->music_volume);
    bk_save_put_float(store, "s_master", settings->master_volume);
    bk_save_put_int(store, "s_cursor", settings->cursor);
    bk_save_put_bool(store, "s_rotate_cursor", settings->rotate_cursor);
    bk_save_put_bool(store, "s_v", settings->vegan);
    bk_save_put_bool(store, "s_as", settings->autosave);
    bk_save_put_bool(store, "s_ap", settings->autopause);
    bk_save_put(store, "s_gp", settings->gamepad);
    bk_save_put_bool(store, "s_vb", settings->vibrate);
    bk_save_put_float(store, "s_ss", settings->sensivity);
    bk_save_put_float(store, "s_gs", settings->game_scale);
    bk_save_put_float(store, "s_fd", settings->floor_darkness);
    bk_save_put_float(store, "s_cr", settings->cursor_radius);
    bk_save_put(store, "s_ln", settings->language);
    bk_save_put_bool(store, "s_mm", settings->minimap);
    bk_save_put_bool(store, "s_fl", settings->flashes);
    bk_save_put_bool(store, "s_lq", settings->low_quality);
    bk_save_put_bool(store, "s_vgn", settings->vignette);
}
