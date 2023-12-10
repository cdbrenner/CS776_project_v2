//TESTS IN MUTATE - & TEST FUNCTIONS EXIST

#include "individual.h"

Individual::Individual()
{
    chromosome_length = -1;
    fitness = -1;
}

Individual::Individual(int length)
{
    chromosome_length = length;
    fitness = -1;
    for(int i = 0; i < MAX_SIZE; i++)
        chromosome[i] = -1;
}

Individual::Individual(int length, int* chromosome)
{
    chromosome_length = length;
    fitness = -1;

    for(int i = 0; i < chromosome_length; i++)
    {
        this->chromosome[i] = chromosome[i];
    }
}

Individual::Individual(const Individual& rhs)
{
    chromosome_length = rhs.chromosome_length;

    if(chromosome_length != 0)
    {
        for(int i = 0; i < chromosome_length; i++)
            chromosome[i] = rhs.chromosome[i];
    }
    
    fitness = rhs.fitness;
    objective_value = rhs.objective_value;
    dimension_count = rhs.dimension_count;

    if(dimension_count != 0)
    {
        for(int i = 0; i < dimension_count; i++)
        {
            dimensions[i] = rhs.dimensions[i];
        }
    }
}

Individual::~Individual() {}

void Individual::set_chromosome_length(int length)
{
    chromosome_length = length;
}

void Individual::set_chromosome(int length, int* chrom)
{
    chromosome_length = length;
    for(int i = 0; i < chromosome_length; i++)
        chromosome[i] = chrom[i];
}

void Individual::set_chromosome_element(int index, int value)
{
    chromosome[index] = value;
}

void Individual::set_fitness(double fitness)
{
    this->fitness = fitness;
}

void Individual::set_objective_value(double objective_value)
{
    this->objective_value = objective_value;
}

void Individual::set_dimensions(double* data, int count)
{
    dimension_count = count;

    for(int i = 0; i < count; i++)
    {
        dimensions[i] = data[i];
    }
}

void Individual::set_id(int id)
{
    this->id = id;
}

int* Individual::get_chromosome()
{
    return chromosome;
}

int Individual::get_chromosome_length()
{
    return chromosome_length;
}

int* Individual::get_mutate_data(int row)
{
    return swap_mutate_data[row];
}

int Individual::get_mutate_count()
{
    return swap_mutate_count;
}

int* Individual::get_transform_data()
{
    return transform_data;
}

double Individual::get_fitness()
{
    return fitness;
}

double Individual::get_objective_value()
{
    return objective_value;
}

int Individual::get_dimension_count()
{
    return dimension_count;
}

double* Individual::get_dimensions()
{
    return dimensions;
}

int Individual::get_id()
{
    return id;
}

void Individual::init_TSP(int random_seed, int srand_offset)
{
    int fail_count = 0;
    for(int i = 0; i < chromosome_length; i++)
    {
        one:
        int value = random_index_in_range(1, (chromosome_length + 1), random_seed, srand_offset*chromosome_length + i + fail_count); // FOR A TOUR STARTING AT 'NODE 1', NOT 'NODE 0' (NODE 0 DOESN'T EXIST IN THIS IMPLEMENTATION)
        if(i == 0)
            chromosome[i] = value;
        else
        {
            for(int j = 0; j < i; j++)
            {
                if(value == chromosome[j])
                {
                    fail_count++;
                    goto one;
                }
            }
            chromosome[i] = value;
        }
    }
}

void Individual::init(int random_seed, int srand_offset)
{
    for(int i = 0; i < chromosome_length; i++)
    {
        chromosome[i] = flip(0.5f, random_seed, srand_offset*chromosome_length + i);
    }

    // UNIT TEST OBJECT: COMMENTED OUT FOR TSP
    transform_data[0] = 1;
}

// NORMAL MUTATE: COMMENTED OUT FOR CURRENT TSP PROGRAM
// void Individual::mutate(double probability, int random_seed, int srand_offset)
// {
//     mutate_count = 0;
//     for(int i = 0; i < chromosome_length; i++)
//     {
//         if(flip(probability, random_seed, srand_offset*chromosome_length + i))
//         {
//             chromosome[i] = 1 - chromosome[i];
//             mutate_data[mutate_count] = i;
//             mutate_count++;
//         }
//     }
// }

void Individual::swap_mutate(double probability, int random_seed, int srand_offset)
{
    swap_mutate_count = 0;
    int fail_count = 0;
    for(int i = 0; i < chromosome_length; i++)
    {
        if(flip(probability, random_seed, srand_offset*chromosome_length + i))
        {
            one:
            int index_to_swap = random_index_in_range(0, chromosome_length, random_seed, srand_offset*chromosome_length + i + fail_count);
            if(index_to_swap == i)
            {
                fail_count++;
                goto one;
            }

            int temp_1 = chromosome[i];
            chromosome[i] = chromosome[index_to_swap];
            chromosome[index_to_swap] = temp_1;
            swap_mutate_data[swap_mutate_count][0] = i;
            swap_mutate_data[swap_mutate_count++][1] = index_to_swap;
        }
    }
}

void Individual::copy_individual_data(const Individual& rhs)
{
    chromosome_length = rhs.chromosome_length;


    for(int i = 0; i < chromosome_length; i++)
    {
        chromosome[i] = rhs.chromosome[i];
    }

}

void Individual::print_ind()
{
    for(int i = 0; i < chromosome_length; i++)
        std::cout << chromosome[i] << " ";
    std::cout << std::endl;
}

void Individual::print_transform_data()
{
    //TEST
    std::cout << "PRINT_TRANSFORM_DATA: TRANSFORM_DATA[0] = " << transform_data[0] << std::endl;

    for(int i = 0; i < transform_data[0]; i++)
    {
        std::cout << transform_data[i] << " ";
    }
    std::cout << std::endl;
}

//FOR TESTING
// double Individual::get_mutation_percentage()
// {
//     return mutation_count/total_mutation_attempts;
// }

// void Individual::fixed_low_fitness_init()
// {
//     for(int i = 0; i < chromosome_length - 1; i++)
//     {
//             chromosome[i] = 0;
//     }
//     chromosome[chromosome_length - 1] = 1;
// }