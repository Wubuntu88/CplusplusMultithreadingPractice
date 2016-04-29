//
//  test2.cpp
//  C++test
//
//  Created by William Edward Gillespie on 4/28/16.
//  Copyright © 2016 William Edward Gillespie. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <vector>
using namespace std;
#define NUM_THREADS 2

typedef struct {
    vector<int> *vec;
    int start;
    int exclusiveEnd;
} ArrayInfo;

void merge(ArrayInfo *arrInfo1, ArrayInfo *arrInfo2){
    int arrCopy[arrInfo2->exclusiveEnd - arrInfo1->start];
    int left = arrInfo1->start;
    int right = arrInfo2->start;
    int current = 0;
    while (left < arrInfo1->exclusiveEnd && right < arrInfo2->exclusiveEnd) {
        if (arrInfo1->vec->at(left) < arrInfo2->vec->at(right)) {
            arrCopy[current++] = arrInfo1->vec->at(left++);
        }else{
            arrCopy[current++] = arrInfo2->vec->at(right++);
        }
    }
    
    while (left < arrInfo1->exclusiveEnd) {
        arrCopy[current++] = arrInfo1->vec->at(left++);
    }
    while (right < arrInfo2->exclusiveEnd) {
        arrCopy[current++] = arrInfo2->vec->at(right++);
    }
    
    vector<int> *vector = arrInfo1->vec;
    int i = 0;
    for (unsigned long k = arrInfo1->start; k < arrInfo2->exclusiveEnd; k++, i++) {
        vector->at(k) = arrCopy[i];
    }
}

void selectionSort(vector<int> *array){
    for (int i = 0; i < array->size() - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < array->size(); j++) {
            if (array->at(j) < array->at(minIndex)) {
                minIndex = j;
            }
        }
        //swap(array[minIndex], array[i]);
        int temp = array->at(i);
        array->at(i) = array->at(minIndex);
        array->at(minIndex) = temp;
    }
}

void *recursiveMergeSort(void * input){
    ArrayInfo *arrInfo = (ArrayInfo *)input;
    ArrayInfo subArrayInfos[NUM_THREADS];
    //do recursive, multithreaded merge sort
    int mid = (arrInfo->exclusiveEnd + arrInfo->start) / 2;
    subArrayInfos[0].vec = arrInfo->vec;
    subArrayInfos[0].start = arrInfo->start;
    subArrayInfos[0].exclusiveEnd = mid + 1;
    
    subArrayInfos[1].vec = arrInfo->vec;
    subArrayInfos[1].start = mid + 1;
    subArrayInfos[1].exclusiveEnd = arrInfo->exclusiveEnd;
    
    if (arrInfo->exclusiveEnd - arrInfo->start > 11) {
        pthread_t threads[NUM_THREADS];
        for (int i = 0; i < NUM_THREADS; i++) {
            pthread_create(&threads[i], NULL, recursiveMergeSort, &subArrayInfos[i]);
        }
        for(int i = 0; i < NUM_THREADS; i++){
            pthread_join(threads[i], NULL);
        }
    } else {
        selectionSort(arrInfo->vec);
    }
    merge(&subArrayInfos[0], &subArrayInfos[1]);
    pthread_exit(NULL);
    return 0;
}

void *mergeSort(void *input){
    vector<int> *vector = static_cast<std::vector<int> *>(input);
    ArrayInfo arrInfo;
    arrInfo.vec = vector;
    arrInfo.start = 0;
    arrInfo.exclusiveEnd = (int)arrInfo.vec->size();
    recursiveMergeSort(&arrInfo);
    pthread_exit(NULL);
    return 0;
}

int main(int argc, const char * argv[]) {
    int size = atoi(argv[1]);
    vector<int> v;
    for (int i = size; i > 0; i--) {
        v.push_back(i);
    }
    
    pthread_t thread;
    
    pthread_create(&thread, NULL, mergeSort, &v);
    pthread_join(thread, NULL);
    selectionSort(&v);
    printf("sorted array (size=%d): \n", (int)v.size());
    for (int i = 0; i < v.size(); i++) {
        printf("%d, ", v[i]);
    }
    printf("\n");
    fflush(stdout);
    return 0;
}



















