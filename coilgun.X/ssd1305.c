#include "ssd1305.h"
#include "driver.h"

void ssd1305_init(const ssd1305_t *const disp) {
    port_clear(disp->port_reset, disp->pin_reset);
    delay_us(3);
    port_set(disp->port_reset, disp->pin_reset);

    ssd1305_cmd(disp, SSD1305_CMD_DISPLAY_MODE(SSD1305_DISPLAY_MODE_ON));

    delay_ms(100);

    ssd1305_cmd2(disp, SSD1305_CMD_ADDRMODE, SSD1305_ADDRMODE_HORIZ);
    ssd1305_cmd(disp, SSD1305_CMD_SET_COM_REMAP(1));
    ssd1305_clear(disp);
}

void ssd1305_cmd(const ssd1305_t *const disp, const uint8_t cmd) {
    port_clear(disp->port_dc, disp->pin_dc);
    spi_write_byte(disp->sercom, cmd);
}

void ssd1305_cmd2(const ssd1305_t *const disp, const uint8_t cmd, const uint8_t param) {
    port_clear(disp->port_dc, disp->pin_dc);
    spi_write_byte(disp->sercom, cmd);
    spi_write_byte(disp->sercom, param);
}

void ssd1305_cmd3(const ssd1305_t *const disp, const uint8_t cmd, const uint8_t param1, const uint8_t param2) {
    port_clear(disp->port_dc, disp->pin_dc);
    spi_write_byte(disp->sercom, cmd);
    spi_write_byte(disp->sercom, param1);
    spi_write_byte(disp->sercom, param2);
}

void ssd1305_data(const ssd1305_t *const disp, const uint8_t *buf, const int len) {
    port_set(disp->port_dc, disp->pin_dc);
    spi_write(disp->sercom, buf, len);
}

void ssd1305_clear(const ssd1305_t *const disp) {
    static const uint32_t nbyte = (SSD1305_MAX_COL + 1) * (SSD1305_MAX_PAGE + 1);

    port_set(disp->port_dc, disp->pin_dc);
    spi_repeat_byte(disp->sercom, 0x00, nbyte);
}
