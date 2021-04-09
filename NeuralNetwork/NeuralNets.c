/*
	CSC D84 - Unit 4 - Neural Networks
	
	This file contains stubs for implementing your Neural Nets classifier.
	
	You will implement a single layer network, and 2-layer network,
	and you will study what is the effect of having different numbers
	of neurons in the network, as well as what happens when you used
	different sigmoid functions.
	    
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

	Starter code: F.J.E. Feb. 16
*/

#include "NeuralNets.h"

int train_1layer_net(double sample[INPUTS], int label, double (*sigmoid)(double input), double weights_io[INPUTS][OUTPUTS])
{
  /*
  *   This is your main training function for 1-layer networks. Recall from lecture that we have a simple,
  *  direct connection between inputs and output neurons (the only layer present here). What we are doing
  *  in effect is training 10 different classifiers, each of which will learn to distinguish one of our
  *  training digits.
  * 
  *  Inputs:
  *   sample  -  Array with the pixel values for the input digit - in this case a 28x28 image (784 pixels)
  *              with values in [0-255], plus one bias term (last entry in the array) which is always 1
  *   label  -   Correct label for this digit (our target class)
  *   sigmoid -  The sigmoid function being used, which will be either the logistic function or the hyperbolic
  *              tangent. You have to implement the logistic function, but math.h provides tanh() already
  *   weights_io - Array of weights connecting inputs to output neurons, weights[i][j] is the weight from input
  *                i to output neuron j. This array has a size of 785x10.
  *
  *   Return values:
  *     An int in [0,9] corresponding to the class that your current network has chosen for this training
  *   sample.
  * 
  */

  /**********************************************************************************************************
  *   TO DO: Implement this function! it must compute for a given input digit the network's output values
  *          for each output neuron, and cause the weights to be updated based on the error for each neuron
  *          and according to the backpropagation algorithm discussed in lecture.
  *          
  *          You will need to complete feedforward_1layer(), backprop_1layer(), and logistic() in order to
  *          be able to complete this function.
  ***********************************************************************************************************/

  int classified_digit = 0; // The variable that will represet the class for this sample (initially set to zero)

  // Feed-forward pass for the input in the current iteration to get the classified digit
  classified_digit = classify_1layer(sample, label, sigmoid, weights_io);

  // Initialize the activations array required for back propagation
  double activations[OUTPUTS];

  // Perform a back propagation with the updated weights. Compute the error of output layer to expected output for that neuron
  // Adjust weights_io in order to reduce the error
  backprop_1layer(sample, activations, sigmoid, label, weights_io);
  // Get the the index of the neuron that "fired" (i.e. the argmax of activations)
  classified_digit = find_max(activations);

  return classified_digit; // <--- This should return the class for this sample
}

int classify_1layer(double sample[INPUTS], int label, double (*sigmoid)(double input), double weights_io[INPUTS][OUTPUTS])
{
  /*
  *   This function classifies an input sample given the current network weights. It returns a class in
  *  [0,9] corresponding to the digit the network has decided is present in the input sample
  * 
  *  Inputs:
  *   sample  -  Array with the pixel values for the input digit - in this case a 28x28 image (784 pixels)
  *              with values in [0-255], plus one bias term (last entry in the array) which is always 1
  *   label  -   Correct label for this digit (our target class)
  *   sigmoid -  The sigmoid function being used, which will be either the logistic function or the hyperbolic
  *              tangent. You have to implement the logistic function, but math.h provides tanh() already
  *   weights_io - Array of weights connecting inputs to output neurons, weights[i][j] is the weight from input
  *                i to output neuron j. This array has a size of 785x10.
  *
  *   Return values:
  *     An int in [0,9] corresponding to the class that your current network has chosen for this training
  *   sample.
  * 
  */

  /**********************************************************************************************************
  *   TO DO: Implement this function! 
  *          
  *          You will need to complete feedforward_1layer(), and logistic() in order to
  *          be able to complete this function.
  ***********************************************************************************************************/

  // Since there are 10 (or OUTPUTS) neurons, let each neuron, indexed from 0 to 9 classify each digit in [0,9], respectively
  // So if neuron j has the highest activation value in activations[OUTPUTS], then classify the input as j
  double activations[OUTPUTS];

  // Perform a feedforward pass to get each neuron's activation unit
  feedforward_1layer(sample, sigmoid, weights_io, activations);

  // Find the neuron with the highest result (i.e. the neuron that "fired" the most)
  int classified_digit = find_max(activations);

  return classified_digit;
}

void feedforward_1layer(double sample[785], double (*sigmoid)(double input), double weights_io[INPUTS][OUTPUTS], double activations[OUTPUTS])
{
  /*
  *  This function performs the feedforward pass of the network's computation - it propagates information
  *  from input to output, determines the input to each neuron, and calls the sigmoid function to
  *  calculate neuron activation.
  * 
  *  Inputs:
  *    sample -      The input sample (see above for a description)
  *    sigmoid -     The sigmoid function being used
  *    weights_op -  Array of current network weights
  *    activations - Array where your function will store the resulting activation for each output neuron
  * 
  *  Return values:
  *    Your function must update the 'activations' array with the output value for each neuron
  * 
  *  NOTE - You must *scale* the input to the sigmoid function using the SIGMOID_SCALE value. Otherwise
  *         the neurons will be totally saturated and learning won't happen.
  */

  /*******************************************************************************************************
   * TO DO: Complete this function. You will need to implement logistic() in order for this to work
   *        with a logistic activation function.
   ******************************************************************************************************/

  // Get the activations for every output neuron
  for (int j = 0; j < OUTPUTS; j++)
  {
    // Calculate the activation of for this neuron (i.e. sum all the weights and their associated inputs)
    double activation = 0;
    for (int i = 0; i < INPUTS - 1; i++)
    {
      activation += weights_io[i][j] * sample[i];
    }
    // Update activations(i.e. sigmoid function on the activation computed above), and scale the input
    activations[j] = sigmoid(activation * SIGMOID_SCALE);
  }
}

void backprop_1layer(double sample[INPUTS], double activations[OUTPUTS], double (*sigmoid)(double input), int label, double weights_io[INPUTS][OUTPUTS])
{
  /*
   *  This function performs the core of the learning process for 1-layer networks. It takes
   *  as input the feed-forward activation for each neuron, the expected label for this training
   *  sample, and the weights array. Then it updates the weights in the array so as to minimize
   *  error across neuron outputs.
   * 
   *  Inputs:
   * 	sample - 	Input sample (see above for details)
   *    activations - 	Neuron outputs as computed above
   *    sigmoid -	Sigmoid function in use
   *    label - 	Correct class for this sample
   *    weights_io -	Network weights
   * 
   *  You have to:
   * 		* Determine the target value for each neuron
   * 			- This depends on the type of sigmoid being used, you should think about
   * 			  this: What should the neuron's output be if the neuron corresponds to
   * 			  the correct label, and what should the output be for every other neuron?
   * 		* Compute an error value given the neuron's target
   * 		* Compute the weight adjustment for each weight (the learning rate is in NeuralNets.h)
   */

  /***************************************************************************************************
    * TO DO: Implement this function to compute and apply the weight updates for all weights in
    *        the network. You will need to find a way to figure out which sigmoid function you're
    *        using. Then use the procedure discussed in lecture to compute weight updates.
    * ************************************************************************************************/

  // Iterate through every neuron so that we can get their actual outputs
  for (int j = 0; j < OUTPUTS; j++)
  {
    // Calculate the activation of for this neuron (i.e. sum all the weights and their associated inputs)
    double activation = 0;
    for (int i = 0; i < INPUTS - 1; i++)
    {
      activation += weights_io[i][j] * sample[i];
    }
    // Update activations(i.e. sigmoid function on the activation computed above), and scale the input
    activations[j] = sigmoid(activation * SIGMOID_SCALE);
  }
  // Identify which sigmoid function is being used
  int sigmoid_id = identify_sigmoid(sigmoid);

  // Now that we have the outputs for each neuron, update the weights based on the expected
  // target for the neurons
  for (int j = 0; j < OUTPUTS; j++)
  {
    // Get the gradient of the squared error over the ouput for this neuron
    double error_gradient = error(label, activations[j], j, sigmoid_id);
    // Get the gradient of the activation over the weight from this input to this output
    // Currently using logistic, need to check if tanh later
    double sigmoid_activation = get_activation(activations, j, sigmoid_id);
    for (int i = 0; i < INPUTS - 1; i++)
    {
      // Get the gradient of the output over the activation (i.e. need one for logistic and hyper-tangent)
      double activation_gradient = sample[i];
      // Mulitply the quantities above together with alpha and update it to the current weight associated to the
      // given input and out put
      double error_change = activation_gradient * sigmoid_activation * error_gradient;
      weights_io[i][j] += ALPHA * error_change;
    }
  }
}

int train_2layer_net(double sample[INPUTS], int label, double (*sigmoid)(double input), int units, double weights_ih[INPUTS][MAX_HIDDEN], double weights_ho[MAX_HIDDEN][OUTPUTS])
{
  /*
  *   This is your main training function for 2-layer networks. Now you have to worry about the hidden
  *  layer at this time. *Do not work on this until you have completed the 1-layer network*.
  * 
  *  Inputs:
  *   sample  -  Array with the pixel values for the input digit - in this case a 28x28 image (784 pixels)
  *              with values in [0-255], plus one bias term (last entry in the array) which is always 1
  *   label  -   Correct label for this digit (our target class)
  *   sigmoid -  The sigmoid function being used, which will be either the logistic function or the hyperbolic
  *              tangent. You have to implement the logistic function, but math.h provides tanh() already
  *   units   -  Number of units in the hidden layer
  *   weights_ih - Array of weights connecting inputs to hidden-layer neurons, weights_ih[i][j] is the 
  *                weight from input i to hidden neuron j. This array has a size of units 785 x 10.
  *   weights_ho - Array of weights connecting hidden-layer units to output neurons, weights_ho[i][j] is the 
  *                weight from hidden unit i to output neuron j. This array has a size of units x 10.
  *
  *   Return values:
  *     An int in [0,9] corresponding to the class that your current network has chosen for this training
  *   sample.
  * 
  */

  /**********************************************************************************************************
  *   TO DO: Implement this function! it must compute for a given input digit the activation for each
  * 	     neuron in the hidden layer, and then use that to compute output neuron activations.
  * 
  *          Then it must use these activations along with the input sample's label to update the weights
  * 	     across the whole network
  *          
  *          You will need to complete feedforward_2layer(), backprop_2layer(), and logistic() in order to
  *          be able to complete this function.
  ***********************************************************************************************************/

  int classified_digit = 0; // The variable that will represet the class for this sample (initially set to zero)
  // Feed-forward pass for the input in the current iteration to get the classified digit
  classified_digit = classify_2layer(sample, label, sigmoid, units, weights_ih, weights_ho);

  // This is for back propagation
  double activations[OUTPUTS];
  double h_activations[units];
  backprop_2layer(sample, h_activations, activations, sigmoid, label, weights_ih, weights_ho, units);

  // Get the the index of the neuron that "fired"
  classified_digit = find_max(activations);

  return classified_digit; // <--- Should return the class for this sample
}

int classify_2layer(double sample[INPUTS], int label, double (*sigmoid)(double input), int units, double weights_ih[INPUTS][MAX_HIDDEN], double weights_ho[MAX_HIDDEN][OUTPUTS])
{
  /*
  *   This function takes an input sample and classifies it using the current network weights. It returns
  *  an int in [0,9] corresponding to which digit the network thinks is present in the input sample.
  * 
  *  Inputs:
  *   sample  -  Array with the pixel values for the input digit - in this case a 28x28 image (784 pixels)
  *              with values in [0-255], plus one bias term (last entry in the array) which is always 1
  *   label  -   Correct label for this digit (our target class)
  *   sigmoid -  The sigmoid function being used, which will be either the logistic function or the hyperbolic
  *              tangent. You have to implement the logistic function, but math.h provides tanh() already
  *   units   -  Number of units in the hidden layer
  *   weights_ih - Array of weights connecting inputs to hidden-layer neurons, weights_ih[i][j] is the 
  *                weight from input i to hidden neuron j. This array has a size of units 785 x 10.
  *   weights_ho - Array of weights connecting hidden-layer units to output neurons, weights_ho[i][j] is the 
  *                weight from hidden unit i to output neuron j. This array has a size of units x 10.
  *
  *   Return values:
  *     An int in [0,9] corresponding to the class that your current network has chosen for this training
  *   sample.
  * 
  */

  /**********************************************************************************************************
  *   TO DO: Implement this function! 
  *          
  *          You will need to complete feedforward_2layer(), and logistic() in order to
  *          be able to complete this function.
  ***********************************************************************************************************/

  double activations[OUTPUTS];
  double h_activations[units];

  // Perform a feedforward and obtain the activations for each layer
  feedforward_2layer(sample, sigmoid, weights_ih, weights_ho, h_activations, activations, units);
  // Find the argmax which is the index of the neuron that fired the highest
  int classified_digit = find_max(activations);

  return classified_digit; // <--- Should return the class for this sample
}

void feedforward_2layer(double sample[INPUTS], double (*sigmoid)(double input), double weights_ih[INPUTS][MAX_HIDDEN], double weights_ho[MAX_HIDDEN][OUTPUTS], double h_activations[MAX_HIDDEN], double activations[OUTPUTS], int units)
{
  /*
  *  Here, implement the feedforward part of the two-layer network's computation.
  * 
  *  Inputs:
  *    sample -      The input sample (see above for a description)
  *    sigmoid -     The sigmoid function being used
  *    weights_ih -  Array of current input-to-hidden weights
  *    weights_ho -  Array of current hidden-to-output weights
  *    h_activations - Array of hidden layer unit activations
  *    activations   - Array of activations for output neurons
  *    units -         Number of units in the hidden layer
  * 
  *  Return values:
  *    Your function must update the 'activations' and 'h_activations' arrays with the output values for each neuron
  * 
  *  NOTE - You must *scale* the input to the sigmoid function using the SIGMOID_SCALE value. Otherwise
  *         the neurons will be totally saturated and learning won't happen.
  */

  /*******************************************************************************************************
   * TO DO: Complete this function. You will need to implement logistic() in order for this to work
   *        with a logistic activation function.
   ******************************************************************************************************/

  /**************************************************************************************************
   * Important note - scaling inputs to neurouns is critical to ensure the neurons don't saturate.
   *                  Scaling for the hidden layer works just like it did for the 1 layer net,
   * 		       simply scale your input by SIGMOID_SCALE. However, for the output layer,
   *                  the scaling factor has to be adjusted by the factor
   *                  SIGMOID_SCALE*(MAX_HIDDEN/units).
   **************************************************************************************************/

  // Get the activations for every neuron in the hidden layer
  get_h_activations(sample, sigmoid, h_activations, weights_ih, units);
  // Get the activations for every neuron in the output layer
  for (int j = 0; j < OUTPUTS; j++)
  {
    // Initialize the total activation to zero
    double activation = 0;
    // Iterate through each neuron in the hidden layer
    for (int i = 0; i < (MAX_HIDDEN / units); i++)
    {
      activation += weights_ho[i][j] * h_activations[i];
    }
    activations[j] = sigmoid(activation * SIGMOID_SCALE * (MAX_HIDDEN / units));
  }
}

void backprop_2layer(double sample[INPUTS], double h_activations[MAX_HIDDEN], double activations[OUTPUTS], double (*sigmoid)(double input), int label, double weights_ih[INPUTS][MAX_HIDDEN], double weights_ho[MAX_HIDDEN][OUTPUTS], int units)
{
  /*
   *  This function performs the core of the learning process for 2-layer networks. It performs
   *  the weights update as discussed in lecture. Note that you require the current weights
   *  between the hidden and output layers in order to update the weights from input to hidden,
   *  however the backprop. algorithm would have you update that weight first. So mind the order
   *  of updates and keep track of what you need.
   * 
   *  Inputs:
   * 	sample - 	Input sample (see above for details)
   *    h_activations - Hidden-layer activations
   *    activations -   Output-layer activations
   *    sigmoid -	Sigmoid function in use
   *    label - 	Correct class for this sample
   *    weights_ih -	Network weights from inputs to hidden layer
   *    weights_ho -    Network weights from hidden layer to output layer
   *    units -         Number of units in the hidden layer
   * 
   *  You have to:
   * 		* Determine the target value for each neuron
   * 			- This depends on the type of sigmoid being used, you should think about
   * 			  this: What should the neuron's output be if the neuron corresponds to
   * 			  the correct label, and what should the output be for every other neuron?
   * 		* Compute an error value given the neuron's target
   * 		* Compute the weight adjustment for each weight (the learning rate is in NeuralNets.h)
   */

  /***************************************************************************************************
    * TO DO: Implement this function to compute and apply the weight updates for all weights in
    *        the network. You will need to find a way to figure out which sigmoid function you're
    *        using. Then use the procedure discussed in lecture to compute weight updates.
    * ************************************************************************************************/
  // Get the activations for every neuron in the hidden layer
  get_h_activations(sample, sigmoid, h_activations, weights_ih, units);
  // Get the activations for every neuron in the outter layer
  for (int j = 0; j < OUTPUTS; j++)
  {
    // Initialize the total activation to zero
    double activation = 0;
    // Iterate through each neuron in the hidden layer
    for (int i = 0; i < (MAX_HIDDEN / units); i++)
    {
      activation += weights_ho[i][j] * h_activations[i];
    }
    activations[j] = sigmoid(activation * SIGMOID_SCALE * (MAX_HIDDEN / units));
  }

  // Identifiy the sigmoid function being used
  int sigmoid_id = identify_sigmoid(sigmoid);

  // First update all the weighted edges connecting an input to a neuron in the hidden layer
  for (int k = 0; k < units; k++)
  {
    double total_err_sum = get_total_err_sum(label, k, activations, weights_ho, sigmoid_id);
    for (int i = 0; i < INPUTS; i++)
    {
      // Get the gradient of the squared error over the ouput for this neuron
      // which is target subtracted from the actual output for that neuron
      double error_gradient = total_err_sum;
      // Get the gradient of the activation over the weight from this input to this output
      double sigmoid_activation = get_h_activation(h_activations, k, sigmoid_id);

      // Get the gradient of the output over the activation (i.e. need one for logistic and hyper-tangent)
      double activation_gradient = sample[i];
      // Mulitply the quantities above together with alpha and update it to the current weight associated to the
      // given input and out put
      double error_change = activation_gradient * sigmoid_activation * error_gradient;

      weights_ih[i][k] += ALPHA * error_change;
    }
  }

  // Now update all the weighted edges connecting a neuron in the hidden layer to a neuron in the outter layer
  for (int j = 0; j < OUTPUTS; j++)
  {
    for (int k = 0; k < units; k++)
    {
      double total_err_sum = error(label, activations[j], j, sigmoid_id);

      // Get the gradient of the activation over the weight from this input to this output
      // Currently using logistic, need to check if tanh later
      double sigmoid_activation = get_activation(activations, j, sigmoid_id);
      double error_h_change = h_activations[k] * sigmoid_activation * total_err_sum;

      weights_ho[k][j] += ALPHA * error_h_change;
    }
  }
}

double logistic(double input)
{
  // This function returns the value of the logistic function evaluated on input
  double exponential_val = exp((-1) * input);
  // Return the logistic function evaluated at the given input (as defined in Unit 5 handout)
  return 1 / (1 + exponential_val);
}

/***************************************************************************************************
                                    HELPER FUNCTIONS
* ************************************************************************************************/

/* Return the left-most index of the largest element in the given array */
int find_max(double arr[OUTPUTS])
{
  int max_elmnt_idx = 0;
  for (int i = 0; i < OUTPUTS; i++)
  {
    if (arr[max_elmnt_idx] < arr[i])
      max_elmnt_idx = i;
  }
  return max_elmnt_idx;
}

/* Return the error for output neuron j and input i.*/
double error(int label, double output_value, int neuron_idx, int sigmoid_id)
{
  const int LOGISTIC = 0;
  const int TANH = 1;
  const int OTHER = -1;
  // Calculate the value of the target output for this neuron. Since j indexes
  //  the output neuron in question, and we classify a digit based on the index of
  // the neuron => check if j and label are the same
  double target;
  if (sigmoid_id == LOGISTIC)
  {
    if (neuron_idx == label)
    {
      // We define the  ouput of a neuron in this case should have an ouput of MATCH_TARGET_VAL
      // which indicates that neuron j clearly classified the label in the sample
      const double MATCH_TARGET_VAL = 0.5;
      target = MATCH_TARGET_VAL;
    }
    else
    {
      // We define ouput of a neuron in this case should have an ouput of NOT_TARGET_VAL
      // so that the ouput of neuron j should be much less than the threshold to indicat
      // that neuron j should not have 'fired"
      const double NOT_TARGET_VAL = 0.0;

      target = NOT_TARGET_VAL;
    }
  }
  else if (sigmoid_id == TANH)
  {
    if (neuron_idx == label)
    {
      const double MATCH_TARGET_VAL = 0;
      target = MATCH_TARGET_VAL;
    }
    else
    {
      const double NOT_TARGET_VAL = -1;
      target = NOT_TARGET_VAL;
    }
  }

  // Return their difference
  return target - output_value;
}

/* Returns the activition needed for calculating the gradient*/
double get_activation(double activations[OUTPUTS], int neuron_idx, int sigmoid_id)
{
  const int LOGISTIC = 0;
  const int TANH = 1;
  double gradient = -1;
  double neuron_activation = activations[neuron_idx];

  if (sigmoid_id == LOGISTIC)
  {
    gradient = neuron_activation * (1 - neuron_activation);
  }
  else if (sigmoid_id == TANH)
  {
    gradient = 1 - neuron_activation * neuron_activation;
  }

  return gradient;
}

/* Returns the activition needed for calculating the gradient in hidden layer. Nothing
different that one-layer just for the sake of array sizes*/
double get_h_activation(double h_activations[MAX_HIDDEN], int neuron_idx, int sigmoid_id)
{
  const int LOGISTIC = 0;
  const int TANH = 1;
  double gradient = -1;
  double neuron_activation = h_activations[neuron_idx];

  if (sigmoid_id == LOGISTIC)
  {
    gradient = neuron_activation * (1 - neuron_activation);
  }
  else if (sigmoid_id == TANH)
  {
    gradient = 1 - neuron_activation * neuron_activation;
  }

  return gradient;
}

/* Return 0 if the sigmoid is a logistic function and if it's
tanh then return 1*/
int identify_sigmoid(double (*sigmoid)(double input))
{
  const double X_VAL = 0;       // Constant value that we will check function type for
  const double LOG_Y_VAL = 0.5; // The y-value if sigmoid is logistic
  const double TANH_VAL = 0;    // The y-value if sigmoid is tanh
  const int LOG_ID = 0;
  const int TANH_ID = 1;
  double value = sigmoid(X_VAL); // The value that is return by our sigmoid that we will test

  int sigmoid_id = -1;

  if (value == LOG_Y_VAL)
  {
    sigmoid_id = LOG_ID;
  }
  else if (value == TANH_VAL)
  {
    sigmoid_id = TANH_VAL;
  }
  return sigmoid_id;
}

/*This is to help feedword so that h_activations is precalculated (not to related to get_h_activation)*/
void get_h_activations(double sample[INPUTS], double (*sigmoid)(double input), double h_activations[MAX_HIDDEN], double weights_ih[INPUTS][MAX_HIDDEN], int units)
{
  // Iterate through each neuron in the hidden layer
  for (int k = 0; k < units; k++)
  {
    // Intialize the total activation for the hidden layer to zero
    double h_activation = 0;
    for (int i = 0; i < INPUTS - 1; i++)
    {
      h_activation += weights_ih[i][k] * sample[i];
    }
    // Pass the summed
    h_activations[k] = sigmoid(h_activation * SIGMOID_SCALE);
  }
}

/* 
This is used to calculate gradient of of the error with respect to the output
of a particular neuron. The calculation is used to compute back propagation for
a 2-layer neural network 
*/
double get_total_err_sum(int label, int k, double activations[OUTPUTS], double weights_ho[MAX_HIDDEN][OUTPUTS], int sigmoid_id)
{
  double total_err_sum = 0;
  for (int j = 0; j < OUTPUTS; j++)
  {
    // Get the required derivative for the sigmoid activation
    double sigmoid_activation = get_activation(activations, j, sigmoid_id);
    // Get the error for output neuron j
    double output_error = error(label, activations[j], j, sigmoid_id);

    // Calculate the summation needed for the chain rule (similar to 1 layer)
    total_err_sum += weights_ho[k][j] * sigmoid_activation * output_error;
  }
  return total_err_sum;
}