/**
 * helpers.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Helper functions for Problem Set 3.
 */
       
#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // TODO: implement a searching algorithm
    if ( n <= 0) 
        return false;
    for(int i = 0; i < n; i++)
        if (value == values[i])
            return true;
    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // TODO: implement an O(n^2) sorting algorithm
    bool ind;
    int k = n - 1;
    do
    {
        ind = false;
        for(int i = 0; i < k; i++)
            if(values[i] > values[i+1])
            {
                int tmp = values[i];
                values[i] = values[i+1];
                values[i+1] = tmp;
                ind = true;
        }
        k--;
    }
    while(ind);
}
