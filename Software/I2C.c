#include "I2C.h"
#include <stdio.h>
#include <rt_misc.h>

#define NO_ERR 0
#define NACK_ERROR 1
#define ARB_LOST 2
#define UNKNOWN_STATUS 3

#define _READ_ 1
#define _WRITE_ 0

static uint32_t status = 0;

/* TODO: Implement functinality for subaddresses */

void I2C_init(void){
	/* Make sure I2C is in reset while programming these? */
	LPC_SYSCON->PRESETCTRL &= ~(1UL << 1);
	
	/*Initialize IO pins */
	LPC_IOCON->PIO0_4 &= ~(0x303);
	LPC_IOCON->PIO0_4 |= (0x1);
	LPC_IOCON->PIO0_5 &= ~(0x303);
	LPC_IOCON->PIO0_5 |= (0x1);
	
	/* Enable I2C clock and disable I2C resets*/
	LPC_SYSCON->SYSAHBCLKCTRL |= (1UL << 5);
	LPC_SYSCON->PRESETCTRL |= (1UL << 1);
	
	/* Initializing I2CON register */
	LPC_I2C->CONCLR = 0x6C; // Clear all bits
	LPC_I2C->CONSET = (1UL << 6); // Set to master transmitter mode (set I2C_enable)
	
	/* Set clock dividers */
	LPC_I2C->SCLH = 0x000F; // Fast mode assuming 12 MHz sysclock (default rate). 
	LPC_I2C->SCLL = 0x000F;
	
	/* Reset local variable status */
	status = 0;
}

static uint8_t write_addr(uint8_t addr, uint8_t r_w) {
	
	LPC_I2C->CONSET |= (1UL << 5); // Set start bit
	
	while(LPC_I2C->STAT == 0xF8){
		// printf("Stuck 1\n");
	}; // Poll to see if SI bit has been set yet
	status = LPC_I2C->STAT;
	switch (status) {
		case (0x08): /* Successful start bit sent*/
			LPC_I2C->DAT = ((addr << 1) | r_w); /* Write address and read/write bit */
			LPC_I2C->CONCLR = 0x28; /* Clear I2C interrupt and start bit */
			break;
		case (0x10): /* Repeated start condition sent */
			//while(1);
			break;
		case (0x38): /* Arbitration lost */
			// printf("Arbitration lost");
			return ARB_LOST;
			//while(1);
			break;
		default:
			// printf("Unknown Status");
			return UNKNOWN_STATUS;
			//while(1);
			break;
	}

	while(LPC_I2C->STAT == 0xF8){
		// printf("Stuck 2\n");
	}; // Poll to see if SI bit has been set yet
	status = LPC_I2C->STAT;
	
	if(r_w == _WRITE_){
		switch (status) {
			case(0x18):  /* Address + write sent correctly */
				return NO_ERR;
				break;
			case (0x20): /* NACK received instead of ACK */
				// printf("No slave response to address");
				//while(1);
				return NACK_ERROR;
				break;
			case (0x38): /* Arbitration lost */
			// 	printf("Arbitration lost");
				return ARB_LOST;
				//while(1);
				break;
			default:
				// printf("Unknown Status");
				return UNKNOWN_STATUS;
				//while(1);
				break;
		}	
	}
	
	else{
		switch (status) {
			case (0x40):  /* Address + read sent correctly */
				break;
			case (0x48): /* NACK received instead of ACK */
				//while(1);
				break;
			case (0x38): /* Arbitration lost */
				//while(1);				
				break;
			default:
				//while(1);
				break;
		}	
	}
	
	return NO_ERR;
}



static uint8_t write_byte(uint8_t data){
	
	// At this point, we know the last operation occured correctly, so we do not check I2C_status yet
	
	LPC_I2C->DAT = data; // Load data into data register
	LPC_I2C->CONCLR = 0x08; // Clear I2C interrupt
	
	while(LPC_I2C->STAT == 0xF8)
	{
		// printf("Stuck 3\n");
	}; // Poll to see if SI bit has been set yet
	status = LPC_I2C->STAT;
	switch (status) {
		case (0x28): /* Data sent correctly, ACK received */
			return NO_ERR;
			break;
		case (0x30): /* Data sent, NACK received */
			// printf("NACK Error");
			return NACK_ERROR;
			//while(1);
			break;
		case (0x38): /* Arbitration lost */
		//	printf("Arb lost write\n");
			return ARB_LOST;
			//while(1);
			break;
		default: /* Some other status was received */
		//	printf("Unknown status");
			return UNKNOWN_STATUS;
			//while(1);
			break;
	}
	
}
/* We pass in pointer to data directly so we can return error codes later */
static void read_byte(uint8_t* data, uint8_t remaining_bytes){
	// At this point, we know the last operation occured correctly, so we do not check I2C_status yet
	*data = LPC_I2C->DAT;
	LPC_I2C->CONCLR = 0x08; // Clear I2C interrupt
	
	while(LPC_I2C->STAT == 0xF8); // Poll to see if SI bit has been set yet
	status = LPC_I2C->STAT;
	switch (status){
		case (0x50): /* Data has been received and ACK was returned */
			if(remaining_bytes == 2){
				LPC_I2C->CONCLR = 0x04; /* Will respond to next data byte with NACK */
			}
			break;
		case (0x58): /* Data has been received and NACK was returned */
			break;
		case (0x38): /* Arbitration lost */
		//	while(1);
			break;
		default:
		//	while(1);
			break;
	}
}


uint8_t I2C_write(uint8_t addr, uint8_t *data, uint32_t length){
	int i;
	uint8_t rval = 0;
	rval = write_addr(addr, _WRITE_);
	if(rval != NO_ERR){
		LPC_I2C->CONSET = 0x10; /* Sets stop flag. When we detect the stop condition, this is cleared automatically */
		LPC_I2C->CONCLR = 0x08;  /* Clear SI flag */
		return rval;
	}
	
	for(i = 0; i < length; i++){
		rval = write_byte(data[i]);
		if (rval != NO_ERR){
			LPC_I2C->CONSET = 0x10; /* Sets stop flag. When we detect the stop condition, this is cleared automatically */
			LPC_I2C->CONCLR = 0x08;  /* Clear SI flag */
			return rval;
		}
	}
	
	/* Set stop condition */
	LPC_I2C->CONSET = 0x10; /* Sets stop flag. When we detect the stop condition, this is cleared automatically */
	LPC_I2C->CONCLR = 0x08;  /* Clear SI flag */
	
	// printf("Successful Write\n");
	return rval;
}

uint8_t I2C_read(uint8_t addr, uint8_t *data, uint32_t length){
	int i;
	/* Set AA here if you want to read multiple bytes */
	if(length > 1){
		LPC_I2C->CONSET = 0x04;
	}
	else{
		LPC_I2C->CONCLR = 0x04;
	}
	
	write_addr(addr, _READ_);
	
	for(i = 0; i < length; i++){
		read_byte(data + i, (length - i)); /* Passing in pointer directly. */
	}
	
		/* Set stop condition */
	LPC_I2C->CONSET = 0x10; /* Sets stop flag. When we detect the stop condition, this is cleared automatically */
	LPC_I2C->CONCLR = 0x08;  /* Clear SI flag */
	
	return 0;
}









