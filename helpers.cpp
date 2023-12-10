#include "helpers.h"

//PRINT HELPERS
std::string add_whitespace(int current_line_number, int total_line_count, bool delimeter)
{
    int current_digits;
    int total_digits;
    if(delimeter)
    {
        current_digits = std::to_string(current_line_number).length() + 1;
        total_digits = std::to_string(total_line_count).length() + 1;
    }
    else
    {
        current_digits = std::to_string(current_line_number).length();
        total_digits = std::to_string(total_line_count).length();
    }

    if(total_digits >= 4 && total_digits < 8)
    {
        if(current_digits == 1)
            return "   ";
        else if(current_digits == 2)
            return "  ";
        else if(current_digits == 3)
            return " ";
    }

    return "";
}

std::string leading_whitespace(double number, int digit_length, int max_digit_length, int precision)
{
    digit_length = std::to_string(digit_length).length();
    int digit_diff = max_digit_length - digit_length;

    std::string temp_1;
    string_stream(temp_1, number, precision);
    for(int i = 0; i < digit_diff; i++)
        temp_1.insert(0," ");
    
    return temp_1;
}

std::string string_precision(double number, int precision)
{
    std::string temp;
    string_stream_fixed(temp, number, precision);
    return temp;
}

void print(std::string message)
{
    std::cout << message << std::endl;
}

void print(std::string message, int number)
{
    std::cout << message << number << std::endl;
}

void print(std::string message_pt1, int iterator, std::string message_pt2, double number)
{
    std::cout << message_pt1 << iterator << message_pt2 << number << std::endl;
}

void print(std::string message_pt1, int iterator_1, std::string message_pt2, int iterator_2, std::string message_pt3, double number)
{
    std::cout << message_pt1 << iterator_1 << message_pt2 << iterator_2 << message_pt3 << number << std::endl;
}

void print(std::string message_pt1, int iterator, std::string message_pt2, double number, int precision)
{
    std::cout << message_pt1 << iterator << message_pt2;
    std::cout << std::fixed << std::setprecision(precision) << number << std::endl;
}

void print(std::string message, double number)
{
    std::cout << message << number << std::endl;
}

void print(std::string message, double number, int precision)
{
    std::cout <<  message << std::fixed << std::setprecision(precision) << number << std::endl;
}

void print(int number, std::string message)
{
    std::cout << number << message;
}

void print(double number, std::string message, int precision)
{
    std::cout << std::fixed << std::setprecision(precision) << number << message;
}

void print(std::string message, void (*function)())
{
    std::cout << message;
    function();
}

void endl()
{
    std::cout << std::endl;
}

void cin()
{
    char temp;
    std::cin >> temp;
}

//LOGGER
void log(std::ofstream& out, std::string message)
{
    out << message << std::endl;
}

void log(std::ofstream& out, std::string message, int number)
{
    out << message << number << std::endl;
}

void log(std::ofstream& out, int number_1, std::string message_1, int number_2, std::string message_2)
{
    out << number_1 << message_1 << number_2 << message_2;
}

void log(std::ofstream& out, std::string message, double number)
{
    out << message << number << std::endl;
}

void log(std::ofstream& out, std::string message, double number, int precision)
{
    out <<  message << std::fixed << std::setprecision(precision) << number << std::endl;
}

void log(std::ofstream& out, int number, std::string message)
{
    out << number << message;
}

void log_endl(std::ofstream& out)
{
    out << std::endl;
}

void delete_log(std::ofstream& out)
{
    out.close();
    std::remove("log.txt");
}

// MATH HELPERS
void round_to_precision(double& number, int precision)
{
    number = (int)(number * pow(10,precision) + 0.5);
    number /= pow(10,precision);
}

// PROBABILITIY HELPERS
int flip(float probability, int random_seed, int offset)
{
    srand(random_seed + offset);
    return (rand() < probability*(float)RAND_MAX ? 1 : 0);
}

int random_index_in_range(int low, int high, int random_seed, int offset)
{
    srand(random_seed + offset);
    return low + rand()%(high - low);
}

void report_averager(int iterations, std::string filename, std::string output_name, int precision, int population_size)
{
    std::cout << filename << std::endl;
    std::cout << output_name << std::endl;

    std::ifstream in(filename);
    std::ofstream out(output_name);

    out << "GEN,\t\tMIN,\t\t\tAVE,\t\t\tMAX,\n";

    std::string temp;
    std::string gen_str;
    std::string min_str;
    std::string ave_str;
    std::string max_str;
    int gen;
    double min;
    double ave;
    double max;
    getline(in, temp);

    for(int i = 0; i < population_size; i++)
    {
        getline(in,gen_str,',');
        getline(in,min_str,',');
        getline(in,ave_str,',');
        getline(in,max_str,',');
        gen = atoi(gen_str.c_str());
        min = atof(min_str.c_str());
        ave = atof(ave_str.c_str());
        max = atof(max_str.c_str());

        min /= iterations;
        ave /= iterations;
        max /= iterations;

        out << std::fixed << std::setprecision(precision) << gen << ",\t\t" << min << ",\t\t" << ave << ",\t\t" << max << "," << std::endl;

    }
    out.close();
    std::remove(filename.c_str());
}

//IO HELPERS
int int_choice()
{
    std::string temp;
    one:
    std::cin >> temp;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    bool valid = validate_int(temp);
    
    if(valid)
        return std::stoi(temp);
    else
    {
        std::cout << "\nPlease enter a valid choice (integer):"  << std::endl;
        goto one;
    }
}

double double_choice()
{
    std::string temp;
    one:
    std::cin >> temp;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    bool valid = validate_real_number(temp);
    
    if(valid)
        return std::strtod(temp.c_str(), NULL);
    else
    {
        std::cout << "\nPlease enter a valid choice (double):"  << std::endl;
        goto one;
    }
}

//TRY-CATCH HELPERS
void try_args_to_int_in_range(char* argv, int& var, int first, int last, int default_value, std::string error_message)
{
    try
    {
        var = std::stoi(argv);

        if(var < first || var > last)
        {
            var = default_value;
            throw(error_message);
        }
    }
    catch(std::string error_message)
    {
        throw(error_message);
    }
    catch(...)
    {
        var = default_value;
        throw(error_message);
    }

}
 
void try_args_to_double_in_range(char* argv, double& var, int first, int last, double default_value, std::string error_message)
{
    try
    {
        if(!validate_real_number(argv))
        {
            var = default_value;
            throw(error_message);
        }

        var = std::strtod(argv, NULL);

        if(var < first || var > last)
        {
            var = default_value;
            throw(error_message);
        }
    }
    catch(std::string error_message)
    {
        throw(error_message);
    }
}

// VALIDATION
bool validate_real_number(std::string temp)
{
    bool valid = true;
    int decimal_count = 0;
    for(int i = 0; i < temp.length(); i++)
    {
        if(i == 0 && temp.c_str()[i] == '-') {}
        else if(isdigit(temp.c_str()[i])) {}
        else if(temp.c_str()[i] == '.')
        {
            decimal_count++;
            if(decimal_count > 1)
            {
                valid = false;
                break;
            }
        }
        else
        {
            valid = false;
            break;
        }
    }

    return valid;
}

bool validate_int(std::string temp)
{
    bool valid = true;
    int decimal_count = 0;
    for(int i = 0; i < temp.length(); i++)
    {
        if(i == 0 && temp.c_str()[i] == '-') {}
        else if(isdigit(temp.c_str()[i])) {}
        else
        {
            valid = false;
            break;
        }
    }

    return valid;
}

bool equals(int a, int b)
{
    if(a == b)
        return true;
    
    return false;
}

// STRING HELPERS
void string_stream(std::string& str, double stream_value, int precision)
{
    std::stringstream ss;
    if(precision != 0)
        ss.precision(precision);
    ss << stream_value;
    ss >> str;
}

void string_stream_fixed(std::string& str, double stream_value, int precision)
{
    std::stringstream ss;
    if(precision != 0)
        ss.precision(precision);
    ss << std::fixed << stream_value;
    ss >> str;
}

void set_variant_name(std::string& variant_name, std::string& variant_name_abbreviation, int ga_variant_option)
{
    switch(ga_variant_option)
    {
        case 1:
            variant_name = "Simple GA";
            variant_name_abbreviation = "S";
            break;
        case 2:
            variant_name = "Simple GA with Extinction Events";
            variant_name_abbreviation = "S-E";
            break;
        case 3:
            variant_name = "CHC";
            variant_name_abbreviation = "CHC";
            break;
        case 4:
            variant_name = "CHC with Extinction Events";
            variant_name_abbreviation = "CHC-E";
            break;
        case 5:
            variant_name = "Genitor";
            variant_name_abbreviation = "G";
            break;
    }
}

void set_reporting_str(std::string& reporting_option_str, int report_option)
{
    if(report_option == 1)
        reporting_option_str = "logging reports";
    else if(report_option == 2)
        reporting_option_str = "ignore reporting";
}

//MENU HELPERS
void menu_choice_with_dynamic_truncation(int argc, int argc_current, int total_menu_items, int& choice)
{
    bool complete = false;
    int menu_level = 1;
    while(!complete)
    {
        choice_with_dynamic_truncation(complete, argc, argc_current, menu_level, total_menu_items, choice);
        argc_current++;
        menu_level++;
    }
}

void choice_with_dynamic_truncation(bool& complete, int argc, int argc_current, int menu_level, int total_menu_items, int& choice)
{
    if(argc <= argc_current)
    {
        one:
        choice = int_choice();
        if(choice < menu_level || choice > total_menu_items)
        {
            std::cout << "Please choose a valid menu entry ("<<menu_level<<" - "<<total_menu_items<<"):" << std::endl;
            goto one;
        }
        complete = true;
    }
}