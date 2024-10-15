//TASK A B
/*
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_ITERATION 1000000
 
 //Global variable initialized to 0
 long Global = 0;
 
 //Barrier forsynchronizing thread start
 pthread_barrier_t barrier;
 
    void* fn(void* args){
		long local = 0;
		
		//Wait for both thread to reach the barrier
		pthread_barrier_wait(&barrier);
		
		//Increment the counters in a loop
		for(int i = 0; i< 1000000; i++)
		{
			local++;studen
			Global++;
		}
		printf("local: %ld\n\r", local);
		printf("GLOBAL: %ld\n\r", Global);
        return NULL;
    }
    

    int main(){	
	pthread_t thread1, thread2;
	
	//Initialize the barrier for 2 threads
	pthread_barrier_init(&barrier, NULL, 2);
	
	//Create the first thread
	pthread_create(&thread1, NULL, fn, NULL);
	
	//Create the second thread
	pthread_create(&thread2, NULL, fn, NULL);
	
	//Wait for both threads to complete
	pthread_join(thread1, NULL);
	pthread_join(thread1, NULL);
	
	//Destroy the barrier
	pthread_barrier_destroy(&barrier);
	
	//Final output of the global variable
	printf("\n\rGLOBAL:%ld\n\r", Global);
}*/

//TASK C
/*
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#define NUM_ITERATION 1000000
 
 //Global variable initialized to 0
 long Global = 0;
 
 //Barrier forsynchronizing thread start
 pthread_barrier_t barrier;
 
 //Semaphore for synchronization
 sem_t semaphore;
 
    void* fn(void* args){
		long local = 0;
		
		//Wait for both thread to reach the barrier
		pthread_barrier_wait(&barrier);
		
		//Increment the counters in a loop
		for(int i = 0; i< 1000000; i++)
		{
			local++;
			sem_wait(&semaphore); //Decrements the semaphore(wait)
			Global++;
			sem_post(&semaphore); //Increments the semaphore(Signal)
			
		}
		printf("local: %ld\n\r", local);
		//printf("GLOBAL: %ld\n\r", Global);
        return NULL;
    }

    int main(){	
	pthread_t thread1, thread2;
	
	//Initialize the barrier for 2 threads
	pthread_barrier_init(&barrier, NULL, 2);
	
	//Initialize the semaphore
	sem_init(&semaphore, 0, 1);
	
	//Create the first thread
	pthread_create(&thread1, NULL, fn, NULL);
	
	//Create the second thread
	pthread_create(&thread2, NULL, fn, NULL);
	
	//Wait for both threads to complete
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	
	//Destroy the barrier
	pthread_barrier_destroy(&barrier);
	sem_destroy(&semaphore);
	
	//Final output of the global variable
	printf("\n\rGLOBAL:%ld\n\r", Global);
}*/

//TASK D



