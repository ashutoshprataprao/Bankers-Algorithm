#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>

int i,j;  //Global variable for loops

int processes,resource;
 
int completed[10];
int avail[10];  //available array
int alloc[10][10]; //allocation matrix for checking how many resources alloacted to a process 
int need[10][10];  //need matrix for checking how many resources need to a process 
int max[10][10];  //Max matrix to check total number of resources required to a process

//Mutex lock for avail and allocation
pthread_mutex_t mut_available;
pthread_mutex_t mut_allocation;

// Method for displaying status of avail,need,allocate and max. 
void print()
{
	//display status of available.
	printf("Status of avail:\n");
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
	printf("\nStatus of allocation:\n");
	for(i=0;i<resource;i++)
	{
		printf("\tR%d",i);
	}
	printf("\n");
	for(i=0;i<processes;i++)
	{
		if(completed[i]==0)
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
	printf("Status of max:\n");
	for(i=0;i<resource;i++)
	{
		printf("\tR%d",i);
	}
	printf("\n");
	for(i=0;i<processes;i++)
	{
		if(completed[i]==0)
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
	printf("Status of need:\n");
	for(i=0;i<resource;i++)
	{
		printf("\tR%d",i);
	}
	printf("\n");
	for(i=0;i<processes;i++)
	{
		if(completed[i]==0)
		{
			printf("P%d\t",i);
			for(j=0;j<resource;j++)
			{
				need[i][j]=max[i][j]-alloc[i][j];
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
		if(completed[i]==0)
		{
			for(j=0;j<resource;j++)
			{
				if(need[i][j]>task[j])
				{
					printf("System is in unsafe state.\n");
					print();
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
	printf("System is in %s",check ? "Safe state" :"Unsafe state");
	print();
	return check;	
}

//Method for requesting the resources.
void *request_resource(void* p)
{
	int process_id=(int)p;
	sleep(1);    	
	int request[resource];
	for(i=0;i<resource;i++)
	{	
		if(need[process_id][i] != 0)
                {
                     request[i] = rand() % need[process_id][i];
                }
                else
                {
                     request[i] = 0;
                }
		printf("Process %d requesting for %d units of R%d.\n",process_id,request[i],i);	
	}
		
	if(fun_allocation(process_id,request)==1)
	{
		printf(" Approved\n");
		sleep(1);
	}
	else
	{
		printf(" Denied\n");
		sleep(1);
	}
	pthread_exit(NULL);
}

int fun_allocation(int process,int *request)
{
	for(i=0;i<resource;i++)
	{
		pthread_mutex_lock(&mut_allocation);
		alloc[process][i]=alloc[process][i]+request[i];
		pthread_mutex_unlock(&mut_allocation);

		pthread_mutex_lock(&mut_available);
		avail[i]=avail[i]-request[i];
		pthread_mutex_unlock(&mut_available);
	}
	if(Bankers()==0)
	{
		for(i=0;i<resource;i++)
		{
			pthread_mutex_lock(&mut_allocation);
			alloc[process][i]=alloc[process][i]-request[i];
			pthread_mutex_unlock(&mut_allocation);

			pthread_mutex_lock(&mut_available);
			avail[i]=avail[i]+request[i];
			pthread_mutex_unlock(&mut_available);
		}		
		return 0;
	}
	return 1;
}

int main()
{
	pthread_mutex_init(&mut_available,NULL);
	pthread_mutex_init(&mut_allocation,NULL);
	printf("\nEnter number of processes: ");
	scanf("%d",&processes);
	for(i=0;i<processes;i++)
	{
		completed[i]=0;
	}
	printf("\nEnter number of resources: ");
	scanf("%d",&resource);
	printf("\nEnter max matrix: ");
	for(i=0;i<processes;i++)
	{
		for(j=0;j<resource;j++)
		{
			scanf("%d",&max[i][j]);
		}
	}
	printf("\nEnter allocation matrix: ");
	for(i=0;i<processes;i++)
	{
		for(j=0;j<resource;j++)
		{
			scanf("%d",&alloc[i][j]);
		}
	}
	printf("\nEnter available resource: ");
	for(i=0;i<resource;i++)
	{
		scanf("%d",&avail[i]);
	}
	printf("\nBefore allocation of resources value of need, max, allocate and available:\n");
	print();
	pthread_t Threads[processes];
	int res;
	for(i=0;i<processes;i++)
	{
		printf("Thread creation for process %d.\n",i);
		res=pthread_create(&Threads[i],NULL,request_resource,(void *)i);
	}
	if(res!=0)
	{
		printf("\nError occurs, with value %d\n",res);
		exit(-1);
	}
	for(i=0;i<processes;i++)
	{
		pthread_join(Threads[i],NULL);
	}
	printf("After allocation of resources value of need, max, allocate and available:\n");
	print();
	
	printf("Successfuly finshed the process\n");
	return 0;
}


