LCD_DRV_MK_DIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
LCD_DRV_DIR := $(LCD_DRV_MK_DIR)

COMMON_VPATH += $(LCD_DRV_DIR)
SRC += $(LCD_DRV_DIR)/lcd.c
SRC += $(LCD_DRV_DIR)/uart3.c
