ifeq ($(strip $(CONSOLE_ENABLE)), yes)
    KEYBOARD_SHARED_EP = no
endif

RULES_MK_DIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))

OPT_DEFS += -DBT_MODE_ENABLE
OPT_DEFS += -DMULTIMODE_ENABLE
OPT_DEFS += -DENTRY_STOP_MODE
OPT_DEFS += -DNO_USB_STARTUP_CHECK

UART_DRIVER_REQUIRED = yes

VPATH += $(RULES_MK_DIR)/common

SRC += common/bt_task.c
SRC += common/retarget_suspend.c
SRC += common/lp_sleep.c

LDFLAGS += -L $(RULES_MK_DIR)/common -l_bts
