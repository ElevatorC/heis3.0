// REPLACE THIS FILE WITH YOUR OWN CODE.
// READ ELEV.H FOR INFORMATION ON HOW TO USE THE ELEVATOR FUNCTIONS.

#include "elev.h"
#include "ui.h"
#include "sm_macro.h"
#include <stdio.h>


// legges i elev.h
static int queues[N_QUEUES][N_FLOORS] = {{0}};

int main()
{
	//initialize states
	int state = STATE_UNDEF;
	int previousState = STATE_UNDEF;

    // Initialize hardware
    if (!elev_init())
    {
        printf(__FILE__ ": Unable to initialize elevator hardware\n");
        return 1;
    }

    if (!ui_init())
    {
        printf(__FILE__ ": Unable to initialize elevator user interface\n");
        return 1;
    }

    printf("Activate obstruction and press the STOP button to stop elevator and exit program.\n");

    //Move elevator towards next bottom floor
    //if (elev_get_floor_sensor_signal() < 0)
    //{
    //    elev_set_speed(-100);
    //}

    while (1) {

        //get button signal
		print_queues(queues);
		ui_button_signals(queues);
		printf("\nChanged queue:\n");
		print_queues(queues);
		printf("\n%d\n",state);

		//if (sm_timer_active())
		//	state = STATE_DOOR_OPEN;

        // STATEMACHINE
        switch(state)
        {

            case STATE_IDLE:
				state = sm_idle(queues,previousState);
				previousState = STATE_IDLE;
				break;

            case STATE_UP:
				state = sm_up(queues);
				previousState = STATE_UP;
				break;

            case STATE_DOWN:
				state = sm_down(queues);
				previousState = STATE_DOWN;
				break;

            case STATE_DOOR_OPEN:
				state = sm_door_open(queues,previousState);
				previousState = STATE_DOOR_OPEN;
				break;

            case STATE_STOP:
				state = sm_stop(queues);
				break;

            case STATE_UNDEF:
            	//If the elevator is not on a floor at start move until it gets to a floor signal
				state = sm_undef();
				break;
         	}


        // Check if stop button and obstruction is active, if so, stop elevator and exit program.
		if (elev_get_obstruction_signal() &&  elev_get_stop_signal() && queues[2][1]) {
            elev_set_speed(0);
            break;
		}
    }//End While

    return 0;

}

