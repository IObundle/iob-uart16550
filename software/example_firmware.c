/*
 * SPDX-FileCopyrightText: 2025 IObundle
 *
 * SPDX-License-Identifier: MIT
 */

#include "iob_printf.h"
#include "iob_uart16550.h"

#define FREQ 100000000
#define BAUD 3000000

int main() {
  // Init uart16550
  uart16550_init(UART16550_BASE, FREQ / BAUD);

  printf("\nHello world!\n");

  uart16550_finish();
}
