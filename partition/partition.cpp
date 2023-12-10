#include "partition.h"

template <typename T>
int Partition<T>::partition(T a, int pivot, int first_index, int last_index)
{
    int i = first_index;
    int j = last_index;
    
    // CB 10/21/23: THE DATATYPE FOR TEMP SHOULD BE 'T', BUT I'M CHANGING IT FOR THE SAKE OF COMPLETING THE GA HW
    // Tm temp = -1;
    double* temp;

    if(j != pivot)
    {
        temp = a[j];
        a[j] = a[pivot];
        a[pivot] = temp;
        pivot = j;
        j--;
    }
    else
        j--;

    while(a[pivot] < a[j] && !(j < i))
    {
        temp = a[j];
        a[j] = a[pivot];
        a[pivot] = temp;
        j--;
        pivot--;
    }

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
        }
        else
            i++;
    }

    if(a[j] > a[pivot] && !(j < i))
    {
        temp = a[j];
        a[j] = a[pivot];
        a[pivot] = temp;
        pivot = j;
    }
    
    return pivot;

}

template <typename T>
int Partition<T>::partition_2D(T a, int pivot, int first_index, int last_index)
{
    int i = first_index;
    int j = last_index;

    // CB 10/21/23: THE DATATYPE FOR TEMP SHOULD BE 'T', BUT I'M CHANGING IT FOR THE SAKE OF COMPLETING THE GA HW
    // T temp = -1;
    double* temp;

    if(j != pivot)
    {
        temp = a[j];
        a[j] = a[pivot];
        a[pivot] = temp;
        pivot = j;
        j--;
    }
    else
        j--;

    while(a[pivot][0] < a[j][0] && !(j <= i)) // SEGMENT FAULTS IF !(j < i) INSTEAD OF THE GIVEN !(j <= i). WHY? (CB.112123): MAYBE IT SEGMENT FAULTS IN THE NEXT LOOP IF i == j. HAVEN'T VERIFIED, JUST A THOUGHT WHILE REVISITING FOR GA-PROJECT.
    {
        temp = a[j];
        a[j] = a[pivot];
        a[pivot] = temp;
        j--;
        pivot--;
    }

    while(j != i && !(j < i)) // THIS COULD BE REFACTORED TO while(!(j <= i)) (CB.112123)
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
            // COULD ADD i++; TEST AT ANOTHER TIME (CB.112123)
        }
        else
            i++;
    }

    if(a[j][0] > a[pivot][0] && !(j < i))
    {
        temp = a[j];
        a[j] = a[pivot];
        a[pivot] = temp;
        pivot = j;
    }
    
    return pivot;

}

template <typename T>
void Partition<T>::partition_sort(T a, int pivot, int first_index, int last_index)
{
    if(first_index != last_index && pivot >= first_index && pivot <= last_index)
    {
        int previous_pivot = partition(a, pivot, first_index, last_index);
        
        int new_pivot_1 = -1;
        int new_pivot_2 = -1;
 
        if(previous_pivot != first_index)
            new_pivot_1 = random_index_in_range(first_index, previous_pivot, time(NULL), previous_pivot);
        
        if(previous_pivot != last_index)
            new_pivot_2 = random_index_in_range(previous_pivot + 1, last_index + 1, time(NULL), previous_pivot + 1);


        if(previous_pivot != first_index && new_pivot_1 != -1)
            partition_sort(a, new_pivot_1, first_index, (previous_pivot - 1));

        if(previous_pivot != last_index && new_pivot_2 != -1)
            partition_sort(a, new_pivot_2, (previous_pivot + 1), last_index);
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
    if(first_index != last_index && pivot >= first_index && pivot <= last_index)
    {
        int previous_pivot = partition_2D(a, pivot, first_index, last_index);
        
        int new_pivot_1 = -1;
        int new_pivot_2 = -1;
 
        if(previous_pivot != first_index)
            new_pivot_1 = random_index_in_range(first_index, previous_pivot, time(NULL), previous_pivot);
        
        if(previous_pivot != last_index)
            new_pivot_2 = random_index_in_range(previous_pivot + 1, last_index + 1, time(NULL), previous_pivot + 1);


        if(previous_pivot != first_index && new_pivot_1 != -1)
            partition_sort_2D(a, new_pivot_1, first_index, (previous_pivot - 1));

        if(previous_pivot != last_index && new_pivot_2 != -1)
            partition_sort_2D(a, new_pivot_2, (previous_pivot + 1), last_index);
    }
    else
    {
        if(pivot < first_index || pivot > last_index)
            std::cout << "Pivot sort failure: Pivot is out of range." << std::endl;
    }
}

template <typename T>
int Partition<T>::random_index_in_range(int low, int high, int random_seed, int offset)
{
    srand(random_seed + offset);
    return low + rand()%(high - low);
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

// TEST
template <typename T>
void Partition<T>::print_array_with_pivot_2D(T a, int pivot, int first_index, int last_index)
{
    if(first_index == 0)
    {
        for(int i = 0; i < last_index + 1; i++)
        {
            if(i != pivot)
                std::cout << "(" << a[i][0] << "," << a[i][1] << ") ";
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