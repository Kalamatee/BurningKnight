#include "entity/entity_manager.h"

#include <math.h>
#include <stdlib.h>

#include "core/tags.h"
#include "util/log.h"

static const float k_player_speed = 240.0f;
static const float k_projectile_speed = 420.0f;
static const float k_mob_speed = 120.0f;
static const float k_spawn_interval = 2.5f;

static int bk_entity_alloc(BKEntityManager *manager) {
    for (int i = 0; i < BK_ENTITY_MAX; i++) {
        if (!manager->entities[i].active) {
            manager->entities[i].active = 1;
            return i;
        }
    }
    return -1;
}

static int bk_entity_intersects(const BKEntity *a, const BKEntity *b) {
    return a->x < b->x + b->width &&
           a->x + a->width > b->x &&
           a->y < b->y + b->height &&
           a->y + a->height > b->y;
}

static void bk_entity_spawn_player(BKEntityManager *manager, int screen_width, int screen_height) {
    int index = bk_entity_alloc(manager);
    if (index < 0) {
        return;
    }

    BKEntity *player = &manager->entities[index];
    player->type = BK_ENTITY_PLAYER;
    player->width = 32;
    player->height = 32;
    player->x = (screen_width - player->width) * 0.5f;
    player->y = (screen_height - player->height) * 0.5f;
    player->vx = 0.0f;
    player->vy = 0.0f;
    player->color = (SDL_Color){231, 77, 64, 255};
    player->hp = 6;
    player->max_hp = 6;
    player->damage = 1;
    player->cooldown = 0.0f;

    manager->player_index = index;
}

static void bk_entity_spawn_mob(BKEntityManager *manager, int screen_width, int screen_height) {
    int index = bk_entity_alloc(manager);
    if (index < 0) {
        return;
    }

    BKEntity *mob = &manager->entities[index];
    mob->type = BK_ENTITY_MOB;
    mob->width = 28;
    mob->height = 28;
    mob->x = (float)(rand() % (screen_width - mob->width));
    mob->y = (float)(rand() % (screen_height - mob->height));
    mob->vx = 0.0f;
    mob->vy = 0.0f;
    mob->color = (SDL_Color){90, 200, 120, 255};
    mob->hp = 3;
    mob->max_hp = 3;
    mob->damage = 1;
    mob->cooldown = 0.0f;
}

static void bk_entity_spawn_item(BKEntityManager *manager, int screen_width, int screen_height) {
    int index = bk_entity_alloc(manager);
    if (index < 0) {
        return;
    }

    BKEntity *item = &manager->entities[index];
    item->type = BK_ENTITY_ITEM;
    item->width = 18;
    item->height = 18;
    item->x = (float)(rand() % (screen_width - item->width));
    item->y = (float)(rand() % (screen_height - item->height));
    item->vx = 0.0f;
    item->vy = 0.0f;
    item->color = (SDL_Color){255, 229, 163, 255};
    item->pickup_health = 2;
}

static void bk_entity_spawn_projectile(BKEntityManager *manager, const BKEntity *source, float dir_x, float dir_y) {
    int index = bk_entity_alloc(manager);
    if (index < 0) {
        return;
    }

    BKEntity *projectile = &manager->entities[index];
    projectile->type = BK_ENTITY_PROJECTILE;
    projectile->width = 8;
    projectile->height = 8;
    projectile->x = source->x + source->width * 0.5f - projectile->width * 0.5f;
    projectile->y = source->y + source->height * 0.5f - projectile->height * 0.5f;
    projectile->vx = dir_x * k_projectile_speed;
    projectile->vy = dir_y * k_projectile_speed;
    projectile->color = (SDL_Color){150, 180, 255, 255};
    projectile->damage = source->damage;
    projectile->lifetime = 1.5f;
}

void bk_entity_manager_init(BKEntityManager *manager, int screen_width, int screen_height) {
    for (int i = 0; i < BK_ENTITY_MAX; i++) {
        manager->entities[i].active = 0;
    }

    manager->player_index = -1;
    manager->spawn_timer = k_spawn_interval;
    manager->input_x = 0.0f;
    manager->input_y = 0.0f;
    manager->input_fire = 0;

    srand((unsigned int)SDL_GetTicks());

    bk_entity_spawn_player(manager, screen_width, screen_height);
    bk_entity_spawn_item(manager, screen_width, screen_height);

    bk_log_info("Entity manager initialized");
}

void bk_entity_manager_shutdown(BKEntityManager *manager) {
    (void)manager;
    bk_log_info("Entity manager shutdown");
}

void bk_entity_manager_set_player_input(BKEntityManager *manager, float input_x, float input_y, int fire) {
    manager->input_x = input_x;
    manager->input_y = input_y;
    manager->input_fire = fire;
}

static void bk_entity_update_player(BKEntityManager *manager, BKEntity *player, float dt, int screen_width, int screen_height) {
    player->vx = manager->input_x * k_player_speed;
    player->vy = manager->input_y * k_player_speed;

    player->x += player->vx * dt;
    player->y += player->vy * dt;

    if (player->x < 0.0f) {
        player->x = 0.0f;
    }
    if (player->y < 0.0f) {
        player->y = 0.0f;
    }

    float max_x = (float)(screen_width - player->width);
    float max_y = (float)(screen_height - player->height);

    if (player->x > max_x) {
        player->x = max_x;
    }
    if (player->y > max_y) {
        player->y = max_y;
    }

    if (player->cooldown > 0.0f) {
        player->cooldown -= dt;
    }

    if (manager->input_fire && player->cooldown <= 0.0f) {
        float dir_x = manager->input_x;
        float dir_y = manager->input_y;
        if (fabsf(dir_x) < 0.01f && fabsf(dir_y) < 0.01f) {
            dir_y = -1.0f;
        }

        float len = SDL_sqrtf(dir_x * dir_x + dir_y * dir_y);
        if (len > 0.01f) {
            dir_x /= len;
            dir_y /= len;
        }

        bk_entity_spawn_projectile(manager, player, dir_x, dir_y);
        player->cooldown = 0.25f;
    }
}

static void bk_entity_update_mob(BKEntityManager *manager, BKEntity *mob, const BKEntity *player, float dt) {
    float dir_x = player->x - mob->x;
    float dir_y = player->y - mob->y;
    float len = SDL_sqrtf(dir_x * dir_x + dir_y * dir_y);
    if (len > 0.01f) {
        dir_x /= len;
        dir_y /= len;
    }

    mob->vx = dir_x * k_mob_speed;
    mob->vy = dir_y * k_mob_speed;
    mob->x += mob->vx * dt;
    mob->y += mob->vy * dt;

    if (mob->cooldown > 0.0f) {
        mob->cooldown -= dt;
    }

    if (bk_entity_intersects(mob, player) && mob->cooldown <= 0.0f) {
        if (player->hp > 0) {
            ((BKEntity *)player)->hp -= mob->damage;
            mob->cooldown = 1.0f;
        }
    }
}

static void bk_entity_update_projectile(BKEntity *projectile, float dt) {
    projectile->x += projectile->vx * dt;
    projectile->y += projectile->vy * dt;
    projectile->lifetime -= dt;
    if (projectile->lifetime <= 0.0f) {
        projectile->active = 0;
    }
}

void bk_entity_manager_update(BKEntityManager *manager, float dt, int screen_width, int screen_height) {
    BKEntity *player = NULL;
    if (manager->player_index >= 0 && manager->entities[manager->player_index].active) {
        player = &manager->entities[manager->player_index];
    }

    if (player && player->hp <= 0) {
        player->active = 0;
    }

    if (player) {
        bk_entity_update_player(manager, player, dt, screen_width, screen_height);
    }

    for (int i = 0; i < BK_ENTITY_MAX; i++) {
        BKEntity *entity = &manager->entities[i];
        if (!entity->active || entity->type == BK_ENTITY_PLAYER) {
            continue;
        }

        if (entity->type == BK_ENTITY_MOB && player) {
            bk_entity_update_mob(manager, entity, player, dt);
        } else if (entity->type == BK_ENTITY_PROJECTILE) {
            bk_entity_update_projectile(entity, dt);
        }
    }

    for (int i = 0; i < BK_ENTITY_MAX; i++) {
        BKEntity *entity = &manager->entities[i];
        if (!entity->active || entity->type != BK_ENTITY_PROJECTILE) {
            continue;
        }

        for (int j = 0; j < BK_ENTITY_MAX; j++) {
            BKEntity *target = &manager->entities[j];
            if (!target->active || target->type != BK_ENTITY_MOB) {
                continue;
            }

            if (bk_entity_intersects(entity, target)) {
                target->hp -= entity->damage;
                entity->active = 0;
                if (target->hp <= 0) {
                    target->active = 0;
                }
                break;
            }
        }
    }

    if (player) {
        for (int i = 0; i < BK_ENTITY_MAX; i++) {
            BKEntity *entity = &manager->entities[i];
            if (!entity->active || entity->type != BK_ENTITY_ITEM) {
                continue;
            }

            if (bk_entity_intersects(entity, player)) {
                player->hp += entity->pickup_health;
                if (player->hp > player->max_hp) {
                    player->hp = player->max_hp;
                }
                entity->active = 0;
            }
        }
    }

    manager->spawn_timer -= dt;
    if (manager->spawn_timer <= 0.0f) {
        manager->spawn_timer = k_spawn_interval;
        bk_entity_spawn_mob(manager, screen_width, screen_height);
    }
}

void bk_entity_manager_render(BKEntityManager *manager, SDL_Renderer *renderer) {
    for (int i = 0; i < BK_ENTITY_MAX; i++) {
        BKEntity *entity = &manager->entities[i];
        if (!entity->active) {
            continue;
        }

        SDL_Rect rect = {
            (int)roundf(entity->x),
            (int)roundf(entity->y),
            entity->width,
            entity->height
        };

        SDL_SetRenderDrawColor(renderer, entity->color.r, entity->color.g, entity->color.b, entity->color.a);
        SDL_RenderFillRect(renderer, &rect);

        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderDrawRect(renderer, &rect);
    }
}

int bk_entity_manager_player_hp(const BKEntityManager *manager) {
    if (manager->player_index < 0) {
        return 0;
    }

    const BKEntity *player = &manager->entities[manager->player_index];
    if (!player->active) {
        return 0;
    }

    return player->hp;
}

int bk_entity_manager_player_max_hp(const BKEntityManager *manager) {
    if (manager->player_index < 0) {
        return 1;
    }

    const BKEntity *player = &manager->entities[manager->player_index];
    if (!player->active) {
        return 1;
    }

    return player->max_hp;
}

int bk_entity_manager_player_alive(const BKEntityManager *manager) {
    if (manager->player_index < 0) {
        return 0;
    }

    return manager->entities[manager->player_index].active != 0;
}
