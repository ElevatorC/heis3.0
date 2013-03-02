/**
	handels button input/user interaction from panel
*/
#include "ui.h"
#include "elev.h"
#include "io.h"
#include "channels.h"

#include <assert.h>

// Number of signals and lamps on a per-floor basis (excl sensor)
#define N_BUTTONS 3

// Matrix of lamp channels indexed by floor and button type. Excuse ugly macro.
#define LCM_SET(f) {LIGHT##f##_UP, LIGHT##f##_DOWN, LIGHT##f##_COMMAND}
static const int lamp_channel_matrix[N_FLOORS][N_BUTTONS] =
  {LCM_SET(1), LCM_SET(2), LCM_SET(3), LCM_SET(4)};


// Matrix of elevator button signal channels indexed by floor and button type.
#define SCM_SET(f) { FLOOR##f##_UP, FLOOR##f##_DOWN, FLOOR##f##_COMMAND }
static const int button_channel_matrix[N_FLOORS][N_BUTTONS] =
  {SCM_SET(1), SCM_SET(2), SCM_SET(3), SCM_SET(4)};

//add to queue.c? to decrease dependancy, change function to add item to queue?
void ui_check_buttons(void){
	int button;
	int floor;
    static int button_pushed[N_BUTTONS][N_FLOORS] = {{0}};
    
	for(button  = 0; button<N_BUTTONS; button++){
        for(floor = 0; floor<N_FLOORS; floor++){
            // Skip non-existing buttons
            if((floor == 0 && button == BUTTON_CALL_DOWN) 
                || (floor == 3 && button == BUTTON_CALL_UP)) 
                continue;
            else if(ui_get_button_signal(button, floor))
            {
                button_pushed[button][floor] = 1; //TRUE;
            }
            // If button is now released, register order
            else if(button_pushed[button][floor] == 1 /*TRUE*/)
            {
                button_pushed[button][floor] = 0; //FALSE;
                //add_to_queues(button,floor,1)
                ui_set_button_lamp(button, floor, 1);
            }
        }
    }

}

//maybe let the queue arrays in queues handle button lamps?
//to decrease dependancy
void ui_set_button_lamp(int button, int floor, int value)
{
	// assert crashes the program deliberately if it's condition does not hold,
	// and prints an informative error message. Useful for debugging.
    assert(floor >= 0);
    assert(floor < N_FLOORS);
    assert(!(button == BUTTON_CALL_UP && floor == N_FLOORS-1));
    assert(!(button == BUTTON_CALL_DOWN && floor == 0));
    assert(button == BUTTON_CALL_UP || button == BUTTON_CALL_DOWN || button ==
            BUTTON_COMMAND);

    if (value == 1)
        io_set_bit(lamp_channel_matrix[floor][button]);
    else
        io_clear_bit(lamp_channel_matrix[floor][button]);        
}

int ui_get_button_signal(int button, int floor)
{
	// assert crashes the program deliberately if it's condition does not hold,
	// and prints an informative error message. Useful for debugging.
    assert(floor >= 0);
    assert(floor < N_FLOORS);
    assert(!(button == BUTTON_CALL_UP && floor == N_FLOORS-1));
    assert(!(button == BUTTON_CALL_DOWN && floor == 0));
    assert(button == BUTTON_CALL_UP || button == BUTTON_CALL_DOWN || button ==
            BUTTON_COMMAND);

    if (io_read_bit(button_channel_matrix[floor][button]))
        return 1;
    else
        return 0;
}

// bruke queues til å fjerne og sette ordre lys?
/*
void ui_remove_order_light(int floor,int direction)
{
	if(direction) {
		switch(floor) 
		{

		case 0:
			CLEAR_BIT(PORT3, LIGHT_UP1);
			CLEAR_BIT(PORT3, LIGHT_ORD1);
			break;

		case 1:
			CLEAR_BIT(PORT3, LIGHT_ORD2);
			CLEAR_BIT(PORT3, LIGHT_UP2);
			break;

		case 2:
			CLEAR_BIT(PORT3, LIGHT_ORD3);
			CLEAR_BIT(PORT3, LIGHT_UP3);
			break;

		case 3:
			CLEAR_BIT(PORT3, LIGHT_DOWN4);
			CLEAR_BIT(PORT3, LIGHT_ORD4);
			break;
		}
	}

	else {
		switch(floor) {
		
		case 0:
			CLEAR_BIT(PORT3, LIGHT_UP1);
			CLEAR_BIT(PORT3, LIGHT_ORD1);
			break;

		case 1:
			CLEAR_BIT(PORT3, LIGHT_ORD2);
			CLEAR_BIT(PORT3, LIGHT_DOWN2);
			break;
		
		case 2:
			CLEAR_BIT(PORT3, LIGHT_ORD3);
			CLEAR_BIT(PORT3, LIGHT_DOWN3);
			break;
		
		case 3:
			CLEAR_BIT(PORT3, LIGHT_DOWN4);
			CLEAR_BIT(PORT3, LIGHT_ORD4);
			break;
		}
	}
}
*/

void ui_set_door_open_lamp(int value)
{
    if (value)
        io_set_bit(DOOR_OPEN);
    else
        io_clear_bit(DOOR_OPEN);
}

//elev_get_stop_signal=value
void ui_set_stop_lamp(int value)
{
    if (value)
        io_set_bit(LIGHT_STOP);
    else
        io_clear_bit(LIGHT_STOP);
}

void ui_set_floor_indicator(int floor)
{
	// assert crashes the program deliberately if it's condition does not hold,
	// and prints an informative error message. Useful for debugging.
    assert(floor >= 0);
    assert(floor < N_FLOORS);

    if (floor & 0x02)
        io_set_bit(FLOOR_IND1);
    else
        io_clear_bit(FLOOR_IND1);
        
    if (floor & 0x01)
        io_set_bit(FLOOR_IND2);
    else
        io_clear_bit(FLOOR_IND2);
}

//own added function reads FLOOR_IND1 and FLOOR_IND2 and returns the floor number which the elevator was at. 
//returns previous floor from 0 to 3
int ui_get_floor_indicator()
{
    int bit1=io_read_bit(FLOOR_IND1);
    int bit2=io_read_bit(FLOOR_IND1);

    if ((bit1 && bit2)==0)
        return 0;
    else if(bit1 == 1 && bit2 == 0)
        return 1;
    else if(bit1 == 0 && bit2 == 1)
        return 2;
    else if(bit1 == 1 && bit2 == 1)
        return 3;
    else
        return -1; //for debugging purposes
}


int ui_init(void)
{
	int i;
	// Zero all floor button lamps
    for (i = 0; i < N_FLOORS; ++i) {
        if (i != 0)
            ui_set_button_lamp(BUTTON_CALL_DOWN, i, 0);

        if (i != N_FLOORS-1)
            ui_set_button_lamp(BUTTON_CALL_UP, i, 0);

        ui_set_button_lamp(BUTTON_COMMAND, i, 0);
    }

    // Clear stop lamp, door open lamp, sets floor indicator to ground floor
    ui_set_stop_lamp(0);
    ui_set_door_open_lamp(0);
	ui_set_floor_indicator(0);
    // Return success.
    return 1;
}