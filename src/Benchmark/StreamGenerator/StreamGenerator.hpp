//
// Created by tuidan on 2021/7/8.
//
#include <iostream>
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#ifndef ONLINEMLBENCHMARK_STREAMGENERATOR_HPP
#define ONLINEMLBENCHMARK_STREAMGENERATOR_HPP

#endif //ONLINEMLBENCHMARK_STREAMGENERATOR_H

pthread_cond_t qready=PTHREAD_COND_INITIALIZER;   //cond
pthread_mutex_t qlock=PTHREAD_MUTEX_INITIALIZER;  //mutex

bool FLAG = false;

void *DataShoot(void *arg);

void *RunningAlgorithm(void *arg);

int ThreadManager();