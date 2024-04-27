#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define NUM 50

int number=0;

void *funA(void* arg ){
    for(int i=0; i<NUM; ++i){

        int cur = number;
        cur++;
        usleep(50);
        number = cur;
        printf("Thread A, id=%lu, number=%d\n", pthread_self(), number);
    }
    return NULL;
}


void *funB(void* arg ){
    for(int i=0; i<NUM; ++i){
        int cur= number;
        cur++;
        number = cur;
        printf("Thread B, id=%lu, number=%d\n", pthread_self(), number);
        usleep(5);
    }
    return NULL;
}


void main(){

    pthread_t p1, p2;

    int res1 = pthread_create(&p1, NULL, funA, NULL);
    int res2 = pthread_create(&p2, NULL, funB, NULL);

    pthread_join(p1,NULL);
    pthread_join(p2,NULL);

    
}