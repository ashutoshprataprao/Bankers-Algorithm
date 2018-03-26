#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<unistd.h>

int Bankers()
{
	//Method for Bankers algorithm which check safe and unsafe state.
}
void* request_resource()
{
	//Method for requesting the resources.	
}

int i,j;
int Processes;
int *End;

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


