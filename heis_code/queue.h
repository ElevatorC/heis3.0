#ifndef QUEUE_H
#define QUEUE_H

#define N_FLOORS 4
#define N_QUEUES 3 

typedef enum queue_tag_type
{
	 QUEUE_UP      = 0
	,QUEUE_DOWN    = 1
	,QUEUE_COMMAND = 2
} queue_t;

/**
Checks if any queues has an ordered element in it.
returns 1(TRUE) if it has orders if not it returns 0(FALSE)
*/
int queue_command_has_orders(int queues[N_QUEUES][N_FLOORS]);


/**
Clear all orders in all queues
*/
void queue_clear_all_orders(int queues[N_QUEUES][N_FLOORS]);

/**
Checks if there are any orders in selected queue from current floor 
and up returns 1(TRUE) if empty if not it returns 0(FALSE).
*/
int queue_from_and_up_empty(int queues[N_QUEUES][N_FLOORS],queue_t queueType, int currenFloor);

/**
Checks if there are any orders in selected queue from current floor and down returns 1(TRUE) if empty
else returns 0.
*/
int queue_from_and_down_empty(int queues[N_QUEUES][N_FLOORS],queue_t queueType, int currenFloor);

/** 
Initializes Queue system
*/
int queue_init(void);

/**
debug functions for printing out the queue matrix
to terminal. Format for printing is 0:Up, 1:Down and 
2:Command queue.
*/
//void queue_print_queues(int queues[N_QUEUES][N_FLOORS]);

#endif
