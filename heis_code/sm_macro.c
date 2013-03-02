#include "elev.h"
#include "ui.h"
#include "queue.h"


current_state_t sm_up(int queues[N_QUEUES][N_FLOORS]){
    if(elev_get_stop_signal()) {
        return STATE_STOP;    
    }
	else if(queue_is_empty(queues[QUEUE_UP])){
        return STATE_IDLE;    
    }
    for(int floor=0;floor<N_FLOORS;floor++){
        //denne logikken fungerer kanskje ikke..
		if(elev_get_floor_sensor_signal() == (queues[QUEUE_UP][floor] || queues[QUEUE_COMMAND][floor])){
        //Open door
            return STATE_DOOR_OPEN;
        }
    }
	//Dette gikk ikke!
	/*
	else {
    elev_set_speed(300);
        return STATE_UP;    
    }
	*/

}

current_state_t sm_down(int queues[N_QUEUES][N_FLOORS]){
    if(elev_get_stop_signal()) {
        return STATE_STOP;    
    }
	else if(queue_is_empty(queues[QUEUE_DOWN])){
        return STATE_IDLE;    
    }
    for(int floor=3;floor>=0;floor--){
		if(elev_get_floor_sensor_signal() == (queues[QUEUE_DOWN][floor] || queues[QUEUE_COMMAND][floor])){
        //Open door
            return STATE_DOOR_OPEN;
        }
    } 
	
	/*else {
    elev_set_speed(-300);
        return STATE_DOWN;    
    }
	*/
}

int sm_idle(int queues[N_QUEUES][N_FLOORS]) {
    elev_set_speed(0);
	if(elev_get_floor_sensor_signal() < 0){
        return STATE_UNDEF;     
    }    
       //ser etter elementer i respektiv kø 
    if(queue_check_queues(queues)==0){
    
        for(int floor=0;floor<N_FLOORS;floor++){
			if(queues[QUEUE_COMMAND][floor] == 1){

                //spesialtilfelle må enders i state!       
                if(elev_get_floor_sensor_signal() == floor || elev_get_floor_sensor_signal()<floor){
                //Dette er bare for å åpne dør, via upstate
                    return STATE_UP;
                }
                else {
                    return STATE_DOWN;
                }
            }                
        }
    }
	//Fryktelig forenklet utgave
    //1=oppkø
    else if(queue_check_queues(queues) == 1){
        return STATE_UP;
    }

    //2=nedkø
    else if(queue_check_queues(queues) == 2) {
        return STATE_DOWN;    
    }
}

current_state_t sm_stop(int queues[N_QUEUES][N_FLOORS]) {
    elev_set_speed(0);    
    queue_clear(queues);
    //evt ta høyde for obstruction
    return STATE_IDLE;
}
