/*******************************************************************************
@Filename:threads.c
@Brief:thread function definition

@Author:Ravi Dubey
@Date:2/12/2018
*******************************************************************************/

#include"threads.h"
#include<stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <syscall.h>
#include <sys/types.h>
#include <time.h>
#include <signal.h>

#include <stdlib.h>

#define gettid() syscall(SYS_gettid)
#define DEFAULT_THREAD_ATTR ((void *)0)

#define HANDLE_ERR(err) {printf("Error: %s\n",strerror(err)); return NULL;}

#define FPRINT_THREAD_INFO  {\
                    ret = pthread_mutex_lock(&file_print_mutex);\
                    if(ret) HANDLE_ERR(errno);\
                    fprintf(pfile,"\n%s\n",thread_name);\
                    fprintf(pfile,"Entry Time:%s",asctime(tm));\
                    pid_t linux_id = gettid();\
                    pthread_t posix_id =  pthread_self();\
                    fprintf(pfile,"Posix Thread id is:%ld\n",posix_id);\
                    fprintf(pfile,"Linux Thread id is:%d\n",linux_id);\
                    ret = pthread_mutex_unlock(&file_print_mutex);\
                    if(ret) HANDLE_ERR(errno);}

#define FPRINTF_SYNC(info) {\
                    ret = pthread_mutex_lock(&file_print_mutex);\
                    if(ret) HANDLE_ERR(errno);\
                    fprintf(pfile,"\n%s\n",info);\
                    ret = pthread_mutex_unlock(&file_print_mutex);\
                    if(ret) HANDLE_ERR(errno);}

void thread1_sigHandler(int signo)
{
if (signo == SIGUSR1) printf("Thread1:SIGUSR1 Rcvd\n");

}

void thread2_sigHandler(int signo)
{
if (signo == SIGUSR2) printf("Thread2:SIGUSR2 Rcvd\n");
}


void *threadFunc0(void *pthread_info)
{
  threadInfo* ppthread_i = (threadInfo*)pthread_info;
  char* pfile_name=ppthread_i->plog_file;

 int ret;
 pthread_mutex_t file_print_mutex;
 ret = pthread_mutex_init(&file_print_mutex,NULL);if(ret) HANDLE_ERR(errno);

  time_t t = time(NULL);
  struct tm *tm = localtime(&t);

  FILE* pfile = fopen(pfile_name,"a+");if(pfile == NULL) HANDLE_ERR(errno);
  char thread_name[]="Master Thread";
  FPRINT_THREAD_INFO;
  fclose(pfile);

  pthread_t thread1;threadInfo thread_info1; //Child Thread1
  pthread_t thread2;threadInfo thread_info2; //Child Thread2
  thread_info1.thread_id = 1; thread_info1.plog_file = pfile_name;
  thread_info2.thread_id = 2; thread_info2.plog_file = pfile_name;
  thread_info1.fp_mutex = file_print_mutex;
  thread_info2.fp_mutex = file_print_mutex;

  ret = pthread_create(
            &thread1,
            DEFAULT_THREAD_ATTR,
            threadFunc1,
            (void *)&(thread_info1)
          );if(ret) HANDLE_ERR(errno);

  ret = pthread_create(
            &thread2,
            DEFAULT_THREAD_ATTR,
            threadFunc2,
            (void *)&(thread_info2)
          );if(ret) HANDLE_ERR(errno);

  pthread_join(thread1, NULL); pthread_join(thread2, NULL);
  while(1) sleep(1);

  tm = localtime(&t);
  pfile = fopen(pfile_name,"a+");
  fprintf(pfile,"Exit Time:%s\n",asctime(tm));
  fclose(pfile);

}

void *threadFunc1(void *pthread_info)
{
  threadInfo* ppthread_i = (threadInfo*)pthread_info;
  pthread_mutex_t file_print_mutex = ppthread_i->fp_mutex;
  time_t t = time(NULL); struct tm *tm = localtime(&t);
  int ret;
  FILE* pfile = fopen(ppthread_i->plog_file,"a+");
  if(pfile == NULL) {printf("Error: %s\n",strerror(errno)); return NULL;}
  char thread_name[]="Child Thread1";
  FPRINT_THREAD_INFO;
  fclose(pfile);
  if(signal(SIGUSR1,thread1_sigHandler) == SIG_ERR ) printf("signal error\n");

  //create a new thread which waits on condition variable for closing operation
  pthread_mutex_t
  tm = localtime(&t);
  pfile = fopen(ppthread_i->plog_file,"a+");
  fprintf(pfile,"Exit Time:%s\n",asctime(tm));
  fclose(pfile);

}

void *threadFunc2(void *pthread_info)
{
  threadInfo* ppthread_i = (threadInfo*)pthread_info;
  pthread_mutex_t file_print_mutex = ppthread_i->fp_mutex;
  time_t t = time(NULL); struct tm *tm = localtime(&t);
  int ret;
  FILE* pfile = fopen(ppthread_i->plog_file,"a+");
  if(pfile == NULL) {printf("Error: %s\n",strerror(errno)); return NULL;}
  char thread_name[]="Child Thread2";
  FPRINT_THREAD_INFO;
  fclose(pfile);
  if(signal(SIGUSR2,thread2_sigHandler) == SIG_ERR ) printf("signal error\n");
  tm = localtime(&t);
  pfile = fopen(ppthread_i->plog_file,"a+");
  fprintf(pfile,"Exit Time:%s\n",asctime(tm));
  fclose(pfile);
}
