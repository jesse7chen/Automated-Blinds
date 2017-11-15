#ifndef _BUTTON_H_
#define _BUTTON_H_

#define BUTTON_LEFT (1<<1)
#define BUTTON_RIGHT (1<<2)
#define BUTTON_UP (1<<3)
#define BUTTON_DOWN (1<<8)
#define BUTTON_ENTER (1<<9)
#define BUTTON_BACK (1<<11)

void init_buttons(void);

int readButtons(void);

int rawButtonPresses(int button);

int leftButtonPressed(void);

int rightButtonPressed(void);

int upButtonPressed(void);

int downButtonPressed(void);

int enterButtonPressed(void);

int backButtonPressed(void);

#endif /* _BUTTON_H_ */
