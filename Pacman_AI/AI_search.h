/*
	CSC D84 - Unit 1 - Search

	This file contains the API function headers for your assignment.
	Please pay close attention to the function prototypes, and
	understand what the arguments are.

	Stubs for implementing each function are to be found in AI_search.c,
	along with clear ** TO DO markers to let you know where to add code.

	You are free to add helper functions within reason. But you must
	provide a prototype *in this file* as well as the implementation
	in the .c program file.

	Starter by: F.J.E., Jul. 2015
	Updated by: F.J.E., Jan. 2018
*/

#ifndef __AI_search_header

#define __AI_search_header

// Generally needed includes
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<malloc.h>
#include<limits.h>

#include "board_layout.h"

// Function prototypes for D84 - Unit 1 - Search assignment solution

void search(double gr[graph_size][4], int path[graph_size][2], int visit_order[size_X][size_Y], int cat_loc[10][2], int cats, int cheese_loc[10][2], int cheeses, int mouse_loc[1][2], int mode, int (*heuristic)(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, double gr[graph_size][4]));
int H_cost(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, double gr[graph_size][4]);
int H_cost_nokitty(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, double gr[graph_size][4]);

// If you need to add any function prototypes yourself, you can do so *below* this line.

/*****************************************************************************************
% THE HELPER FUNCTION PROTOTYPES BELOW FOR SETTING UP DATA STRUCTURES ARE NOT CODED BY US
% THEY ARE OBTAINED FROM https://www.geeksforgeeks.org/
 ****************************************************************************************/
struct queue {
  int items[graph_size];
  int front;
  int rear;
};

struct queue* createQueue();
void enqueue(struct queue* q, int);
int dequeue(struct queue* q);
int isEmpty(struct queue* q);

struct Stack {
    int top;
    unsigned capacity;
    int* array;
};

// Node
typedef struct node { 
    int data; 
    // Lower values indicate higher priority 
    int priority; 
    struct node* next; 
} Node; 
/*****************************************************************************************
% THE HELPER FUNCTION PROTOTYPES ABOVE FOR SETTING UP DATA STRUCTURES ARE NOT CODED BY US
% THEY ARE OBTAINED FROM https://www.geeksforgeeks.org/
 ****************************************************************************************/


// Our own helper function prototypes

int get_graph_index(int x, int y);

struct graph_location {
	int x;
	int y;
};

struct graph_location get_graph_location (int index);

int calculate_manhattan_distance(struct graph_location p1, struct graph_location p2);
#endif

