#ifndef UI_H
#define UI_H
/* definert i elev
typedef enum tag_ui_lamp_type { 
    BUTTON_CALL_UP = 0, 
    BUTTON_CALL_DOWN = 1, 
    BUTTON_COMMAND = 2 
} ui_button_type_t;
*/

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

void ui_button_signals(int queues[N_QUEUES][N_FLOORS],int floor);

void ui_set_lamps(int queues[N_QUEUES][N_FLOORS]);

/** 
Checks what buttons are being pushed and add ordered floor to
its respective queue.
*/
//void ui_check_button_signal();

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
int ui_get_floor_indicator(void);

void ui_set_button_lamp(int button, int floor, int value);

/**
  Initialize user interface.
  @return 1 on success, 0 on failure.
*/
int ui_init(void);


#endif
