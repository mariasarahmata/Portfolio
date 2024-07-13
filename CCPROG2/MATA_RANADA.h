/*
	CCPROG2 MP2 header file (.h source code)
    
    Do not forget to encode your name, section and date of submission.
    
    LASTNAME1, FIRSTNAME1: MATA, MARIA SARAH ALTHEA      SECTION1: S11
    LASTNAME2, FIRSTNAME2: RANADA, ARIANNE               SECTION2: S11
    
    DATE SUBMITTED     : NOVEMBER 28, 2023
		
	Rename this file using your last names in alphabetical order, for example CRUZ_TAN.h
		
	Make sure to include your header file in your C source code for MP Part 2.  
*/

/*
    Encode below this comment the following:
    
    1. #define directives
    2. typedef alias(es)for your string(s)
    3. declaration of your user-defined struct data type(s)
    4. typedef alias(es) for your struct data type(s)

    You may have as many typedef as you deem necessary.

    This header should NOT include the codes for any function definitions.  Function definitions
    should be encoded in your C source file only.
*/

#define MAX_TERRITORIES 204
#define COLUMNS 15
#define MAX_NAME_LENGTH 50

typedef char String [MAX_NAME_LENGTH]; //Alias for all string variables that will be used

struct territoryTag {
	String territoryName; //Stores all territory names
	double baseline; //Stores baseline life expectancy values per territory
	double air_pollution; //Stores reduction in life expectancy values due to air_pollution for all territories
	double ambient; //Stores reduction in life expectancy values due to ambient for all territories
	double ozone; //Stores reduction in life expectancy values due to ozone for all territories
	double HAP; //Stores reduction in life expectancy values due to HAP for all territories
	double environmental; //Stores reduction in life expectancy values due to environmental for all territories
	double occupational;  //Stores reduction in life expectancy values due to occupational for all territories
	double unsafe_wash; //Stores reduction in life expectancy values due to unsafe_wash for all territories
	double metabolic; //Stores reduction in life expectancy values due to metabolic for all territories
	double dietary; //Stores reduction in life expectancy values due to dietary for all territories
	double high_plasma; //Stores reduction in life expectancy values due to high_plasma for all territories
	double tobacco; //Stores reduction in life expectancy values due to tobacco for all territories
	double smoking; //Stores reduction in life expectancy values due to smoking for all territories
	double secondhand_smoke; //Stores reduction in life expectancy values due to secondhand_smoke for all territories
	double unsafe_sex; //Stores reduction in life expectancy values due to unsafe_sex for all territories
	
};

typedef struct territoryTag TData; //Alias for struct territoryTag

/* You have to put an inline comment for each struct member as part of the documentation. */

