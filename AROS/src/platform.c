#include "platform.h"

#include "util/log.h"

int bk_platform_init(BKPlatformContext *context, const char *title, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) != 0) {
        bk_log_error("SDL_Init failed: %s", SDL_GetError());
        return -1;
    }

    context->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                       width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!context->window) {
        bk_log_error("SDL_CreateWindow failed: %s", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    context->renderer = SDL_CreateRenderer(context->window, -1,
                                           SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!context->renderer) {
        bk_log_error("SDL_CreateRenderer failed: %s", SDL_GetError());
        SDL_DestroyWindow(context->window);
        SDL_Quit();
        return -1;
    }

    context->width = width;
    context->height = height;
    return 0;
}

void bk_platform_shutdown(BKPlatformContext *context) {
    if (context->renderer) {
        SDL_DestroyRenderer(context->renderer);
    }
    if (context->window) {
        SDL_DestroyWindow(context->window);
    }
    SDL_Quit();
}
