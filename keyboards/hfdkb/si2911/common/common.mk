THIS_MK_DIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))

OPT_DEFS += -DMULTIMODE_ENABLE
OPT_DEFS += -DENTRY_STOP_MODE
OPT_DEFS += -DNO_USB_STARTUP_CHECK
# OPT_DEFS += -DWWDG_ENABLE

UART_DRIVER_REQUIRED = yes

COMMON_VPATH += $(THIS_MK_DIR)

SRC += $(THIS_MK_DIR)/bt_task.c
SRC += $(THIS_MK_DIR)/retarget_suspend.c
SRC += $(THIS_MK_DIR)/lp_sleep.c
SRC += $(THIS_MK_DIR)/wb32_wwdg.c
SRC += $(THIS_MK_DIR)/wwdg.c

LDFLAGS += -L $(THIS_MK_DIR) -l_bts
