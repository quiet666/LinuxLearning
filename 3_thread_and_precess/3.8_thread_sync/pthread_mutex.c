#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define NUM 50

int number=0;

pthread_mutex_t mutex;
int aCount=0;
int bCount=0;

void *funA(void* arg ){
    for(int i=0; i<NUM; ++i){
        aCount++;
        // 上锁
        pthread_mutex_lock(&mutex);

        int cur = number;
        cur++;
        usleep(50);
        number = cur;

        // 解锁
        pthread_mutex_unlock(&mutex);
        printf("Thread A, id=%lu, number=%d\n", pthread_self(), number);
        
        
    }
    return NULL;
}


void *funB(void* arg ){
    for(int i=0; i<NUM; ++i){
        bCount++;
        pthread_mutex_lock(&mutex);
        int cur= number;
        cur++;
        number = cur;
        pthread_mutex_unlock(&mutex);
        printf("Thread B, id=%lu, number=%d\n", pthread_self(), number);
        usleep(5);
    }
    return NULL;
}


void main(){

    pthread_t p1, p2;
    
    // 初始化
    pthread_mutex_init(&mutex, NULL);

    int res2 = pthread_create(&p2, NULL, funB, NULL);
    int res1 = pthread_create(&p1, NULL, funA, NULL);
    

    pthread_join(p1,NULL);
    pthread_join(p2,NULL);

    printf("funA执行了%d次, funB执行了%d次\n",aCount,bCount );

    // 销毁
    pthread_mutex_destroy(&mutex);

    
}