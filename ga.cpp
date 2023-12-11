#include "ga.h"

GA::GA(int iterator, std::string problem_type, int ga_variant_option, std::string ga_variant_name_abbreviation, double mutation_rate, double xover_rate, int chromosome_length, std::ofstream& log_stream, std::string tsp_filename)
{
    options.tsp_data = new int*[chromosome_length];
    for(int i = 0; i < chromosome_length; i++)
    {
        options.tsp_data[i] = new int[chromosome_length];
        for(int j = 0; j < chromosome_length; j++)
            options.tsp_data[i][j] = -1;
    }

    setup_options(iterator, problem_type, ga_variant_option, ga_variant_name_abbreviation, mutation_rate, xover_rate, chromosome_length, log_stream);
}

GA::GA(int population_size, double mutation_rate, double xover_rate, int chromosome_length, int iteration, std::string tsp_filename)
{
    options.tsp_data = new int*[chromosome_length];
    for(int i = 0; i < chromosome_length; i++)
    {
        options.tsp_data[i] = new int[chromosome_length];
        for(int j = 0; j < chromosome_length; j++)
            options.tsp_data[i][j] = -1;
    }

    setup_options(population_size, mutation_rate, xover_rate, chromosome_length, iteration);

    // TEST
    // print("GA::GA: After setup options");

    set_tsp_tour_name(tsp_filename);

    // TEST
    // print("GA::GA: After set_tsp_tour_name");

    set_tsp_data_option(tsp_filename);

    // TEST
    // print("GA::GA: After set_tsp_data_option");
    
    if(get_eval_option() == 2 || get_eval_option() == 3)
    {
        write_EUC2Dcoords_to_file(m_tsp_tour_name);
        
        // TEST
        // print("GA::GA: After write_EUC2Dcoords_to_file");
    }

    // TEST
    // print("GA::GA: END");
}


GA::~GA()
{
    delete parent;
    delete child;
    delete m_temp;
    for(int i = 0; i < options.chromosome_length; i++)
        delete[] options.tsp_data[i];
    delete[] options.tsp_data;
}

void GA::setup_options(int iterator, std::string problem_type, int ga_variant_option, std::string ga_variant_name_abbreviation, double mutation_rate, double xover_rate, int chromosome_length, std::ofstream& log_stream)
{
    options.log_stream = &log_stream;

    options.GA_iteration = iterator;
    options.random_seed = time(NULL) + iterator;
    options.ga_variant_option = ga_variant_option;
    options.mutation_rate = mutation_rate;
    options.xover_rate = xover_rate;
    options.chromosome_length = chromosome_length;
    options.extinction_delay = 1;
    options.convergence_resolution_threshold = 0.00001;
    options.super_individual_threshold = 2;
    options.semi_super_individual_threshold = 1.2;
    options.selection_pressure = 1.9;
    
    options.population_size = 200; //POPULATION SIZE CANNOT EQUAL ODD NUMBER OR SEGMENT FAULT!!!
    options.max_generations = 30000;

    std::string temp_1;
    std::string temp_2;
    string_stream(temp_1, options.xover_rate, 2);
    string_stream(temp_2, options.mutation_rate, 3);
    options.parameter_str =  temp_2 + "M_" + temp_1 + "X";
    options.problem_type = problem_type;
    options.ga_variant_name_abbreviation = ga_variant_name_abbreviation;
    set_option_output_files(std::to_string(iterator));

    options.ave_file = "output_TSP_AVE.txt";
    options.ave_file_o = "output_TSP_AVE_ObjFnc.txt";
    options.print_precision = 4;
    options.print_precision_o = 2;
    
}

void GA::setup_options(int population_size, double mutation_rate, double xover_rate, int chromosome_length, int iteration)
{
    options.random_seed = time(NULL) + iteration;
    options.mutation_rate = mutation_rate;
    options.xover_rate = xover_rate;
    options.chromosome_length = chromosome_length;
    options.extinction_delay = 1;
    options.convergence_resolution_threshold = 0.00001;
    options.super_individual_threshold = 2;
    options.semi_super_individual_threshold = 1.2;
    options.selection_pressure = 1.9;
    options.ga_variant_name_abbreviation = "G";
    
    options.population_size = population_size; //POPULATION SIZE CANNOT EQUAL ODD NUMBER OR SEGMENT FAULT!!! - This may not be true for genitor (its an old note...) but I havent verified.(CB.231210) 
    options.max_generations = 30000;

    options.GA_iteration = iteration;
}

void GA::set_option_output_files(std::string run_number)
{
    options.output_file = options.problem_type + "_" + options.ga_variant_name_abbreviation + "_" + options.parameter_str + "_" + run_number  + ".txt";
    options.output_file_o = options.problem_type + "_" + options.ga_variant_name_abbreviation + "_" + options.parameter_str + "_" + run_number  + "_Obj.txt";
}

void GA::set_eval_option(int option)
{
    options.eval_option = option;
}

void GA::set_reporting_option(int option)
{
    options.reporting_option = option;
}

void GA::set_tsp_tour_name(std::string tsp_filename)
{
    int j = 0;
    bool verify_dir = false;
    while(!verify_dir)
    {
        if(tsp_filename[j++] == '/')
            verify_dir = true;
    }
    if(!verify_dir)
        j = 0;
    for(int i = j; i < tsp_filename.length(); i++)
    {
        if(tsp_filename[i] == '.')
            break;

        if(!(tsp_filename[i] == '_' || tsp_filename[i] == '/'))
            m_tsp_tour_name += tsp_filename[i];
    }
}

void GA::init(int eval_option, int report_option)
{
    if(report_option == 1)
    {
        std::ofstream out(options.output_file);
        out << "RANDOM SEED, " << options.random_seed << std::endl;
        out << "GEN,\tMIN,\t\t\t\tAVE,\t\t\t\tMAX,\t\t\t\tEXTINCT,\t\tCONV,\t\t\tSEMI,\t\tSUPER,\t\tT_SEMI,\t\tT_SUPER,\t\t\tTOUR\n";
        out.close();
    
        std::ofstream out_2(options.output_file_o);
        out_2 << "RANDOM SEED, " << options.random_seed << std::endl;
        out_2 << "GEN,\tMIN,\t\t\tAVE,\t\t\tMAX,\t\t\tEXTINCT,\t\tCONV,\t\t\tSEMI,\t\tSUPER,\t\tT_SEMI,\t\tT_SUPER,\t\t\tTOUR\n";
        out_2.close();
    }

    extinction_counter = 0;
    parent = new Population(options);
    
    if(options.ga_variant_name_abbreviation != "G")
    {
        child = new Population(options);
        m_temp = new Population(options);
    }

    try
    {
        parent->evaluate(options.tsp_edge_weight_format, options.tsp_data, options.random_seed, 0, m_eval_option);
    }
    catch(std::string error_message)
    {
        throw(error_message);
    }
    parent->set_member_ids();

    parent->stats(total_super_individuals, total_semi_super_individuals);

    if(report_option == 1)
        parent->report(0, 1, total_super_individuals, total_semi_super_individuals, false);
}

void GA::run(int eval_option, int report_option)
{
    bool extinction = false;

    for(int i = 1; i < options.max_generations; i++)
    {
        long srand_offset = i*options.population_size;
        
        parent->generation(child, i);
        try
        {
            child->evaluate(eval_option, options.tsp_data, options.random_seed, srand_offset, m_eval_option);
        }
        catch(std::string error_message)
        {
            throw(error_message);
        }

        if(options.ga_variant_name_abbreviation == "CHC" || options.ga_variant_name_abbreviation == "CHC-E")
            parent->CHC_generation(child, m_temp, srand_offset);

        child->stats(total_super_individuals, total_semi_super_individuals);

        if(report_option == 1)
        {
            child->report(i, 1, total_super_individuals, total_semi_super_individuals, extinction);
            extinction ? extinction = false : 0;
        }
        
        Population *temp = parent;
        parent = child;
        child = temp;

        parent->reset_super_individual_count();
        child->reset_super_individual_count();

        if(options.ga_variant_name_abbreviation == "S-E" || options.ga_variant_name_abbreviation == "CHC-E")
            extinction = extinction_check(eval_option, options.random_seed, srand_offset);
    }
}

void GA::run_genitor(long srand_offset)
{
    for(int i = 1; i < options.max_generations; i++)
    {
        parent->genitor(i*srand_offset, m_eval_option);
        parent->stats(total_super_individuals, total_semi_super_individuals);
        if(options.reporting_option == 1)
            parent->report(i, 1, total_super_individuals, total_semi_super_individuals, 0);
    }
}

void GA::run_genitor_on_imGui(long srand_offset)
{
    // TEST
    // print("GA::run_genitor_on_imGui");

    if(srand_offset == 0)
    {
        // TEST
        // print("IN IF: iteration = ", srand_offset);

        parent = new Population(options);
        try
        {
            parent->evaluate(options.tsp_edge_weight_format, options.tsp_data, options.random_seed, 0, m_eval_option);
            parent->stats(total_super_individuals, total_semi_super_individuals);
            parent->report_single();
            parent->write_population_to_file();
        }
        catch(std::string error_message)
        {
            throw(error_message);
        }
    }
    else
    {
        // TEST
        // print("IN ELSE: iteration = ", srand_offset);

        std::ifstream in("population.txt");
        parent = new Population(options, in);
        in.close();

        try
        {
            parent->genitor(srand_offset, m_eval_option);
            parent->stats(total_super_individuals, total_semi_super_individuals);
            parent->report_single();
            parent->write_population_to_file();
        }
        catch(std::string error_message)
        {
            throw(error_message);
        }
    }
}

bool GA::extinction_check(int eval_option, int random_seed, long srand_offset)
{
    double convergence = parent->get_convergence();
    double convergence_check = convergence - 1;

    convergence_check < options.convergence_resolution_threshold ? extinction_counter++ : extinction_counter = 0;

    return extinction_counter == options.extinction_delay ? extinction_event(eval_option, options.random_seed, srand_offset) : false;
}

bool GA::extinction_event(int eval_option, int random_seed, long srand_offset)
{
    extinction_counter = 0;
    if(options.ga_variant_name == "S-E")
    {
        int max_fitness_member = parent->find_max_fitness_member();
        parent->get_members()[0] = parent->get_members()[max_fitness_member];
    }

    for(int i = 1; i < options.population_size; i++)
    {
        parent->get_members()[i].init_TSP(random_seed, srand_offset + i);
    }

    try
    {
        parent->evaluate(eval_option, options.tsp_data, options.random_seed, srand_offset, m_eval_option);
    }
    catch(double variable_value[])
    {
        throw(variable_value);
    }

    parent->stats(total_super_individuals, total_semi_super_individuals);

    return true;
}

void GA::report_averager(int total_run_count)
{
    double temp_fitness_data[options.max_generations][4];
    double temp_objective_data[options.max_generations][4];
    for(int i = 0; i < options.max_generations; i++)
    {
        for(int j = 1; j < 4; j++)
        {
            temp_fitness_data[i][0] = i;
            temp_fitness_data[i][j] = 0;
            
            temp_objective_data[i][0] = i;
            temp_objective_data[i][j] = 0;
        }
    }
    
    for(int i = 0; i < total_run_count; i++)
    {
        std::string filename = options.problem_type + "_" + options.ga_variant_name_abbreviation + "_" + options.parameter_str + "_" + std::to_string(i) + ".txt";
        std::string filename_o = options.problem_type + "_" + options.ga_variant_name_abbreviation + "_" + options.parameter_str + "_" + std::to_string(i) + "_Obj.txt";
        std::ifstream in(filename);
        std::ifstream in_o(filename_o);

        for(int k = 0; k < options.max_generations + 2; k++)
        {
            std::string temp_1;
            std::string temp_2;
            if(k == 0 || k == 1) // GETS RID OF 1ST LINE (RANDOM SEED) AND 2nd LINE (HEADERS)
            {
                getline(in, temp_1);
                getline(in_o, temp_2);
            }
            else
            {
                getline(in,temp_1,',');
                getline(in_o,temp_2,',');
                for(int j = 1; j < 4; j++)
                {
                    getline(in,temp_1,',');
                    getline(in_o,temp_2,',');
                    temp_fitness_data[k-2][j] += std::strtod(temp_1.c_str(), NULL);
                    temp_objective_data[k-2][j] += std::strtod(temp_2.c_str(), NULL);
                }
             
                getline(in,temp_1);
                getline(in_o,temp_1);
            }
        }

        in.close();
        in_o.close();
    }
    
    for(int i = 0; i < options.max_generations; i++)
    {
        for(int j = 1; j < 4; j++)
        {
            temp_fitness_data[i][j] /= total_run_count;
            temp_objective_data[i][j] /= total_run_count;
        }
    }

    std::string out_filename = options.problem_type + "_" + options.ga_variant_name_abbreviation + "_" + options.parameter_str + "_AVE.txt";
    std::string out_filename_o = options.problem_type + "_" + options.ga_variant_name_abbreviation + "_" + options.parameter_str + "_AVE_Obj.txt";
    std::ofstream out(out_filename);
    std::ofstream out_o(out_filename_o);

    out << "RANDOM SEED, " << options.random_seed << std::endl;
    out << "GEN,\tMIN,\t\t\tAVE,\t\t\tMAX,\n";
    out_o << "RANDOM SEED, " << options.random_seed << std::endl;
    out_o << "GEN,\tMIN,\t\t\tAVE,\t\t\tMAX,\n";
    for(int i = 0; i < options.max_generations; i++)
    {
        out << std::scientific << std::setprecision(options.print_precision) << add_whitespace(i, options.max_generations, true)
                << (int)temp_fitness_data[i][0] << ",\t\t" << temp_fitness_data[i][1] << ",\t\t" << temp_fitness_data[i][2] << ",\t\t" << temp_fitness_data[i][3] << std::endl;

        out_o << std::fixed << std::setprecision(options.print_precision_o) << add_whitespace(i, options.max_generations, true)
                << (int)temp_objective_data[i][0] << ",\t\t" << temp_objective_data[i][1] << ",\t\t" << temp_objective_data[i][2] << ",\t\t" << temp_objective_data[i][3] << std::endl;
    }
    out.close();
    out_o.close();
}

Options GA::get_options() const
{
    return options;
}

int GA::get_eval_option() const
{
    return m_eval_option;
}

Population*& GA::get_parent()
{
    return parent;
}

// TSP
void GA::get_tour_length(std::ifstream& in, int& chromosome_length)
{
    if(in.good())
    {
        std::string temp;
        int count = 0;
        while(!(temp == "EDGE_WEIGHT_SECTION" || temp == "NODE_COORD_SECTION"))
            getline(in, temp);

        while(temp != "EOF")
        {
            getline(in, temp);
            if(temp != "EOF")
            {
                count++;
                print("temp = " + temp);
                print("count = ", count);
                endl();
            }
        }
        chromosome_length = count;
    }
    else
        throw(std::string("GA::set_tsp_specs: TSP file doesn't exist. Program aborted."));
}

void GA::set_tsp_data_option(std::string filename)
{
    std::ifstream in(filename);
    if(in.good())
    {
        int choice = get_tsp_specs(in);
        switch(choice) // Valid choices based on return equation given in GA::get_tsp_specs()
        {
            case 11: // EDGE_WEIGHT_TYPE: EXPLICIT; EDGE_WEIGHT_FORMAT: LOWER_DIAG_ROW
                in.clear();
                in.seekg(0);
                set_tsp_LTM_data_option(in);
                m_eval_option = 1;
                break;
            case 2: // 
                in.clear();
                in.seekg(0);
                set_tsp_EUC2D_data_option(in);
                m_eval_option = 2;
                break;
            default:
                in.close();
                throw(std::string("GA::set_tsp_data_option: TSP specs are not recognizable."));
        }
        in.close();
    }
    else
    {
        in.close();
        throw(std::string("GA::set_tsp_data_option: TSP file doesn't exist. Program aborted."));
    }
}

int GA::get_tsp_specs(std::ifstream& in)
{
    if(in.good())
    {
        int a = 0;
        int b = 0;
        std::string temp_1;
        while(!(temp_1 == "EDGE_WEIGHT_TYPE:" || temp_1 == "EDGE_WEIGHT_TYPE") && in.good())
            in >> temp_1;
        in >> temp_1;

        if(temp_1 == ":")
        {

            in >> temp_1;
        }

        if(temp_1 == "EXPLICIT")
            a = 1;
        else if(temp_1 == "EUC_2D")
        {
            a = 2;
            options.tsp_edge_weight_format = 2;
        }
        else if(temp_1 == "GEO")
        {
            a = 3;
            options.tsp_edge_weight_format = 3;
        }
        // EXPAND IF-STATEMENTS HERE FOR MORE SPECS IF NEEDED

        in.clear();
        in.seekg(0);

        while(temp_1 != "EDGE_WEIGHT_FORMAT:" && in.good())
            in >> temp_1;
        in >> temp_1;

        if(temp_1 == "LOWER_DIAG_ROW")
            b = 10;
        // EXPAND IF-STATEMENTS HERE FOR MORE SPECS IF NEEDED

        return a+b;
    }
    else
        throw(std::string("GA::set_tsp_specs: TSP file doesn't exist. Program aborted."));
}

// LTM = Lower Triangular Matrix
void GA::set_tsp_LTM_data_option(std::ifstream& in)
{
    if(in.good())
    {
        std::string temp;
        while(temp != "EDGE_WEIGHT_SECTION")
            getline(in, temp);

        int row = 0;
        int column = 0;
        while(temp != "EOF")
        {

            in >> temp;

            if(temp == "0")
            {
                row++;
                column = 0;
            }
            else if(temp != "EOF")
            {
                options.tsp_data[row][column++] = std::strtod(temp.c_str(), NULL);
            }
        }

        for(int i = 0; i < options.chromosome_length; i++)
        {
            for(int j = 0; j < options.chromosome_length; j++)
            {
                if(i == j)
                    options.tsp_data[i][j] = 0;

                if(options.tsp_data[i][j] == -1)
                    options.tsp_data[i][j] = options.tsp_data[j][i];
            }
        }
    }
    else
        throw(std::string("GA::set_tsp_LTM_data: TSP file doesn't exist. Program aborted."));
}

void GA::set_tsp_EUC2D_data_option(std::ifstream& in)
{
    std::string temp;
    while(temp != "NODE_COORD_SECTION")
        getline(in,temp);
    
    int tour_data_count = 0;
    while(temp != "EOF")
    {
        getline(in, temp);
        std::stringstream ss;
        std::string temp_1 = "";
        std::string temp_2 = "";
        std::string temp_3= "";
        ss << temp;

        ss >> temp_1;
        ss >> temp_2;
        ss >> temp_3;

        if(temp_1 != "EOF")
        {
            options.tsp_data[tour_data_count][0] = std::strtod(temp_1.c_str(), NULL);
            options.tsp_data[tour_data_count][1] = std::strtod(temp_2.c_str(), NULL);
            options.tsp_data[tour_data_count++][2] = std::strtod(temp_3.c_str(), NULL);
        }
    }
}

void GA::set_tsp_xml_data_option(std::ifstream& in)
{

    if(in.good())
    {
        std::string temp;
        int row = 0;
        while(in.good())
        {
            while(temp != "    <vertex>" && in.good())
                getline(in, temp);

            while(temp != "    </vertex>" && in.good())
            {
                getline(in, temp);
                int i = 0;
                while(temp.c_str()[i++] != '\"');

                std::string edge_weight_str;
                while(temp.c_str()[i] != '\"')
                    edge_weight_str.push_back(temp.c_str()[i++]);
                while(temp.c_str()[i++] != '>');
                std::string vertex_str;
                while(temp.c_str()[i] != '<')
                    vertex_str.push_back(temp.c_str()[i++]);
                double edge_weight = std::strtod(edge_weight_str.c_str(), NULL);
                int vertex = std::stoi(vertex_str);
                options.tsp_data[row][vertex] = (int)edge_weight;
            }
            row++;
        }
    }
    else
        throw(std::string("GA::set_tsp_xml_data: TSP file doesn't exist. Program aborted."));
}

void GA::write_EUC2Dcoords_to_file(std::string tsp_tour_name)
{
    std::ofstream out(tsp_tour_name + "_coords.txt");

    for(int i = 0; i < options.chromosome_length; i++)
        out << options.tsp_data[i][0] << " " << options.tsp_data[i][1] << " " << options.tsp_data[i][2] << std::endl;

    out.close();
}