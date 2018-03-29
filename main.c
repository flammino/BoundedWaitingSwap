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
#include <signal.h>

#define THREADS 10

bool lock = false;
int x;
int y;
int result;
int totalLoops = 3000;
int totalLoopsCompleted = 0;

bool waiting [THREADS];
int timesRan[THREADS];

bool compareAndSwap(bool *lock, bool expected, bool newValue){
    bool temp = *lock;
    if (*lock == expected){ // swaps to allow for locking
        *lock = newValue;
    }
    return temp; // returns value of lock when originally called
}



void * first(void * t){
    int thread = *(( int *)t);
    int i = 0;
    int j = -1;
    bool waitVal = false;
    for(totalLoopsCompleted; totalLoopsCompleted < totalLoops; totalLoopsCompleted++) {
        waiting[thread] = true; // Thread is waiting to start once it hits this point
        while (!(compareAndSwap(&lock, false, true)) && waiting[thread]) //stalls until lock or waiting is false
            // (waiting false means turn is up)
        x = thread;
        y = 7;
        result = x + y;
        if (x + y != result){ // Checks to make sure variables weren't overwritten during lock
            printf("\n\nMutual Exclusion Error! Thread %i, loop %i.",
                   thread, totalLoopsCompleted);
            exit(1);
        };
        while(j < 0 || j >= THREADS){ // I put this in a while loop since I kept getting a weird segfault around loop 15, line 59
            j = thread + 1;
            j %= THREADS; // starts at next thread
            waitVal = waiting[j];
        }
        while ((j + 1) % THREADS != i && !waitVal){ // Cycles through looking for next waiting thread
            j = j + 1 % THREADS;
            waitVal = waiting[j]; // if not for the segfault, waitVal could be omitted and just test waiting[j] directly in the while
        }
        if (j == thread){ // No waiting threads, open the lock
            lock = false;
        }
        else{
            waiting[j] = false; // Allow next waiting thread to continue
        }
        i++;
        timesRan[thread] = i; // for ensuring all threads are running evenly
        usleep(rand() % 100);
    }
}


int main() {
    srand(time(NULL));
    pthread_t tid[THREADS];

    for (int i = 0; i < THREADS; i++){
        waiting[i] = false; // no threads are waiting before being called
    }

    for (int i = 0; i < THREADS; i++){
        int *arg = malloc(sizeof(*arg));// needed to keep memory straight between threads
        if ( arg == NULL ) { // arg setup from https://stackoverflow.com/questions/19232957/pthread-create-passing-an-integer-as-the-last-argument
            fprintf(stderr, "Couldn't allocate memory for thread arg.\n");
            exit(EXIT_FAILURE);
        }
        *arg = i;
        pthread_create(&tid[i], NULL, &first, arg);
    }

    for (int i = 0; i < THREADS; i++){ // Waits for threads to finish
        pthread_join(tid[i], NULL);
    }

    for (int i = 0; i < THREADS; i ++){
        float percentageRan = 100.0*timesRan[i] / totalLoopsCompleted;
        printf("\nThread %i ran %f percent of the time.\n", i, percentageRan);
    }
    printf("%i loops completed, no errors.", totalLoopsCompleted);
    return 0;
}