ifeq ($(strip $(CONSOLE_ENABLE)), yes)
    KEYBOARD_SHARED_EP = no
endif

RULES_MK_DIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))

UART_DRIVER_REQUIRED = yes

OPT_DEFS += -DBT_MODE_ENABLE
OPT_DEFS += -DENTRY_STOP_MODE
OPT_DEFS += -DNO_USB_STARTUP_CHECK

SRC += common/bt_task.c
SRC += common/lp_sleep.c
SRC += common/retarget_suspend.c
SRC += lcd_drv/lcd.c
SRC += lcd_drv/uart3.c

LDFLAGS += -L $(RULES_MK_DIR)/common -l_bts
