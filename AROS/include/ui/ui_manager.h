#ifndef BK_AROS_UI_MANAGER_H
#define BK_AROS_UI_MANAGER_H

#include <SDL.h>

#include "state/state_manager.h"

typedef struct BKUiManager {
    int bar_height;
} BKUiManager;

void bk_ui_manager_init(BKUiManager *manager);
void bk_ui_manager_shutdown(BKUiManager *manager);
void bk_ui_manager_update(BKUiManager *manager, float dt);
void bk_ui_manager_render(BKUiManager *manager, SDL_Renderer *renderer, const BKStateManager *state,
                          int width, int player_hp, int player_max_hp);

#endif
