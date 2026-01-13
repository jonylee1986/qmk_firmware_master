# Extra module configuration - must be defined first before use
# Select module variant: async, sync, sync_a1, or sync_a2
# Can be overridden: make MULTIMODE_VARIANT=async
MULTIMODE_VARIANT ?= async

KEYBOARD_BASE_DIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))

ifeq ($(strip $(MULTIMODE_VARIANT)), async)
    MODULE_DIR := $(KEYBOARD_BASE_DIR)/async/common
# else ifeq ($(strip $(MULTIMODE_VARIANT)), sync)
#     MODULE_DIR := $(KEYBOARD_BASE_DIR)/sync/common
else ifeq ($(strip $(MULTIMODE_VARIANT)), sync_a1)
    MODULE_DIR := $(KEYBOARD_BASE_DIR)/sync_a1/common
else ifeq ($(strip $(MULTIMODE_VARIANT)), sync_a2)
    MODULE_DIR := $(KEYBOARD_BASE_DIR)/sync_a2/common
else
    $(error Invalid MULTIMODE_VARIANT: $(MULTIMODE_VARIANT). Valid options: async, sync_a1, sync_a2)
endif

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
