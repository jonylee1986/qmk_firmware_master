ifeq ($(strip $(CONSOLE_ENABLE)), yes)
    KEYBOARD_SHARED_EP = no
endif

# I2C_DRIVER_REQUIRED = yes

OPT_DEFS += -DMM_MODE_ENABLE
OPT_DEFS += -DENTRY_STOP_MODE
OPT_DEFS += -DNO_USB_STARTUP_CHECK

include keyboards/hfdkb/k982c/common/common.mk
include keyboards/hfdkb/k982c/lcd_drv/lcd_drv.mk
