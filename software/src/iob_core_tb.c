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

#define BYTE_1 (0x81)
#define BYTE_2 (0x42)

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

uint8_t uart_data_ready() {
  return (iob_uart16550_csrs_get_ls() & (1 << IOB_UART16550_LS_DR));
}

int test_single_byte(uint32_t send_addr, uint32_t rcv_addr, uint8_t byte) {
  int failed = 0;
  int timeout = 500;
  int ticks = 0;

  // Send test byte
  iob_uart16550_csrs_init_baseaddr(send_addr);
  iob_uart16550_csrs_set_tr(byte);
  printf("Sending: %x\n", byte);

  // Receive test bytes
  // wait for data ready
  iob_uart16550_csrs_init_baseaddr(rcv_addr);
  while ((ticks < timeout) & (uart_data_ready() == 0)) {
    ticks++;
  }
  uint8_t rcv_data = iob_uart16550_csrs_get_rb();
  printf("Data out: %x\n", rcv_data);
  if (rcv_data != byte) {
    printf("Error: expected %x but received %x\n", byte, rcv_data);
    failed = 1;
  }
  return failed;
}

int test_write_regs(uint32_t base_address) {
  iob_uart16550_csrs_init_baseaddr(base_address);
  // Transmitter Holding Register
  iob_uart16550_csrs_set_tr(0xFF);
  iob_uart16550_csrs_set_tr(0x00);
  // Interrupt Enable
  iob_uart16550_csrs_set_ie(0xFF);
  iob_uart16550_csrs_set_ie(0x00);
  // FIFO Control
  iob_uart16550_csrs_set_fc(0xFF);
  iob_uart16550_csrs_set_fc(0x00);
  iob_uart16550_csrs_set_fc(0b11000000);
  // Line Control
  iob_uart16550_csrs_set_lc(0xFF);
  iob_uart16550_csrs_set_lc(0b11);
  // Modem Control
  iob_uart16550_csrs_set_mc(0xFF);
  iob_uart16550_csrs_set_mc(0b11000000);
  return 0;
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

  // Send test bytes
  failed += test_single_byte(UART0_BASE, UART1_BASE, BYTE_1);
  failed += test_single_byte(UART0_BASE, UART1_BASE, BYTE_2);

  // Exercise write registers
  failed += test_write_regs(UART0_BASE);
  failed += test_write_regs(UART1_BASE);

  printf("UART16550 test complete.\n");
  return failed;
}
