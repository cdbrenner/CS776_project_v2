#include "test.h"

void max_floorplanning_objective_value(int choice, int weight_normal, int weight_special)
{
    if(choice == 1)
    {
        double variables[7] = {20,18,18,6,17,20,18};

        double living = 1.5*pow((8 + variables[0]),2);
        living < 120 ? living = 300 + weight_normal*(120 - living) : 0;
        living > 300 ? living = 300 + weight_normal*(living - 300) : 0;

        double kitchen = (6 + variables[1])*(6 + variables[2]);
        kitchen < 50 ? kitchen = 120 + weight_special*(50 - kitchen) : 0;
        kitchen > 120 ? kitchen = 120 + weight_special*(kitchen - 120) : 0;

        double hall = 5.5*(3.5 + variables[3]);
        hall < 19 ? hall = 72 + weight_normal*(19 - hall) : 0;
        hall > 72 ? hall = 72 + weight_normal*(hall - 72) : 0;

        double bed_1 = 1.5*pow((10 + variables[4]),2);
        bed_1 < 100 ? bed_1 = 180 + weight_normal*(100 - bed_1) : 0;
        bed_1 > 180 ? bed_1 = 180 + weight_normal*(bed_1 - 180) : 0;

        double bed_2 = 1.5*pow((9 + variables[5]),2);
        bed_2 < 100 ? bed_2 = 180 + weight_normal*(100 - bed_2) : 0;
        bed_2 > 180 ? bed_2 = 180 + weight_normal*(bed_2 - 180) : 0;

        double bed_3 = 1.5*pow((8 + variables[6]),2);
        bed_3 < 100 ? bed_3 = 180 + weight_normal*(100 - bed_3) : 0;
        bed_3 > 180 ? bed_3 = 180 + weight_normal*(bed_3 - 180) : 0;

        double objective_function = living + 2*kitchen + hall + bed_1 + bed_2 + bed_3;

        std::cout << objective_function << std::endl;
    }
    else if(choice == 2)
    {
        double variables[7] = {20,18,18,6,17,20,18};

        double living = 1.5*pow((8 + variables[0]),2);

        double kitchen = (6 + variables[1])*(6 + variables[2]);

        double hall = 5.5*(3.5 + variables[3]);

        double bed_1 = 1.5*pow((10 + variables[4]),2);

        double bed_2 = 1.5*pow((9 + variables[5]),2);

        double bed_3 = 1.5*pow((8 + variables[6]),2);

        double objective_function = living + 2*kitchen + hall + bed_1 + bed_2 + bed_3;

        std::cout << objective_function << std::endl;
    }
    else
        std::cout << "CHOOSE 1 or 2 FOR TEST" << std::endl;
}

void test_nine_variables_in_random_choice(int runs, int bit_length)
{
    int neg = 0;
    int zero = 0;
    int one = 0;
    int two = 0;
    int three = 0;
    int four = 0;
    int five = 0;
    int six = 0;
    int seven = 0;
    int eight = 0;
    int nine = 0;

    for(int i = 0; i < runs; i++)
    {
        switch(random_index_in_range(0, 10, 1991, i))
        {
            case -1:
                neg++;
                break;
            case 0:
                zero++;
                break;
            case 1:
                one++;
                break;
            case 2:
                two++;
                break;
            case 3:
                three++;
                break;
            case 4:
                four++;
                break;
            case 5:
                five++;
                break;
            case 6:
                six++;
                break;
            case 7:
                seven++;
                break;
            case 8:
                eight++;
                break;
            case 9:
                nine++;
                break;
        }
    }

    std::cout << "neg = " << (double)neg/runs << std::endl;
    std::cout << "zero = " << (double)zero/runs << std::endl;
    std::cout << "one = " << (double)one/runs << std::endl;
    std::cout << "two = " << (double)two/runs << std::endl;
    std::cout << "three = " << (double)three/runs << std::endl;
    std::cout << "four = " << (double)four/runs << std::endl;
    std::cout << "five = " << (double)five/runs << std::endl;
    std::cout << "six = " << (double)six/runs << std::endl;
    std::cout << "seven = " << (double)seven/runs << std::endl;
    std::cout << "eight = " << (double)eight/runs << std::endl;
    std::cout << "nine = " << (double)nine/runs << std::endl;
}

void verify_xover_mutation_mechanism(Population* parent, Population* child)
{
    std::string error_message = "XOVER-MUTATE MECHANISM TEST FAILED";

    bool equal = true;
    for(int i = 0; i < parent->get_options()->population_size; i++)
    {
        int isParent = child->get_transform_data(i)[child->get_options()->m_isParent_index];

        if(!isParent)
        {
            int parent_1_index = child->get_transform_data(i)[child->get_options()->m_parent_1_index];
            int parent_2_index = child->get_transform_data(i)[child->get_options()->m_parent_2_index];
            int xover_index = child->get_transform_data(i)[child->get_options()->m_xover_index];
            int total_mutations = child->get_transform_data(i)[0] - child->get_options()->m_meta_length;
            
            int mutation_count = 0;
        
            for(int j = 0; xover_index != -1 ? j <= xover_index : j < parent->get_options()->chromosome_length; j++)
            {
                if(parent->get_members()[parent_1_index].get_chromosome()[j] != child->get_members()[i].get_chromosome()[j])
                // if(child->get_members()[i].get_chromosome()[j] != (isParent ? grandparent->get_members()[parent_1_index].get_chromosome()[j]
                //                                                                 : parent->get_members()[parent_1_index].get_chromosome()[j]))
                {
                    if(mutation_count == total_mutations || j != child->get_transform_data(i)[child->get_options()->m_meta_length + mutation_count]) // 4 PREVIOUS ARRAY MEMBERS + MUTATION_COUNT BEFORE VERIFYING MUTATION
                    {
                        std::cout << "::TEST: FAILED!" << std::endl;
                        std::cout << "CHILD["<<i<<"]\n";
                        std::cout << "BEFORE XOVER" << std::endl;
                        std::cout << "total mutations = " << total_mutations << std::endl;
                        std::cout << "mutation count =  " << mutation_count << std::endl;
                        std::cout << "j =               " << j << std::endl;
                        std::cout << "mutation index =  " << child->get_transform_data(i)[child->get_options()->m_meta_length + mutation_count] << std::endl;

                        equal = false;
                        break;
                    }
                    mutation_count++;
                }
            }
            if(xover_index != - 1)
            {
                for(int j = xover_index+1; j < parent->get_options()->chromosome_length; j++)
                {
                    if(parent->get_members()[parent_2_index].get_chromosome()[j] != child->get_members()[i].get_chromosome()[j])
                    // if(child->get_members()[i].get_chromosome()[j] != (isParent ? grandparent->get_members()[parent_2_index].get_chromosome()[j]
                    //                                                             : parent->get_members()[parent_2_index].get_chromosome()[j]))
                    {
                        if(mutation_count == total_mutations || j != child->get_transform_data(i)[child->get_options()->m_meta_length + mutation_count])
                        {
                            std::cout << "::TEST: FAILED!" << std::endl;
                            std::cout << "CHILD["<<i<<"]\n";
                            std::cout << "AFTER XOVER" << std::endl;
                            std::cout << "total mutations = " << total_mutations << std::endl;
                            std::cout << "mutation count =  " << mutation_count << std::endl;
                            std::cout << "j =               " << j << std::endl;
                            std::cout << "mutation index =  " << child->get_transform_data(i)[child->get_options()->m_meta_length + mutation_count] << std::endl;

                            equal = false;
                            break;
                        }
                        mutation_count++;
                    }
                }
            }
        }
        
        if(!equal)
            break;
    }
    
    if(!equal)
        throw(error_message);
}

void verify_string_equivalence(Individual* one, Individual* two, std::string function_location)
{
    int one_length = one->get_chromosome_length();
    int two_length = two->get_chromosome_length();
    if(one_length != two_length)
    {
        std::string error_message = "STRING EQUIVALENCE FAILED DUE TO NON-EQUAL LENGTHS IN " + function_location;
        throw(error_message);
    }
    for(int i = 0; i < one_length; i++)
    {
        if(one->get_chromosome()[i] != two->get_chromosome()[i])
        {
            std::string error_message = "STRING EQUIVALENCE FAILED DUE TO NON-MATCHING ELEMENTS IN " + function_location;
            throw(error_message);
        }
    }
}

bool duplicate_values_in_string(Individual& ind, int chromosome_length)
{
    for(int i = 0; i < chromosome_length; i++)
    {
        int count = 0;
        for(int j = 0; j < chromosome_length; j++)
        {
            if(ind.get_chromosome()[j] == i+1)
                count++;

            if(count == 2)
                return true;
        }
    }
    return false;
}

bool symmetric_matrix(int** tour_data, int chromosome_length)
{
    for(int i = 0; i < chromosome_length; i++)
        for(int j = 0; j < chromosome_length; j++)
            if(tour_data[i][j] != tour_data[j][i])
                return false;
    
    return true;
}

bool identical_matrices(int tour_data_1[][48], int tour_data_2[][48], int chromosome_length)
{
    for(int i = 0; i < chromosome_length; i++)
        for(int j = 0; j < chromosome_length; j++)
            if(tour_data_1[i][j] != tour_data_2[i][j])
                return false;
    
    return true;
}