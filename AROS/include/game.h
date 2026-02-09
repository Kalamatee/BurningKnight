#ifndef BK_AROS_GAME_H
#define BK_AROS_GAME_H

#include <SDL.h>

typedef struct BKGame BKGame;

BKGame *bk_game_create(SDL_Renderer *renderer);
void bk_game_destroy(BKGame *game);

void bk_game_handle_event(BKGame *game, const SDL_Event *event);
void bk_game_update(BKGame *game, float dt);
void bk_game_render(BKGame *game);

#endif
