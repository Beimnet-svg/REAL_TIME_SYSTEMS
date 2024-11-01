//TASK D PERIOD POLLING(0.5ms) WITH DISTURBANCE
/*#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sched.h>
#include <wiringPi.h>

#define PININ1  8
#define PINOUT1 9
#define PININ2  7
#define PINOUT2 15
#define PININ3  16
#define PINOUT3 1

// GPIO Initialization
void io_init(void) {
    wiringPiSetup();
    
    pinMode(PININ1, INPUT); pinMode(PINOUT1, OUTPUT); 
    pinMode(PININ2, INPUT); pinMode(PINOUT2, OUTPUT); 
    pinMode(PININ3, INPUT); pinMode(PINOUT3, OUTPUT); 
    
    // Initialize outputs to HIGH
    digitalWrite(PINOUT1, HIGH);
    digitalWrite(PINOUT2, HIGH);
    digitalWrite(PINOUT3, HIGH);
}

// Function to make a thread sleep for a specified duration in nanoseconds
void sleep_realtime(long nanoseconds) {
    struct timespec ts;
    ts.tv_sec = nanoseconds / 1e9;
    ts.tv_nsec = nanoseconds % (long)1e9;
    clock_nanosleep(CLOCK_MONOTONIC, 0, &ts, NULL);
}

// Function to set CPU affinity to a specific core
int set_cpu(int cpu_id) {
    int num_cores = sysconf(_SC_NPROCESSORS_ONLN);
    if (cpu_id < 0 || cpu_id >= num_cores)
        return -1;

    cpu_set_t cpu;
    CPU_ZERO(&cpu);
    CPU_SET(cpu_id, &cpu);
    return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu);
}

// Periodic task for polling and responding
void* periodic_task(void* arg) {
    int pin_in = *(int*)arg;
    int pin_out = pin_in == PININ1 ? PINOUT1 : (pin_in == PININ2 ? PINOUT2 : PINOUT3);

    set_cpu(0);  // Bind thread to CPU core 0
    while (1) {
        // Poll the input pin every 2ms
        if (digitalRead(pin_in) == LOW) {
            digitalWrite(pin_out, LOW);  // Set response signal
        } else {
            digitalWrite(pin_out, HIGH); // Reset response signal
        }
        sleep_realtime(0.5 * 1e6);  // Sleep for 0.5 milliseconds
    }
    return NULL;
}

void* disturbance_thread(void* arg) {
    set_cpu(0);  // Bind this thread to CPU core 0
    while (1) {
        asm volatile("" ::: "memory");  // Simple infinite loop to create CPU load
    }
    return NULL;
}

int main() {
    // Create periodic threads
    pthread_t thread_A, thread_B, thread_C;
    io_init();
    int pin_in1 = PININ1, pin_in2 = PININ2, pin_in3 = PININ3;
    pthread_create(&thread_A, NULL, periodic_task, &pin_in1);
    pthread_create(&thread_B, NULL, periodic_task, &pin_in2);
    pthread_create(&thread_C, NULL, periodic_task, &pin_in3);

    // Create 10 disturbance threads
    pthread_t disturbance_threads[10];
    for (int i = 0; i < 10; i++) {
        pthread_create(&disturbance_threads[i], NULL, disturbance_thread, NULL);
    }
    // Join threads (optional)
    pthread_join(thread_A, NULL);
    pthread_join(thread_B, NULL);
    pthread_join(thread_C, NULL);
    for (int i = 0; i < 10; i++) {
        pthread_join(disturbance_threads[i], NULL);
    }


    return 0;
}*/


//TASK D SHORTER PERIOD POLLING(0.5ms) WITHOUT DISTURBANCE
/*#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sched.h>
#include <wiringPi.h>

#define PININ1  8
#define PINOUT1 9
#define PININ2  7
#define PINOUT2 15
#define PININ3  16
#define PINOUT3 1

// GPIO Initialization
void io_init(void) {
    wiringPiSetup();
    
    pinMode(PININ1, INPUT); pinMode(PINOUT1, OUTPUT); 
    pinMode(PININ2, INPUT); pinMode(PINOUT2, OUTPUT); 
    pinMode(PININ3, INPUT); pinMode(PINOUT3, OUTPUT); 
    
    // Initialize outputs to HIGH
    digitalWrite(PINOUT1, HIGH);
    digitalWrite(PINOUT2, HIGH);
    digitalWrite(PINOUT3, HIGH);
}

// Function to make a thread sleep for a specified duration in nanoseconds
void sleep_realtime(long nanoseconds) {
    struct timespec ts;
    ts.tv_sec = nanoseconds / 1e9;
    ts.tv_nsec = nanoseconds % (long)1e9;
    clock_nanosleep(CLOCK_MONOTONIC, 0, &ts, NULL);
}

// Function to set CPU affinity to a specific core
int set_cpu(int cpu_id) {
    int num_cores = sysconf(_SC_NPROCESSORS_ONLN);
    if (cpu_id < 0 || cpu_id >= num_cores)
        return -1;

    cpu_set_t cpu;
    CPU_ZERO(&cpu);
    CPU_SET(cpu_id, &cpu);
    return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu);
}

// Periodic task for polling and responding
void* periodic_task(void* arg) {
    int pin_in = *(int*)arg;
    int pin_out = pin_in == PININ1 ? PINOUT1 : (pin_in == PININ2 ? PINOUT2 : PINOUT3);

    set_cpu(0);  // Bind thread to CPU core 0
    while (1) {
        // Poll the input pin every 0.5ms
        if (digitalRead(pin_in) == LOW) {
            digitalWrite(pin_out, LOW);  // Set response signal
        } else {
            digitalWrite(pin_out, HIGH); // Reset response signal
        }
        sleep_realtime(0.5 * 1e6);  // Sleep for 2 milliseconds
    }
    return NULL;
}

int main() {
    // Create periodic threads
    pthread_t thread_A, thread_B, thread_C;
    io_init();
    int pin_in1 = PININ1, pin_in2 = PININ2, pin_in3 = PININ3;
    pthread_create(&thread_A, NULL, periodic_task, &pin_in1);
    pthread_create(&thread_B, NULL, periodic_task, &pin_in2);
    pthread_create(&thread_C, NULL, periodic_task, &pin_in3);

    // Join threads (optional, keeps the main program running)
    pthread_join(thread_A, NULL);
    pthread_join(thread_B, NULL);
    pthread_join(thread_C, NULL);

    return 0;
}*/



//TASK D PERIOD POLLING(2ms) WITH DISTURBANCE
#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sched.h>
#include <wiringPi.h>

#define PININ1  8
#define PINOUT1 9
#define PININ2  7
#define PINOUT2 15
#define PININ3  16
#define PINOUT3 1

// GPIO Initialization
void io_init(void) {
    wiringPiSetup();
    
    pinMode(PININ1, INPUT); pinMode(PINOUT1, OUTPUT); 
    pinMode(PININ2, INPUT); pinMode(PINOUT2, OUTPUT); 
    pinMode(PININ3, INPUT); pinMode(PINOUT3, OUTPUT); 
    
    // Initialize outputs to HIGH
    digitalWrite(PINOUT1, HIGH);
    digitalWrite(PINOUT2, HIGH);
    digitalWrite(PINOUT3, HIGH);
}

// Function to make a thread sleep for a specified duration in nanoseconds
void sleep_realtime(long nanoseconds) {
    struct timespec ts;
    ts.tv_sec = nanoseconds / 1e9;
    ts.tv_nsec = nanoseconds % (long)1e9;
    clock_nanosleep(CLOCK_MONOTONIC, 0, &ts, NULL);
}

// Function to set CPU affinity to a specific core
int set_cpu(int cpu_id) {
    int num_cores = sysconf(_SC_NPROCESSORS_ONLN);
    if (cpu_id < 0 || cpu_id >= num_cores)
        return -1;

    cpu_set_t cpu;
    CPU_ZERO(&cpu);
    CPU_SET(cpu_id, &cpu);
    return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu);
}

// Periodic task for polling and responding
void* periodic_task(void* arg) {
    int pin_in = *(int*)arg;
    int pin_out = pin_in == PININ1 ? PINOUT1 : (pin_in == PININ2 ? PINOUT2 : PINOUT3);

    set_cpu(0);  // Bind thread to CPU core 0
    while (1) {
        // Poll the input pin every 2ms
        if (digitalRead(pin_in) == LOW) {
            digitalWrite(pin_out, LOW);  // Set response signal
        } else {
            digitalWrite(pin_out, HIGH); // Reset response signal
        }
        sleep_realtime(2000*1000);  // Sleep for 2 milliseconds
    }
    return NULL;
}

void* disturbance_thread(void* arg) {
    set_cpu(0);  // Bind this thread to CPU core 0
    while (1) {
        asm volatile("" ::: "memory");  // Simple infinite loop to create CPU load
    }
    return NULL;
}

int main() {
    // Create periodic threads
    pthread_t thread_A, thread_B, thread_C;
    io_init();
    int pin_in1 = PININ1, pin_in2 = PININ2, pin_in3 = PININ3;
    pthread_create(&thread_A, NULL, periodic_task, &pin_in1);
    pthread_create(&thread_B, NULL, periodic_task, &pin_in2);
    pthread_create(&thread_C, NULL, periodic_task, &pin_in3);

    // Create 10 disturbance threads
    pthread_t disturbance_threads[100];
    for (int i = 0; i < 100; i++) {
        pthread_create(&disturbance_threads[i], NULL, disturbance_thread, NULL);
    }

    // Join threads (optional)
    pthread_join(thread_A, NULL);
    pthread_join(thread_B, NULL);
    pthread_join(thread_C, NULL);
    for (int i = 0; i < 100; i++) {
        pthread_join(disturbance_threads[i], NULL);
    }


    return 0;
}




//TASK D PERIOD POLLING(2ms) WITHOUT DISTURBANCE
/*#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sched.h>
#include <wiringPi.h>

#define PININ1  8
#define PINOUT1 9
#define PININ2  7
#define PINOUT2 15
#define PININ3  16
#define PINOUT3 1

// GPIO Initialization
void io_init(void) {
    wiringPiSetup();
    
    pinMode(PININ1, INPUT); pinMode(PINOUT1, OUTPUT); 
    pinMode(PININ2, INPUT); pinMode(PINOUT2, OUTPUT); 
    pinMode(PININ3, INPUT); pinMode(PINOUT3, OUTPUT); 
    
    // Initialize outputs to HIGH
    digitalWrite(PINOUT1, HIGH);
    digitalWrite(PINOUT2, HIGH);
    digitalWrite(PINOUT3, HIGH);
}

// Function to make a thread sleep for a specified duration in nanoseconds
void sleep_realtime(long nanoseconds) {
    struct timespec ts;
    ts.tv_sec = nanoseconds / 1e9;
    ts.tv_nsec = nanoseconds % (long)1e9;
    clock_nanosleep(CLOCK_MONOTONIC, 0, &ts, NULL);
}

// Function to set CPU affinity to a specific core
int set_cpu(int cpu_id) {
    int num_cores = sysconf(_SC_NPROCESSORS_ONLN);
    if (cpu_id < 0 || cpu_id >= num_cores)
        return -1;

    cpu_set_t cpu;
    CPU_ZERO(&cpu);
    CPU_SET(cpu_id, &cpu);
    return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu);
}

// Periodic task for polling and responding
void* periodic_task(void* arg) {
    int pin_in = *(int*)arg;
    int pin_out = pin_in == PININ1 ? PINOUT1 : (pin_in == PININ2 ? PINOUT2 : PINOUT3);

    set_cpu(1);  // Bind thread to CPU core 1
    while (1) {
        // Poll the input pin every 2ms
        if (digitalRead(pin_in) == LOW) {
            digitalWrite(pin_out, LOW);  // Set response signal
        } else {
            digitalWrite(pin_out, HIGH); // Reset response signal
        }
        sleep_realtime(2 * 1e6);  // Sleep for 2 milliseconds
    }
    return NULL;
}

int main() {
    // Create periodic threads
    pthread_t thread_A, thread_B, thread_C;
    io_init();
    int pin_in1 = PININ1, pin_in2 = PININ2, pin_in3 = PININ3;
    pthread_create(&thread_A, NULL, periodic_task, &pin_in1);
    pthread_create(&thread_B, NULL, periodic_task, &pin_in2);
    pthread_create(&thread_C, NULL, periodic_task, &pin_in3);

    // Join threads (optional, keeps the main program running)
    pthread_join(thread_A, NULL);
    pthread_join(thread_B, NULL);
    pthread_join(thread_C, NULL);

    return 0;
}*/



//PERIODICALLY RUNNING THREAD
/*#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

// Function to make a thread sleep for a specified duration in nanoseconds
void sleep_realtime(long nanoseconds) {
    struct timespec ts;
    ts.tv_sec = nanoseconds / 1e9;
    ts.tv_nsec = nanoseconds % (long)1e9;
    clock_nanosleep(CLOCK_MONOTONIC, 0, &ts, NULL);
}

// Periodic thread function
void* periodic_thread(void* arg) {
    while (1) {
        printf("Periodic message every 500 ms\n");
        //fflush(stdout);  // Ensure the message is printed immediately
        sleep_realtime(500 * 1e6);  // Sleep for 500 milliseconds (500 million nanoseconds)
    }
    return NULL;
}

int main() {
    pthread_t thread;

    // Create the periodic thread
    pthread_create(&thread, NULL, periodic_thread, NULL);

    // Join the thread (optional, keeps the main program running)
    pthread_join(thread, NULL);

    return 0;
}*/


//TASK C
/*#define _GNU_SOURCE
#include <pthread.h>
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <errno.h>

#define PININ1  8
#define PINOUT1 9
#define PININ2  7
#define PINOUT2 15
#define PININ3  16
#define PINOUT3 1

// Thread function prototypes
void* test_thread_A(void* arg);
void* test_thread_B(void* arg);
void* test_thread_C(void* arg);
void* disturbance_thread(void* arg);

// GPIO Initialization
void io_init(void) {
    wiringPiSetup();
    
    pinMode(PININ1, INPUT); pinMode(PINOUT1, OUTPUT); 
    pinMode(PININ2, INPUT); pinMode(PINOUT2, OUTPUT); 
    pinMode(PININ3, INPUT); pinMode(PINOUT3, OUTPUT); 
    
    // Initialize outputs to HIGH
    digitalWrite(PINOUT1, HIGH);
    digitalWrite(PINOUT2, HIGH);
    digitalWrite(PINOUT3, HIGH);
}

// Function to set CPU affinity to a specific core
int set_cpu(int cpu_id) {
    int num_cores = sysconf(_SC_NPROCESSORS_ONLN);
    if (cpu_id < 0 || cpu_id >= num_cores)
        return EINVAL;

    cpu_set_t cpu;
    CPU_ZERO(&cpu);
    CPU_SET(cpu_id, &cpu);
    return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu);
}

// Test thread A
void* test_thread_A(void* arg) {
    set_cpu(1);
    while (1) {
        //usleep(1000);  // Small delay
        if (digitalRead(PININ1) == LOW) {
            digitalWrite(PINOUT1, LOW);  // Send response
        } else {
            digitalWrite(PINOUT1, HIGH); // Reset response
        }
    }
    return NULL;
}

// Test thread B
void* test_thread_B(void* arg) {
    set_cpu(1);
    while (1) {
      //  usleep(1000);  // Small delay
        if (digitalRead(PININ2) == LOW) {
            digitalWrite(PINOUT2, LOW);  // Send response
        } else {
            digitalWrite(PINOUT2, HIGH); // Reset response
        }
    }
    return NULL;
}

// Test thread C
void* test_thread_C(void* arg) {
    set_cpu(1);
    while (1) {
       // usleep(1000);  // Small delay
        if (digitalRead(PININ3) == LOW) {
            digitalWrite(PINOUT3, LOW);  // Send response
        } else {
            digitalWrite(PINOUT3, HIGH); // Reset response
        }
    }
    return NULL;
}

// Disturbance thread
void* disturbance_thread(void* arg) {
    set_cpu(1);  //Assign the disturbance thread to CPU 0
    while (1) {
        asm volatile("" ::: "memory");  // Simple infinite loop to cause CPU load
    }
    return NULL;
}

int main() {
    pthread_t thread_A, thread_B, thread_C;
    pthread_t disturbance_threads[10];

    // Initialize GPIO pins
    io_init();

    // Create threads for each test
    pthread_create(&thread_A, NULL, test_thread_A, NULL);
    pthread_create(&thread_B, NULL, test_thread_B, NULL);
    pthread_create(&thread_C, NULL, test_thread_C, NULL);

    // Create 10 disturbance threads
    for (int i = 0; i < 10; i++) {
        pthread_create(&disturbance_threads[i], NULL, disturbance_thread, NULL);
    }

    // Join test threads
    pthread_join(thread_A, NULL);
    pthread_join(thread_B, NULL);
    pthread_join(thread_C, NULL);

    // Join disturbance threads (optional, as these run indefinitely)
    for (int i = 0; i < 10; i++) {
        pthread_join(disturbance_threads[i], NULL);
    }

    return 0;
}*/

//TASK B
/*
#define _GNU_SOURCE 
#include <pthread.h>
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <errno.h>

#define PININ1  8
#define PINOUT1 9
#define PININ2  7
#define PINOUT2 15
#define PININ3  16
#define PINOUT3 1

// Thread function prototypes
void* test_thread_A(void* arg);
void* test_thread_B(void* arg);
void* test_thread_C(void* arg);

// GPIO Initialization
void io_init(void) {
    wiringPiSetup();
    
    pinMode(PININ1, INPUT); pinMode(PINOUT1, OUTPUT); 
    pinMode(PININ2, INPUT); pinMode(PINOUT2, OUTPUT); 
    pinMode(PININ3, INPUT); pinMode(PINOUT3, OUTPUT); 
    
    // Initialize outputs to HIGH
    digitalWrite(PINOUT1, HIGH);
    digitalWrite(PINOUT2, HIGH);
    digitalWrite(PINOUT3, HIGH);
}

//SELECT CPU CORE
int set_cpu(int cpu_id)
{
    int num_cores = sysconf(_SC_NPROCESSORS_ONLN);
    if (cpu_id < 0 || cpu_id >= num_cores)
        return EINVAL;

    cpu_set_t cpu;
    CPU_ZERO(&cpu);
    CPU_SET(cpu_id, &cpu);
    return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu);
}

// Test thread A
void* test_thread_A(void* arg) {
    set_cpu(1);
    while (1) {
        // Wait for input signal on PININ1
       // usleep(1000);
        //busy_delay_ms(1000);
        if(digitalRead(PININ1) == LOW) {
            // Send response signal on PINOUT1
            digitalWrite(PINOUT1, LOW);  // Send response
        }
        else{
            digitalWrite(PINOUT1, HIGH); // Reset response
        }
    }
    return NULL;
}

// Test thread B
void* test_thread_B(void* arg) {
    set_cpu(1);
    while (1) {
       // usleep(1000);
        if (digitalRead(PININ2) == LOW) {
            // Send response signal on PINOUT2
            digitalWrite(PINOUT2, LOW);  // Send response
        }
        else{
            digitalWrite(PINOUT2, HIGH); // Reset response
        }
    }
    return NULL;
}

// Test thread C
void* test_thread_C(void* arg) {
    set_cpu(1);
    while (1) {
       //usleep(1000);
        if (digitalRead(PININ3) == LOW) {
            // Send response signal on PINOUT3
            digitalWrite(PINOUT3, LOW);  // Send response
          }
          else{
            digitalWrite(PINOUT3, HIGH); // Reset response
        }
    }
    return NULL;
}

int main() {
    pthread_t thread_A, thread_B, thread_C;

    // Initialize GPIO pins
    io_init();

    // Create threads for each test
    pthread_create(&thread_A, NULL, test_thread_A, NULL);
    pthread_create(&thread_B, NULL, test_thread_B, NULL);
    pthread_create(&thread_C, NULL, test_thread_C, NULL);

    // Join threads
    pthread_join(thread_A, NULL);
    pthread_join(thread_B, NULL);
    pthread_join(thread_C, NULL);

    return 0;
}*/










 //TASK A
 /*
#include <pthread.h>
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PININ1  8
#define PINOUT1 9
#define PININ2  7
#define PINOUT2 15
#define PININ3  16
#define PINOUT3 1

// Thread function prototypes
void* test_thread_A(void* arg);
void* test_thread_B(void* arg);
void* test_thread_C(void* arg);

// GPIO Initialization
void io_init(void) {
    wiringPiSetup();
    
    pinMode(PININ1, INPUT); pinMode(PINOUT1, OUTPUT); 
    pinMode(PININ2, INPUT); pinMode(PINOUT2, OUTPUT); 
    pinMode(PININ3, INPUT); pinMode(PINOUT3, OUTPUT); 
    
    // Initialize outputs to HIGH
    digitalWrite(PINOUT1, HIGH);
    digitalWrite(PINOUT2, HIGH);
    digitalWrite(PINOUT3, HIGH);
}

// Test thread A
void* test_thread_A(void* arg) {
    while (1) {
        // Wait for input signal on PININ1
       // usleep(1000);
        //busy_delay_ms(1000);
        if(digitalRead(PININ1) == LOW) {
            // Send response signal on PINOUT1
            digitalWrite(PINOUT1, LOW);  // Send response
        }
        else{
            digitalWrite(PINOUT1, HIGH); // Reset response
        }
    }
    return NULL;
}

// Test thread B
void* test_thread_B(void* arg) {
    while (1) {
       // usleep(1000);
        if (digitalRead(PININ2) == LOW) {
            // Send response signal on PINOUT2
            digitalWrite(PINOUT2, LOW);  // Send response
        }
        else{
            digitalWrite(PINOUT2, HIGH); // Reset response
        }
    }
    return NULL;
}

// Test thread C
void* test_thread_C(void* arg) {
    while (1) {
       // usleep(1000);
        if (digitalRead(PININ3) == LOW) {
            // Send response signal on PINOUT3
            digitalWrite(PINOUT3, LOW);  // Send response
        }
        else{
            digitalWrite(PINOUT3, HIGH); // Reset response
        }
    }
    return NULL;
}


int main() {
    pthread_t thread_A, thread_B, thread_C;

    // Initialize GPIO pins
    io_init();

    // Create threads for each test
    pthread_create(&thread_A, NULL, test_thread_A, NULL);
    pthread_create(&thread_B, NULL, test_thread_B, NULL);
    pthread_create(&thread_C, NULL, test_thread_C, NULL);

    // Join threads
    pthread_join(thread_A, NULL);
    pthread_join(thread_B, NULL);
    pthread_join(thread_C, NULL);

    return 0;
}*/



