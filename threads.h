/*******************************************************************************
@Filename:threads.h
@Brief:An abstarction for thread functions defined in threads.c
declares exposed structures and functions for threads.
@Author:Ravi Dubey
@Date:2/12/2018
*******************************************************************************/

#ifndef THREADS_H
#define THREADS_H

#include <pthread.h>
#include <sched.h>

/**
*@brief:structure to pass info to thread functions
*/
typedef struct
{
  int thread_id;
} threadInfo;


/**
*@brief:Implements thread function for first child thread
*Thread function passed at the time of thread creation to implement thread
*oprtn. Does logging, and searching through string using linked list.
*@param: Threadparam cast as void*
*@return: Pointer cast as void*
*/
void *threadFunc1(void *pthread_info);

/**
*@brief:Implements thread function for second child thread
*Thread function passed at the time of thread creation to implement thread
*oprtn. Does logging and CPU utilization metrics.
*@param: Threadparam cast as void*
*@return: Pointer cast as void*
*/
void *threadFunc2(void *pthread_info);


#endif
