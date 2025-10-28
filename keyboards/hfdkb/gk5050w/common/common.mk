THIS_MK_DIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
COMMON_DIR := $(THIS_MK_DIR)

ifeq ($(strip $(MULTIMODE_ENABLE)), yes)
    OPT_DEFS += -DMULTIMODE_ENABLE
    OPT_DEFS += -DENTRY_STOP_MODE
    OPT_DEFS += -DNO_USB_STARTUP_CHECK

    QUANTUM_LIB_SRC += uart_serial.c

    COMMON_VPATH += $(COMMON_DIR)

    SRC += $(COMMON_DIR)/bt_task.c
    SRC += $(COMMON_DIR)/retarget_suspend.c
    SRC += $(COMMON_DIR)/lp_sleep.c
    
    LDFLAGS += -L $(COMMON_DIR)/ -l_bts
endif
