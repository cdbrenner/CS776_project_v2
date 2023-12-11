#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
// #include <unistd.h>
#include <limits>
#include <cmath>

//PRINT HELPERS
std::string add_whitespace(int current_line_number, int total_line_count, bool delimeter);
std::string leading_whitespace(double number, int digit_length, int max_digit_length, int precision);
std::string string_precision(double number, int precision);
void print(std::string);
void print(std::string, int);
void print(std::string, int, std::string, int, std::string, double);
void print(std::string, int, std::string, double);
void print(std::string, int, std::string, double, int);
void print(std::string, long);
void print(std::string, double);
void print(std::string, double, int);
void print(int, std::string);
void print(double, std::string, int);
void print(std::string, void(*)());
void endl();
void cin();

//LOGGER
void log(std::ofstream&, std::string);
void log(std::ofstream&, std::string, int);
void log(std::ofstream&, int, std::string, int, std::string);
void log(std::ofstream&, std::string, double);
void log(std::ofstream&, std::string, double, int);
void log(std::ofstream&, int, std::string);
void log_endl(std::ofstream&);
void delete_log(std::ofstream&);

// MATH HELPERS
void round_to_precision(double& number, int precision);
long digit_count(long number);

//PROBABILITY HELPERS
int flip(float probability, int random_seed, int offset);
int random_index_in_range(int low, int high, int random_seed, int offset);
void report_averager(int iterations, std::string filename, std::string output_name, int precision, int population_size);

//IO HELPERS
int int_choice();
double double_choice();

//TRY-CATCH HELPERS
void try_args_to_int_in_range(char*, int&, int, int, int default_value, std::string error_message);
void try_args_to_double_in_range(char*, double&, int, int, double default_value, std::string error_message);

//VALIDATION
bool validate_real_number(std::string real_number_as_string);
bool validate_int(std::string int_as_string);
bool equals(int, int);

//STRING HELPERS
void string_stream(std::string&, double stream_value, int stream_precision);
void string_stream_fixed(std::string&, double stream_value, int stream_precision);
void set_variant_name(std::string& variant_name, std::string& variant_name_abbreviation, int ga_variant_option);
void set_reporting_str(std::string& reporting_str, int reporting_option);

//MENU HELPERS
void menu_choice_with_dynamic_truncation(int argc, int argc_current, int total_menu_items, int& choice);
void choice_with_dynamic_truncation(bool& complete, int argc, int argc_current, int menu_level, int total_menu_items, int& choice);