// NOTES
// May want to change ER operator so that rather than first city be among those with least number of edges to any random city first.
// When implementing selection pressure scaling, if good results aren't obtained, consider scaling all members from average member to fittest member, excluding all members below averaga member.
// Currently, selection pressure scaling is implemented by scaling all members towards the least fit member, which may be an issue. And the least fit member isn't scaled at all, which give promotes its value.
// When should I mutate? Before creating a child? After creating a child? Should I apply mutation to the whole population? Or only the new child?

#include "ga.h"
#include "test.h"
#include <chrono>

// TEST
#include "options.h"

// CB 10/20/23: THIS PROGRAM BREAKS IF I INSTANTIATE 2 POPULATION OBJECTS STATICALLY, BUT DOES NOT BREAK IF THEY ARE INSTANTIATED DYNAMICALLY. STATICALLY ALLOCATING A SINGLE POPULATION OBJECT DOESN'T BREAK THE PROGRAM. WHY?
int main(int argc, char* argv[])
{

    // TESTING EVALUATION IMPLEMENTATION
    // int population_size = 200;
    // double m_rate = 0.5;
    // double x_rate = 1;
    // std::ofstream log("log.txt");
    // int chrom_len = 52;
    // std::string filename = "raw_data/berlin52.tsp";
    // GA ga(population_size, m_rate, x_rate, chrom_len, 0, filename, log);
    // int berlin[52]{1, 49, 32, 45, 19, 41, 8, 9, 10, 43, 33, 51, 11, 52, 14, 13, 47, 26, 27, 28, 12, 25, 4, 6, 15, 5, 24, 48, 38, 37, 40, 39, 36, 35, 34, 44, 46, 16, 29, 50, 20, 23, 30, 2, 7, 42, 21, 17, 3, 18, 31, 22};
    // // int berlin[14]{1,13,2,3,4,5,11,6,12,7,10,8,9,14};
    // Individual ind(chrom_len, berlin);
    // Eval_results er;
    // TSP(ind, ga.get_options().tsp_data, ga.get_options().tsp_edge_weight_format, er, ga.get_eval_option());
    // print("berlin ind:");
    // ind.print_ind();
    // std::cout << "berlin Result:\n";
    // print("ga.get_options().tsp_edge_weight_format = ", ga.get_options().tsp_edge_weight_format);
    // print("ga.get_eval_option() = ", ga.get_eval_option());
    // std::cout << er.objective << std::endl;
    // return 0;

    // TEST FOR IM_GUI USAGE
    // int population_size = 200;
    // double m_rate = 0.01;
    // double x_rate = 1;
    // int chrom_len = std::stoi(argv[2]);
    // std::string filename = argv[1];
    // // int chrom_len = 76;
    // // std::string filename = "raw_data/eil76.tsp";
    // // int chrom_len = 24;
    // // std::string filename = "raw_data/gr24.tsp";
    // std::ofstream log("log.txt");
    // GA ga(population_size, m_rate, x_rate, chrom_len, 0, filename, log);
    // for(int i = 0; i < 30000; i ++)
    // {
    //     try
    //     {

    //         ga.run_genitor_on_imGui(i*pow(10,digit_count(ga.get_options().random_seed)), i);
            
    //         // TEST
    //         // if(!population_copy_achieved(*ga.get_parent()))
    //         // {
    //         //     print("iteration = ", i);
    //         //     return EXIT_FAILURE;
    //         // }
    //         // cin();
    //     }
    //     catch(std::string error_message)
    //     {
    //         print(error_message);
    //         return 0;
    //     }
    // }
    // log.close();
    // return 0;
    // END TEST
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::string problem_type = "TSP";
    int guaranteed_args = 4;

    if(argc < guaranteed_args)
    {
        std::cout << "Command line requirements:" << std::endl;
        std::cout << "argv[1] = TSP filename must be given." << std::endl;
        std::cout << "argv[2] = tour length" << std::endl;
        std::cout << "argv[3] = (1) to modify settings or (2) for default settings" << std::endl;
        std::cout << "\nOptional command line arguments:" << std::endl;
        std::cout << "argv[4] = reporting option" << std::endl;
        std::cout << "argv[5] = # of runs" << std::endl;
        std::cout << "argv[6] = GA variant " << std::endl;
        std::cout << "argv[7] = mutation rate " << std::endl;
        std::cout << "argv[8] = crossover rate " << std::endl;
        std::cout << "argv[9] = population size " << std::endl;
        std::cout << "argv[10] = max generations " << std::endl;
        std::cout << "\nCommand line requirements not given. Aborting." << std::endl;
        return EXIT_FAILURE;
    }

    int chromosome_length = -1;
    int argv_3 = -1;
    try
    {
        try_args_to_int_in_range(argv[2], chromosome_length, 5, 500, 500, "argv[2]");
        try_args_to_int_in_range(argv[3], argv_3, 1, 2, 2, "argv[3]");
    }
    catch(std::string error_message)
    {
        if(error_message == "argv[2]")
        {
            std::cout << "Invalid argv[2] given:" << std::endl;
            std::cout << "argv[2] = [5, 500]" << std::endl;
            std::cout << "Aborting" << std::endl;
        }
        else if(error_message == "argv[3]")
        {
            std::cout << "Invalid argv[3] given:" << std::endl;
            std::cout << "argv[3] = (1) to modify settings or (2) for default settings" << std::endl;
            std::cout << "Aborting" << std::endl;
        }
        return EXIT_FAILURE;
    }

    std::string TSP_tour_name;
    int j = 0;
    bool verify_dir = false;
    while(!verify_dir)
    {
        if(argv[1][j++] == '/')
            verify_dir = true;
    }
    if(!verify_dir)
        j = 0;
    for(int i = j; i < std::string(argv[1]).length(); i++)
    {
        if(argv[1][i] == '.')
            break;

        if(!(argv[1][i] == '_' || argv[1][i] == '/'))
            TSP_tour_name += argv[1][i];
    }
    problem_type += "_" + TSP_tour_name;
    
    std::string tsp_filename(argv[1]);

    // Reporting variables
    std::string reporting_option_str =  "ERROR";
    int report_option = -1;
    int report_option_default = 2;
    int report_option_first = 1;
    int report_option_last = 2;
    bool reporting_preset = false;
    
    // # of runs variables
    int runs = -1;
    int runs_default = 1;
    int runs_first = 1;
    int runs_last = 50;
    bool runs_preset = false;
    
    // GA-variant name variables
    std::string variant_name = "ERROR";
    std::string variant_name_abbreviation = "ERROR";
    int ga_variant_option = -1;
    int ga_variant_default = 5;
    int ga_variant_first = 1;
    int ga_variant_last = 5;
    bool ga_variant_preset = false;
    
    // Mutation rate variables
    double mutation_rate = -1;
    double mutation_rate_default = 0.05;
    double mutation_rate_first = 0;
    double mutation_rate_last = 1;
    bool mutation_rate_preset = false;
    
    // Crossover rate variables
    double xover_rate = -1;
    double xover_rate_default = 0.667;
    double xover_rate_first = 0;
    double xover_rate_last = 1;
    bool xover_rate_preset = false;
    
    // Crossover rate variables
    double pop_size = -1;
    double pop_size_default = 200;
    double pop_size_first = 2;
    double pop_size_last = 2000;
    bool pop_size_preset = false;
    
    // Crossover rate variables
    double max_generations = -1;
    double max_generations_default = 50000;
    double max_generations_first = 2;
    double max_generations_last = 1000000;
    bool max_generations_preset = false;

    bool all_preset = false;

    if(argc >= guaranteed_args + 1)
    {
        try
        {
            try_args_to_int_in_range(argv[guaranteed_args], report_option, report_option_first, report_option_last, report_option_default, "argv[" + std::to_string(guaranteed_args) + "]");
            set_reporting_str(reporting_option_str, report_option);
        }
        catch(std::string error_message)
        {
            set_reporting_str(reporting_option_str, report_option);
            std::cout << "\nInvalid value given for " << error_message << "; reporting option defaulting to " << reporting_option_str << std::endl;
        }
        reporting_preset = true;
    }
    if(argc >= guaranteed_args + 2)
    {
        try
        {
            try_args_to_int_in_range(argv[guaranteed_args + 1], runs, runs_first, runs_last, runs_default, "argv[" + std::to_string(guaranteed_args + 1) + "]");
        }
        catch(std::string error_message)
        {
            std::cout << "\nInvalid value given for " << error_message << "; number of runs defaulting to " << runs << std::endl;
        }
        runs_preset = true;
    }
    if(argc >= guaranteed_args + 3)
    {
        try
        {
            try_args_to_int_in_range(argv[guaranteed_args + 2], ga_variant_option, ga_variant_first, ga_variant_last, ga_variant_default, "argv[" + std::to_string(guaranteed_args + 2) + "]");
            set_variant_name(variant_name, variant_name_abbreviation, ga_variant_option);
        }
        catch(std::string error_message)
        {
            set_variant_name(variant_name, variant_name_abbreviation, ga_variant_option);
            std::cout << "\nInvalid value given for " << error_message << "; GA variant defaulting to " << variant_name << std::endl;
        }
        ga_variant_preset = true;
    }
    if(argc >= guaranteed_args + 4)
    {
        try
        {
            try_args_to_double_in_range(argv[guaranteed_args + 3], mutation_rate, mutation_rate_first, mutation_rate_last, mutation_rate_default, "argv[" + std::to_string(guaranteed_args + 3) + "]");
        }
        catch(std::string error_message)
        {
            std::cout << "\nInvalid value given for " << error_message << "; mutation rate defaulting to " << mutation_rate << std::endl;
        }
        mutation_rate_preset = true;
    }
    if(argc >= guaranteed_args + 5)
    {
        try
        {
            try_args_to_double_in_range(argv[guaranteed_args + 4], xover_rate, xover_rate_first, xover_rate_last, xover_rate_default, "argv[" + std::to_string(guaranteed_args + 4) + "]");
        }
        catch(std::string error_message)
        {
            std::cout << "\nInvalid value given for " << error_message << "; crossover rate defaulting to " << xover_rate << std::endl;
        }
        xover_rate_preset = true;
    }
    if(argc >= guaranteed_args + 6)
    {
        try
        {
            try_args_to_double_in_range(argv[guaranteed_args + 5], pop_size, pop_size_first, pop_size_last, pop_size_default, "argv[" + std::to_string(guaranteed_args + 5) + "]");
        }
        catch(std::string error_message)
        {
            std::cout << "\nInvalid value given for " << error_message << "; population size defaulting to " << pop_size << std::endl;
        }
        pop_size_preset = true;
    }
    if(argc >= guaranteed_args + 7)
    {
        try
        {
            try_args_to_double_in_range(argv[guaranteed_args + 6], max_generations, max_generations_first, max_generations_last, max_generations_default, "argv[" + std::to_string(guaranteed_args + 6) + "]");
        }
        catch(std::string error_message)
        {
            std::cout << "\nInvalid value given for " << error_message << "; max generations defaulting to " << max_generations << std::endl;
        }
        max_generations_preset = true;
    }

    if(reporting_preset && runs_preset && ga_variant_preset && mutation_rate_preset && max_generations_preset && pop_size_preset && max_generations_preset)
        all_preset = true;
        

    if(argc >= guaranteed_args && argv_3 == 1)
    {
        int choice = -1;
        int run_program_choice = 9;
        while(choice != run_program_choice)
        {
            switch(choice)
            {
                case 1:
                    std::cout << "\nChoose reporting option:" << std::endl;
                    std::cout << "1. Make reports" << std::endl;
                    std::cout << "2. Ignore reporting" << std::endl;
                    report_option = int_choice();
                    if(report_option < report_option_first || report_option > report_option_last)
                    {
                        std::cout << "\nInvalid option. Returning to main menu." << std::endl;
                    }
                    choice = -1;
                    break;
                case 2:
                    std::cout << "\nGive number of runs in the range [1, 50]" << std::endl;
                    runs = int_choice();
                    if(runs < runs_first || runs > runs_last)
                    {
                        std::cout << "\nInvalid option. Returning to main menu." << std::endl;
                    }
                    choice = -1;
                    break;
                case 3:
                    std::cout << "\nChoose GA variant:" << std::endl;
                    std::cout << "1. Simple GA" << std::endl;
                    std::cout << "2. Simple GA with Extinction" << std::endl;
                    std::cout << "3. CHC" << std::endl;
                    std::cout << "4. CHC with Extinction" << std::endl;
                    std::cout << "5. Genitor" << std::endl;
                    ga_variant_option = int_choice();
                    if(ga_variant_option < ga_variant_first || ga_variant_option > ga_variant_last)
                    {
                        std::cout << "\nInvalid option. Returning to main menu." << std::endl;
                    }
                    choice = -1;
                    break;
                case 4:
                    std::cout << "\nChoose mutation rate in the range [0, 1]:" << std::endl;
                    mutation_rate = double_choice();
                    if(mutation_rate < mutation_rate_first || mutation_rate > mutation_rate_last)
                    {
                        std::cout << "\nInvalid option. Returning to main menu." << std::endl;
                    }
                    choice = -1;
                    break;
                case 5:
                    std::cout << "\nChoose xover rate in the range [0, 1]:" << std::endl;
                    xover_rate = double_choice();
                    if(xover_rate < xover_rate_first || xover_rate > xover_rate_last)
                    {
                        std::cout << "\nInvalid option. Returning to main menu." << std::endl;
                    }
                    choice = -1;
                    break;
                case 6:
                    std::cout << "\nChoose population size in the range [2, 2000]:" << std::endl;
                    pop_size = int_choice();
                    if(pop_size < pop_size_first || pop_size > pop_size_last)
                    {
                        std::cout << "\nInvalid option. Returning to main menu." << std::endl;
                    }
                    choice = -1;
                    break;
                case 7:
                    std::cout << std::fixed << std::setprecision(0) << "\nChoose max generations in the range ["<<max_generations_first<<", "<<max_generations_last<<"]:" << std::endl;
                    max_generations = int_choice();
                    if(max_generations < max_generations_first || max_generations > max_generations_last)
                    {
                        std::cout << "\nInvalid option. Returning to main menu." << std::endl;
                    }
                    choice = -1;
                    break;
                case 8:
                    return 0;
                default:
                    if(!all_preset)
                    {
                        std::cout << "\nChoose a setting to modify:" << std::endl;
                    }
                    else
                    {
                        choice = run_program_choice;
                        break;
                    }
                    if(!reporting_preset)
                        std::cout << "1. Reporting" << std::endl;
                    else
                    {
                        std::cout << "1. NOT AN OPTION (REPORTING PRESET) - Current setting: " << reporting_option_str << std::endl;
                    }
                    if(!runs_preset)
                        std::cout << "2. Number of runs" << std::endl;
                    else
                    {
                        std::cout << "2. NOT AN OPTION (RUNS PRESET) - Current setting: " << runs << std::endl;
                    }
                    if(!ga_variant_preset)
                        std::cout << "3. GA variant" << std::endl;
                    else
                    {
                        std::cout << "3. NOT AN OPTION (GA VARIANT PRESET) - Current setting: " << variant_name << std::endl;
                    }
                    if(!mutation_rate_preset)
                        std::cout << "4. Mutation rate" << std::endl;
                    else
                    {
                        std::cout << "4. NOT AN OPTION (MUTATION RATE PRESET) - Current setting: " << std::fixed << std::setprecision(3) << mutation_rate << std::endl;
                    }
                    if(!xover_rate_preset)
                        std::cout << "5. Crossover rate" << std::endl;
                    else
                    {
                        std::cout << "5. NOT AN OPTION (CROSSOVER RATE PRESET) - Current setting: " << std::fixed << std::setprecision(3) << xover_rate << std::endl;
                    }
                    if(!pop_size_preset)
                        std::cout << "6. Population size" << std::endl;
                    else
                    {
                        std::cout << "6. NOT AN OPTION (POPULATION SIZE PRESET) - Current setting: " << pop_size << std::endl;
                    }
                    if(!max_generations_preset)
                        std::cout << "7. Max Geneartions" << std::endl;
                    else
                    {
                        choice = run_program_choice;
                        break;
                    }
                    std::cout << "8. Exit program" << std::endl;
                    std::cout << "9. Run program" << std::endl;

                    menu_choice_with_dynamic_truncation(argc, guaranteed_args + 1, run_program_choice, choice);
            }
        }

        if(equals(report_option, -1))
            report_option = report_option_default;
        if(equals(runs, -1))
            runs = runs_default;
        if(equals(ga_variant_option, -1))
        {
            ga_variant_option = ga_variant_default;
            set_variant_name(variant_name, variant_name_abbreviation, ga_variant_option);
        }
        else
            set_variant_name(variant_name, variant_name_abbreviation, ga_variant_option);
        if(equals(mutation_rate, -1))
            mutation_rate = mutation_rate_default;
        if(equals(xover_rate, -1))
            xover_rate = xover_rate_default;
        if(equals(pop_size, -1))
            pop_size = pop_size_default;
        if(equals(max_generations, -1))
            max_generations = max_generations_default;
    }

    if(argv_3 == 2)
    {
        report_option = report_option_default;
        runs = runs_default;
        ga_variant_option = ga_variant_default;
        set_variant_name(variant_name, variant_name_abbreviation, ga_variant_option);
        mutation_rate = mutation_rate_default;
        xover_rate = xover_rate_default;
        pop_size = pop_size_default;
        max_generations = max_generations_default;
    }

    switch(ga_variant_option)
    {
        case 1:
            std::cout << "Simple GA\n";
            break;
        case 2:
            std::cout << "Simple GA w/extinction\n";
            break;
        case 3:
            std::cout << "CHC\n";
            break;
        case 4:
            std::cout << "CHC w/extinction\n";
            break;
        case 5:
            std::cout << "Genitor\n";
            break;
    }

    // FOR RUNNING THE AVERAGER POST-TERMINATION /////////////////////////////////////////////
    // std::ofstream out;
    // GA ga(runs, problem_type, ga_variant_option, variant_name_abbreviation, mutation_rate, xover_rate, pop_size, max_generations, chromosome_length, out, tsp_filename);
    // ga.report_averager(runs);
    // return 0;
    //////////////////////////////////////////////////////////////////////////////////////////

    for(int j = 0; j < runs; j++)
    {
        auto start = std::chrono::high_resolution_clock::now();
        std::ofstream log_stream("log.txt", std::ios::app);

        try
        {
            GA ga(j, problem_type, ga_variant_option, variant_name_abbreviation, mutation_rate, xover_rate, pop_size, max_generations, chromosome_length, log_stream, tsp_filename);
            ga.set_reporting_option(report_option);

            // TEST
            print("RUN: ", j+1);
            // print("GA population size: ", ga.get_options().population_size);
            // print("GA max generations: ", ga.get_options().max_generations);
            
            try
            {
                ga.set_tsp_data_option(tsp_filename);
                if(ga.get_eval_option() == 2 || ga.get_eval_option() == 3)
                    ga.write_EUC2Dcoords_to_file(TSP_tour_name);
                
                // UNIT TEST: Symmetric TSP Matrix testing
                // if(!symmetric_matrix(ga.get_options().tsp_data,ga.get_options().chromosome_length))
                //     throw(std::string("Non-symmetric Matrix. Aborting program."));

                // TEST
                // for(int i = 0; i < ga.get_options().chromosome_length; i++)
                // {
                //     if(ga.get_eval_option() == 1)
                //     {
                //         for(int j =0 ; j < ga.get_options().chromosome_length; j++)
                //             print(ga.get_options().tsp_data[i][j], " ");
                //         endl();
                //     }
                //     if(ga.get_eval_option() == 2)
                //     {
                //         for(int j =0 ; j < 4; j++)
                //             print(ga.get_options().tsp_data[i][j], " ");
                //         endl();
                //     }
                // }
                // endl();
                // cin();
            }
            catch(std::string error_message)
            {
                throw(error_message);
            }

            int eval_option = ga.get_options().eval_option;

            try
                {ga.init(eval_option, report_option);}
            catch(std::string error_message)
            {
                throw(error_message);
            }

            try
            {
                if(variant_name_abbreviation != "G")
                    ga.run(eval_option, report_option);
                else
                    ga.run_genitor(j+1);
            }
            catch(std::string error_message)
            {
                std::cout << error_message << std::endl;
                return EXIT_FAILURE;
            }
        }
        catch (std::string error_message)
        {
            std::cout << error_message << std::endl;
            return EXIT_FAILURE;
        }

        delete_log(log_stream);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "\nDURATION = " << duration.count() << std::endl << std::endl;
    }

    // FILENAME SETUP FOR REPORT AVERAGER (REPORT AVERAGER RUNS AFTER FILENAME SETUP)
    if(report_option == 1)
    {
        std::ofstream out;
        GA ga(runs, problem_type, ga_variant_option, variant_name_abbreviation, mutation_rate, xover_rate, pop_size, max_generations, chromosome_length, out, tsp_filename);
        ga.report_averager(runs);
    }
    std::string tsp_coords_filename = TSP_tour_name + "_coords.txt";
    std::remove(tsp_coords_filename.c_str());
}