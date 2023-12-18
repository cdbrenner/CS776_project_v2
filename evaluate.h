#pragma once
#include <cmath>
#include <random>
#include "individual.h"
#include "eval_results.h"

void eval(Individual&, Eval_results& results, double** tour_data, int choice, int random_seed, long srand_offset, int eval_option);

double* decode(Individual&, int bit_length, double scaler);
double* decode_withVarsOfDiffBitLength_onlyPositive(Individual&, int variable_count, int bit_length[], double scaler[]);

// TSP
void TSP(Individual& individual, double** tour_data, int choice, Eval_results& results, int eval_option);
void TSP_EXPLICIT(Individual& individual, double** tour_data, int choice, Eval_results& results);
void TSP_COORDS(Individual& individual, double** tour_data, int choice, Eval_results& results);
double distance(double x1, double y1, double x2, double y2);
double distance_geo(double x1, double y1, double x2, double y2);