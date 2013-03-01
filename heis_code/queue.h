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

struct queue {
   bool queue_up[4];
   bool queue_down[4];
   bool queue_command[4]; 
};

//Add floor to selected queue
void queue_add(int queue,int floor);

//Remove floor order from selected queue
void queue_remove(int queue,int floor);

//Checks if queue array is empty
bool queue_is_empty(int *queues);

bool queue_check_queues(int queues[N_QUEUES][N_FLOORS]);

void queue_clear(int &queues);
/** 
Initializes Queue system
*/
bool queue_init(void);


#endif
