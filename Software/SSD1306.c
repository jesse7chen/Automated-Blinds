#include "LPC11xx.h"
#include "I2C.h"
#include "SSD1306.h"


#define SCREEN_ADDR (0x3c)
#define SCREEN_HEIGHT (32)
#define SCREEN_WIDTH (128)

static uint8_t buffer[((SCREEN_HEIGHT * SCREEN_WIDTH)/8) + 1] = { 0x40,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7,
0xC7, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7,
0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xE0, 0xE0, 0xF8, 0xF8, 0xF0, 0xF0, 0xF1, 0xF1, 0xF1, 0xE1, 0xE1,
0xE3, 0xE3, 0xC7, 0xC7, 0xC7, 0x87, 0x87, 0x8F, 0x8F, 0x0F, 0x0F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F,
0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x7F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3,
0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3,
0x03, 0x03, 0x03, 0x03, 0x03, 0x07, 0x07, 0x1F, 0x1F, 0x0F, 0x0F, 0x8F, 0x8F, 0x8F, 0x87, 0x87,
0xC7, 0xC7, 0xC3, 0xC3, 0xC3, 0xE3, 0xE3, 0xF1, 0xF1, 0xF0, 0xF0, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8,
0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

/*uint32_t us_to_cycles(uint32_t cycles){

}*/

static void pin0_7_off(void)
{
	LPC_GPIO0->DATA &= ~(1<<7);
}

static void pin0_7_on(void)
{						 
	LPC_GPIO0->DATA |= (1<<7);
}

static uint8_t send_command(uint8_t command){
	uint8_t control_byte = 0x00;
	uint8_t data[2];
	data[0] = control_byte;
	data[1] = command;
	
	return I2C_write(SCREEN_ADDR, data, 2);
}

void power_on_seq(void){
	int i = 0;
	pin0_7_on();
	for (i = 0; i < 0x0007FFFF; i++)
	{
	}
	pin0_7_off();
	
	for (i = 0; i < 0x00000FFF; i++)
	{
	}
	pin0_7_on();
}

uint8_t screen_init(void){
	uint8_t rval = 0;
	power_on_seq();
	
	rval |= send_command(SCREEN_DISPLAY_OFF);
	
	rval |= send_command(SCREEN_SETDISPLAYCLOCKDIV);
	rval |= send_command(0x80);
	
	rval |= send_command(SCREEN_SETMULTIPLEX);
	rval |= send_command(SCREEN_HEIGHT - 1);
	
	rval |= send_command(SCREEN_SETDISPLAYOFFSET);
	rval |= send_command(0x00);
	
	rval |= send_command(SCREEN_SETSTARTLINE | 0x00);
	
	rval |= send_command(SCREEN_SETCHARGEPUMP);
	rval |= send_command(0x14);
	
	rval |= send_command(SCREEN_SETMEMORYMODE);
	rval |= send_command(0x00);
	
	rval |= send_command(SCREEN_SEGREMAP | 0x1);
	
	rval |= send_command(SCREEN_COMSCANDIR);
	
	rval |= send_command(SCREEN_SETCOMPINS);
	rval |= send_command(0x02);
	
	rval |= send_command(SCREEN_SETCONTRAST);
	rval |= send_command(0x8F);
	
	rval |= send_command(SCREEN_SETPRECHARGE);
	rval |= send_command(0xF1);
	
	rval |= send_command(SCREEN_DISPLAYALL_RESUME);
	rval |= send_command(SCREEN_SETNORMAL);

	rval |= send_command(SCREEN_STOP_SCROLL);
	
	rval |= send_command(SCREEN_DISPLAY_ON);
	
	return rval;
}

uint8_t screen_display(void){
	uint8_t rval = 0;
	uint8_t control_byte = 0x40;
	uint8_t data[17];
	int i = 0;
	int j = 1;
	
	rval |= send_command(SCREEN_SETCOLUMNADDR);
	rval |= send_command(0x00);
	rval |= send_command(SCREEN_WIDTH - 1);
	
	rval |= send_command(SCREEN_SETPAGEADDR);
	rval |= send_command(0x00);
	rval |= send_command((SCREEN_HEIGHT/8) - 1);
	
	/* Begin sending a ton of data */
	/*for(i = 0; i < (SCREEN_HEIGHT * SCREEN_WIDTH)/8; i+= 16){
		data[0] = control_byte;
		for(j = 1; j < 17; j++){
			data[j] = buffer[i + j - 1];
		}
		rval |= I2C_write(SCREEN_ADDR, data, 17);
	}*/
	
	rval |= I2C_write(SCREEN_ADDR, buffer, ((SCREEN_HEIGHT * SCREEN_WIDTH)/8) + 1);
	
	return rval;
}
