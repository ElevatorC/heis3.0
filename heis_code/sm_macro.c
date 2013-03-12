/**
	handels the elevator logic with macros for the states
	that are called in main and returns the state the state
	machine should go too.
*/

#include "elev.h"
#include "ui.h"
#include "queue.h"
#include "sm_macro.h"

#include <stdio.h>
#include <time.h>

static time_t timer;

//starts timer.
void startTimer()
{
	time(&timer);
}

/**
@Param number of seconds
Checks the timer if spescified seconds has passed.
Returns 1(true) if the amount of seconds has not 
passed it returns 0(false)
*/
int checkTimer(int seconds)
{
	time_t currentTime;
	time(&currentTime);
	int secondsPassed = difftime(currentTime,timer);

	return (secondsPassed > seconds);
}

//Makes elev check the floor sensors signals
int sm_check_floor_sensor_signal(void){
	return elev_get_floor_sensor_signal();
}

//Makes ui check for button input from elevator panel.
void sm_check_button_signals(int queues[N_QUEUES][N_FLOORS])
{
	ui_button_signals(queues);
}

//Makes UI set the floor indicator lamp
void sm_floor_indicator(int floor)
{
	if(floor != -1) //checks if the elevator is at a valid floor
	{
	ui_set_floor_indicator(floor);
	}

}

//Checks if the stop button is pushed
int sm_stop_signal(){

	if(ui_get_stop_signal())
	{
		return 1; //true
	}

	else
	{
		return 0; //false
	}

}

/*
Statemachine macros takes in the queue matrix execute elevator
logic and checks itappropriately.
*/

sm_state_t sm_move_up(int queues[N_QUEUES][N_FLOORS]){

	//Checks if the door is forced open if it is stop the elevator and light open door.
	if(elev_get_obstruction_signal()) 
	{
		return STATE_OPEN_DOOR;
	}

	//Sets currentFloor to the last floor seen.
	int currentFloor = ui_get_floor_indicator();

	//Sets currentFloor to the current floor it is at.
	if(elev_get_floor_sensor_signal()>=0)
	{
     currentFloor = elev_get_floor_sensor_signal();
	}
	
	//Sets the motor speed to 100 upwards.
	elev_set_speed(100);

	//Checks if there are orders in the up- or (command- and up-) queue at current floor.
	if(queues[QUEUE_UP][currentFloor] ||(queues[QUEUE_COMMAND][currentFloor] && queues[QUEUE_UP][currentFloor]))
	{	
		if(elev_get_floor_sensor_signal() != -1) //Checks if it is at a floor.
		{
		  return STATE_OPEN_DOOR;
		}
		else 
		{
		  return STATE_MOVE_UP;
		}
	}

	//Checks if the command queue has an order at current floor and that the elevator is at an actual floor.
	else if(queues[QUEUE_COMMAND][currentFloor] && (elev_get_floor_sensor_signal() != -1))
	{
		return STATE_OPEN_DOOR;
	}
	
	//Checks if the command queue has order above the current floor
	else if(!queue_from_and_up_empty(queues, QUEUE_COMMAND, currentFloor))
	{
		return STATE_MOVE_UP;
	}

	//Checks if the top floor is ordered in the down queue and the up queue does not have orders above the current floor
	else if(queues[QUEUE_DOWN][N_FLOORS-1] == 1 && queue_from_and_up_empty(queues, QUEUE_UP, currentFloor))
	{	
			//To make sure this conidtion only opens the door when the elevator is on the top floor.
			if(currentFloor != N_FLOORS-1)
			{
				return STATE_MOVE_UP;
			}
			else
			{
				return STATE_OPEN_DOOR;
			}
	}

	//Checks if the down queue has order on current floor and there is no orders over the current floor in up queue.
	else if(queues[QUEUE_DOWN][currentFloor] && queue_from_and_up_empty(queues, QUEUE_UP, currentFloor))
	{
		if(elev_get_floor_sensor_signal() != -1) //Checks if it is at a floor.
		{
		  return STATE_OPEN_DOOR;
		}
		else 
		{
		  return STATE_MOVE_UP;
		}
	}
	
	//Checks if it is order over current floor in up or down queue.
	else if(!queue_from_and_up_empty(queues, QUEUE_UP, currentFloor)||!queue_from_and_up_empty(queues, QUEUE_DOWN, currentFloor))
	{
		return STATE_MOVE_UP;
	}

	//Finished all upwards orders.
	else
		return STATE_IDLE;
}

sm_state_t sm_move_down(int queues[N_QUEUES][N_FLOORS]){

	//Checks if the door is forced open if it is stop the elevator and light open door and stops the elevator.
	if(elev_get_obstruction_signal()) 
	{
		return STATE_OPEN_DOOR;
	}
	
	//Sets currentFloor to the last floor seen.
	int currentFloor = ui_get_floor_indicator();

	//Sets currentFloor to the current floor it is at.
	if(elev_get_floor_sensor_signal()>=0)
	{
	  currentFloor = elev_get_floor_sensor_signal();
	}
	
	//Sets the motor speed to 100 downwards.
	elev_set_speed(-100);

	//Checks if there are orders in the down- or (command- and down-) queue at current floor.
	if((queues[QUEUE_DOWN][currentFloor]||((queues[QUEUE_COMMAND][currentFloor]) && queues[QUEUE_DOWN][currentFloor])))
	{
		if(elev_get_floor_sensor_signal() != -1) //Checks if it is at a floor.
		{
		  return STATE_OPEN_DOOR;
		}
		else
		{
		  return STATE_MOVE_DOWN;
		}
	}

	//Checks if the command queue has an order at current floor and that the elevator is at an actual floor.
	else if(queues[QUEUE_COMMAND][currentFloor] && (elev_get_floor_sensor_signal() != -1))
	{
		return STATE_OPEN_DOOR;
	}

	//Checks if the down queue has any orders bellow current floor.
	else if(!queue_from_and_down_empty(queues, QUEUE_COMMAND, currentFloor))
	{
		return STATE_MOVE_DOWN;
	}
	
	//Checks if the bottom floor is ordered in the up queue and the down queue does not have orders bellow the current floor.
	else if(queues[QUEUE_UP][0] == 1 && queue_from_and_down_empty(queues, QUEUE_DOWN, currentFloor))
	{		

		if(currentFloor != 0)
		{
			return STATE_MOVE_DOWN;
		}
		else
		{
		  return STATE_OPEN_DOOR;
		}

	}
	
	//Checks if the up queue has order on current floor and there is no orders over the current floor in the down queue.
	else if(queues[QUEUE_UP][currentFloor] && queue_from_and_down_empty(queues, QUEUE_DOWN, currentFloor))
	{
		if(elev_get_floor_sensor_signal() != -1) //Checks if it is at a floor.
		{
		  return STATE_OPEN_DOOR;
		}
		else
		{
		  return STATE_MOVE_DOWN;
		}
	}

	//Checks if it is order bellow current floor in up or down queue.
	else if(!queue_from_and_down_empty(queues, QUEUE_UP, currentFloor) || !queue_from_and_down_empty(queues, QUEUE_DOWN, currentFloor))
	{
		return STATE_MOVE_DOWN;
	}
	
	//Finished all downwards orders.
	else
	{
	return STATE_IDLE;
	}
}

sm_state_t sm_idle(int queues[N_QUEUES][N_FLOORS], int previousState) {
	
	//Stops the elevator,
	elev_set_speed(0);
	
	//Sets currentFloor to the last floor seen.
	int currentFloor = ui_get_floor_indicator();

	//Sets currentFloor to the current floor it is at.
	if(elev_get_floor_sensor_signal()>=0)
	{
     currentFloor = elev_get_floor_sensor_signal();
	}
	
	//Checks if there are any floors over the current floor and goes to move up state.
	if(!queue_from_and_up_empty(queues, QUEUE_UP, currentFloor)||!queue_from_and_up_empty(queues, QUEUE_DOWN, currentFloor)){
				return STATE_MOVE_UP;
	}
	
	//Checks if there are any floors under the current floor and goes to move down state.
	else if(!queue_from_and_down_empty(queues, QUEUE_UP, currentFloor)||!queue_from_and_down_empty(queues, QUEUE_DOWN, currentFloor)){
				return STATE_MOVE_DOWN;
	}

	//Command is put in its own else if for readability.
	//Checks if there are any orders in the command under the current floor.
	else if(!queue_from_and_down_empty(queues, QUEUE_COMMAND, currentFloor))
	{
				return STATE_MOVE_DOWN;
	}
	
	//Checks if there are any orders in the command over the current floor.
	else if(!queue_from_and_up_empty(queues, QUEUE_COMMAND, currentFloor))
	{
				return STATE_MOVE_UP;
	}

	else if(elev_get_floor_sensor_signal() == -1)
	{
		return STATE_UNDEFINED; //If the elevator is not on a floor in idle the state is undefined.
	}

	else
	{
		return STATE_IDLE; //Nothing to do return to idle
	}
}

sm_state_t sm_stop(int queues[N_QUEUES][N_FLOORS]) {
	 
    elev_set_speed(0); //Stops the elevator
    ui_set_stop_lamp(1); //Light the stop light
   
	//Checks if any queues has order if not return stop state.
	if(queue_has_orders(queues) && !ui_get_stop_signal())
    {
    	ui_set_stop_lamp(0);//Turns of the stop light
        return STATE_UNDEFINED;
    }
	
	else
	{
    queue_clear_all_orders(queues); //Clear all the queue orders.
	return STATE_STOP;
	}
	
}

sm_state_t sm_undefined(void){

	ui_set_door_open_lamp(0); //Closes the door after stop state

	if(elev_get_floor_sensor_signal()==-1){
		elev_set_speed(-100);
		return STATE_UNDEFINED;	
	}
	return STATE_IDLE;
}

sm_state_t sm_open_door(int queues[N_QUEUES][N_FLOORS],sm_state_t previousState){

	elev_set_speed(0); //Stops the elevator
	
	int currentFloor = elev_get_floor_sensor_signal(); //Gets the current floor the elevator.

	// If the elevator has an obstruction between floors stop the elevator
	while(elev_get_obstruction_signal() && currentFloor == -1)
	{
		return previousState;
	}
	
	if(currentFloor == -1)
	{
		return previousState;
	}

	ui_set_door_open_lamp(1); //Open doors
	
	startTimer();
	while(checkTimer(3) != 1){

		if(ui_get_stop_signal()) //Checks stop button for input while the door is open.
		{
            return STATE_STOP;
		}
		
		ui_button_signals(queues); //Checks for button input while the door is open.
		 
	}

	while(elev_get_obstruction_signal() != 0){

		ui_button_signals(queues); //Checks for button input while the obstruction is active.

		if(ui_get_stop_signal())  //Checks stop button for input when obstructed.
		{
            return STATE_STOP;
		}  

		ui_set_door_open_lamp(1); //Sets door lamp to Open.

	}

	ui_set_door_open_lamp(0); //Sets door lamp to closed.

	//If it came from the state of moving up the it checks the following cases.
    if (previousState == STATE_MOVE_UP)
	{
		queues[QUEUE_COMMAND][currentFloor] = 0; //Reached ordered command floor and clears queue element.

		//If the top floor is reached it switches state to check if there is any orders bellow.
		if(currentFloor == N_FLOORS-1) 
		{
			queues[QUEUE_DOWN][currentFloor] = 0;  //Clear ordered floor in down queue.
			return STATE_MOVE_DOWN;
		}

		//Reached queue element in up that is ordered and clears it.
		else if(!queue_from_and_up_empty(queues, QUEUE_UP, currentFloor)) 
		{
			queues[QUEUE_UP][currentFloor] = 0;
		}
		
		//If no orders over current in up queue AND orders below in down queue.
		else if(queue_from_and_up_empty(queues, QUEUE_UP, currentFloor) && !queue_from_and_down_empty(queues, QUEUE_DOWN, currentFloor))
		{ 
			queues[QUEUE_DOWN][currentFloor] = 0;
			return STATE_MOVE_DOWN; 
		}

		//More orders in up queue so returns to its previous state and continoues.
		return STATE_MOVE_UP;

	}
	
	//If it came from the state of moving down the it checks the following cases.
	else if(previousState == STATE_MOVE_DOWN)
	{
		queues[QUEUE_COMMAND][currentFloor] = 0; //Reached ordered command floor and clears queue element

		//If the bottom floor is reached it switches state to check if there is any orders abow.
		if(currentFloor == 0)
		{
			queues[QUEUE_UP][currentFloor] = 0;
			return STATE_MOVE_UP;
		}

		//Reached queue element in down that is ordered and clears it.
		else if(!queue_from_and_down_empty(queues, QUEUE_DOWN, currentFloor))
		{
			queues[QUEUE_DOWN][currentFloor] = 0;
		}

		//If no orders below in down queue AND orders over in up queue.
		else if(queue_from_and_down_empty(queues, QUEUE_DOWN, currentFloor) && !queue_from_and_up_empty(queues, QUEUE_UP, currentFloor))
		{
			queues[QUEUE_UP][currentFloor] = 0;
			return STATE_MOVE_UP;
		}
		
		//More orders in down queue so returns to its previous state and continoues.
		return STATE_MOVE_DOWN;

	}
	
	//Returns to previous state
	else if(previousState == STATE_MOVE_UP)
	{
			return STATE_MOVE_UP;
	}

	else
	{
			return STATE_MOVE_DOWN;
	}

}

// Initialize statemachine
int sm_init(){

	// Initialize hardware
    if (!elev_init())
    {
        printf(__FILE__ ": Unable to initialize elevator hardware\n");
        return 1;
    }

	// Initialize User interface
    if (!ui_init())
    {
        printf(__FILE__ ": Unable to initialize elevator user interface\n");
        return 1;
    }

   return 1;

}