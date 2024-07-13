/*********************************************************************
	
	This is to certify that this project is my own work, based on my
	own work, based on my personal efforts in studying and applying
	the concepts learned. I have constructed the functions and their
	respective algorithms and corresponding code by myself. The 
	program was run, tested, and debugged by my own efforts. I further
	certify that I have not copied in part or whole or otherwise 
	plagiarized the work of other students and/or persons.
	
								Maria Sarah Althea A. Mata, 12208477
								
*********************************************************************/


/*
	Description: This is a text based program that follows the game 
				 mechanics of the game "Animal Crossing New Horizons".
	Programmed by: Maria Sarah Althea A. Mata S15
	Last Modified: July 30, 2023
	Version: 2.0
	[Acknowledgements: https://www.geeksforgeeks.org/rand-and-srand-in-ccpp/]
	[				 : https://www.tutorialspoint.com/rand-and-srand-in-c]
	[				 : https://www.tutorialspoint.com/header-files-stdio-h-and-stdlib-h-in-c]
	[				 : https://www.geeksforgeeks.org/time-h-header-file-in-c-with-examples/]
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
	This function is a random number generator for the Sunday price and
	it generates random number between 90-110 for Daisy Mae's turnip prices
	@return the random seller price
*/
int sellerPrice() 
{
	int price, i;
	  
	srand(time(0));
	  
	for(i=0; i<1; i++) 
	  {
	    price = (rand() % (110 - 90 + 1)) + 90;
	  }
	
	return price;
}

/*
	This function is a random weekly price trend generator 
	for the daily store prices that generates random number  
	for store prices based on random weekly trends.
	@return the weekly trend price
*/

int weeklyPriceTrend() 
{
	srand(time(NULL));
	  
	return (rand() % (3 - 1 + 1)) + 1;
}

/* 
	This function computes for the Average Trend
	@param sundayPrice is the weekly seller price
	@return the random average trend price
*/
int averageTrend(int sundayPrice) 
{
	int max = sundayPrice + (sundayPrice * 0.05);
	
	srand(time(NULL));
	
	return (rand() % (max - 80 + 1)) + 80;
}

/* 
	This function computes for the Awesome Trend
	@param sundayPrice is the weekly seller price
	@return the random awesome trend price
*/
int awesomeTrend(int sundayPrice) 
{
	int max = sundayPrice * 3;
	
	srand(time(NULL));
	
	return (rand() % (max - sundayPrice + 1)) + sundayPrice;
}

/* 
	This function computes for the Bad Trend
	@param sundayPrice is the weekly seller price
	@return the random bad trend price
*/
int badTrend(int sundayPrice) 
{
	int max = sundayPrice;
	
	srand(time(NULL));
	
	return (rand() % (max - 20 + 1)) + 20;
}

/* 
	This function determines the weekly store prices based on weekly trend
	@param sundayPrice is seller price
	@param trend are trend prices
	@return the current store price
*/
int storePrices(int sundayPrice, int trend) 
{
	int storePrice = 0;
	  
	if (trend == 1) 
	{
		storePrice =  averageTrend(sundayPrice);
	} 
	else if (trend == 2)
	{
		storePrice = awesomeTrend(sundayPrice);
	} 
	else if (trend == 3) 
	{
		storePrice = badTrend(sundayPrice);
	}
	return storePrice;
}
    

/* 
	This function asks the player about their choices or options and makes sure that 
	only characters Y, y, n, N are considered as inputs or else, it will display an 
	invalid choice message and ask again.
	Precondition: input should only be the characters Y, y, N, n
	@return the user choice
*/
char askOption() 
{
	char choice;
	  
	int loop = 1;
	  
	while(loop != 0) 
	{
		scanf(" %c", &choice);
		if(choice == 'Y' || choice == 'y' || choice == 'n' || choice == 'N') 
		{
			loop = 0;
		} 
		
		else
		{
		printf("Invalid choice.\nChoose only from Y/N: ");
		}
	}
	return choice;
}

/* 
	This function asks the player about the quantity of turnip stack they want to buy 
	and makes sure that the number of stacks is not more than what their current bells 
	can afford or is beyond the number of stacks they can buy which is 10.
	Precondition: input should only be positive integer
	@param affordStack is the calculated stalk the user can afford base on current bells
	@return the number of stalk the user to buy
*/
int buyTurnipStack(int affordStack) 
{
	int stack = 0;
	int loop = 1;
	while(loop != 0) 
	{
		scanf("%d", &stack);
		if(stack > affordStack || stack > 10) 
		{
			printf("\nYou are not allowed to buy stack more than you can afford or more than 10 stacks.\n");
			printf("How many turnip stacks do you want to buy? ");
		} 
		else if(stack < 0) 
		{
			printf("\nInvalid input!\n");
		} 
		else 
		{
			loop = 0;
		}
	}
	return stack;
}

/* 
	This function handles the player's selling of stack and it makes sure that 
	they can only sell the number of stack that are currently available to them.
	Precondition: input should only be positive integer
	@param currentStack is the avaiable stacks the user have
	@return the number of stack the user wants to sell
*/
int sellTurnipStack (int currentStack) 
{
	int stack = 0;
	int loop = 1;
  
	while(loop != 0)
	{
		printf("How many turnip stack do you want to sell?: ");
		scanf("%d", &stack);
		
		if(stack > currentStack) 
		{
			printf("\nYou only have %d stack(s) available for selling.\n", currentStack);
		} 
		else if(stack < 0) 
		{
			printf("Invalid input!\n");
		} 
		else 
		{
			loop = 0;
		}
	}
	
	return stack;
	
}

/* 
	This function that checks for spoiled 
	stacks and informs the player if there is.
	@param currentStack is user's available stacks
*/
void spoiledTurnips(int *currentStack)
{
	if(*currentStack > 0) 
	{
		printf("\nSpoiled turnip stacks: %d\n", *currentStack);
		*currentStack = 0;
	}
}


/* Main function */

int main() 
{
	// defining variables needed for this program..
	int sundayPrice, storePrice, week = 1, day = 1, bells = 10000;
	int currentStack = 0, affordStack, orderStack;
	int soldStack, orderStackAmount, trend;
	
	char option;
	
	// runs a loop 10 times that represents 10 weeks.
	while(week <= 10) 
	{
	
		// calling spoiledTurnips() function.
		spoiledTurnips(&currentStack);
		
		// resetting stacks to 0 for each week.
		currentStack = 0;
		
		// taking weekly trend price base.
		// expected return 1, 2, 3
		trend = weeklyPriceTrend();
		      
		// calling sellerPrice() function and assigning the value to sundayPrice variable.
		// these will be the bases for sunday prices value through out the game.
		sundayPrice = sellerPrice();
		
		// check if player bells isn't sufficient now to buy 1 stack. If true, stop the game.
		if(bells < (sundayPrice * 10))
		{
			printf("\nYou do not have enough bells to buy a stack. Game terminating..\n");
			day = 8;
			week = 11;
		}
		
		// runs a loop for 7 times. these represents out days in a week.
		while(day <= 7) 
		{
			printf("\n--------------------------------------------------\n");
			printf("\nWeek #%d Day %d:\n\n", week, day);
			
			// check if day is 1, then it's time for the player to buy turnips.
			if(day == 1) 
			{
			
				// calculate if how many stacks can a player afford to buy.
				affordStack = bells / (sundayPrice * 10);
				printf("Welcome! Today is Sunday.\n\n");
				printf("Daisy Mae is here to sell turnips. \nShe is selling turnips for %d bells.\n", sundayPrice);
				printf("You currently have %d bells.\n", bells);
				printf("\nWould you like to buy? [Y/N]: ");
				
				
				// calls askOption() function and assign value to option variable.
				option = askOption();
				
				// check if player choosen Y or y
				if(option == 'Y' || option == 'y') 
				{
					printf("\nYou can afford to buy %d stacks\n", affordStack);
					printf("How many turnip stack would you like to buy?: ");
					orderStack = buyTurnipStack(affordStack);
					
					// calculate total amount to be paid for ordered stacks.
					orderStackAmount = orderStack * 10 * sundayPrice;
					printf("\nThis is the computation of your purchase\n");
					printf("%d stacks = %d Turnips\n", orderStack, orderStack * 10);
					printf("%d x %d = %d Bells\n", orderStack*10, sundayPrice, orderStackAmount);
					
					// subtract total amount paid to player's bells.
					bells -= orderStackAmount;
					
					// assign number of ordered stacks to our currentStack variable. This will be the bases for selling stacks later in our program.
					currentStack = orderStack;
				}
				else 
				{
					// but if the player decided not to buy a stack, just prints okay then move to next week.
					sundayPrice = sellerPrice();
					printf("Okay.\n");
				}
			}
			else 
			{
				// in case day value is not 1.
				
				// check if player's stack is empty, then inform them that they can't make any selling the next day until they buy new stacks.
				if(currentStack == 0) 
				{
					printf("Your stack of turnips is now is empty. You have nothing to sell this day.\n");	    
				} 
				
				else 
				{
					// if stack is not empty.
					
					// call weeklyPriceTrend() function to get random price trends daily.
					storePrice = storePrices(sundayPrice, trend);
					printf("Welcome to Nook's Cranny!\n");
					printf("\nCurrent Turnip Stack: %d\n\n", currentStack);
					printf("Sunday Price: %d\n", sundayPrice);
					printf("Store Price: %d\n\n", storePrice);
					printf("Do you want to sell? [Y/N]: ");
					
					// ask player if they want to sell a stack this day.
					option = askOption();
					
					// check if player wants to sell this day.
					if(option == 'Y' || option == 'y')
					{
					
						// calling sellTurnipStack() function and assign the value (number of stack the player wants to sell) to soldStack variable.
						soldStack = sellTurnipStack(currentStack);
						// calculate the total value of stacks sold and add it to player's current bells.
						bells += (soldStack * 10) * storePrice;
						// subtract the number of stacks sold to player's current stack.
						currentStack -= soldStack;
					}
				}
			}
			
			// add 1 to day value
			day++;
		}
		
		// checks if it is week 10 already and if the player's total bells is more than or equal to 1M. If yes, congratulate the player and exit the game.
		if(week == 10 && bells >= 1000000)
		{
			printf("\nCONGRATULATIONS!!!\nYou made it to our goal! You are now a millionaire!\nYour total bells now is: %d", bells);
		}
		
		// checks if it is now week 10 and if total player's bells is less than 1M. If not, it displays a message if they want to try again.
		if(week == 10 && bells < 1000000) 
		{
			bells += (soldStack * 10) * storePrice;
			printf("\nYou currently have %d bells. Sorry, you did not make it to our goal.\n", bells);
		}
		
		week++;
		day = 1;
	}
	
	printf("\nThanks for playing...\n\n");
	    
	return 0;
}
