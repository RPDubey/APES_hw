/*******************************************************************************
   @Filename  :dlink_list.c
   @Brief     :This file implements double linked list functions

   @Author    :Ravi Dubey
   @Date      :1/30/2018
*******************************************************************************/
#include <stdio.h>

#include "dlink_list.h"
#include <stdint.h>

/********************************************************************************/
node_head *insert_at_end(node_head *head, char node_data) {
        // printf("head:%pnext:%pprev:%p\n",head,head->next,head->prev );
        // null head
        if (head == NULL)
                return NULL;

        // if no head exists
        else if (head->next == NULL) {

                head->next = head; // next pointer of the "Head" node_head points to itself
                head->prev = NULL;
                node_info *this_node_info =
                        GET_LIST_CONTAINER(head, node_info, this_node_head);
                this_node_info->data = node_data;
                this_node_info->count = 1;
                return head;
        }

        // if head exists, create a new node
        else {

                node_head *list_head = head;
                node_info *this_node_info;
                node_head* tail;

                size_t list_size = size(list_head);
                for(int i=0; i<list_size; i++) {
                        tail = head;
                        this_node_info = GET_LIST_CONTAINER(head, node_info, this_node_head);
                        if (this_node_info->data == node_data) {
                                this_node_info->count++;
                                return list_head;
                        } else
                                head = head->next;

                }

                node_info *new_node = (node_info *)malloc(sizeof(node_info));
                if (new_node == NULL)
                        return NULL;

                tail->next = &(new_node->this_node_head);
                (new_node->this_node_head).next = NULL;
                (new_node->this_node_head).prev = head;
                new_node->data = node_data;
                new_node->count = 1;
                return list_head;
        }
}

/******************************************************************************/
node_head *destroy(node_head *head) {

        if (head == NULL)
                return NULL;
        // cae of memory alloted but node uninitialized
        if (head->prev == NULL && head->next == NULL) {
                free(GET_LIST_CONTAINER(head, node_info, this_node_head));
                return NULL;
        }
        node_info* info;
        node_head *next_head;
        size_t length = size(head);
        for (int i = 0; i < length; i++) {
                next_head = head->next;

                info = GET_LIST_CONTAINER(head, node_info, this_node_head);
                if(info == NULL) break;
                else free(info);
                head=next_head;

        }

        return NULL;
}

/******************************************************************************/
size_t size(node_head *node) {

        if (node == NULL)
                return -1;

        if ((node->next == NULL) && (node->prev == NULL))
                return 0;

        if (node->next == node) {
                return 1;
        }

        if ((node->prev == NULL)) {
                if (node->next->next == NULL)
                        return 2;
        }

        if ((node->next == NULL)) {
                if (node->prev->prev == NULL)
                        return 2;
        }

        size_t i = 0;

        node_head *node_copy = node;

        while (node->next != NULL) {
                i++;
                // if(i>6) break;
                node = node->next;
        }

        node = node_copy;
        while (node->prev != NULL) {
                i++;
                node = node->prev;
        }
        return i + 1;
}
