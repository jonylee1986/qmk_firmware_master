BLED_MK_DIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
BLED_DIR := $(BLED_MK_DIR)

COMMON_VPATH += $(BLED_DIR)
SRC += $(BLED_DIR)/bled.c
