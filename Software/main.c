#include "LPC11xx.h"
#include <stdio.h>
#include <rt_misc.h>
#include "I2C.h"
#include "SSD1306.h"
#include "UI.h"
#include "core_cm0.h"

#define PCLK_HZ 12000000U

/* Import external functions from Serial.c file                               */
extern void SER_init (void);

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
    //LPC_GPIO0->DIR |= (1<<7);
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
	SER_init();
	configureGPIO();
	I2C_init();
	printf("Return value: %d\r\n", screen_init());
	printf("Return value2: %d\r\n", screen_display());



	while (1)
	{
		for (i = 0; i < 0x008FFFFF; i++)
		{
		}
		
		clear_display();	
		display_start_screen();
		screen_display();
		
		for (i = 0; i < 0x008FFFFF; i++)
		{
		}

		for(shift = 0; shift < 4; shift++){

			for(increment = 0; increment < 10; increment++){			
				clear_display();
				display_time_screen();
				screen_display();
				change_time_on_display(1);

				for (i = 0; i < 0x000FFFFF; i++)
				{
				}
			}
			
			clear_display();
			display_time_screen();
			screen_display();
			change_time_on_display(2);

			for (i = 0; i < 0x000FFFFF; i++)
			{
			}
		}
		
		
		/*ledOn();
		printf("Led On, Iteration %d\n\r", j);
		for (i = 0; i < 0x0007FFFF; i++)
		{
		}
		ledOff();
		printf("Led Off, Iteration %d\n\r", j);
		for (i = 0; i < 0x0007FFFF; i++)
		{
		}
		j++;*/
		/*for (i = 0; i < 0x008FFFFF; i++)
		{
		}
		I2C_write(SCREEN_ADDR, &data, 1);*/
	}
}
