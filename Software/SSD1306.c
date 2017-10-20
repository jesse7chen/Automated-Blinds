#include "SSD1306.h"
#include <stdio.h>
#include "font.h"
#include "LPC11xx.h"
#include "I2C.h"

#define SCREEN_ADDR (0x3c)
#define SCREEN_HEIGHT (32)
#define SCREEN_WIDTH (128)
#define BUFFER_SIZE (((SCREEN_HEIGHT * SCREEN_WIDTH)/8) + 1)

// Used to keep track of location when writing a string
static uint16_t curr_row = 0;
static uint16_t curr_col = 0;

/* TODO: Write code to return error values. Should function return immediately if byte is tried to be written out of bounds? */
/* TODO: Add functionality for different font sizes since I need to change times */
/* TODO: Draw better spacing function for char spacing */

// IMPORTANT: MAKE SURE YOU ADDRESS AT DESIRED INDEX + 1 OF BUFFER, DUE TO EXTRA BYTE IN BEGINNING
static uint8_t buffer[((SCREEN_HEIGHT * SCREEN_WIDTH)/8) + 1] = { 0x40,   /* This extra byte serves as the control byte, which tells the screen the rest of the data which follow is graphics data when the whole buffer is sent out.  */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0xFE, 0xF8,
0x00, 0xF0, 0xFE, 0x0E, 0xFE, 0xF0, 0x00, 0xF8, 0xFE, 0x0E, 0x00, 0xC0, 0xE0, 0xB0, 0xB0, 0xB0,
0xE0, 0xC0, 0x00, 0x00, 0xFE, 0xFE, 0x00, 0x00, 0xC0, 0xE0, 0x30, 0x30, 0x70, 0x60, 0x00, 0x00,
0xC0, 0xE0, 0x70, 0x30, 0x70, 0xE0, 0xC0, 0x00, 0x00, 0xF0, 0xF0, 0x20, 0x30, 0xF0, 0xE0, 0x20,
0x30, 0xF0, 0xE0, 0x00, 0x00, 0xC0, 0xE0, 0xB0, 0xB0, 0xB0, 0xE0, 0xC0, 0x00, 0x00, 0xFE, 0xFE,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x0F,
0x0E, 0x0F, 0x00, 0x00, 0x00, 0x0F, 0x0E, 0x0F, 0x01, 0x00, 0x00, 0x03, 0x07, 0x0D, 0x0D, 0x0D,
0x0D, 0x05, 0x00, 0x00, 0x0F, 0x0F, 0x00, 0x00, 0x03, 0x07, 0x0C, 0x0C, 0x0E, 0x06, 0x00, 0x00,
0x03, 0x07, 0x0E, 0x0C, 0x0E, 0x07, 0x03, 0x00, 0x00, 0x0F, 0x0F, 0x00, 0x00, 0x0F, 0x0F, 0x00,
0x00, 0x0F, 0x0F, 0x00, 0x00, 0x03, 0x07, 0x0D, 0x0D, 0x0D, 0x0D, 0x05, 0x00, 0x00, 0x0D, 0x0D,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
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
	/* TODO: Set for loop values to be more exact or just implement a timer interrupt */
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
//	uint8_t control_byte = 0x40;
//	uint8_t data[17];
//	int i = 0;
//	int j = 1;
	
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

/* Data should be a 1 or a 0 */
void draw_pixel(uint8_t x, uint8_t y, uint8_t data){
	int buffer_idx;
	uint8_t value = 0;
	int bit_idx = 0;
	
	buffer_idx = 1 + (x) + ((y/8)*(128));

	/* Do some checks to make sure we don't access out of bounds data */
	if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT){
		printf("Out of index user error in draw_pixel\r\n");
		return;
	}
	if(buffer_idx > BUFFER_SIZE){
		printf("Out of index error calculation in draw_pixel\r\n");
		return;
	}
	/* We only want to check if data is 1 or 0 */
	bit_idx = y%8;
	value = (1 << bit_idx);

	if (data > 0){
		buffer[buffer_idx] |= value;
	}
	else{
		buffer[buffer_idx] &= ~(value);
	}
}

// There is probably a more efficient way to do this, but let's see how this works. This is the most robust method at least
void draw_solid_rectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t data){
	int i = 0;
	int j = 0;
	
	for(i = 0; i < width; i++){
		for(j = 0; j < height; j++){
			draw_pixel(x+i, y+j, data);
		}
	}
}

// This only really works if each character is 8 lines tall. Also, might need to change this to depend on size
void draw_space_between_chars(uint8_t x, uint8_t y, uint8_t data){
	int buffer_idx;
	
	buffer_idx = 1 + (x) + ((y/8)*(128));

	/* Do some checks to make sure we don't access out of bounds data */
	if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT){
		printf("Out of index user error in draw_space_between_chars\r\n");
		return;
	}
	if(buffer_idx > BUFFER_SIZE){
		printf("Out of index error calculation in draw_space_between_chars\r\n");
		return;
	}	
	/* We only want to check if data is 1 or 0 */
	if (data > 0){
		buffer[buffer_idx] |= 0x7F;
	}
	else{
		buffer[buffer_idx] &= ~(0x7F);
	}
}

void draw_vertical_line_slow(uint8_t x, uint8_t y, uint8_t length, uint8_t data){
	int i = 0;
	
	for(i = 0; i < length; i++){
			if (y+length >= SCREEN_HEIGHT || x >= SCREEN_WIDTH){
				printf("Line offscreen");
				return;
			}
			draw_pixel(x, y+i, data);
	}
	curr_col = x+1;
	curr_row = y;
}

void clear_display(void) {
	int i = 0;
	for(i = 1; i < BUFFER_SIZE; i++){
		buffer[i] = 0x00;
	}
}

void draw_char(uint8_t x, uint8_t y, unsigned char c, uint8_t size, uint8_t inverse){
	int i = 0;
	int j = 0;
	int curr_bit = 0;
	/* Do some size checking here */
	if(x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT){
		printf("Char would not appear on screen\r\n");
		return;
	}
	for(i = 0; i < 5; i++){
		curr_bit = font[(c*5) + i];
		for(j = 0; j < 7; j++, curr_bit >>= 1){
			if(curr_bit & 0x01){
				if(inverse){
					if(size == 1){
						draw_pixel(x + i, y + j, 0);
					}
					else{
						draw_solid_rectangle(x+i*size, y+j*size, size, size, 0);
					}
				}
				else{
					if(size == 1){
						draw_pixel(x + i, y + j, 1);
					}
					else{
						draw_solid_rectangle(x+i*size, y+j*size, size, size, 1);
					}
				}
			}
			else{
				if(inverse){
					if(size == 1){
						draw_pixel(x + i, y + j, 1);
					}
					else{
						draw_solid_rectangle(x+i*size, y+j*size, size, size, 1);
					}
				}
				else{
					if(size == 1){
						draw_pixel(x + i, y + j, 0);
					}
					else{
						draw_solid_rectangle(x+i*size, y+j*size, size, size, 0);
					}
				}
			}
		}
	}
	curr_col = x+i*size; // Incrementing column pointer for string writing function
	curr_row = y;
}

void draw_string(uint8_t x, uint8_t y, char* s, uint8_t size, uint8_t inverse){
	unsigned char curr_char;
	int i = 0;
	
	curr_col = x;
	curr_row = y;
	
	curr_char = s[i++];
	while(curr_char != '\0'){
		if((curr_col + 5) >= SCREEN_WIDTH){ //Add 5 to account for font width
			curr_col = 0;
			curr_row = (curr_row + 1) + 7; // Add 7 to account for font height
		}
		draw_char(curr_col, curr_row, curr_char, size, inverse); // Curr_col should be updated automatically
		curr_char = s[i++];
		draw_vertical_line_slow(curr_col++, curr_row, 7*size, inverse); // It's ok for this to be "out of bounds"
	}
}

uint16_t get_curr_col(void){
	return curr_col;
}

uint16_t get_curr_row(void){
	return curr_row;
}
