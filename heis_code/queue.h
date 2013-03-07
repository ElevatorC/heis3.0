#ifndef QUEUE_H
#define QUEUE_H

#define N_FLOORS 4
#define N_QUEUES 3 

//Instead of matrix maybe have 3 queues in a struct?
typedef enum queue_tag
{
	 QUEUE_UP      = 0 //BUTTON_CALL_UP
	,QUEUE_DOWN    = 1 //BUTTON_CALL_DOWN
	,QUEUE_COMMAND = 2 //BUTTON_COMMAND
} queue_t;

//Add floor to selected queue
void queue_add(int queues[N_QUEUES][N_FLOORS],int queueType, int floor);

//Remove floor order from selected queue
void queue_remove(int queues[N_QUEUES][N_FLOORS],int queueType, int floor);

//Checks if queue array is empty
int queue_has_orders(int queues[N_QUEUES][N_FLOORS]);

int queue_check_queues(int queues[N_QUEUES][N_FLOORS]);

int queue_queueType_has_orders(int queues[N_QUEUES][N_FLOORS],int queueType);

<<<<<<< HEAD
void queue_clear(int queues[N_QUEUES][N_FLOORS]);

int queue_check_queues(int queues[N_QUEUES][N_FLOORS]);

int queue_from_and_up_empty(int queues[N_QUEUES][N_FLOORS],queue_t queueType, int currentFloor);

int queue_from_and_down_empty(int queues[N_QUEUES][N_FLOORS],queue_t queueType, int currentFloor);

=======
>>>>>>> parent of 8144ebc... More from the lab
/** 
Initializes Queue system
*/
int queue_init(void);


#endif
