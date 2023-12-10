#pragma once
#include <iostream>
#include <string>
#include "evaluate.h"
#include "population.h"
#include "ga.h"

void max_floorplanning_objective_value(int, int, int);

void test_nine_variables_in_random_choice(int, int);

void verify_xover_mutation_mechanism(Population* parent, Population* child);

void verify_string_equivalence(Individual* one, Individual* two, std::string function_location);

bool duplicate_values_in_string(Individual&, int chromosome_length);

bool symmetric_matrix(int** tour_data, int chromosome_length);

bool identical_matrices(int tour_data_1[][48], int tour_data_2[][48], int chromosome_length);