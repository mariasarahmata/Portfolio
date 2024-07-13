/*
	CCPROG2 MP3 SOURCE CODE

    Do not forget to encode your name, section and date of submission.
    
    LASTNAME1, FIRSTNAME1: MATA, MARIA SARAH ALTHEA      SECTION1: S11
    LASTNAME2, FIRSTNAME2: RANADA, ARIANNE               SECTION2: S11
    
    DATE SUBMITTED     : NOVEMBER 28, 2023
		
	Rename this file using your last names in alphabetical order, for example CRUZ_TAN.c
		
	Make sure to compile your C source code with a -Wall directive.  
	   Example: CCPROG2> gcc -Wall LASTNAME1_LASTNAME2.c
	   
	Fix all warnings.  Do NOT run the program if you still have a warning.  You must have
	0 error, 0 warning before running the program.
	   
	DO NOT run your a.exe with input/output redirection anymore because MP3 is
   already supposed to be on text file processing.
   
   You should run your a.exe file simply as follows:
      Example: a 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// include your own header file
#include "MATA_RANADA.h"   

/* Do NOT include other header files. */ 

/* 
	HARD REQUIREMENTS: NON-COMPLIANCE WILL MAKE YOUR SOLUTION INCORRECT or CAUSE POINT DEDUCTIONS!
	
	1. You must apply struct data type, and text file processing MP Part 3 (MP3).
       a. Re-use the contents of your header file LASTNAME1_LASTNAME2.h.
       b. Follow the same hard requirements in MP2 on struct data type, and structure access using 
          * and ., and -> operators.
   2. Data should be read from SoGA_DATASET.txt using fscanf().
   3. The main() function should call the appropriate C function, and then call fprintf() to 
      write the answer to each question onto the OUTPUT_LASTNAME1_LASTNAME2.txt file. Numeric 
      answers with double data type must be written with 6 digits after the decimal point.  
   4. There should NOT be any printf() and scanf() statement in the entire source code, 
      not even in main(), and in the function that reads the SoGA data text file.  
   5. Document your codes with SENSIBLE comments.
   6.Use double data type (not float) for all floating point values/variables/functions/return type.
   7. Do NOT use any global variables.
   8. Do NOT use library functions that were NOT discussed in our class.
*/



/*
    ALL typedef declarations must be encoded in your header file.  
    All #define must be encoded in your header file.
    Do NOT place them in this file.
*/


/*
    Define any function that you need below this comment.  

    For functions that serve as answers to the questions, include the following as comments
    BEFORE the function definition itself:
       a. Question number, and the actual question.
       b. Thereafter, type the answer number, and the corresponding expected answer.
    
    For example: 
      Q1:  Which country has the lowest baseline life expectancy?
      A1:  Lesotho

    The following should be included as comments and appear BEFORE each function definition:
       a. Purpose: describe succintly what the function is supposed to accomplish.
       b. Parameters: describe succintly the nature/purpose of each parameter.
       c. Return type: if the function is not void, describe succintly what it will return as a result.          
       
    Document the body of the function by including comments alongside codes that you think
    may not be easy to understand to the person reading the codes. 
*/

/*-----------------------------------------------------------------------
    Purpose: Reads the data from the dataset into an array of TData structs.
    @param territoryNames[] is an array of TData structs to store the data.
    @param FILE *fp is used to read data from a file.
    @return The total number of rows entered by the dataset.
-----------------------------------------------------------------------*/
int Initialize(TData territoryNames[], FILE *fp)
{
	TData temp;
	int ctr = 0;
	
	while (fscanf(fp, "%s %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
				temp.territoryName,
				&temp.baseline,
				&temp.air_pollution,
				&temp.ambient,
				&temp.ozone,
				&temp.HAP,
				&temp.environmental,
				&temp.occupational,
				&temp.unsafe_wash,
				&temp.metabolic,
				&temp.dietary,
				&temp.high_plasma,
				&temp.tobacco,
				&temp.smoking,
				&temp.secondhand_smoke,
				&temp.unsafe_sex) == 16) {
					
		territoryNames[ctr] = temp;
		ctr++;	
	}
	
	return ctr; // Total number of rows entered
}

/*-----------------------------------------------------------------------
    Purpose: Sorts rows of TData structs in ascending order according to reduction in life expectancy due to tobacco 
    @param territoryNames is an array of TData structs to be sorted.
    @param n is the number of elements in the territoryNames array
-----------------------------------------------------------------------*/
void Sort_Ascending_Tobacco(TData territoryNames[], int n) 
{
	TData temp; //Temporary variable for swapping
	int i, j, min;
	
	//Sorts dataset in ascending order according to change in life expectancy due to tobacco	
	for(i = 0; i < n - 1; i++)
	{
		min = i;
		
		for(j = i + 1; j < n; j++)
			if(territoryNames[min].tobacco > territoryNames[j].tobacco)
				min = j;
		
		if(i != min)
		{
			temp = territoryNames[i];
			territoryNames[i] = territoryNames[min];
			territoryNames[min] = temp;
		}
	}
}

/*-----------------------------------------------------------------------
    Purpose: Sorts rows of TData structs alphabetically by territory names
    @param territoryNames is an array of TData structs to be sorted.
    @param n is the number of elements in the territoryNames array
-----------------------------------------------------------------------*/
void Sort_Alphabetical(TData territoryNames[], int n) 
{
    int i, j, min;
    TData temp; // Temporary variable for swapping.
    
    for (i = 0; i < n - 1; i++) 
	{
        min = i;
        
        for (j = i + 1; j < n; j++) 
            if (strcmp(territoryNames[min].territoryName, territoryNames[j].territoryName) > 0)
                min = j;
        
        if (i != min) 
		{	// Swap the territories using a temporary struct.
            temp = territoryNames[i];
            territoryNames[i] = territoryNames[min];
            territoryNames[min] = temp;
        }
    }
}

/*-----------------------------------------------------------------------
    Purpose: Performs a binary search to find a key in an array of TData structs
    @param key is the territory name to search for
    @param territoryNames is an array of TData structs to search in
    @param n is the number of elements in the territoryNames array
    return the index where the key was found, or -1 if not found
-----------------------------------------------------------------------*/
int BinarySearch (String key, TData territoryNames[], int n) 
{
    int low = 0;
    int high = n - 1;
    int mid;
    int found = 0; // Default return value for 'not found'.

    while(!found && low <= high) 
	{
        mid = (low + high) / 2;
        int cmp = strcmp(key, territoryNames[mid].territoryName);
        
        // Found the key
		if (cmp == 0) 
            found = 1;
        
        // Key is less than the middle element, search lower half
		else if (cmp == -1) 
            high = mid - 1;
        
        // Key is greater than the middle element, search upper half
			else 
            	low = mid + 1;
    }
    
    if (found)
    	return mid; //Returns the index number where the key was found
    else 
		return -1; //Not Found
}

/*-----------------------------------------------------------------------
	Q1: Which location has the highest base line life expectancy? Display the name of 
	the location and the corresponding life expectancy. [Maximum]
	
	A1: Singapore, 84.565300
	
    Purpose: Finds the index of the territory with the highest baseline life expectancy
    @param territoryNames is an array of TData structs containing life expectancy data
    @param n is the number of elements in the territoryNames array
    return the index of the territory with the highest life expectancy
-----------------------------------------------------------------------*/
int Q1(TData territoryNames[], int n) 
{
	int i;
    int maxIndex = 0; // Assume first index has the maximum life expectancy
    for (i = 1; i < n; i++) 
        // Compare life expectancy stored at index 0 in the data array of the struct
        if (territoryNames[i].baseline > territoryNames[maxIndex].baseline) 
            maxIndex = i; // Found a new maximum, update the index

    return maxIndex; // Return the index of the territory with the highest life expectancy
}

/*-----------------------------------------------------------------------
	Q2: What is the baseline life expectancy for <paramater_territory_name>? [Linear Search]
	
	A2:
	---> If parameter_Serbia
		: Serbia, 75.651801

	---> If parameter_Philippines
		: Philippines, 71.798423

	---> If parameter_Prog3CutiePie
		Territory 'Prog3CutiePie' not found.
	
    Purpose: Finds the baseline life expectancy for a given territory
    @param territory_name is the name of the territory to search for
    @param territoryNames is an array of TData structs containing life expectancy data
    @param n is the number of elements in the territoryNames array
    return the baseline life expectancy for the specified territory or -1 if not found
-----------------------------------------------------------------------*/
double Q2(String territory_name, TData territoryNames[], int n) 
{
	int i;
	
    for (i = 0; i < n; i++) 
	{
        if (strcmp((territoryNames + i)->territoryName, territory_name) == 0) 
		{
            return (territoryNames + i)->baseline; // Return the baseline life expectancy.
        }
    }
    return -1; // Return -1 if not found.
}

/*-----------------------------------------------------------------------
	Q3: How many territories have at least <parameter_value> year/s change in life 
	expectancy due to tobacco?[Count]
	
	A3: 
	---> If parameter_3.000000
		: 9

	---> If parameter_-1.000000
		: 203
	
	---> If parameter_0.450000
		: 200

	---> If parameter_100.000000
		: 0

	---> If parameter_0.000000
		: 203
	
    Purpose: Counts the number of territories that satisfies the criteria given parameter_value
    @param territoryNames is an array of TData structs containing life expectancy data
    @param n is the number of territories in the territoryNames array
    @param param_value is the minimum threshold that a territory must meet to be considered in the count
    returns total number of territories that satisfies given parameter_value
-----------------------------------------------------------------------*/
int Q3(TData territoryNames[], int n, double param_value)
{
	int count = 0; //Initialize counter
	int i; //Loops counter
	
	for (i = 0; i < n; i++)
		if (territoryNames[i].tobacco >= param_value)
			count++; //Increments whenever the value meets the specified amount of year/s
			
	return count; //Return total count of territories
}

/*-----------------------------------------------------------------------
	Q4: Which territories have the value that falls between the range of <parameter_value> year/s and 
	<parameter_value> year/s change in life expectancy due to tobacco? List only three territories 
	starting from the highest value. [Selection Sort]
	
	A4:

	---> If <parameter_1.400000> && <parameter_1.600000>
		(1) Tajikistan, 1.568172
		(2) Uzbekistan, 1.554100
		(3) Saudi_Arabia, 1.541952

	---> If <parameter_2.900000> && <parameter_3.000000>
		(1) Hungary, 2.995802
		(2) No territory found within specified range, -888.000000
		(3) No territory found within specified range, -888.000000

	---> If <parameter_-1.000000> && <parameter_1.000000>
		(1) Mexico, 0.978727
		(2) Togo, 0.973486
		(3) Sierra_Leone, 0.969914

	---> If <parameter_0.000000> && <parameter_0.200000>
		(1) No territory found within specified range, -888.000000
		(2) No territory found within specified range, -888.000000
		(3) No territory found within specified range, -888.000000
	
    Purpose: Find the top 3 territories within the specified range of tobacco change in life expectancy
    @param territoryNames is an array of TData structs containing territory data
    @param n is the number of territories in the territoryNames array
    @param lower_bound is the lower bound of the range for the tobacco change
    @param upper_bound is the upper bound of the range for the tobacco change
    @param top_three is an array of TData structs to store the top 3 territories found
-----------------------------------------------------------------------*/
void Q4(TData territoryNames[], int n, double lower_bound, double upper_bound, TData top_three[]) 
{
	double change; //Change in life expectancy value due to tobacco
	int ctr = 0; //Sets condition that only a max of 3 values is stored
	int i; //Loop counter
	
	for(i = 0; i < 3; i++) //Inittialize default values if territory is not within specified range
	{
		(*(top_three + i)).tobacco = -888;
		strcpy((*(top_three + i)).territoryName, "No territory found within specified range");
	}
	
	for (i = n - 1; i >= 0; i--) //Start from bottom since dataset is arranged in ascending order
	{	
		if (ctr == 3)
			break;
			
		change = (*(territoryNames + i)).tobacco;
		
        if (change > lower_bound && change < upper_bound && ctr < 3) 
		{
			strcpy((*(top_three + ctr)).territoryName, (*(territoryNames + i)).territoryName);
			(*(top_three + ctr)).tobacco = (*(territoryNames + i)).tobacco;
        	ctr++; //Limits the storage count of top_three to max of 3 only
        }
	}
}

/*-----------------------------------------------------------------------
	Q5: What is the change in life expectancy due to metabolic factor for <paramater_territory_name> 
	and <parameter_territory_name>? Compare which one is higher. [Binary Search after sort]
	
	A5:

	---> If parameter_Australia and parameter_Samoa
		: Australia has 3.790678 which is less than Samoa with 8.453508

	---> If parameter_Samoa and parameter_Australia
		: Australia has 3.790678 which is less than Samoa with 8.453508

	---> If parameter_Philippines and parameter_Prog3Cutie
		: No Answer. Territory 'Philippines' was found but Territory 'Prog3Cutie' was NOT found

	---> If parameter_Rawr and parameter_Meow
		: No Answer. Both Territory 'Rawr' and Territory 'Meow' were NOT found.

	---> If parameter_Japan and parameter_Japan
		: Both Japan and Japan have 2.546600
	
    Purpose: Compares the reduction of life expectancy values between 2 given territories
    @param territoryNames is an array of TData structs containing life expectancy data
    @param n is the number of territories in the territoryNames array
    @param paramater_territory_name[] contains the territories to be searched for
    @parama compare_q5[] is an array of TData struct that stores territories' names and metabolic factor values if found in the dataset
    @param checker updates its value given a condition when one or both parameter territories was not found and vice versa
-----------------------------------------------------------------------*/
void Q5(TData territoryNames[], int n, String paramater_territory_name[], TData compare_q5[], int *checker)
{
	TData temp; //Temp variable for swapping
	String temp_string; //Temp variable for swapping
	int indexFound[2]; //Stores the index numbers where the parameters were found
	
	if (BinarySearch(paramater_territory_name[0], territoryNames, n) != -1 && 
		BinarySearch(paramater_territory_name[1], territoryNames, n) != -1) //Both territories were found
	{	//Store the indexes where the parameters were found
		indexFound[0] = BinarySearch(paramater_territory_name[0], territoryNames, n);
		indexFound[1] = BinarySearch(paramater_territory_name[1], territoryNames, n);
		//Store in array struct
		strcpy(compare_q5[0].territoryName, territoryNames[indexFound[0]].territoryName);
		strcpy(compare_q5[1].territoryName, territoryNames[indexFound[1]].territoryName);
		
		compare_q5[0].metabolic = territoryNames[indexFound[0]].metabolic;
		compare_q5[1].metabolic = territoryNames[indexFound[1]].metabolic;
		
		if(compare_q5[0].metabolic > compare_q5[1].metabolic || compare_q5[0].metabolic < compare_q5[1].metabolic) //Check if values are not equal
		{
			*checker = 1; //Updates value of variable checker
			
			//If the 1st territory is greater, switch their index places. Territory with lesser value is to be saved in the 1st index and printed first
			if (compare_q5[0].metabolic > compare_q5[1].metabolic)
			{	//Swap index positions
				temp = compare_q5[1];
				compare_q5[1] = compare_q5[0];
				compare_q5[0] = temp;
			}
		}
		
		else //Both territories have the same change in life expectancy
			*checker = 2; //Updates value of variable checker
	}
	
	else if (BinarySearch(paramater_territory_name[0], territoryNames, n) == -1 && 
			 BinarySearch(paramater_territory_name[1], territoryNames, n) == -1) //Both territories are not found
				*checker = -1; //Updates value of variable checker
				
		else //Only 1 territory was not found
			 {
			 	if (BinarySearch(paramater_territory_name[0], territoryNames, n) == -1) //First paramater_territory_name was not found
			 	{	//Swap the positions of paramater_territory_name. The territory that was found will be saved in the 1st index as it will be printed first
			 		strcpy(temp_string, paramater_territory_name[1]);
				 	strcpy(paramater_territory_name[1], paramater_territory_name[0]);
				 	strcpy(paramater_territory_name[0], temp_string);
				 }
				 
				 *checker = 0; //Updates value of variable checker
			 }
}

/*-----------------------------------------------------------------------
	Purpose: Orchestrates the flow of the program, providing the necessary
			 inputs to functions, and presenting the output from these
			 functions in an understandable format to the user.
-----------------------------------------------------------------------*/
int
main()
{
    /* Declare your own local variables. Describe the purpose of your local variables. */
    TData territoryNames[MAX_TERRITORIES]; // Declare an array of TData Structs
    TData top_three[3]; // Declare an array to store the top 3 territories
    TData compare_q5[2]; // Struct array to store the values to be used for Q5
	String param_territory; // Parameter to be searched for Q2
	String param_territories_name[2]; // Parameters to be searched for Q5		
	double life_expectancy; // Stores the baseline life expectancy value to be used for Q2
	double param_value; // Parameter value in life expectancy to be used for Q3						
	double lower_bound; // Define the lower bound for a range to be used for Q4
    double upper_bound; // Define the upper bound for a range to be used for Q4
    int num_territories; // Declare a variable to store the total number of territories
    int highestIndex; // Stores index value where territory with highest baseline was found to be used for Q1
	int territory_count; // Stores total number of territories that satisfies criteria for Q3
	int checker = 0; // Sets which condition was met for Q5;
	int i; //Loop counter
    FILE *fp; //Pointer to a FILE object for reading data from a file
    FILE *output_fp; //Pointer to a FILE object for writing output data to a file
    
    /* 
       Call the function that answers a question. Thereafter, use fprintf() NOT printf()
       to output the the question and the corresponding answer onto OUTPUT_LASTNAME1_LASTNAME2.TXT.
       
       expectancy = Q1_Answer(territory, param_x, param_y); // where expectancy, territory, param_x and param_y are local variables.

       fprintf(fp, "Q1: Which territory has the lowest baseline life expectancy?\n");
       fprintf(fp, "A1: %s %lf\n", territory, expectancy);       
    
    */
    
    
    fp = fopen("Soga_DATASET.txt", "r"); // Open input file
    if (fp == NULL)
    {
    	fprintf(stderr, "Error opening input file");
    	exit (1); // Terminate the program abruptly
	}
    
    output_fp = fopen("OUTPUT_MATA_RANADA.txt", "w"); // Open output file
	
	num_territories = Initialize(territoryNames, fp);
 
 /*----------------------------
    #1 Question and Answer
-----------------------------*/ 
  
    // Find the territory with the highest life expectancy.
    highestIndex = Q1(territoryNames, num_territories);
    fprintf(output_fp, "Q1: Which location has the highest baseline life expectancy? Display the name of the location and the corresponding life expectancy. [Maximum]\n");
   	fprintf(output_fp, "\nA1: %s, %.6lf\n",
           territoryNames[highestIndex].territoryName, territoryNames[highestIndex].baseline); //Prints answer

/*----------------------------
    #2 Question and Answer
-----------------------------*/ 
           
    // Find the life expectancy for the given territory using linear search.
    fprintf(output_fp, "\nQ2: What is the baseline life expectancy for <paramater_territory_name>? [Linear Search]\n");
    fprintf(output_fp, "\nA2:\n");
    
    //TEST CASE #1
    strcpy(param_territory, "Serbia"); //Initialize territory to be searched for
    fprintf(output_fp, "\n---> If parameter_%s\n", param_territory);
    
    life_expectancy = Q2(param_territory, territoryNames, num_territories); //Stoes life expectancy value
	if (life_expectancy != -1) // Param_territory was found
        fprintf(output_fp, "\n: %s, %.6lf\n", param_territory, life_expectancy);
    
	else // Param_territory was not found
        fprintf(output_fp, "Territory '%s' not found.\n", param_territory);
    
    //TEST CASE #2
    strcpy(param_territory, "Philippines"); //Initialize territory to be searched for
    fprintf(output_fp, "\n---> If parameter_%s\n", param_territory);
    
    life_expectancy = Q2(param_territory, territoryNames, num_territories); //Stoes life expectancy value
	if (life_expectancy != -1) // Param_territory was found
        fprintf(output_fp, "\n: %s, %.6lf\n", param_territory, life_expectancy);
    
	else // Param_territory was not found
        fprintf(output_fp, "Territory '%s' not found.\n", param_territory);
     
	//TEST CASE #3  
    strcpy(param_territory, "Prog3CutiePie"); //Initialize territory to be searched for
    fprintf(output_fp, "\n---> If parameter_%s\n", param_territory);
    
    life_expectancy = Q2(param_territory, territoryNames, num_territories); //Stoes life expectancy value
	if (life_expectancy != -1) // Param_territory was found
        fprintf(output_fp, "\n: %s, %.6lf\n", param_territory, life_expectancy);
    
	else // Param_territory was not found
        fprintf(output_fp, "\nTerritory '%s' not found.\n", param_territory);
        
/*----------------------------
    #3 Question and Answer
-----------------------------*/  

	// Find the number of territories that satisfies the given minimum parameter value in life expectancy due to tobacco
	fprintf(output_fp, "\nQ3: How many territories have at least <parameter_value> year/s change in life expectancy due to tobacco? [Count]\n");
	fprintf(output_fp, "\nA3:\n");
	
	//TEST CASE #1
	param_value = 3; //Initialize
	fprintf(output_fp, "\n---> If parameter_%.6lf\n", param_value);
	territory_count = Q3(territoryNames, num_territories, param_value); //Gets total count of territories
	fprintf(output_fp, "\n: %d\n", territory_count); //Prints total number of territories that satisfies criteria

	//TEST CASE #2
	param_value = -1; //Initialize
	fprintf(output_fp, "\n---> If parameter_%.6lf\n", param_value);
	territory_count = Q3(territoryNames, num_territories, param_value); //Gets total count of territories
	fprintf(output_fp, "\n: %d\n", territory_count); //Prints total number of territories that satisfies criteria
	
	//TEST CASE #3
	param_value = 0.45; //Initialize
	fprintf(output_fp, "\n---> If parameter_%.6lf\n", param_value);
	territory_count = Q3(territoryNames, num_territories, param_value); //Gets total count of territories
	fprintf(output_fp, "\n: %d\n", territory_count); //Prints total number of territories that satisfies criteria

	//TEST CASE #4
	param_value = 100; //Initialize
	fprintf(output_fp, "\n---> If parameter_%.6lf\n", param_value);
	territory_count = Q3(territoryNames, num_territories, param_value); //Gets total count of territories
	fprintf(output_fp, "\n: %d\n", territory_count); //Prints total number of territories that satisfies criteria
	
	//TEST CASE #5
	param_value = 0; //Initialize
	fprintf(output_fp, "\n---> If parameter_%.6lf\n", param_value);
	territory_count = Q3(territoryNames, num_territories, param_value); //Gets total count of territories
	fprintf(output_fp, "\n: %d\n", territory_count); //Prints total number of territories that satisfies criteria
	
/*----------------------------
    #4 Question and Answer
-----------------------------*/  
   
   	// Find the top 3 territories within the specified range of tobacco change in life expectancy
   	fprintf(output_fp, "\nQ4: Which territories have the value that falls between the range of <parameter_value> year/s and <parameter_value> year/s change in life "
	   		"expectancy due to tobacco? List only three territories starting from the highest value. [Selection Sort]\n");
    fprintf(output_fp, "\nA4:\n");
    // Call the sorting function.
    Sort_Ascending_Tobacco(territoryNames, num_territories);
    
    //TEST CASE #1
    lower_bound = 1.40;
    upper_bound = 1.6;
    fprintf(output_fp, "\n---> If <parameter_%.6lf> && <parameter_%.6lf>\n\n", lower_bound, upper_bound);
    
    Q4(territoryNames, num_territories, lower_bound, upper_bound, top_three);
    // Print out the top three territories
	for (i = 0; i < 3; i++) 
        fprintf(output_fp, "(%d) %s, %.6lf\n", i + 1, top_three[i].territoryName, top_three[i].tobacco);

    //TEST CASE #2
    lower_bound = 2.9;
    upper_bound = 3;
    fprintf(output_fp, "\n---> If <parameter_%.6lf> && <parameter_%.6lf>\n\n", lower_bound, upper_bound);
    
    Q4(territoryNames, num_territories, lower_bound, upper_bound, top_three);
    // Print out the top three territories
	for (i = 0; i < 3; i++) 
        fprintf(output_fp, "(%d) %s, %.6lf\n", i + 1, top_three[i].territoryName, top_three[i].tobacco);

    //TEST CASE #3
    lower_bound = -1;
    upper_bound = 1;
    fprintf(output_fp, "\n---> If <parameter_%.6lf> && <parameter_%.6lf>\n\n", lower_bound, upper_bound);
    
    Q4(territoryNames, num_territories, lower_bound, upper_bound, top_three);
    // Print out the top three territories
	for (i = 0; i < 3; i++) 
        fprintf(output_fp, "(%d) %s, %.6lf\n", i + 1, top_three[i].territoryName, top_three[i].tobacco);

    //TEST CASE #4
    lower_bound = 0;
    upper_bound = 0.2;
    fprintf(output_fp, "\n---> If <parameter_%.6lf> && <parameter_%.6lf>\n\n", lower_bound, upper_bound);
    
    Q4(territoryNames, num_territories, lower_bound, upper_bound, top_three);
    // Print out the top three territories
	for (i = 0; i < 3; i++) 
        fprintf(output_fp, "(%d) %s, %.6lf\n", i + 1, top_three[i].territoryName, top_three[i].tobacco);
        
/*----------------------------
    #5 Question and Answer
-----------------------------*/

	// Search and compare the change in life expectancy of the 2 parameters due to metabolic factor
	fprintf(output_fp, "\nQ5: What is the change in life expectancy due to metabolic factor for <paramater_territory_name> and <paramater_territory_name>? "
			"Compare which one is higher. [Binary Search after sort]\n");
	fprintf(output_fp, "\nA5:\n");
	
	// Call the sorting function.
    Sort_Alphabetical(territoryNames, num_territories);
    
	//TEST CASE #1
	strcpy(param_territories_name[0], "Australia");
	strcpy(param_territories_name[1], "Samoa");
	fprintf(output_fp, "\n---> If parameter_%s and parameter_%s\n", param_territories_name[0], param_territories_name[1]);
	
	Q5(territoryNames, num_territories, param_territories_name, compare_q5, &checker);
		
	if (checker == 1) //Both given territory parameters were found
		fprintf(output_fp, "\n: %s has %.6lf which is less than %s with %.6lf\n", compare_q5[0].territoryName, compare_q5[0].metabolic, compare_q5[1].territoryName, compare_q5[1].metabolic);
		
	else if (checker == 2) //Both given territory parameters were found and have the same value
			fprintf(output_fp, "\n: Both %s and %s have %.6lf\n", compare_q5[0].territoryName, compare_q5[1].territoryName, compare_q5[0].metabolic);
			
		else if (!checker) //At least one parameter terriory was not found
				fprintf(output_fp, "\n: No Answer. Territory '%s' was found but Territory '%s' was NOT found\n", param_territories_name[0], param_territories_name[1]);

			else //Both parameter terriories were not found
				fprintf(output_fp, "\n: No Answer. Both Territory '%s' and Territory '%s' were NOT found.\n", param_territories_name[0], param_territories_name[1]);
			
	//TEST CASE #2
	strcpy(param_territories_name[0], "Samoa");
	strcpy(param_territories_name[1], "Australia");
	fprintf(output_fp, "\n---> If parameter_%s and parameter_%s\n", param_territories_name[0], param_territories_name[1]);
	
	Q5(territoryNames, num_territories, param_territories_name, compare_q5, &checker);
		
	if (checker == 1) //Both given territory parameters were found
		fprintf(output_fp, "\n: %s has %.6lf which is less than %s with %.6lf\n", compare_q5[0].territoryName, compare_q5[0].metabolic, compare_q5[1].territoryName, compare_q5[1].metabolic);
		
	else if (checker == 2) //Both given territory parameters were found and have the same value
			fprintf(output_fp, "\n: Both %s and %s have %.6lf\n", compare_q5[0].territoryName, compare_q5[1].territoryName, compare_q5[0].metabolic);
			
		else if (!checker) //At least one parameter terriory was not found
				fprintf(output_fp, "\n: No Answer. Territory '%s' was found but Territory '%s' was NOT found\n", param_territories_name[0], param_territories_name[1]);

			else //Both parameter terriories were not found
				fprintf(output_fp, "\n: No Answer. Both Territory '%s' and Territory '%s' were NOT found.\n", param_territories_name[0], param_territories_name[1]);
			
	//TEST CASE #3
	strcpy(param_territories_name[0], "Prog3Cutie");
	strcpy(param_territories_name[1], "Philippines");
	fprintf(output_fp, "\n---> If parameter_%s and parameter_%s\n", param_territories_name[0], param_territories_name[1]);
	
	Q5(territoryNames, num_territories, param_territories_name, compare_q5, &checker);
		
	if (checker == 1)
		fprintf(output_fp, "\n: %s has %.6lf which is less than %s with %.6lf\n", compare_q5[0].territoryName, compare_q5[0].metabolic, compare_q5[1].territoryName, compare_q5[1].metabolic);
		
	else if (checker == 2)
			fprintf(output_fp, "\n: Both %s and %s have %.6lf\n", compare_q5[0].territoryName, compare_q5[1].territoryName, compare_q5[0].metabolic);
			
		else if (!checker)
				fprintf(output_fp, "\n: No Answer. Territory '%s' was found but Territory '%s' was NOT found\n", param_territories_name[0], param_territories_name[1]);

			else
				fprintf(stdout, "\n: No Answer. Both Territory '%s' and Territory '%s' were NOT found.\n", param_territories_name[0], param_territories_name[1]);
			
	//TEST CASE #4
	strcpy(param_territories_name[0], "Rawr");
	strcpy(param_territories_name[1], "Meow");
	fprintf(output_fp, "\n---> If parameter_%s and parameter_%s\n", param_territories_name[0], param_territories_name[1]);
	
	Q5(territoryNames, num_territories, param_territories_name, compare_q5, &checker);
		
	if (checker == 1)
		fprintf(output_fp, "\n: %s has %.6lf which is less than %s with %.6lf\n", compare_q5[0].territoryName, compare_q5[0].metabolic, compare_q5[1].territoryName, compare_q5[1].metabolic);
		
	else if (checker == 2)
			fprintf(output_fp, "\n: Both %s and %s have %.6lf\n", compare_q5[0].territoryName, compare_q5[1].territoryName, compare_q5[0].metabolic);
			
		else if (!checker)
				fprintf(output_fp, "\n: No Answer. Territory '%s' was found but Territory '%s' was NOT found\n", param_territories_name[0], param_territories_name[1]);

			else
				fprintf(output_fp, "\n: No Answer. Both Territory '%s' and Territory '%s' were NOT found.\n", param_territories_name[0], param_territories_name[1]);

	//TEST CASE #5
	strcpy(param_territories_name[0], "Japan");
	strcpy(param_territories_name[1], "Japan");
	fprintf(output_fp, "\n---> If parameter_%s and parameter_%s\n", param_territories_name[0], param_territories_name[1]);
	
	Q5(territoryNames, num_territories, param_territories_name, compare_q5, &checker);
		
	if (checker == 1)
		fprintf(output_fp, "\n: %s has %.6lf which is less than %s with %.6lf\n", compare_q5[0].territoryName, compare_q5[0].metabolic, compare_q5[1].territoryName, compare_q5[1].metabolic);
		
	else if (checker == 2)
			fprintf(output_fp, "\n: Both %s and %s have %.6lf\n", compare_q5[0].territoryName, compare_q5[1].territoryName, compare_q5[0].metabolic);
			
		else if (!checker)
				fprintf(output_fp, "\n: No Answer. Territory '%s' was found but Territory '%s' was NOT found\n", param_territories_name[0], param_territories_name[1]);

			else
				fprintf(output_fp, "\n: No Answer. Both Territory '%s' and Territory '%s' were NOT found.\n", param_territories_name[0], param_territories_name[1]);		
	
   /* Don't forget to call fclose() before the return statement! */
	fclose(fp); // Close the input file
    fclose(output_fp); // Close the output file

    return 0; // Return 0 to indicate successful execution

}
