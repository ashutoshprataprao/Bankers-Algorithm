#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<unistd.h>

int i,j;  //Global variable for looping
int Processes,resource; 
int *end;
int *avail;  //available array
int **alloc; //allocation matrix for checking how many resources alloacted to a process 
int **need;  //need matrix for checking how many resources need to a process 
int **max;  //Max matrix to check total number of resources required to a process

// Method for displaying status of avail,need,allocate and max. 
void display()
{
	//display status of available.
	for(i=0;i<resource;i++)
	{
		printf("\tR%d",i);
	}
	printf("\n");
	for(i=0;i<resource;i++)
	{
		printf("\t%d",avail[i]);
	}
	
	//display status of allocation
	for(i=0;i<resource;i++)
	{
		printf("\tR%d",i);
	}
	printf("\n");
	for(i=0;i<processes;i++)
	{
		if(end[i]==0)
		{
			printf("P%d\t",i);
			for(j=0;j<resource;j++)
			{
				printf("%d\t",alloc[i][j]);
			}
			printf("\n");
		}
	}
	
	//display status of max.
	for(i=0;i<resource;i++)
	{
		printf("\tR%d",i);
	}
	printf("\n");
	for(i=0;i<processes;i++)
	{
		if(end[i]==0)
		{
			printf("P%d\t",i);
			for(j=0;j<resource;j++)
			{
				printf("%d\t",max[i][j]);
			}
			printf("\n");
		}
	}
	
	//display status of need.
	for(i=0;i<resource;i++)
	{
		printf("\tR%d",i);
	}
	printf("\n");
	for(i=0;i<processes;i++)
	{
		if(end[i]==0)
		{
			printf("P%d\t",i);
			for(j=0;j<resource;j++)
			{
				printf("%d\t",need[i][j]);
			}
			printf("\n");
		}
	}
}


/**
 Method for Bankers algorithm which check safe and unsafe state.
It return true if system is in safe state, false if unsafe state.
**/

int Bankers()
{
	int check = 0;
	int *task;  //An array for task.
	if(!(task= malloc(resource * sizeof(int)) ))
	{
		return -1;	
	}
	for(i=0;i<resource;i++)
	{
		task[i]=avail[i];
	}
	for(i=0;i<processes;i++)
	{
		if(end[i]==0)
		{
			for(j=0;j<resource;j++)
			{
				if(need[i][j]>task[j])
				{
					printf("System is in unsafe state.");
					return check;
				}
			}
			for(j=0;j<resource;j++)
			{
				task[j]=task[j]+alloc[i][j];
			}
			check=1;
		}	
	}
	printf("System is in %s",check ? "Safe" :"Unsafe");
	return check;	
}


void* request_resource()
{
	//Method for requesting the resources.	
}



int main()
{
	pthread_t Threads[Processes];
	int res;
	printf("Thread creation for process %d.\n",i);
	res=pthread_create(&Threads[i],NULL,request_resource,NULL);
	if(res!=0)
	{
		printf("\nError occurs, with value %d\n",res);
		exit(-1);
	}
	printf("Successfuly finshed the process\n");
	return 0;
}


