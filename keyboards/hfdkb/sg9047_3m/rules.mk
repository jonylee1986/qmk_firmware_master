ifeq ($(strip $(CONSOLE_ENABLE)), yes)
    KEYBOARD_SHARED_EP = no
endif

# Extra module configuration
MODULE_DIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
include $(MODULE_DIR)/common/common.mk
include $(MODULE_DIR)/bled/bled.mk
