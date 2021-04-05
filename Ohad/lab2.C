/* lab2.c - xmain, findDiv */

#include <conf.h>
#include <kernel.h>

/* external variables are shared by all processes */
int counter, primaryNum, pid;
int mainsem, notPrimarySem, Csem;
int winStarter;
unsigned long int num, divisor;

/*------------------------------------------------------------------------
 *  xmain 
 *------------------------------------------------------------------------
 */
xmain()
{
	int findDiv();
	int i, processesNum, starter, interval;
	char procName[] = "proc_X";
	primaryNum = 0;

	//sems intilize
	mainsem = screate(0);
	Csem = screate(1);
	notPrimarySem = screate(1);

	printf("Enter unsigned long integer NUM:\n");
	scanf("%lu", &num);

	do
	{
		printf("Enter number of processes:\n");
		scanf("%d", &processesNum);
		if (processesNum > 30)
			printf("***\nThe max limit is 30!\n***\n");
	} while (processesNum > 30);

	counter = processesNum;
	interval = processesNum * 2;

	//even validation
	if (num != 2 && num % 2 == 0)
	{
		printf("The number you entered is even !");
		return 1;
	}

	for (i = 1; i <= processesNum; i++)
	{
		procName[5] = i + '0'; //dynamic process name by his index
		starter = 3 + 2 * (i - 1);
		resume(create(findDiv, INITSTK, INITPRIO, procName, 2, interval, starter));
	}
	//waiting until the last process we have opened will signal this semaphore
	wait(mainsem);

	//result check and printing it
	if (primaryNum == 0)
	{
		printf("%lu is prime\n", num);
	}
	else
	{
		printf("%lu is NOT prime\n", num);
		printf("%lu / %lu = %lu\n", num, divisor, num / divisor);
		printf("Winner pid  = %d winner start = %d \n", pid, winStarter);
	}
	return 0;
}

/*------------------------------------------------------------------------
 *  findDiv  --  A function that looks for a divisor for global variable 'num'
 * 				 from the series that starts with a starter and each additional step add the variable 'interval' (Totalprocesses*2)
 *------------------------------------------------------------------------
 */
findDiv(interval, starter) int interval, starter;
{
	unsigned long int limit = num / 2;
	unsigned long int currDiv = starter;

	//status flag - if the func find divisor. 0 - find the divisor or starter greater than limit, 1 - didn't find
	int notFind = 0;

	while (currDiv <= limit)
	{
		wait(notPrimarySem);
		if (primaryNum == 1) //other process found the divisor
		{
			signal(notPrimarySem);
			break;
		}
		if ((num % currDiv) == 0) //the current process found the diviser
		{
			primaryNum = 1; //let other processes know the current find the divisor
			notFind = 0;	//set status flag

			//saving information at global variables for later printf
			pid = getpid();
			divisor = currDiv;
			winStarter = starter;
			signal(notPrimarySem);
			break;
		}
		notFind = 1;
		currDiv += interval;
		signal(notPrimarySem);
	}
	wait(Csem);
	if (notFind == 1) //check if needed dec interval from the last divisor search
		currDiv -= interval;
	printf("Process with pid %d started with %d incremented by %d. Tested up to %lu \n", getpid(), starter, interval, currDiv);
	if (--counter == 0) //check if this is the last process to run from those we start to find divisor
		signal(mainsem);
	signal(Csem);
	return 0;
}