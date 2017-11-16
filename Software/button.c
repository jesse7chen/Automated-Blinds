#include "button.h"
#include "LPC11xx.h"
#include "core_cm0.h"
#include "stdint.h"
#include "stdio.h"

#define NUM_BUTTONS 6
#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3
#define ENTER 4
#define BACK 5

static int lastButtonPressed = -1;



void init_buttons(void){
    /* In IOCON, all pins should be GPIO and pull-up enabled by default. They should also be configured as inputs by default */
    /*LPC_GPIO0->DIR &= ~(BUTTON_LEFT);
    LPC_GPIO0->DIR &= ~(BUTTON_RIGHT);
    LPC_GPIO0->DIR &= ~(BUTTON_UP);
    LPC_GPIO0->DIR &= ~(BUTTON_DOWN);
    LPC_GPIO0->DIR &= ~(BUTTON_ENTER);
    LPC_GPIO0->DIR &= ~(BUTTON_BACK);*/
    
    //LPC_IOCON->PIO0_8 |= (1UL << 2);
    /* Enable clock for our timer */
    LPC_SYSCON->SYSAHBCLKCTRL |= (1UL << 7 | 1UL << 8);
    
    /* Enable our timer */
    LPC_TMR16B0->TCR = 1UL;
    
    printf("RUNNING INIT BUTTONS\r\n");
    printf("TCR value: %x\r\n", LPC_TMR16B0->TCR);

    /* Generate interrupt and reset timer when we hit count limit */
    LPC_TMR16B0->MCR = (1UL << 1) | (1UL << 0);
    /* Count up to 60,000, this should be ~5ms */
    LPC_TMR16B0->MR0 = 0xEA60;

    /*  Enable timer interrupt */
    NVIC_SetPriority(TIMER_16_0_IRQn, 0);
    //NVIC_ClearPendingIRQ(TIMER_16_0_IRQn);
    NVIC_EnableIRQ(TIMER_16_0_IRQn);

}

/* 1 = up, 2 = right, 3 = down, 4 = left */
/* Return button index if a button has been pressed, else return -1 */
int readButtons(void){
    static uint16_t states[6] = {0};
    int button = 0;

    /* I invert it because I think the button has a pull-up resistor */
    for(button = 0; button < NUM_BUTTONS; button++){
        states[button] = (states[button] << 1) | !rawButtonPresses(button) | 0xE000;
        if(states[button] == 0xF000){
            if(lastButtonPressed == -1){
                lastButtonPressed = button;
            }
            printf("Button %d pressed\r\n", button);
            return button;
        }
    }

    return -1;
}

/* Returns 1 if pressed, 0 otherise */
static int rawButtonPresses(int button){
    switch (button){
        case(LEFT):
            return leftButtonPressed();
        case(RIGHT):
            return rightButtonPressed();
        case(UP):
            return upButtonPressed();
        case(DOWN):
            return downButtonPressed();
        case(ENTER):
            return enterButtonPressed();
        case(BACK):
            return backButtonPressed();
        default:
            return 0;
    }
}
static int leftButtonPressed(void){
    return LPC_GPIO0->DATA & (BUTTON_LEFT);
}

static int rightButtonPressed(void){
    return LPC_GPIO0->DATA & (BUTTON_RIGHT);
}

static int upButtonPressed(void){
    return LPC_GPIO0->DATA & (BUTTON_UP);
}

static int downButtonPressed(void){
    return LPC_GPIO0->DATA & (BUTTON_DOWN);
}

static int enterButtonPressed(void){
    return LPC_GPIO0->DATA & (BUTTON_ENTER);
}

static int backButtonPressed(void){
    return LPC_GPIO0->DATA & (BUTTON_BACK);
}

int getLastPressed(void){
    return lastButtonPressed;
}

void resetLastPressed(void){
    lastButtonPressed = -1;
}
