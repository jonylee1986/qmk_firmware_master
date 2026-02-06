ifeq ($(strip $(CONSOLE_ENABLE)), yes)
    KEYBOARD_SHARED_EP = no
endif

OPT_DEFS += -DBT_MODE_ENABLE
OPT_DEFS += -DWWDG_ENABLE
OPT_DEFS += -DENTRY_STOP_MODE
OPT_DEFS += -DNO_USB_STARTUP_CHECK

# QUANTUM_LIB_SRC += uart_serial.c
UART_DRIVER_REQUIRED = yes

SRC += common/bt_task.c
SRC += common/lp_sleep.c
SRC += common/retarget_suspend.c

RULES_MK_DIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
LDFLAGS += -L $(RULES_MK_DIR)/common -l_bts
