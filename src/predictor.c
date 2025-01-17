//========================================================//
//  predictor.c                                           //
//  Source file for the Branch Predictor                  //
//                                                        //
//  Implement the various branch predictors below as      //
//  described in the README                               //
//========================================================//
#include <stdio.h>
#include "predictor.h"
#include "gshare.c"
#include "tournament.c"
#include "perceptron.c"

const char *studentName1 = "Shivani Hariprasad";
const char *studentID1   = "A59019698";
const char *email1       = "shariprasad@ucsd.edu";

const char *studentName2 = "Rahul M Patil";
const char *studentID2   = "A59012896";
const char *email2       = "rmpatil@ucsd.edu";

//------------------------------------//
//      Predictor Configuration       //
//------------------------------------//

// Handy Global for use in output routines
const char *bpName[4] = { "Static", "Gshare",
                          "Tournament", "Custom" };

//------------------------------------//
//        Predictor Functions         //
//------------------------------------//

// Initialize the predictor
//
int bpType;       // Branch Prediction Type

//remove
int verbose;

void
init_predictor()
{
  // Call init based on prediction algorithm
  switch (bpType) {
    case STATIC: break;
    case GSHARE: init_gshare(); break;
    case TOURNAMENT: init_tournament(); break;
    case CUSTOM: init_perceptron(); break;
    default:
      break;
  }
}

// Make a prediction for conditional branch instruction at PC 'pc'
// Returning TAKEN indicates a prediction of taken; returning NOTTAKEN
// indicates a prediction of not taken
//
uint8_t
make_prediction(uint32_t pc)
{
  // Make a prediction based on the bpType
  switch (bpType) {
    case STATIC:
      return TAKEN; break;
    case GSHARE: 
      return fetch_gshare_prediction(pc); break;
    case TOURNAMENT: 
      return fetch_tournament_prediction(pc); break;
    case CUSTOM: 
      return fetch_perceptron_prediction(pc); break;
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
  // Call trainer based on prediction algorithm
  switch (bpType) {
    case STATIC: break;
    case GSHARE: training_gshare(pc, outcome); break;
    case TOURNAMENT: training_tournament(pc, outcome); break;
    case CUSTOM: training_perceptron(pc, outcome); break;
    default:
      break;
  }
}
