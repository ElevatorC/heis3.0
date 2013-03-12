/**
	This module handels the queue system
*/

#include "queue.h"
#include<stdio.h>

/*
Checks if all the queue items from current floor and up in the selected queue type is empty
returns 1(true) if it is empty. Returns 0(false) 
*/
int queue_from_and_up_empty(int queues[N_QUEUES][N_FLOORS],queue_t queueType, int currentFloor)
{
	int floor;
	
	for(floor=currentFloor; floor < N_FLOORS; floor++){ 
			if(queues[queueType][floor] == 1)
			return 0; //FALSE
	}

	return 1; //TRUE

}

int queue_from_and_down_empty(int queues[N_QUEUES][N_FLOORS],queue_t queueType, int currentFloor){
	int floor;

	for(floor=currentFloor; floor >= 0; --floor)
	{ 
			if(queues[queueType][floor] == 1)
			return 0; //FALSE
	}

	return 1; //TRUE

}


void queue_clear_all_orders(int queues[N_QUEUES][N_FLOORS]){
	int queue;
	int floor;

	for(queue=0; queue < N_QUEUES; queue++){
		for(floor = 0; floor < N_FLOORS; floor++){
			queues[queue][floor]=0;
		}
	}
}


int queue_command_has_orders(int queues[N_QUEUES][N_FLOORS]){
	int floor;

	for(floor = 0; floor < N_FLOORS; floor++){
		if(queues[QUEUE_COMMAND][floor]==1)
		{
			return 1; //TRUE
		}
	}
	return 0; //false

}

//debug function
void queue_print_queues(int queues[N_QUEUES][N_FLOORS]){
	int queue;
	int floor;
	
	for(queue=0; queue < N_QUEUES; queue++){
		printf("%d : [ ",queue);

		for(floor = 0; floor < N_FLOORS; floor++){
			printf("%d ",queues[queue][floor]);
		}
		printf("]\n");
	}
}
