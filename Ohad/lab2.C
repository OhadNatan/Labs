/* lab2.c - xmain, findDiv */

#include <conf.h>
#include <kernel.h>

/* external variables are shared by all processes */
int counter, primaryNum, pid;
int mainsem, notPrimarySem, Csem;
int Totalprocesses;
unsigned long int divisor, winStarter;

/*------------------------------------------------------------------------
 *  xmain  --  producer and consumer processes synchronized with semaphores
 *------------------------------------------------------------------------
 */
xmain()
{
	int	findDiv();
	int	i, processesNum;
	unsigned long int num, starter;
	char procName[] = "proc_X";
	primaryNum = 0;

	//sems intilize 
	mainsem = screate(0);
	Csem = screate(1);
	notPrimarySem = screate(1);
	
	printf("Enter unsigned long integer NUM:\n");
	scanf("%lu",&num);

	printf("Enter number of processes:\n");
	scanf("%d",&processesNum);

	counter = processesNum;
	Totalprocesses = processesNum;

	//even validation
	if(num!=2 && num%2==0)
	{
		printf("The number you entered is even !");
		return 1;
	}

	for(i=1;i<=processesNum;i++)
	{
		procName[5] = i + '0';
		starter = 3+2*(i-1);
		resume(create(findDiv,INITSTK,INITPRIO,procName,3,num,starter));
	}
	wait(mainsem);
	wait(notPrimarySem);
	if(primaryNum==0){
		printf("%lu is prime\n",num);
	}
	else
	{
		printf("%lu is NOT prime\n", num);
		printf("%lu / %lu = %lu\n", num, divisor, num/divisor);
		printf("Winner pid  = %d winner start = %d \n", pid,winStarter );
	}
	return 0;
}

/*------------------------------------------------------------------------
 *  findDiv  --  
 *------------------------------------------------------------------------
 */
findDiv( num, starter)
unsigned long int num, starter;
{
	int interval = Totalprocesses*2;
	unsigned long int limit = num/2;
	unsigned long int currDiv = starter;
	int finder = 0;
	int NotFind = 0;

	while(currDiv <= limit)
	{
		wait(notPrimarySem);
		if(primaryNum==1)
		{
			signal(notPrimarySem);
			break;
		}
		if((num%currDiv)==0)
		{
			primaryNum = 1;
			pid = getpid();
			finder = 1;
			divisor = currDiv;
			winStarter = starter;
			signal(notPrimarySem);
			break;
		}
		NotFind = 1;
		currDiv += interval;
		signal(notPrimarySem);
	}
	wait(Csem);
	if(finder != 1 && NotFind == 1)
		currDiv -= interval;
	printf("Process with pid %d started with %lu incremented by %d. Tested up to %lu \n",getpid(), starter, interval, currDiv);
	if(--counter == 0)
		signal(mainsem);
	signal(Csem);
	return 0;
}