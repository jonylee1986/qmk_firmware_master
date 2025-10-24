# UART_DRIVER_REQUIRED = yes

ifeq ($(strip $(CONSOLE_ENABLE)), yes)
    KEYBOARD_SHARED_EP = yes
endif

QUANTUM_LIB_SRC += uart_serial.c
SRC += common/bt_task.c
SRC += common/lp_sleep.c
SRC += common/retarget_suspend.c
SRC += lcd_drv/lcd.c
SRC += lcd_drv/uart3.c

RULES_MK_DIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
LDFLAGS += -L $(RULES_MK_DIR)/common -l_bts
