#pragma once
#include <iostream>
#include <cstdlib>
#include <array>

template <typename T>
class Partition
{
    public:
        int partition(T, int, int, int);
        int partition_2D(T, int, int, int);
        int random_index_in_range(int low, int high, int random_seed, int offset);
        void partition_sort(T a, int pivot, int first_index, int last_index);
        void partition_sort_2D(T a, int pivot, int first_index, int last_index);

        //TEST
        void print_array_with_pivot(T a, int pivot, int first_index, int last_index);
        void print_array_with_pivot_2D(T a, int pivot, int first_index, int last_index);
};