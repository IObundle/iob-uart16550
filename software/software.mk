include $(UART16550_DIR)/config.mk

UART16550_SW_DIR:=$(UART16550_DIR)/software

#include
INCLUDE+=-I$(UART16550_SW_DIR)

#headers
HDR+=$(UART16550_SW_DIR)/*.h iob_uart16550_swreg.h

#sources
SRC+=$(UART16550_SW_DIR)/iob-uart.c

iob_uart16550_swreg.h:
	$(MKREGS) iob_uart16550 $(UART16550_DIR) SW
