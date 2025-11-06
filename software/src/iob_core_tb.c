/*
 * SPDX-FileCopyrightText: 2025 IObundle
 *
 * SPDX-License-Identifier: MIT
 */

#include "iob_uart16550.h"

#include <stdio.h>

#define UART16550_ADDR_W (5)
#define UART0_BASE (0)
#define UART1_BASE (1 << UART16550_ADDR_W)

int iob_core_tb() {

  int failed = 0;

  // print welcome message
  printf("IOB UART16550 testbench\n");

  // print the reset message
  printf("Reset complete\n");

  // init UART0
  uart16550_init(UART0_BASE, 3);
  // init UART0
  uart16550_init(UART1_BASE, 3);

  uint32_t i, word;

  printf("UART16550 test complete.\n");
  return failed;
}
