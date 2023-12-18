#pragma once
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <filesystem>
#include "population.h"
#include "test.h"

class GA
{
    private:
        Population *parent = nullptr;
        Population *child = nullptr;
        Population *m_temp = nullptr;
        int m_stats_count = 0;
        Options options;
        int extinction_counter;
        int total_super_individuals = 0;
        int total_semi_super_individuals = 0;
        int m_eval_option = -1;
        std::string m_tsp_tour_name;

    public:
        GA(int iterator, std::string problem_type, int ga_variant, std::string ga_variant_name_abbreviation, double mutation_probability, double xover_probability, int pop_size, int max_generations, int chromosome_length, std::ofstream& log_file_stream, std::string tsp_filename);
        GA(int population_size, double mutation_rate, double xover_rate, int chromosome_length, int iteration, std::string tsp_filename, std::ofstream& log);
        ~GA();

        void setup_options(int iterator, std::string problem_type, int ga_variant_option, std::string ga_variant_name_abbreviation, double mutation_rate, double xover_rate, int pop_size, int max_generations, int chromosome_length, std::ofstream& log_file_stream);
        void setup_options(int population_size, double mutation_rate, double xover_rate, int chromosome_length, int iteration, std::ofstream& log);
        void set_option_output_files(std::string iteration);
        void set_eval_option(int);
        void set_reporting_option(int);
        void set_tsp_tour_name(std::string tsp_filename);

        void init(int eval_option, int report_option);
        void run(int eval_option, int report_option);
        void run_genitor(long srand_offset);
        void run_genitor_on_imGui(long srand_offset, int iteration);
        bool extinction_check(int eval_option, int random_seed, long srand_offset);
        bool extinction_event(int eval_option, int random_seed, long srand_offset);
        void report_averager(int total_run_count);

        Options get_options() const;
        int get_eval_option() const;
        Population*& get_parent();

        //TSP
        void get_tour_length(std::ifstream& input_file_stream, int& chromosome_length);
        void set_tsp_data_option(std::string filename);
        int get_tsp_specs(std::ifstream& input_file_stream);
        void set_tsp_LTM_data_option(std::ifstream& input_file_stream); // LTM = Lower Triangular Matrix
        void set_tsp_COORDS_data_option(std::ifstream& input_file_stream);
        void set_tsp_xml_data_option(std::ifstream& input_file_stream);
        void write_EUC2Dcoords_to_file(std::string tsp_tour_name);
};