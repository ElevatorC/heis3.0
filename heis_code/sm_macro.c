
#include "elev.h"
#include "ui.h"
#include "queue.h"

#include <unistd.h>
#include <stdio.h>


current_state_t sm_up(int queues[N_QUEUES][N_FLOORS]){
	int currentFloor = ui_get_floor_indicator(); //virker kanskje ikke
	if(elev_get_floor_sensor_signal()>=0){
     currentFloor = elev_get_floor_sensor_signal();
	}
	elev_set_speed(100);
	if(queues[QUEUE_UP][currentFloor]){
		printf("\ncurrentFloor: %i\n",currentFloor);
		return STATE_DOOR_OPEN;	
	}
	else if(queues[QUEUE_DOWN][currentFloor] && queue_up_empty(queues, QUEUE_UP, currentFloor)){
		printf("\ncurrentFloor: %i\n",currentFloor);
		return STATE_DOOR_OPEN;	
	}
	else if(!queue_up_empty(queues, QUEUE_UP, currentFloor)||!queue_up_empty(queues, QUEUE_DOWN, currentFloor)){
			printf("\nQueue Not empty Current floor is: %i\n",currentFloor);
			return STATE_UP;
	}
	//finished orders
	return STATE_IDLE;
}

current_state_t sm_down(int queues[N_QUEUES][N_FLOORS]){
	int currentFloor=ui_get_floor_indicator(); //virker kanskje ikke
	if(elev_get_floor_sensor_signal() >= 0){
   		currentFloor = elev_get_floor_sensor_signal();
	}
	printf("\nState down failer\n");
	elev_set_speed(-100);
	printf("\ncurrentFloor\n");
	if(queues[QUEUE_DOWN][currentFloor]){
		printf("\ncurrentFloor: %i\n",currentFloor);
		return STATE_DOOR_OPEN;	
	}
	else if(queues[QUEUE_UP][currentFloor] && queue_down_empty(queues, QUEUE_DOWN, currentFloor)){
		printf("\ncurrentFloor: %i\n",currentFloor);
		return STATE_DOOR_OPEN;	
	}
	else if(!queue_down_empty(queues, QUEUE_UP, currentFloor) || !queue_down_empty(queues, QUEUE_DOWN, currentFloor)){
		printf("\nQueue Not empty Current floor is: %i\n",currentFloor);			
		return STATE_DOWN;
	}
	//finished orders
	return STATE_IDLE;
}

current_state_t sm_idle(int queues[N_QUEUES][N_FLOORS], int previousState) {
	elev_set_speed(0);
	int currentFloor = ui_get_floor_indicator(); //virker kanskje ikke
	if(elev_get_floor_sensor_signal()>=0){
   		currentFloor = elev_get_floor_sensor_signal();
	}
//	if(previousState == STATE_DOWN){
		if(!queue_up_empty(queues, QUEUE_UP, currentFloor)||!queue_up_empty(queues, QUEUE_DOWN, currentFloor)){
				return STATE_UP;
		}
	//}
	//if(previousState == STATE_UP){
		if(!queue_down_empty(queues, QUEUE_UP, currentFloor)||!queue_down_empty(queues, QUEUE_DOWN, currentFloor)){
				return STATE_DOWN;
			}
	//}
		
	if(elev_get_floor_sensor_signal()==-1)
		return STATE_UNDEF;
   // if(elev_get_obstruction_signal() == 1)
	//	return STATE_DOOR_OPEN;
	
		return STATE_IDLE; //kanskje fjernes (mulig feil i logikk)
}

current_state_t sm_stop(int queues[N_QUEUES][N_FLOORS]) {
    elev_set_speed(0);
    queue_clear(queues);
	//while check if any quueues has order if not return stop state!
    //evt ta høyde for obstruction
	// en check på is empty
    return STATE_IDLE;
}

current_state_t sm_undef(void){
	if(elev_get_floor_sensor_signal()==-1){
		elev_set_speed(-100);
		return STATE_UNDEF;	
	}
	return STATE_IDLE;
}

//1 is up and 0 is down
current_state_t sm_door_open(int queues[N_QUEUES][N_FLOORS], int previousState){
	elev_set_speed(0);
	int currentFloor = elev_get_floor_sensor_signal(); 

	ui_set_door_open_lamp(1);
	sleep(3);
	while(elev_get_obstruction_signal() != 0){                
		ui_set_door_open_lamp(1);
		sleep(1);
		ui_set_door_open_lamp(0);
		sleep(1);
		printf("\nObstruction");
    }
	ui_set_door_open_lamp(0);

	//if(queues[QUEUE_COMMAND][elev_get_floor_sensor_signal()]){
	//	queues[QUEUE_COMMAND][elev_get_floor_sensor_signal()] = 0;
	//	printf("\ncommand Queue");
		
//	}
    if (previousState == STATE_UP)
	{
		// dette må også fikses
		queues[QUEUE_UP][elev_get_floor_sensor_signal()] = 0;
	if(queue_up_empty(queues, QUEUE_UP, currentFloor)){ // and command over currentFloor
		queues[QUEUE_DOWN][elev_get_floor_sensor_signal()] = 0;
		return STATE_DOWN;
	}
		printf("\nup Queue");
		return STATE_UP;
	}
	else if(previousState == STATE_DOWN){
		// dette må fikses
		queues[QUEUE_DOWN][elev_get_floor_sensor_signal()] = 0;
	if(queue_down_empty(queues, QUEUE_DOWN, currentFloor)){
		queues[QUEUE_UP][elev_get_floor_sensor_signal()] = 0;
		return STATE_UP;
	}
		printf("\ndown Queue");
		return STATE_DOWN;
	}
	
	// missing something here, ether it needs to know which
	//state it came from to delete 

	return STATE_IDLE;

}
