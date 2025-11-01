#include "quantum.h"
#include "uart3.h"
#include "common/bt_task.h"

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
    if ((get_highest_layer(default_layer_state) == 3))
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
    uint8_t charge_data[3] = {0};

    charge_data[0] = 0x04;
    // charge_data[1] = bts_info.bt_info.pvol;

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
