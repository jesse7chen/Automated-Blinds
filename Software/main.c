#include "LPC11xx.h"
#include <stdio.h>
#include <rt_misc.h>
#include "I2C.h"
#include "SSD1306.h"
#include "UI.h"
#include "button.h"
#include "bsp.h"
#include "IR.h"

/* Import external functions from Serial.c file                               */
extern void SER_init (void);

int global_count = 0;

void configureGPIO()
{
	//Enable CLKOUT
	/*
	LPC_IOCON->PIO0_1 &= ~0x3F; // Select clkout function for P0.1
	LPC_IOCON->PIO0_1 |= 0x01;
	LPC_SYSCON->CLKOUTCLKSEL = 0x00; // IRC: 0x00 System osci: 0x01 WTD: 0x02 Main clk: 0x03
	LPC_SYSCON->CLKOUTUEN = 0x01; // Update clock
	LPC_SYSCON->CLKOUTUEN = 0x00; // Toggle update register once
	LPC_SYSCON->CLKOUTUEN = 0x01;
	while ( !(LPC_SYSCON->CLKOUTUEN & 0x01) ); // Wait until updated
	LPC_SYSCON->CLKOUTDIV = 1; // Divided by 255
	*/

	//set port 0_7 to output (high current drain in LPC1114)
    LPC_GPIO0->DIR |= (1<<7);
}

void ledOn(void)
{
	LPC_GPIO0->DATA &= ~(1<<7);
}

void ledOff(void)
{						 
	LPC_GPIO0->DATA |= (1<<7);
}

int main()
{
	int i = 0;
	//int j =0;
	int shift = 0;
	int increment = 0;
    int lastButton = -1;
	SER_init();
	configureGPIO();
	I2C_init();
    
	printf("Return value: %d\r\n", screen_init());
	printf("Return value2: %d\r\n", screen_display());
    
    init_buttons();
    initIR();
    
    __enable_irq();
    
    for (i = 0; i < 0x008FFFFF; i++)
    {
    }


	while (1)
	{
        
        clear_display();
        lastButton = getLastPressed();
        if(lastButton != -1){
            modifyScreen(lastButton);
            resetLastPressed();
        }
        select_screen_to_display();
        screen_display();

	}
}
