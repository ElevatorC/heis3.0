// REPLACE THIS FILE WITH YOUR OWN CODE.
// READ ELEV.H FOR INFORMATION ON HOW TO USE THE ELEVATOR FUNCTIONS.

#include "elev.h"
#include "sm_macros.h"
#include <stdio.h>

// legges i elev.h


int main()
{ 
    // Initialize global queue arrays
    // kanskje de ikke skal være global?
    // int *queue_up    = {0,0,0,0};
    // int *queue_down  = {0,0,0,0};

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
        switch(current_state_t)
        {
            
            case IDLE:
            //ikke obst.

            //bestilling
              
            
            case UP:
                // Finish up que itterativt so after floor 1 it only cares about 2-4 etc..
                // IF order on floor above current flore(function that read) and elevator in motion upwards
                // when finished go to down after que_down check else go to idle
                sm_up();
            
            case DOWN:
                // Finish up que decitterativt so after floor 4 it only cares about 3-1 etc..
                // IF order on floor above current flore(function that read) and elevator in motion upwards
                // when finished go to up after que_down check else go to idle
                sm_down();
            
            case DOOR_OPEN:
                elev_set_speed(0);
                while(elev_get_obstruction_signal() != 0){
                        current_state = DOOR_OPEN;    
                }
                //egen macro?
                //sm_obstruction();    
                
                //light door pannel
                
                usleep(3000)
                //queue_delete que element 
                //slett element i listen          
            
            case STOP:
                elev_set_speed(0);
                //kanskje gøre denne mer modulær type
                sm_stop(*queue_up,*queue_down);
            
            //case EMERGENCY:

            case UNDEF:
            if (elev_get_floor_sensor_signal() < 0)
            {
                elev_set_speed(-100);
                return UNDEF;
            }
            else if(elev_get_floor_sensor() > 0){
                return IDLE;            
            }   
                //break;//crash it!
            
            default:
                //current_state=UNDEF;
        }


        // Check if stop button and obstruction is active, if so, stop elevator and exit program.
        if (elev_get_obstruction_signal() && elev_get_stop_signal()) {
            elev_set_speed(0);
            break;
        }
    }

    return 0;

}

