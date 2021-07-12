//
// Created by tuidan on 2021/6/30.
//
#include "StreamGenerator.hpp"


void *RunningAlgorithm(void *arg)
{
    cout<<"Initializing algorithms..."<<endl;
    /// here we run the initialization functions of the algorithms
    pthread_mutex_lock(&qlock);
    while(FLAG)
    {
        pthread_cond_wait(&qready,&qlock);
        sleep(3);
        cout<<"Received the data, running algorithms..."<<endl;
        /// here we run the algorithm.
        // 这里qready不改变的话系统就不会检查qlock状态从而默认一直阻塞DataShoot
    }
    pthread_mutex_unlock(&qlock);
    sleep(3);
    cout<<"RunningAlgorithm end"<<endl;
}

void *DataShoot(void *arg)
{
    cout<<"Timestamp start,shooting data..."<<endl;
    pthread_mutex_lock(&qlock);
    flag = true;
    cout<<"has a change,flag="<<FLAG<<endl;
    pthread_mutex_unlock(&qlock);
    if(FLAG)
    {
        pthread_cond_signal(&qready);
        // qlock在之前已经改变了，所以qready这里改变完就会去通知DataShoot线程检查qlock状态从而重新启动DataShoot
    }
    cout<<"DataShoot end"<<endl;
}

int ThreadManager()
{
    pthread_t tids[2];
    int re;

    re=pthread_create(&tids[0],NULL,RunningAlgorithm,NULL);
    if(re){
        cout<<"pthread RunningAlgorithm create error "<<endl;
        return re;
    }
    sleep(2);

    re=pthread_create(&tids[1],NULL,DataShoot,NULL);
    if(re){
        cout<<"pthread DataShoot create erro "<<endl;
        return re;
    }

    sleep(5);
    return 0;

