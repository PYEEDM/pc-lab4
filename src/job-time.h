#ifndef FOO_H_
#define FOO_H_

#include <time.h>
#include <inttypes.h>

struct timespec time_gettime ();

int64_t time_elapsed (struct timespec a, struct timespec b);

void time_print_elapsed (const char *id, struct timespec start, struct timespec finish);

#endif