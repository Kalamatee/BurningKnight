#include "util/log.h"

#include <stdarg.h>
#include <stdio.h>

static void bk_log_write(FILE *stream, const char *label, const char *fmt, va_list args) {
    fprintf(stream, "[%s] ", label);
    vfprintf(stream, fmt, args);
    fprintf(stream, "\n");
}

void bk_log_info(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    bk_log_write(stdout, "INFO", fmt, args);
    va_end(args);
}

void bk_log_error(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    bk_log_write(stderr, "ERROR", fmt, args);
    va_end(args);
}
