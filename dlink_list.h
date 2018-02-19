/*******************************************************************************
   @Filename:dlink_list.h
   @Brief: An abstraction for double linked list operations implemented in
   dlink_list.c

   This file declares double linked list(Henceforth dll) functions and external
   structures for dlink_list.h

   @Author:Ravi Dubey
   @Date:1/30/2018
*******************************************************************************/

#ifndef DLINK_LIST_H
#define DLINK_LIST_H

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/*
 *@brief:structure to hold link list nodes
 * This structure holds the pointer to the previous and next nodes. It dosent hold
   data or pointer to data.
 */
typedef struct node_head {
								struct node_head* prev;
								struct node_head* next;
}node_head;

/*
 *@brief:structure to hold node info
 * This structure is where the data is stored
 */
typedef struct {
								char data;
								uint32_t count;
								struct node_head this_node_head;
}node_info;


#define OFFSET(type,member) ((size_t)(& (((type*)0)->member)))

#define GET_LIST_CONTAINER(addr,type,member)  \
								( (type*) ( ((char*)addr) - OFFSET(type,member) ) )


/*
 *@brief:Implements destroying all nodes of doulbe linked list(dll)
 * Destroys all nodes in the linked list by freeing the memory
 *@param: a linked list data structure pointer
 *@return: pointer to the head of the linked list
 */
node_head* destroy(node_head*);



/*
 *@brief:Implements adding node at the end of dll
 * Add a node to the end of the linked list if the data dosent aleady exist.
 * If the data already exists, increase the count by 1.
 * Should add head node if it does not exist.
 *@param: the head node pointer and the data to add
 *@return: Pointer to the head of the linked
 */
node_head* insert_at_end(node_head*, char data );

/*
 *@brief:Implements calculation of number of nodes in dll
 * Determine the number of links in your linked list
 *@param:a node pointer
 *@return:size of linked list in node
 */
size_t size(node_head*);


#endif
