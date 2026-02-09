#ifndef BK_AROS_LEVEL_MANAGER_H
#define BK_AROS_LEVEL_MANAGER_H

#include <SDL.h>

typedef struct BKLevelManager {
    int tile_size;
    SDL_Color floor_color;
    SDL_Color accent_color;
} BKLevelManager;

void bk_level_manager_init(BKLevelManager *manager);
void bk_level_manager_shutdown(BKLevelManager *manager);
void bk_level_manager_update(BKLevelManager *manager, float dt);
void bk_level_manager_render(BKLevelManager *manager, SDL_Renderer *renderer, int width, int height);

#endif
