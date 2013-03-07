
#include "elev.h"
#include "ui.h"
#include "queue.h"

#include <unistd.h>


//sjekk stopp i main
void sm_button_signals(int queues[N_QUEUES][N_FLOORS],int currentFloor){
	ui_check_buttons(queues);
	ui_set_lamps(queues);
	ui_set_floor_indicator(currentFloor);
}

current_state_t sm_idle(int queues[N_QUEUES][N_FLOORS], int previousState) {
	int currentFloor = ui_get_floor_indicator();
	elev_set_speed(0);
	
	if(previousState == STATE_IDLE)
	{//bare la inn noen verdier for testing
		if(queue_queueType_has_orders(queues,QUEUE_UP)){
		//check up queue
			return STATE_UP;
		}
		else if(queue_queueType_has_orders(queues,QUEUE_DOWN)){
			return STATE_DOWN;
		//check down queue
		}
	}
	
	else if(previousState == STATE_UP){
		return STATE_DOWN;
	}
	
	else if(previousState == STATE_DOWN)
	{
		return STATE_UP;
	}

	return STATE_IDLE;
}

current_state_t sm_up(int queues[N_QUEUES][N_FLOORS]){
	int currentFloor = ui_get_floor_indicator();
	//trenger knaskje ikke dette, bruke ui_get_floor_indicator er nok
	if (elev_get_floor_sensor_signal() >= 0)
	{
		currentFloor = elev_get_floor_sensor_signal();
	}

	elev_set_speed(300);

	if (queues[QUEUE_UP][currentFloor] == 1)
	{
		return STATE_DOOR_OPEN;
	}

	else if (queue_from_and_up_empty(queues,QUEUE_UP,currentFloor))
	{
		return STATE_IDLE;
	}
	return STATE_UP;
}

current_state_t sm_down(int queues[N_QUEUES][N_FLOORS]){
	int currentFloor = ui_get_floor_indicator();
	
	if (elev_get_floor_sensor_signal() >= 0)
	{
		currentFloor = elev_get_floor_sensor_signal();
	}

	elev_set_speed(-300);


	if (queues[QUEUE_DOWN][currentFloor] == 1)
	{
		return STATE_DOOR_OPEN;
	}

	else if (queue_from_and_down_empty(queues,QUEUE_DOWN,currentFloor))
	{
		return STATE_IDLE;
	}
	return STATE_DOWN;
}

current_state_t sm_stop(int queues[N_QUEUES][N_FLOORS]) {
    elev_set_speed(0);
    queue_clear(queues);
	ui_set_stop_lamp(1);
	if (!queue_has_orders(queues))
	{
		ui_set_stop_lamp(1);
		sleep(1);
		ui_set_stop_lamp(0);
		return STATE_STOP;
	}
    return STATE_IDLE;
}

//require previous state where STATE_UP and STATE_DOWN is defined in enum
current_state_t sm_door_open(int queues[N_QUEUES][N_FLOORS], int previousState){
	elev_set_speed(0);

	int currentFloor = elev_get_floor_sensor_signal();
	
	ui_set_door_open_lamp(1);
	sleep(3);
	while(elev_get_obstruction_signal() != 0){                
		ui_set_door_open_lamp(1);
		sleep(1);
		ui_set_door_open_lamp(0);
    }
	
	ui_set_door_open_lamp(0);
	queues[QUEUE_COMMAND][floor] = 0;
	if(previousState == STATE_UP)
	{
		queues[QUEUE_UP][currentFloor] = 0; // kanskje ha en funksjon for det for å gjøre det modulært?
		return STATE_UP;
	}
	else if(previousState == STATE_DOWN)
	{
		queues[QUEUE_DOWN][currentFloor] = 0;
		return STATE_DOWN;
	}
	return STATE_IDLE;
}

current_state_t sm_undef(void){
	if (elev_get_floor_sensor_signal() == -1)
	{
		elev_set_speed(-100);
		return STATE_UNDEF;
	}
		else if(elev_get_floor_sensor_signal() >= 0){
		return STATE_IDLE;
	}			
}