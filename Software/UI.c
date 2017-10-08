#include "UI.h"
#include "SSD1306.h"

#define TIME_SIZE 3

#define TIME_COLUMN 26
#define TIME_ROW 5

/* Make time characters look */

typedef enum Screen {
	startScreen,
	setAbsoluteTime,
	setOpenTime,
	setCloseTime
} Screen;

typedef enum StartScreenOption {
	changeBlindTime_selected,
	setAbsoluteTime_selected
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

/* I used a int array to store the time, since it's easier to manipulate */
static int displayed_time[4] = {0,0,0,0};

void select_screen_to_display(void){
	switch(curr_screen){
		case (startScreen):
				display_start_screen();
			break;
		case (setAbsoluteTime):
			break;
		case (setOpenTime):
			break;
		case (setCloseTime):
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
			break;
		case (setAbsoluteTime_selected):
			draw_string(get_curr_col(), get_curr_row(), "Change times ", 1, NOT_SELECTED);
			draw_string(get_curr_col(), get_curr_row(), "Set current time", 1, SELECTED);
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

/* 1 = up, 2 = right, 3 = down, 4 = left */
void change_time_on_display(uint8_t button_press){
	
	switch(button_press){
		case(1):
			increment_time();
			break;
		case(2):
			curr_time_char = (TimeChar)(((int)curr_time_char+1) % 4);
			break;
		case(3):
			decrement_time();
			break;
		case(4):
			if((int)curr_time_char - 1 >= 0){
				curr_time_char--;
			}
			else{
				curr_time_char = minute_right;
			}
			break;
		default:
			break;
	}
	
}

void increment_time(void){
	displayed_time[curr_time_char] = (displayed_time[(int)curr_time_char] + 1) % time_char_mod_values[(int)curr_time_char];
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
