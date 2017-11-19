/* Board Support Package */
#include "bsp.h"
#include "IR.h"
#include "LPC11xx.h"
#include "button.h"
#include <stdio.h>


void TIMER16_0_IRQHandler(void){
    /* Check which match register triggered the interrupt here */

    if((LPC_TMR16B0->IR & 0x01) == 0x01){
        /* Call read button handler if the interrupt was timer used for the button by reading if bit 0 is 1 */
        /* Clear interrupt register */
        LPC_TMR16B0->IR = (0x01);
        readButtons();
    }
    NVIC_ClearPendingIRQ(TIMER_16_0_IRQn);
}

void TIMER32_0_IRQHandler(void){
    /* Check which match register triggered the interrupt here */

    if((LPC_TMR32B0->IR & 0x01) == 0x01){
        /* Call read button handler if the interrupt was timer used for the button by reading if bit 0 is 1 */
        /* Clear interrupt register */
        LPC_TMR32B0->IR = (0x01);
        sendIRBit();
    }
    NVIC_ClearPendingIRQ(TIMER_32_0_IRQn);
}
