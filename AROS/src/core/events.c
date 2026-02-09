#include "core/events.h"

#include <time.h>

void bk_events_init(BKEvents *events) {
    time_t now = time(NULL);
    struct tm *local = localtime(&now);

    if (!local) {
        events->xmas = 0;
        events->halloween = 0;
        return;
    }

    events->xmas = (local->tm_mon == 11) && (local->tm_mday < 26);
    events->halloween = (local->tm_mon == 9);
}
