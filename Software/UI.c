#include "UI.h"

typedef enum Screen {
	startScreen,
	setAbsoluteTime,
	setOpenTime,
	setCloseTime
} Screen;

static Screen curr_screen = startScreen;

void screen(void){
	switch(curr_screen){
		case (startScreen):
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