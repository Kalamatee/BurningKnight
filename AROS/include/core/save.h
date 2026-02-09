#ifndef BK_AROS_SAVE_H
#define BK_AROS_SAVE_H

#include <stddef.h>

typedef struct BKSaveEntry {
    char key[64];
    char value[128];
} BKSaveEntry;

typedef struct BKSaveStore {
    BKSaveEntry *entries;
    size_t count;
    size_t capacity;
} BKSaveStore;

void bk_save_init(BKSaveStore *store);
void bk_save_clear(BKSaveStore *store);
int bk_save_load(BKSaveStore *store, const char *path);
int bk_save_flush(const BKSaveStore *store, const char *path);

const char *bk_save_get(const BKSaveStore *store, const char *key);
int bk_save_get_bool(const BKSaveStore *store, const char *key, int fallback);
int bk_save_get_int(const BKSaveStore *store, const char *key, int fallback);
float bk_save_get_float(const BKSaveStore *store, const char *key, float fallback);

void bk_save_put(BKSaveStore *store, const char *key, const char *value);
void bk_save_put_bool(BKSaveStore *store, const char *key, int value);
void bk_save_put_int(BKSaveStore *store, const char *key, int value);
void bk_save_put_float(BKSaveStore *store, const char *key, float value);

#endif
