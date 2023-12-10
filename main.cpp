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

    // TEST
    // Options opt;
    // opt.chromosome_length = 6;
    // opt.population_size = 100;
    // opt.selection_pressure = 1.9;
    // opt.mutation_rate = .05;
    // opt.random_seed = time(NULL);

    // Population test(opt);
    // int n = 99;
    // for(int i = 0; i < 100; i++)
    // {
    //     if(i%2 == 0)
    //         test.get_members()[i].set_fitness(i*2);
    //     else
    //     {
    //         test.get_members()[i].set_fitness(n*2);
    //         n--;
    //     }
    // }

    // int total_fitness = 0;
    // for(int i = 0; i < 100; i++)
    //     total_fitness += test.get_members()[i].get_fitness();

    // print("total_fitness = ", total_fitness);
    // endl();

    // test.set_member_ids();
    // test.sort_member_ids_by_fitness(opt.random_seed, 0);
    // print("Before rank conversion");
    // test.print_member_ids();
    // endl();
    // print("After rank conversion");
    // test.convert_member_ids_fitness_to_rank();
    // test.print_member_ids();
    // endl();

    // test.set_average_rank_before_scaling();
    // std::cout << "average = " << test.get_average_before_scaling() << std::endl;
    // for(int i = 0; i < 100000; i++)
    // {
    //     int index_1 = test.rank_selection(i*2);
    //     int index_2 = test.rank_selection(i*2 + 1);

    //     // TEST
    //     // print("index_1 = ", index_1);
    //     // print("index_2 = ", index_2);
    //     // endl();

    //     test.rank_selection_stats(index_1, index_2, 2);
    // }

    // test.print_selection_count_vertical(2);
    // endl();
    // test.print_rand_count_vertical(4);

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

    if(reporting_preset && runs_preset && ga_variant_preset && mutation_rate_preset && xover_rate_preset)
        all_preset = true;
        

    if(argc >= guaranteed_args && argv_3 == 1)
    {
        int choice = -1;
        int run_program_choice = 7;
        int count_of_first_argv_variable = 5;
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
                        report_option = 2;
                    }
                    choice = -1;
                    break;
                case 2:
                    std::cout << "\nGive number of runs in the range [1, 50]" << std::endl;
                    runs = int_choice();
                    if(runs < runs_first || runs > runs_last)
                    {
                        std::cout << "\nInvalid option. Returning to main menu." << std::endl;
                        runs = 1;
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
                        ga_variant_option = 5;
                    }
                    choice = -1;
                    break;
                case 4:
                    std::cout << "\nChoose mutation rate in the range [0, 1]:" << std::endl;
                    mutation_rate = double_choice();
                    if(mutation_rate < mutation_rate_first || mutation_rate > mutation_rate_last)
                    {
                        std::cout << "\nInvalid option. Returning to main menu." << std::endl;
                        mutation_rate = 0.01;
                    }
                    choice = -1;
                    break;
                case 5:
                    std::cout << "\nChoose xover rate in the range [0, 1]:" << std::endl;
                    xover_rate = double_choice();
                    if(xover_rate < xover_rate_first || xover_rate > xover_rate_last)
                    {
                        std::cout << "\nInvalid option. Returning to main menu." << std::endl;
                        xover_rate = 0.667;
                    }
                    choice = -1;
                    break;
                case 6:
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
                        choice = run_program_choice;
                        break;
                    }
                    std::cout << "6. Exit program" << std::endl;
                    std::cout << "7. Run program" << std::endl;

                    menu_choice_with_dynamic_truncation(argc, count_of_first_argv_variable, run_program_choice, choice);
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
    }

    if(argv_3 == 2)
    {
        report_option = report_option_default;
        runs = runs_default;
        ga_variant_option = ga_variant_default;
        set_variant_name(variant_name, variant_name_abbreviation, ga_variant_option);
        mutation_rate = mutation_rate_default;
        xover_rate = xover_rate_default;
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

    for(int j = 0; j < runs; j++)
    {
        auto start = std::chrono::high_resolution_clock::now();
        std::ofstream log_stream("log.txt", std::ios::app);

        try
        {
            GA ga(argc, argv, j, problem_type, ga_variant_option, variant_name_abbreviation, mutation_rate, xover_rate, chromosome_length, log_stream, tsp_filename);
            ga.set_reporting_option(report_option);
            
            try
            {
                ga.set_tsp_data_option(tsp_filename);
                
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
        GA ga(argc, argv, runs, problem_type, ga_variant_option, variant_name_abbreviation, mutation_rate, xover_rate, chromosome_length, out, tsp_filename);
        ga.report_averager(runs);
    }
}