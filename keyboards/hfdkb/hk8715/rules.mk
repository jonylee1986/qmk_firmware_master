# Extra module configuration
MODULE_DIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
include $(MODULE_DIR)/bled/bled.mk
