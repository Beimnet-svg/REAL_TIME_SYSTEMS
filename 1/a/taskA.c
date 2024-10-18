#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "func_ref.h"
void busy_wait(struct timespec t) {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    struct timespec then = timespec_add(now, t);

    while (timespec_cmp(now, then) < 0) {
        for (int i = 0; i < 10000; i++) {
        }
        clock_gettime(CLOCK_MONOTONIC,&now);
    }
}
int times_cmp(struct timespec t1, struct timespec t2) {
    return memcmp(&t1, &t2, sizeof(struct timespec));
}

void busy_wait_times(struct timespec t) {
    struct timespec now, then;
    clock_gettime(CLOCK_MONOTONIC, &now);
    then = timespec_add(now, t);

    while (times_cmp(now, then) < 0) {
        times(&now);
        //clock_gettime(CLOCK_MONOTONIC, &now);
    }
}

int main() {
    
    struct timespec delay = {1,0};//1 seconds
      //Using sleep()
      //printf("Using sleep():\n");
      //sleep(1);
    
    
      // Using busy_wait() with clock_gettime()
    //printf("Using busy_wait() with clock_gettime():\n");
    //busy_wait(delay);
    
    // Using busy_wait() with times()
    printf("Using busy_wait() with times():\n");
    busy_wait_times(delay);

    return 0;
}

