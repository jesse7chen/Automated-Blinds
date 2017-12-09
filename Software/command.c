#include "command.h"
#include "IR.h"
#include <stddef.h>

#define COMMAND_LEN_BITS 24
#define START_SEQ 0x80

/* All memory accesses are little-endian! */

/* We set the first nibble to 8 to simulate a start bit of 1 */
/* Command format: 
1 start bit (set to 1)
3 identifier bits (set to 000)
4 command bits
16 data bits (4 bits for each digit of time)
*/
static uint8_t command[3] = {START_SEQ, 0x00, 0x00};

/* Could pass in the four digits as individual arguments as well */
void makeCommand(uint8_t commandType, const uint8_t time[]){
    command[0] = (START_SEQ) | commandType;
    
    if(time != NULL && (commandType == SET_OPEN_TIME_CMD || commandType == SET_ABSOLUTE_TIME_CMD || commandType == SET_CLOSE_TIME_CMD)){
        command[1] = (time[0] << 4) | time[1];
        command[2] = (time[2] << 4) | time[3];
    }

    sendCommand(command);
    command[0] = START_SEQ;
    command[1] = 0x00;
    command[2] = 0x00;
    
}




