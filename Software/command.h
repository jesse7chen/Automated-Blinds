#ifndef _COMMAND_H_
#define _COMMAND_H_

#include <stdint.h>

#define OPEN_BLIND_CMD 0
#define CLOSE_BLIND_CMD 1
#define SET_OPEN_TIME_CMD 2
#define SET_ABSOLUTE_TIME_CMD 3
#define SET_CLOSE_TIME_CMD 4

void makeCommand(uint8_t commandType, const uint8_t time[]);


#endif /* _COMMAND_H_ */
