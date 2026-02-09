#include "core/tags.h"

#include <stdlib.h>
#include <string.h>

#include "util/log.h"

static void bk_tag_registry_reserve(BKTagRegistry *registry, size_t capacity) {
    if (registry->capacity >= capacity) {
        return;
    }

    size_t new_capacity = registry->capacity == 0 ? 16 : registry->capacity * 2;
    while (new_capacity < capacity) {
        new_capacity *= 2;
    }

    char **names = realloc(registry->names, new_capacity * sizeof(char *));
    if (!names) {
        bk_log_error("Failed to grow tag registry");
        return;
    }

    registry->names = names;
    registry->capacity = new_capacity;
}

void bk_tag_registry_init(BKTagRegistry *registry) {
    registry->names = NULL;
    registry->count = 0;
    registry->capacity = 0;
}

void bk_tag_registry_shutdown(BKTagRegistry *registry) {
    for (size_t i = 0; i < registry->count; i++) {
        free(registry->names[i]);
    }
    free(registry->names);
    registry->names = NULL;
    registry->count = 0;
    registry->capacity = 0;
}

int bk_tag_registry_register(BKTagRegistry *registry, const char *name) {
    for (size_t i = 0; i < registry->count; i++) {
        if (strcmp(registry->names[i], name) == 0) {
            return (int)i;
        }
    }

    bk_tag_registry_reserve(registry, registry->count + 1);
    char *copy = strdup(name);
    if (!copy) {
        bk_log_error("Failed to allocate tag name");
        return -1;
    }

    registry->names[registry->count] = copy;
    return (int)registry->count++;
}

const char *bk_tag_registry_name(const BKTagRegistry *registry, int tag) {
    if (tag < 0 || (size_t)tag >= registry->count) {
        return NULL;
    }
    return registry->names[tag];
}

void bk_tags_init(BKTags *tags, BKTagRegistry *registry) {
    tags->player = bk_tag_registry_register(registry, "player");
    tags->player_target = bk_tag_registry_register(registry, "player_target");
    tags->cursor = bk_tag_registry_register(registry, "cursor");
    tags->mob = bk_tag_registry_register(registry, "mob");
    tags->boss = bk_tag_registry_register(registry, "boss");
    tags->burning_knight = bk_tag_registry_register(registry, "burning_knight");
    tags->player_save = bk_tag_registry_register(registry, "player_save");
    tags->level_save = bk_tag_registry_register(registry, "level_save");
    tags->bomb = bk_tag_registry_register(registry, "bomb");

    tags->room = bk_tag_registry_register(registry, "room");
    tags->teleport_trigger = bk_tag_registry_register(registry, "teleport_trigger");
    tags->lock_tag = bk_tag_registry_register(registry, "lock");
    tags->must_be_killed = bk_tag_registry_register(registry, "must_be_killed");
    tags->teleport = bk_tag_registry_register(registry, "teleport");
    tags->statue = bk_tag_registry_register(registry, "statue");

    tags->has_shadow = bk_tag_registry_register(registry, "shadow");
    tags->mess = bk_tag_registry_register(registry, "mess");

    tags->checkpoint = bk_tag_registry_register(registry, "checkpoint");
    tags->entrance = bk_tag_registry_register(registry, "entrance");
    tags->hidden_entrance = bk_tag_registry_register(registry, "hidden_entrance");
    tags->item = bk_tag_registry_register(registry, "item");
    tags->shop_keeper = bk_tag_registry_register(registry, "shop_keeper");
    tags->npc = bk_tag_registry_register(registry, "npc");

    tags->button = bk_tag_registry_register(registry, "button");
    tags->chest = bk_tag_registry_register(registry, "chest");
    tags->projectile = bk_tag_registry_register(registry, "projectile");
    tags->mob_projectile = bk_tag_registry_register(registry, "mob_projectile");
    tags->player_projectile = bk_tag_registry_register(registry, "player_projectile");

    tags->laser = bk_tag_registry_register(registry, "laser");
    tags->fire_particle = bk_tag_registry_register(registry, "fire_particle");
    tags->text_particle = bk_tag_registry_register(registry, "text_particle");
    tags->door = bk_tag_registry_register(registry, "door");
}
