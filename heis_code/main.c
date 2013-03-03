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
    if (elev_get_floor_sensor_signal() < 0)
    {
        elev_set_speed(-100);
    }

    while (1) {

        //get button signal

        // STATEMACHINE
        // Trenger en metode for å sjekke knapp input og legge det i kø array
        switch(state)
        {

            case STATE_IDLE:
            	elev_set_speed(0);
				state = sm_idle(queues);
				break;
            //ikke obst.

            //bestilling


            case STATE_UP:
                // Finish up queue and command queue increasing so after 1. floor it only cares about 2-4.
                // IF order on floor above current floor(function that read) and elevator in motion upwards
                // when finished go to down after que_down check else go to idle
                elev_set_speed(300);
				state = sm_up(queues);
				break;

            case STATE_DOWN:
                // Finish up queue and command queue decsending so after 4. floor it only cares about 3-1.
                // IF order on floor above current floor(function that read) and elevator in motion upwards
                // when finished go to up after que_down check else go to idle
                elev_set_speed(-300);
				state = sm_down(queues);

            case STATE_DOOR_OPEN:
                elev_set_speed(0);
                while(elev_get_obstruction_signal() != 0){
                        state = STATE_DOOR_OPEN;
                }
				state = STATE_IDLE;
				break;
                //egen macro?
                //sm_obstruction();

                //light door pannel

                //usleep(3000);
                //queue_delete que element
                //slett element i listen

            case STATE_STOP:
                elev_set_speed(0);
                //kanskje gøre denne mer modulær type
				state = sm_stop(queues);
				break;

            //case EMERGENCY:

            case STATE_UNDEF:
            	//If the elevator do not are on a floor, move until it gets to the floor underneath
				if (elev_get_floor_sensor_signal() < 0)
				{
					elev_set_speed(-100);
					state = STATE_UNDEF;
				}

				else if(elev_get_floor_sensor_signal() > 0){
					state = STATE_IDLE;
				}
				break;
                //break;//crash it!
		}


        // Check if stop button and obstruction is active, if so, stop elevator and exit program.
		if (elev_get_obstruction_signal() && elev_get_stop_signal()) {
            elev_set_speed(0);
            break;
        }

    }//End While

    return 0;

}

