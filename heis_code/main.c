#include <stdio.h>
#include "elev.h"
#include "ui.h"
#include "sm_macro.h"
#include "io.h"
#include "channels.h"

// legges i elev.h!!!!!!!!!!!!!!!!
static int queues[N_QUEUES][N_FLOORS] = {{0}};

int main()
{
	//initialize states
	int state = STATE_UNDEF;
	int previousState = 0;
	int floor = 0;
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

    while (1) {
		if(elev_get_floor_sensor_signal()>=0)
		{
   		floor = elev_get_floor_sensor_signal();
		}
        //sette dette ut til SM_MACRO!!!!!!!!!!!!!!!!!!!!!!!
        //se kommunikasjonsdiagram
		ui_button_signals(queues);
		ui_set_floor_indicator(floor);

        if(elev_get_stop_signal())
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

           case STATE_UP:
				state = sm_up(queues);
				previousState = STATE_UP;
				break;

            case STATE_DOWN:
			    state = sm_down(queues);
				previousState = STATE_DOWN;
                break;

            case STATE_DOOR_OPEN:
  				state =sm_door_open(queues,previousState);
                previousState = STATE_DOOR_OPEN;
				break;

            case STATE_STOP:
                state = sm_stop(queues);
				break;

            case STATE_UNDEF:
            	//If the elevator do not are on a floor, move until it gets to the floor underneath
				state = sm_undef();
				break;
		}
	//End While
	}
    return 0;
}

