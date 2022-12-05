#include "predictor.h"

//Data Structures for tournament
uint8_t *tournament_gshare_bht;
uint64_t tournament_global_history;
uint8_t *tournament_local_bht;
uint16_t *tournament_local_predictor;
uint8_t *tournament_metapredictor;

int ghistoryBits; // Number of bits used for Global BHT
int lhistoryBits; //Number of bits used for local BHT
int pcIndexBits; // Number of bit used for local PHT

uint8_t localOutcome, globalOutcome;
// //Helper Functions
// void prediction_helper(uint8_t *bht, uint32_t index, uint8_t outcome){
//     uint8_t prediction = bht[index];
//     if(prediction == WN){
//     bht[index] = (outcome == NOTTAKEN) ? SN : WT;
//     }
//     else if (prediction == SN){
//     bht[index] = (outcome == NOTTAKEN) ? SN : WN;
//     }
//     else if (prediction == WT){
//     bht[index] = (outcome == NOTTAKEN) ? WN : ST;
//     }
//     else if (prediction == ST){
//     bht[index] = (outcome == NOTTAKEN) ? WT : ST;
//     }
// }

//Init Function for tournament
void init_tournament(){
    int gshare_bht_size = 1 << ghistoryBits;
    tournament_gshare_bht = malloc((gshare_bht_size * sizeof(uint8_t)));
    memset(tournament_gshare_bht, WN, (gshare_bht_size * sizeof(uint8_t)));
    tournament_global_history = 0;

    int local_predictor_size = 1 << pcIndexBits;
    tournament_local_predictor =  malloc(local_predictor_size * sizeof(uint32_t));
    memset(tournament_local_predictor, 0, (gshare_bht_size * sizeof(uint32_t)));

    int local_bht_size = 1 << lhistoryBits;
    tournament_local_bht = malloc(local_bht_size * sizeof(uint8_t));
    memset(tournament_local_bht, WN, (local_bht_size * sizeof(uint8_t)));

    int meta_predictor_size = 1 << ghistoryBits;
    tournament_metapredictor =  malloc(meta_predictor_size * sizeof(uint8_t));
    memset(tournament_metapredictor, WN, (meta_predictor_size * sizeof(uint8_t)));
}

uint8_t fetch_tournament_prediction(uint32_t pc){

    int localPHTIndex = pc & ((1<<pcIndexBits) - 1);
    uint8_t local_predicton =  tournament_local_bht[tournament_local_predictor[localPHTIndex]];
    localOutcome = ((local_predicton == WN || local_predicton == SN) ? NOTTAKEN : TAKEN);

    int globalBHTIndex = (tournament_global_history) & ( (1 << ghistoryBits) - 1);
    uint8_t global_prediction = tournament_gshare_bht[globalBHTIndex];
    globalOutcome = ((global_prediction == WN || global_prediction == SN) ? NOTTAKEN : TAKEN);

    uint32_t predictor = tournament_metapredictor[globalBHTIndex];
    if(predictor == WT || predictor == ST ){
    return localOutcome;
    }
    else{
    return globalOutcome;
  }
}


void training_tournament(uint32_t pc, uint8_t outcome){
    uint32_t localPHTIndex = pc & ((1<<pcIndexBits) - 1);
    uint32_t globalBHTIndex = (tournament_global_history) & ((1 << ghistoryBits) - 1);
    prediction_helper(tournament_gshare_bht, globalBHTIndex, outcome);
    prediction_helper(tournament_local_bht, tournament_local_predictor[localPHTIndex], outcome);
    if(globalOutcome!=localOutcome){
        prediction_helper(tournament_metapredictor, globalBHTIndex,  (localOutcome == outcome) ? TAKEN : NOTTAKEN);
    }
    
   //Updating the global register
    tournament_global_history = (tournament_global_history << 1);
    tournament_global_history &= ((1 << ghistoryBits) - 1);
    tournament_global_history = (tournament_global_history  | outcome);

    //Updating the local predictor
    tournament_local_predictor[localPHTIndex] <<= 1;
    tournament_local_predictor[localPHTIndex] &= ((1<<lhistoryBits)-1);
    tournament_local_predictor[localPHTIndex] = (tournament_local_predictor[localPHTIndex] | outcome);
}




