/*
	CSC D84 - Unit 1 - Search

	This file contains stubs for implementing the different search
	algorithms covered in the course. Please read the assignment
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

	Starter code: F.J.E., Jul. 15
	Updated: F.J.E., Jan. 18
*/

/**********************************************************************
% COMPLETE THIS TEXT BOX:
%
% 1) Student Name:	Dishant Patel	
% 2) Student Name:	Omid Najmeddini		
%
% 1) Student number: 999013749
% 2) Student number: 1002358441
% 
% 1) UtorID:  pateldi7
% 2) UtorID:  najmeddi
% 
% We hereby certify that the work contained here is our own
%
%  Dishant Patel                    Omid Najmeddini
% (sign with your name)            (sign with your name)
***********************************************************************/

#include "AI_search.h"

/******************************************************************************
% THE HELPER FUNCTIONS BELOW FOR SETTING UP DATA STRUCTURES ARE NOT CODED BY US
% THEY ARE OBTAINED FROM https://www.geeksforgeeks.org/
 ******************************************************************************/

// Create a queue
struct queue *createQueue()
{
	struct queue *q = (queue *)malloc(sizeof(struct queue));
	q->front = -1;
	q->rear = -1;
	return q;
}

// Check if the queue is empty
int isEmpty(struct queue *q)
{
	if (q->rear == -1)
		return 1;
	else
		return 0;
}

// Adding elements into queue
void enqueue(struct queue *q, int value)
{
	if (q->rear == graph_size - 1)
		printf("\nQueue is Full!!");
	else
	{
		if (q->front == -1)
			q->front = 0;
		q->rear++;
		q->items[q->rear] = value;
	}
}

// Removing elements from queue
int dequeue(struct queue *q)
{
	int item;
	if (isEmpty(q))
	{
		item = -1;
	}
	else
	{
		item = q->items[q->front];
		q->front++;
		if (q->front > q->rear)
		{
			q->front = q->rear = -1;
		}
	}
	return item;
}

struct Stack *createStack(unsigned capacity)
{
	struct Stack *stack = (struct Stack *)malloc(sizeof(struct Stack));
	stack->capacity = capacity;
	stack->top = -1;
	stack->array = (int *)malloc(stack->capacity * sizeof(int));
	return stack;
}

// Stack is full when top is equal to the last index
int isFull(struct Stack *stack)
{
	return stack->top == stack->capacity - 1;
}

// Stack is empty when top is equal to -1
int isEmptyStack(struct Stack *stack)
{
	return stack->top == -1;
}

// Function to add an item to stack.  It increases top by 1
void push(struct Stack *stack, int item)
{
	if (isFull(stack))
		return;
	stack->array[++stack->top] = item;
}

// Function to remove an item from stack.  It decreases top by 1
int pop(struct Stack *stack)
{
	if (isEmptyStack(stack))
		return INT_MIN;
	return stack->array[stack->top--];
}

// Function to Create A New Node 
Node* newNode(int d, int p) 
{ 
    Node* temp = (Node*)malloc(sizeof(Node)); 
    temp->data = d; 
    temp->priority = p; 
    temp->next = NULL; 
    return temp; 
} 
// Return the value at head 
int priority_peek(Node** head) 
{ 
	return (*head)->data; 
} 
// Removes the element with the 
// highest priority form the list 
void priority_pop(Node** head) 
{ 
    Node* temp = *head; 
    (*head) = (*head)->next; 
    free(temp); 
} 
 
// Function to push according to priority 
void priority_push(Node** head, int d, int p) 
{ 
    Node* start = (*head); 
 
    // Create new Node 
    Node* temp = newNode(d, p); 
 
    // Special Case: The head of list has lesser 
    // priority than new node. So insert new 
    // node before head node and change head node. 
    if ((*head)->priority > p) { 
 
        // Insert New Node before head 
        temp->next = *head; 
        (*head) = temp; 
    } 
    else { 
 
        // Traverse the list and find a 
        // position to insert new node 
        while (start->next != NULL && 
            start->next->priority < p) { 
            start = start->next; 
        } 
 
        // Either at the ends of the list 
        // or at required position 
        temp->next = start->next; 
        start->next = temp; 
    } 
} 
 
// Function to check is list is empty 
int priority_isEmpty(Node** head) 
{ 
    return (*head) == NULL; 
} 


/******************************************************************************
% THE HELPER FUNCTIONS ABOVE FOR SETTING UP DATA STRUCTURES ARE NOT CODED BY US
% THEY ARE OBTAINED FROM https://www.geeksforgeeks.org/
 ******************************************************************************/

void search(double gr[graph_size][4], int path[graph_size][2], int visit_order[size_X][size_Y], int cat_loc[10][2], int cats, int cheese_loc[10][2], int cheeses, int mouse_loc[1][2], int mode, int (*heuristic)(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, double gr[graph_size][4]))
{
	/*
   This function is the interface between your solution for the assignment and the driver code. The driver code
   in AI_search_core_GL will call this function once per frame, and provide the following data
   
   Board and game layout:
	
	The game takes place on a grid of size 32x32, the file board_layout.h specifies the size and defines two
	constants 'size_X' and 'size_Y' for the horizontal and vertical size of the board, respectively. For our
	purposes, the grid of locations is represented by a graph with one node per grid location, so given
	the 32x32 cells, the graph has 1024 nodes. 

	To create a maze, we connect cell locations in the grid in such a way that a) there is a path from any
	grid location to any other grid location (i.e. there are no disconnected subsets of nodes in the graph),
	and b) there are loops.

	Since each node represents a grid location, each node can be connected to up to 4 neighbours in the
	top, right, bottom, and left directions respectively:

			node at (i,j-1)
				^
				|
	(node at i-1, j) <- node at (i,j) -> node at (i+1, j)
				|
				v
			node at (i,j+1)

	The graph is theredore stored as an adjacency list with size 1024 x 4, with one row per node in the
	graph, and 4 columns corresponding to the weight of an edge linking the node with each of its 4
	possible neighbours in the order towp, right, bottom, left (clockwise from top).

	Since all we care is whether nodes are connected. Weights will be either 0 or 1, if the weight is
	1, then the neighbouring nodes are connected, if the weight is 0, they are not. For example, if

	graph[i][0] = 0 
	graph[i][1] = 1
	graph[i][2] = 0
	graph[i][3] = 1

	then node i is connected to the right and left neighbours, but not to top or bottom.

	The index in the graph for the node corresponding to grid location (x,y) is

	index = x + (y*size_X) 		or in this case		index = x + (y*32)

	Conversely, if you have the index and want to figure out the grid location,

	x = index % size_X		or in this case		x = index % 32
	y = index / size_Y		or in this case		y = index / 32

	(all of the above are *integer* operations!)

	A path is a sequence of (x,y) grid locations. We store it using an array of size	
	1024 x 2 (since there are 1024 locations, this is the maximum length of any
	path that visits locations only once).

	Agent locations are coordinate pairs (x,y)	

   Arguments:
		gr[graph_size][4]   - This is an adjacency list for the maze
		path[graph_size][2] - An initially empty path for your code to fill.
				      In this case, empty means all entries are initially -1
		visit_order[size_X][size_Y]	- An array in which your code will store the
						  *order* in which grid locations were 
						  visited during search. For example, while
						  doing BFS, the initial location is the
						  start location, it's visit order is 1.
						  Then the search would expand the immediate
						  neighbours of the initial node in some order,
						  these would get a visit order of 2, 3, 4, and
						  5 respectively, and so on.
					
						  This array will be used to display the search
						  pattern generated by each search method.

		cat_loc[10][2], cats   - Location of cats and number of cats (we can have at most 10,
					 but there can be fewer). Only valid cat locations are 0 to (cats-1)
		cheese_loc[10][2], cheeses - Location and number of cheese chunks (again at most 10,
					     but possibly fewer). Valid locations are 0 to (cheeses-1)
		mouse_loc[1][2] - Mouse location - there can be only one!
		mode - Search mode selection:
					mode = 0 	- BFS
					mode = 1	- DFS
					mode = 2	- A*

		(*heuristic)(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[10][2], int cats, int cheeses)
				- This is a pointer to one of the heuristic functions you will implement, either H_cost()
				  or H_cost_nokitty(). The driver in AI_search_core_GL will pass the appropriate pointer
				  depending on what search the user wants to run.
			
				  If the mode is 0 or 1, this pointer is NULL

				  * How to call the heuristic function from within this function : *
					- Like any other function:
						h = heuristic( x, y, cat_loc, cheese_loc, mouse_loc, cats, cheeses);

   Return values:
		Your search code will directly update data passed-in as arguments:

		- path[graph_size][2]	: Your search code will update this array to contain the path from
					  the mouse to one of the cheese chunks. The order matters, so
					  path[0][:] must be the mouse's current location, path[1][:]
					  is the next move for the mouse. Each successive row will contain
					  the next move toward the cheese, and the path ends at a location
					  whose coordinates correspond to one of the cheese chunks.
					  Any entries beyond that must remain set to -1
		- visit_order[size_X][size_Y] 	:  Your search code will update this array to contain the
						   order in which each location in the grid was expanded
						   during search. This means, when that particular location
						   was checked for being a goal, and if not a goal, had its
						   neighbours added as candidates for future expansion (in
						   whatever order is dictated by the search mode).

						   Note that since there are 1024 locations, the values in 
						   this array must always be in [0, 1023]. The driver code
						   will then display search order as a yellow-colored
						   brightness map where nodes expanded earlier will look
						   brighter.

		* Your code MUST NOT modify the locations or numbers of cats and/or cheeses, the graph,
	     	  or the location of the mouse - if you try, the driver code will know it *
			
		That's that, now, implement your solution!
 */

	/********************************************************************************************************
 * 
 * TO DO:	Implement code to carry out the different types of search depending on the
 *		mode.
 *
 *		You can do this by writing code within this single function (and being clever about it!)
 *		Or, you can use this function as a wrapper that calls a different search function
 *		 (BFS, DFS, A*) depending on the mode. Note that in the latter case, you will have
 *	    	  to inform your A* function somehow of what heuristic it's supposed to use.
 *
 *		Visiting Order: When adding the neighbours of a node to your list of candidates for
 *				expansion, do so in the order top, right, bottom, left.
 *
 *		NOTE: Your search functions should be smart enough to not choose a path that goes
 *		      through a cat! this is easily done without any heuristics.
 * 
 *		How you design your solution is up to you. But:
 *
 *		- Document your implementation by adding concise and clear comments in this file
 *		- Document your design (how you implemented the solution, and why) in the report
 *
 ********************************************************************************************************/

	/******************************************************************************
% ======================== Breadth First Search (BFS) ========================
 ******************************************************************************/

	if (mode == 0)
	{
		// Get the location and index of our starting position
		int starting_mouse_locX = mouse_loc[0][0];
		int starting_mouse_locY = mouse_loc[0][1];
		int starting_mouse_index = get_graph_index(starting_mouse_locX, starting_mouse_locY);

		// Declared some variables and arrays to keep track of number of
		// nodes visited and cheese found
		int cheese_found = 0;
		int cheese_index;
		int num_of_visited_nodes = 1;
		int pred[graph_size];

		// Mark first node as visited and add it to our queue, node_queue
		visit_order[starting_mouse_locX][starting_mouse_locY] = num_of_visited_nodes;
		struct queue *node_queue = createQueue();
		enqueue(node_queue, starting_mouse_index);

		// Loop till first cheese is found
		while (cheese_found == 0)
		{
			// Get the x and y coordinates of current_index
			int current_index = dequeue(node_queue);
			struct graph_location current_location = get_graph_location(current_index);

			// Check if there is cheese at the current location
			for (int i = 0; i < cheeses; i++)
			{
				if (cheese_loc[i][0] == current_location.x && cheese_loc[i][1] == current_location.y)
				{
					cheese_found += 1;
					cheese_index = current_index;
				}
			}

			// Visit top node if it has not been visited yet
			if (gr[current_index][0] == 1)
			{
				int top_neighbour_x = current_index % size_X;
				int top_neighbour_y = (current_index / size_Y) - 1;
				int top_neighbour_index = top_neighbour_x + (top_neighbour_y * size_X);
				if (visit_order[top_neighbour_x][top_neighbour_y] <= 0)
				{
					num_of_visited_nodes += 1;
					visit_order[top_neighbour_x][top_neighbour_y] = num_of_visited_nodes;
					pred[top_neighbour_index] = current_index;
					enqueue(node_queue, top_neighbour_index);
				}
			}

			// Visit right node if it has not been visited yet
			if (gr[current_index][1] == 1)
			{
				int right_neighbour_x = (current_index % size_X) + 1;
				int right_neighbour_y = current_index / size_Y;
				int right_neighbour_index = right_neighbour_x + (right_neighbour_y * size_X);

				if (visit_order[right_neighbour_x][right_neighbour_y] <= 0)
				{
					num_of_visited_nodes += 1;
					visit_order[right_neighbour_x][right_neighbour_y] = num_of_visited_nodes;
					pred[right_neighbour_index] = current_index;
					enqueue(node_queue, right_neighbour_index);
				}
			}

			// Visit bottom node if it has not been visited yet
			if (gr[current_index][2] == 1)
			{
				int bot_neighbour_x = current_index % size_X;
				int bot_neighbour_y = (current_index / size_Y) + 1;
				int bot_neighbour_index = bot_neighbour_x + (bot_neighbour_y * size_X);

				if (visit_order[bot_neighbour_x][bot_neighbour_y] <= 0)
				{
					num_of_visited_nodes += 1;
					visit_order[bot_neighbour_x][bot_neighbour_y] = num_of_visited_nodes;
					pred[bot_neighbour_index] = current_index;
					enqueue(node_queue, bot_neighbour_index);
				}
			}

			// Visit left node if it has not been visited yet
			if (gr[current_index][3] == 1)
			{
				int left_neighbour_x = (current_index % size_X) - 1;
				int left_neighbour_y = current_index / size_Y;
				int left_neighbour_index = left_neighbour_x + (left_neighbour_y * size_X);

				if (visit_order[left_neighbour_x][left_neighbour_y] <= 0)
				{
					num_of_visited_nodes += 1;
					visit_order[left_neighbour_x][left_neighbour_y] = num_of_visited_nodes;
					pred[left_neighbour_index] = current_index;
					enqueue(node_queue, left_neighbour_index);
				}
			}
		}

		// Add the index of the path from cheese to mouse on a stack
		struct Stack *stack = createStack(graph_size);
		int pred_index = cheese_index;
		while (pred_index != starting_mouse_index)
		{
			push(stack, pred_index);
			pred_index = pred[pred_index];
		}

		// Add initial location of the mouse to path
		path[0][0] = starting_mouse_locX;
		path[0][1] = starting_mouse_locY;

		// Pop from stack and add the location from mouse's 2nd location to cheese
		int path_item = 1;
		while (!isEmptyStack(stack))
		{
			int pop_x = pop(stack);
			int pth_x = pop_x % size_X;
			int pth_y = pop_x / size_Y;
			path[path_item][0] = pth_x;
			path[path_item][1] = pth_y;
			path_item++;
		}
		return;
	}

	/******************************************************************************
% ======================== Depth First Search (DFS) ========================
******************************************************************************/
	else if (mode == 1)
	{
		// Get the location and index of our starting position
		int starting_mouse_locX = mouse_loc[0][0];
		int starting_mouse_locY = mouse_loc[0][1];
		int starting_mouse_index = get_graph_index(starting_mouse_locX, starting_mouse_locY);

		// Declared some variables and arrays to keep track of number of
		// nodes visited and cheese found
		int cheese_found = 0;
		int cheese_index;
		int num_of_visited_nodes = 1;
		int pred[graph_size];

		// Mark first node as visited and add it to our stack
		visit_order[starting_mouse_locX][starting_mouse_locY] = num_of_visited_nodes;
		struct Stack *stack = createStack(graph_size);
		push(stack, starting_mouse_index);

		// Loop till first cheese is found
		while (cheese_found == 0)
		{
			// Get the x and y coordinates of current_index
			int current_index = pop(stack);
			struct graph_location current_location = get_graph_location(current_index);

			// Check if there is cheese at the current location
			for (int i = 0; i < cheeses; i++)
			{
				if (cheese_loc[i][0] == current_location.x && cheese_loc[i][1] == current_location.y)
				{
					cheese_found += 1;
					cheese_index = current_index;
				}
			}

			// Since we are adding to a stack, we add the nodes in the reverse order
			// Visit left node if it has not been added yet
			if (gr[current_index][3] == 1)
			{
				int left_neighbour_x = (current_index % size_X) - 1;
				int left_neighbour_y = current_index / size_Y;
				int left_neighbour_index = left_neighbour_x + (left_neighbour_y * size_X);

				if (visit_order[left_neighbour_x][left_neighbour_y] <= 0)
				{
					num_of_visited_nodes += 1;
					visit_order[left_neighbour_x][left_neighbour_y] = num_of_visited_nodes;
					pred[left_neighbour_index] = current_index;
					push(stack, left_neighbour_index);
				}
			}

			// Visit bottom node if it has not been added yet
			if (gr[current_index][2] == 1)
			{
				int bot_neighbour_x = current_index % size_X;
				int bot_neighbour_y = (current_index / size_Y) + 1;
				int bot_neighbour_index = bot_neighbour_x + (bot_neighbour_y * size_X);

				if (visit_order[bot_neighbour_x][bot_neighbour_y] <= 0)
				{
					num_of_visited_nodes += 1;
					visit_order[bot_neighbour_x][bot_neighbour_y] = num_of_visited_nodes;
					pred[bot_neighbour_index] = current_index;
					push(stack, bot_neighbour_index);
				}
			}

			// Visit right node if it has not been added yet
			if (gr[current_index][1] == 1)
			{
				int right_neighbour_x = (current_index % size_X) + 1;
				int right_neighbour_y = current_index / size_Y;
				int right_neighbour_index = right_neighbour_x + (right_neighbour_y * size_X);

				if (visit_order[right_neighbour_x][right_neighbour_y] <= 0)
				{
					num_of_visited_nodes += 1;
					visit_order[right_neighbour_x][right_neighbour_y] = num_of_visited_nodes;
					pred[right_neighbour_index] = current_index;
					push(stack, right_neighbour_index);
				}
			}

			// Visit top node if it has not been added yet
			if (gr[current_index][0] == 1)
			{
				int top_neighbour_x = current_index % size_X;
				int top_neighbour_y = (current_index / size_Y) - 1;
				int top_neighbour_index = top_neighbour_x + (top_neighbour_y * size_X);

				if (visit_order[top_neighbour_x][top_neighbour_y] <= 0)
				{
					num_of_visited_nodes += 1;
					visit_order[top_neighbour_x][top_neighbour_y] = num_of_visited_nodes;
					pred[top_neighbour_index] = current_index;
					push(stack, top_neighbour_index);
				}
			}
		}

		// Add the index of the path from cheese to mouse on a stack
		struct Stack *result_stack = createStack(graph_size);
		int pred_index = cheese_index;
		while (pred_index != starting_mouse_index)
		{
			push(result_stack, pred_index);
			pred_index = pred[pred_index];
		}

		// Add initial location of the mouse to path
		path[0][0] = starting_mouse_locX;
		path[0][1] = starting_mouse_locY;

		// Pop from stack and add the location from mouse's 2nd location to cheese
		int path_item = 1;
		while (!isEmptyStack(result_stack))
		{
			int pop_x = pop(result_stack);
			int pth_x = pop_x % size_X;
			int pth_y = pop_x / size_Y;
			path[path_item][0] = pth_x;
			path[path_item][1] = pth_y;
			path_item++;
		}
		return;
	}
	/******************************************************************************
% ======================== Heuristic Search (A*) ========================
******************************************************************************/
	else if (mode == 2)
	{
		// Get the location and index of our starting position
		int starting_mouse_locX = mouse_loc[0][0];
		int starting_mouse_locY = mouse_loc[0][1];
		int starting_mouse_index = get_graph_index(starting_mouse_locX, starting_mouse_locY);

		// Declared some variables and arrays to keep track of number of
		// nodes visited and cheese found
		int cheese_found = 0;
		int cheese_index;
		int num_of_visited_nodes = 1;
		int pred[graph_size];

		// Mark first node as visited and add it to our queue, node_queue
		visit_order[starting_mouse_locX][starting_mouse_locY] = num_of_visited_nodes;
		int start_to_goal_cost = heuristic(starting_mouse_locX, starting_mouse_locY, cat_loc, cheese_loc, mouse_loc, cats, cheeses, gr);
		Node* priority_queue = newNode(graph_size, graph_size);
		priority_push(&priority_queue, starting_mouse_index, start_to_goal_cost);

		// Loop till first cheese is found
		while (cheese_found == 0 && priority_peek(&priority_queue) != graph_size)
		{
			// Get the x and y coordinates of current_index
			int current_index = priority_peek(&priority_queue);
			priority_pop(&priority_queue);
			struct graph_location current_location = get_graph_location(current_index);

			// Check if there is cheese at the current location
			for (int i = 0; i < cheeses; i++)
			{
				if (cheese_loc[i][0] == current_location.x && cheese_loc[i][1] == current_location.y)
				{
					cheese_found += 1;
					cheese_index = current_index;
				}
			}

			// Visit top node if it has not been visited yet
			if (gr[current_index][0] == 1)
			{
				int top_neighbour_x = current_index % size_X;
				int top_neighbour_y = (current_index / size_Y) - 1;
				int top_neighbour_index = top_neighbour_x + (top_neighbour_y * size_X);

				if (visit_order[top_neighbour_x][top_neighbour_y] <= 0)
				{
					num_of_visited_nodes += 1;
					visit_order[top_neighbour_x][top_neighbour_y] = num_of_visited_nodes;
					pred[top_neighbour_index] = current_index;
					int top_to_goal_cost = heuristic(top_neighbour_x, top_neighbour_y, cat_loc, cheese_loc, mouse_loc, cats, cheeses, gr);
					priority_push(&priority_queue, top_neighbour_index, top_to_goal_cost);
				}
			}

			// Visit right node if it has not been visited yet
			if (gr[current_index][1] == 1)
			{
				int right_neighbour_x = (current_index % size_X) + 1;
				int right_neighbour_y = current_index / size_Y;
				int right_neighbour_index = right_neighbour_x + (right_neighbour_y * size_X);

				if (visit_order[right_neighbour_x][right_neighbour_y] <= 0)
				{
					num_of_visited_nodes += 1;
					visit_order[right_neighbour_x][right_neighbour_y] = num_of_visited_nodes;
					pred[right_neighbour_index] = current_index;
					int right_to_goal_cost = heuristic(right_neighbour_x, right_neighbour_y, cat_loc, cheese_loc, mouse_loc, cats, cheeses, gr);
					priority_push(&priority_queue, right_neighbour_index, right_to_goal_cost);
				}
			}

			// Visit bottom node if it has not been visited yet
			if (gr[current_index][2] == 1)
			{
				int bot_neighbour_x = current_index % size_X;
				int bot_neighbour_y = (current_index / size_Y) + 1;
				int bot_neighbour_index = bot_neighbour_x + (bot_neighbour_y * size_X);

				if (visit_order[bot_neighbour_x][bot_neighbour_y] <= 0)
				{
					num_of_visited_nodes += 1;
					visit_order[bot_neighbour_x][bot_neighbour_y] = num_of_visited_nodes;
					pred[bot_neighbour_index] = current_index;
					int bot_to_goal_cost = heuristic(bot_neighbour_x, bot_neighbour_y, cat_loc, cheese_loc, mouse_loc, cats, cheeses, gr);
					priority_push(&priority_queue, bot_neighbour_index, bot_to_goal_cost);
				}
			}

			// Visit left node if it has not been visited yet
			if (gr[current_index][3] == 1)
			{
				int left_neighbour_x = (current_index % size_X) - 1;
				int left_neighbour_y = current_index / size_Y;
				int left_neighbour_index = left_neighbour_x + (left_neighbour_y * size_X);

				if (visit_order[left_neighbour_x][left_neighbour_y] <= 0)
				{
					num_of_visited_nodes += 1;
					visit_order[left_neighbour_x][left_neighbour_y] = num_of_visited_nodes;
					pred[left_neighbour_index] = current_index;
					int left_to_goal_cost = heuristic(left_neighbour_x, left_neighbour_y, cat_loc, cheese_loc, mouse_loc, cats, cheeses, gr);
					priority_push(&priority_queue, left_neighbour_index, left_to_goal_cost);
				}
			}
		}

		// Add the index of the path from cheese to mouse on a stack
		struct Stack *stack = createStack(graph_size);
		int pred_index = cheese_index;
		while (pred_index != starting_mouse_index)
		{
			push(stack, pred_index);
			pred_index = pred[pred_index];
		}

		// Add initial location of the mouse to path
		path[0][0] = starting_mouse_locX;
		path[0][1] = starting_mouse_locY;

		// Pop from stack and add the location from mouse's 2nd location to cheese
		int path_item = 1;
		while (!isEmptyStack(stack))
		{
			int pop_x = pop(stack);
			int pth_x = pop_x % size_X;
			int pth_y = pop_x / size_Y;
			path[path_item][0] = pth_x;
			path[path_item][1] = pth_y;
			path_item++;
		}
		return;
	}
}

int H_cost(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, double gr[graph_size][4])
{
	/*
	This function computes and returns the heuristic cost for location x,y.
	As discussed in lecture, this means estimating the cost of getting from x,y to the goal. 
	The goal is cheese. Which cheese is up to you.
	Whatever you code here, your heuristic must be admissible.

	Input arguments:

		x,y - Location for which this function will compute a heuristic search cost
		cat_loc - Cat locations
		cheese_loc - Cheese locations
		mouse_loc - Mouse location
		cats - # of cats
		cheeses - # of cheeses
		gr - The graph's adjacency list for the maze

		These arguments are as described in the search() function above
 */
	struct graph_location location;
	// Store the closest cheese location
	struct graph_location min_cheese_location;

	int distances[cheeses]; // stores Manhattan distance from (x,y) to each cheese
	int min_cheese_index = 0; // Index to help remember the min index. Initially first cheese
	int cost;

	location.x = x;
	location.y = y;

	for (int i = 0; i < cheeses; i++)
	{
		struct graph_location cheese_location;
		cheese_location.x = cheese_loc[i][0];
		cheese_location.y = cheese_loc[i][1];

		distances[i] = calculate_manhattan_distance(location, cheese_location);
		// Check if the distance from the (x,y) to index i is shorter than
		// the distance from the (x,y) to the min index
		if (i != min_cheese_index && distances[i] < distances[min_cheese_index])
		{
			min_cheese_index = i;
		}
	}

	min_cheese_location.x = cheese_loc[min_cheese_index][0];
	min_cheese_location.y = cheese_loc[min_cheese_index][1];

	// Calculate cost by how much the distance to cheese is shortened
	// when compared to the distance where the mouse currently is
	cost = distances[min_cheese_index];

	return cost; 

}

int H_cost_nokitty(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, double gr[graph_size][4])
{
	/*
	This function computes and returns the heuristic cost for location x,y.
	As discussed in lecture, this means estimating the cost of getting from x,y to the goal. 
	The goal is cheese. 

	However - this time you want your heuristic function to help the mouse avoid being eaten.
	Therefore: You have to somehow incorporate knowledge of the cats' locations into your
	heuristic cost estimate. How well you do this will determine how well your mouse behaves
	and how good it is at escaping kitties.

	This heuristic *does not have to* be admissible.

	Input arguments have the same meaning as in the H_cost() function above.
 */
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


