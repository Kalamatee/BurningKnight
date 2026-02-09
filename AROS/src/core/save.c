#include "core/save.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util/log.h"

static void bk_save_reserve(BKSaveStore *store, size_t capacity) {
    if (store->capacity >= capacity) {
        return;
    }

    size_t new_capacity = store->capacity == 0 ? 16 : store->capacity * 2;
    while (new_capacity < capacity) {
        new_capacity *= 2;
    }

    BKSaveEntry *entries = realloc(store->entries, new_capacity * sizeof(BKSaveEntry));
    if (!entries) {
        bk_log_error("Failed to grow save store");
        return;
    }

    store->entries = entries;
    store->capacity = new_capacity;
}

void bk_save_init(BKSaveStore *store) {
    store->entries = NULL;
    store->count = 0;
    store->capacity = 0;
}

void bk_save_clear(BKSaveStore *store) {
    free(store->entries);
    store->entries = NULL;
    store->count = 0;
    store->capacity = 0;
}

static void bk_save_trim(char *text) {
    char *start = text;
    while (*start && isspace((unsigned char)*start)) {
        start++;
    }

    char *end = start + strlen(start);
    while (end > start && isspace((unsigned char)*(end - 1))) {
        end--;
    }

    size_t length = (size_t)(end - start);
    memmove(text, start, length);
    text[length] = '\0';
}

static BKSaveEntry *bk_save_find(BKSaveStore *store, const char *key) {
    for (size_t i = 0; i < store->count; i++) {
        if (strcmp(store->entries[i].key, key) == 0) {
            return &store->entries[i];
        }
    }
    return NULL;
}

int bk_save_load(BKSaveStore *store, const char *path) {
    FILE *file = fopen(path, "r");
    if (!file) {
        return -1;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char *equals = strchr(line, '=');
        if (!equals) {
            continue;
        }

        *equals = '\0';
        char *key = line;
        char *value = equals + 1;

        bk_save_trim(key);
        bk_save_trim(value);

        if (key[0] == '\0') {
            continue;
        }

        bk_save_put(store, key, value);
    }

    fclose(file);
    return 0;
}

int bk_save_flush(const BKSaveStore *store, const char *path) {
    FILE *file = fopen(path, "w");
    if (!file) {
        return -1;
    }

    for (size_t i = 0; i < store->count; i++) {
        fprintf(file, "%s=%s\n", store->entries[i].key, store->entries[i].value);
    }

    fclose(file);
    return 0;
}

const char *bk_save_get(const BKSaveStore *store, const char *key) {
    for (size_t i = 0; i < store->count; i++) {
        if (strcmp(store->entries[i].key, key) == 0) {
            return store->entries[i].value;
        }
    }
    return NULL;
}

int bk_save_get_bool(const BKSaveStore *store, const char *key, int fallback) {
    const char *value = bk_save_get(store, key);
    if (!value) {
        return fallback;
    }
    return strcmp(value, "true") == 0 || strcmp(value, "1") == 0;
}

int bk_save_get_int(const BKSaveStore *store, const char *key, int fallback) {
    const char *value = bk_save_get(store, key);
    if (!value) {
        return fallback;
    }
    return atoi(value);
}

float bk_save_get_float(const BKSaveStore *store, const char *key, float fallback) {
    const char *value = bk_save_get(store, key);
    if (!value) {
        return fallback;
    }
    return (float)atof(value);
}

void bk_save_put(BKSaveStore *store, const char *key, const char *value) {
    BKSaveEntry *entry = bk_save_find(store, key);
    if (!entry) {
        bk_save_reserve(store, store->count + 1);
        entry = &store->entries[store->count++];
        strncpy(entry->key, key, sizeof(entry->key) - 1);
        entry->key[sizeof(entry->key) - 1] = '\0';
    }

    strncpy(entry->value, value, sizeof(entry->value) - 1);
    entry->value[sizeof(entry->value) - 1] = '\0';
}

void bk_save_put_bool(BKSaveStore *store, const char *key, int value) {
    bk_save_put(store, key, value ? "true" : "false");
}

void bk_save_put_int(BKSaveStore *store, const char *key, int value) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%d", value);
    bk_save_put(store, key, buffer);
}

void bk_save_put_float(BKSaveStore *store, const char *key, float value) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%.3f", value);
    bk_save_put(store, key, buffer);
}
