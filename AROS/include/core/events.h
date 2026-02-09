#ifndef BK_AROS_EVENTS_H
#define BK_AROS_EVENTS_H

typedef struct BKEvents {
    int xmas;
    int halloween;
} BKEvents;

void bk_events_init(BKEvents *events);

#endif
