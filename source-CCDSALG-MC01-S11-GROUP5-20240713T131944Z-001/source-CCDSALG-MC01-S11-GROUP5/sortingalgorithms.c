#ifndef SORTINGALGORITHMS_C
#define SORTINGALGORITHMS_C

#include <stdlib.h>
#include <stdio.h>
#include "record.c"
#include <string.h>

/*
* You may declare additional variables and helper functions
* as needed by the sorting algorithms here.
*/

void insertionSort(Record *arr, int n)
{
    // TODO: Implement this sorting algorithm here.
	int i, j, key;
	char strtemp[500];
	
	for(i = 1; i < n ; i++)
	{
		key = arr[i].idNumber;
		strcpy(strtemp, arr[i].name);
		j = i - 1;
		
		while(j >= 0 && arr[j].idNumber > key)
		{
			arr[j + 1].idNumber = arr[j].idNumber;
			strcpy(arr[j + 1].name, arr[j].name);
			
			j = j - 1;
		}
		arr[j + 1].idNumber = key;
		strcpy(arr[j + 1].name, strtemp);
	}



}

void selectionSort(Record *arr, int n)
{
    // TODO: Implement this sorting algorithm here.
	int i, j, min_idx;
    
    for (i = 0; i < n - 1; i++) 
	{
        // Find the minimum element in unsorted array
        min_idx = i;
        for (j = i + 1; j < n; j++) 
		{
            if (arr[j].idNumber < arr[min_idx].idNumber)
			{
                min_idx = j;
            }
        }
        
        // Swap the found minimum element with the first element
        Record temp = arr[min_idx]; 
        arr[min_idx] = arr[i]; 
        arr[i] = temp;  
    }
}

/*
 * Merges two subarrays of arr[].
 * First subarray is arr[l..m]
 * Second subarray is arr[m+1..r]
 * @param arr Array of Record structures to be merged.
 * @param l Left index of the first subarray.
 * @param m Middle index, marking the end of the first subarray and beginning of the second.
 * @param r Right index of the second subarray.
 */
void merge_sorted_arrays(Record *arr, int l, int m, int r)
{
	int i, j, k;
	
	int left_length = m - l + 1; //Length of left subarray
	int right_length = r - m; //Length of right subarray
	
  	// Dynamically allocate memory for temporary subarrays
	Record *temp_left = malloc(left_length * sizeof(Record));
	Record *temp_right = malloc(right_length * sizeof(Record));
	
	// Storing data to temporary subarrays temp_left[] and temp_right[]
	for (i = 0; i < left_length; i++) 
    	temp_left[i] = arr[l + i];
    	
    for (i = 0; i < right_length; i++)
    	temp_right[i] = arr[m + 1 + i];
    	
    // Merging the temp arrays back into arr[]	
    for (i = 0, j = 0, k = l; k <= r; k++)
    {
    	if ((i < left_length) && (j >= right_length || temp_left[i].idNumber  <= temp_right[j].idNumber))
    	{
      		arr[k] = temp_left[i];
      		i++;
    	}
    	else
    	{
      		arr[k] = temp_right[j];
      		j++;
    	}
	}
	// Free dynamically allocated memory
  	free(temp_left);
    free(temp_right);
}

/*
 * Recursively applies merge sort to sort the subarray arr[left..right]
 * @param arr Array of Record structures to be sorted.
 * @param p Left index of the subarray to be sorted.
 * @param r Right index of the subarray to be sorted.
 */
void mergeSort(Record *arr, int p, int r)
{
	int mid;
	
	if (p < r)
	{
    	mid = p + (r - p) / 2; // Finding midpoint of the array
	
		// Recursively sort first and second halves
    	mergeSort(arr, p, mid);
    	mergeSort(arr, mid + 1, r);
    
  		// Merge the sorted halves
    	merge_sorted_arrays(arr, p, mid, r);
  	}
}

/*
* Define AT LEAST ONE more sorting algorithm here, apart from the
* ones given above. Make sure that the method accepts an array of
* record structures.
*/
void swap(Record *a, Record *b) {
    Record temp = *a;
    *a = *b;
    *b = temp;
}

int partition(Record *arr, int p, int r) {
    int pivotIndex = (p + r) / 2;              // Calculate the index of the middle element
    int pivotValue = arr[pivotIndex].idNumber; // Choose the idNumber of the middle element as the pivot
    int i = p - 1; 							   // Index of smaller element
    										   // Choosing the last index as a pivot doesn't work for reversed datasets
    swap(&arr[pivotIndex], &arr[r]);           // Moves the pivot to the end
    
    int j;
    for (j = p; j < r; j++) {
        if (arr[j].idNumber < pivotValue) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[r]);                // Moves the pivot to its correct position
    return i + 1;
}


void quickSort(Record *arr, int p, int r) {
	
    if (p < r) {
        int q = partition(arr, p, r);
        quickSort(arr, p, q - 1);
        
        quickSort(arr, q + 1, r);
       
    }
   
    
}

#endif
