#include "quantum.h"
#include "common/bt_task.h"
#include "lcd.h"
#include "usb_main.h"

void LCD_IND_update(void) {
    uint8_t IND_data[5] = {0};

    IND_data[0] = 0xAA;
    IND_data[1] = 0x02;

    if (host_keyboard_led_state().num_lock)
        IND_data[2] |= 0x01;
    else
        IND_data[2] &= ~0x01;
    if (host_keyboard_led_state().caps_lock)
        if (dev_info.devs == DEVS_USB && USB_DRIVER.state != USB_ACTIVE) {
            IND_data[2] &= ~0x02;
        } else {
            IND_data[2] |= 0x02;
        }
    else
        IND_data[2] &= ~0x02;
    if (host_keyboard_led_state().scroll_lock)
        IND_data[2] |= 0x04;
    else
        IND_data[2] &= ~0x04;
    if (keymap_config.no_gui)
        IND_data[2] |= 0x08;
    else
        IND_data[2] &= ~0x08;

    if ((get_highest_layer(default_layer_state) == 2))
        IND_data[2] |= 0x10;
    else
        IND_data[2] &= ~0x10;

    IND_data[2] &= ~0xE0;
    switch (dev_info.devs) {
        case DEVS_HOST1: {
            IND_data[2] |= 0x20;
        } break;
        case DEVS_HOST2: {
            IND_data[2] |= 0x40;
        } break;
        case DEVS_HOST3: {
            IND_data[2] |= 0x80;
        } break;
        case DEVS_2_4G: {
            IND_data[2] |= 0xE0;
        } break;
        default:
            break;
    }

    IND_data[3] = IND_data[1] + IND_data[2];
    IND_data[4] = 0xBB;
    uart3_transmit(IND_data, 5);
}

void LCD_Page_update(uint8_t page_num) {
    uint8_t Page_data[5] = {0};

    Page_data[0] = 0xAA;
    Page_data[1] = 0x01;
    switch (page_num) {
        case 0:
            Page_data[2] = 0x01;
            break;
        case 1:
            Page_data[2] = 0x02;
            break;
        case 2:
            Page_data[2] = 0x03;
            break;
        default:
            return;
    }
    Page_data[3] = Page_data[1] + Page_data[2];
    Page_data[4] = 0xBB;
    uart3_transmit(Page_data, 5);
}

void LCD_vol_update(bool vol_inc) {
    uint8_t vol_data[5] = {0};

    vol_data[0] = 0xAA;
    vol_data[1] = 0x03;
    if (vol_inc)
        vol_data[2] = 0x01;
    else
        vol_data[2] = 0x02;
    vol_data[3] = vol_data[1] + vol_data[2];
    vol_data[4] = 0xBB;
    uart3_transmit(vol_data, 5);
}

extern bool is_charging(void);
extern bool is_fully_charged(void);

void LCD_charge_update(void) {
    uint8_t charge_data[5] = {0};

    charge_data[0] = 0xAA;
    charge_data[1] = 0x04;

    if (!readPin(MM_CABLE_PIN)) {
        if (readPin(MM_CHARGE_PIN)) {
            charge_data[2] &= ~0x80;
        } else {
            charge_data[2] |= 0x80;
        }
    } else {
        charge_data[2] &= ~0x80;
    }

    charge_data[3] = charge_data[1] + charge_data[2];
    charge_data[4] = 0xBB;
    uart3_transmit(charge_data, 5);
}

void LCD_command_update(uint8_t LCD_command) {
    uint8_t command_data[5] = {0};

    command_data[0] = 0xAA;
    command_data[1] = 0x05;
    command_data[2] = LCD_command;
    command_data[3] = command_data[1] + command_data[2];
    command_data[4] = 0xBB;
    uart3_transmit(command_data, 5);
}
