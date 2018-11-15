// ===== OS Project ===== //

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "buffer.h"
#include <unistd.h>

buffer_item buffer[BUFFER_SIZE];

// ===== Insert Item Function =====
int insert_item(buffer_item item) {
    
    // insert item into buffer
    // return status 0 if successful
    // return status -1 if failed
    
    int status = 0;
    return status;
    
}

// ===== Remove Item Function =====
int remove_item(buffer_item *item) {
    // remove item from buffer
    // return status 0 if successful
    // return status -1 if failed
    int status = 0;
    return status;
}

// ===== Producer Thread =====
void *producer (void *arguments) {
    
    buffer_item item;
    
    while (1) {
        
        // Random sleepTime ... sleep
        // Generate random number to produce
        item = rand();
        if (insert_item(item)) {
            fprintf(stderr, "ERROR\n");
        }
        else {
            printf("producer produced %d\n", item);
        }
        
    }
    
}

// ===== Consumer Thread =====
void *consumer (void *arguments) {
    
    buffer_item item;
    
    while (1) {
        
        // Random sleepTime ... sleep
        if(remove_item(&item))
            fprintf(stderr, "ERROR\n");
        else {
            printf("consumer consumed %d\n", item);
        }
        
    }
}


int main(int argc, const char * argv[]) {
   
    // Capturing Command Line Arguments
    int sleepTime = atoi(argv[1]);
    int numProducer = atoi(argv[2]);
    int numConsumer = atoi(argv[3]);
    
    // ===== Create Producer & Consumer Threads =====
    pthread_t producerThread;
    pthread_t consumerThread;
    
    pthread_create(&producerThread, NULL, producer, NULL);
    pthread_create(&consumerThread, NULL, consumer, NULL);
    
    // Sleep for certain amount of time specified by user then exit
    sleep(sleepTime);
    exit(0);
}
