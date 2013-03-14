#include "sm_macro.h"
#include <stdio.h>

//initialize queue system
static int queues[N_QUEUES][N_FLOORS] = {{0}};

//initializes state system
static sm_state_t state = STATE_UNDEFINED;
static sm_state_t previousState = STATE_UNDEFINED;

//initialize floor indicator variable
static int floor = 0;

int main()
{
    
	//Initialize Statemachine macros
	if(!sm_init()){
		printf(__FILE__ ": Unable to initialize Statemachine Logic\n");
	}

    while (1) {

		//Set the floor variable to the last floor seen by the sensors.
   		floor = sm_check_floor_sensor_signal();

		//Checks for buttons signals from panel.
		sm_check_order_button_signals(queues);

		//Make UI light up the floor indicator lamp at the correct last seen floor.
		sm_floor_indicator(floor);
        
		//Stops the elevator if the stop button is pressed during transition of states
        if(sm_stop_signal())
        {
            state = STATE_STOP;
        }
        
        // STATEMACHINE
        switch(state)
        {
            case STATE_IDLE:
				state = sm_idle(queues,previousState);
				previousState = STATE_IDLE;
				break;

            case STATE_MOVE_UP:	        
				state = sm_move_up(queues);
				previousState = STATE_MOVE_UP;
				break;

            case STATE_MOVE_DOWN:
				state = sm_move_down(queues);
				previousState = STATE_MOVE_DOWN;
				break;

            case STATE_OPEN_DOOR:
                state =sm_open_door(queues,previousState);
                previousState = STATE_OPEN_DOOR;
				break;

            case STATE_STOP:
                state = sm_stop(queues);
				break;

            case STATE_UNDEFINED:
				//If the elevator do not have a defined state, move until it gets to the bottom
				//of the elevator shaft or sens a floor sensor underneath current position.
				state = sm_undefined(queues);			
				break;
		}//End Switch
    }//End While

    return 0;

}

