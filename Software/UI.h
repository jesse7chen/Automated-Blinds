#ifndef _UI_H_
#define _UI_H_

#include <stdint.h>

void select_screen_to_display(void);

void modifyScreen(int buttonPress);
    
void display_start_screen(void);
void display_time_screen(void);
void change_time_on_display(int buttonPress);
char u_int_to_char(int i);

static void increment_time(void);
static void decrement_time(void);
static void resetTime(void);
static void modifyStartScreen(int buttonPress);
static void displayPresetScreen(void);
static void modifyPresetScreen(int buttonPress);

#endif /* _UI_H_ */
