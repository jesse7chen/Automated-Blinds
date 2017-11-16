#include "UI.h"
#include "SSD1306.h"

#define TIME_SIZE 3

#define TIME_COLUMN 26
#define TIME_ROW 5

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3
#define ENTER 4
#define BACK 5

#define NUM_START_SCREEN_OPTIONS 4

/* Make time characters look a little nicer */
/* TODO: Prettify code and fix bug where user can set hour to above 24 by setting -x to 9 while x- is less than 2 */

typedef enum Screen {
	startScreen,
	setAbsoluteTime,
	setOpenTime,
	setCloseTime
} Screen;

typedef enum StartScreenOption {
	changeBlindTime_selected,
	setAbsoluteTime_selected,
    openBlinds_selected,
    closeBlinds_selected
} StartScreenOption;

typedef enum TimeChar {
	hour_left = 0,
	hour_right = 1,
	minute_left = 2,
	minute_right = 3
} TimeChar;
/* Hour left stands for the '1' in "12:34", hour right stands for the '2', etc. */

/* These values correspond to the proper modulo of a digit to ensure it does not enter a improper state. For example, we can't have a time like "32:99"
Each value is listed in order from the modulo for the left-most digit to the right-most digit so that the proper digit can be accessed with (int)curr_time_char*/
static int time_char_mod_values[4] = {
	3, /*left hour digit mod value */
	10, /* right hour digit mod value */ 
	6, /* left minute digit mod value */
	10 /* right minute digit mod value */
};

static TimeChar curr_time_char = hour_left;
static Screen curr_screen = startScreen;
static StartScreenOption start_screen_option = changeBlindTime_selected;

/* I used a int array to store the time, since it's easier to manipulate. I guess I could have also made this a struct or something */
static int displayed_time[4] = {0,0,0,0};

void select_screen_to_display(void){
	switch(curr_screen){
		case (startScreen):
            display_start_screen();
			break;
		case (setAbsoluteTime):
            display_time_screen();
			break;
		case (setOpenTime):
			break;
		case (setCloseTime):
			break;
		default:
			break;
	}
}

void modifyScreen(int buttonPress){
    switch(curr_screen){
		case (startScreen):
            modifyStartScreen(buttonPress);
			break;
		case (setAbsoluteTime):
            change_time_on_display(buttonPress);
			break;
		case (setOpenTime):
			break;
		case (setCloseTime):
			break;
		default:
			break;
    }
}

/* 0 = up, 1 = right, 2 = down, 3 = left */
static void modifyStartScreen(buttonPress){
    switch(buttonPress){
        case(UP):
            start_screen_option = (start_screen_option - 1 + NUM_START_SCREEN_OPTIONS) % NUM_START_SCREEN_OPTIONS;
			break;
		case(RIGHT):
            start_screen_option = (start_screen_option + 1) % NUM_START_SCREEN_OPTIONS;
			break;
		case(DOWN):
            start_screen_option = (start_screen_option + 1) % NUM_START_SCREEN_OPTIONS;
			break;
		case(LEFT):
            start_screen_option = (start_screen_option - 1 + NUM_START_SCREEN_OPTIONS) % NUM_START_SCREEN_OPTIONS;
			break;
        case(ENTER):
            curr_screen = setAbsoluteTime;
            break;
        case(BACK):
            break;
		default:
			break;
    }
}

void display_start_screen(void){
	draw_string(0, 0, "Select option:", 1, NOT_SELECTED);
	switch(start_screen_option){
		case (changeBlindTime_selected):
			draw_string(get_curr_col(), get_curr_row(), "Change times", 1, SELECTED);
			draw_string(get_curr_col(), get_curr_row(), " Set current time", 1, NOT_SELECTED);
            draw_string(get_curr_col(), get_curr_row(), " Open blinds", 1, NOT_SELECTED);
            draw_string(get_curr_col(), get_curr_row(), " Close blinds", 1, NOT_SELECTED);
			break;
		case (setAbsoluteTime_selected):
			draw_string(get_curr_col(), get_curr_row(), "Change times ", 1, NOT_SELECTED);
			draw_string(get_curr_col(), get_curr_row(), "Set current time", 1, SELECTED);
            draw_string(get_curr_col(), get_curr_row(), " Open blinds", 1, NOT_SELECTED);
            draw_string(get_curr_col(), get_curr_row(), " Close blinds", 1, NOT_SELECTED);
			break;
        case (openBlinds_selected):
			draw_string(get_curr_col(), get_curr_row(), "Change times ", 1, NOT_SELECTED);
			draw_string(get_curr_col(), get_curr_row(), "Set current time ", 1, NOT_SELECTED);
            draw_string(get_curr_col(), get_curr_row(), "Open blinds", 1, SELECTED);
            draw_string(get_curr_col(), get_curr_row(), " Close blinds", 1, NOT_SELECTED);
			break;
        case (closeBlinds_selected):
			draw_string(get_curr_col(), get_curr_row(), "Change times ", 1, NOT_SELECTED);
			draw_string(get_curr_col(), get_curr_row(), "Set current time ", 1, NOT_SELECTED);
            draw_string(get_curr_col(), get_curr_row(), "Open blinds ", 1, NOT_SELECTED);
            draw_string(get_curr_col(), get_curr_row(), "Close blinds", 1, SELECTED);
			break;
		default:
			break;
	}
}

void display_time_screen(void){
	/* The == method of determining what character is highlighted is more code efficient, but less cycle efficient. It's also a little bit sketchy... */
	draw_char(TIME_COLUMN, TIME_ROW, u_int_to_char(displayed_time[(int)hour_left]), TIME_SIZE, curr_time_char == hour_left);
	draw_vertical_line_slow(get_curr_col(), get_curr_row(), 7*TIME_SIZE, 0);
	
	draw_char(get_curr_col(), get_curr_row(), u_int_to_char(displayed_time[(int)hour_right]), TIME_SIZE, curr_time_char == hour_right);
	draw_vertical_line_slow(get_curr_col(), get_curr_row(), 7*TIME_SIZE, 0);
	
	draw_char(get_curr_col(), get_curr_row(), ':', TIME_SIZE, NOT_SELECTED);
	draw_vertical_line_slow(get_curr_col(), get_curr_row(), 7*TIME_SIZE, 0);
	
	draw_char(get_curr_col(), get_curr_row(), u_int_to_char(displayed_time[(int)minute_left]), TIME_SIZE, curr_time_char == minute_left);
	draw_vertical_line_slow(get_curr_col(), get_curr_row(), 7*TIME_SIZE, 0);
	
	draw_char(get_curr_col(), get_curr_row(), u_int_to_char(displayed_time[(int)minute_right]), TIME_SIZE, curr_time_char == minute_right);
}

/* 0 = up, 1 = right, 2 = down, 3 = left */
void change_time_on_display(int buttonPress){
	
	switch(buttonPress){
		case(UP):
			increment_time();
			break;
		case(RIGHT):
			curr_time_char = (TimeChar)(((int)curr_time_char+1) % 4);
			break;
		case(DOWN):
			decrement_time();
			break;
		case(LEFT):
			if((int)curr_time_char - 1 >= 0){
				curr_time_char--;
			}
			else{
				curr_time_char = minute_right;
			}
			break;
        case(ENTER):
            break;
        case(BACK):
            curr_screen = startScreen;
		default:
			break;
	}
	
}

void increment_time(void){
    if(!(displayed_time[0] == 2 && curr_time_char == hour_right)){
        displayed_time[curr_time_char] = (displayed_time[(int)curr_time_char] + 1) % time_char_mod_values[(int)curr_time_char];
    }
    else{
        displayed_time[curr_time_char] = (displayed_time[(int)curr_time_char] + 1) % 4;
    }
}

char u_int_to_char(int i){
	return (char)(i + 48);
}

void decrement_time(void){
	int curr_time = displayed_time[(int)curr_time_char];
	if(curr_time - 1 < 0){
		displayed_time[(int)curr_time_char] = (int)time_char_mod_values[(int)curr_time_char] - 1;
	}
	else{
		displayed_time[(int)curr_time_char]--;
	}
}
