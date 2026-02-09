#ifndef BK_AROS_PLATFORM_H
#define BK_AROS_PLATFORM_H

#include <SDL.h>

typedef struct BKPlatformContext {
    SDL_Window *window;
    SDL_Renderer *renderer;
    int width;
    int height;
} BKPlatformContext;

int bk_platform_init(BKPlatformContext *context, const char *title, int width, int height);
void bk_platform_shutdown(BKPlatformContext *context);

#endif
