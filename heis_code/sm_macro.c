
#include "elev.h"
#include "ui.h"
#include "queue.h"

#include <stdio.h>
#include <time.h>

static time_t timer;

void startTimer() {
	time(&timer);
}

int checkTimer(int seconds) {
	time_t currentTime;
	time(&currentTime);
	int secondsPassed = difftime(currentTime,timer);
	return secondsPassed > seconds;
}
//takes in the order matrix
current_state_t sm_up(int queues[N_QUEUES][N_FLOORS]) {
	//trengs dette?????????????
	int currentFloor = ui_get_floor_indicator();

	if(elev_get_floor_sensor_signal()>=0)
	{
    	currentFloor = elev_get_floor_sensor_signal();
	}
	elev_set_speed(100);
	//checks if there are orders in the up- or (command- and up-) queue at curren floor
	if(queues[QUEUE_UP][currentFloor] ||(queues[QUEUE_COMMAND][currentFloor] && queues[QUEUE_UP][currentFloor]))
	{
		return STATE_DOOR_OPEN;
	}
	//checks command queue and that the elevator is at an actual floor
	else if(queues[QUEUE_COMMAND][currentFloor] && (elev_get_floor_sensor_signal() != -1))
	{
		return STATE_DOOR_OPEN;
	}
	//if command queue has order above current floor
	else if(!queue_up_empty(queues, QUEUE_COMMAND, currentFloor))
	{
		return STATE_UP;
	}
	//if the top floor is ordered in the down queue and the up queue does not have orders above current floor
	else if(queues[QUEUE_DOWN][N_FLOORS-1] == 1 && queue_up_empty(queues, QUEUE_UP, currentFloor))
	{
			//to make sure this conidtion only opens the door when the elevator is on the top floor
			if(currentFloor != (N_FLOORS-1))
			{
				return STATE_UP;
			}
		return STATE_DOOR_OPEN;
	}
	//then down queue has order on current floor and there is no orders over the current floor in up queue
	else if(queues[QUEUE_DOWN][currentFloor] && queue_up_empty(queues, QUEUE_UP, currentFloor))
	{
		return STATE_DOOR_OPEN;
	}
	//if it is order over current floor in up OR down queue
	else if(!queue_up_empty(queues, QUEUE_UP, currentFloor)||!queue_up_empty(queues, QUEUE_DOWN, currentFloor))
	{
		return STATE_UP;
	}
	//no more orders
	return STATE_IDLE;
}

current_state_t sm_down(int queues[N_QUEUES][N_FLOORS]) {

	int currentFloor=ui_get_floor_indicator();

	if(elev_get_floor_sensor_signal() >= 0)
	{
   		currentFloor = elev_get_floor_sensor_signal();
	}

	elev_set_speed(-100);

	if(queues[QUEUE_DOWN][currentFloor]||((queues[QUEUE_COMMAND][currentFloor]) && queues[QUEUE_DOWN][currentFloor]))
	{
		return STATE_DOOR_OPEN;
	}

	else if(queues[QUEUE_COMMAND][currentFloor] && (elev_get_floor_sensor_signal() != -1))
	{
		return STATE_DOOR_OPEN;
	}

	else if(!queue_down_empty(queues, QUEUE_COMMAND, currentFloor))
	{
		return STATE_DOWN;
	}

	else if(queues[QUEUE_UP][0] == 1 && queue_down_empty(queues, QUEUE_DOWN, currentFloor))
	{
			if(currentFloor != 0)
			{
				return STATE_DOWN;
			}
		return STATE_DOOR_OPEN;
	}
	else if(queues[QUEUE_UP][currentFloor] && queue_down_empty(queues, QUEUE_DOWN, currentFloor))
	{
		return STATE_DOOR_OPEN;
	}

	else if(!queue_down_empty(queues, QUEUE_UP, currentFloor) || !queue_down_empty(queues, QUEUE_DOWN, currentFloor))
	{
		return STATE_DOWN;
	}
	//finished orders
	return STATE_IDLE;
}
//else if????
current_state_t sm_idle(int queues[N_QUEUES][N_FLOORS], int previousState) {
	elev_set_speed(0);

	int currentFloor = ui_get_floor_indicator();

	if(elev_get_floor_sensor_signal()>=0)
	{
   		currentFloor = elev_get_floor_sensor_signal();
	}
	// legge ved commandkøen til denne ifen????????????
	if(!queue_up_empty(queues, QUEUE_UP, currentFloor)||!queue_up_empty(queues, QUEUE_DOWN, currentFloor))
	{
		return STATE_UP;
	}
	//samme her!!!!!!!!!!!!!!!!
	if(!queue_down_empty(queues, QUEUE_UP, currentFloor)||!queue_down_empty(queues, QUEUE_DOWN, currentFloor))
	{
			return STATE_DOWN;
	}
	if(!queue_down_empty(queues, QUEUE_COMMAND, currentFloor))
	{
			return STATE_DOWN;
	}
	if(!queue_up_empty(queues, QUEUE_COMMAND, currentFloor))
	{
			return STATE_UP;
	}
	//handels if none of the conditions above and still between floors -> undef
	if(elev_get_floor_sensor_signal()==-1)
	{
		return STATE_UNDEF;
	}
	return STATE_IDLE;
}

current_state_t sm_stop(int queues[N_QUEUES][N_FLOORS]) {
    elev_set_speed(0);
    ui_set_stop_lamp(1);
    //this takes the elevator out of the stop state
    if(queue_has_orders(queues) && !elev_get_stop_signal())
    {
    	ui_set_stop_lamp(0);
        return STATE_UNDEF;
    }
    queue_clear(queues);
	return STATE_STOP;
}
//undefined floor state
current_state_t sm_undef(void){
	if(elev_get_floor_sensor_signal()==-1)
	{
		//by default take the elevator to the floor below if between floors
		elev_set_speed(-100);
		return STATE_UNDEF;
	}
	return STATE_IDLE;
}

//1 is up and 0 is down
current_state_t sm_door_open(int queues[N_QUEUES][N_FLOORS], int previousState) {

	elev_set_speed(0);
	int timer = 0;
	int currentFloor = elev_get_floor_sensor_signal();

	ui_set_door_open_lamp(1);
	startTimer();
	//bool
	while(timer != 1)
	{
		if(elev_get_stop_signal())
		{
            return STATE_STOP;
		}
		//listens for button signals, to take stop and order while the door is open
		ui_button_signals(queues);
		timer = checkTimer(3);
	}
	while(elev_get_obstruction_signal() != 0)
	{
		if(elev_get_stop_signal())
		{
			ui_set_door_open_lamp(0);
            return STATE_STOP;
		}
		ui_set_door_open_lamp(1);
	}
	ui_set_door_open_lamp(0);

    if (previousState == STATE_UP)
	{
		queues[QUEUE_COMMAND][currentFloor] = 0;
		//if on the top floor
		if(currentFloor == (N_FLOORS-1))
		{
			queues[QUEUE_DOWN][currentFloor] = 0;
		}
		//if orders over current in up queue
		else if(!queue_up_empty(queues, QUEUE_UP, currentFloor))
		{
			queues[QUEUE_UP][currentFloor] = 0;
		}
		//if no orders over current in up queue AND orders below in down queue
		else if(queue_up_empty(queues, QUEUE_UP, currentFloor) && !queue_down_empty(queues, QUEUE_DOWN, currentFloor))
		{
			queues[QUEUE_DOWN][currentFloor] = 0;
			return STATE_DOWN;
		}
		return STATE_UP;
	}

	else if(previousState == STATE_DOWN)
	{
		queues[QUEUE_COMMAND][currentFloor] = 0;
		//if on bottom floor
		if(currentFloor == 0)
		{
			queues[QUEUE_UP][currentFloor] = 0;
			//return IDLE??????????????????????????????
		}
		//if orders below in down queue
		else if(!queue_down_empty(queues, QUEUE_DOWN, currentFloor))
		{
			queues[QUEUE_DOWN][currentFloor] = 0;
		}
		//if no orders below in down queue and orders over in up queue
		else if(queue_down_empty(queues, QUEUE_DOWN, currentFloor) && !queue_up_empty(queues, QUEUE_UP, currentFloor))
		{
			queues[QUEUE_UP][currentFloor] = 0;
			return STATE_UP;
		}
		return STATE_DOWN;
	}
	return STATE_IDLE;
}
