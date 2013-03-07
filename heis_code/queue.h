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


/* Not needed
struct queue {
   int queue_up[4];
   int queue_down[4];
   int queue_command[4]; 
};
*/

//Add floor to selected queue
void queue_add(int queues[N_QUEUES][N_FLOORS],int queueType, int floor);

//Remove floor order from selected queue
void queue_remove(int queues[N_QUEUES][N_FLOORS],int queueType, int floor);

//Checks if queue array is empty
int queue_is_empty(int queues[N_QUEUES][N_FLOORS],queue_t queueType);

int queue_check_queues(int queues[N_QUEUES][N_FLOORS]);

void queue_clear(int queues[N_QUEUES][N_FLOORS]);

int queue_check_relevant_command(int queues[N_QUEUES][N_FLOORS],int floor, int dir);

int queue_up_empty(int queues[N_QUEUES][N_FLOORS],queue_t queueType, int currenFloor);

int queue_down_empty(int queues[N_QUEUES][N_FLOORS],queue_t queueType, int currenFloor);

int queue_check_orders(int queues[N_QUEUES][N_FLOORS], int currentFloor, int previousState);

int queue_check_queues(int queues[N_QUEUES][N_FLOORS]);

int queue_has_orders(int queues[N_QUEUES][N_FLOORS]);

/** 
Initializes Queue system
*/
int queue_init(void);


#endif
