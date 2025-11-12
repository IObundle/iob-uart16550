/*
 * SPDX-FileCopyrightText: 2025 IObundle
 *
 * SPDX-License-Identifier: MIT
 */

#include "iob_uart16550_csrs.h"

#include <stdint.h>
#include <stdio.h>

#define UART16550_ADDR_W (5)
#define UART0_BASE (0)
#define UART1_BASE (1 << UART16550_ADDR_W)

#define BYTE_1 (0x81)
#define BYTE_2 (0x42)

static inline void set_bit(uint8_t *v, int bit) { (*v) |= (1 << bit); }

static inline void clr_bit(uint8_t *v, int bit) { (*v) &= ~(1 << bit); }

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

inline uint8_t uart_data_ready() {
  return (iob_uart16550_csrs_get_ls() & (1 << IOB_UART16550_LS_DR));
}

inline uint8_t uart_overrun_error() {
  return (iob_uart16550_csrs_get_ls() & (1 << IOB_UART16550_LS_OE));
}

inline uint8_t uart_parity_error() {
  return (iob_uart16550_csrs_get_ls() & (1 << IOB_UART16550_LS_PE));
}

inline uint8_t uart_framing_error() {
  return (iob_uart16550_csrs_get_ls() & (1 << IOB_UART16550_LS_FE));
}

inline uint8_t uart_transmitter_empty() {
  return (iob_uart16550_csrs_get_ls() & (1 << IOB_UART16550_LS_TE));
}

inline uint8_t uart_break_interrupt() {
  return (iob_uart16550_csrs_get_ls() & (1 << IOB_UART16550_LS_BI));
}

inline uint8_t uart_pending_interrupt() {
  return ((iob_uart16550_csrs_get_ii() & (1 << IOB_UART16550_II_PND)) == 0);
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

int test_read_regs(uint32_t base_address) {
  uint8_t ret = 0;

  // set base address
  iob_uart16550_csrs_init_baseaddr(base_address);

  ret = iob_uart16550_csrs_get_rb();
  printf("\tRB: %x\n", ret);
  ret = iob_uart16550_csrs_get_ie();
  printf("\tIE: %x\n", ret);
  ret = iob_uart16550_csrs_get_ii();
  printf("\tII: %x\n", ret);
  ret = iob_uart16550_csrs_get_lc();
  printf("\tLC: %x\n", ret);
  ret = iob_uart16550_csrs_get_ls();
  printf("\tLS: %x\n", ret);
  ret = iob_uart16550_csrs_get_ms();
  printf("\tMS: %x\n", ret);
  ret = iob_uart16550_csrs_get_dl1();
  printf("\tDL1: %x\n", ret);
  ret = iob_uart16550_csrs_get_dl2();
  printf("\tDL2: %x\n", ret);
  return 0;
}

int test_addr(uint32_t base_address) {
  uint32_t max_addr = (1 << IOB_UART16550_CSRS_CSRS_ADDR_W) - 1;
  iob_read(base_address + max_addr, IOB_UART16550_CSRS_W);
  return 0;
}

int test_read_debug_regs(uint32_t base_address) {
  // set base address
  iob_uart16550_csrs_init_baseaddr(base_address);

  printf("\tDebug1: %x\n", iob_uart16550_csrs_get_db1());
  printf("\tDebug2: %x\n", iob_uart16550_csrs_get_db2());
  return 0;
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

void reset_uart(uint8_t base_address) {
  uint8_t cmd = 0;
  uint8_t ret = 0;
  iob_uart16550_csrs_init_baseaddr(base_address);
  // default interrupts
  iob_uart16550_csrs_set_ie(0x00);
  ret = iob_uart16550_csrs_get_ii();
  // Clear FIFOs
  cmd = (1 << IOB_UART16550_FC_RF);  // clear RX FIFO
  cmd |= (1 << IOB_UART16550_FC_TF); // clear TX FIFO
  iob_uart16550_csrs_set_fc(cmd);
  cmd = 0b11000000;
  iob_uart16550_csrs_set_fc(cmd); // default value
  cmd = 0b11;
  iob_uart16550_csrs_set_lc(cmd); // default value
  cmd = 0;
  iob_uart16550_csrs_set_mc(cmd); // default value
  // clear pending status / data
  // read data until empty
  while (uart_data_ready()) {
    ret = iob_uart16550_csrs_get_rb();
  }
  ret = iob_uart16550_csrs_get_ms();
}

int test_line_status(uint32_t test_base, uint32_t aux_base) {
  int failed = 0;
  int timeout = 500;
  int ticks = 0;
  int i = 0;
  uint8_t ret = 0;
  uint8_t cmd = 0;
  // bit 0: Data Ready indicator
  // Send test byte
  iob_uart16550_csrs_init_baseaddr(aux_base);
  iob_uart16550_csrs_set_tr(0x55);
  iob_uart16550_csrs_init_baseaddr(test_base);
  while ((ticks < timeout) & (uart_data_ready() == 0)) {
    ticks++;
  }
  failed += (ticks >= timeout);
  if (failed) {
    printf("Error: Data Ready timeout\n");
  }
  // bit 1: Overrun Error - FIFO DEPTH = 256
  iob_uart16550_csrs_init_baseaddr(aux_base);
  for (i = 0; i < 256; i++) {
    iob_uart16550_csrs_set_tr((i & 0xFF));
    while (uart_transmitter_empty() == 0)
      ; // wait to send data
  }
  iob_uart16550_csrs_init_baseaddr(test_base);
  failed += (uart_overrun_error() == 0);
  if (failed) {
    printf("Error: Overrun Error not set\n");
  }
  reset_uart(test_base);
  reset_uart(aux_base);
  // bit 2: Parity Error indicator
  // configure each uart with different parity configurations to trigger error
  iob_uart16550_csrs_init_baseaddr(test_base);
  cmd = iob_uart16550_csrs_get_lc();
  set_bit(&cmd, IOB_UART16550_LC_PE); // Parity Enable (1)
  set_bit(&cmd, IOB_UART16550_LC_EP); // Even Parity Select (1)
  iob_uart16550_csrs_set_lc(cmd);
  iob_uart16550_csrs_init_baseaddr(aux_base);
  cmd = iob_uart16550_csrs_get_lc();
  set_bit(&cmd, IOB_UART16550_LC_PE); // Parity Enable (1)
  clr_bit(&cmd, IOB_UART16550_LC_EP); // Even Parity Select (0)
  iob_uart16550_csrs_set_lc(cmd);
  iob_uart16550_csrs_init_baseaddr(aux_base);
  iob_uart16550_csrs_set_tr(0x55);
  while (uart_transmitter_empty() == 0)
    ; // wait to send data
  iob_uart16550_csrs_init_baseaddr(test_base);
  failed += (uart_parity_error() == 0);
  if (failed) {
    printf("Error: Parity Error not set\n");
  }
  reset_uart(test_base);
  reset_uart(aux_base);
  // bit 3: Framing Error indicator
  // mismatching number of character bits: TX sends 8bit, RX expects 5bit, reads
  // last 3 bits as parity and stop bits
  iob_uart16550_csrs_init_baseaddr(test_base);
  cmd = iob_uart16550_csrs_get_lc();
  clr_bit(&cmd, IOB_UART16550_LC_BITS);
  clr_bit(&cmd, (IOB_UART16550_LC_BITS + 1)); // 5 bits per character
  iob_uart16550_csrs_set_lc(cmd);
  iob_uart16550_csrs_init_baseaddr(aux_base);
  cmd = iob_uart16550_csrs_get_lc();
  set_bit(&cmd, IOB_UART16550_LC_BITS);
  set_bit(&cmd, (IOB_UART16550_LC_BITS + 1)); // 8 bits per character
  iob_uart16550_csrs_set_lc(cmd);
  iob_uart16550_csrs_set_tr(0x1F); // 3 MSBs set to 0 (last 3 transmitted bits)
  while (uart_transmitter_empty() == 0)
    ; // wait to send data
  iob_uart16550_csrs_init_baseaddr(test_base);
  failed += (uart_framing_error() == 0);
  if (failed) {
    printf("Error: Framing Error not set\n");
  }
  reset_uart(test_base);
  reset_uart(aux_base);
  // bit 4: Break Interrupt
  iob_uart16550_csrs_init_baseaddr(test_base);
  cmd = iob_uart16550_csrs_get_ie();
  set_bit(&cmd, (IOB_UART16550_IE_RLS)); // enable Line Status interrupts
  iob_uart16550_csrs_set_ie(cmd);
  iob_uart16550_csrs_init_baseaddr(aux_base);
  cmd = iob_uart16550_csrs_get_lc();
  set_bit(&cmd, IOB_UART16550_LC_BC); // enable break state
  iob_uart16550_csrs_set_lc(cmd);
  iob_uart16550_csrs_init_baseaddr(test_base);
  // wait for Line Status Interrupt
  while (uart_pending_interrupt() == 0)
    ;
  failed += (uart_break_interrupt() == 0);
  if (failed) {
    printf("Error: Break Interrupt not set\n");
  }
  reset_uart(test_base);
  reset_uart(aux_base);
  //
  // bits 5-7 triggered by previous conditions

  return failed;
}

int test_rdata(uint32_t read_base, uint32_t write_base) {
  int failed = 0;
  uint8_t ret = 0;
  // rdata[7:0]
  failed += test_single_byte(write_base, read_base, 0xFF);
  // rdata[15:8]
  // test line status register
  failed += test_line_status(read_base, write_base);
  // rdata[23:16]
  // Modem status register not exercised
  // rdata[31:24]
  iob_uart16550_csrs_init_baseaddr(read_base);
  iob_uart16550_csrs_set_lc(0xFF);
  ret = iob_uart16550_csrs_get_lc();
  printf("\tLC: %x\n", ret);
  iob_uart16550_csrs_set_lc(0b00);
  ret = iob_uart16550_csrs_get_lc();
  printf("\tLC: %x\n", ret);
  iob_uart16550_csrs_set_lc(0b11);

  return failed;
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

  // Exercise read registers
  failed += test_read_regs(UART0_BASE);
  failed += test_read_debug_regs(UART0_BASE);
  failed += test_read_regs(UART1_BASE);
  failed += test_read_debug_regs(UART1_BASE);

  failed += test_addr(UART0_BASE);
  failed += test_addr(UART1_BASE);

  failed += test_rdata(UART0_BASE, UART1_BASE);
  failed += test_rdata(UART1_BASE, UART0_BASE);

  printf("UART16550 test complete.\n");
  return failed;
}
