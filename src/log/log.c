#include <stdio.h>
#include <stdarg.h>

#include "../globals.h"

void log_debug(const char* format, ...) {
	if (!DEBUG) {
		return;
	}

	va_list args;
	va_start(args, format);
	printf(CYAN " [DEBUG] " RESET);
	vprintf(format, args);
	va_end(args);
}
