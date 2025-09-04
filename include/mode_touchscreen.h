#ifndef _MODE_TOUCHSCREEN_H
#define _MODE_TOUCHSCREEN_H
#include "nextion.h"
#include "4131.h"

/**
* @brief updates the main text box on the touchscreen page
* @return void
*/
void update_txt_box(char *val);
/**
* @brief clears all 3 buttons on touchscreen page after destination tower is selected
* @return void
*/
void clear_boxes(void);
void select_box(int tower_idx);
// Page 2 button functions


#endif

