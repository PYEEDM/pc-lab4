/*
This code is taken from the course's Brightspace page:
https://brightspace.ru.nl/d2l/le/content/163516/viewContent/1202763/View 
*/

#include "job-time.h"

#include <stdio.h>
#include <time.h>
#include <inttypes.h>

struct timespec time_gettime ()
{
    struct timespec s;
    clock_gettime(CLOCK_REALTIME, &s);
    return s;
}

int64_t time_elapsed (struct timespec a, struct timespec b)
{
    return ((int64_t)a.tv_sec - b.tv_sec) * 1000000 + ((int64_t)a.tv_nsec - b.tv_nsec) / 1000LL;
}

void time_print_elapsed (const char *id, struct timespec start, struct timespec finish)
{
    (void) fprintf (stdout, "--- [%s] time elapsed %03li ms\n", id, time_elapsed (finish, start)/1000);
}