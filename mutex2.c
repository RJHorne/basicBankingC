#include <stdio.h>
#include <pthread.h>
#include <time.h>

#define NUM_THREADS 20
#define ITERATIONS 1000000

// Shared resources
pthread_mutex_t counter_mutex;
int shared_counter = 0;
int enable = 0;

void* increment_counter(void* arg) {
    for (int i = 0; i < ITERATIONS; i++) {
       pthread_mutex_lock(&counter_mutex);
        shared_counter++;  // Protected operation
       pthread_mutex_unlock(&counter_mutex);
    }
    return NULL;
}

int main() {

    clock_t t; 
    t = clock();
    pthread_t threads[NUM_THREADS];
    int i = 0;
    while (i < 10)
    {
    // Initialize mutex
    if (pthread_mutex_init(&counter_mutex, NULL) != 0) {
        perror("Mutex initialization failed");
        return 1;
    }
    
        
    
    
    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, increment_counter, NULL) != 0) {
            perror("Thread creation failed");
            return 2;
        }
    }

    // Wait for completion
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Cleanup
    pthread_mutex_destroy(&counter_mutex);

    printf("Final counter value: %d (Expected: %d)\n", 
           shared_counter, NUM_THREADS * ITERATIONS);
           shared_counter = 0;
    i++;       
    }
    t = clock() - t; 
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds 

    printf("This took %f seconds to execute \n", time_taken); 
    return 0;
}
