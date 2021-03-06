#ifndef UI_H
#define UI_H

/**
  Button types for function ui_set_button_lamp() and ui_get_button().
*/

typedef enum tag_ui_lamp_type { 
    BUTTON_CALL_UP	 = 0, 
    BUTTON_CALL_DOWN = 1, 
    BUTTON_COMMAND	 = 2 
} ui_button_type_t;

#define N_QUEUES 3
#define N_FLOORS 4


/**
  Gets a button signal.
  @param button Which button type to check. Can be BUTTON_CALL_UP,
    BUTTON_CALL_DOWN or BUTTON_COMMAND (button "inside the elevator).
  @param floor Which floor to check button. Must be 0-3.
  @return 0 if button is not pushed. 1 if button is pushed.
*/
int ui_get_button_signal(int button, int floor);

/** 
Checks what buttons are being pushed and add ordered floor to
its respective queue.
*/
void ui_check_buttons(int queues[N_QUEUES][N_FLOORS]);

//Check for button press
void ui_button_signals(int queues[N_QUEUES][N_FLOORS]);

//set lights
void ui_set_order_buttons_lamp(int queues[N_QUEUES][N_FLOORS]);

/**
  Get signal from stop button.
  @return 1 if stop button is pushed, 0 if not.
*/
int ui_get_stop_signal(void);

/**
  Turn door-open lamp on or off.
  @param value Non-zero value turns lamp on, 0 turns lamp off.
*/
void ui_set_door_open_lamp(int value);

/**
  Turn stop lamp on or off.
  @param value Non-zero value turns lamp on, 0 turns lamp off.
*/
void ui_set_stop_lamp(int value);

/**
  Set floor indicator lamp for a given floor.
  @param floor Which floor lamp to turn on. Other floor lamps are turned off.
*/
void ui_set_floor_indicator(int floor);

/**
  @returns current floor indicator lamp value
  from 0..3 
*/
int ui_get_floor_indicator(void);

/**
  Set a button lamp.
  @param lamp Which type of lamp to set. Can be BUTTON_CALL_UP,
    BUTTON_CALL_DOWN or BUTTON_COMMAND (button "inside" the elevator).
  @param floor Floor of lamp to set. Must be 0-3
  @param value Non-zero value turns lamp on, 0 turns lamp off.
*/
void ui_set_button_lamp(int button, int floor, int value);

/**
  Initialize user interface.
  @return 1 on success, 0 on failure.
*/
int ui_init(void);


#endif
