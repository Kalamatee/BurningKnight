#include <SDL.h>

#include "game.h"
#include "log.h"
#include "platform.h"

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    BKPlatformContext platform = {0};
    if (bk_platform_init(&platform, "Burning Knight (AROS)", 1280, 720) != 0) {
        return 1;
    }

    BKGame *game = bk_game_create(platform.renderer);
    if (!game) {
        bk_platform_shutdown(&platform);
        return 1;
    }

    int running = 1;
    uint32_t last_ticks = SDL_GetTicks();

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }

            bk_game_handle_event(game, &event);
        }

        uint32_t current_ticks = SDL_GetTicks();
        float dt = (current_ticks - last_ticks) / 1000.0f;
        last_ticks = current_ticks;

        bk_game_update(game, dt);

        SDL_SetRenderDrawColor(platform.renderer, 12, 12, 18, 255);
        SDL_RenderClear(platform.renderer);

        bk_game_render(game);
        SDL_RenderPresent(platform.renderer);
    }

    bk_game_destroy(game);
    bk_platform_shutdown(&platform);
    return 0;
}
