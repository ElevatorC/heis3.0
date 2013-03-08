
#include "elev.h"
#include "ui.h"
#include "queue.h"

#include <stdio.h>
#include <time.h>

static time_t timer;

void startTimer(){
	time(&timer);
}

int checkTimer(int seconds){
	time_t currentTime;
	time(&currentTime);
	int secondsPassed = difftime(currentTime,timer);
	return secondsPassed > seconds;
}

current_state_t sm_up(int queues[N_QUEUES][N_FLOORS]){
	
	int currentFloor = ui_get_floor_indicator();
	
	if(elev_get_floor_sensor_signal()>=0){
     currentFloor = elev_get_floor_sensor_signal();
	}
	
	elev_set_speed(100);

	if(queues[QUEUE_UP][currentFloor] ||(queues[QUEUE_COMMAND][currentFloor] && queues[QUEUE_UP][currentFloor])){
		return STATE_DOOR_OPEN;	
	}
	else if(queues[QUEUE_COMMAND][currentFloor] && (elev_get_floor_sensor_signal() != -1))
	{
		return STATE_DOOR_OPEN;
	}

	else if(!queue_up_empty(queues, QUEUE_COMMAND, currentFloor)){
		return STATE_UP;
	}
	
	else if(queues[QUEUE_DOWN][3] == 1 && queue_up_empty(queues, QUEUE_UP, currentFloor))
	{		
			if(currentFloor != 3){
				return STATE_UP;
			}
		return STATE_DOOR_OPEN;
	}

	else if(queues[QUEUE_DOWN][currentFloor] && queue_up_empty(queues, QUEUE_UP, currentFloor))
	{
		return STATE_DOOR_OPEN;	
	}
	
	else if(!queue_up_empty(queues, QUEUE_UP, currentFloor)||!queue_up_empty(queues, QUEUE_DOWN, currentFloor))
	{
		return STATE_UP;
	}

	
	//finished orders
	return STATE_IDLE;
}

current_state_t sm_down(int queues[N_QUEUES][N_FLOORS]){
	
	int currentFloor=ui_get_floor_indicator(); 

	if(elev_get_floor_sensor_signal() >= 0){
   		currentFloor = elev_get_floor_sensor_signal();
	}

	elev_set_speed(-100);

	if(queues[QUEUE_DOWN][currentFloor]||((queues[QUEUE_COMMAND][currentFloor]) && queues[QUEUE_DOWN][currentFloor])){
		return STATE_DOOR_OPEN;	
	}

	else if(queues[QUEUE_COMMAND][currentFloor] && (elev_get_floor_sensor_signal() != -1))
	{
		return STATE_DOOR_OPEN;
	}

	else if(!queue_down_empty(queues, QUEUE_COMMAND, currentFloor)){
		return STATE_DOWN;
	}
	
	else if(queues[QUEUE_UP][0] == 1 && queue_down_empty(queues, QUEUE_DOWN, currentFloor)) //bug med (2ned og 2opp 3ned og 3opp aktiv + ende pungt stopper heisen)
	{		
			if(currentFloor != 0){
				return STATE_DOWN;
			}
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
		if(!queue_down_empty(queues, QUEUE_COMMAND, currentFloor))
		{
				return STATE_DOWN;
		}
		if(!queue_up_empty(queues, QUEUE_COMMAND, currentFloor))
		{
				return STATE_UP;
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
    ui_set_stop_lamp(1);
    if(queue_has_orders(queues) && !elev_get_stop_signal())
    {

    	ui_set_stop_lamp(0); //Skrur av stoplamp
        return STATE_UNDEF; // IDLE fiks
    }
    queue_clear(queues);
	return STATE_STOP;
	
	//while check if any quueues has order if not return stop state!
    //evt ta høyde for obstruction
	// en check på is empty
    
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
	int timer = 0;
	int currentFloor = elev_get_floor_sensor_signal(); 

	ui_set_door_open_lamp(1);
	startTimer();
	while(timer != 1){

		if(elev_get_stop_signal())
		{
            return STATE_STOP;
		}
			
		ui_button_signals(queues);//finner button signals
		timer = checkTimer(3);
		 
	}

	while(elev_get_obstruction_signal() != 0){
		
		if(elev_get_stop_signal())
		{
			ui_set_door_open_lamp(0);
            return STATE_STOP;
		}  

		ui_set_door_open_lamp(1);
		printf("\nObstruction");

	    }
     //kanskje lage no som ikker slukker lampen med en gang
	ui_set_door_open_lamp(0);

	//if(queues[QUEUE_COMMAND][elev_get_floor_sensor_signal()]){
	//	queues[QUEUE_COMMAND][elev_get_floor_sensor_signal()] = 0;
	//	printf("\ncommand Queue");
		
//	}
    if (previousState == STATE_UP)
	{
		queues[QUEUE_COMMAND][currentFloor] = 0;
		// dette må også fikses
		if(currentFloor == 3){
			queues[QUEUE_DOWN][currentFloor] = 0;
		}
		else if(!queue_up_empty(queues, QUEUE_UP, currentFloor)){
			queues[QUEUE_UP][currentFloor] = 0;
		}
		else if(queue_up_empty(queues, QUEUE_UP, currentFloor) && !queue_down_empty(queues, QUEUE_DOWN, currentFloor))
		{ // and command over currentFloor
			queues[QUEUE_DOWN][currentFloor] = 0;
			return STATE_DOWN;
		}
		
		printf("\nup Queue");
		return STATE_UP;
	}
	
	else if(previousState == STATE_DOWN)
	{
		queues[QUEUE_COMMAND][currentFloor] = 0;

		if(currentFloor == 0){
			queues[QUEUE_UP][currentFloor] = 0;
		}
		//se over denne
		else if(!queue_down_empty(queues, QUEUE_DOWN, currentFloor)){
			queues[QUEUE_DOWN][currentFloor] = 0;
		}

		else if(queue_down_empty(queues, QUEUE_DOWN, currentFloor) && !queue_up_empty(queues, QUEUE_UP, currentFloor))
		{
			queues[QUEUE_UP][currentFloor] = 0;
			return STATE_UP;
		}
		

		printf("\ndown Queue");
		return STATE_DOWN;

	}
	
	// missing something here, ether it needs to know which
	//state it came from to delete 

	return STATE_IDLE;

}
