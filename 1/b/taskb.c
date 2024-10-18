#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>
#include <stdint.h>
#include <sched.h>

#define NUM_ITERATIONS 10000000

uint64_t rdtsc(void) {
    uint64_t val;
    asm volatile("isb; mrs %0, cntvct_el0; isb; " : "=r"(val) :: "memory");
    return val;
}

void estimate_latency_and_resolution(const char *timer_name, void (*timer_func)(struct timespec *)) {
    struct timespec start, end, wastevar;
    int ns_max = 50;
    int histogram[ns_max];
    int ns_final;

    memset(histogram, 0, sizeof(int) * ns_max);

    // Estimate access latency
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        timer_func(&wastevar);
        //times(&timenow)
        //clock_gettime...
        //rdtsc
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    double latency = (double)(end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / NUM_ITERATIONS;
    if(latency<0)
        {
            latency= -latency;
        }
    // Estimate resolution
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        timer_func(&start);
        sched_yield();
        timer_func(&end);
        int ns = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/ NUM_ITERATIONS;;
        if(ns<0)
        {
            ns= -ns;
        }
        ns_final = ns;
    
        if (ns >= 0 && ns < ns_max) {
            histogram[ns]++;
        }
    }

    printf("%s:\n", timer_name);
    printf("Access latency: %.2f ns\n", latency);
    for (int i = 0; i < ns_max; i++) {
        
        //printf("%d\n", histogram[i]);
    }
    printf("Resolution: %d ns\n", ns_final);
}

int main() {
    estimate_latency_and_resolution("rdtsc", rdtsc);
    estimate_latency_and_resolution("clock_gettime", clock_gettime);
    estimate_latency_and_resolution("times", times);
    printf("\n\n");
    //estimate_latency_and_resolution("sched_yield", sched_yield);

    return 0;
}
