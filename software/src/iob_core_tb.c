/*
 * SPDX-FileCopyrightText: 2025 IObundle
 *
 * SPDX-License-Identifier: MIT
 */

#include "iob_uart16550_csrs.h"

#include <stdio.h>

#define UART16550_ADDR_W (5)
#define UART0_BASE (0)
#define UART1_BASE (1 << UART16550_ADDR_W)

void uart16550_init(uint32_t base_address, uint16_t div) {
  uint8_t div1 = (uint8_t)(div & 0xFF);
  uint8_t div2 = (uint8_t)((div >> 8) & 0xFF);

  // set base address
  iob_uart16550_csrs_init_baseaddr(base_address);

  // set divisor latches
  iob_uart16550_csrs_set_dl1(div1);
  iob_uart16550_csrs_set_dl2(div2);

  // set FIFO trigger level: 14 bits
  iob_uart16550_csrs_set_fc(IOB_UART16550_FC_TL_14 << IOB_UART16550_FC_TL);

  // set interrupt configuration
  uint8_t int_en_cfg = (1 << IOB_UART16550_IE_RDA); // Received Data Available
  int_en_cfg |=
      (1 << IOB_UART16550_IE_THRE); // Transmitter Holding Register Empty
  iob_uart16550_csrs_set_ie(int_en_cfg);
}

int iob_core_tb() {

  int failed = 0;

  // print welcome message
  printf("IOB UART16550 testbench\n");

  // print the reset message
  printf("Reset complete\n");

  // init UART0
  uart16550_init(UART0_BASE, 3);

  // init UART1
  uart16550_init(UART1_BASE, 3);

  uint32_t i, word;

  printf("UART16550 test complete.\n");
  return failed;
}
