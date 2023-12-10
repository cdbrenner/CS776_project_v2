#include "partition.h"

template <typename T>
int Partition<T>::partition(T a, int pivot, int first_index, int last_index)
{
    int i = first_index;
    int j = last_index;
    int temp = -1;

    // TEST
    // std::cout << "PARTITION FUNCTION" << std::endl;
    // std::cout << "pivot = " << pivot << std::endl;
    // std::cout << "i = first_index = " << i << std::endl;
    // std::cout << "j = last_index = " << j << std::endl;
    // std::cout << "a[j] = a["<<j<<"] = " << a[j] << std::endl;
    // std::cout << "a[pivot] = a["<<pivot<<"] = " << a[pivot] << std::endl;
    // print_array_with_pivot(a, pivot, first_index, last_index);
    // char temp_char;
    // std::cin >> temp_char;

    if(j != pivot)
    {

        // TEST
        // std::cout << "PARTITION FUNCTION: if(j != pivot)" << std::endl;
        
        temp = a[j];
        a[j] = a[pivot];
        a[pivot] = temp;
        pivot = j;
        j--;
    }
    else
        j--;

    // TEST
    // std::cout << "PARTITION FUNCTION: after switching a[[j] with a[pivot]" << std::endl;
    // std::cout << "pivot = " << pivot << std::endl;
    // std::cout << "j = " << j << std::endl;
    // std::cout << "a[j] = a["<<j<<"] = " << a[j] << std::endl;
    // std::cout << "a[pivot] = a["<<pivot<<"] = " << a[pivot] << std::endl << std::endl;
    // print_array_with_pivot(a, pivot, first_index, last_index);
    // std::cin >> temp_char;
 
    while(a[pivot] < a[j] && !(j < i))
    {
        temp = a[j];
        a[j] = a[pivot];
        a[pivot] = temp;
        j--;
        pivot--;
    }

    // TEST
    // std::cout << "PARTITION FUNCTION: after bubbling pivot leftwards" << std::endl;
    // std::cout << "pivot = " << pivot << std::endl;
    // std::cout << "j = " << j << std::endl;
    // std::cout << "a[j] = a["<<j<<"] = " << a[j] << std::endl;
    // std::cout << "a[pivot] = a["<<pivot<<"] = " << a[pivot] << std::endl << std::endl;
    // print_array_with_pivot(a, pivot, first_index, last_index);
    // std::cout << "PARTITION FUNCTION: going into while(j != i):\n" << std::endl;
    // std::cin >> temp_char;

    while(j != i && !(j < i))
    {
        if(a[i] > a[pivot])
        {
            temp = a[j];
            a[j] = a[i];
            a[i] = temp;
            temp = a[j];
            a[j] = a[pivot];
            a[pivot] = temp;
            j--;
            pivot--;

        // TEST
        // std::cout << "if(a[i] > a[pivot]):" << std::endl;
        // std::cout << "pivot = " << pivot << std::endl;
        // std::cout << "j = " << j << std::endl;
        // std::cout << "a[i] = a["<<i<<"] = " << a[i] << std::endl;
        // std::cout << "a[j] = a["<<j<<"] = " << a[j] << std::endl;
        // std::cout << "a[pivot] = a["<<pivot<<"] = " << a[pivot] << std::endl;
        // print_array_with_pivot(a, pivot, first_index, last_index);
        // std::cin >> temp_char;
    
        }
        else
        {
            i++;

            // TEST
            // std::cout << "!!! -> if(a[i] > a[pivot]):" << std::endl;
            // // std::cout << "i_last_to_move = " << i_last_to_move << std::endl;
            // std::cout << "i = " << i << std::endl;
            // std::cout << "a[i] = a["<<i<<"] = " << a[i] << std::endl;
            // std::cout << "a[pivot] = a["<<pivot<<"] = " << a[pivot] << std::endl;
            // print_array_with_pivot(a, pivot, first_index, last_index);
            // std::cin >> temp_char;
        }
    }

    // TEST
    // std::cout << "PARTITION: while loop complete\n\n";

    if(a[j] > a[pivot] && !(j < i))
    {
        temp = a[j];
        a[j] = a[pivot];
        a[pivot] = temp;
        pivot = j;

        // TEST
        // std::cout << "if(i_last_to_move):" << std::endl;
        // std::cout << "pivot = " << pivot << std::endl;
        // std::cout << "j = " << j << std::endl;
        // std::cout << "a[j] = a["<<j<<"] = " << a[j] << std::endl;
        // std::cout << "a[pivot] = a["<<pivot<<"] = " << a[pivot] << std::endl;
        // print_array_with_pivot(a, pivot, first_index, last_index);
        // std::cin >> temp_char;
    
    }

    // TEST
    // std::cout << "PARTITION:: END" << std::endl;
    // print_array_with_pivot(a, pivot, first_index, last_index);
    // std::cout << "return pivot = " << pivot << std::endl << std::endl;
    // std::cin >> temp_char;
    
    return pivot;

}

template <typename T>
int Partition<T>::random_index_in_range(int low, int high, int random_seed, int offset)
{
    srand(random_seed + offset);
    return low + rand()%(high - low);
}

template <typename T>
int Partition<T>::partition_2D(T a, int pivot, int first_index, int last_index)
{
    int i = first_index;
    int j = last_index;
    
    // REAL
    // T temp = -1;
    double* temp;

    // TEST
    std::cout << "PARTITION FUNCTION" << std::endl;
    std::cout << "pivot = " << pivot << std::endl;
    std::cout << "i = first_index = " << i << std::endl;
    std::cout << "j = last_index = " << j << std::endl;
    std::cout << "a[j][0] = a["<<j<<"] = " << a[j][0] << std::endl;
    std::cout << "a[pivot] = a["<<pivot<<"][0] = " << a[pivot][0] << std::endl;
    print_array_with_pivot_2D(a, pivot, first_index, last_index);
    char temp_char;
    std::cin >> temp_char;

    if(j != pivot)
    {

        // TEST
        std::cout << "PARTITION FUNCTION: if(j != pivot)" << std::endl;
        
        temp = a[j];
        a[j] = a[pivot];
        a[pivot] = temp;
        pivot = j;
        j--;
    }
    else
        j--;

    // TEST
    std::cout << "PARTITION FUNCTION: after switching a[[j] with a[pivot]" << std::endl;
    std::cout << "pivot = " << pivot << std::endl;
    std::cout << "i = " << i << std::endl;
    std::cout << "j = " << j << std::endl;
    std::cout << "a[j][0] = a["<<j<<"][0] = " << a[j][0] << std::endl;
    std::cout << "a[pivot][0] = a["<<pivot<<"][0] = " << a[pivot][0] << std::endl << std::endl;
    print_array_with_pivot_2D(a, pivot, first_index, last_index);
    std::cin >> temp_char;
    int while_counter = 1;

    while(a[pivot][0] < a[j][0] && !(j < i))
    {
        // TEST
        std::cout << "PARTITION FUNCTION: while(a[pivot][0] < a[j][0] && !(j < i))" << std::endl;
        std::cout << "iteration = " << while_counter++ << std::endl;
        std::cout << "a[j][0] = " << a[j][0] << std::endl;
        std::cout << "a[pivot][0] = " << a[pivot][0] << std::endl;
        print_array_with_pivot_2D(a, pivot, first_index, last_index);

        temp = a[j];

        std::cout << "temp = a[j]" << std::endl;

        a[j] = a[pivot];

        std::cout << "a[j] = a[pivot]" << std::endl;
         
        a[pivot] = temp;
         
        std::cout << "a[pivot] = temp;" << std::endl;
 
        j--;
         
        std::cout << "j--" << std::endl;
 
        pivot--;
         
        std::cout << "pivot--" << std::endl;
 
    }

    // TEST
    std::cout << "PARTITION FUNCTION: after bubbling pivot leftwards" << std::endl;
    std::cout << "pivot = " << pivot << std::endl;
    std::cout << "j = " << j << std::endl;
    std::cout << "a[j][0] = a["<<j<<"][0] = " << a[j][0] << std::endl;
    std::cout << "a[pivot][0] = a["<<pivot<<"][0] = " << a[pivot][0] << std::endl << std::endl;
    print_array_with_pivot_2D(a, pivot, first_index, last_index);
    std::cout << "PARTITION FUNCTION: going into while(j != i):\n" << std::endl;
    std::cin >> temp_char;

    while(j != i && !(j < i))
    {
        if(a[i][0] > a[pivot][0])
        {
            temp = a[j];
            a[j] = a[i];
            a[i] = temp;
            temp = a[j];
            a[j] = a[pivot];
            a[pivot] = temp;
            j--;
            pivot--;

        // TEST
        std::cout << "if(a[i][0] > a[pivot][0]):" << std::endl;
        std::cout << "pivot = " << pivot << std::endl;
        std::cout << "j = " << j << std::endl;
        std::cout << "a[i][0] = a["<<i<<"][0] = " << a[i][0] << std::endl;
        std::cout << "a[j][0] = a["<<j<<"][0] = " << a[j][0] << std::endl;
        std::cout << "a[pivot][0] = a["<<pivot<<"][0] = " << a[pivot][0] << std::endl;
        print_array_with_pivot_2D(a, pivot, first_index, last_index);
        std::cin >> temp_char;
    
        }
        else
        {
            i++;

            // TEST
            std::cout << "!!! -> if(a[i][0] > a[pivot][0]):" << std::endl;
            // std::cout << "i_last_to_move = " << i_last_to_move << std::endl;
            std::cout << "i = " << i << std::endl;
            std::cout << "a[i][0] = a["<<i<<"][0] = " << a[i][0] << std::endl;
            std::cout << "a[pivot][0] = a["<<pivot<<"][0] = " << a[pivot][0] << std::endl;
            print_array_with_pivot_2D(a, pivot, first_index, last_index);
            std::cin >> temp_char;
        }
    }

    // TEST
    std::cout << "PARTITION: while loop complete\n\n";

    if(a[j][0] > a[pivot][0] && !(j < i))
    {
        temp = a[j];
        a[j] = a[pivot];
        a[pivot] = temp;
        pivot = j;

        // TEST
        std::cout << "if(i_last_to_move):" << std::endl;
        std::cout << "pivot = " << pivot << std::endl;
        std::cout << "j = " << j << std::endl;
        std::cout << "a[j][0] = a["<<j<<"][0] = " << a[j][0] << std::endl;
        std::cout << "a[pivot][0] = a["<<pivot<<"][0] = " << a[pivot][0] << std::endl;
        print_array_with_pivot_2D(a, pivot, first_index, last_index);
        std::cin >> temp_char;
    
    }

    // TEST
    std::cout << "PARTITION:: END" << std::endl;
    print_array_with_pivot_2D(a, pivot, first_index, last_index);
    std::cout << "return pivot = " << pivot << std::endl << std::endl;
    std::cin >> temp_char;
    
    return pivot;

}

template <typename T>
void Partition<T>::partition_sort(T a, int pivot, int first_index, int last_index)
{
    // TEST
    // std::cout << "PARTITION::PARTITION_SORT" << std::endl;
    // std::cout << "pivot = " << pivot << std::endl;
    // std::cout << "first_index = " << first_index << std::endl;
    // std::cout << "last_index = " << last_index << std::endl;
    // std::cout << "working section:" << std::endl;
    // if( first_index != last_index)
    // {
    //     std::cout << "pivot will sort around the following 2 arrays:" << std::endl;
    //     print_array_with_pivot(a, pivot, first_index, last_index);
    // }
    // else
    //     std::cout << "first == last\n\n";
    // char temp_char;
    // std::cin >> temp_char;

    if(first_index != last_index && pivot >= first_index && pivot <= last_index)
    {

        // TEST
        // std::cout << "PARTITION:PARTITION_SORT: Going into partition:" <<std::endl;
 
        int previous_pivot = partition(a, pivot, first_index, last_index);
        
        // TEST
        // std::cout << "PARTITION:PARTITION_SORT: After partitioning:" <<std::endl;
        // print_array_with_pivot(a, previous_pivot, first_index,last_index);
        // std::cin >> temp_char;
        
        int new_pivot_1 = -1;
        int new_pivot_2 = -1;
        if(previous_pivot != first_index)
            new_pivot_1 = random_index_in_range(first_index, previous_pivot, time(NULL), previous_pivot);
        
        if(previous_pivot != last_index)
            new_pivot_2 = random_index_in_range(previous_pivot + 1, last_index + 1, time(NULL), previous_pivot + 1);

        //TEST
        // std::cout << "PARTITION::PARTITION_SORT: after getting new pivots (random indicies)" << std::endl;
        // std::cout << "new_pivot_1 = " << new_pivot_1 << std::endl;
        // std::cout << "new_pivot_2 = " << new_pivot_2 << std::endl;
        // std::cout << std::endl;

        if(previous_pivot != first_index && new_pivot_1 != -1)
        {
            // TEST
            // std::cout << "\n\nGOING INTO LEFT-SIDE PARTITION!!!\n\n\n";

            partition_sort(a, new_pivot_1, first_index, (previous_pivot - 1));
        }
        if(previous_pivot != last_index && new_pivot_2 != -1)
        {
            // TEST
            // std::cout << "\n\nGOING INTO RIGHT-SIDE PARTITION!!!\n\n\n";

            partition_sort(a, new_pivot_2, (previous_pivot + 1), last_index);
        }
    }
    else
    {
        if(pivot < first_index || pivot > last_index)
            std::cout << "Pivot sort failure: Pivot is out of range." << std::endl;
    }
}

template <typename T>
void Partition<T>::partition_sort_2D(T a, int pivot, int first_index, int last_index)
{
    // TEST
    std::cout << "PARTITION::PARTITION_SORT_2D" << std::endl;
    std::cout << "pivot = " << pivot << std::endl;
    std::cout << "first_index = " << first_index << std::endl;
    std::cout << "last_index = " << last_index << std::endl;
    std::cout << "working section:" << std::endl;
    if( first_index != last_index)
    {
        std::cout << "pivot will sort around the following 2 arrays:" << std::endl;
        print_array_with_pivot_2D(a, pivot, first_index, last_index);
    }
    else
        std::cout << "first == last\n\n";
    char temp_char;
    std::cin >> temp_char;

    if(first_index != last_index && pivot >= first_index && pivot <= last_index)
    {

        // TEST
        std::cout << "PARTITION:PARTITION_SORT: Going into partition:" <<std::endl;
 
        int previous_pivot = partition_2D(a, pivot, first_index, last_index);
        
        // TEST
        std::cout << "PARTITION:PARTITION_SORT: After partitioning:" <<std::endl;
        print_array_with_pivot_2D(a, previous_pivot, first_index,last_index);
        std::cin >> temp_char;
        
        int new_pivot_1 = -1;
        int new_pivot_2 = -1;
        if(previous_pivot != first_index)
            new_pivot_1 = random_index_in_range(first_index, previous_pivot, time(NULL), previous_pivot);
        
        if(previous_pivot != last_index)
            new_pivot_2 = random_index_in_range(previous_pivot + 1, last_index + 1, time(NULL), previous_pivot + 1);

        //TEST
        std::cout << "PARTITION::PARTITION_SORT: after getting new pivots (random indicies)" << std::endl;
        std::cout << "new_pivot_1 = " << new_pivot_1 << std::endl;
        std::cout << "new_pivot_2 = " << new_pivot_2 << std::endl;
        std::cout << std::endl;

        if(previous_pivot != first_index && new_pivot_1 != -1)
        {
            // TEST
            std::cout << "\n\nGOING INTO LEFT-SIDE PARTITION!!!\n\n\n";

            partition_sort_2D(a, new_pivot_1, first_index, (previous_pivot - 1));
        }
        if(previous_pivot != last_index && new_pivot_2 != -1)
        {
            // TEST
            std::cout << "\n\nGOING INTO RIGHT-SIDE PARTITION!!!\n\n\n";

            partition_sort_2D(a, new_pivot_2, (previous_pivot + 1), last_index);
        }
    }
    else
    {
        if(pivot < first_index || pivot > last_index)
            std::cout << "Pivot sort failure: Pivot is out of range." << std::endl;
    }
}

// TEST
template <typename T>
void Partition<T>::print_array_with_pivot(T a, int pivot, int first_index, int last_index)
{
    if(first_index == 0)
    {
        for(int i = 0; i < last_index + 1; i++)
        {
            if( i != pivot)
                std::cout << a[i] << " ";
            else
                std::cout << "|| " << a[i] << " || ";
        }
        std::cout << std::endl;
    }
    else
    {
        for(int i = first_index; i < last_index + 1; i++)
        {
            if( i != pivot)
                std::cout << a[i] << " ";
            else
                std::cout << "|| " << a[i] << " || ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

template <typename T>
void Partition<T>::print_array_with_pivot_2D(T a, int pivot, int first_index, int last_index)
{

    // TEST
    // std::cout << "PARTITION::PRINT_2D" << std::endl;
    // std::cout << "a[0][0] = " << a[0][0] << std::endl;
    // std::cout << "a[0][1] = " << a[0][1] << std::endl;
    // char temp_c;
    // std::cin >> temp_c;

    if(first_index == 0)
    {
        for(int i = 0; i < last_index + 1; i++)
        {
            // TEST
            // std::cout << "INSIDE FOR LOOP" << std::endl;
            // std::cout << "pivot = " << pivot << std::endl;
            // std::cout << "a[0][0] = " << a[0][0] << std::endl;
            // std::cout << "a[i][0] = " << a[i][0] << std::endl;
            // std::cin >> temp_c;

            if(i != pivot)
            {
                // TEST
                // std::cout << "INSIDE if(i != pivot)" << std::endl;
                // std::cout << "a[i][0] = " << a[i][0] << std::endl;
                // std::cout << "a[i][1] = " << a[i][1] << std::endl;
                // std::cin >> temp_c;


                std::cout << "(" << a[i][0] << "," << a[i][1] << ") ";
            }
            else
                std::cout << "|| (" << a[i][0] << "," << a[i][1] << ") || ";
        }
        std::cout << std::endl;
    }
    else
    {
        for(int i = first_index; i < last_index + 1; i++)
        {
            if( i != pivot)
                std::cout << "(" << a[i][0] << "," << a[i][1] << ") ";
            else
                std::cout << "|| (" << a[i][0] << "," << a[i][1] << ") || ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}