#ifndef _IR_H_
#define _IR_H_
#include <stdint.h>

/* IR is connected to pin PIO1_9 */
#define IR_PIN (1 << 9)

void initIR(void);
void sendCommand(const uint8_t command[]);
void sendIRBit(void);
static void IR_on (void);
static void IR_off (void);


#endif /* _IR_H_ */
