/*
SYST44288 Operating Systems Design & Systems Programming
Term Project
Consumer-Producer solution implementing semaphores and mutex on buffer

Group Members:
Justin Crozier
Elie Nsair
Sean Ramsay

Date: Nov 17 2018
Edited: Nov 18 2018 Final Mastered 
*/

// ===== OS Project ===== //

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "buffer.h"
#include <unistd.h>

buffer_item buffer[BUFFER_SIZE];
int bufferAdd = 0;
int bufferRemove = 0;

// ===== Creating Mutex for Locking & Semaphore Variables =====
pthread_mutex_t mutex;
sem_t full, empty;

// ===== Insert Item Function =====
int insert_item(buffer_item item) {
    
    int status = -1;

    // If buffer is not full add item to buffer
    if (bufferAdd < BUFFER_SIZE) {
        
        // Lock before accessing C.S.
        pthread_mutex_lock(&mutex);
        // Decrement the empty semaphore
        sem_wait(&empty);
        
        // add item to buffer
        buffer[bufferAdd] = item;
        
        // create circular array behaviour (loops 0, 1, 2, 3, 4, 0...)
        bufferAdd++;
        bufferAdd = bufferAdd % BUFFER_SIZE;
        
        status = 0;
        
        // Unlock after accessing C.S.
        pthread_mutex_unlock(&mutex);
        // Increment the full semaphore
        sem_post(&full);
        
    } else {
        
        // ERROR: Buffer is full, notify user
        fprintf(stderr, "===== ERROR: Buffer Full ===== \n");
        status = -1;
        
    }

    return status;
    
}

// ===== Remove Item Function =====
int remove_item(buffer_item *item) {
    
    int status = -1;

    // if bufferRemove is in the buffers range then remove that item
    if (bufferRemove < BUFFER_SIZE && bufferRemove >= 0) {
        
        // Lock before entering C.S.
        pthread_mutex_lock(&mutex);
        // Decrement the full semaphore
        sem_wait(&full);
        
        // Place consumed value from buffer into item
        *item = buffer[bufferRemove];
        
        // Remove previous value and set it to 0 (act as deletion)
        buffer[bufferRemove] = 0;
        
        // Circular buffer behaviour
        bufferRemove++;
        bufferRemove = bufferRemove % BUFFER_SIZE;
        
        status = 0;
        
        // Unlock after accessing C.S.
        pthread_mutex_unlock(&mutex);
        // Increment the empty semaphore
        sem_post(&empty);
        
    } else {
        
        // ERROR: Buffer is full, notify user
        fprintf(stderr, "===== ERROR: Buffer Full ===== \n");
        status = -1;
        
    }
    
    return status;
}

// ===== Producer Thread =====
void *producer (void *arguments) {
    
    buffer_item item;
    
    while (1) {
        
        // Random sleepTime from 1 to (BUFFER_SIZE - 1)
        int sleepTime = (rand() % BUFFER_SIZE - 1) + 1;
        sleep(sleepTime);
        
        // Generate random number to produce
        item = rand();
        
        // Try to insert random number into buffer
        if (insert_item(item)) {
            fprintf(stderr, "ERROR: Could not add item to buffer\n");
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
        
        // Random sleepTime from 1 to (BUFFER_SIZE - 1)
        int sleepTime = (rand() % BUFFER_SIZE - 1) + 1;
        sleep(sleepTime);
        
        // Try to remove item from buffer
        if(remove_item(&item))
            fprintf(stderr, "ERROR\n");
        else {
            printf("consumer consumed %d\n", item);
        }
        
    }
}

int main(int argc, const char * argv[]) {
    
    // ===== Initializing Unnamed Semaphore =====
    pthread_mutex_init(&mutex, NULL);
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, BUFFER_SIZE);
    
    // Error Control: If users don't enter valid number of arguments prompt user
    if (argc != 4) {
        fprintf(stderr, "ERROR: usage --> %s sleepTime #Producers #Consumers\n", argv[0]);
        exit(1);
    }
    
    // Capturing Command Line Arguments
    int sleepTime = atoi(argv[1]);
    int numProducer = atoi(argv[2]);
    int numConsumer = atoi(argv[3]);
    
    // ===== Create Producer & Consumer Threads =====
    pthread_t producerThread;
    pthread_t consumerThread;
    
    for (int i = 0; i < numProducer; i++) {
        pthread_create(&producerThread, NULL, producer, NULL);
    }
    
    for (int i = 0; i < numConsumer; i++) {
        pthread_create(&consumerThread, NULL, consumer, NULL);
    }

    // Sleep for certain amount of time specified by user then exit
    sleep(sleepTime);
    exit(0);
   
}
