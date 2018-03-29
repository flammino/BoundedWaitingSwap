/**
 * Adam Flammino
 * SDEV 385
 * This was written and tested on Debian 9.4
 */

#include <stdio.h>
#include<stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define THREADS 3

int lock = 0;
int x;
int y;
int result1;
int result2;
int result3;
int totalLoops = 3000;
int totalLoopsCompleted = 0;

int waiting [THREADS];

int compareAndSwap(int *lock, int expected, int newValue){
    int temp = *lock;
    if (*lock == expected){ // swaps to allow for locking
        *lock = newValue;
    }
    return temp; // returns value of lock when originally called
}



void * first(){
    int i = 0;
    for(totalLoopsCompleted; totalLoopsCompleted < totalLoops; totalLoopsCompleted++) {
        i++;
        printf("\nThread one starting for the %i time\n", i);
        while (compareAndSwap(&lock, 0, 1) == 0) // spinlock
        x = 5;
        y = 2;
        result1 = x + y;
        lock = 0; // Unlocks
        printf("Thread 1 thinks 5 + 2 = %i\n", result1);
        usleep(rand() % 100);
    }
    result1 = i;
}

void * second(){
    int i = 0;
    for(totalLoopsCompleted; totalLoopsCompleted < totalLoops; totalLoopsCompleted++) {
        i++;
        printf("\nThread two starting for the %i time\n", i);
        while (compareAndSwap(&lock, 0, 1) == 0) // spinlock
        x = 3;
        y = 7;
        result2 = x - y;
        lock = 0; // Unlocks
        printf("Thread 2 thinks 3 - 7 = %i\n", result2);
        usleep(rand() % 100);
    }
    result2 = i;
}

void * third(){
    int i = 0;
    for(totalLoopsCompleted; totalLoopsCompleted < totalLoops; totalLoopsCompleted++) {
        i++;
        printf("\nThread three starting for the %i time\n", i);
        while (compareAndSwap(&lock, 0, 1) == 0) // spinlock
        x = 9;
        y = 6;
        result3 = x * y;
        lock = 0; // Unlocks
        printf("Thread 3 thinks 9 * 6 = %i", result3);
        usleep(rand() % 100);
    }
    result3 = i;
}

int main() {
    int threads = 2;
    srand(time(NULL));
    pthread_t tid[threads];
    pthread_create(&tid[0], NULL, &first, NULL);
    pthread_create(&tid[1], NULL, &second, NULL);
    pthread_create(&tid[2], NULL, &third, NULL);
    pthread_join(tid[0],NULL);
    pthread_join(tid[1],NULL);
    pthread_join(tid[2],NULL);
    return 0;
}