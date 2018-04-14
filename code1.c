#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdbool.h>
#include<stdlib.h>

int processes,resource;
 
int safe[10]; //To store the safe sequence
bool completed[10];  //To check whether a process is completed or not
int avail[10];  //available array
int alloc[10][10]; //allocation matrix for checking how many resources alloacted to a process 
int need[10][10];  //need matrix for checking how many resources need to a process 
int max[10][10];  //Max matrix to check total number of resources required to a process

//Mutex lock for avail and allocation
pthread_mutex_t mut_available;
pthread_mutex_t mut_allocation;
pthread_mutex_t mut_need;
pthread_mutex_t lock;

bool fun_allocation(int process,int *request);


// Method for displaying status of avail,need,allocate and max. 
void print()
{
	//display status of available.
	printf("Status of avail:\n");
	for(int i=0;i<resource;i++)
	{
		printf("\tR%d",i);
	}
	printf("\n");
	for(int i=0;i<resource;i++)
	{
		printf("\t%d",avail[i]);
	}
	
	//display status of allocation
	printf("\nStatus of allocation:\n");
	for(int i=0;i<resource;i++)
	{
		printf("\tR%d",i);
	}
	printf("\n");
	for(int i=0;i<processes;i++)
	{
		
		printf("P%d\t",i);
		for(int j=0;j<resource;j++)
		{
			printf("%d\t",alloc[i][j]);
		}
		printf("\n");
		
	}
	
	//display status of max.
	printf("Status of max:\n");
	for(int i=0;i<resource;i++)
	{
		printf("\tR%d",i);
	}
	printf("\n");
	for(int i=0;i<processes;i++)
	{
		
		printf("P%d\t",i);
		for(int j=0;j<resource;j++)
		{
			printf("%d\t",max[i][j]);
		}
		printf("\n");
		
	}
	
	//display status of need.
	printf("Status of need:\n");
	for(int i=0;i<resource;i++)
	{
		printf("\tR%d",i);
	}
	printf("\n");
	for(int i=0;i<processes;i++)
	{
		printf("P%d\t",i);
		for(int j=0;j<resource;j++)
		{
			printf("%d\t",need[i][j]);
		}
		printf("\n");
		
	}
}

//Method for  update the value of need.
void update_need()
{
	printf("Status of need:\n");
	for(int i=0;i<resource;i++)
	{
		printf("\tR%d",i);
	}
	printf("\n");
	for(int i=0;i<processes;i++)
	{
		printf("P%d\t",i);
		for(int j=0;j<resource;j++)
		{
			need[i][j]=max[i][j]-alloc[i][j];
			printf("%d\t",need[i][j]);
		}
		printf("\n");
		
	}
}

/**
 Method for Bankers algorithm which check safe and unsafe state.
It return true if system is in safe state, false if unsafe state.
**/

bool Bankers()
{
	int task[10];  //A duplicate array for avail.
	for(int i=0;i<processes;i++)
	{
		completed[i]=false;
	}
	for(int i=0;i<resource;i++)
	{
		task[i]=avail[i];
	}
	int count1=0;
	print();
	while(count1<processes)
	{
		bool check=false;
		for(int i=0;i<processes;i++)
		{
			if(completed[i]==false)
			{
				int j;
				for(j=0;j<resource;j++)
				{
					if(need[i][j]>task[j])
					{
						break;
					}
				}
				if(j==resource)
				{
					for(int k=0;k<resource;k++)
					{			
						task[k]+=alloc[i][k];
						
					}
					safe[count1++]=i;
					completed[i]=true;
					check=true;
				}	
			}
			
		}	
		if(check==false)
		{
			printf("System is in unsafe state\n");
			return false;
		}
	}
	printf("Your system is in safe state and safe sequence is\n");
	printf("<");
	for(int i=0;i<processes;i++)
	{
		printf("P%d ",safe[i]);
	}
	printf(">");
	printf("\n\n");
	return true;	
}


//Method for requesting the resources.
void *request_resource()
{
	pthread_mutex_lock(&lock);
	int process_id;
	printf("Enter process id which  requesting for resource: ");
	scanf("%d",&process_id);    	
	int request[10];
	printf("Process P%d requesting for resource: \n",process_id);
	for(int i=0;i<resource;i++)
	{	
		printf("Enter number of instance required for resource R%d: ",i);
		scanf("%d",&request[i]);
	}	
	if(fun_allocation(process_id,request)==true)
	{
		printf(" Reuest Approved\n");
		print();
		sleep(1);
	}
	else
	{
		printf(" Request Denied\n");
		print();
		sleep(1);
	}
	pthread_mutex_unlock(&lock);
	pthread_exit(NULL);
}


bool fun_allocation(int process,int *request)
{
	for(int r=0;r<resource;r++)
	{
		if((request[r]<=need[process][r]) && (request[r]<=avail[r]))
		{
		pthread_mutex_lock(&mut_allocation);
		alloc[process][r]=alloc[process][r]+request[r];
		pthread_mutex_unlock(&mut_allocation);

		pthread_mutex_lock(&mut_available);
		avail[r]=avail[r]-request[r];
		pthread_mutex_unlock(&mut_available);

		pthread_mutex_lock(&mut_need);
		need[process][r]=need[process][r]-request[r];
		pthread_mutex_unlock(&mut_need);
		}
		else
		{
			return false;
		}
	}
	printf("You are in Fun allocation method\n");
	print();
	if(Bankers()==false)
	{
		printf("\nYou are inside bankers algorithm\n");
		for(int r=0;r<resource;r++)
		{
			pthread_mutex_lock(&mut_allocation);
			alloc[process][r]=alloc[process][r]-request[r];
			pthread_mutex_unlock(&mut_allocation);

			pthread_mutex_lock(&mut_available);
			avail[r]=avail[r]+request[r];
			pthread_mutex_unlock(&mut_available);

			pthread_mutex_lock(&mut_need);
			need[process][r]=need[process][r]+request[r];
			pthread_mutex_unlock(&mut_need);
		}		
		return false;
	}
	return true;
}

int main()
{
	pthread_mutex_init(&mut_available,NULL);
	pthread_mutex_init(&mut_allocation,NULL);
	pthread_mutex_init(&mut_need,NULL);
	pthread_mutex_init(&lock,NULL);
	printf("\nEnter number of processes: ");
	scanf("%d",&processes);
	for(int i=0;i<processes;i++)
	{
		completed[i]=false;
	}
	printf("\nEnter number of resources: ");
	scanf("%d",&resource);
	printf("\nEnter max matrix: ");
	for(int i=0;i<processes;i++)
	{
		for(int j=0;j<resource;j++)
		{
			scanf("%d",&max[i][j]);
		}
	}
	printf("\nEnter allocation matrix: ");
	for(int i=0;i<processes;i++)
	{
		for(int j=0;j<resource;j++)
		{
			scanf("%d",&alloc[i][j]);
		}
	}
	printf("\nEnter available resource: ");
	for(int i=0;i<resource;i++)
	{
		scanf("%d",&avail[i]);
	}
	printf("\nvalue of need, max, allocate and available:\n");
	print();
	printf("\nvalue of updated need:\n");
	update_need();
	Bankers();
	printf("\nvalue of need, max, allocate and available:\n");
	print();
	pthread_t Threads[processes];
	int res;
	for(int i=0;i<processes;i++)
	{
		res=pthread_create(&Threads[i],NULL,request_resource,NULL);
	}
	if(res!=0)
	{
		printf("\nError occurs, with value %d\n",res);
		exit(-1);
	}
	printf("Thread created successfully.\n");
	for(int i=0;i<processes;i++)
	{
		pthread_join(Threads[i],NULL);
	}
	return 0;
}


