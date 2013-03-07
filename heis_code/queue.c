/**
	This module handels the queue system
*/

#include "ui.h"
#include "elev.h"
#include "io.h"
#include "channels.h"
#include "queue.h"

#include<assert.h>



int queue_floor_has_orders(int queues[N_QUEUES][N_FLOORS], queue_t queueType, int floor)
{
    assert(floor != -1);
    return queues[queueType][floor];
}

//general type, can be changed too 
void queue_change_queue(int queues[N_QUEUES][N_FLOORS], queue_t queueType, int floor,int value){
	queues[queueType][floor] = value;
}

//ser etter elementer over current
int queue_up_empty(int queues[N_QUEUES][N_FLOORS],queue_t queueType, int currentFloor){
	int floor;
	
	for(floor=currentFloor; floor < N_FLOORS; floor++){ 
			if(queues[queueType][floor] == 1)
			return 0; //FALSE
	}

	return 1; //TRUE

}
//ser etter elementer under current
int queue_down_empty(int queues[N_QUEUES][N_FLOORS],queue_t queueType, int currentFloor){
	int floor;
	for(floor=currentFloor; floor >= 0; --floor){ // kanskje post
			if(queues[queueType][floor] == 1)
			return 0; //FALSE
	}

	return 1; //TRUE

}
void queue_clear(int queues[N_QUEUES][N_FLOORS]){
	int queue;
	int floor;

	for(queue=0; queue < N_QUEUES; queue++){
		for(floor = 0; floor < N_FLOORS; floor++){
			queues[queue][floor]=0;
		}
	}
}

int queue_has_orders(int queues[N_QUEUES][N_FLOORS]){
	int queue;
	int floor;

	for(queue=0; queue < N_QUEUES; queue++){
		for(floor = 0; floor < N_FLOORS; floor++){
			if(queues[queue][floor]==1)
				return 1; //TRUE
		}
	}
	return 0; //false

}

int queue_queueType_order(int queues[N_QUEUES][N_FLOORS], int currentFloor, int previousState,int queueType){	
	int floor;	
	for(floor = currentFloor; floor < N_FLOORS; floor++){
				if(queues[queueType][floor]) {
					return STATE_UP;			
	}
	}
	for(floor = currentFloor; floor >= 0; floor--){
				if(queues[queueType][floor]) {
					return STATE_DOWN;			
	}
	}
	return -1;
	
}
