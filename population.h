#pragma once
#include <memory>
#include "helpers.h"
#include "options.h"
#include "individual.h"
#include "evaluate.h"
// #include "partition/partition_debug.cpp"
#include "partition/partition.cpp"
#define TRANSFORM_DATA_SIZE 1000

class Population
{
    private:
        Options* options;
        Eval_results m_results;
        Individual* members = nullptr;
        std::ofstream* log_stream = nullptr;
        int transform_data[TRANSFORM_DATA_SIZE][TRANSFORM_DATA_SIZE];
        int transform_data_count = 0;
        int child_transform_data[TRANSFORM_DATA_SIZE][TRANSFORM_DATA_SIZE];
        int child_transform_data_count = 0;
        double average, max, min, sum_fitness, convergence;
        double average_objective, max_objective, min_objective, sum_objective;
        int super_individuals = 0;
        int semi_super_individuals = 0;
        int max_fitness_member_index = -1;
        int m_srand_offset_count = 0;

        // TSP
            //PMX
            int pmx_indices[2] = {-1,-1};
            //RANK
            double** member_ids = nullptr; // DOESN'T HAVE A DELETE[] YET
            double sum_rank = 0;
            double m_average_rank_before_scaling = 0;

        //MEMBERS FOR TESTING PROBABILITY ACCURACY
        double proportional_fitnesses[4];
        double member_chosen_count[4];

        // VALIDATE STATS
        double selection_count[1000][2];
        int selection_count_length = 0;
        double selection_count_total = 0;
        double rand_count[10000][2];
        int rand_count_length = 0;
        double rand_count_total = 0;
        double mutation_count = 0;
        double mutation_total_possible = 0;

    public:
        Population();
        Population(Options&);
        Population(Options&, int);
        Population(Population& copy_population);
        ~Population();

        Eval_results& get_results();

        void init_transform_data(int row);
        void set_options(Options&);
        void set_members();
        void set_members(int);
        void set_members_ptr(Individual*);
        void copy_members_and_update_id(const Population& population_copy);
        void copy_population(const Population& population_copy);
        void set_transform_data_by_row(int* transform_data_row, int row);
        void set_child_transform_data_by_row(int* child_transform_data, int row);
        void reset_super_individual_count();
        int find_max_fitness_member();
        int find_min_fitness_member();

        Individual* get_members();
        int* get_transform_data(int row);
        int* get_child_transform_data(int row);
        double get_average();
        double get_max();
        double get_min();
        double get_sum_fitness();
        double get_convergence();
        Options*& get_options();
        int* get_pmx_indices();

        //FUNCTIONS FOR TESTING PROBABILITY ACCURACY
        // void set_proportional_fitnesses();
        // void set_member_chosen_count();
        void get_member_chosen_stats();

        void evaluate(int choice, int** tsp_data, int random_seed, int srand_offset, int eval_option);
        void evaluate_single(Individual&, int choice, int** tsp_data, int random_seed, int srand_offset, int eval_option);
        void stats(int& total_super_individuals, int& total_semi_super_individuals);
        void report(int generation, int option, int total_super_individuals, int total_semi_super_individuals, bool extinction_event);
        void generation(Population*& child, int srand_offset);
        void genitor(int srand_offset, int eval_option);
        void CHC_generation(Population* child, Population *temp, int srand_offset);
        int proportional_selection(int srand_offset);
        void xover_mutate(Individual*,Individual*,Individual*,Individual*, int);
        
        // TSP
        void set_member_ids();
        double** get_member_ids();
        void sort_member_ids_by_fitness(int random_seed, int srand_offset);
        void convert_member_ids_fitness_to_rank();
        void selection_pressure_scaling();
        int average_rank();
        void rank_selection_prep(int srand_offset);
        int rank_selection(int srand_offset); // IMPLEMENTED SO THAT EQUAL FITNESS MEMBERS HAVE THE SAME RANK. THEREFORE, EXPECT SLOW CONVERGENCE.
        void edge_recombination(Individual*, Individual*, Individual&, int);
        void build_edge_map(Individual* parent_1, Individual* parent_2, int edge_map[][5]);
        void rebuild_edge_map(int edge_map[][5], int city_to_remove, int previous_city);
        bool edge_check(int edge_map[][5], int city, int edge);
        
        
        int one_point_xover(Individual*&,Individual*&,Individual*&,Individual*&, int);
        void pmx(Individual*,Individual*,Individual*,Individual*, int);
        void scramble_mutation(Individual&, int srand_offset);

        // STATS
        void rank_selection_stats(int parent_1_index, int parent_2_index, int precision);
        void rand_stats(double number, int precision);

        //TEST
        void print_pop();
        void print_member_ids();
        void print_edge_map(int [][5]);
        void print_selection_count_horizontal(int precision);
        void print_selection_count_vertical(int precision);
        void print_rand_count_vertical(int precision);
        void set_average_rank_before_scaling();
        double get_average_before_scaling();
};