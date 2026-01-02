ifeq ($(strip $(CONSOLE_ENABLE)), yes)
    KEYBOARD_SHARED_EP = no
endif

# I2C_DRIVER_REQUIRED = yes

OPT_DEFS += -DMM_MODE_ENABLE

include keyboards/hfdkb/k982c/common/common.mk
