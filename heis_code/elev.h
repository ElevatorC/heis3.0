// Wrapper for libComedi Elevator control.
// These functions provides an interface to the elevators in the real time lab
//
// 2007, Martin Korsgaard
#ifndef __INCLUDE_DRIVER_H__
#define __INCLUDE_DRIVER_H__

/**
  Sets the speed of the elevator. 
  @param speed New speed of elevator. Positive values denote upward movement
    and vice versa. Set speed to 0 to stop the elevator. From -300 to 300 gives
    sensible speeds. 
*/
void elev_set_speed(int speed);


/**
  Get signal from obstruction switch.
  @return 1 if obstruction is enabled. 0 if not.
*/
int elev_get_obstruction_signal(void);

/**
  Get floor sensor signal.
  @return -1 if elevator is not on a floor. 0-3 if elevator is on floor. 0 is
    ground floor, 3 is top floor.
*/
int elev_get_floor_sensor_signal(void);

/**
  Initialize elevator hardware.
  @return Non-zero on success, 0 on failure.
*/
int elev_init(void);


#endif // #ifndef __INCLUDE_DRIVER_H__

