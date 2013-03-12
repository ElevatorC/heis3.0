/**
	handels button input/user interaction from panel
*/
#include "ui.h"
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

//Checks button signals and set lamps.
void ui_button_signals(int queues[N_QUEUES][N_FLOORS])
{
	ui_check_buttons(queues);
	ui_set_lamps(queues);
}

//Checks if there are any button pressed on the panel
void ui_check_buttons(int queues[N_QUEUES][N_FLOORS]){
	
	int button;
	int floor;
    
	for(button  = 0; button<N_QUEUES; button++){
        for(floor = 0; floor<N_FLOORS; floor++){
            // Skips non-existing buttons
			if((floor == 0 && button == BUTTON_CALL_DOWN) || (floor == 3 && button == BUTTON_CALL_UP))
			{
                continue;
			}
            else if(ui_get_button_signal(button, floor))
            {
                queues[button][floor] = 1; //TRUE;
            }
        }
    }

}

void ui_set_lamps(int queues[N_QUEUES][N_FLOORS]){
	int queue;
	int floor;
	
	for(queue=0; queue < N_QUEUES; queue++){
		for(floor = 0; floor < N_FLOORS; floor++){
			if(queues[queue][floor] == 1){
				ui_set_button_lamp(queue, floor, 1);
			}
			else if (queues[queue][floor] == 0)
			{
				ui_set_button_lamp(queue, floor, 0);
			}

		}
	}
}
 
void ui_set_button_lamp(int button, int floor, int value)
{
    assert(floor >= 0);
    assert(floor < N_FLOORS);

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

void ui_set_door_open_lamp(int value)
{
    if (value)
        io_set_bit(DOOR_OPEN);
    else
        io_clear_bit(DOOR_OPEN);
}

int ui_get_stop_signal(void)
{
    return io_read_bit(STOP);
}

void ui_set_stop_lamp(int value)
{
    if (value)
	{
        io_set_bit(LIGHT_STOP);
	}

	else
	{
        io_clear_bit(LIGHT_STOP);
	}
}

void ui_set_floor_indicator(int floor)
{
	// assert crashes the program deliberately if it's condition does not hold,
	// and prints an informative error message. Useful for debugging.
    //assert(floor >= 0);
    //assert(floor < N_FLOORS);

    if (floor & 0x02)
        io_set_bit(FLOOR_IND1);
    else
        io_clear_bit(FLOOR_IND1);
        
    if (floor & 0x01)
        io_set_bit(FLOOR_IND2);
    else
        io_clear_bit(FLOOR_IND2);
}

/**
This funcion reads the floor indicator from I/O and returns its
floor value as an integerer. Returns: 0..3
*/
int ui_get_floor_indicator(void)
{

    if(io_read_bit(FLOOR_IND1) == 0 && io_read_bit(FLOOR_IND2) == 0)
        return 0;
    if(io_read_bit(FLOOR_IND1) == 0 && io_read_bit(FLOOR_IND2) == 1)
		return 1;
	if(io_read_bit(FLOOR_IND1) == 1 && io_read_bit(FLOOR_IND2) == 0)
        return 2;
    if(io_read_bit(FLOOR_IND1) == 1 && io_read_bit(FLOOR_IND2) == 1)
        return 3;

    return 0;

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