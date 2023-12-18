//TESTS IN REPORT(), STATS()
// UNIT TESTS IN GENERATION(), XOVER_MUTATE(), CHC_GENERATION()
#include "population.h"
#include "test.h"

Population::Population()
{
    for(int i = 0; i < options->population_size; i++)
    {
        child_transform_data[i][0] = 2;
        child_transform_data[i][1] = 0;
    }
    
    for(int i = 0; i < options->population_size; i++)
    {
        transform_data[i][0] = 2;
        transform_data[i][1] = 1;
    }
 
    average = min = max = sum_fitness = convergence = -1;
    for(int i = 0; i < options->population_size; i++)
    {
        member_chosen_count[i] = 0;
    }

    for(int i = 0; i < 1000; i++)
    {
        selection_count[i][0] = -1;
        selection_count[i][1] = -1;
    }
}

//FOR PROGRAMS REQUIRING ONLY A SINGLE INITIAL POPULATION
Population::Population(Options& opt)
{
    options = &opt;
    
    for(int i = 0; i < options->population_size; i++)
    {
        child_transform_data[i][0] = 2;
        child_transform_data[i][1] = 0;
    }
    
    for(int i = 0; i < options->population_size; i++)
    {
        transform_data[i][0] = 2;
        transform_data[i][1] = 1;
    }
 
    average = min = max = sum_fitness = convergence = -1;
    members = new Individual[this->options->population_size];
    for(int i = 0; i < this->options->population_size; i++)
    {
        members[i].set_chromosome_length(this->options->chromosome_length);
        members[i].init_TSP(options->random_seed, i);
        members[i].set_id(i);
    }
}

//FOR PROGRAMS REQUIRING MULTIPLE INITIAL POPULATIONS
Population::Population(Options& opt, long srand_offset)
{
    options = &opt;

    for(int i = 0; i < options->population_size; i++)
    {
        child_transform_data[i][0] = 2;
        child_transform_data[i][1] = 0;
    }
    
    for(int i = 0; i < options->population_size; i++)
    {
        transform_data[i][0] = 2;
        transform_data[i][1] = 1;
    }
 
    average = min = max = sum_fitness = convergence = -1;
    members = new Individual[this->options->population_size];
    for(int i = 0; i < this->options->population_size; i++)
    {
        members[i].set_chromosome_length(this->options->chromosome_length);
        members[i].init_TSP(options->random_seed, srand_offset*options->population_size + i);
        members[i].set_id(i);
    }
}

Population::Population(Options& opt, std::ifstream& in)
{
    // TEST
    // print("POPULATION::POPULATION(ifstream)");
    
    options = &opt;

    members = new Individual[options->population_size];

    std::string temp;
    for(int i = 0; i < options->population_size; i++)
    {
        members[i].set_chromosome_length(options->chromosome_length);
        members[i].set_id(i);
        for(int j = 0; j < options->chromosome_length; j++)
        {
            in >> temp;
            members[i].get_chromosome()[j] = std::stoi(temp);
        }
        in >> temp;
        members[i].set_fitness(std::strtod(temp.c_str(), NULL));
        in >> temp;
        members[i].set_objective_value(std::strtod(temp.c_str(), NULL));
        
        // TEST
        // print("temp = " + temp);
        // print("members[",i,"] = ",members[i].get_fitness());

        // TEST
        // std::cout << "members["<< i <<"]: ";
        // members[i].print_ind();
        // cin();
    } 
}

Population::Population(Population& rhs)
{
    options = rhs.options;

    members = new Individual[options->population_size];

    for(int i = 0; i < options->population_size; i++)
    {
        members[i].copy_individual_data(rhs.get_members()[i]);
    }
}

Population::~Population()
{
    delete[] members;
}

Eval_results& Population::get_results()
{
    return m_results;
}

void Population::init_transform_data(int row)
{
    transform_data[row][0] = options->m_meta_length;
    transform_data[row][1] = 1;
    transform_data[row][2] = row;
    transform_data[row][3] = row;
    transform_data[row][4] = -1;
}

void Population::set_options(Options& opt)
{
    options = &opt;
}

//FOR PROGRAMS REQUIRING ONLY A SINGLE INITIAL POPULATION
void Population::set_members()
{
    members = new Individual[this->options->population_size];
    for(int i = 0; i < this->options->population_size; i++)
    {
        members[i].set_chromosome_length(this->options->chromosome_length);
        members[i].init(options->random_seed, i);
    }
}

//FOR PROGRAMS REQUIRING MULTIPLE INITIAL POPULATIONS
void Population::set_members(long srand_offset)
{
    members = new Individual[this->options->population_size];
    for(int i = 0; i < this->options->population_size; i++)
    {
        members[i].set_chromosome_length(this->options->chromosome_length);
        members[i].init(options->random_seed, srand_offset*options->population_size + i);
    }
}

void Population::set_members_ptr(Individual* mem)
{
    members = mem;
}

void Population::copy_members_and_update_id(const Population &copy)
{
    for(int i = 0; i < options->population_size; i++)
    {
        members[i] = copy.members[i];
        members[i].set_id(i);
    }
}

void Population::copy_population(const Population &copy)
{
    copy_members_and_update_id(copy);

    for(int i = 0; i < options->population_size; i++)
    {
        for(int j = 0; j < copy.transform_data[i][0]; j++)
        {
            transform_data[i][j] = copy.transform_data[i][j];
        }
    }

    for(int i = 0; i < options->population_size; i++)
    {
        for(int j = 0; j < copy.child_transform_data[i][0]; j++)
        {
            child_transform_data[i][j] = copy.child_transform_data[i][j];
        }
    }

}

void Population::set_transform_data_by_row(int* data, int row)
{
    for(int i = 0; i < data[0]; i++)
    {
        transform_data[row][i] = data[i];
    }
}

void Population::set_child_transform_data_by_row(int* data, int row)
{
    for(int i = 0; i < data[0]; i++)
    {
        child_transform_data[row][i] = data[i];
    }
}

void Population::reset_super_individual_count()
{
    super_individuals = 0;
    semi_super_individuals = 0;
}

int Population::find_max_fitness_member()
{
    double temp_fitness = -1;
    double temp_max = 0;
    int temp_max_member_index = 0;

    for(int i = 0; i < options->population_size; i++)
    {
        temp_fitness = members[i].get_fitness();

        if(temp_fitness > temp_max)
        {
            temp_max = temp_fitness;
            temp_max_member_index = i;
        }
    }

    return temp_max_member_index;
}

int Population::find_min_fitness_member()
{
    double temp_fitness = -1;
    double temp_min = std::numeric_limits<double>::max();
    int temp_min_member_index = 0;

    for(int i = 0; i < options->population_size; i++)
    {
        temp_fitness = members[i].get_fitness();

        if(temp_fitness < temp_min)
        {
            temp_min = temp_fitness;
            temp_min_member_index = i;
        }
    }

    return temp_min_member_index;
}

Individual* Population::get_members()
{
    return members;
}

int* Population::get_transform_data(int row)
{
    return transform_data[row];
}

int* Population::get_child_transform_data(int row)
{
    return child_transform_data[row];
}

double Population::get_min()
{
    return min;
}

double Population::get_max()
{
    return max;
}

double Population::get_average()
{
    return average;
}

double Population::get_sum_fitness()
{
    return sum_fitness;
}

double Population::get_convergence()
{
    return convergence;
}

void Population::get_member_chosen_stats()
{
    double total = 0;
    for(int i = 0; i < options->population_size; i++)
    {
        total += member_chosen_count[i];
    }

    std::cout << "total = " << total << std::endl;

    for(int i = 0; i < options->population_size; i++)
    {
        std::cout << "Member[" << i << "] fitness-frac = " << proportional_fitnesses[i] << std::endl;
        std::cout << "Member[" << i << "] occurence rate = " << member_chosen_count[i]/total << std::endl;
    }
}

Options*& Population::get_options()
{
    return*& options;
}

int* Population::get_pmx_indices()
{
    return pmx_indices;
}

void Population::evaluate(int choice, double** tsp_data, int random_seed, long srand_offset, int eval_option)
{
    for(int i = 0; i < options->population_size; i++)
    {
        try
        {
            eval(members[i], m_results, tsp_data, choice, random_seed, srand_offset + i, eval_option);
        }
        catch(std::string error_message)
            {throw error_message;}
        
        members[i].set_fitness(m_results.fitness);
        members[i].set_objective_value(m_results.objective);
    }
}

void Population::evaluate_single(Individual& member, int choice, double** tsp_data, int random_seed, long srand_offset, int eval_option)
{
    for(int i = 0; i < options->population_size; i++)
    {
        try
        {
            eval(member, m_results, tsp_data, choice, random_seed, srand_offset + i, eval_option);
        }
        catch(std::string error_message)
            {throw error_message;}
        
        member.set_fitness(m_results.fitness);
        member.set_objective_value(m_results.objective);
    }
}

void Population::stats(int& total_super_individuals, int& total_semi_super_individuals)
{
    // FITNESS STATS
    sum_fitness =  0;
    min = max = members[0].get_fitness();

    max_fitness_member_index = 0;
    double fitness = -1;
    for(int i = 0; i < options->population_size; i++)
    {
        fitness = members[i].get_fitness();

        sum_fitness += fitness;

        if(fitness < min)
        {
            min = fitness;
        }
        if(fitness > max)
        {
            max = fitness;
            max_fitness_member_index = i;
        }
    }
    average = sum_fitness/options->population_size;
    // END FITNESS STATS

    // OBJECTIVE STATS
    sum_objective = 0;
    min_objective = max_objective = members[0].get_objective_value();
    
    double objective_value = -1;
    for(int i = 0; i < options->population_size; i++)
    {
        objective_value = members[i].get_objective_value();

        sum_objective += objective_value;

        if(objective_value < min_objective)
        {
            min_objective = objective_value;
        }
        if(objective_value > max_objective)
        {
            max_objective = objective_value;
        }
    }
    average_objective = sum_objective/options->population_size;
    // END OBJECTIVE STATS

    //MAYBE THE NUMBER OF ITERATIONS IN THIS SEARCH FOR SUPER INDIVIDUALS COULD BE REDUCED HEURISTICALLY 
    convergence = 0;
    for(int i = 0; i < options->population_size; i++)
    {
        double scale = members[i].get_fitness()/average;

        if(scale >= options->semi_super_individual_threshold && scale < options->super_individual_threshold)
            semi_super_individuals++;
        else if(scale >= options->super_individual_threshold)
            super_individuals++;

        if(convergence < scale)
        {
            convergence = scale;
        }
    }

    total_super_individuals += super_individuals;
    total_semi_super_individuals += semi_super_individuals;
}

//SET OPTION = 1 IF AVERAGING IS NOT REQUIRED
void Population::report(int generation, int option, int total_super_individuals, int total_semi_super_individuals, bool extinction_event)
{
    if(generation == (options->max_generations - 1))
    {
        for(int i = 0; i < selection_count_length; i++)
            selection_count_total += selection_count[i][1];
    }

    std::ofstream out(options->output_file, std::ios::app);
    out << std::scientific << std::setprecision(5) << add_whitespace(generation, options->max_generations, true)
            << generation << ",\t\t" << min << ",\t\t" << average << ",\t\t" << max
                << ",\t\t" << (extinction_event ? 1 : 0)
                    << ",\t\t\t\t" << std::fixed <<  std::setprecision(5) << convergence << ",\t\t" << semi_super_individuals << ",\t\t\t" << super_individuals << ",\t\t\t" << total_semi_super_individuals << ",\t\t\t" << total_super_individuals
                        << ",\t\t\t\t\t";
                        for(int i = 0; i < options->chromosome_length; i++)
                        {
                            out << members[max_fitness_member_index].get_chromosome()[i] << ", ";
                        }
                        out << std::endl;

    if(generation == (options->max_generations - 1))
    {
        out << std::endl;
        out << "MUT_RATE, " << mutation_count/mutation_total_possible << ",\n";
        out << "SLXN_TOTAL, " << selection_count_total << ",\n";
        out << "SLXN_RANKS, SLXN_COUNT,\tSLXN_RATE" << std::endl;
        for(int i = 0; i < selection_count_length; i++)
        {
            out << string_precision(selection_count[i][0], 2) << ",\t\t\t\t\t" << leading_whitespace(selection_count[i][1], selection_count[i][1], 3, 0) 
                << ",\t\t\t\t\t\t" << string_precision(selection_count[i][1]/selection_count_total*100, 2) << std::endl;
        }
        out << std::endl;
    }
    

    out.close();

    std::ofstream out_2(options->output_file_o, std::ios::app);
    out_2 << std::fixed << std::setprecision(options->print_precision_o) << add_whitespace(generation, options->max_generations, true)
            << generation << ",\t\t" << (int) min_objective << ",\t\t" << (int) average_objective << ",\t\t" << (int) max_objective
                << ",\t\t" << (extinction_event ? 1 : 0)
                    << ",\t\t\t\t" << std::setprecision(5) << convergence << ",\t\t" << semi_super_individuals << ",\t\t\t" << super_individuals << ",\t\t\t" << total_semi_super_individuals << ",\t\t\t" << total_super_individuals
                        << ",\t\t\t\t\t";
                        for(int i = 0; i < options->chromosome_length; i++)
                        {
                            out_2 << members[max_fitness_member_index].get_chromosome()[i] << ", ";
                        }
                        out_2 << std::endl;

    if(generation == (options->max_generations - 1))
    {
        out_2 << std::endl;
        out_2 << "MUT_RATE, " << mutation_count/mutation_total_possible << ",\n";
        out_2 << "SLXN_TOTAL, " << selection_count_total << ",\n";
        out_2 << "SLXN_RANKS,\t\tSLXN_COUNT,\t\t\tSLXN_RATE" << std::endl;
        for(int i = 0; i < selection_count_length; i++)
        {
            out_2 << string_precision(selection_count[i][0], 2) << ",\t\t\t\t\t" << leading_whitespace(selection_count[i][1], selection_count[i][1], 3, 0) 
                << ",\t\t\t\t\t\t" << string_precision(selection_count[i][1]/selection_count_total*100, 2) << std::endl;
        }
        out_2 << std::endl;
    }
    out_2.close();
}

void Population::report_single()
{
    std::ofstream out("stats.csv");
    out << options->GA_iteration << ",\t\t" << (int)min_objective << ",\t\t" << (int)average_objective << ",\t\t";
    for(int i = 0; i < options->chromosome_length; i++)
    {
        out << members[max_fitness_member_index].get_chromosome()[i] << " ";
    }
    out << std::endl;
}

void Population::generation(Population*& child, long srand_offset)
{
    int parent_index_1 = 0;
    int parent_index_2 = 0;
    int child_index_1 = 0;
    int child_index_2 = 0;

    Individual *parent_1, *parent_2, *child_1, *child_2;
;
    for(int i = 0; i < options->population_size; i += 2)
    {   
        // PROPORTIONAL SELECTION
        // parent_index_1 = proportional_selection(srand_offset*options->population_size + i);
 
        // RANK SELECTION
        rank_selection(srand_offset*options->population_size + i, parent_index_1);
 
        // THE FOLLOWING LOOP STOPS ASEXUAL SELECTION
        // ADDED THE k OFFSET FOR SRAND() BECAUSE OCCASIONALLY GETTING HELD UP IN THIS LOOP, PROBABLY DUE TO TIME(NULL) SEED
        int k = 0;

        rank_selection(srand_offset*options->population_size + i + 1 + k, parent_index_2);
        
        while(parent_index_2 == parent_index_1)
        {
            k++;

            // PROPORTIONAL SELECTION
            // parent_index_2 = proportional_selection(srand_offset*options->population_size + i + 1 + k);
        
            // RANK SELECTION
            rank_selection(srand_offset*options->population_size + i + 1 + k, parent_index_2);
        }

        child_index_1 = i;
        child_index_2 = i+1;

        parent_1 = &members[parent_index_1];
        parent_2 = &members[parent_index_2];
        
        child_1 = &(child->members[child_index_1]);
        child_2 = &(child->members[child_index_2]);

        xover_mutate(parent_1, parent_2, child_1, child_2, srand_offset*options->population_size + i);

        parent_1 = nullptr;
        parent_2 = nullptr;
        child_1 = nullptr;
        child_2 = nullptr;
    }
}

void Population::genitor(long srand_offset, int eval_option)
{
    int parent_index_1 = 0;
    int parent_index_2 = 0;

    Individual *parent_1, *parent_2;
    Individual child(options->chromosome_length);

    rank_selection_prep(srand_offset);
    rank_selection(srand_offset + m_srand_offset_count++, parent_index_1);

    // THE FOLLOWING LOOP STOPS ASEXUAL SELECTION
    // ADDED THE m_srand_offset_count OFFSET FOR SRAND() BECAUSE OCCASIONALLY GETTING HELD UP IN THIS LOOP, PROBABLY DUE TO TIME(NULL) SEED
    parent_index_2 = parent_index_1;
    while(parent_index_2 == parent_index_1) // COULD ALLOW SAME PARENT MATING WHEN USING TSP-ER XOVER - SOMETHING TO CONSIDER... (112823)
        rank_selection(srand_offset + (parent_index_1 + 1) + m_srand_offset_count++, parent_index_2); // ADDING (parent_index_1 + 1) TO SRAND_OFFSET INTRODUCES ADDITIONAL RANDOMNESS

    // COLLECT SELECTION COUNTS FOR SELECTION STATS
    rank_selection_stats(parent_index_1, parent_index_2, 2);

    parent_1 = &members[parent_index_1];
    parent_2 = &members[parent_index_2];

    // // TEST
    // print("POP::GENITOR: Above edge_recombination; child:");
    // for(int i = 0; i < options->chromosome_length; i++)
    //     std::cout << child.get_chromosome()[i] << " ";
    // endl();
    
    edge_recombination(parent_1, parent_2, child, srand_offset + (parent_index_2 + 1) + m_srand_offset_count++);

    // MAY WANT TO CHANGE THIS SO THAT ENTIRE POPULATION IS GIVEN CHANCE TO MUTATE, NOT JUST CHILD
    // ^ FOLLOWING UP (112823) WILL TRY FULL POPULATION MUTATION
    // FULL POPULATION MUTATION MAY NOT BE CORRECT, ACCORDING TO SUSHIL - RETURNING TO SINGLE CHILD MUTATION
    bool flip_check = flip(options->mutation_rate, options->random_seed, srand_offset + m_srand_offset_count++);
    if(flip_check)
    {
        scramble_mutation(child, srand_offset + m_srand_offset_count++);
        mutation_count++;
    }

    // UNIT TEST
    // if(duplicate_values_in_string(child, options->chromosome_length))
    //     throw(std::string("POPULATION::GENITOR: Duplicates in child string"));
 
    // REAL
    TSP(child, options->tsp_data, options->tsp_edge_weight_format, m_results, eval_option);
    child.set_fitness(m_results.fitness);
    child.set_objective_value(m_results.objective);
    
    int index_min_fit_member = find_min_fitness_member();

    bool check =  child.get_fitness() > members[index_min_fit_member].get_fitness();
    if(check)
    {
        members[index_min_fit_member].set_chromosome(options->chromosome_length, child.get_chromosome());
        members[index_min_fit_member].set_fitness(child.get_fitness());
        members[index_min_fit_member].set_objective_value(child.get_objective_value());
    }

    parent_1 = nullptr;
    parent_2 = nullptr;

    // TEST
    // print("POP::genitor: Bottom");
}

int Population::proportional_selection(long srand_offset)
{
    srand(options->random_seed + srand_offset);
    double random_fraction = (double)rand()/RAND_MAX;
    double* limits = new double[options->population_size];
    limits[0] = members[0].get_fitness()/sum_fitness;

    for(int i = 1; i < options->population_size; i++)
        limits[i] = limits[i-1] + members[i].get_fitness()/sum_fitness;

    for(int i = 0; i < options->population_size; i++)
    {
        if(random_fraction <= limits[i])
        {
            delete[] limits;
            return i;
        }
    }

    delete[] limits;
    return -1;
}

void Population::xover_mutate(Individual* parent_1, Individual* parent_2, Individual* child_1, Individual* child_2, long srand_offset)
{
    for(int i = 0; i < options->chromosome_length; i++)
    {
        child_1->get_chromosome()[i] = parent_1->get_chromosome()[i];
        child_2->get_chromosome()[i] = parent_2->get_chromosome()[i];
    }
   
    int index = -1;
    if(flip(options->xover_rate, options->random_seed, srand_offset))
    {
        // ONE POINT XOVER
        // index = one_point_xover(parent_1, parent_2, child_1, child_2, srand_offset);

        // PMX
        pmx(parent_1, parent_2, child_1, child_2, srand_offset);
    }

    // SWAP MUTATE
    child_1->swap_mutate(options->mutation_rate, options->random_seed, srand_offset);
    child_2->swap_mutate(options->mutation_rate, options->random_seed, srand_offset + rand());
}

void Population::set_member_ids()
{
    if(member_ids != nullptr)
    {
        for(int i = 0; i < options->population_size; i++)
            delete[] member_ids[i];
        delete[] member_ids;
    }
    
    member_ids = new double*[options->population_size];
    for(int i = 0; i < options->population_size; i++)
    {
        member_ids[i] = new double[2];
        member_ids[i][0] = members[i].get_fitness();
        member_ids[i][1] = members[i].get_id();
    }
}

double** Population::get_member_ids()
{
    return member_ids;
}

void Population::sort_member_ids_by_fitness(int random_seed, long srand_offset)
{
    Partition<double**> sort;
    int pivot = sort.random_index_in_range(0, options->population_size, random_seed, srand_offset);
    sort.partition_sort_2D(member_ids, pivot, 0, options->population_size - 1);
}

void Population::rank_selection_prep(long srand_offset)
{
    set_member_ids();
    sort_member_ids_by_fitness(options->random_seed, srand_offset);
    convert_member_ids_fitness_to_rank();
    m_average_rank_before_scaling = average_rank();
    if(options->selection_pressure > 1)
        selection_pressure_scaling();
}

int Population::rank_selection(long srand_offset, int& member_id)
{
    sum_rank = 0;
    for(int i = 0; i < options->population_size; i++)
        sum_rank += member_ids[i][0];

    // TEST
    // print("\nsum_rank = ", sum_rank);

    srand(options->random_seed + srand_offset);
    double random_fraction = (double)rand()/RAND_MAX;

    // UNIT TEST: RANDOM STATS
    // rand_stats(random_fraction, 2);

    double** limits = new double*[options->population_size];
    
    for(int i = 0; i < options->population_size; i++)
        limits[i] = new double[options->population_size];

    limits[0][0] = member_ids[0][0]/sum_rank;
    limits[0][1] = member_ids[0][1];

    // TEST
    // print("random_fraction = ", random_fraction);
    // print("member_ids[0][0] = ", member_ids[0][0]);
    // print("average before scaling = ", m_average_rank_before_scaling);
    // print("limits[0] = ", limits[0], 5);

    for(int i = 1; i < options->population_size; i++)
    {
        limits[i][0] = limits[i-1][0] + member_ids[i][0]/sum_rank;
        limits[i][1] = member_ids[i][1];
        
        // TEST
        // print("\nmember_ids[",i,"][0]", member_ids[i][0]);
        // print("average before scaling = ", m_average_rank_before_scaling);
        // print("limits[",i,"]", limits[i]);
    }

    for(int i = 0; i < options->population_size; i++)
    {
        if(random_fraction <= limits[i][0])
        {
            // print("\nreturn index = ", i);
            member_id = limits[i][1];
            for(int i = 0; i < options->population_size; i++)
                delete[] limits[i];
            delete[] limits;
            return 1;
        }
    }

    for(int i = 0; i < options->population_size; i++)
        delete[] limits[i];
    delete[] limits;
    member_id = -INT32_MAX;
    return 0;
}

void Population::convert_member_ids_fitness_to_rank()
{
    int rank = 1;
    double previous_fitness = -1;
    for(int i = 0; i < options->population_size; i++)
    {
        if(i == 0)
        {
            previous_fitness = member_ids[i][0];
            member_ids[i][0] = rank;
        }
        else
        {
            if(member_ids[i][0] == previous_fitness)
                member_ids[i][0] = rank;
            else
            {
                previous_fitness = member_ids[i][0];
                member_ids[i][0] = ++rank;
            }
        }
    }
}

void Population::selection_pressure_scaling()
{
    double max = member_ids[options->population_size - 1][0];
    double ave_rank = average_rank();

    // SCALES ALL RANKS ABOVE THE AVERAGE DOWN, SO THAT THE MAX RANK IS THE MAX SCALER MULTIPLE (FACTOR_STEP*MAX_STEPS [SEE BELOW]) OF THE OPTIONS.SELECTION_PRESURE CONSTANT
    // RANKS IN BETWEEN THE AVERAGE AND MAX RANK ARE A SCALER MULTIPLE OF THEIR RANK RELATIVE TO THE AVERAGE RANK (FACTOR_STEP*RELATIVE_RANK(RELATIVE_RANK = CURRENT_STEP) [SEE BELOW]) OF THE OPTIONS.SELECTION_PRESURE CONSTANT
    double factor = options->selection_pressure - 1;
    double rank_steps = max - ave_rank;
    double factor_step = factor/rank_steps;
    double current_step = 0;
    double scaler;
    double previous_rank_before_scaling = 0;
    for(int i = 0; i < options->population_size; i++)
    {
        if(member_ids[i][0] > ave_rank)
        {
            if(previous_rank_before_scaling < member_ids[i][0])
                current_step++;
         
            previous_rank_before_scaling = member_ids[i][0];
            scaler = ave_rank*(1 + factor_step*current_step)/member_ids[i][0];
            member_ids[i][0] *= scaler;
        }
    }

}

int Population::average_rank()
{
    double ave_rank = 0;
    for(int i = 0; i < options->population_size; i++)
        ave_rank += member_ids[i][0];
    ave_rank /= options->population_size;
    ave_rank = nearbyint(ave_rank);
    return ave_rank;
}

void Population::edge_recombination(Individual* parent_1, Individual* parent_2, Individual& child, long srand_offset)
{
    // TEST
    // log(*options->log_stream, "POP::EDGE_RECOBINATION: Top");
    // print("POP::EDGE_RECOBINATION: Top");

    int** edge_map = new int*[options->chromosome_length];
    for(int i = 0; i < options->chromosome_length; i++)
        edge_map[i] = new int[5];
    
    // LOG: PARENT CHROMSOMES
    // log(*options->log_stream, "\nParent 1:");
    // for(int i = 0; i < options->chromosome_length; i++)
    //     log(*options->log_stream, parent_1->get_chromosome()[i], " ");
    // log_endl(*options->log_stream);
    // log(*options->log_stream, "Parent 2:");
    // for(int i = 0; i < options->chromosome_length; i++)
    //     log(*options->log_stream, parent_2->get_chromosome()[i], " ");
    // log_endl(*options->log_stream);
    // log_endl(*options->log_stream);

    build_edge_map(parent_1, parent_2, edge_map);
    
    // TEST
    // print("POP::EDGE_RECOBINATION: Below build_edge_map");
    
    int cities_appended_to_child = 0;
    int current_city = -1;
    int previous_city = -1;
    int num_least_edge_cities = 0;
    int min_edge_count = INT32_MAX;
    bool single_edge = false;
    bool no_other_cities_have_edges = false;
    int repeat_current_city = -1;
    bool edge_map_empty = false;
    bool first_city_chosen_random = false;

    // TEST
    bool test_bool = false;

    while(cities_appended_to_child != options->chromosome_length)
    {
        num_least_edge_cities = 0;
        min_edge_count = INT32_MAX;
        int least_edge_cities[100];
        int random_cities[1000];
        int num_random_cities = 0;
        bool random_city_chosen = false;
        single_edge = false;

        // LOG
        // LOGGING EDGE MAP //////////////////////////////////////////////////
        // log(*options->log_stream, "EDGE MAP:");
        // for(int i = 0; i < options->chromosome_length; i++)
        // {
        //     int edge_cnt = edge_map[i][0];
        //     for(int j = 0; j < edge_cnt + 1; j++)
        //     {
        //         if(j == 0)
        //             log(*options->log_stream, i+1, " ", edge_map[i][j], " ");
        //         else
        //             log(*options->log_stream, edge_map[i][j], " ");
        //     }
        //     log_endl(*options->log_stream);
        // }
        // log_endl(*options->log_stream);
        // END EDGE_MAP LOG //////////////////////////////////////////////////
        // log(*options->log_stream, "CHILD:");
        // for(int i = 0; i < cities_appended_to_child; i++)
        //     log(*options->log_stream, child.get_chromosome()[i], " ");
        // log_endl(*options->log_stream);
        // log_endl(*options->log_stream);

        if(current_city != -1 && edge_map[current_city - 1][0] == 1)
        {
            current_city = edge_map[current_city - 1][1];
            previous_city = child.get_chromosome()[cities_appended_to_child - 1];
            single_edge = true;
            
            // LOG
            // log(*options->log_stream, "Current city only has one edge remaining");
            // log(*options->log_stream, "Next current city = ", current_city);
            // log(*options->log_stream, "previous city = ", previous_city);
            // log_endl(*options->log_stream);
        }
        else
        {
            int zero_edge_count = 0;
            for(int i = 0; i < options->chromosome_length; i++)
            {
                // PREVIOUS IMPLEMENTATION FOR (cities_appended_to_child == 0 || edge_map[current_city - 1][0] == 0) CONDITION TO CHOOSE NEXT CITY WITH MIN EDGES
                // NOW ONLY IF (cities_appended_to_child == 0), AND BELOW WILL RANDOMLY CHOOSE CITY FOR RANDOMIZE IF (edge_map[current_city - 1][0] == 0) CONDITION.(CB231214)
                // if(cities_appended_to_child == 0 || edge_map[current_city - 1][0] == 0)
                if(cities_appended_to_child == 0)
                {
                    // LOG
                    // log(*options->log_stream, i + 1, " (cities_appended_to_child == 0)", true);
                    // log_endl(*options->log_stream);

                    // if(edge_map[i][0] < min_edge_count && edge_map[i][0] != 0) // ORIGINAL, SEE NOTE ABOVE.(CB231214)
                    // if(edge_map[i][0] < min_edge_count) // SECONDARY ORIGINAL. NOW IMPLEMENTING FIRST PICK RANDOM.(CB231214)
                    //     min_edge_count = edge_map[i][0];

                    random_city_chosen = true;

                }
                // else if(edge_check(edge_map, current_city, i + 1) && edge_map[i][0] < min_edge_count) // ORIGINAL, SEE NOTE ABOVE.(CB231214)
                else if(edge_check(edge_map, current_city, i + 1) && edge_map[i][0] < min_edge_count)
                {
                
                    // LOG
                    // log(*options->log_stream, "finding min_edge_count");
                    // log(*options->log_stream, "current city = ", current_city);
                    // log(*options->log_stream, "current edge = ", (i + 1));
                    // log_endl(*options->log_stream);

                    min_edge_count = edge_map[i][0];
                }

                if(edge_map[i][0] == 0)
                    zero_edge_count++;

                if(zero_edge_count == (options->chromosome_length - 1) && !no_other_cities_have_edges)
                {
                    no_other_cities_have_edges = true;
                    repeat_current_city = current_city;
                }

                if(zero_edge_count == options->chromosome_length)
                    edge_map_empty = true;

                if(current_city != -1 && edge_map[current_city - 1][0] == 0 && edge_map[i][0] != 0)
                {
                    random_cities[num_random_cities++] = i + 1;
                    if(!random_city_chosen)
                        random_city_chosen = true;
                }

            }
           
            // LOG
            // log(*options->log_stream, "min_edge_count = ", min_edge_count);
            // log(*options->log_stream, "no_other_cities_have_edges = ", no_other_cities_have_edges);
            // log(*options->log_stream, "edge_map_empty = ", edge_map_empty);
            // log(*options->log_stream, "random_city_chosen = ", random_city_chosen);
            // log_endl(*options->log_stream);
            
            if(!no_other_cities_have_edges && !edge_map_empty && !random_city_chosen)
            {
                for(int i = 0; i < options->chromosome_length; i++)
                {
                    // DUE TO RANDOMIZING FOR (edge_map[current_city - 1][0] == 0) CONDITION.(CB231214)
                    // if(cities_appended_to_child == 0 || edge_map[current_city - 1][0] == 0) // ORIGINAL.(CB231214)
                    if(cities_appended_to_child == 0) // THIS WILL NEED TO BE REMOVED IF RANDOMIZING THE FIRST CITY.(CB231214)
                    {
                        if(edge_map[i][0] == min_edge_count)
                            least_edge_cities[num_least_edge_cities++] = (i + 1);
                    }
                    else if(edge_check(edge_map, current_city, i + 1) && edge_map[i][0] == min_edge_count)
                    {
                        // LOG
                        // log(*options->log_stream, "adding edge to least_edge_cities");
                        // log(*options->log_stream, "current city = ", current_city);
                        // log(*options->log_stream, "current edge = ", (i + 1));
                        // log_endl(*options->log_stream);

                        least_edge_cities[num_least_edge_cities++] = (i + 1);
                    }
                }
            }

            // LOG
            // log(*options->log_stream, "num_least_edge_cities = ", num_least_edge_cities);
            // log_endl(*options->log_stream);
        }

        // if(num_least_edge_cities == 0 && !single_edge && !no_other_cities_have_edges && !edge_map_empty) // ORIGINAL, SEE COMMENT ABOVE OF SAME DATE ON RANDOMIZING.(CB231214)
        if(num_least_edge_cities == 0 && !single_edge && !no_other_cities_have_edges && !edge_map_empty && !random_city_chosen) // THIS IS A FIX. SHOULD BE COMMENTED OUT, PROGRAM RAN AND LOGGED, AND ULTIMATELY REFACTORED
        {
            // LOG
            // LOGGING EDGE MAP ////////////////////////////////////////////////
            // log(*options->log_stream, "if(num_least_edge_cities == 0)");
            // log(*options->log_stream, "Edge Map:");
            // for(int i = 0; i < options->chromosome_length; i++)
            // {
            //     int edge_cnt = edge_map[i][0];
            //     for(int j = 0; j < edge_cnt + 1; j++)
            //     {
            //         if(j == 0)
            //             log(*options->log_stream, i+1, " ", edge_map[i][j], " ");
            //         else
            //             log(*options->log_stream, edge_map[i][j], " ");
            //     }
            //     log_endl(*options->log_stream);
            // }
            // log_endl(*options->log_stream);
            // END EDGE_MAP LOG //////////////////////////////////////////////////

            for(int i = 0; i < options->chromosome_length; i++)
            {
                if(edge_map[i][0] < min_edge_count && edge_map[i][0] != 0)
                    min_edge_count = edge_map[i][0];
            }
            for(int i = 0; i < options->chromosome_length; i++)
            {
                if(edge_map[i][0] == min_edge_count && edge_map[i][0] != 0)
                    least_edge_cities[num_least_edge_cities++] = (i + 1);
            }

            // LOG
            // log(*options->log_stream, "min_edge_count = ", min_edge_count);
            // log(*options->log_stream, "num_least_edge_cities = ", num_least_edge_cities);
            // log_endl(*options->log_stream);
        }
        if(!single_edge)
        {
            int index = -1;
            
            // if(!no_other_cities_have_edges && !edge_map_empty) // ORIGINAL, SEE COMMENT ABOVE OF SAME DATE ON RANDOMIZING.(CB231214)
            if(!no_other_cities_have_edges && !edge_map_empty && !random_city_chosen)
            {
                index = random_index_in_range(0, num_least_edge_cities, options->random_seed, srand_offset + cities_appended_to_child);
                current_city = least_edge_cities[index];

                // LOG
                // log(*options->log_stream, "not-single edge");
                // log(*options->log_stream, "pick from least edge cities:");
                // log(*options->log_stream, "index= ", index);
                // log(*options->log_stream, "current city = ", current_city);
                // log_endl(*options->log_stream);
            }
            // else if(!edge_map_empty) // ORIGINAL, SEE COMMENT ABOVE OF SAME DATE ON RANDOMIZING.(CB231214)
            else if(!edge_map_empty && !random_city_chosen)
            {
                int num_edges = edge_map[current_city - 1][0];
                index = random_index_in_range(0, num_edges, options->random_seed, srand_offset + cities_appended_to_child);
                current_city = edge_map[current_city - 1][index + 1];
 
                // LOG
                // log(*options->log_stream, "not-single edge");
                // log(*options->log_stream, "no other cities have edges:");
                // log(*options->log_stream, "index + 1 = ", index + 1);
                // log(*options->log_stream, "current city = ", current_city);
                // log_endl(*options->log_stream);
            }
            // NEW ELSE-IF FOR RANDOMIZING WHEN CURRENT CITY HAS NO EDGES; SEE COMMENTS ABOVE OF SAME DATE.(CB231214)
            else if(!edge_map_empty)
            {
                if(first_city_chosen_random)
                {
                    index = random_index_in_range(0, num_random_cities, options->random_seed, srand_offset + cities_appended_to_child);
                    current_city = random_cities[index];
                    
                    // LOG
                    // log(*options->log_stream, "RANDOM CITY!!");
                    // log(*options->log_stream, "not-single edge");
                    // log(*options->log_stream, "random city:");
                    // log(*options->log_stream, "Random city choices:");
                    // for(int i = 0; i < num_random_cities; i++)
                    //     log(*options->log_stream, random_cities[i], " ");
                    // log_endl(*options->log_stream);
                    // log(*options->log_stream, "index + 1 = ", index + 1);
                    // log(*options->log_stream, "current city = ", current_city);
                    // log_endl(*options->log_stream);
                }
                else
                {
                    index = random_index_in_range(0, options->chromosome_length, options->random_seed, srand_offset + cities_appended_to_child);
                    current_city = index + 1;
                    first_city_chosen_random = true;
                    
                    // LOG
                    // log(*options->log_stream, "FIRST RANDOM CITY!!");
                    // log(*options->log_stream, "current city = ", current_city);
                    // log_endl(*options->log_stream);
                }

            }

        }
        if(edge_map_empty) // THIS IS A CLEANUP - SHOULD BE REFACTORED.(CB231214)
        {
            int final_choices[100];
            int final_choice_count = 0;
            bool exists = false;
            for(int i = 0; i < options->chromosome_length; i++)
            {
                exists = false;
                for(int j = 0; j < cities_appended_to_child; j++)
                {
                    if((i+1) == child.get_chromosome()[j])
                    {
                        exists = true;
                        break;
                    }
                }
                if(!exists)
                    final_choices[final_choice_count++] = (i+1);
            }

            int index = -1;
            while(cities_appended_to_child != options->chromosome_length)
            {
                index = random_index_in_range(0, final_choice_count, options->random_seed, srand_offset + cities_appended_to_child);
                child.get_chromosome()[cities_appended_to_child++] = final_choices[index]; // CONSIDER RANDOMLY INSERTING THESE RATHER THAN APPENDING.(CB231214)
                final_choice_count--;
                for(int i = index; i < final_choice_count; i++)
                {
                    final_choices[i] = final_choices[i+1];
                }
            }

            // LOG
            // log(*options->log_stream, "edge_map empty:");
            // log(*options->log_stream, "CHILD:");
            // for(int i = 0; i < cities_appended_to_child; i++)
            //     log(*options->log_stream, child.get_chromosome()[i], " ");
            // log_endl(*options->log_stream);
            // log(*options->log_stream, "cities appended = ", cities_appended_to_child);
            // log_endl(*options->log_stream);
        }
        
        if(!edge_map_empty)
        {
            // LOG
            // log(*options->log_stream, "current_city to append = ", current_city);
            // log_endl(*options->log_stream);
    
            child.get_chromosome()[cities_appended_to_child++] = current_city;
            if(cities_appended_to_child > 0)
                previous_city = child.get_chromosome()[cities_appended_to_child - 2];
    
            if(!no_other_cities_have_edges)
            {
                // LOG
                // log(*options->log_stream, "other cities still have edges");
                // log(*options->log_stream, "rebuild edge map");
                // log_endl(*options->log_stream);

                rebuild_edge_map(edge_map, current_city, previous_city);
            }
            else
            {
                rebuild_edge_map(edge_map, current_city, -1);
                current_city = repeat_current_city;

                // LOG
                // log(*options->log_stream, "no_other_cities_have_edges");
                // log(*options->log_stream, "rebuild edge map");
                // log(*options->log_stream, "current city = previous city = ", current_city);
                // log_endl(*options->log_stream);
            }
        }

        // LOG
        // else
        // {
        //     log(*options->log_stream, "POP::ER_XOVER: Edge Map Empty -> DIDN'T ENTER ANY OF THE OTHER STUFF");
        //     log_endl(*options->log_stream);
        // }
        // log(*options->log_stream, "cities appended = ", cities_appended_to_child);
        // log_endl(*options->log_stream);
    }

    // LOG
    // log(*options->log_stream, "POP::ER_XOVER: End");
    // log_endl(*options->log_stream);

    for(int i = 0; i < options->chromosome_length; i++)
        delete[] edge_map[i];
    delete[] edge_map;
    
    // TEST
    // print("END OF ER XOVER");
    // cin();
}

void Population::build_edge_map(Individual* parent_1, Individual* parent_2, int** edge_map)
{
    for(int i = 0; i < options->chromosome_length; i++)
    {
        for(int j = 0; j < 5; j++)
            edge_map[i][j] = -1;
    }
    
    int city_1;
    int city_2;
    int city_edge_11;
    int city_edge_12;
    int city_edge_21;
    int city_edge_22;
    int edge_count;

    for(int j = 0; j < options->chromosome_length; j++)
    {
        if(j == 0)
        {
            city_1 = parent_1->get_chromosome()[j];
            city_edge_11 = parent_1->get_chromosome()[options->chromosome_length - 1];
            city_edge_12 = parent_1->get_chromosome()[j + 1];

            city_2 = parent_2->get_chromosome()[j];
            city_edge_21 = parent_2->get_chromosome()[options->chromosome_length - 1];
            city_edge_22 = parent_2->get_chromosome()[j + 1];
        }
        else if(j == (options->chromosome_length - 1))
        {
            city_1 = parent_1->get_chromosome()[j];
            city_edge_11 = parent_1->get_chromosome()[j - 1];
            city_edge_12 = parent_1->get_chromosome()[0];

            city_2 = parent_2->get_chromosome()[j];
            city_edge_21 = parent_2->get_chromosome()[j - 1];
            city_edge_22 = parent_2->get_chromosome()[0];
        }
        else
        {
            city_1 = parent_1->get_chromosome()[j];
            city_edge_11 = parent_1->get_chromosome()[j - 1];
            city_edge_12 = parent_1->get_chromosome()[j + 1];

            city_2 = parent_2->get_chromosome()[j];
            city_edge_21 = parent_2->get_chromosome()[j - 1];
            city_edge_22 = parent_2->get_chromosome()[j + 1];
        }
        
        edge_count = edge_map[city_1 - 1][0];
        if(edge_count == -1)
        {
            edge_map[city_1 - 1][0] = 2;
            edge_map[city_1 - 1][1] = city_edge_11;
            edge_map[city_1 - 1][2] = city_edge_12;
        }
        else
        {
            bool city_edge_1 = false;
            bool city_edge_2 = false;
            for(int i = 0; i < edge_count; i++)
            {
                if(edge_map[city_1 - 1][i + 1] == city_edge_11)
                    city_edge_1 = true;

                if(edge_map[city_1 - 1][i + 1] == city_edge_12)
                    city_edge_2 = true;
            }
            if(!city_edge_1)
            {
                edge_map[city_1 - 1][edge_count + 1] = city_edge_11;
                edge_map[city_1 - 1][0] = ++edge_count;
            }
            if(!city_edge_2)
            {
                edge_map[city_1 - 1][edge_count + 1] = city_edge_12;
                edge_map[city_1 - 1][0] = ++edge_count;
            }
        }
        
        edge_count = edge_map[city_2 - 1][0];
        if(edge_count == -1)
        {
            edge_map[city_2 - 1][0] = 2;
            edge_map[city_2 - 1][1] = city_edge_21;
            edge_map[city_2 - 1][2] = city_edge_22;
        }
        else
        {
            bool city_edge_1 = false;
            bool city_edge_2 = false;
            for(int i = 0; i < edge_count; i++)
            {
                if(edge_map[city_2 - 1][i + 1] == city_edge_21)
                    city_edge_1 = true;

                if(edge_map[city_2 - 1][i + 1] == city_edge_22)
                    city_edge_2 = true;
            }
            if(!city_edge_1)
            {
                edge_map[city_2 - 1][edge_count + 1] = city_edge_21;
                edge_map[city_2 - 1][0] = ++edge_count;
            }
            if(!city_edge_2)
            {
                edge_map[city_2 - 1][edge_count + 1] = city_edge_22;
                edge_map[city_2 - 1][0] = ++edge_count;
            }
        }
    }
}

void Population::rebuild_edge_map(int** edge_map, int city_to_remove, int previous_city)
{
    for(int i = 0; i < options->chromosome_length; i++)
    {
        int edge_count = edge_map[i][0];

        for(int j = 1; j < (edge_count + 1); j++)
        {
            if(i == (previous_city - 1))
            {
                edge_map[i][0] = 0;
                break;
            }
            else
            {
                if(edge_map[i][j] == city_to_remove)
                {
                    for(int k = j; k < edge_count; k++)
                    {
                        int temp = edge_map[i][k];
                        edge_map[i][k] = edge_map[i][k+1];
                        edge_map[i][k+1] = temp;
                    }
                    edge_map[i][0] -= 1;
                    break;
                }
            }
        }
    }
}

bool Population::edge_check(int** edge_map, int city, int edge)
{
    for(int i = 1; i < (edge_map[city - 1][0] + 1); i++)
        if(edge_map[city - 1][i] == edge)
            return true;

    return false;
}

int Population::one_point_xover(Individual*& parent_1, Individual*& parent_2, Individual*& child_1, Individual*& child_2, long srand_offset)
{
    int index = random_index_in_range(0, options->chromosome_length, options->random_seed, srand_offset);

    for(int i = index + 1; i < options->chromosome_length; i++)
    {
            child_1->get_chromosome()[i] = parent_2->get_chromosome()[i];
            child_2->get_chromosome()[i] = parent_1->get_chromosome()[i];
    }

    return index;
}

// THIS PMX FUNCTION ASSUMES THAT CHILD_1/CHILD_2 IS ALREADY A COPY OF PARENT_1/PARENT_2
void Population::pmx(Individual* parent_1, Individual* parent_2, Individual* child_1, Individual* child_2, long srand_offset)
{
    int index_1 = random_index_in_range(0, options->chromosome_length, options->random_seed, srand_offset);

    int index_2 = -1;
    if(index_1 != options->chromosome_length - 1)
    {
        index_2 = random_index_in_range(index_1, options->chromosome_length - 1, options->random_seed, srand_offset);
        index_2 += 1; //ENSURES ITS AT LEAST ONE INDEX FURTHER THAN INDEX_1
    }

    pmx_indices[0] = index_1;
    pmx_indices[1] = index_2;

    if(index_2 != -1)
    {
        // INDICES_ACCOUNTED_FOR WILL STORE INDICES OF ALLELES THAT HAVE ALREADY BEEN PROVIDED TO THE CHILD BY THE OPPOSITE PARENT
        int indices_accounted_1[400];
        int indices_accounted_2[400];

        // INDICES TO CHANGE WILL FIRST BE INITIALIZED WITH ALL 1s. ONCE INDICES ACCOUNTED FOR IS FILLED, THE ELEMENT IN INDICES_TO_CHANGE AT EACH ACCOUNTED_FOR INDEX WILL BE UPDATED TO -1
        int indices_to_change_1[400];
        int indices_to_change_2[400];

        int count_1 = 0;
        int count_2 = 0;
        for(int i = 0; i < options->chromosome_length; i++)
        {
            if(i >= (index_1 + 1) && i <= index_2)
            {
                indices_accounted_1[count_1++] = i;
                indices_accounted_2[count_2++] = i;
            }
            else
            {
                indices_accounted_1[i] = -1;
                indices_accounted_2[i] = -1;
            }
            
            
            indices_to_change_1[i] = 1;
            indices_to_change_2[i] = 1;
        }

        bool copy_1 = false;
        bool copy_2 = false;
        int compare_1 = -1;
        int compare_2 = -1;
        for(int j = index_1 + 1; j <= index_2; j++)
        {

            copy_1 = false;
            copy_2 = false;
            compare_1 = parent_1->get_chromosome()[j];
            compare_2 = parent_2->get_chromosome()[j];
            
            // CHILD_1 ALGORITHM PT2 (PLACES THE REMAINING NON-COPY VALUES OF THE PARENT_2 XOVER REGION IN THE CHILD_1 CHROMOSOME)
            for(int i = index_1 + 1; i <= index_2; i++)
            {
                if(compare_2 == parent_1->get_chromosome()[i])
                {
                    copy_1 = true;
                    break;
                }
            }
            if(!copy_1)
            {

                int value_to_swap = parent_1->get_chromosome()[j];
                
                one:
                for(int i = 0; i < options->chromosome_length; i++)
                {
                    if(parent_2->get_chromosome()[i] == value_to_swap)
                    {
                        if(i >= (index_1 + 1) && i <= index_2)
                        {
                            value_to_swap = parent_1->get_chromosome()[i];
                            goto one;
                        }
                        else
                        {
                            child_1->get_chromosome()[i] = compare_2;
                            indices_accounted_1[count_1++] = i;
                        }

                        break;
                    }
                }
            }
            // END CHILD_1 ALGORITHM PT2

            // CHILD_2 ALGORITHM PT2 (PLACES THE REMAINING NON-COPY VALUES OF THE PARENT_1 XOVER REGION IN THE CHILD_2 CHROMOSOME)
            for(int i = index_1 + 1; i <= index_2; i++)
            {
                if(compare_1 == parent_2->get_chromosome()[i])
                {
                    copy_2 = true;
                    break;
                }
            }
            if(!copy_2)
            {
                int value_to_swap = parent_2->get_chromosome()[j];
                
                two:
                for(int i = 0; i < options->chromosome_length; i++)
                {
                    if(parent_1->get_chromosome()[i] == value_to_swap)
                    {
                        if(i >= (index_1 + 1) && i <= index_2)
                        {
                            value_to_swap = parent_2->get_chromosome()[i];
                            goto two;
                        }
                        else
                        {
                            child_2->get_chromosome()[i] = compare_1;
                            indices_accounted_2[count_2++] = i;
                        }

                        break;
                    }
                }
            }
            //END CHILD_2 ALGORITHM PT2
        } // END FOR

        // IDENTIFY REMAINING INDICES THAT NEED TO BE XOVER'D FROM PARENT TO OPPOSITE CHILD
        for(int i = 0; i < count_1; i++) //COUNT_1 == COUNT_2 IS ALWAYS TRUE
        {
            indices_to_change_1[indices_accounted_1[i]] = -1;
            indices_to_change_2[indices_accounted_2[i]] = -1;
        }
        for(int i = 0; i < options->chromosome_length; i++)
        {
            if(indices_to_change_1[i] == 1)
                child_1->get_chromosome()[i] = parent_2->get_chromosome()[i];
            
            if(indices_to_change_2[i] == 1)
                child_2->get_chromosome()[i] = parent_1->get_chromosome()[i];
        }
    }
    else // IF INDEX_1 IS THE 2ND TO LAST INDEX OF THE CHROMOSOME, RESULTS IN ONLY SWAPPING THE VERY LAST ALLELE OF THE CHROMOSOME
    {
        child_1->get_chromosome()[index_1 + 1] = parent_2->get_chromosome()[index_1 + 1];
        child_2->get_chromosome()[index_1 + 1] = parent_1->get_chromosome()[index_1 + 1];
    }
}

// AS IMPLEMENTED, THIS MUTATION HAS THE POTENTIAL TO "SCRAMBLE" ELEMENTS TO THE SAME VALUE - COULD CONSIDER AN IF-STATEMENT TO NOT ALLOW SUCH; OR COULD SIMPLY INCREASE MUTATION RATE.
void Population::scramble_mutation(Individual& member, long srand_offset)
{
    int first_index = random_index_in_range(0, options->chromosome_length - 1, options->random_seed, srand_offset);
    int last_index;
    if(first_index == options->chromosome_length - 2)
        last_index = options->chromosome_length - 1;
    else
    {
        last_index = first_index - 1;
        while(last_index <= first_index)
            last_index = random_index_in_range(1, options->chromosome_length, options->random_seed, srand_offset + (first_index + 1) + m_srand_offset_count++); // OFFSET + (FIRST_INDEX + 1) TO INCREASE RANDOMNESS. FIRST_INDEX + 1 TO GUARANTEE THE ADDITION DOESN'T EQUATE TO ZERO
    }

    int subsequence_length = last_index - first_index + 1;
    int* original_subsequence = new int[subsequence_length];
    int counting_index = first_index;
    int subsequence_index = 0;

    while(counting_index <= last_index)
        original_subsequence[subsequence_index++] = member.get_chromosome()[counting_index++];
    
    counting_index = 0;
    int* indices_chosen = new int[subsequence_length];
    int scramble_index = -2; // -2 SO IT CAN BE USED AS A SRAND_OFFSET ADDITION WITHOUT EQUATING TO ZERO (SEE BELOW)
    bool matching;
    
    while(counting_index < subsequence_length)
    {
        matching = false;
        scramble_index = random_index_in_range(0, subsequence_length, options->random_seed, srand_offset + (scramble_index + 1) + m_srand_offset_count++);
        for(int i = 0; i < counting_index; i++)
        {
            if(scramble_index == indices_chosen[i])
            {
                matching = true;
                break;
            }
        }
        if(!matching)
        {
            member.get_chromosome()[first_index + counting_index] = original_subsequence[scramble_index];
            indices_chosen[counting_index++] = scramble_index;
        }
    }

    delete[] original_subsequence;
    delete[] indices_chosen;
}    

void Population::CHC_generation(Population* child, Population *temp, long srand_offset)
{
    // TEST
    char temp_c;

    int index = -1;
    for(int j = 0; j < options->population_size; j++)
    {
        double fitness = 0;
        for(int i = 0; i < options->population_size*2; i++)
        {
            if(i < options->population_size)
            {
                if(members[i].get_fitness() >= fitness)
                {
                    fitness = members[i].get_fitness();
                    index = i;
                }
            }
            else
            {
                if(child->members[i - options->population_size].get_fitness() >= fitness)
                {
                    fitness = child->members[i - options->population_size].get_fitness();
                    index = i;
                }
            }
        }

        if(index < options->population_size)
        {
            temp->members[j] = members[index];
            members[index].set_fitness(-1);
        }
        else
        {
            temp->members[j] = child->members[index - options->population_size];
            child->members[index - options->population_size].set_fitness(-1);
        }


    }

    child->copy_members_and_update_id(*temp);
    child->sort_member_ids_by_fitness(options->random_seed, srand_offset);
}

// STATS
void Population::rank_selection_stats(int parent_index_1, int parent_index_2, int precision)
{
    double id_1 = members[parent_index_1].get_id();
    double id_2 = members[parent_index_2].get_id();

    // TEST
    // print("mem_id_1 = ", id_1);
    // print("mem_id_2 = ", id_2);
    // endl();
    
    double rank_1 = -1;
    double rank_2 = -1;
    bool rank_1_found = false;
    bool rank_2_found = false;
    for(int i = 0; i < options->population_size; i++)
    {
        if(!rank_1_found && member_ids[i][1] == id_1)
        {
            rank_1 = member_ids[i][0];
            rank_1_found = true;
        }
        if(!rank_2_found && member_ids[i][1] == id_2)
        {
            rank_2 = member_ids[i][0];
            rank_2_found = true;
        }
        if(rank_1_found && rank_2_found)
            break;
    }

    // TEST
    // print("rank_1 = ", rank_1);
    // print("rank_2 = ", rank_2);
    // endl();

    rank_1 /= m_average_rank_before_scaling;
    rank_2 /= m_average_rank_before_scaling;
    
    // TEST
    // print("PRE-ROUNDING: rank_1 = ", rank_1, 6);
    // print("PRE-ROUNDING: rank_2 = ", rank_2, 6);

    round_to_precision(rank_1, precision);
    round_to_precision(rank_2, precision);
    
    // TEST
    // double a = 1.432;
    // print("rank_1 = ", rank_1, 6);
    // print("rank_2 = ", rank_2, 6);
    // std::cout << "a = " << a << std::endl;
    // cin();

    int rank_1_index = -1;
    bool rank_1_index_found = false;
    for(int i = 0; i < selection_count_length; i++)
    {
        if(!rank_1_index_found && selection_count[i][0] == rank_1)
        {
            rank_1_index = i;
            rank_1_index_found = true;
            break;
        }
    }
    if(rank_1_index_found)
        selection_count[rank_1_index][1]++;
    else
    {
        int j = selection_count_length;
        while(selection_count[j-1][0] > rank_1 && j > 0)
        {
            selection_count[j][0] = selection_count[j-1][0];
            selection_count[j][1] = selection_count[j-1][1];
            j--;
        }
        selection_count[j][0] = rank_1;
        selection_count[j][1] = 1;
        selection_count_length++;
    }

    int rank_2_index = -1;
    bool rank_2_index_found = false;
    for(int i = 0; i < selection_count_length; i++)
    {
        if(!rank_2_index_found && selection_count[i][0] == rank_2)
        {
            rank_2_index = i;
            rank_2_index_found = true;
            break;
        }
    }

    if(rank_2_index_found)
        selection_count[rank_2_index][1]++;
    else
    {
        int j = selection_count_length;
        while(selection_count[j-1][0] > rank_2 && j > 0)
        {
            selection_count[j][0] = selection_count[j-1][0];
            selection_count[j][1] = selection_count[j-1][1];
            j--;
        }
        selection_count[j][0] = rank_2;
        selection_count[j][1] = 1;
        selection_count_length++;
    }

    // TEST
    // print("selection_count_length = ", selection_count_length);
    // endl();
}

void Population::rand_stats(double number, int precision)
{
    round_to_precision(number, precision);

    int number_index = -1;
    bool number_index_found = false;
    for(int i = 0; i < rand_count_length; i++)
    {
        if(!number_index_found && rand_count[i][0] == number)
        {
            number_index = i;
            number_index_found = true;
            break;
        }
    }

    if(number_index_found)
        rand_count[number_index][1]++;
    else
    {

        int j = rand_count_length;
        while(rand_count[j-1][0] > number && j > 0)
        {
            rand_count[j][0] = rand_count[j-1][0];
            rand_count[j][1] = rand_count[j-1][1];
            j--;
        }

        rand_count[j][0] = number;
        rand_count[j][1] = 1;
        rand_count_length++;
    }
}

// FILE IO
void Population::write_population_to_file()
{
    std::ofstream out("population.txt");
    for(int i = 0; i < options->population_size; i++)
    {
        for(int j = 0; j < options->chromosome_length; j++)
        {
            out << members[i].get_chromosome()[j] << " ";
        }
        out << std::fixed << std::setprecision(15) << members[i].get_fitness() << " ";
        out << std::setprecision(0) << members[i].get_objective_value() << std::endl;
    }
    out.close();
}

//TEST
void Population::print_pop()
{
    for(int i = 0; i < options->population_size; i++)
    {
        std::cout << "MEM_"<<i<<": ";
        members[i].print_ind();
    }
    std::cout << std::endl;
}

// TEST
void Population::print_member_ids()
{
    for(int i = 0; i < options->population_size; i++)
    {
        if(i!=0 && i%10 == 0)
            std::cout << std::fixed << std::setprecision(2) << "(" << member_ids[i][0] << "," << std::setprecision(0) << member_ids[i][1] << ") " << std::endl;
        else
            std::cout << std::fixed << std::setprecision(2) << "(" << member_ids[i][0] << "," << std::setprecision(0) << member_ids[i][1] << ") ";
    }
    std::cout << std::endl;
}

void Population::print_edge_map(int edge_map[][5])
{
    for(int i = 0; i < options->chromosome_length; i++)
    {
        int edge_cnt = edge_map[i][0];
        for(int j = 0; j < edge_cnt + 1; j++)
        {
            if(j == 0)
                std::cout << i+1 << " " << edge_map[i][j] << " ";
            else
                std::cout << edge_map[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void Population::print_selection_count_horizontal(int precision)
{
    for(int i = 0; i < selection_count_length; i++)
        print(selection_count[i][0], " ", 2);
    endl();
    for(int i = 0; i < selection_count_length; i++)
        print(selection_count[i][1], " ", precision);
    endl();
}

void Population::print_selection_count_vertical(int precision)
{
    int total = 0;
    for(int i = 0; i < selection_count_length; i++)
        total += selection_count[i][1];
    print("Rank sum = ", sum_rank);
    print("Stats length = " , selection_count_length);
    print("Total = " , total);
    for(int i = 0; i < selection_count_length; i++)
    {
        std::cout << string_precision(selection_count[i][0], precision) << "\t" << (int)selection_count[i][1] << "\t" << string_precision(selection_count[i][1]*100/total, 3) << std::endl;
    }
}

void Population::print_rand_count_vertical(int precision)
{
    int total = 0;
    for(int i = 0; i < rand_count_length; i++)
        total += rand_count[i][1];
    print("Total = " , total);
    for(int i = 0; i < rand_count_length; i++)
    {
        std::cout << string_precision(rand_count[i][0], precision) << "\t" << (int)rand_count[i][1] << "\t" << string_precision(rand_count[i][1]*100/total, 3) << std::endl;
    }
}

void Population::set_average_rank_before_scaling()
{
    m_average_rank_before_scaling = average_rank();
}

double Population::get_average_before_scaling()
{
    return m_average_rank_before_scaling;
}