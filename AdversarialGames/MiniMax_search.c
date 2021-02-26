#include "MiniMax_search.h"
/**
 * Function MiniMax uses the MiniMaxHelper function for mode 0 (regular MiniMax)
 * and AlphaBetaPruning function for mode 1 (Alpha Beta Pruning)
 */
double MiniMax(double gr[graph_size][4], int path[1][2], double minmax_cost[size_X][size_Y], int cat_loc[10][2], 
	int cats, int cheese_loc[10][2], int cheeses, int mouse_loc[1][2], int mode, 
	double (*utility)(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, int depth, double gr[graph_size][4]), 
	int agentId, int depth, int maxDepth, double alpha, double beta)
{
	// =================== Regular MiniMax =================== //
	if (mode == 0) {
		return MiniMax_Helper(gr, path, minmax_cost, cat_loc, cats, 
			cheese_loc, cheeses, mouse_loc, mode, utility, agentId, depth, maxDepth, alpha, beta);
	}
	// =================== Alpha Beta Pruning =================== //
	else if (mode == 1) {
		alpha = -999999;
		beta = 999999;
		return AlphaBetaPruning(gr, path, minmax_cost, cat_loc, cats, 
			cheese_loc, cheeses, mouse_loc, mode, utility, agentId, depth, maxDepth, alpha, beta);
	}
	return(0.0);
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
  ************************************* Different Utility Functions **************************************
 ********************************************************************************************************/
double utility(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, int depth, double gr[graph_size][4])
{
	double utility_cost;

	int distance_to_cat = distanceToClosestItem(mouse_loc, cat_loc, cats);
	int distance_to_cheese = distanceToClosestItem(mouse_loc, cheese_loc, cheeses);

	if (distance_to_cat <= 1){
		utility_cost = -50000;
	} else if (distance_to_cheese <= 1){
		utility_cost = 50000;
	} else {
		utility_cost = (10 * distance_to_cheese) - (5 * distance_to_cat);
	}
	return utility_cost;
}

double first_utility_function(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, int depth, double gr[graph_size][4])
{

	double utility_cost;

	int distance_to_cat = distanceToClosestItem(mouse_loc, cat_loc, cats);
	int distance_to_cheese = distanceToClosestItem(mouse_loc, cheese_loc, cheeses);
	int distance_1 = distanceToClosestItem(cat_loc, cheese_loc, cheeses);
	

	if (distance_to_cat <= 1){
		utility_cost = -500;
	} else if (distance_to_cheese <= 1){
		utility_cost = 500;
	} else {
		int num_walls = 0;
		
		for (int i = 0; i < graph_size; i ++ ){
			for (int j = 0; j < 4; j ++){
				if (gr[i][j] == 0)
					num_walls ++;
			}
			
		}
		utility_cost = ((10 * distance_to_cheese)) - (5 * distance_to_cat ) + (num_walls);
	}
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

	return status * H_cost_nokitty(mouse_loc[0][0], mouse_loc[0][1], cat_loc, cheese_loc, mouse_loc, cats, cheeses, gr);
}

 /********************************************************************************************************
  ***************************************** New Helper Functions *****************************************
 ********************************************************************************************************/


/**
 * This function is used by MiniMax 
 * It has some extra parameters such as visited nodes and previous distance to closest cat and cheese
 * which are used to calculate different utilities
 **/
double MiniMax_Helper(double gr[graph_size][4], int path[1][2], double minmax_cost[size_X][size_Y], int cat_loc[10][2], int cats, int cheese_loc[10][2], int cheeses, int mouse_loc[1][2], 
	int mode, double (*utility)(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, int depth, double gr[graph_size][4]), 
	int agentId, int depth, int maxDepth, double alpha, double beta){
	
	// Return utility cost if we've reached max depth or terminal point
	if (depth == maxDepth || checkForTerminal(mouse_loc, cat_loc, cheese_loc, cats, cheeses) == 1){
		// return utility(cat_loc, cheese_loc, mouse_loc, cats, cheeses, depth, gr);
		return first_utility_function(cat_loc, cheese_loc, mouse_loc, cats, cheeses, depth, gr);
		// return second_utility_function(cat_loc, cheese_loc, mouse_loc, cats, cheeses, depth, gr);
	}
	
	// Agent is Mouse
	if (agentId == 0) {
		// Declare variables to keep track of data
		double max_node_val;
		// ========== new code ================
		double* max_node_val_ptr = &max_node_val;
		// =========== end new code =============
		int newAgentId = 1;
		int newDepth = depth + 1;

		// Flag to track if best location has been assigned or not
		int has_best_location = 0;

		// Array to store new mouse location
		int new_mouse_loc[1][2];
		// Struct to store the best location (i.e. best move for the mouse)
		struct graph_location best_location;
		
		// Get mouse location index
		int current_mouse_index = get_graph_index(mouse_loc[0][0], mouse_loc[0][1]);


		// Check if mouse can move up
		if (gr[current_mouse_index][0] == 1) {
			// New coordinates for the mouse
			// Stuct to store mouse location
			struct graph_location mouse_location = get_graph_location(current_mouse_index);
			mouse_location.y = mouse_location.y - 1;
			new_mouse_loc[0][0] = mouse_location.x;
			new_mouse_loc[0][1] = mouse_location.y;
			
			// Recursively check MiniMax_Helper for moving the mouse up
			double current_node_val = MiniMax_Helper(gr, path, minmax_cost, cat_loc, cats, cheese_loc, cheeses, new_mouse_loc, 
				mode, utility, newAgentId, newDepth, maxDepth, alpha, beta);

			// Update the minimax_cost array
			minmax_cost[mouse_location.x][mouse_location.y] = current_node_val;

			// Ensure agent (mouse) picks the largest value (maximizes)
			// Update best location
			if (max_node_val_ptr != NULL || max_node_val){
				if(max_node_val <= current_node_val){
					max_node_val = current_node_val;
					best_location = mouse_location;
					has_best_location = 1;
				}
			} else {
				max_node_val = current_node_val;
				best_location = mouse_location;
				has_best_location = 1;
			}
		}

		// Check if mouse can move right
		if (gr[current_mouse_index][1] == 1) {
			// New coordinates for the mouse
			// Stuct to store mouse location
			struct graph_location mouse_location = get_graph_location(current_mouse_index);
			mouse_location.x = mouse_location.x + 1;
			new_mouse_loc[0][0] = mouse_location.x;
			new_mouse_loc[0][1] = mouse_location.y;

			// Recursively check MiniMax_Helper for moving the mouse right
			double current_node_val = MiniMax_Helper(gr, path, minmax_cost, cat_loc, cats, cheese_loc, cheeses, new_mouse_loc, 
				mode, utility, newAgentId, newDepth, maxDepth, alpha, beta);

			// Update the minimax_cost array
			minmax_cost[mouse_location.x][mouse_location.y] = current_node_val;

			// Ensure agent (mouse) picks the largest value (maximizes)
			// Update best location
			if (max_node_val_ptr != NULL || max_node_val){
				if(max_node_val <= current_node_val){
					max_node_val = current_node_val;
					best_location = mouse_location;
					has_best_location = 1;
				}
			} else {
				max_node_val = current_node_val;
				best_location = mouse_location;
				has_best_location = 1;
			}
		}

		// Check if mouse can move bottom
		if (gr[current_mouse_index][2] == 1) {
			// New coordinates for the mouse
			// Stuct to store mouse location
			struct graph_location mouse_location = get_graph_location(current_mouse_index);
			mouse_location.y = mouse_location.y + 1;
			new_mouse_loc[0][0] = mouse_location.x;
			new_mouse_loc[0][1] = mouse_location.y;

			// Recursively check MiniMax_Helper for moving the mouse bottom
			double current_node_val = MiniMax_Helper(gr, path, minmax_cost, cat_loc, cats, cheese_loc, cheeses, new_mouse_loc, 
				mode, utility, newAgentId, newDepth, maxDepth, alpha, beta);
			
			// Update the minimax_cost array
			minmax_cost[mouse_location.x][mouse_location.y] = current_node_val;

			// Ensure agent (mouse) picks the largest value (maximizes)
			// Update best location
			if (max_node_val_ptr != NULL || max_node_val){
				if(max_node_val <= current_node_val){
					max_node_val = current_node_val;
					best_location = mouse_location;
					has_best_location = 1;
				}
			} else {
				max_node_val = current_node_val;
				best_location = mouse_location;
				has_best_location = 1;
			}
		}

		// Check if mouse can move left
		if (gr[current_mouse_index][3] == 1) {
			// New coordinates for the mouse
			// Stuct to store mouse location
			struct graph_location mouse_location = get_graph_location(current_mouse_index);
			mouse_location.x = mouse_location.x - 1;
			new_mouse_loc[0][0] = mouse_location.x;
			new_mouse_loc[0][1] = mouse_location.y;
			
			// Recursively check MiniMax_Helper for moving the mouse left
			double current_node_val = MiniMax_Helper(gr, path, minmax_cost, cat_loc, cats, cheese_loc, cheeses, new_mouse_loc, 
				mode, utility, newAgentId, newDepth, maxDepth, alpha, beta);
			
			// Update the minimax_cost array
			minmax_cost[mouse_location.x][mouse_location.y] = current_node_val;

			// Ensure agent (mouse) picks the largest value (maximizes)
			// Update best location
			if (max_node_val_ptr != NULL || max_node_val){
				if(max_node_val <= current_node_val){
					max_node_val = current_node_val;
					best_location = mouse_location;
					has_best_location = 1;
				}
			} else {
				max_node_val = current_node_val;
				best_location = mouse_location;
				has_best_location = 1;
			}
		}
		if (has_best_location == 1){
			// Update the path for the mouse
			path[0][0] = best_location.x;
			path[0][1] = best_location.y;
			// printf("Path: [%d, %d]\n", path[0][0], path[0][1]);
		} 
		
		// Return the best option for the Mouse
		return max_node_val;
	} 
	
	// Agent is Cat
	else {
		// Increment depth
		int newDepth = depth + 1;
		int newAgentId;

		// If all the cats have made their move, pass the turn back to the mouse
		if (agentId == cats) {
			newAgentId = 0;
		} else {
			// Increase AgentId 
			newAgentId = agentId + 1;
		}

		// Get cat location index
		int currentCat = agentId - 1;
		int current_cat_index = get_graph_index(cat_loc[currentCat][0], cat_loc[currentCat][1]);
		
		// Store min value from available options
		double min_node_val;
		// ========= new code ==============
		double* min_node_val_ptr = &min_node_val;
		// ========= end new code ============
		// Array to store new cat location
		int new_cat_loc[10][2];
		for (int i = 0; i < cats; i++){
			new_cat_loc[i][0] = cat_loc[i][0];
			new_cat_loc[i][1] = cat_loc[i][1];
		}

		
		
		// Check if cat can move up
		if (gr[current_cat_index][0] == 1) {
			// New coordinates for the cat
			// Stuct to store current mouse location
			struct graph_location cat_location = get_graph_location(current_cat_index);
			cat_location.y = cat_location.y - 1;
			new_cat_loc[currentCat][0] = cat_location.x;
			new_cat_loc[currentCat][1] = cat_location.y;
			
			// Recursively check MiniMax_Helper for moving the cat up
			double current_node_val = MiniMax_Helper(gr, path, minmax_cost, new_cat_loc, cats, cheese_loc, cheeses, mouse_loc,
				 mode, utility, newAgentId, newDepth, maxDepth, alpha, beta);
			
			// Ensure agent (cat) picks the smallest value (minimizes)
			if (min_node_val_ptr != NULL || min_node_val){
				if(min_node_val >= current_node_val){
					min_node_val = current_node_val;
				}
			} else {
				min_node_val = current_node_val;
			}
		}

		// Check if cat can move right
		if (gr[current_cat_index][1] == 1) {
			// New coordinates for the cat
			// Stuct to store current mouse location
			struct graph_location cat_location = get_graph_location(current_cat_index);
			cat_location.x = cat_location.x + 1;
			new_cat_loc[currentCat][0] = cat_location.x;
			new_cat_loc[currentCat][1] = cat_location.y;
			
			// Recursively check MiniMax_Helper for moving the cat right
			double current_node_val = MiniMax_Helper(gr, path, minmax_cost, new_cat_loc, cats, cheese_loc, cheeses, mouse_loc, 
				mode, utility, newAgentId, newDepth, maxDepth, alpha, beta);
			
			// Ensure agent (cat) picks the smallest value (minimizes)
			if (min_node_val_ptr != NULL || min_node_val){
				if(min_node_val >= current_node_val){
					min_node_val = current_node_val;
				}
			} else {
				min_node_val = current_node_val;
			}
		}

		// Check if cat can move bottom
		if (gr[current_cat_index][2] == 1) {
			// New coordinates for the cat
			// Stuct to store current mouse location
			struct graph_location cat_location = get_graph_location(current_cat_index);
			cat_location.y = cat_location.y + 1;
			new_cat_loc[currentCat][0] = cat_location.x;
			new_cat_loc[currentCat][1] = cat_location.y;
			
			// Recursively check MiniMax_Helper for moving the cat bottom
			double current_node_val = MiniMax_Helper(gr, path, minmax_cost, new_cat_loc, cats, cheese_loc, cheeses, mouse_loc, 
				mode, utility, newAgentId, newDepth, maxDepth, alpha, beta);
			
			// Ensure agent (cat) picks the smallest value (minimizes)
			if (min_node_val_ptr != NULL || min_node_val){
				if(min_node_val >= current_node_val){
					min_node_val = current_node_val;
				}
			} else {
				min_node_val = current_node_val;
			}
		}

		// Check if cat can move left
		if (gr[current_cat_index][3] == 1) {
			
			// New coordinates for the cat
			// Stuct to store current mouse location
			struct graph_location cat_location = get_graph_location(current_cat_index);
			cat_location.x = cat_location.x - 1;
			new_cat_loc[currentCat][0] = cat_location.x;
			new_cat_loc[currentCat][1] = cat_location.y;
			
			// Recursively check MiniMax_Helper for moving the cat left
			double current_node_val = MiniMax_Helper(gr, path, minmax_cost, new_cat_loc, cats, cheese_loc, cheeses, mouse_loc, 
				mode, utility, newAgentId, newDepth, maxDepth, alpha, beta);
			
			// Ensure agent (cat) picks the smallest value (minimizes)
			if (min_node_val_ptr != NULL || min_node_val){
				if(min_node_val >= current_node_val){
					min_node_val = current_node_val;
				}
			} else {
				min_node_val = current_node_val;
			}
		}
		if (min_node_val_ptr == NULL)
			min_node_val = 0.0;
		// return the best option for the cat
		return min_node_val;	
	}
}


/**
 * AlphaBetaPruning is a very similar function to MiniMax Helper
 * It stops traversing the tree (prunes) when alpha >= beta
 **/
double AlphaBetaPruning(double gr[graph_size][4], int path[1][2], double minmax_cost[size_X][size_Y], int cat_loc[10][2], int cats, int cheese_loc[10][2], int cheeses, int mouse_loc[1][2], 
	int mode, double (*utility)(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, int depth, double gr[graph_size][4]), 
	int agentId, int depth, int maxDepth, double alpha, double beta) {
	
	// Return utility cost if we've reached max depth or terminal point
	if (depth == maxDepth || checkForTerminal(mouse_loc, cat_loc, cheese_loc, cats, cheeses) == 1){
		return utility(cat_loc, cheese_loc, mouse_loc, cats, cheeses, depth, gr);
		// return first_utility_function(cat_loc, cheese_loc, mouse_loc, cats, cheeses, depth, gr);
		// return second_utility_function(cat_loc, cheese_loc, mouse_loc, cats, cheeses, depth, gr);
	}

	// Agent is Mouse
	if (agentId == 0) {
		
		// Declare variables to keep track of data
		double max_node_val = -999999;
		int newAgentId = 1;
		int newDepth = depth + 1;

		// Array to store new mouse location
		int new_mouse_loc[1][2];
		// Struct to store the best location (i.e. best move for the mouse)
		struct graph_location best_location;
		// Get mouse location index
		int current_mouse_index = get_graph_index(mouse_loc[0][0], mouse_loc[0][1]);
		// Stuct to store mouse location
		struct graph_location mouse_location = get_graph_location(current_mouse_index);

		// Check if mouse can move up
		if (gr[current_mouse_index][0] == 1 && alpha < beta) {
			// New coordinates for the mouse
			mouse_location.y = mouse_location.y - 1;
			new_mouse_loc[0][0] = mouse_location.x;
			new_mouse_loc[0][1] = mouse_location.y;
			
			// Recursively check AlphaBetaPruning for moving the mouse up
			double current_node_val = AlphaBetaPruning(gr, path, minmax_cost, cat_loc, cats, cheese_loc, cheeses, new_mouse_loc, 
				mode, utility, newAgentId, newDepth, maxDepth, alpha, beta);

			// Update the minimax_cost array
			minmax_cost[mouse_location.x][mouse_location.y] = current_node_val;

			// Compare current_node_val with max_node_val
			if(max_node_val < current_node_val){
				max_node_val = current_node_val;
				best_location = mouse_location;
			}

			// Update alpha, if max_node_value > alpha
			if (alpha < max_node_val){
				alpha = max_node_val;
			}		
		}

		// Check if mouse can move right
		if (gr[current_mouse_index][1] == 1 && alpha < beta) {
			// New coordinates for the mouse
			mouse_location.x = mouse_location.x + 1;
			new_mouse_loc[0][0] = mouse_location.x;
			new_mouse_loc[0][1] = mouse_location.y;

			// Recursively check AlphaBetaPruning for moving the mouse right
			double current_node_val = AlphaBetaPruning(gr, path, minmax_cost, cat_loc, cats, cheese_loc, cheeses, new_mouse_loc, 
				mode, utility, newAgentId, newDepth, maxDepth, alpha, beta);

			// Update the minimax_cost array
			minmax_cost[mouse_location.x][mouse_location.y] = current_node_val;

			// Compare current_node_val with max_node_val
			if(max_node_val < current_node_val){
				max_node_val = current_node_val;
				best_location = mouse_location;
			}

			// Update alpha, if max_node_value > alpha
			if (alpha < max_node_val){
				alpha = max_node_val;
			}	
		}

		// Check if mouse can move bottom
		if (gr[current_mouse_index][2] == 1 && alpha < beta) {
			// New coordinates for the mouse
			mouse_location.y = mouse_location.y + 1;
			new_mouse_loc[0][0] = mouse_location.x;
			new_mouse_loc[0][1] = mouse_location.y;

			// Recursively check AlphaBetaPruning for moving the mouse bottom
			double current_node_val = AlphaBetaPruning(gr, path, minmax_cost, cat_loc, cats, cheese_loc, cheeses, new_mouse_loc, 
				mode, utility, newAgentId, newDepth, maxDepth, alpha, beta);
			
			// Update the minimax_cost array
			minmax_cost[mouse_location.x][mouse_location.y] = current_node_val;

			// Compare current_node_val with max_node_val
			if(max_node_val < current_node_val){
				max_node_val = current_node_val;
				best_location = mouse_location;
			}

			// Update alpha, if max_node_value > alpha
			if (alpha < max_node_val){
				alpha = max_node_val;
			}	
		}

		// Check if mouse can move left
		if (gr[current_mouse_index][3] == 1 && alpha < beta) {
			// New coordinates for the mouse
			mouse_location.x = mouse_location.x - 1;
			new_mouse_loc[0][0] = mouse_location.x;
			new_mouse_loc[0][1] = mouse_location.y;
			
			// Recursively check AlphaBetaPruning for moving the mouse left
			double current_node_val = AlphaBetaPruning(gr, path, minmax_cost, cat_loc, cats, cheese_loc, cheeses, new_mouse_loc, 
				mode, utility, newAgentId, newDepth, maxDepth, alpha, beta);
			
			// Update the minimax_cost array
			minmax_cost[mouse_location.x][mouse_location.y] = current_node_val;

			// Compare current_node_val with max_node_val
			if(max_node_val < current_node_val){
				max_node_val = current_node_val;
				best_location = mouse_location;
			}

			// Update alpha, if max_node_value > alpha
			if (alpha < max_node_val){
				alpha = max_node_val;
			}	
		}
		
		// Update the path for the mouse
		path[0][0] = best_location.x;
		path[0][1] = best_location.y;
		// Return the best option for the Mouse
		return max_node_val;
	} 
	
	// Agent is Cat
	else {
		// Increment depth
		int newDepth = depth + 1;
		int newAgentId;

		// If all the cats have made their move, pass the turn back to the mouse
		if (agentId == cats) {
			newAgentId = 0;
		} else {
			// Increase AgentId 
			newAgentId = agentId + 1;
		}

		// Get cat location index
		int currentCat = agentId - 1;
		int current_cat_index = get_graph_index(cat_loc[currentCat][0], cat_loc[currentCat][1]);
		
		// Store min value from available options
		double min_node_val;
		
		// Array to store new cat location
		int new_cat_loc[10][2];
		for (int i = 0; i < cats; i++){
			new_cat_loc[i][0] = cat_loc[i][0];
			new_cat_loc[i][1] = cat_loc[i][1];
		}

		// Stuct to store current mouse location
		struct graph_location cat_location = get_graph_location(current_cat_index);
		
		// Check if cat can move up
		if (gr[current_cat_index][0] == 1 && alpha < beta) {
			// Initialize min node value to a really large number
			min_node_val = 999999;
			// New coordinates for the cat
			cat_location.y = cat_location.y - 1;
			new_cat_loc[currentCat][0] = cat_location.x;
			new_cat_loc[currentCat][1] = cat_location.y;
			
			// Recursively check AlphaBetaPruning for moving the cat up
			double current_node_val = AlphaBetaPruning(gr, path, minmax_cost, new_cat_loc, cats, cheese_loc, cheeses, mouse_loc,
				 mode, utility, newAgentId, newDepth, maxDepth, alpha, beta);
			
			// Ensure agent (cat) picks the smallest value (minimizes)
			if(min_node_val > current_node_val){
				min_node_val = current_node_val;
			}

			// update beta if min_node_val < beta
			if(beta > min_node_val){
				beta = min_node_val;
			}
			
		}

		// Check if cat can move right
		if (gr[current_cat_index][1] == 1 && alpha < beta) {
			// Initialize min node value to a really large number
			min_node_val = 999999;
			// New coordinates for the cat
			cat_location.x = cat_location.x + 1;
			new_cat_loc[currentCat][0] = cat_location.x;
			new_cat_loc[currentCat][1] = cat_location.y;
			
			// Recursively check AlphaBetaPruning for moving the cat right
			double current_node_val = AlphaBetaPruning(gr, path, minmax_cost, new_cat_loc, cats, cheese_loc, cheeses, mouse_loc, 
				mode, utility, newAgentId, newDepth, maxDepth, alpha, beta);
			
			// Ensure agent (cat) picks the smallest value (minimizes)
			if(min_node_val > current_node_val){
				min_node_val = current_node_val;
			}

			// update beta if min_node_val < beta
			if(beta > min_node_val){
				beta = min_node_val;
			}
		}

		// Check if cat can move bottom
		if (gr[current_cat_index][2] == 1 && alpha < beta) {
			// Initialize min node value to a really large number
			min_node_val = 999999;
			// New coordinates for the cat
			cat_location.y = cat_location.y + 1;
			new_cat_loc[currentCat][0] = cat_location.x;
			new_cat_loc[currentCat][1] = cat_location.y;
			
			// Recursively check AlphaBetaPruning for moving the cat bottom
			double current_node_val = AlphaBetaPruning(gr, path, minmax_cost, new_cat_loc, cats, cheese_loc, cheeses, mouse_loc, 
				mode, utility, newAgentId, newDepth, maxDepth, alpha, beta);
			
			// Ensure agent (cat) picks the smallest value (minimizes)
			if(min_node_val > current_node_val){
				min_node_val = current_node_val;
			}

			// update beta if min_node_val < beta
			if(beta > min_node_val){
				beta = min_node_val;
			}
		}

		// Check if cat can move left
		if (gr[current_cat_index][3] == 1 && alpha < beta) {
			// Initialize min node value to a really large number
			min_node_val = 999999;
			// New coordinates for the cat
			cat_location.x = cat_location.x - 1;
			new_cat_loc[currentCat][0] = cat_location.x;
			new_cat_loc[currentCat][1] = cat_location.y;
			
			// Recursively check AlphaBetaPruning for moving the cat left
			double current_node_val = AlphaBetaPruning(gr, path, minmax_cost, new_cat_loc, cats, cheese_loc, cheeses, mouse_loc, 
				mode, utility, newAgentId, newDepth, maxDepth, alpha, beta);
			
			// Ensure agent (cat) picks the smallest value (minimizes)
			if(min_node_val > current_node_val){
				min_node_val = current_node_val;
			}

			// update beta if min_node_val < beta
			if(beta > min_node_val){
				beta = min_node_val;
			}
		}
		// return the best option for the cat
		return min_node_val;	
	}
}



 /********************************************************************************************************
  ********************************** Helper Functions From Assignment 1 **********************************
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
/**
 * H_cost_nokitty used in Assignment 1
 **/ 
int H_cost_nokitty(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, double gr[graph_size][4])
{
	struct graph_location location;
	// Store the closest cheese location
	struct graph_location min_cheese_location;

	int distances[cheeses][cats]; // stores Manhattan distance from (x,y) to each cheese relative to cats
	int min_cheese_cat_index_x = 0; // Index to help remember the min index. Initially first cheese
	int min_cheese_cat_index_y = 0; // Index to help remember the min index. Initially first cheese
	int cost;

	location.x = x;
	location.y = y;

	for (int i = 0; i < cheeses; i++)
	{
		struct graph_location cheese_location;
		cheese_location.x = cheese_loc[i][0];
		cheese_location.y = cheese_loc[i][1];

		int temp_distance_cheese = calculate_manhattan_distance(location, cheese_location);

		for (int j = 0; j < cats; j++) {
			struct graph_location cat_location;
			cat_location.x = cat_loc[j][0];
			cat_location.y = cat_loc[j][1];
			int temp_distance_cat = calculate_manhattan_distance(cheese_location, cat_location);
			distances[i][j] = round(temp_distance_cheese - temp_distance_cat);

			// Check if the distance from the (x,y) to index i is shorter than
			// the distance from the (x,y) to the min index
			if (distances[i][j] < distances[min_cheese_cat_index_x][min_cheese_cat_index_y])
			{
				min_cheese_cat_index_x = i;
				min_cheese_cat_index_y = j;
			}
		}	
	}

	// Calculate cost by how much the distance to cheese is shortened
	// when compared to the distance where the mouse currently is
	cost = distances[min_cheese_cat_index_x][min_cheese_cat_index_y];

	return cost; 
}