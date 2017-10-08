#ifndef _SSD1306_H_
#define _SSD1306_H_

#include <stdint.h>

#define SELECTED 1
#define NOT_SELECTED 0

#define SCREEN_ADDR (0x3c)

#define SCREEN_DISPLAYALL_RESUME (0xA4)
#define SCREEN_DISPLAYALL (0xA5)
#define SCREEN_SETNORMAL (0xA6)
#define SCREEN_SETINVERSE (0xA7)
#define SCREEN_DISPLAY_ON (0xAF)
#define SCREEN_DISPLAY_OFF (0xAE)

#define SCREEN_SETDISPLAYCLOCKDIV (0xD5)
#define SCREEN_SETDISPLAYOFFSET (0xD3)
#define SCREEN_SETSTARTLINE (0x40) /* Need to OR this with your start line value */
#define SCREEN_SETMULTIPLEX (0xA8) /* Sets MUX ratio to N+1 MUX. Follow this command with MUX value, which is height of screen - 1 */
#define SCREEN_SETCHARGEPUMP (0x8D) /* Follow this with desired setting. We should be using the charge pump, so follow by 0x14 */
#define SCREEN_SETMEMORYMODE (0x20) /* To use horizontal addressing mode, follow with 0x00 */
#define SCREEN_SEGREMAP (0xA0) /* Decide if you want to switch order of segment indexes */
#define SCREEN_COMSCANDIR (0xC8) /* Remap COM scan drection */
#define SCREEN_SETCOMPINS (0xDA) /* Set COM pin configuration. Follow this command with desired configuration */
#define SCREEN_SETCONTRAST (0x81) /* Follow this command with desired constrast setting */
#define SCREEN_SETPRECHARGE (0xD9) /* Follow this command with desired precharge rate */
#define SCREEN_SETVCOMDETECT (0xDB) /* Follow with detect level */
#define SCREEN_STOP_SCROLL (0x2E)

#define SCREEN_SETCOLUMNADDR (0x21) /* Follow this with two parameters for both column start and end addresses. Sets column address pointer to column start address */
#define SCREEN_SETPAGEADDR (0x22)  /* Same as above basically */

void power_on_seq(void);
static uint8_t send_command(uint8_t command);
uint8_t screen_init(void);
uint8_t screen_display(void);
void clear_display(void);
void draw_pixel(uint8_t x, uint8_t y, uint8_t data);
void draw_char(uint8_t x, uint8_t y, unsigned char c, uint8_t size, uint8_t inverse);
void draw_string(uint8_t x, uint8_t y, char* s, uint8_t size, uint8_t inverse);
void draw_space_between_chars(uint8_t x, uint8_t y, uint8_t data);
void draw_solid_rectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t data);
void draw_vertical_line_slow(uint8_t x, uint8_t y, uint8_t length, uint8_t data);

uint16_t get_curr_col(void);
uint16_t get_curr_row(void);

#endif /*_SSD1306_H_*/
