/* Board Support Package */
#include "LPC11xx.h"
#include "button.h"

void TIMER16_0_IRQHandler(void){
    /* You should check which interrupt it was here */
    NVIC_ClearPendingIRQ(TIMER_16_0_IRQn);
    
    if(LPC_TMR16B0->IR == 0x01){
        /* Call read button handler */
    }
}
