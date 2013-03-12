#ifndef SM_MACRO_H
#define SM_MACRO_H

#define N_QUEUES 3 
#define N_FLOORS 4

/**
  State types for statemachine
*/
typedef enum state { 
    STATE_IDLE		= 0, 
    STATE_MOVE_UP	= 1, 
    STATE_MOVE_DOWN	= 2,
    STATE_OPEN_DOOR = 3,
    STATE_STOP		= 4,
    STATE_UNDEFINED	= 5
} sm_state_t;

/**
Checks if the stop button is pushed returns 1(true) if it has and 0(false9 if not
*/
int sm_stop_signal();

/**
Checks the floor sensor signal and returns 0..3
*/
int sm_check_floor_sensor_signal(void);

/**
Checks for button press and sets the light on appropriate
button and add elements to queue.
*/
void sm_check_button_signals(int queues[N_QUEUES][N_FLOORS]);

/**
Sets the correct floor indicator light.
*/
void sm_floor_indicator(int floor);

/**
Initialize everything and the statemachine logic.
*/
int sm_init();

//the statemachine's  logic is programmed here
/**
Contains the move up state logic and returns open door, idle or move up
*/
sm_state_t sm_move_up(int queues[N_QUEUES][N_FLOORS]);

/**
Contains the move down state logic and returns open door, idle or move down
*/
sm_state_t sm_move_down(int queues[N_QUEUES][N_FLOORS]);

/**
Contains the idle state logic and returns open door, idle or move down.
*/
sm_state_t sm_idle(int queues[N_QUEUES][N_FLOORS], int previousState);


/**
Contains the stop state logic and returns stop or undefined.
*/
sm_state_t sm_stop(int queues[N_QUEUES][N_FLOORS]);


/**
Contains the idle state logic and returns open door, move up, move down or stop.
*/
sm_state_t sm_open_door(int queues[N_QUEUES][N_FLOORS], sm_state_t previousState);

/**
Contains the undefined state logic and returns undefined or idle.
*/
sm_state_t sm_undefined(void);


#endif
