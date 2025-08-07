EEPROM_DRIVER = wear_leveling
WEAR_LEVELING_DRIVER = spi_flash

ifeq ($(strip $(CONSOLE_ENABLE)), yes)
    KEYBOARD_SHARED_EP = yes
endif

# MCU_SERIES = WB32FQ95xx
# UART_DRIVER_REQUIRED = yes
QUANTUM_LIB_SRC += uart_serial.c
# QUANTUM_LIB_SRC += uart.c
SRC += common/bt_task.c
SRC += common/lp_sleep.c
SRC += common/retarget_suspend.c
SRC += lcd_drv/lcd.c
SRC += lcd_drv/uart3_serial.c

RULES_MK_DIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
LDFLAGS += -L $(RULES_MK_DIR)/common -l_bts
