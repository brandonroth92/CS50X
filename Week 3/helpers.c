/**
 * helpers.c
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
    // Only search if length of values array is greater than zero
    if (n > 0)
    {
        // Initialize tracking variables first, last, middle
        int first, last, middle;
        first = 0;
        last = n - 1;
        middle = (first+last) / 2;
        
        // While array length is at least size 1
        while (first <= last)
        {
            // Return true if value found
            if (values[middle] == value)
            {
                return true;
            }
            // If middle value is less than value, value must be in larger half of array
            else if (values[middle] < value)
            {
                first = middle + 1;
            }
            // Else, value must be in lesser half of array
            else
            {
                last = middle - 1;
            }
            middle = (first+last) / 2;
        }
    }
    // Return false if values array is not found or if length is 0
    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // Get max value for use when creating counting array length
    int max = 0;    
    for (int i = 0; i < n; i++)
    {
        max = (values[i] > max) ? values[i] : max;    
    }
    
    // create temp array to store indices (values) from counting array
    int result[n];

    // Initialize counting array and set indices to 0
    int count[max + 1];
    for (int i = 0; i < max + 1; i++)
    {
        count[i] = 0;   
    }
    
    // Add value of 1 to indices which represent a value in the values array   
    for (int i = 0; i < n; i++)
    {
        count[values[i]]++;
    }

    // Modify count array so that each element at each index stores the sum of the previous counts
    for (int i = 1; i < max + 1; i++)
    {
        count[i] = count[i] + count[i-1];
    }
    
    // Fill result array with sorted values
    for (int i = n-1; i >= 0; i--) 
    { 
        result[count[values[i]]-1] = values[i]; 
        count[values[i]]--; 
    } 
    
    // Put sorted values back in original values array
    for (int i = 0; i < n; i++)
    {
        values[i] = result[i];
    }
}