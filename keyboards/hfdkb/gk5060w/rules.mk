ifeq ($(strip $(CONSOLE_ENABLE)), yes)
    KEYBOARD_SHARED_EP = no
endif

I2C_DRIVER_REQUIRED = yes

UART_DRIVER_REQUIRED = yes

SRC += common/bt_task.c
SRC += common/lp_sleep.c
SRC += common/retarget_suspend.c
SRC += common/cw2017.c

# RULES_MK_DIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
# LDFLAGS += -L $(RULES_MK_DIR)/common -l_bts

OPT_DEFS += -DBT_MODE_ENABLE
OPT_DEFS += -DENTRY_STOP_MODE
OPT_DEFS += -DNO_USB_STARTUP_CHECK

# include keyboards/hfdkb/gk5060w/common/common.mk
LDFLAGS += -L $(TOP_DIR)/keyboards/hfdkb/gk5060w/common -l_bts

VPATH += $(TOP_DIR)/keyboards/hfdkb/gk5060w/common
