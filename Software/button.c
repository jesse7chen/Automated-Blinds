#include "button.h"
#include "LPC11xx.h"
#include "core_cm0.h"


void init_buttons(void){
    /* In IOCON, all pins should be GPIO and pull-up enabled by default. They should also be configured as inputs by default */
    LPC_GPIO0->DIR &= ~(BUTTON_LEFT);
    LPC_GPIO0->DIR &= ~(BUTTON_RIGHT);
    LPC_GPIO0->DIR &= ~(BUTTON_UP);
    LPC_GPIO0->DIR &= ~(BUTTON_DOWN);
    LPC_GPIO0->DIR &= ~(BUTTON_ENTER);
    LPC_GPIO0->DIR &= ~(BUTTON_BACK);
    
    /* Initialize our timer */
    LPC_TMR16B0->TCR = 0x01; 
    /* Generate interrupt and reset timer when we hit count limit */
    LPC_TMR16B0->MCR = (1U << 1) | (1U << 0); 
    /* Count up to 60,000, this should be ~5ms */
    LPC_TMR16B0->MR0 = 0xEA60; 
    /*  Enable timer interrupt */
    NVIC_EnableIRQ(TIMER_16_0_IRQn);
    
}

void read_button(void){
    
}


