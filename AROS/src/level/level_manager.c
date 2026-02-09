#include "level/level_manager.h"

#include "util/log.h"

void bk_level_manager_init(BKLevelManager *manager) {
    manager->tile_size = 64;
    manager->floor_color = (SDL_Color){22, 22, 32, 255};
    manager->accent_color = (SDL_Color){30, 30, 42, 255};
    bk_log_info("Level manager initialized");
}

void bk_level_manager_shutdown(BKLevelManager *manager) {
    (void)manager;
    bk_log_info("Level manager shutdown");
}

void bk_level_manager_update(BKLevelManager *manager, float dt) {
    (void)manager;
    (void)dt;
}

void bk_level_manager_render(BKLevelManager *manager, SDL_Renderer *renderer, int width, int height) {
    int tiles_x = (width + manager->tile_size - 1) / manager->tile_size;
    int tiles_y = (height + manager->tile_size - 1) / manager->tile_size;

    for (int y = 0; y < tiles_y; y++) {
        for (int x = 0; x < tiles_x; x++) {
            SDL_Color color = ((x + y) % 2 == 0) ? manager->floor_color : manager->accent_color;
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

            SDL_Rect tile = {
                x * manager->tile_size,
                y * manager->tile_size,
                manager->tile_size,
                manager->tile_size
            };
            SDL_RenderFillRect(renderer, &tile);
        }
    }
}
