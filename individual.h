#pragma once
#include "helpers.h"
#define MAX_SIZE 1000

class Individual
{
    private:
        int chromosome[MAX_SIZE];
        int chromosome_length;
        
        // NORMAL MUTATE: COMMENTED OUT FOR CURRENT TSP PROGRAM
        // int mutate_data[MAX_SIZE];
        // int mutate_count;
        
        // TSP
            //MUTATE
            int swap_mutate_data[MAX_SIZE][2];
            int swap_mutate_count;
            //RANK
            int id = -1;
        
        double fitness;
        double objective_value;

        // TRANSFORMATION DATA CONTAINS IS USED TO TEST WHETHER THE XOVER & MUTATE MECHANISMS WORKED CORRECTLY
        // LENGTH OF TRANSFORM_DATA IS ALWAYS THE FIRST ELEMENT (transform_data[0])
        int transform_data[MAX_SIZE];
        int transform_data_length = 0;
        
        //floorplanning
        int dimension_count;
        double dimensions[100];
        
        // VARIABLES FOR TESTING PROBABILITY ACCURACY
        // double mutation_count = 0;
        // double total_mutation_attempts = 0;

    public:
        Individual();
        Individual(int);
        Individual(int, int*);
        Individual(const Individual& rhs);
        ~Individual();

        void set_chromosome_length(int);
        void set_chromosome(int chromosome_length, int* chromosome);
        void set_chromosome_element(int index, int value);
        void set_fitness(double);
        void set_objective_value(double);
        void set_dimensions(double* dynamic_dimensions_data, int dimensions_count);

        int* get_chromosome();
        int get_chromosome_length();
        int* get_mutate_data(int row);
        int get_mutate_count();
        int* get_transform_data();
        double get_fitness();
        double get_objective_value();
        int get_dimension_count();
        double* get_dimensions();

        // TSP
        int get_id();
        void set_id(int id);
        void init_TSP(int random_seed, int srand_offset);
        
        //FUNCTIONS FOR TESTING PROBABILITY ACCURACY
        double get_mutation_percentage();

        void init(int random_seed, int srand_offset);
        void copy_individual_data(const Individual& individual_copy);

        // NORMAL MUTATE: COMMENTED OUT FOR CURRENT TSP PROGRAM
        // void mutate(double probability, int random_seed, int srand_offset);
        // TSP
        void swap_mutate(double probability, int random_seed, int srand_offset);

        //TEST FUNCTIONS
        void print_ind();
        void print_transform_data();
        // void fixed_low_fitness_init();
};