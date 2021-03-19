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

  double expected_reward_s_new = max_expected_reward(s_new, QTable);
  double expected_reward_s = *(QTable + (4 * s_new) + a);
  *(QTable + (4 * s) + a) = alpha * (r + (lambda * expected_reward_s_new) - expected_reward_s);
  
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
  int action;

  // Generate random number to determine whether
  // to choose from a random action or from the Q-table
  double rand_num;
  srand((unsigned) time(NULL));
  rand_num = (double) rand() / (double) RAND_MAX;

  // ---------- Choose RANDOM valid action ---------- //
  if (pct > rand_num) {
    int random_dir_valid = 0;
    // Keep looping till a VALID random direction is chosen
    while (random_dir_valid == 0) {

      // Choose a random direction
      srand((unsigned) time(NULL));
      int rand_action = (rand() % 3);

      // Get the mouse index
      int mouse_index = get_graph_index(mouse_pos[0][0], mouse_pos[0][1], size_X);
      // Check if the mouse can move in the chosen direction
      if (gr[mouse_index][rand_action] == 1) {
        random_dir_valid = 1;
        action = rand_action;
      }
    }
  }

  // ---------- Choose OPTIMAL valid action from Q-table ---------- //
  else {
    int i = mouse_pos[0][0];
    int j = mouse_pos[0][1];
    int k = cats[0][0];
    int l = cats[0][1];
    int m = cheeses[0][0];
    int n = cheeses[0][1];
    int state = (i+(j*size_X)) + ((k+(l*size_X))*graph_size) + ((m+(n*size_X))*graph_size*graph_size);
    action = max_expected_action(state, QTable);
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
    reward = -10;
  }
  
  // Mouse, and Cat on the same position
  else if (mouse_pos[0][0] == cats[0][0] && 
           mouse_pos[0][1] == cats[0][1]) 
  {
    reward = -11;
  }
  
  // Mouse, and Cheese on the same position
  else if (mouse_pos[0][0] == cheeses[0][0] &&
           mouse_pos[0][1] == cheeses[0][1]) 
  {
    reward = 1;
  }

  // If not a terminal point, return a really small value
  else {
    reward = __DBL_EPSILON__;
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
  int features[25];
  // Evaluate features
  evaluateFeatures(gr, features, mouse_pos, cats, cheeses, size_X, graph_size);
  // Get expected reward for state s
  double qs = Qsa(weights, features);
  // Variables to store max expected reward and action for s'
  double *maxU;
  int *maxA;
  maxQsa(gr, weights, mouse_pos, cats, cheeses, size_X, graph_size, maxU, maxA);
  double qs_new = *maxU;

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
  int action;

  // Generate random number to determine whether
  // to choose from a random action or from the Q-table
  double rand_num;
  srand((unsigned) time(NULL));
  rand_num = (double) rand() / (double) RAND_MAX;

  // ---------- Choose RANDOM valid action ---------- //
  if (pct > rand_num) {
    int random_dir_valid = 0;
    // Keep looping till a VALID random direction is chosen
    while (random_dir_valid == 0) {

      // Choose a random direction
      srand((unsigned) time(NULL));
      int rand_action = (rand() % 3);

      // Get the mouse index
      int mouse_index = get_graph_index(mouse_pos[0][0], mouse_pos[0][1], size_X);
      // Check if the mouse can move in the chosen direction
      if (gr[mouse_index][rand_action] == 1) {
        random_dir_valid = 1;
        action = rand_action;
      }
    }
  }

  // ---------- Choose OPTIMAL valid action from Q-table ---------- //
  else {
    double *maxU;
    int *maxA;
    maxQsa(gr, weights, mouse_pos, cats, cheeses, size_X, graph_size, maxU, maxA);
    action = *maxA;
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

   /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/      
   
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

  int feat[25];
  // Get the mouse index
  int mouse_index = get_graph_index(mouse_pos[0][0], mouse_pos[0][1], size_X);

  // Initial values 
  int opt_action = 0;
  double max_reward = 0;

  // Iterate through the valid actions (directions) and get s'
  for (int action = 0; action < 4; action++) {
    if (gr[mouse_index][action] == 1) {
      // Array to store new mouse position
      int new_mouse_pos[1][2];
      // Get new mouse position given the action
      get_mouse_pos(mouse_index, action, new_mouse_pos, size_X);
      // Evaluate features for s'
      evaluateFeatures(gr, feat,  new_mouse_pos, cats, cheeses, size_X, graph_size);

      // get current reward using function Qsa
      double current_reward = Qsa(weights, feat);
      if (max_reward < current_reward) {
        max_reward = current_reward;
        opt_action = action;
      }
    }
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
 * Determine the max expected reward given state s using 
 * Q-table
 **/
double max_expected_reward(int state, double *QTable) {
  // Initialize the max_expected_reward with lowest value
  double max_expected_reward = (double) -__INT_MAX__;
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
