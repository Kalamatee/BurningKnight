#ifndef BK_AROS_TAGS_H
#define BK_AROS_TAGS_H

#include <stddef.h>

typedef struct BKTagRegistry {
    char **names;
    size_t count;
    size_t capacity;
} BKTagRegistry;

typedef struct BKTags {
    int player;
    int player_target;
    int cursor;
    int mob;
    int boss;
    int burning_knight;
    int player_save;
    int level_save;
    int bomb;
    int room;
    int teleport_trigger;
    int lock_tag;
    int must_be_killed;
    int teleport;
    int statue;
    int has_shadow;
    int mess;
    int checkpoint;
    int entrance;
    int hidden_entrance;
    int item;
    int shop_keeper;
    int npc;
    int button;
    int chest;
    int projectile;
    int mob_projectile;
    int player_projectile;
    int laser;
    int fire_particle;
    int text_particle;
    int door;
} BKTags;

void bk_tag_registry_init(BKTagRegistry *registry);
void bk_tag_registry_shutdown(BKTagRegistry *registry);
int bk_tag_registry_register(BKTagRegistry *registry, const char *name);
const char *bk_tag_registry_name(const BKTagRegistry *registry, int tag);

void bk_tags_init(BKTags *tags, BKTagRegistry *registry);

#endif
