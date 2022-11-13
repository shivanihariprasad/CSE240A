//========================================================//
//  predictor.c                                           //
//  Source file for the Branch Predictor                  //
//                                                        //
//  Implement the various branch predictors below as      //
//  described in the README                               //
//========================================================//
#include <stdio.h>
#include "predictor.h"

//
// TODO:Student Information
//
const char *studentName = "Shivani Hariprasad";
const char *studentID   = "A59019698";
const char *email       = "shariprasad@ucsd.edu";

//------------------------------------//
//      Predictor Configuration       //
//------------------------------------//

// Handy Global for use in output routines
const char *bpName[4] = { "Static", "Gshare",
                          "Tournament", "Custom" };

int ghistoryBits; // Number of bits used for Global History
int lhistoryBits; // Number of bits used for Local History
int pcIndexBits;  // Number of bits used for PC index
int bpType;       // Branch Prediction Type
int verbose;

//Data Structures for gshare
uint8_t *gshare_bht;
uint64_t global_history; //history

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
    bht[index] = (outcome == NOTTAKEN) ? WT : ST
  }
}



//Init Function for ghare
void init_gshare(){
  int gshare_bht_size = 1 << ghistoryBits;
  gshare_bht = malloc((gshare_bht_size * sizeof(uint8_t));
  memset(gshare_bht, WN, (gshare_bht_size * sizeof(uint8_t));
  global_history = 0;

}
//
//TODO: Add your own Branch Predictor data structures here
//

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
  global_history = (global_history  | outcome);
}

//------------------------------------//
//        Predictor Functions         //
//------------------------------------//

// Initialize the predictor
//
void
init_predictor()
{
  //
  //TODO: Initialize Branch Predictor Data Structures
  //
}

// Make a prediction for conditional branch instruction at PC 'pc'
// Returning TAKEN indicates a prediction of taken; returning NOTTAKEN
// indicates a prediction of not taken
//
uint8_t
make_prediction(uint32_t pc)
{
  //
  //TODO: Implement prediction scheme
  //

  // Make a prediction based on the bpType
  switch (bpType) {
    case STATIC:
      return TAKEN;
    case GSHARE:
    case TOURNAMENT:
    case CUSTOM:
    default:
      break;
  }

  // If there is not a compatable bpType then return NOTTAKEN
  return NOTTAKEN;
}

// Train the predictor the last executed branch at PC 'pc' and with
// outcome 'outcome' (true indicates that the branch was taken, false
// indicates that the branch was not taken)
//
void
train_predictor(uint32_t pc, uint8_t outcome)
{
  //
  //TODO: Implement Predictor training
  //
}
