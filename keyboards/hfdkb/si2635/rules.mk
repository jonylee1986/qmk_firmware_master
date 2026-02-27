ifeq ($(strip $(CONSOLE_ENABLE)), yes)
    KEYBOARD_SHARED_EP = no
endif

MODULE_DIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))

OPT_DEFS += -DMULTIMODE_ENABLE
OPT_DEFS += -DENTRY_STOP_MODE
OPT_DEFS += -DNO_USB_STARTUP_CHECK

include $(MODULE_DIR)/common/common.mk
include $(MODULE_DIR)/bled/bled.mk

VPATH += $(MODULE_DIR)/common/
VPATH += $(MODULE_DIR)/bled/

LDFLAGS += -L $(MODULE_DIR)/common -l_bts
