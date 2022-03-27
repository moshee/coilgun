#ifndef SSD1305_H
#define	SSD1305_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "samc20j16a.h"

#define SSD1305_MAX_COL 127
#define SSD1305_MAX_PAGE 7

#define SSD1305_CMD_PAGE_COL_ADDRL(x) ((x) & 0x0F)
#define SSD1305_CMD_PAGE_COL_ADDRH(x) ((((x) >> 4) & 0x0F) | 0x10)
#define SSD1305_CMD_ADDRMODE 0x20
#define SSD1305_CMD_COL_ADDR 0x21
#define SSD1305_CMD_PAGE_ADDR 0x22
#define SSD1305_CMD_DISPLAY_START_LINE(x) (((x) & 0x3F) | 0x40)
#define SSD1305_CMD_CONTRAST 0x81
#define SSD1305_CMD_CONTRAST_COLOR 0x82
#define SSD1305_CMD_LUT 0x91
#define SSD1305_CMD_SEGMENT_REMAP(x) (((x) & 0x01) | 0xA0)
#define SSD1305_CMD_DISPLAY_ALL_ON(x) (((x) & 0x01) | 0xA4)
#define SSD1305_CMD_INVERSE_DISPLAY(x) (((x) & 0x01) | 0xA6)
#define SSD1305_CMD_MUX_RATIO 0xA8
#define SSD1305_CMD_DIM_MODE 0xAB
#define SSD1305_CMD_DISPLAY_MODE(x) (((x) & 0x03) | 0xAC)
#define SSD1305_CMD_PAGE_START_ADDR(x) (((x) & 0x07) | 0xB0)
#define SSD1305_CMD_SET_COM_REMAP(x) ((((x) & 0x01) << 3) | 0xC0)
#define SSD1305_CMD_SET_COM_CONFIG 0xDA
#define SSD1305_COM_CONFIG(x) ((((x) & 0x03) << 4) | 0x02)
#define SSD1305_COM_CONFIG_SEQ_COM 0x00
#define SSD1305_COM_CONFIG_ALT_COM 0x01
#define SSD1305_COM_CONFIG_NO_REMAP 0x00
#define SSD1305_COM_CONFIG_LR_REMAP 0x02

#define SSD1305_ADDRMODE_HORIZ 0x00
#define SSD1305_ADDRMODE_VERT 0x01
#define SSD1305_ADDRMODE_PAGE 0x02

#define SSD1305_DISPLAY_MODE_DIM 0x00
#define SSD1305_DISPLAY_MODE_OFF 0x01
#define SSD1305_DISPLAY_MODE_ON 0x03

typedef struct ssd1305 {
	uint8_t port_reset, port_dc;
	uint32_t pin_reset, pin_dc;
	sercom_registers_t *sercom;
} ssd1305_t;

void ssd1305_init(const ssd1305_t *const disp);
void ssd1305_cmd(const ssd1305_t *const disp, const uint8_t cmd);
void ssd1305_cmd2(const ssd1305_t *const disp, const uint8_t cmd, const uint8_t param);
void ssd1305_cmd3(const ssd1305_t *const disp, const uint8_t cmd, const uint8_t param1, const uint8_t param2);
void ssd1305_data(const ssd1305_t *const disp, const uint8_t *buf, const int len);
void ssd1305_clear(const ssd1305_t *disp);

// void ssd1305_set_addr(const ssd1305_t *disp, uint8_t page, uint8_t col);

#ifdef	__cplusplus
}
#endif

#endif	/* SSD1305_H */

