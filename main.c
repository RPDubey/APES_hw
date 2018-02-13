/*******************************************************************************
@Filename:main.c
@Brief:This file implements requirements for HW3
@Author:Ravi Dubey
@Date:2/12/2018
*******************************************************************************/

#include <pthread.h>
#include <sched.h>

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include"threads.h"

#define NUM_THREADS    (1)
#define DEFAULT_THREAD_ATTR ((void *)0)


int main ()
{

  pthread_t thread1, thread2;
  threadInfo thread_info1,thread_info2;
  int ret;

  ret = pthread_create(
            &thread1,
            DEFAULT_THREAD_ATTR,
            threadFunc1,
            (void *)&(thread_info1)
          );
  if(ret) {printf("Error: %s\n",strerror(errno)); return ret;}

  pthread_join(thread1, NULL);
  printf("TEST COMPLETE\n");
  return 0;
}
