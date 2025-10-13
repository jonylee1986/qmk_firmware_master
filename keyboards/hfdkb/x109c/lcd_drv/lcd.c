#include "quantum.h"
#include "uart3.h"
#include "uart.h"
#include "x109c.h"
#include "lcd.h"

void LCD_IND_update(void) {
    uint8_t IND_data[3];
    IND_data[0] = 0x02;
    if (host_keyboard_led_state().num_lock)
        IND_data[1] |= 0x01;
    else
        IND_data[1] &= ~0x01;
    if (host_keyboard_led_state().caps_lock)
        IND_data[1] |= 0x02;
    else
        IND_data[1] &= ~0x02;
    if (host_keyboard_led_state().scroll_lock)
        IND_data[1] |= 0x04;
    else
        IND_data[1] &= ~0x04;
    if (keymap_config.no_gui)
        IND_data[1] |= 0x08;
    else
        IND_data[1] &= ~0x08;
    if ((get_highest_layer(default_layer_state) == 2))
        IND_data[1] |= 0x10;
    else
        IND_data[1] &= ~0x10;

    IND_data[1] &= ~0xE0;
    switch (dev_info.devs) {
        case DEVS_HOST1: {
            IND_data[1] |= 0x20;
        } break;
        case DEVS_HOST2: {
            IND_data[1] |= 0x40;
        } break;
        case DEVS_HOST3: {
            IND_data[1] |= 0x80;
        } break;
        case DEVS_2_4G: {
            IND_data[1] |= 0xE0;
        } break;
        default:
            break;
    }
    IND_data[2] = IND_data[0] + IND_data[1];
    uart3_transmit(IND_data, 3);
}

void LCD_Page_update(uint8_t page_num) {
    uint8_t Page_data[3];
    Page_data[0] = 0x01;
    switch (page_num) {
        case 0:
            Page_data[1] = 0x01;
            break;
        case 1:
            Page_data[1] = 0x02;
            break;
        case 2:
            Page_data[1] = 0x03;
            break;
        default:
            return;
    }
    Page_data[2] = Page_data[0] + Page_data[1];
    uart3_transmit(Page_data, 3);
}

void LCD_vol_update(bool vol_inc) {
    uint8_t vol_data[3];
    vol_data[0] = 0x03;
    if (vol_inc)
        vol_data[1] = 0x01;
    else
        vol_data[1] = 0x02;
    vol_data[2] = vol_data[0] + vol_data[1];
    uart3_transmit(vol_data, 3);
}

void LCD_charge_update(void) {
    uint8_t charge_data[3];
    charge_data[0] = 0x04;
    charge_data[1] = bts_info.bt_info.pvol;
    if (!readPin(BT_CABLE_PIN))
        charge_data[1] |= 0x80;
    else
        charge_data[1] &= ~0x80;
    charge_data[2] = charge_data[0] + charge_data[1];
    uart3_transmit(charge_data, 3);
}

void LCD_command_update(uint8_t LCD_command) {
    uint8_t command_data[3];
    command_data[0] = 0x05;
    command_data[1] = LCD_command;
    command_data[2] = command_data[0] + command_data[1];
    uart3_transmit(command_data, 3);
}

void LCD_DATA_receive(void) {
    uint8_t lcd_data[3];
    if (uart3_available()) {
        // lcd_data[0] = uart3_read();
        // lcd_data[1] = uart3_read();
        // lcd_data[2] = uart3_read();

        uart3_receive(lcd_data, 3);

        lcd_data[0] = 0xa7;
        lcd_data[1] = (lcd_data[1] << 1) >> 1;
        lcd_data[2] = (lcd_data[0] + lcd_data[1]) & 0xff;

        uart_transmit(lcd_data, 3);

        uint8_t charge_data[3];
        charge_data[0] = 0x04;
        charge_data[1] = lcd_data[1];
        if (!readPin(BT_CABLE_PIN))
            charge_data[1] |= 0x80;
        else
            charge_data[1] &= ~0x80;
        charge_data[2] = charge_data[0] + charge_data[1];
        uart3_transmit(charge_data, 3);

        // uart_write(lcd_data[0]);
        // uart_write(lcd_data[1]);
        // uart_write(lcd_data[2]);
// #ifdef CONSOLE_ENABLE
    // dprintf("当前电量:%d\n",lcd_data[1]);
        // dprintf("power");
// #endif

    }
}

// typedef enum {
//     lcd_bat = 0x04
// } lcd_header;

// typedef enum {
//     ble_bat = 0x04
// } ble_header;

void lcd_bat_receive(void) {
    uint8_t lcd_data[3];
    if (uart3_available()) {
        uart3_receive(lcd_data, 3);

        // if(lcd_data[0] == 0x04) {
        if(lcd_data[0] == 0xa7) {
            uint8_t ble_bat[3];
            ble_bat[0] = 0xa7;
            // ble_bat[1] = (lcd_data[1] << 1) >> 1;
            ble_bat[1] = lcd_data[1];
            ble_bat[2] = (ble_bat[0] + ble_bat[1]) & 0xff;
            uart_transmit(ble_bat, 3);

            // static uint8_t prev_bat = 0;

            // uint8_t charge_data[3];
            // charge_data[0] = 0x04;
            // // charge_data[1] = lcd_data[1];
            // charge_data[1] = 0;
            // if (!readPin(BT_CABLE_PIN))
            //     charge_data[1] |= 0x80;
            // else
            //     charge_data[1] &= ~0x80;
            // charge_data[2] = charge_data[0] + charge_data[1];
            // uart3_transmit(charge_data, 3);
        }
    }
}

void lcd_cable_send(void) {
    uint8_t charge_data[3];
    charge_data[0] = 0x04;
    charge_data[1] = 0;
    if (!readPin(BT_CABLE_PIN))
        charge_data[1] |= 0x80;
    else
        charge_data[1] &= ~0x80;
    charge_data[2] = charge_data[0] + charge_data[1];
    uart3_transmit(charge_data, 3);
}

// void ble_bat_send(void) {

// }
