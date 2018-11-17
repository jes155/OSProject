/*
SYST44288 Operating Systems Design & Systems Programming
Term Project
Consumer-Producer solution implementing semaphores and mutex on buffer

Group Members:
Justin Crozier
Elie Nsair
Sean Ramsay

Date: Nov 17 2018
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

// ===== Insert Item Function =====
int insert_item(buffer_item item) {
    
    int status = -1;
    
    // If buffer is not full add item to buffer
    if (bufferAdd < BUFFER_SIZE) {
        
        // add item to buffer
        buffer[bufferAdd] = item;
        bufferAdd++;
        status = 0;
        
    } else {
        
        // ERROR: Buffer if full, notify user
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
        
        // Place value from buffer into item
        *item = buffer[bufferRemove];
        
        // Remove previous value and set it to 0 (act as deletion)
        buffer[bufferRemove] = 0;
        bufferRemove++;
        status = 0;

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
        
        // Random sleepTime from 1 to 9
        int sleepTime = rand() % 10;
        sleep(sleepTime);
        
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
        
        // Random sleepTime from 1 to 9
        int sleepTime = rand() % 10;
        sleep(sleepTime);
        
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
