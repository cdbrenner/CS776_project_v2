#pragma once
#include <sstream>
#include "population.h"
#include "test.h"
#include "stats.h"

class GA
{
    private:
        Population *parent = nullptr;
        Population *child = nullptr;
        Population *m_temp = nullptr;
        Stats *m_stats = nullptr;
        int** tsp_data;
        int m_stats_count = 0;
        Options options;
        int extinction_counter;
        int total_super_individuals = 0;
        int total_semi_super_individuals = 0;
        int m_eval_option = -1;

    public:
        GA(int argc, char *argv[], int iterator, std::string problem_type, int ga_variant, std::string ga_variant_name_abbreviation, double mutation_probability, double xover_probability, int chromosome_length, std::ofstream& log_file_stream, std::string tsp_filename);
        ~GA();

        void setup_options(int argc, char *argv[], int iterator, std::string problem_type, int ga_variant_option, std::string ga_variant_name_abbreviation, double mutation_rate, double xover_rate, int chromosome_length, std::ofstream& log_file_stream);
        void set_option_output_files(std::string iteration);
        void set_eval_option(int);
        void set_reporting_option(int);

        void init(int eval_option, int report_option);
        void run(int eval_option, int report_option);
        void run_genitor(int srand_offset);
        bool extinction_check(int eval_option, int random_seed, int srand_offset);
        bool extinction_event(int eval_option, int random_seed, int srand_offset);
        void report_averager(int total_run_count);

        Options get_options() const;
        int get_eval_option() const;

        //TSP
        void get_tour_length(std::ifstream& input_file_stream, int& chromosome_length);
        void set_tsp_data_option(std::string filename);
        int get_tsp_specs(std::ifstream& input_file_stream);
        void set_tsp_LTM_data_option(std::ifstream& input_file_stream); // LTM = Lower Triangular Matrix
        void set_tsp_EUC2D_data_option(std::ifstream& input_file_stream);
        void set_tsp_xml_data_option(std::ifstream& input_file_stream);
};