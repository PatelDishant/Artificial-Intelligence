/*
	CSC D84 - Unit 3 - Reinforcement Learning
	
	This file contains stubs for implementing the Q-Learning method
	for reinforcement learning as discussed in lecture. You have to
	complete two versions of Q-Learning.
	
	* Standard Q-Learning, based on a full-state representation and
	  a large Q-Table
	* Feature based Q-Learning to handle problems too big to allow
	  for a full-state representation
	    
	Read the assignment handout carefully, then implement the
	required functions below. Sections where you have to add code
	are marked

	**************
	*** TO DO:
	**************

	If you add any helper functions, make sure you document them
	properly and indicate in the report.txt file what you added.
	
	Have fun!

	DO NOT FORGET TO 'valgrind' YOUR CODE - We will check for pointer
	management being done properly, and for memory leaks.

	Starter code: F.J.E. Jan. 16
*/

#include "QLearn.h"

void QLearn_update(int s, int a, double r, int s_new, double *QTable)
{
 /*
   This function implementes the Q-Learning update as stated in Lecture. It 
   receives as input a <s,a,r,s'> tuple, and updates the Q-table accordingly.
   
   Your work here is to calculate the required update for the Q-table entry
   for state s, and apply it to the Q-table
     
   The update involves two constants, alpha and lambda, which are defined in QLearn.h - you should not 
   have to change their values. Use them as they are.
     
   Details on how states are used for indexing into the QTable are shown
   below, in the comments for QLearn_action. Be sure to read those as well!
 */

  // Find best action in s' using helper function max_expected_reward
  double max_expected_reward_s_new = max_expected_reward(s_new, QTable);
  // Get the expected reward for action s from QTable
  double expected_reward_s = *(QTable + (4 * s) + a);
  // Update the value in QTable
  *(QTable + (4 * s) + a) += alpha * (r + (lambda * max_expected_reward_s_new) - expected_reward_s);
}

int QLearn_action(double gr[max_graph_size][4], int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], double pct, double *QTable, int size_X, int graph_size)
{
  /*
     This function decides the action the mouse will take. It receives as inputs
     - The graph - so you can check for walls! The mouse must never move through walls
     - The mouse position
     - The cat position
     - The chees position
     - A 'pct' value in [0,1] indicating the amount of time the mouse uses the QTable to decide its action,
       for example, if pct=.25, then 25% of the time the mouse uses the QTable to choose its action,
       the remaining 75% of the time it chooses randomly among the available actions.
       
     Remember that the training process involves random exploration initially, but as training
     proceeds we use the QTable more and more, in order to improve our QTable values around promising
     actions.
     
     The value of pct is controlled by QLearn_core_GL, and increases with each round of training.
     
     This function *must return* an action index in [0,3] where
        0 - move up
        1 - move right
        2 - move down
        3 - move left
        
     QLearn_core_GL will print a warning if your action makes the mouse cross a wall, or if it makes
     the mouse leave the map - this should not happen. If you see a warning, fix the code in this
     function!
     
   The Q-table has been pre-allocated and initialized to 0. The Q-table has
   a size of
   
        graph_size^3 x 4
        
   This is because the table requires one entry for each possible state, and
   the state is comprised of the position of the mouse, cat, and cheese. 
   Since each of these agents can be in one of graph_size positions, all
   possible combinations yield graph_size^3 states.
   
   Now, for each state, the mouse has up to 4 possible moves (up, right,
   down, and left). We ignore here the fact that some moves are not possible
   from some states (due to walls) - it is up to the QLearn_action() function
   to make sure the mouse never crosses a wall. 
   
   So all in all, you have a big table.
        
   For example, on an 8x8 maze, the Q-table will have a size of
   
       64^3 x 4  entries
       
       with 
       
       size_X = 8		<--- size of one side of the maze
       graph_size = 64		<--- Total number of nodes in the graph
       
   Indexing within the Q-table works as follows:
   
     say the mouse is at   i,j
         the cat is at     k,l
         the cheese is at  m,n
         
     state = (i+(j*size_X)) + ((k+(l*size_X))*graph_size) + ((m+(n*size_X))*graph_size*graph_size)
     ** Make sure you undestand the state encoding above!
     
     Entries in the Q-table for this state are

     *(QTable+(4*state)+a)      <-- here a is the action in [0,3]
     
     (yes, it's a linear array, no shorcuts with brackets!)
     
     NOTE: There is only one cat and once cheese, so you only need to use cats[0][:] and cheeses[0][:]
   */
  
 
  // Store the action to be returned in this variable
  int action = -1;

  // Generate random number to determine whether
  // to choose from a random action or from the Q-table
  srand((unsigned) time(NULL));
  double c  = (rand() % 101) / 100.00;
  // ---------- Choose RANDOM valid action ---------- //
  if (c > pct) {      
    // Create an array of all actions
    int num_valid_actions = 4;  // Assume all actions are valid
    // Create an array of valid actions that we will use to compute
    int valid_action_arr[4];
    // The current index in the valid actions array to have its element set next
    int curr_index = 0;

    // Loop through each action to check whether it's valid given this state
    for(int a = 0; a < 4; a ++) {
      // Get the mouse index
      int mouse_index = get_graph_index(mouse_pos[0][0], mouse_pos[0][1], size_X);
      // Check if the mouse can move in the chosen direction
      if (gr[mouse_index][a] == 1) {
        valid_action_arr[curr_index] = a;
        curr_index ++;
      } else {
        num_valid_actions --;
      }
    }

    // Choose a random direction
    srand((unsigned) time(NULL));
    int rand_action_index = rand() % num_valid_actions;
    action = valid_action_arr[rand_action_index];
  }

  // ---------- Choose OPTIMAL valid action from Q-table ---------- //
  else {
    // Get the values required to calculate state
    int i = mouse_pos[0][0];
    int j = mouse_pos[0][1];
    int k = cats[0][0];
    int l = cats[0][1];
    int m = cheeses[0][0];
    int n = cheeses[0][1];
    // Determine the state
    int state = (i+(j*size_X)) + ((k+(l*size_X))*graph_size) + ((m+(n*size_X))*graph_size*graph_size);

    // Initialize the max_expected_action with 0
    int max_expected_action = -2;
    // Initialize the max_expected_reward with lowest value
    double max_expected_reward = (double) -INFINITY;
    // Get the mouse index
    int mouse_index = get_graph_index(i, j, size_X);  
    // Iterate through the different directions to determine 
    // optimal action
    for (int direction = 0; direction < 4; direction++) {
      // Check if direction is valid 
      if (gr[mouse_index][direction] == 1) {
        double current_expected_reward = *(QTable + (4 * state) + direction);
        // Check if larger than current max value
        if (current_expected_reward >= max_expected_reward) {
          max_expected_reward = current_expected_reward;
          max_expected_action = direction;
        } else {
          if(max_expected_action == -2) {
            max_expected_reward = current_expected_reward;
            max_expected_action = direction;
          }
        }
      } 
    }

    action = max_expected_action;
  }

  // Check that action is a valid value
  if (action < 0 || action > 3) {
    printf("Action not properly set [action=%d].\n", action);
    exit(1);
  }
  return action;
  
}

double QLearn_reward(double gr[max_graph_size][4], int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X, int graph_size)
{
  /*
    This function computes and returns a reward for the state represented by the input mouse, cat, and
    cheese position. 
    
    You can make this function as simple or as complex as you like. But it should return positive values
    for states that are favorable to the mouse, and negative values for states that are bad for the 
    mouse.
    
    I am providing you with the graph, in case you want to do some processing on the maze in order to
    decide the reward. 
        
    This function should return a maximim/minimum reward when the mouse eats/gets eaten respectively.      
   */

  // Variable to store the reward
  double reward;

  // Mouse, Cat, and Cheese all on the same position
  if (mouse_pos[0][0] == cats[0][0] && 
      mouse_pos[0][1] == cats[0][1] && 
      mouse_pos[0][0] == cheeses[0][0] &&
      mouse_pos[0][1] == cheeses[0][1]) 
  {
    reward = -100;
  }
  
  // Mouse, and Cat on the same position
  else if (mouse_pos[0][0] == cats[0][0] && 
           mouse_pos[0][1] == cats[0][1]) 
  {
    reward = -110;
  }
  
  // Mouse, and Cheese on the same position
  else if (mouse_pos[0][0] == cheeses[0][0] &&
           mouse_pos[0][1] == cheeses[0][1]) 
  {
    reward = 10;
  }

  // If not a terminal point, return a really small value
  else {
    reward = -__DBL_EPSILON__;
  }

  return reward;     
}

void feat_QLearn_update(double gr[max_graph_size][4],double weights[25], double reward, int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X, int graph_size)
{
  /*
    This function performs the Q-learning adjustment to all the weights associated with your
    features. Unlike standard Q-learning, you don't receive a <s,a,r,s'> tuple, instead,
    you receive the current state (mouse, cats, and cheese potisions), and the reward 
    associated with this action (this is called immediately after the mouse makes a move,
    so implicit in this is the mouse having selected some action)
    
    Your code must then evaluate the update and apply it to the weights in the weight array.    
   */
  
  // Initialize int array to store feature values
  double features[25];
  // Evaluate features
  evaluateFeatures(gr, features, mouse_pos, cats, cheeses, size_X, graph_size);
  // Get expected reward for state s
  double qs = Qsa(weights, features);
  // Variables to store max expected reward and action for s'
  double maxU;
  int maxA;
  maxQsa(gr, weights, mouse_pos, cats, cheeses, size_X, graph_size, &maxU, &maxA);
  double qs_new = maxU;

  // Use the formula to update the weights
  for (int index = 0; index < numFeatures; index++) {
    weights[index] += alpha * (reward + (lambda * qs_new) - qs) * features[index];
  }      
}

int feat_QLearn_action(double gr[max_graph_size][4],double weights[25], int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], double pct, int size_X, int graph_size)
{
  /*
    Similar to its counterpart for standard Q-learning, this function returns the index of the next
    action to be taken by the mouse.
    
    Once more, the 'pct' value controls the percent of time that the function chooses an optimal
    action given the current policy.
    
    E.g. if 'pct' is .15, then 15% of the time the function uses the current weights and chooses
    the optimal action. The remaining 85% of the time, a random action is chosen.
    
    As before, the mouse must never select an action that causes it to walk through walls or leave
    the maze.    
   */

  // Store the action to be returned in this variable
  int action = -4;

  // Generate random number to determine whether
  // to choose from a random action or from the Q-table
  srand((unsigned) time(NULL));
  double c = (rand() % 101)/100.00;

  // ---------- Choose RANDOM valid action ---------- //
  if (c > pct) {
    // Create an array of all actions
    int num_valid_actions = 4;  // Assume all actions are valid
    // Create an array of valid actions that we will use to compute
    int valid_action_arr[4];
    // The current index in the valid actions array to have its element set next
    int curr_index = 0;

    // Loop through each action to check whether it's valid given this state
    for(int a = 0; a < 4; a ++) {
      // Get the mouse index
      int mouse_index = get_graph_index(mouse_pos[0][0], mouse_pos[0][1], size_X);
      // Check if the mouse can move in the chosen direction
      if (gr[mouse_index][a] == 1) {
        valid_action_arr[curr_index] = a;
        curr_index ++;
      } else {
        num_valid_actions --;
      }
    }

    // Choose a random direction
    srand((unsigned) time(NULL));
    int rand_action_index = rand() % num_valid_actions;
    action = valid_action_arr[rand_action_index];

  }

  // ---------- Choose OPTIMAL valid action from Q-table ---------- //
  else {
    double maxU;
    int maxA;
    maxQsa(gr, weights, mouse_pos, cats, cheeses, size_X, graph_size, &maxU, &maxA);
    action = maxA;
  }

  // Check that action is a valid value
  if (action < 0 || action > 3) {
    printf("Action not properly set [action=%d].\n", action);
    exit(1);
  }

  return action;

}

void evaluateFeatures(double gr[max_graph_size][4],double features[25], int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X, int graph_size)
{
  /*
   This function evaluates all the features you defined for the game configuration given by the input
   mouse, cats, and cheese positions. You are free to define up to 25 features. This function will
   evaluate each, and return all the feature values in the features[] array.
   
   Take some time to think about what features would be useful to have, the better your features, the
   smarter your mouse!
   
   Note that instead of passing down the number of cats and the number of cheese chunks (too many parms!)
   the arrays themselves will tell you what are valid cat/cheese locations.
   
   You can have up to 5 cats and up to 5 cheese chunks, and array entries for the remaining cats/cheese
   will have a value of -1 - check this when evaluating your features!
  */
 
  const int MANHATTAN = 0;
  const int EUCLIDEAN = 1;
  double min_cat_dist_manhattan = distanceToClosestItem(mouse_pos, cats, 5, size_X, MANHATTAN);
  double min_cheese_dist_manhattan = distanceToClosestItem(mouse_pos,cheeses, 5, size_X, MANHATTAN);
  double min_cat_dist_euclidean = distanceToClosestItem(mouse_pos, cats, 5, size_X, EUCLIDEAN);
  double min_cheese_dist_euclidean = distanceToClosestItem(mouse_pos,cheeses, 5, size_X, EUCLIDEAN);
  // double num_valid_actions = numValidActions(gr, mouse_pos, size_X);

  // features[0] = -100/(min_cat_dist_manhattan + 1);
  // features[1] = 10/(min_cheese_dist_manhattan + 1);

  features[0] = (min_cat_dist_manhattan);
  features[1] = -(min_cheese_dist_manhattan);
  features[2] = min_cat_dist_euclidean;
  features[3] = -min_cheese_dist_euclidean;
  // features[4] = num_valid_actions/100;
        
   
}

double Qsa(double weights[25], double features[25])
{
  /*
    Compute and return the Qsa value given the input features and current weights
   */

  // Initialize the reward to 0
  double reward = 0;
  // Iterate through the features and sum w_i*f_i
  for (int index = 0; index < numFeatures; index++) {
    reward += weights[index] * features[index];
  }  
  
  return reward;
}

void maxQsa(double gr[max_graph_size][4],double weights[25],int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X, int graph_size, double *maxU, int *maxA)
{
 /*
   Given the state represented by the input positions for mouse, cats, and cheese, this function evaluates
   the Q-value at all possible neighbour states and returns the max. The maximum value is returned in maxU
   and the index of the action corresponding to this value is returned in maxA.
   
   You should make sure the function does not evaluate moves that would make the mouse walk through a
   wall. 
  */

  double features[25];
  // Get the mouse index
  int mouse_index = get_graph_index(mouse_pos[0][0], mouse_pos[0][1], size_X);

  // Initial values 
  int opt_action = -3;
  double max_reward = (double) -INFINITY;

  // Iterate through the valid actions (directions) and get s'
  for (int action = 0; action < 4; action++) {
    if (gr[mouse_index][action] == 1) {
      // Array to store new mouse position
      int new_mouse_pos[1][2];
      // Get new mouse position given the action
      // get_mouse_pos(mouse_index, action, new_mouse_pos, size_X);

      int x = mouse_pos[0][0];
      int y = mouse_pos[0][1];

      // Update x or y coordinate based on action
      switch(action) {
        case 0:
          y = y - 1;
          break;
        case 1: 
          x = x + 1;
          break;
        case 2: 
          y = y + 1;
          break;
        case 3: 
          x = x - 1;
          break;
        default:
          printf("Wrong direction selected");
          exit(1);
          break;
      }

      new_mouse_pos[0][0] = x;
      new_mouse_pos[0][1] = y;
      
      // Evaluate features for s'
      evaluateFeatures(gr, features, new_mouse_pos, cats, cheeses, size_X, graph_size);

      // get current reward using function Qsa
      double current_reward = Qsa(weights, features);
      if (max_reward <= current_reward) {
        max_reward = current_reward;
        opt_action = action;
      } else {
        if(opt_action == -3) {
          max_reward = current_reward;
          opt_action = action;
        }
      }
    }
  }

  // Check that action is a valid value
  if (opt_action < 0 || opt_action > 3) {
    printf("Action not properly set [action=%d].\n", opt_action);
    exit(1);
  }

  *maxU=max_reward;
  *maxA=opt_action;
  return;
   
}

/***************************************************************************************************
 *  Add any functions needed to compute your features below 
 *                 ---->  THIS BOX <-----
 * *************************************************************************************************/

/**
 * Convert the location to an index in the graph and return it
 **/
int get_graph_index(int x, int y, int size_X)
{
	return x + (y * size_X);
}

/**
 * Convert the index to a location in the graph and return it
 **/
struct graph_location get_graph_location(int index, int size_X)
{
	struct graph_location location;
	// Corresponding (x,y) location
	location.x = index % size_X;
	location.y = index / size_X;
	return location;
}

/**
 * Determine the max expected reward given state s using 
 * Q-table
 **/
double max_expected_reward(int state, double *QTable) {
  // Initialize the max_expected_reward with lowest value
  double max_expected_reward = (double) -INFINITY;
  // Iterate through the actions to find the
  // action with max expected reward
  for (int action = 0; action < 4; action++) {
    double current_expected_reward = *(QTable + (4 * state) + action);
    // Check if larger than current max value
    if (current_expected_reward > max_expected_reward) {
      max_expected_reward = current_expected_reward;
    }
  }
  return max_expected_reward;
}

/**
 * Determine the max expected action given state s using 
 * Q-table
 **/
int max_expected_action(int state, double *QTable) {
  // Initialize the max_expected_action with 0
  int max_expected_action = 0;
  // Initialize the max_expected_reward with lowest value
  double max_expected_reward = (double) -__INT_MAX__;

  // Iterate through the actions to find the
  // action with max expected reward
  for (int action = 0; action < 4; action++) {
    double current_expected_reward = *(QTable + (4 * state) + action);
    // Check if larger than current max value
    if (current_expected_reward > max_expected_reward) {
      max_expected_reward = current_expected_reward;
      max_expected_action = action;
    }
  }
  return max_expected_action;
}

/**
 * Get mouse position after moving, given index and action (direction)
 **/
void get_mouse_pos(int mouse_index, int action, int new_mouse_pos[1][2], int size_X) {
  int x = mouse_index % size_X;
  int y = mouse_index / size_X;

  switch(action){
    case 0:
      y = y - 1;
      break;
    case 1: 
      x = x + 1;
      break;
    case 2: 
      y = y + 1;
      break;
    case 3: 
      x = x - 1;
      break;
    default:
      break;
  }

  new_mouse_pos[0][0] = x;
  new_mouse_pos[0][1] = y;
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
 * Calculate the Euclidean distance of two points on graph
 **/ 
double calculate_euclidean_distance(struct graph_location p1, struct graph_location p2) {
  const int LEN_SIDE_A = (p2.x - p1.x);
	const int LEN_SIDE_B = (p2.y - p1.y);
	return (double) sqrt((LEN_SIDE_A * LEN_SIDE_A) + (LEN_SIDE_B * LEN_SIDE_B));
}

/**
 * Calculate the distance to closest cheese or closest cat
 * 
 **/
double distanceToClosestItem(int mouse_loc[1][2], int item_loc[5][2], int num_items, int size_X, int mode) {

  const int MANHATTAN = 0;
  const int EUCLIDEAN = 1;
	// Store the mouse location
	int mouse_index = get_graph_index(mouse_loc[0][0], mouse_loc[0][1], size_X);
	struct graph_location mouse_location = get_graph_location(mouse_index, size_X);
	
	// Store the closest cheese/cat location
	struct graph_location min_item_location;

	double distances[num_items]; // stores Manhattan distance from location of mouse to each cheese/cat

  // Initialize array to really large values
  for (int i = 0; i < num_items; i++) {
    distances[i] = (double) __INT_MAX__;
  }

	int min_item_index = 0; // Index to help remember the min index. Initially first cheese/cat
	double distance_to_closest_item;

  int item_index = 0;
  while (item_index < num_items && (item_loc[item_index][0] != -1 && item_loc[item_index][1] != -1)) {
    struct graph_location item_location;
		item_location.x = item_loc[item_index][0];
		item_location.y = item_loc[item_index][1];

    if (mode == MANHATTAN) {
		  distances[item_index] = calculate_manhattan_distance(mouse_location, item_location);
    } else if (mode == EUCLIDEAN) {
		  distances[item_index] = calculate_euclidean_distance(mouse_location, item_location);
    }
		
		// Get the min value from distances array
		if (item_index != min_item_index && distances[item_index] < distances[min_item_index])
		{
			min_item_index = item_index;
		}
    item_index++;
  }

	min_item_location.x = item_loc[min_item_index][0];
	min_item_location.y = item_loc[min_item_index][1];

	distance_to_closest_item = distances[min_item_index];
	return distance_to_closest_item;
}

/**
 * Number of valid actions
 */ 
int numValidActions(double gr[max_graph_size][4], int mouse_pos[1][2], int size_X) {
  int mouse_index = get_graph_index(mouse_pos[0][0], mouse_pos[0][1], size_X);
  int num_valid_actions = 0;

  for (int direction = 0; direction < 4; direction++) {
    if (gr[mouse_index][direction] == 1) {
      num_valid_actions += 1;
    }
  }
   
  return num_valid_actions;
} 