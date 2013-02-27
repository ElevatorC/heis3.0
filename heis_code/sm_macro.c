#include "elev.h"

int sm_up(){
    if(elev_get_stop_signal()) {
        return STOP;    
    }
    else if(queue_is_empty(queues.queue_up)){
        return IDLE;    
    }
    for(int floor=0;floor<N_FLOORS;floor++){
        //denne logikken fungerer kanskje ikke..
        if(elev_check_floor_sensor() == (queues.queue_up[floor] || queues.queue_command[floor])){
        //Open door
            return OPEN_DOOR;
        }
    } else {
    elev_set_speed(300);
        return UP;    
    }

}

int sm_down(){
    if(elev_get_stop_signal()) {
        return STOP;    
    }
    else if(queue_is_empty(queues.queue_down)){
        return IDLE;    
    }
    for(int floor=3;floor>=0;floor--){
        if(elev_check_floor_sensor() == (queues.queue_down[floor] || queues.queue_command[floor])){
        //Open door
            return OPEN_DOOR;
        }
    } else {
    elev_set_speed(-300);
        return DOWN;    
    }
}
int sm_idle() {
    elev_set_speed(0)
    if(elev_get_floor_sensor() < 0){
        return UNDEF;     
    }    
       //ser etter elementer i respektiv kø 
    if(queues_check()==0){
    
        for(int floor=0;floor<N_FLOORS;floor++){
            if(queues.queue_command[floor]==1){

                //spesialtilfelle må enders i state!       
                if(elev_get_floor_sensor()==floor || elev_get_floor_sensor()<floor){
                //Dette er bare for å åpne dør, via upstate
                    return UP;
                }
                else {
                    return DOWN;
                }
            }                
        }
    }
//Fryktelig forenklet utgave
    //1=oppkø
    else if(queues_check == 1){
        return UP;
    }
    //2=nedkø
    else if(quees_checks == 2) {
        return DOWN;    
    }
}






    } else if(queues_check()==1){
        return UP;
    } else if(queues_check()==0 {
        return DOWN;
    } else {
        return IDLE;
    }
}
int sm_stop() {
    elev_set_speed(0);    
    queue_clear();
    //evt ta høyde for obstruction
    return IDLE;
}
