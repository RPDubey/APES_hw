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
#include <sys/types.h>
#include <unistd.h>
#define FILE_NAME_SIZE  (15)
#define NUM_THREADS    (1)
#define DEFAULT_THREAD_ATTR ((void *)0)


int main ()
{

  char file_name[FILE_NAME_SIZE];
  pthread_t thread0; //Master Thread
  threadInfo thread_info0;
  int ret;
  printf("Enter File Name\n");
  scanf("%s",file_name);
  printf("Main PID:%d\n",getpid());
  thread_info0.thread_id = 0;
  thread_info0.plog_file = file_name;
  ret = pthread_create(
            &thread0,
            DEFAULT_THREAD_ATTR,
            threadFunc0,
            (void *)&(thread_info0)
          );
  if(ret) {printf("Error: %s\n",strerror(errno)); return ret;}

  pthread_join(thread0, NULL);
  printf("TEST COMPLETE\n");
  return 0;
}
