#include "predictor.h"

//Data Structures for gshare
uint8_t *gshare_bht;
uint64_t global_history; //history

int ghistoryBits; // Number of bits used for Global BHT

//Helper Functions
void prediction_helper(uint8_t *bht, uint32_t index, uint8_t outcome){
  uint8_t prediction = bht[index];
  if(prediction == WN){
    bht[index] = (outcome == NOTTAKEN) ? SN : WT;
  }
  else if (prediction == SN){
    bht[index] = (outcome == NOTTAKEN) ? SN : WN;
  }
  else if (prediction == WT){
    bht[index] = (outcome == NOTTAKEN) ? WN : ST;
  }
  else if (prediction == ST){
    bht[index] = (outcome == NOTTAKEN) ? WT : ST;
  }
}

//Init Function for ghare
void init_gshare(){
  int gshare_bht_size = 1 << ghistoryBits;
  gshare_bht = malloc((gshare_bht_size * sizeof(uint8_t)));
  memset(gshare_bht, WN, (gshare_bht_size * sizeof(uint8_t)));
  global_history = 0;

}

uint8_t fetch_gshare_prediction(uint32_t pc){
  uint32_t gshare_bht_size = 1 << ghistoryBits;
  uint32_t gshare_bht_index = (global_history ^ pc) & (gshare_bht_size - 1);
  uint8_t gshare_prediction = gshare_bht[gshare_bht_index];
  uint8_t outcome;
  if(gshare_prediction == WN || gshare_prediction == SN){
    outcome = NOTTAKEN;
  }
  else{
    outcome = TAKEN;
  }
  return outcome;
}

void training_gshare(uint32_t pc, uint8_t outcome){
  uint32_t gshare_bht_size = 1 << ghistoryBits;
  uint32_t gshare_bht_index = (global_history ^ pc) & (gshare_bht_size - 1);
  prediction_helper(gshare_bht, gshare_bht_index, outcome);
  //Updating the global register
  global_history = (global_history << 1);
  global_history &= ((1 << ghistoryBits) - 1);
  global_history = (global_history  | outcome);
}