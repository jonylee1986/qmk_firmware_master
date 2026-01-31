
QUANTUM_LIB_SRC += uart_serial.c

SRC += common/bt_task.c
SRC += common/lp_sleep.c
SRC += common/retarget_suspend.c
# SRC += common/cw2017.c

LDFLAGS += -L $(TOP_DIR)/keyboards/hfdkb/k982c/common -l_bts

VPATH += $(TOP_DIR)/keyboards/hfdkb/k982c/common
