/*
	CSC D84 - Unit 2 - MiniMax and adversarial games

	This file contains the API function headers for your assignment.
	Please pay close attention to the function prototypes, and
	understand what the arguments are.

	Stubs for implementing each function are to be found in MiniMax_search.c,
	along with clear ** TO DO markers to let you know where to add code.

	You are free to add helper functions within reason. But you must
	provide a prototype *in this file* as well as the implementation
	in the .c program file.

	Script by: F.J.E., Sep. 2015
*/

#ifndef __MiniMax_search_header

#define __MiniMax_search_header

// Generally needed includes
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<malloc.h>
#include<string.h>

#include "board_layout.h"

// Function prototypes for D84 - Unit 2 - MiniMax assignment solution
double MiniMax(double gr[graph_size][4], int path[1][2], double minmax_cost[size_X][size_Y], int cat_loc[10][2], int cats, int cheese_loc[10][2], int cheeses, int mouse_loc[1][2], int mode, double (*utility)(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, int depth, double gr[graph_size][4]), int agentId, int depth, int maxDepth, double alpha, double beta);

double utility(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, int depth, double gr[graph_size][4]);

int checkForTerminal(int mouse_loc[1][2],int cat_loc[10][2],int cheese_loc[10][2],int cats,int cheeses);


// If you need to add any function prototypes yourself, you can do so *below* this line.

int get_graph_index(int x, int y);

struct graph_location {
	int x;
	int y;
};

struct graph_location get_graph_location (int index);

int calculate_manhattan_distance(struct graph_location p1, struct graph_location p2);

int distanceToClosestItem(int mouse_loc[1][2], int item_loc[10][2], int num_items);

double first_utility_function(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, int depth, double gr[graph_size][4], int prev_distance_cheese, int prev_distance_cat);

double second_utility_function(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, int depth, double gr[graph_size][4]);

double MiniMax_Helper(double gr[graph_size][4], int path[1][2], double minmax_cost[size_X][size_Y], int cat_loc[10][2], int cats, int cheese_loc[10][2], int cheeses, 
	int mouse_loc[1][2], int mode, double (*utility)(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, int depth, 
		double gr[graph_size][4]), int agentId, int depth, int maxDepth, double alpha, double beta, int prev_distance_to_cheese, int prev_distance_to_cat, int visited_nodes[size_X][size_Y]);

double AlphaBetaPruning(double gr[graph_size][4], int path[1][2], double minmax_cost[size_X][size_Y], int cat_loc[10][2], int cats, int cheese_loc[10][2], int cheeses, int mouse_loc[1][2], 
	int mode, double (*utility)(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, int depth, double gr[graph_size][4]), 
	int agentId, int depth, int maxDepth, double alpha, double beta, int prev_distance_to_cheese, int prev_distance_to_cat, int visited_nodes[size_X][size_Y]);
	
int H_cost_nokitty(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, double gr[graph_size][4]);

#endif

