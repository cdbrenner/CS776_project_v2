#pragma once
#include <iostream>

struct Options
{
    std::string output_file;
    std::string output_file_o;
    std::string ave_file;
    std::string ave_file_o;
    std::string problem_type;
    std::string ga_variant_name;
    std::string ga_variant_name_abbreviation;
    std::string parameter_str;
    std::ofstream* log_stream;
    int reporting_option;
    int ga_variant_option;
    int GA_iteration;
    int print_precision;
    int print_precision_o;
    int eval_option;
    long int random_seed;
    int population_size;
    int chromosome_length;
    int max_generations;
    double xover_rate;
    double mutation_rate;
    double selection_pressure;
    double convergence_resolution_threshold;
    int extinction_delay;
    double super_individual_threshold;
    double semi_super_individual_threshold;

    // TSP
    int** tsp_data;
    int tsp_edge_weight_format = -1;
    
    //INDIVIDUAL::TRANSFORM_DATA INDICES
    int m_isParent_index = 1; // USED IN CHC TO TELL WHETHER COMPARISON IS MADE TO GRANDPARENT (1) OR CHILD (0)
    int m_parent_1_index = 2;
    int m_parent_2_index = 3;
    int m_xover_index = 4;
    int m_meta_length = m_xover_index + 1; // NUMBER OF ELEMENTS BEFORE ATTACHING MUTATE DATA
};