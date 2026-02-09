#include "game.h"

#include <SDL.h>
#include <stdlib.h>

#include "core/events.h"
#include "core/save.h"
#include "core/settings.h"
#include "core/tags.h"
#include "entity/entity_manager.h"
#include "level/level_manager.h"
#include "state/state_manager.h"
#include "ui/ui_manager.h"
#include "util/log.h"

static const char *k_settings_path = "aros_settings.cfg";

typedef struct BKInputState {
    float move_x;
    float move_y;
    int fire;
} BKInputState;

typedef struct BKGame {
    SDL_Renderer *renderer;
    BKEntityManager entities;
    BKLevelManager level;
    BKStateManager state;
    BKUiManager ui;
    BKInputState input;
    BKSaveStore save_store;
    BKSettings settings;
    BKTagRegistry tag_registry;
    BKTags tags;
    BKEvents events;
    int screen_width;
    int screen_height;
} BKGame;

BKGame *bk_game_create(SDL_Renderer *renderer) {
    BKGame *game = calloc(1, sizeof(BKGame));
    if (!game) {
        bk_log_error("Failed to allocate BKGame");
        return NULL;
    }

    game->renderer = renderer;
    SDL_GetRendererOutputSize(renderer, &game->screen_width, &game->screen_height);

    bk_save_init(&game->save_store);
    bk_settings_setup(&game->settings);
    bk_save_load(&game->save_store, k_settings_path);
    bk_settings_load(&game->settings, &game->save_store);

    bk_tag_registry_init(&game->tag_registry);
    bk_tags_init(&game->tags, &game->tag_registry);
    bk_events_init(&game->events);

    bk_entity_manager_init(&game->entities, game->screen_width, game->screen_height);
    bk_level_manager_init(&game->level);
    bk_state_manager_init(&game->state);
    bk_ui_manager_init(&game->ui);

    bk_log_info("BKGame initialized");
    return game;
}

void bk_game_destroy(BKGame *game) {
    if (!game) {
        return;
    }

    bk_ui_manager_shutdown(&game->ui);
    bk_state_manager_shutdown(&game->state);
    bk_level_manager_shutdown(&game->level);
    bk_entity_manager_shutdown(&game->entities);

    if (game->settings.autosave) {
        bk_settings_save(&game->settings, &game->save_store);
        bk_save_flush(&game->save_store, k_settings_path);
    }

    bk_tag_registry_shutdown(&game->tag_registry);
    bk_save_clear(&game->save_store);

    free(game);
}

void bk_game_handle_event(BKGame *game, const SDL_Event *event) {
    if (!game || !event) {
        return;
    }

    if (event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
        game->screen_width = event->window.data1;
        game->screen_height = event->window.data2;
    }

    if (event->type == SDL_KEYDOWN && event->key.repeat == 0) {
        switch (event->key.keysym.sym) {
            case SDLK_p:
                bk_state_manager_toggle_pause(&game->state);
                break;
            case SDLK_r:
                bk_entity_manager_init(&game->entities, game->screen_width, game->screen_height);
                bk_state_manager_init(&game->state);
                bk_log_info("Run reset");
                break;
            default:
                break;
        }
    }
}

static void bk_game_poll_input(BKGame *game) {
    const uint8_t *keys = SDL_GetKeyboardState(NULL);
    game->input.move_x = 0.0f;
    game->input.move_y = 0.0f;
    game->input.fire = 0;

    if (keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A]) {
        game->input.move_x -= 1.0f;
    }
    if (keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D]) {
        game->input.move_x += 1.0f;
    }
    if (keys[SDL_SCANCODE_UP] || keys[SDL_SCANCODE_W]) {
        game->input.move_y -= 1.0f;
    }
    if (keys[SDL_SCANCODE_DOWN] || keys[SDL_SCANCODE_S]) {
        game->input.move_y += 1.0f;
    }

    if (keys[SDL_SCANCODE_SPACE]) {
        game->input.fire = 1;
    }

    float length_sq = (game->input.move_x * game->input.move_x) + (game->input.move_y * game->input.move_y);
    if (length_sq > 1.0f) {
        float inv_len = 1.0f / SDL_sqrtf(length_sq);
        game->input.move_x *= inv_len;
        game->input.move_y *= inv_len;
    }
}

void bk_game_update(BKGame *game, float dt) {
    if (!game) {
        return;
    }

    if (dt > 0.05f) {
        dt = 0.05f;
    }

    bk_game_poll_input(game);
    bk_state_manager_update(&game->state, dt);
    bk_level_manager_update(&game->level, dt);

    if (!bk_state_manager_is_paused(&game->state) && game->state.state != BK_GAME_STATE_GAME_OVER) {
        bk_entity_manager_set_player_input(&game->entities, game->input.move_x, game->input.move_y, game->input.fire);
        bk_entity_manager_update(&game->entities, dt, game->screen_width, game->screen_height);
    }

    if (!bk_entity_manager_player_alive(&game->entities)) {
        bk_state_manager_set_game_over(&game->state);
    }

    bk_ui_manager_update(&game->ui, dt);
}

void bk_game_render(BKGame *game) {
    if (!game) {
        return;
    }

    bk_level_manager_render(&game->level, game->renderer, game->screen_width, game->screen_height);
    bk_entity_manager_render(&game->entities, game->renderer);
    bk_ui_manager_render(&game->ui, game->renderer, &game->state, game->screen_width,
                         bk_entity_manager_player_hp(&game->entities),
                         bk_entity_manager_player_max_hp(&game->entities));
}
