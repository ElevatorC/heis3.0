/**
	This module handels the queue system
*/

#include "ui.h"
#include "elev.h"
#include "io.h"
#include "channels.h"
#include "queue.h"

#include<assert.h>


//singel item version of queue has order
int queue_floor_has_orders(int queues[N_QUEUES][N_FLOORS], queue_t queueType, int floor)
{
    assert(floor != -1);
    return queues[queueType][floor];

}

//general type, can be changed too 
//Let's see if we'll use 1 generic or 2 setters
void queue_set_order(int queues[N_QUEUES][N_FLOORS], queue_t queueType, int floor,int value){
	queues[queueType][floor] = value;
}

<<<<<<< HEAD
void queue_add_to_queue(int queues[N_QUEUES][N_FLOORS], queue_t queueType, int floor){
	queues[queueType][floor] = 1;
}

//Removes order from queue
void queue_remove_from_queue(int queues[N_QUEUES][N_FLOORS], queue_t queueType, int floor){
	queues[queueType][floor] = 0;
}

/*	return FALSE if queue matrix contains orders 
	returns TRUE if queue matrix does not contain orders */
int queue_has_orders(int queues[N_QUEUES][N_FLOORS]){
	int floor;
	
	for(floor=0; floor < N_FLOORS; floor++){
		if(queues[QUEUE_UP][floor] || queues[QUEUE_DOWN][floor] || queues[QUEUE_COMMAND][floor]){
			return 1; //TRUE
		}
=======

int queue_is_empty(int queues[N_QUEUES][N_FLOORS],queue_t queueType){
	int floor;
	
	for(floor=0; floor < N_FLOORS; floor++){
		if(queues[queueType][floor] == 1)
			return 0; //FALSE
>>>>>>> parent of 8144ebc... More from the lab
	}
	
	return 0; //FALSE

}
<<<<<<< HEAD

int queue_queueType_has_orders(int queues[N_QUEUES][N_FLOORS],int queueType){
	int floor;

	for(floor = 0; floor < N_FLOORS; floor++){
			if(queues[queueType][floor] == 1){
				return 1; //TRUE		
			}
	}

	return 0; //FALSE

}

//Clear all the queues
void queue_clear_orders(int queues[N_QUEUES][N_FLOORS]){
	int queue;
	int floor;

	for(queue=0; queue < N_QUEUES; queue++){
		for(floor = 0; floor < N_FLOORS; floor++){
			queues[queue][floor]=0;
		}
	}
}

//Checks the queues and returns the 1st queueType with orders??
int queue_check_ordered_queue_type(int queues[N_QUEUES][N_FLOORS]){
	int queue;
	int floor;

	for(queue=0; queue < N_QUEUES; queue++){
		for(floor = 0; floor < N_FLOORS; floor++){
			if(queues[queue][floor] == 1){
				return queue;		
			}
		}
	}
	return -1;
	
}
	
//ser etter elementer over current
int queue_from_and_up_empty(int queues[N_QUEUES][N_FLOORS],queue_t queueType, int currentFloor){
	int floor;
	
	for(floor=currentFloor; floor < N_FLOORS; floor++){ 
			if(queues[queueType][floor] == 1)
			return 0; //FALSE
=======

/* return TRUE if order table contains orders */
int queue_check_orders(int queues[N_QUEUES][N_FLOORS]){
	int floor;
    for(floor=0; floor < N_FLOORS; floor++){
        if(queues[QUEUE_UP][floor] || queues[QUEUE_DOWN][floor] || queues[QUEUE_COMMAND][floor])
        {
            return 1; //TRUE;
        }
    }
    return 0; //FALSE;

}

//Olav kode
//!!!!!brukes for å se om den har flere relevante elementer i command køen i sm_macro!!!!!
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
>>>>>>> parent of 8144ebc... More from the lab
	}

	return 1; //TRUE

}
//ser etter elementer under current
int queue_from_and_down_empty(int queues[N_QUEUES][N_FLOORS],queue_t queueType, int currentFloor){
	int floor;
	for(floor=currentFloor; floor >= 0; --floor){ // kanskje post
			if(queues[queueType][floor] == 1)
			return 0; //FALSE
	}
<<<<<<< HEAD

	return 1; //TRUE

=======
>>>>>>> parent of 8144ebc... More from the lab
}
