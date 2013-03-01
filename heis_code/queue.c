/**
	This module handels the queue system
*/

#include "ui.h"
#include "elev.h"
#include "io.h"
#include "channels.h"
#include "queue.h"

#define N_QUEUES 3 //QUEUE_UP QUEUE_DOWN QUEUE_COMMAND

//Matrix of queues
static int queues[N_QUEUES][N_FLOORS] = {{0}};


bool queue_floor_has_orders(queue_t queueType, int floor)
{
    //assert(floor != -1);
    return queues[queueType][floor];

}

/* return TRUE if order table contains orders */
int queue_check_orders(void){

    for(int floor=0; floor < N_FLOORS; floor++){
        if(queues[QUEUE_UP][floor] || queues[QUEUE_DOWN][floor] || queues[QUEUE_COMMAND][floor])            
        {
            return 1;//TRUE;
        }
    }
    return 0; //FALSE;

}   
