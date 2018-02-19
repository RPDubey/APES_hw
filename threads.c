/*******************************************************************************
   @Filename:threads.c
   @Brief:thread function definition

   @Author:Ravi Dubey
   @Date:2/12/2018
*******************************************************************************/

#include "threads.h"
#include <errno.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <syscall.h>
#include <time.h>
#include <unistd.h>

#include "dlink_list.h"

#define TRUE (1)
#define FALSE (0)
#define STRING_BUFFER_LENGTH (200)
#define gettid() syscall(SYS_gettid)
#define DEFAULT_THREAD_ATTR ((void *)0)

#define HANDLE_ERR(err){ \
                printf("Error:%s\n",strerror(err)); \
                return NULL;}

int fprintf_sync(char *string_buffer, pthread_mutex_t *file_print_mutex,
                 char *pfile_name) {
        int ret;
        FILE *pfile = fopen(pfile_name, "a+");
        if (pfile == NULL) {
                printf("Error: %s\n", strerror(errno));
                return -1;
        }

        ret = pthread_mutex_lock(file_print_mutex);
        if (ret) {
                printf("Error: %s\n", strerror(errno));
                return -1;
        }
        fprintf(pfile, "%s\n", string_buffer);
        ret = pthread_mutex_unlock(file_print_mutex);
        if (ret) {
                printf("Error: %s\n", strerror(errno));
                return -1;
        }

        fclose(pfile);
        return 0;
}

bool thread1_flag = FALSE;
bool thread2_flag = FALSE;
/*For deterministic behaviour SIGUSR1 is used to stop Child thread1 using flag variable thread1_flag*/
void thread1_sigHandler(int signo) {
        if (signo == SIGUSR1) {
                printf("Thread1:SIGUSR1 Rcvd\n");
                thread1_flag = TRUE;
        }
}

/*SIGUSR2 to stop Child thread2 using flag variable thread2_flag*/
void thread2_sigHandler(int signo) {
        if (signo == SIGUSR2) {
                printf("Thread2:SIGUSR2 Rcvd\n");
                thread2_flag = TRUE;
        }
}

/*************************Master Thread Function*******************************/
void *threadFunc0(void *pthread_info) {
        threadInfo *ppthread_i = (threadInfo *)pthread_info;
        char *pfile_name = ppthread_i->plog_file;

        int ret;
        pthread_mutex_t file_print_mutex;
        ret = pthread_mutex_init(&file_print_mutex, NULL);
        if (ret != 0) {HANDLE_ERR(errno);}

        char string_buffer[STRING_BUFFER_LENGTH];
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        pid_t linux_id = gettid();
        pthread_t posix_id = pthread_self();
        sprintf(string_buffer, "\nMaster Thread\nPosix Thread id is:%ld\
                         \nLinux Thread id is:%d\nEntry Time:%s",             posix_id, linux_id, asctime(tm));
        printf("%s\n",string_buffer);
        ret = fprintf_sync(string_buffer, &file_print_mutex, pfile_name);
        if (ret != 0) {HANDLE_ERR(errno);}
        pthread_t thread1;// Child Thread1
        threadInfo thread_info1;
        pthread_t thread2;// Child Thread2
        threadInfo thread_info2;
        thread_info1.thread_id = 1;
        thread_info1.plog_file = pfile_name;
        thread_info2.thread_id = 2;
        thread_info2.plog_file = pfile_name;
        thread_info1.fp_mutex = &file_print_mutex;
        thread_info2.fp_mutex = &file_print_mutex;

        ret = pthread_create(
                &thread2,
                DEFAULT_THREAD_ATTR,
                threadFunc2,
                (void *)&(thread_info2)
                );
        if (ret != 0) {HANDLE_ERR(errno);}

        ret = pthread_create(&thread1,
                             DEFAULT_THREAD_ATTR,
                             threadFunc1,
                             (void *)&(thread_info1));
        if (ret != 0) {HANDLE_ERR(errno);}

        pthread_join(thread2, NULL);
        pthread_join(thread1, NULL);

        tm = localtime(&t);
        printf("Master Thread Exiting:%s\n",asctime(tm));
        ret = fprintf_sync("Master Thread Exiting",&file_print_mutex, pfile_name);
        if (ret != 0) {HANDLE_ERR(errno);}
        ret = fprintf_sync(asctime(tm), &file_print_mutex, pfile_name);
        if (ret != 0) {HANDLE_ERR(errno);}
        return NULL;
}

/*************************Child Thread1 Function*******************************/
void *threadFunc1(void *pthread_info) {
        if (signal(SIGUSR1, thread1_sigHandler) == SIG_ERR)
                printf("signal error\n");
        threadInfo *ppthread_i = (threadInfo *)pthread_info;
        pthread_mutex_t *file_print_mutex = ppthread_i->fp_mutex;
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        int ret;

        char string_buffer[STRING_BUFFER_LENGTH];
        pid_t linux_id = gettid();
        pthread_t posix_id = pthread_self();
        sprintf(string_buffer, "\nChild Thread1\nPosix Thread id is:%ld\
                         \nLinux Thread id is:%d\nEntry Time:%s",
                posix_id, linux_id, asctime(tm));
        printf("%s\n",string_buffer);
        ret = fprintf_sync(string_buffer, file_print_mutex, ppthread_i->plog_file);
        if (ret != 0) {HANDLE_ERR(errno);}
        node_info *first_node_info = (node_info *)malloc(sizeof(node_info));
        if (first_node_info == NULL) {
                printf("Malloc Failed\n");
                return NULL;
        }
        (first_node_info->this_node_head).next = NULL;
        (first_node_info->this_node_head).prev = NULL;
        char c;
        FILE *ptext_file = fopen("Valentinesday.txt", "r");
        c = fgetc(ptext_file);
        //sleep(20);//for testing purpose
        while (c != EOF) {
                if(thread2_flag == TRUE) break;
                if (c > 96 && c < 123) c -= 32;
                if ((c > 64 && c < 91)) {insert_at_end(&(first_node_info->this_node_head), c);}
                c = fgetc(ptext_file);// sleep(1);
        };
        if(thread2_flag == FALSE) {
                node_head *head = &(first_node_info->this_node_head);
                node_info *info = first_node_info;
                size_t list_size = size(&(first_node_info->this_node_head));
                printf("Characters with Three occurances\n");
                ret = fprintf_sync("Characters with Three occurances\n", file_print_mutex, ppthread_i->plog_file);
                if (ret != 0) {HANDLE_ERR(errno);}
                for (int i = 0; i < list_size; i++) {
                        if(info->count == 3) {
                                printf("%c\n",info->data );
                                ret = fprintf_sync(&(info->data), file_print_mutex, ppthread_i->plog_file);
                                if (ret != 0) {HANDLE_ERR(errno);}
                        }
                        head = head->next;
                        info = GET_LIST_CONTAINER(head, node_info, this_node_head);
                }
        }
        destroy(&(first_node_info->this_node_head));
        tm = localtime(&t);
        printf("Child Thread1 Exiting:%s\n",asctime(tm));
        ret = fprintf_sync("Child Thread1 Exiting:", file_print_mutex, ppthread_i->plog_file);
        if (ret != 0) {HANDLE_ERR(errno);}
        ret = fprintf_sync(asctime(tm), file_print_mutex, ppthread_i->plog_file);
        if (ret != 0) {HANDLE_ERR(errno);}
        return NULL;
}

/*************************Child Thread2 Function*******************************/
void *threadFunc2(void *pthread_info) {

        if (signal(SIGUSR2, thread2_sigHandler) == SIG_ERR)
                printf("signal error\n");

        threadInfo *ppthread_i = (threadInfo *)pthread_info;
        pthread_mutex_t *file_print_mutex = ppthread_i->fp_mutex;
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        int ret;
        char string_buffer[STRING_BUFFER_LENGTH];
        pid_t linux_id = gettid();
        pthread_t posix_id = pthread_self();
        sprintf(string_buffer, "\nChild Thread2\nPosix Thread id is:%ld\
                         \nLinux Thread id is:%d\nEntry Time:%s",
                posix_id, linux_id, asctime(tm));
        printf("%s\n",string_buffer);
        ret = fprintf_sync(string_buffer, file_print_mutex, ppthread_i->plog_file);
        if (ret != 0) {HANDLE_ERR(errno);}
        struct timespec sleep_time, rem_time;
        while (!thread2_flag) {
                clock_gettime(CLOCK_MONOTONIC,&sleep_time);
                char c;
                system("uptime > load.txt");
                FILE *ptext_file = fopen("load.txt", "r");
                fgets(string_buffer,80,ptext_file);
                fclose(ptext_file);
                printf("%s\n",string_buffer);
                ret = fprintf_sync(string_buffer, file_print_mutex, ppthread_i->plog_file);
                if (ret != 0) {HANDLE_ERR(errno);}
                clock_gettime(CLOCK_MONOTONIC,&rem_time);
                sleep_time.tv_sec = 0;
                sleep_time.tv_nsec = 100000000-(rem_time.tv_nsec - sleep_time.tv_nsec);
                rem_time.tv_sec = 0; rem_time.tv_nsec = 0;
                do {
                        nanosleep(&sleep_time,&rem_time);
                        sleep_time.tv_sec = 0;
                        sleep_time.tv_nsec = rem_time.tv_nsec;

                } while( (rem_time.tv_sec > 0) || (rem_time.tv_nsec > 0) );

        }

        tm = localtime(&t);
        printf("Child Thread2 Exiting:%s\n",asctime(tm));
        ret = fprintf_sync("Child Thread2 Exiting:", file_print_mutex, ppthread_i->plog_file);
        if (ret != 0) {HANDLE_ERR(errno);}

        ret = fprintf_sync(asctime(tm), file_print_mutex, ppthread_i->plog_file);
        if (ret != 0) {HANDLE_ERR(errno);}
        return NULL;
}
