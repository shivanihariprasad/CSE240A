#include "predictor.h"

//Data Structures for perceptron
int8_t weight_limit_lower;
int8_t weight_limit_upper;
int **perceptron_table; // stores weights - uses row-major ordering
uint64_t perceptron_global_history;

int perceptron_table_length; 
int perceptron_table_length_bits; 
int num_weights;
int num_weights_bits; 
int weight_bit_limit;
int perceptron_theta;

uint16_t get_power_of_2(int power){
  return 1 << power;
}

//Init Function for perceptron
void init_perceptron(){

  weight_limit_lower = -1 * get_power_of_2(weight_bit_limit);
  weight_limit_upper = get_power_of_2(weight_bit_limit) - 1;
  num_weights = get_power_of_2(num_weights_bits);
  perceptron_table_length = get_power_of_2(perceptron_table_length_bits);
    
  perceptron_table = (int**) malloc(perceptron_table_length * num_weights * sizeof(int*)); // stores weights
  for(int i=0; i<perceptron_table_length; i++)
				perceptron_table[i] = (int*) calloc(perceptron_table_length, sizeof(int));
  perceptron_global_history = 0;
    
  //printf("weight_bit_limit=%d, weight_limit_lower=%d, weight_limit_upper=%d, num_weights=%d, perceptron_table_length=%d, perceptron_theta=%d\n", weight_bit_limit, weight_limit_lower, weight_limit_upper, num_weights, perceptron_table_length, perceptron_theta);
}

// get index into the table of perceptrons
uint8_t get_perceptron_table_index(uint32_t pc){
  return pc & ((1 << perceptron_table_length_bits) - 1);
}

// get predicted outcome based on weights and global history
int get_perceptron_sum(uint8_t perceptron_table_idx){
  int sum = 0;
  
  // adding the bias term first
  sum += perceptron_table[perceptron_table_idx][0];
  for(uint8_t weight_idx = 1; weight_idx < num_weights; weight_idx++){
    if ((perceptron_global_history >> weight_idx) & 1){
      sum += perceptron_table[perceptron_table_idx][weight_idx];
    } else {
      sum -= perceptron_table[perceptron_table_idx][weight_idx];
    }
  } 
  
  return sum; 
}

// get outcome for branch
uint8_t fetch_perceptron_prediction(uint32_t pc){
    uint8_t perceptron_table_idx = get_perceptron_table_index(pc);
    //int8_t* perceptron_weights = get_perceptron_weights(perceptron_table_idx);
    
    return (get_perceptron_sum(perceptron_table_idx) >= 0) ? TAKEN : NOTTAKEN;
}

int set_weight_limit(int *weight_value)
{
	if(*weight_value > weight_limit_upper)
		*weight_value--;
	else if(*weight_value < weight_limit_lower)
		*weight_value++;
}

// update weights of perceptron
void training_perceptron(uint32_t pc, uint8_t outcome){
  uint8_t perceptron_table_idx = get_perceptron_table_index(pc);
  //int8_t* perceptron_weights = get_perceptron_weights(perceptron_table_idx);
  
  int sum = get_perceptron_sum(perceptron_table_idx);
  uint8_t perceptron_prediction = (sum >= 0) ? TAKEN : NOTTAKEN;
  
  if((perceptron_prediction != outcome) || (abs(sum) <= perceptron_theta)){
    // update bias term
    perceptron_table[perceptron_table_idx][0] += (outcome == TAKEN) ? 1 : -1;
    set_weight_limit(&perceptron_table[perceptron_table_idx][0]);
    // update other perceptron weights
    for(uint8_t weight_idx = 1; weight_idx < num_weights; weight_idx++){
      if(((perceptron_global_history >> weight_idx) & 1) == outcome){
        // when x_i and t are same then x_i * t = 1 -> so add 1
        perceptron_table[perceptron_table_idx][weight_idx] += 1;
      } else {
        // else it is = -1, subtract 1
        perceptron_table[perceptron_table_idx ][weight_idx] += -1;
      }
      set_weight_limit(&perceptron_table[perceptron_table_idx][weight_idx]);  
    }
  }
  
  //Updating the global register
  perceptron_global_history = (perceptron_global_history << 1 | outcome) & ((1 << (num_weights-1)) - 1);
  return;
}






