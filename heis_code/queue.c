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

/* return TRUE if order table contains orders 
int queue_check_orders(int queues[N_QUEUES][N_FLOORS]){
	int floor;
    for(floor=0; floor < N_FLOORS; floor++){
        if(queues[QUEUE_UP][floor] || queues[QUEUE_DOWN][floor] || queues[QUEUE_COMMAND][floor])
        {
            return 1; //TRUE;
        }
    }
    return 0; //FALSE;

}*/
/* 
int queue_check_orders(int queues[N_QUEUES][N_FLOORS], int currentFloor, int previousState){
	int queue;
	int floor;

	//for(queue = 1; queue < N_QUEUES; queue++){
		if(previousState == STATE_UP)
			for(floor = 0; floor < N_FLOORS; floor++){
				if(queues[1][floor]) {
					return STATE_UP;			
				}
 
			}
		else if(previousState == STATE_DOWN) {
		
		}

		}
	
	return -1;
	
}*/

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
	

//Olav kode
//!!!!!brukes for å se om den har flere relevante elementer i command køen i sm_macro!!!!!
/*
int queue_check_relevant_command(int queues[N_QUEUES][N_FLOORS],int floor, int dir){
	//if the elevator is heading up
	if(dir) {
		for(floor+=1;floor<N_FLOORS;floor++){
			//if the command queue has a elements who are over the current
			if(queues[QUEUE_COMMAND][floor]) {
				return 1; //TRUE
			}
			else {
				return 0; //FALSE
			}
		}
	//else if elevator is heading down
	}

	//if !dir er det same some else motoren har alltid en rettning 1 eller 0
	else {
		//if the command queue has a elements who are under the current
		for(floor-=1;floor<=0;floor--){
			if(queues[QUEUE_COMMAND][floor]) {
				return 1; //TRUE
			}
			else { //denne else'en kan settes utenfor mener jeg
				return 0; //FALSE
			}
		}
	}
return -1; //debug
}
*/
