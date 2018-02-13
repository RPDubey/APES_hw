/*******************************************************************************
@Filename:threads.c
@Brief:thread function definition

@Author:Ravi Dubey
@Date:2/12/2018
*******************************************************************************/

#include"threads.h"
#include<stdio.h>

void *threadFunc1(void *pthread_info)
{
threadInfo* ppthread_i = (threadInfo*)pthread_info;
printf("Thread id is: %d\n",ppthread_i->thread_id);
}

void *threadFunc2(void *pthread_info)
{
threadInfo* ppthread_i = (threadInfo*)pthread_info;
printf("Thread id is: %d\n",ppthread_i->thread_id);
}
