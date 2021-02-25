/*
	CSC D84 - Unit 2 - MiniMax search and adversarial games

	This file contains stubs for implementing a MiniMax search
        procedure with alpha-beta pruning. Please read the assignment
	handout carefully - it describes the game, the data you will
	have to handle, and the search functions you must provide.

	Once you have read the handout carefully, implement your search
	code in the sections below marked with

	**************
	*** TO DO:
	**************

	Make sure to add it to your report.txt file - it will be marked!

	Have fun!

	DO NOT FORGET TO 'valgrind' YOUR CODE - We will check for pointer
	management being done properly, and for memory leaks.

	Starter code: F.J.E. Sep. 15
*/

#include "MiniMax_search.h"

double MiniMax(double gr[graph_size][4], int path[1][2], double minmax_cost[size_X][size_Y], int cat_loc[10][2], int cats, int cheese_loc[10][2], int cheeses, int mouse_loc[1][2], int mode, double (*utility)(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, int depth, double gr[graph_size][4]), int agentId, int depth, int maxDepth, double alpha, double beta)
{
 /*
   This function is the interface between your solution for the assignment and the driver code. The driver code
   in MiniMax_search_core_GL will call this function once per frame, and provide the following data
   
   Board and game layout:

	Exactly the same as for Assignment 1 - have a look at your code if you need a reminder of how the adjacency
	list and agent positions are stored.	

	Note that in this case, the path will contain a single move - at the top level, this function will provide
	an agent with the 'optimal' mini-max move given the game state.


   IMPORTANT NOTE: Mini-max is a recursive procedure. This function will need to fill-in the mini-max values for 
	 	   all game states down to the maximum search depth specified by the user. In order to do that,
		   the function needs to be called with the correct state at each specific node in the mini-max
		   search tree.

		   The game state is composed of:

			* Mouse, cat, and cheese positions (and number of cats and cheeses)
			
		   At the top level (when this function is called by the mini-max driver code), the game state
		   correspond to the current situation of the game. But once you start recursively calling
		   this function for lower levels of the search tree the positions of agents will have changed.
		   
		   Therefore, you will need to define local variables to keep the game state at each node of the
		   mini-max search tree, and you will need to update this state when calling recursively so that
		   the search does the right thing.

		   This function *must check* whether:
			* A candidate move results in a terminal configuration (cat eats mouse, mouse eats cheese)
			  at which point it calls the utility function to get a value
	 		* Maximum search depth has been reached (depth==maxDepth), at which point it will also call
			  the utility function to get a value
			* Otherwise, call recursively using the candidate configuration to find out what happens
			  deeper into the mini-max tree.

   Arguments:
		gr[graph_size][4]   		- This is an adjacency list for the maze
		path[1][2] 			- Your function will return the optimal mini-max move in this array.
		minmax_cost[size_X][size_Y]	- An array in which your code will store the
						  minimax value for maze locations expanded by
						  the search *when called for the mouse, not
						  for the cats!*

						  This array will be used to provide a visual 
						  display of minimax values during the game.

		cat_loc[10][2], cats   - Location of cats and number of cats (we can have at most 10,
					 but there can be fewer). Only valid cat locations are 0 to (cats-1)
		cheese_loc[10][2], cheeses - Location and number of cheese chunks (again at most 10,
					     but possibly fewer). Valid locations are 0 to (cheeses-1)
		mouse_loc[1][2] - Mouse location - there can be only one!
		mode - Search mode selection:
					mode = 0 	- No alpha-beta pruning
					mode = 1	- Alpha-beta pruning

		(*utility)(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, int depth, double gr[graph_size][4]);
				- This is a pointer to the utility function which returns a value for a specific game configuration

				   NOTE: Unlike the search assignment, this utility function also gets access to the graph so you can do any processing 					 
				   that requires knowledge of the maze for computing the utility values.

				  * How to call the utility function from within this function : *
					- Like any other function:
						u = utility(cat_loc, cheese_loc, mouse_loc, cats, cheeses, depth, gr);
						
		agentId: Identifies which agent we are doing MiniMax for. agentId=0 for the mouse, agentId in [1, cats] for cats. Notice that recursive calls
                         to this function should increase the agentId to reflect the fact that the next level down corresponds to the next agent! For a game
                         with two cats and a mouse, the agentIds for the recursion should look like 0, 1, 2, 0, 1, 2, ...
	
		depth: Current search depth - whether this is a MIN or a MAX node depends both on depth and agentId.
		
		maxDepth: maximum desired search depth - once reached, your code should somehow return
			  a minimax utility value for this location.

		alpha. beta: alpha and beta values passed from the parent node to constrain search at this
			     level.

   Return values:
		Your search code will directly update data passed-in as arguments:
		
		- Mini-Max value	: Notice this function returns a double precision number. This is
					  the minimax value at this level of the tree. It will be used 
					  as the recursion backtracks filling-in the mini-max values back
					  from the leaves to the root of the search tree. 

		- path[1][2]		: Your MiniMax function will return the location for the agent's 
					  next location (i.e. the optimal move for the agent). 
		- minmax_cost[size_X][size_Y] 	:  Your search code will update this array to contain the
						   minimax value for locations that were expanded during
						   the search. This must be done *only* for the mouse.

						   Values in this array will be in the range returned by
						   your utility function.

		* Your code MUST NOT modify the locations or numbers of cats and/or cheeses, the graph,
	     	  or the location of the mouse - if you try, the driver code will know it *
			
		That's that, now, implement your solution!
 */

 /********************************************************************************************************
 * 
 * TO DO:	Implement code to perform a MiniMax search. This will involve a limited-depth BFS-like
 *              expansion. Once nodes below return values, your function will propagate minimax utilities
 *		as per the minimax algorithm.
 *	
 *		Note that if alpha-beta pruning is specified, you must keep track of alphas and betas
 *		along the path.
 *
 *		You can use helper functions if it seems reasonable. Add them to the MiniMax_search.h
 *		file and explain in your code why they are needed and how they are used.
 *
 *		Recursion should appear somewhere.
 *
 *		MiniMax cost: If the agentId=0 (Mouse), then once you have a MiniMax value for a location
 *		in the maze, you must update minmax_cost[][] for that location.
 *
 *		How you design your solution is up to you. But:
 *
 *		- Document your implementation by adding concise and clear comments in this file
 *		- Document your design (how you implemented the solution, and why) in the report
 *
 ********************************************************************************************************/
// Regular MiniMax
if (mode == 0) {
	
	if (depth == maxDepth || checkForTerminal(mouse_loc, cat_loc, cheese_loc, cats, cheeses) == 1){
		return second_utility_function(cat_loc, cheese_loc, mouse_loc, cats, cheeses, depth, gr);
	}
	if (agentId == 0) {
		// Get mouse location index
		int current_mouse_index = get_graph_index(mouse_loc[0][0], mouse_loc[0][1]);
		// Check possible moves
		int max_node_val;
		int newAgentId = 1;
		int newDepth = depth + 1;
		// Array to store new mouse location
		int new_mouse_loc[1][2];
		// Stuct to store current mouse location
		struct graph_location mouse_location = get_graph_location(current_mouse_index);

		// Struct to store the best location
		struct graph_location best_location;
		
		// Check Top
		if (gr[current_mouse_index][0] == 1) {
			mouse_location.y = mouse_location.y - 1;
			new_mouse_loc[0][0] = mouse_location.x;
			new_mouse_loc[0][1] = mouse_location.y;
			int current_node_val = MiniMax(gr, path, minmax_cost, cat_loc, cats, cheese_loc, cheeses, new_mouse_loc, mode, utility, newAgentId, newDepth, maxDepth, alpha, beta);
			minmax_cost[mouse_location.x][mouse_location.y] = current_node_val;
			if (max_node_val){
				if(max_node_val < current_node_val){
					max_node_val = current_node_val;
					best_location = mouse_location;
				}
			} else {
				max_node_val = current_node_val;
				best_location = mouse_location;
			}
		}

		// Check Right
		if (gr[current_mouse_index][1] == 1) {
			mouse_location.x = mouse_location.x + 1;
			new_mouse_loc[0][0] = mouse_location.x;
			new_mouse_loc[0][1] = mouse_location.y;
			int current_node_val = MiniMax(gr, path, minmax_cost, cat_loc, cats, cheese_loc, cheeses, new_mouse_loc, mode, utility, newAgentId, newDepth, maxDepth, alpha, beta);
			minmax_cost[mouse_location.x][mouse_location.y] = current_node_val;
			if (max_node_val){
				if(max_node_val < current_node_val){
					max_node_val = current_node_val;
					best_location = mouse_location;
				}
			} else {
				max_node_val = current_node_val;
				best_location = mouse_location;
			}
		}

		// Check Bottom
		if (gr[current_mouse_index][2] == 1) {
			mouse_location.y = mouse_location.y + 1;
			new_mouse_loc[0][0] = mouse_location.x;
			new_mouse_loc[0][1] = mouse_location.y;
			int current_node_val = MiniMax(gr, path, minmax_cost, cat_loc, cats, cheese_loc, cheeses, new_mouse_loc, mode, utility, newAgentId, newDepth, maxDepth, alpha, beta);
			minmax_cost[mouse_location.x][mouse_location.y] = current_node_val;
			if (max_node_val){
				if(max_node_val < current_node_val){
					max_node_val = current_node_val;
					best_location = mouse_location;
				}
			} else {
				max_node_val = current_node_val;
				best_location = mouse_location;
			}
		}

		// Check Left
		if (gr[current_mouse_index][3] == 1) {
			mouse_location.x = mouse_location.x - 1;
			new_mouse_loc[0][0] = mouse_location.x;
			new_mouse_loc[0][1] = mouse_location.y;
			int current_node_val = MiniMax(gr, path, minmax_cost, cat_loc, cats, cheese_loc, cheeses, new_mouse_loc, mode, utility, newAgentId, newDepth, maxDepth, alpha, beta);
			minmax_cost[mouse_location.x][mouse_location.y] = current_node_val;
			if (max_node_val){
				if(max_node_val < current_node_val){
					max_node_val = current_node_val;
					best_location = mouse_location;
				}
			} else {
				max_node_val = current_node_val;
				best_location = mouse_location;
			}
		}
		// Update the path for the mouse
		path[0][0] = best_location.x;
		path[0][1] = best_location.y;
		// Return the best option for the Mouse
		return max_node_val;


	} 
	
	else {
		// increment depth
		int newDepth = depth + 1;
		int newAgentId;
		// If all the cats have made their move, pass the turn back to the mouse
		if (agentId == cats) {
			newAgentId = 0;
		} else {
			// Increase AgentId and Depth
			newAgentId = agentId + 1;
		}

		// Get cat location index
		int currentCat = agentId - 1;
		int current_cat_index = get_graph_index(cat_loc[currentCat][0], cat_loc[currentCat][1]);
		
		// Store min value from available options
		int min_node_val;
		
		// Array to store new cat location
		int new_cat_loc[10][2];
		for (int i = 0; i < cats; i++){
			new_cat_loc[i][0] = cat_loc[i][0];
			new_cat_loc[i][1] = cat_loc[i][1];
		}

		// Stuct to store current mouse location
		struct graph_location cat_location = get_graph_location(current_cat_index);
		
		// Check Top
		if (gr[current_cat_index][0] == 1) {
			cat_location.y = cat_location.y - 1;
			new_cat_loc[currentCat][0] = cat_location.x;
			new_cat_loc[currentCat][1] = cat_location.y;
			int current_node_val = MiniMax(gr, path, minmax_cost, new_cat_loc, cats, cheese_loc, cheeses, mouse_loc, mode, utility, newAgentId, newDepth, maxDepth, alpha, beta);
			if (min_node_val){
				if(min_node_val > current_node_val){
					min_node_val = current_node_val;
				}
			} else {
				min_node_val = current_node_val;
			}
		}

		// Check Right
		if (gr[current_cat_index][1] == 1) {
			cat_location.x = cat_location.x + 1;
			new_cat_loc[currentCat][0] = cat_location.x;
			new_cat_loc[currentCat][1] = cat_location.y;
			int current_node_val = MiniMax(gr, path, minmax_cost, new_cat_loc, cats, cheese_loc, cheeses, mouse_loc, mode, utility, newAgentId, newDepth, maxDepth, alpha, beta);
			if (min_node_val){
				if(min_node_val > current_node_val){
					min_node_val = current_node_val;
				}
			} else {
				min_node_val = current_node_val;
			}
		}

		// Check Bottom
		if (gr[current_cat_index][2] == 1) {
			cat_location.y = cat_location.y + 1;
			new_cat_loc[currentCat][0] = cat_location.x;
			new_cat_loc[currentCat][1] = cat_location.y;
			int current_node_val = MiniMax(gr, path, minmax_cost, new_cat_loc, cats, cheese_loc, cheeses, mouse_loc, mode, utility, newAgentId, newDepth, maxDepth, alpha, beta);
			if (min_node_val){
				if(min_node_val > current_node_val){
					min_node_val = current_node_val;
				}
			} else {
				min_node_val = current_node_val;
			}
		}

		// Check Left
		if (gr[current_cat_index][3] == 1) {
			cat_location.x = cat_location.x - 1;
			new_cat_loc[currentCat][0] = cat_location.x;
			new_cat_loc[currentCat][1] = cat_location.y;
			int current_node_val = MiniMax(gr, path, minmax_cost, new_cat_loc, cats, cheese_loc, cheeses, mouse_loc, mode, utility, newAgentId, newDepth, maxDepth, alpha, beta);
			if (min_node_val){
				if(min_node_val > current_node_val){
					min_node_val = current_node_val;
				}
			} else {
				min_node_val = current_node_val;
			}
		}
		// return the best option for the cat
		return min_node_val;	
	}


}
// Alpha-Beta Pruning
else if (mode == 1) {

}
else {
	printf("\nSelect a valid mode (0 or 1)");
}
 
 // Stub so that the code compiles/runs - This will be removed and replaced by your code!

 path[0][0]=mouse_loc[0][0];
 path[0][1]=mouse_loc[0][1];

 return(0.0);
}

double utility(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, int depth, double gr[graph_size][4])
{
 /*
	This function computes and returns the utility value for a given game configuration.
	As discussed in lecture, this should return a positive value for configurations that are 'good'
	for the mouse, and a negative value for locations that are 'bad' for the mouse.

	How to define 'good' and 'bad' is up to you. Note that you can write a utility function
	that favours your mouse or favours the cats, but that would be a bad idea... (why?)

	Input arguments:

		cat_loc - Cat locations
		cheese_loc - Cheese locations
		mouse_loc - Mouse location
		cats - # of cats
		cheeses - # of cheeses
		depth - current search depth
		gr - The graph's adjacency list for the maze

		These arguments are as described in A1. Do have a look at your solution!
 */
	int utility_cost;
	// Store constants for worse/best case scenarios
	const int LARGE_POSITIVE = graph_size;
	const int LARGE_NEGATIVE = -1 * graph_size;
	// Get the distance to closest cheese using helper function
	int distance_to_closest_cheese = distanceToClosestItem(mouse_loc, cheese_loc, cheeses);
	// Get the distance to closest cat using helper function
	int distance_to_closest_cat = distanceToClosestItem(mouse_loc, cat_loc, cats);

	if (distance_to_closest_cat <= 1) {
		utility_cost = LARGE_NEGATIVE;
	}
	return utility_cost;
//  return(1);   // <--- Obviously, this will be replaced by your computer utilities
}


double new_utility(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, int depth, double gr[graph_size][4], int prev_distance_cheese, int prev_distance_cat)
{
 /*
	This function computes and returns the utility value for a given game configuration.
	As discussed in lecture, this should return a positive value for configurations that are 'good'
	for the mouse, and a negative value for locations that are 'bad' for the mouse.

	How to define 'good' and 'bad' is up to you. Note that you can write a utility function
	that favours your mouse or favours the cats, but that would be a bad idea... (why?)

	Input arguments:

		cat_loc - Cat locations
		cheese_loc - Cheese locations
		mouse_loc - Mouse location
		cats - # of cats
		cheeses - # of cheeses
		depth - current search depth
		gr - The graph's adjacency list for the maze

		These arguments are as described in A1. Do have a look at your solution!
 */
	// Try changing to see performance
	const int CONSTANT_FACTOR = 1;
	double utility_cost;
	// // Store constants for worse/best case scenarios
	// const int LARGE_POSITIVE = graph_size;
	// const int LARGE_NEGATIVE = -1 * graph_size;

	int new_distance_cat = distanceToClosestItem(mouse_loc, cat_loc, cats);
	int new_distance_cheese = distanceToClosestItem(mouse_loc, cheese_loc, cheeses);

	// Difference in Manhattan distance (mouse to cat) from initial configuration to new configuration
	int difference_in_cat_distance = new_distance_cat - prev_distance_cat;
	// Difference in Manhattan distance (mouse to cheese) from initial configuration to new configuration
	int difference_in_cheese_distance = prev_distance_cheese - new_distance_cheese;

	utility_cost = (CONSTANT_FACTOR * (difference_in_cat_distance)) + difference_in_cheese_distance;

	return utility_cost;
}

double second_utility_function(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, int depth, double gr[graph_size][4])
{
	// Distance to closest cat and closest cheese
	int dist_mouse_to_cheese = distanceToClosestItem(mouse_loc, cheese_loc, cheeses);
	int dist_mouse_to_cat= distanceToClosestItem(mouse_loc, cat_loc, cats);   

	double status;
	// Mouse is of equal distance from the mouse and cat
	if (dist_mouse_to_cheese == dist_mouse_to_cat){
		status = 0;
	}
	// The mouse is closer to the cheese / the cat is farther from the mouse
	else if (dist_mouse_to_cheese < dist_mouse_to_cat){
		status = 1;
	}
	// The mouse is farther from the cheese / the cat is closer to the mouse
	else {
		status = -1;
	}

	return status * (dist_mouse_to_cheese + dist_mouse_to_cat);
}

int checkForTerminal(int mouse_loc[1][2],int cat_loc[10][2],int cheese_loc[10][2],int cats,int cheeses)
{
 /* 
   This function determines whether a given configuration constitutes a terminal node.
   Terminal nodes are those for which:
     - A cat eats the mouse
     or
     - The mouse eats a cheese
   
   If the node is a terminal, the function returns 1, else it returns 0
 */

 // Check for cats having lunch
 for (int i=0; i<cats; i++)
  if (mouse_loc[0][0]==cat_loc[i][0]&&mouse_loc[0][1]==cat_loc[i][1]) return(1);

 // Check for mouse having lunch
 for (int i=0; i<cheeses; i++)
  if (mouse_loc[0][0]==cheese_loc[i][0]&&mouse_loc[0][1]==cheese_loc[i][1]) return(1);

 return(0);

}



 /********************************************************************************************************
  * Helper functions from A1
 ********************************************************************************************************/
/**
 * Convert the location to an index in the graph and return it
 **/
int get_graph_index(int x, int y)
{
	return x + (y * size_X);
}

/**
 * Convert the index to a location in the graph and return it
 **/
struct graph_location get_graph_location(int index)
{
	struct graph_location location;
	// Corresponding (x,y) location
	location.x = index % size_X;
	location.y = index / size_Y;
	return location;
}

/**
 * Calculate the Manhattan distance of two points on a graph  
 **/
int calculate_manhattan_distance(struct graph_location p1, struct graph_location p2)
{
	const int LEN_SIDE_A = abs(p2.x - p1.x);
	const int LEN_SIDE_B = abs(p2.y - p1.y);
	return (int) (LEN_SIDE_A + LEN_SIDE_B);
}
/**
 * Calculate the distance to closest cheese or closest cat
 * 
 **/
int distanceToClosestItem(int mouse_loc[1][2], int item_loc[10][2], int num_items){
	
	// Store the mouse location
	int mouse_index = get_graph_index(mouse_loc[0][0], mouse_loc[0][1]);
	struct graph_location mouse_location = get_graph_location(mouse_index);
	
	// Store the closest cheese/cat location
	struct graph_location min_item_location;

	int distances[num_items]; // stores Manhattan distance from location of mouse to each cheese/cat
	int min_item_index = 0; // Index to help remember the min index. Initially first cheese/cat
	int distance_to_closest_item;

	for (int item_index = 0; item_index < num_items; item_index++)
	{
		struct graph_location item_location;
		item_location.x = item_loc[item_index][0];
		item_location.y = item_loc[item_index][1];

		distances[item_index] = calculate_manhattan_distance(mouse_location, item_location);
		
		// Get the min value from distances array
		if (item_index != min_item_index && distances[item_index] < distances[min_item_index])
		{
			min_item_index = item_index;
		}
	}

	min_item_location.x = item_loc[min_item_index][0];
	min_item_location.y = item_loc[min_item_index][1];

	distance_to_closest_item = distances[min_item_index];
	return distance_to_closest_item;
}