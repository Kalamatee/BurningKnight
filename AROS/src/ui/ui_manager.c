#include "ui/ui_manager.h"

#include "util/log.h"

void bk_ui_manager_init(BKUiManager *manager) {
    manager->bar_height = 36;
    bk_log_info("UI manager initialized");
}

void bk_ui_manager_shutdown(BKUiManager *manager) {
    (void)manager;
    bk_log_info("UI manager shutdown");
}

void bk_ui_manager_update(BKUiManager *manager, float dt) {
    (void)manager;
    (void)dt;
}

void bk_ui_manager_render(BKUiManager *manager, SDL_Renderer *renderer, const BKStateManager *state,
                          int width, int player_hp, int player_max_hp) {
    SDL_Rect bar = {0, 0, width, manager->bar_height};
    SDL_SetRenderDrawColor(renderer, 18, 18, 28, 220);
    SDL_RenderFillRect(renderer, &bar);

    SDL_SetRenderDrawColor(renderer, 255, 229, 163, 255);
    SDL_RenderDrawRect(renderer, &bar);

    int health_width = (int)((width - 20) * (player_max_hp > 0 ? (float)player_hp / player_max_hp : 0.0f));
    if (health_width < 0) {
        health_width = 0;
    }

    SDL_Rect health = {10, 10, health_width, 16};
    SDL_SetRenderDrawColor(renderer, 231, 77, 64, 255);
    SDL_RenderFillRect(renderer, &health);

    if (state && bk_state_manager_is_paused(state)) {
        SDL_Rect paused = {width - 120, 8, 96, 20};
        SDL_SetRenderDrawColor(renderer, 231, 77, 64, 255);
        SDL_RenderFillRect(renderer, &paused);
        SDL_SetRenderDrawColor(renderer, 255, 229, 163, 255);
        SDL_RenderDrawRect(renderer, &paused);
    }

    if (state && state->state == BK_GAME_STATE_GAME_OVER) {
        SDL_Rect over = {width / 2 - 80, 8, 160, 20};
        SDL_SetRenderDrawColor(renderer, 90, 20, 20, 255);
        SDL_RenderFillRect(renderer, &over);
        SDL_SetRenderDrawColor(renderer, 255, 229, 163, 255);
        SDL_RenderDrawRect(renderer, &over);
    }
}
