LED_MK_DIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
LED_DIR := $(LED_MK_DIR)

COMMON_VPATH += $(LED_DIR)
SRC += $(LED_DIR)/led.c
