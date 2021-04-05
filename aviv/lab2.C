/* lab2.c - xmain, findDiv */

#include <conf.h>
#include <kernel.h>

/* global paremeters sherd by all proc */
int primaryNum = 0, pid, processesNum;						 //pid that win the race , primary num is a flag
int mainsem, functionSem;			 //main Sem is going to go from 1-numOfProc
unsigned long int divisor, winning_starter, num; // divisor is what the find div function look for

xmain()
{
	//find div dicliration
	int findDiv();

	//parameters
	int starter, i;
	char process_name[] = "process_?";

	//start value for semaphores
	mainsem = screate(0);
	functionSem = screate(1);
	printf("Enter unsigned long integer NUM:\n");
	scanf("%lu", &num);
	// check if the number is not even or the num == 2
	if (num % 2 == 0 && num != 2)
	{
		printf("The number %lu you enterd is even try again ", num);
		return 1;
	}
	printf("Enter number of processes:\n");
	scanf("%d", &processesNum);
	if (processesNum > 30)
	{ //check the limit of proc`s in xinu
		printf("The number %d you enterd is to big ", processesNum);
		return 1;
	}
	//Loop to initialize multiple processes depending on user selection
	for (i = 1; i <= processesNum; i++)
	{
		process_name[8] = i + '0';
		starter = 3 + 2 * (i - 1);
		resume(create(findDiv, INITSTK, INITPRIO - 1, process_name, 1, starter));
	}
	//wait for all the serch process to end
	for (i = 0; i < processesNum; i++)
	{
		wait(mainsem);
	}
	//if we didnt find a div number to the number in check
	if (!primaryNum)
	{
		printf("%lu is prime\n", num);
	}
	//if we find a div number
	else
	{
		printf("%lu is NOT prime\n", num);
		printf("%lu / %lu = %lu\n", num, divisor, num / divisor);
		printf("Winner pid  = %d winner starter = %lu \n", pid, winning_starter);
	}
	return 0;
}

/*------------------------------------------------------------------------
 *  findDiv  - check if the num get divided by one of the number in the seria
 *------------------------------------------------------------------------*/
findDiv( starter) int starter;
{
	int interval = processesNum * 2, flag = 1;
	unsigned long int checkUntil = num / 2, currDiv = starter;
	if (starter > checkUntil)
	{
		flag = 0;
		currDiv += interval;
	}
	//printf( "\n num is %lu \n starter = %lu \n",num,starter);
	while ((currDiv <= checkUntil) && flag)
	{
		wait(functionSem);
		if (primaryNum) //if other process already find the divisor
		{
			signal(functionSem);
			break;
		}
		if ((num % currDiv) == 0) //if the divisor is good
		{
			primaryNum = 1;
			pid = getpid();
			divisor = currDiv;
			winning_starter = starter;
			currDiv += interval;
			signal(functionSem);
			break;
		}
		currDiv += interval; //add the interval to the divisor that we check next
		signal(functionSem);
	}
	//A semaphore that works to prevent stopping printing in the middle
	wait(functionSem);
	printf("Process with pid %d started with starter %d incremented by %d. Tested up to %lu \n", getpid(), starter, interval, currDiv - interval);
	signal(functionSem);

	signal(mainsem);
	return 0;
}
