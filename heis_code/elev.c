// Wrapper for libComedi Elevator control.
// These functions provides an interface to the elevators in the real time lab
//
// 2007, Martin Korsgaard

//Edited by Sigvart M. Hovland and Olav Kallerud



#include "channels.h"
#include "elev.h"
#include "io.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>


void elev_set_speed(int speed)
{
    // In order to sharply stop the elevator, the direction bit is toggled
    // before setting speed to zero.
    static int last_speed = 0;
    
    // If to start (speed > 0)
    if (speed > 0)
        io_clear_bit(MOTORDIR);
    else if (speed < 0)
        io_set_bit(MOTORDIR);

    // If to stop (speed == 0)
    else if (last_speed < 0)
        io_clear_bit(MOTORDIR);
    else if (last_speed > 0)
        io_set_bit(MOTORDIR);

    last_speed = speed ;

    // Write new setting to motor.
    io_write_analog(MOTOR, 2048 + 4*abs(speed));
}

int elev_get_obstruction_signal(void)
{
    return io_read_bit(OBSTRUCTION);
}

int elev_get_stop_signal(void)
{
    return io_read_bit(STOP);
}

int elev_get_floor_sensor_signal(void)
{
    if (io_read_bit(SENSOR1))
        return 0;
    else if (io_read_bit(SENSOR2))
        return 1;
    else if (io_read_bit(SENSOR3))
        return 2;
    else if (io_read_bit(SENSOR4))
        return 3;
    else
        return -1;
}

int elev_get_motor_dir(void){
	return io_read_bit(MOTORDIR);
}

int elev_init(void)
{

    // Init hardware
    if (!io_init())
        return 0;
    // Return success.
    return 1;
}

// debug function
void print_queues(int queues[N_QUEUES][N_FLOORS]){
	int queue;
	int floor;
	printf("\n0:Up queue, 1:Down queue and 2:Command Queue\n");
	for(queue=0; queue < N_QUEUES; queue++){
		printf("%d : [ ",queue);

		for(floor = 0; floor < N_FLOORS; floor++){
			printf("%d ",queues[queue][floor]);
		}
		printf("]\n");
	}
}



