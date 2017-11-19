#include "IR.h"
#include "LPC11xx.h"
#include "core_cm0.h"
#include <stdio.h>

/* Could use a global queue? */
static uint8_t buffer[3] = {0x00, 0x00, 0x00};
static int bitIdx = 0;
static int byteIdx = 0;

void initIR(void){
    /* Set data pin to be output */
    LPC_GPIO1->DIR |= (IR_PIN);
    /* Enable clock for our 32 bit timer */
    LPC_SYSCON->SYSAHBCLKCTRL |= 1UL << 9;
    
    /* Activate timer, but set it to halt */
    LPC_TMR32B0->TCR = 0x03;
    
    /* Generate interrupt and reset timer when we hit count limit */
    LPC_TMR32B0->MCR = (1UL << 1) | (1UL << 0);
    /* Count up to 300,000, this should be ~1ms */
    LPC_TMR32B0->MR0 = 300000;
    /*  Enable timer interrupt */
    NVIC_SetPriority(TIMER_32_0_IRQn, 0);
    //NVIC_ClearPendingIRQ(TIMER_32_0_IRQn);
    NVIC_EnableIRQ(TIMER_32_0_IRQn);

    IR_off();
}

/* Command should be uint8_t array of length 3 */
void sendCommand(const uint8_t command[]){
    int i = 0;
    /* Copy command into tranmission buffer */
    for(; i < 3; i++){
        buffer[i] = command[i];
    }
    
    /* Restart timer */
    LPC_TMR32B0->TCR = 1UL;
    
    /* Send first bit */
    sendIRBit();
}

void sendIRBit(void){
    if((buffer[byteIdx] & 0x80) != 0){
        //printf("1");
        IR_on();
    }
    else{
        //printf("0");
        IR_off();
    }
    
    buffer[byteIdx] = buffer[byteIdx] << 1;
    
    /* Update index counts */
    if(bitIdx < 7){
        bitIdx++;
    }
    else{
        bitIdx = 0;
        byteIdx++;
    }
    /* Stop timer and reset indexes if at end of command */
    if(byteIdx > 2){
        LPC_TMR32B0->TCR = 0x03;
        byteIdx = 0;
        bitIdx = 0;
        IR_off();
        //printf("\r\n");
    }
}



static void IR_on(void){
    LPC_GPIO1->DATA &= ~(IR_PIN);
}

static void IR_off(void){
    LPC_GPIO1->DATA |= IR_PIN;
}

