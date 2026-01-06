# Extra module configuration - must be defined first before use
# /home/lalalademaxiya/Documents/qmk_firmware_master/keyboards/hfdkb/m89u/
MODULE_DIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))/async/common
# MODULE_DIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))/sync/common

ifeq ($(strip $(CONSOLE_ENABLE)), yes)
    KEYBOARD_SHARED_EP = no
endif

OPT_DEFS += -DMULTIMODE_ENABLE
OPT_DEFS += -DENTRY_STOP_MODE
OPT_DEFS += -DNO_USB_STARTUP_CHECK

UART_DRIVER_REQUIRED = yes

SRC += $(MODULE_DIR)/bt_task.c
SRC += $(MODULE_DIR)/retarget_suspend.c
SRC += $(MODULE_DIR)/lp_sleep.c

# include $(MODULE_DIR)/common/common.mk

LDFLAGS += -L $(MODULE_DIR)/ -l_bts
# LDFLAGS += -L $(TOP_DIR)/keyboards/hfdkb/m89u/async/common -l_bts

VPATH += $(MODULE_DIR)/
