ifeq ($(filter UART16550, $(SW_MODULES)),)

SW_MODULES+=UART16550

include $(UART16550_DIR)/software/software.mk

endif
