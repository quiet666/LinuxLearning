#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define PTHREAD_NUM 3


pthread_mutex_t mutex;
// pthread_cond_t cond;

sem_t semp; // 生产者的信号
sem_t semc; // 消费者的信号


// 任务链表
struct Node{
    int number;
    struct Node *next;
};

struct Node *head=NULL;  // 用于指向最新的节点



// 生产者的回调函数
void *producerWork(void *arg){
    while(1){

        // 检查生产者是否有资源
        sem_wait(&semp);

        // 线程同步。注意，需要在sem_wait后面上锁
        pthread_mutex_lock(&mutex);
        // 创建一个新节点
        struct Node *nextNode = (struct Node*)malloc(sizeof(struct Node));
        nextNode->number = rand()%1000;
        nextNode->next = head; // head涉及共享资源，因此需要上锁

        // 头指针前移
        head = nextNode;   
        printf("+++任务%d 已经被添加到任务队列, Pid=%ld\n",head->number, pthread_self());

        pthread_mutex_unlock(&mutex);
        // pthread_cond_broadcast(&cond);
        
        // 给消费者的资源+1
        sem_post(&semc);

        // 不要太快
        sleep(rand()%3); 
    }

    return NULL;
}

// 消费者
void *comsumerWork(void *arg){
    while(1){
        // 
        sem_wait(&semc);
        pthread_mutex_lock(&mutex);
        // 不能用if,不然会出错
        // while(head==NULL){ 
        //     pthread_cond_wait(&cond, &mutex);
        // }
        printf("---任务%d 已经被处理, Pid=%ld\n",head->number, pthread_self());
        struct Node *node=head;
        head = head->next; // 处理掉一个节点
        free(node); // 不要忘记回收内存

        pthread_mutex_unlock(&mutex);
        sem_post(&semp);
        sleep(rand()%2);
    }
    
    return NULL;
}

void main(){
    pthread_mutex_init(&mutex, NULL);
    // pthread_cond_init(&cond, NULL);

    // 生产者
    sem_init(&semp, 0, PTHREAD_NUM);
    // 消费者。注意，初始化为0
    sem_init(&semc, 0, 0);

    pthread_t producerId[PTHREAD_NUM];
    pthread_t comsumerId[PTHREAD_NUM];

    for(int i=0;i<PTHREAD_NUM;++i){
        pthread_create(&producerId[i], NULL, producerWork, NULL);
        pthread_create(&comsumerId[i], NULL, comsumerWork, NULL);
    }



    for(int i=0;i<5;++i){
        pthread_join(producerId[i],NULL); //join只需要ip，不需要地址
        pthread_join(comsumerId[i],NULL);
    }


    pthread_mutex_destroy(&mutex);
    // pthread_cond_destroy(&cond);
    sem_destroy(&semp);
    sem_destroy(&semc);
}