#ifndef BK_AROS_ENTITY_MANAGER_H
#define BK_AROS_ENTITY_MANAGER_H

#include <SDL.h>
#include <stdint.h>

#define BK_ENTITY_MAX 256

typedef enum BKEntityType {
    BK_ENTITY_PLAYER,
    BK_ENTITY_MOB,
    BK_ENTITY_PROJECTILE,
    BK_ENTITY_ITEM
} BKEntityType;

typedef struct BKEntity {
    int active;
    BKEntityType type;
    uint64_t tags;
    float x;
    float y;
    float vx;
    float vy;
    int width;
    int height;
    SDL_Color color;
    int hp;
    int max_hp;
    int damage;
    float cooldown;
    float lifetime;
    int pickup_health;
} BKEntity;

typedef struct BKEntityManager {
    BKEntity entities[BK_ENTITY_MAX];
    int player_index;
    float spawn_timer;
    float input_x;
    float input_y;
    int input_fire;
} BKEntityManager;

void bk_entity_manager_init(BKEntityManager *manager, int screen_width, int screen_height);
void bk_entity_manager_shutdown(BKEntityManager *manager);
void bk_entity_manager_set_player_input(BKEntityManager *manager, float input_x, float input_y, int fire);
void bk_entity_manager_update(BKEntityManager *manager, float dt, int screen_width, int screen_height);
void bk_entity_manager_render(BKEntityManager *manager, SDL_Renderer *renderer);

int bk_entity_manager_player_hp(const BKEntityManager *manager);
int bk_entity_manager_player_max_hp(const BKEntityManager *manager);
int bk_entity_manager_player_alive(const BKEntityManager *manager);

#endif
