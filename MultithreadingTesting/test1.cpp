//
//  test1.cpp
//  C++test
//
//  Created by William Edward Gillespie on 4/27/16.
//  Copyright © 2016 William Edward Gillespie. All rights reserved.
//
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <pthread.h>
using namespace std;
#define NUM_THREADS 5

void *print_hello(void *thread_id){
    long t_id = (long)thread_id;
    printf("thread id: %lu\n", t_id);
    //cout << "thread id: " << t_id << endl;
    pthread_exit(NULL);
}

int main(int argc, const char * argv[]) {
    pthread_t threads[NUM_THREADS];
    int rc;
    for (long i = 0; i < NUM_THREADS; i++) {
        printf("main() creating thread: %lu\n", i);
        //cout << "main() creating thread: " << i << endl;
        rc = pthread_create(&threads[i], NULL, print_hello, (void*)i);
        if (rc) {
            cout << "Error: unable to create thread " << i << endl;
            exit(-1);
        }
    }
    pthread_exit(NULL);
    return 0;
}




















