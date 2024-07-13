#include "record.c"
#include "sortingalgorithms.c"
#include "filereader.c"
#include "timer.c"
#include <stdio.h>
#include <string.h>
#include <stdlib.h> // for dynamic memory allocation

#define LENGTH 100

int main() {

    Record *recs = malloc(LENGTH * sizeof(Record));
    if (recs == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    readFile(recs, "random100.txt");
    long startTime, endTime, executionTime;
    int i;
    
    int choice;

    do {
    	printf("==================================\n");
    	printf("---- || SORTING ALGORITHMS || ----\n");
    	printf("==================================\n");
    	printf("Select an option:\n");
        printf("1. Insertion Sort\n");
        printf("2. Selection Sort\n");
        printf("3. Merge Sort\n");
        printf("4. Quick Sort\n");
        printf("5. Exit\n");
        printf("Enter Choice from 1-5: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                startTime = currentTimeMillis();
                insertionSort(recs, LENGTH);
                endTime = currentTimeMillis();
                executionTime = endTime - startTime;

                // For checking only
                for (i = 0; i < LENGTH; i++)
                    printf("%d %s \n", recs[i].idNumber, recs[i].name);
                printf("Execution Time: %ld\n", executionTime);
                printf("---------> End of Sorting <---------\n\n");
                
                break;
            case 2:
            	startTime = currentTimeMillis();
            	selectionSort(recs, LENGTH);
            	endTime = currentTimeMillis();
            	executionTime = endTime - startTime;
            	
            	//For checking only
            	for (i = 0; i < LENGTH; i++)
            		printf("%d %s \n", recs[i].idNumber, recs[i].name);
            	printf("Execution Time: %ld\n", executionTime);
            	printf("---------> End of Sorting <---------\n\n");
            	break;
            case 3:
            	startTime = currentTimeMillis();
            	mergeSort(recs, 0, LENGTH - 1); 
                endTime = currentTimeMillis();
                executionTime = endTime - startTime;

            	// For checking only
                for (i = 0; i < LENGTH; i++)
                    printf("%d %s \n", recs[i].idNumber, recs[i].name);
                printf("Execution Time: %ld\n", executionTime);
                printf("---------> End of Sorting <---------\n\n");
                break;
            case 4:
            	
                startTime = currentTimeMillis();
                quickSort(recs, 0, LENGTH - 1); 
                endTime = currentTimeMillis();
                executionTime = endTime - startTime;

                // For checking only
                for (i = 0; i < LENGTH; i++)
                    printf("%d %s \n", recs[i].idNumber, recs[i].name);
                printf("Execution Time: %ld\n", executionTime);
              
                printf("---------> End of Sorting <---------\n\n");
                break;
            
            case 5:
            	printf("=================================\n");
            	printf("-------------> END <-------------\n");
            	printf("=================================\n");
            	break;
            default:
                printf("Not an option. Enter again\n\n");
                
        }
    }while (choice != 5);
        
    free(recs); // Free dynamically allocated memory
    return 0;
}
