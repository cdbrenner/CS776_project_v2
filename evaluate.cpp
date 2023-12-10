#include "evaluate.h"

void eval(Individual& individual, Eval_results& results, int** tour_data, int choice, int random_seed, int srand_offset, int eval_option)
{
    try
        // {TSP(individual, tour_data, tour _count, choice, results);}
        {TSP(individual, tour_data, choice, results, eval_option);}
    catch(std::string error_message)
        {throw error_message;}
}

double* decode(Individual& individual, int bit_length, double scaler)
{
    int variable_count = individual.get_chromosome_length()/bit_length;

    double* variables = new double[variable_count];

    for(int j = 0; j < variable_count; j++)
    {
        double variable_sum = 0;
        double variable_sign = 0;
        for(int i = 0; i < bit_length; i++)
        {
            if(i == 0)
            {
                individual.get_chromosome()[(j*bit_length) + i] == 1 ? variable_sign = 1 : variable_sign = -1;
            }
            else
            {
                individual.get_chromosome()[(j*bit_length) + i] == 1 ? variable_sum += pow(2,bit_length - (i+1)) : 0;
            }
        }
        variable_sum /= scaler;
        variable_sum *= variable_sign;
        variables[j] = variable_sum;
    }
    
    return variables;
}

double* decode_withVarsOfDiffBitLength_onlyPositive(Individual& individual, int variable_count, int bit_length[], double scaler[])
{
    double* variables = new double[variable_count];

    int chromosome_position = 0;
    for(int j = 0; j < variable_count; j++)
    {

        double variable_sum = 0;
        for(int i = 0; i < bit_length[j]; i++)
            individual.get_chromosome()[chromosome_position + i] == 1 ? variable_sum += pow(2,bit_length[j] - (i+1)) : 0;
        
        chromosome_position += bit_length[j];

        variable_sum /= scaler[j];
        variables[j] = variable_sum;
    }
    
    return variables;
}

void TSP(Individual& individual, int** tour_data, int choice, Eval_results& results, int eval_option)
{
    // TEST
    // print("EVALUATE:: TSP: eval_option = ", eval_option);
    // cin();

    if(eval_option == 1 || eval_option == 2)
    {
        switch(eval_option)
        {
            case 1:
                TSP_EXPLICIT(individual, tour_data, choice, results);
                break;
            case 2:
                TSP_EUC2D(individual, tour_data, choice, results);
                break;
        }
    }
    else
    {
        std::string error_message = "EVALUATE::TSP: Invalid evaluation option given. Aborting.";
        throw(error_message);
    }
}

void TSP_EXPLICIT(Individual& individual, int** tour_data, int choice, Eval_results& results)
{
    results.objective = 0;
    for(int i = 1; i < individual.get_chromosome_length(); i++)
    {
        int city_1 = individual.get_chromosome()[i - 1];
        int city_2 = individual.get_chromosome()[i];

        if(choice == 2) //EUC2D WILL BE DECODED
        {

        }
        else
            results.objective += tour_data[city_1 - 1][city_2 - 1];
    }
    results.objective += tour_data[individual.get_chromosome()[0] - 1][individual.get_chromosome()[individual.get_chromosome_length() - 1] - 1];

    if(results.objective != 0)
        results.fitness = 1/results.objective;
    else if(results.objective == 0)
    {
        std::string error_message = "EVALUATE::TSP_EXPLICIT: Evaluation failed because objective function equals 0.";
        throw(error_message);
    }
    else if(results.objective < 0)
    {
        std::string error_message = "EVALUATE::TSP_EXPLICIT: Evaluation failed because objective function is less than 0.";
        throw(error_message);
    }
}

void TSP_EUC2D(Individual& individual, int** tour_data, int choice, Eval_results& results)
{
    // TEST
    // print("EVALUATE:: Top of TSP_EUC2D");
    // print("choice = ", choice);
    // cin();

    double x1 = -1;
    double y1 = -1;
    double x2 = -1;
    double y2 = -1;

    results.objective = 0;
    for(int i = 1; i < individual.get_chromosome_length(); i++)
    {
        int city_1 = individual.get_chromosome()[i - 1];
        x1 = -1;
        y1 = -1;
        int city_2 = individual.get_chromosome()[i];
        x2 = -1;
        y2 = -1;

        for(int j = 0; j < individual.get_chromosome_length(); j++)
        {
            if(city_1 == tour_data[j][0])
            {
                x1 = tour_data[j][1];
                y1 = tour_data[j][2];
                break;
            }
        }

        for(int j = 0; j < individual.get_chromosome_length(); j++)
        {
            if(city_2 == tour_data[j][0])
            {
                x2 = tour_data[j][1];
                y2 = tour_data[j][2];
                break;
            }
        }

        // TEST
        // print("city_1 = ", city_1);
        // print("x1 = ", x1);
        // print("y1 = ", y1);
        // print("city_2 = ", city_2);
        // print("x2 = ", x2);
        // print("y2 = ", y2);
        // cin();

        if(choice == 2)
            results.objective += distance(x1,y1,x2,y2);
        else if(choice == 3)
            results.objective += distance_geo(x1,y1,x2,y2);
    }
    

    if(results.objective != 0)
        results.fitness = 1/results.objective;
    else if(results.objective == 0)
    {
        std::string error_message = "EVALUATE::TSP_EUC2D: Evaluation failed because objective function equals 0.";
        throw(error_message);
    }
    else if(results.objective < 0)
    {
        std::string error_message = "EVALUATE::TSP_EUC2D: Evaluation failed because objective function is less than 0.";
        throw(error_message);
    }
}

double distance(double x1, double y1, double x2, double y2)
{
    return nearbyint(sqrt(pow((x2 - x1),2) + pow((y2 - y1),2)));
}

double distance_geo(double x1, double y1, double x2, double y2)
{
    double pi = 3.141592;

    double deg_x1 = nearbyint(x1);
    double min_x1 = x1 - deg_x1;
    double latitude_x1 = pi * (deg_x1 + 5 * min_x1/3)/180;

    double deg_y1 = nearbyint(y1);
    double min_y1 = y1 - deg_y1;
    double longitude_y1 = pi * (deg_y1 + 5 * min_y1/3)/180;

    double deg_x2 = nearbyint(x2);
    double min_x2 = x2 - deg_x2;
    double latitude_x2 = pi * (deg_x2 + 5 * min_x2/3)/180;

    double deg_y2 = nearbyint(y2);
    double min_y2 = y2 - deg_y2;
    double longitude_y2 = pi * (deg_y2 + 5 * min_y2/3)/180;

    double rrr = 6378.388;

    double q1 = cos(longitude_y2 - longitude_y1);
    double q2 = cos(latitude_x2 - latitude_x1);
    double q3 = cos(latitude_x2 + latitude_x1);

    return (int) (rrr * acos(0.5*((1 + q1)*q2 - (1 - q1)*q3)) + 1);
}