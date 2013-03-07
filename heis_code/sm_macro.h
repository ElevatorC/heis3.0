#ifndef SM_MACRO_H
#define SM_MACRO_H

#define N_QUEUES 3 //QUEUE_UP QUEUE_DOWN QUEUE_COMMAND 


//the UP state's logic is programmed here
//returns the value of the next state
current_state_t sm_up(int queues[N_QUEUES][N_FLOORS]);
current_state_t sm_down(int queues[N_QUEUES][N_FLOORS]);
current_state_t sm_idle(int queues[N_QUEUES][N_FLOORS]);
current_state_t sm_stop(int queues[N_QUEUES][N_FLOORS]);
<<<<<<< HEAD
current_state_t sm_door_open(int queues[N_QUEUES][N_FLOORS], int previousState);
=======
current_state_t sm_door_open(int queues[N_QUEUES][N_FLOORS]);
>>>>>>> parent of 8144ebc... More from the lab
current_state_t sm_undef();

#endif
