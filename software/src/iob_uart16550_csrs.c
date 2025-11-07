/*
 * SPDX-FileCopyrightText: 2025 IObundle, Lda
 *
 * SPDX-License-Identifier: MIT
 */

#include "iob_uart16550_csrs.h"
#include <stdint.h>

static inline void set_bit(uint8_t *v, int bit) { *v |= (1 << bit); }

static inline void clr_bit(uint8_t *v, int bit) { *v &= ~(1 << bit); }

// Base Address
static uint32_t base;
void iob_uart16550_csrs_init_baseaddr(uint32_t addr) { base = addr; }

// Core Setters and Getters
uint8_t iob_uart16550_csrs_get_rb() {
  // ensure LCR bit 7 is 0
  uint8_t lcr = iob_uart16550_csrs_get_lc();
  uint8_t lcr_tmp = lcr;
  clr_bit(&lcr_tmp, IOB_UART16550_LC_DL);
  iob_uart16550_csrs_set_lc(lcr_tmp);
  // read receiver buffer
  uint8_t rcv_buf =
      iob_read(base + IOB_UART16550_CSRS_RB_ADDR, IOB_UART16550_CSRS_W);
  // restore LCR value
  iob_uart16550_csrs_set_lc(lcr);
  return rcv_buf;
}

void iob_uart16550_csrs_set_tr(uint8_t value) {
  // ensure LCR bit 7 is 0
  uint8_t lcr = iob_uart16550_csrs_get_lc();
  uint8_t lcr_tmp = lcr;
  clr_bit(&lcr_tmp, IOB_UART16550_LC_DL);
  iob_uart16550_csrs_set_lc(lcr_tmp);
  // write tx buffer
  iob_write(base + IOB_UART16550_CSRS_TR_ADDR, IOB_UART16550_CSRS_W, value);
  // restore LCR value
  iob_uart16550_csrs_set_lc(lcr);
}

uint8_t iob_uart16550_csrs_get_ie() {
  return iob_read(base + IOB_UART16550_CSRS_IE_ADDR, IOB_UART16550_CSRS_W);
}

void iob_uart16550_csrs_set_ie(uint8_t value) {
  iob_write(base + IOB_UART16550_CSRS_IE_ADDR, IOB_UART16550_CSRS_W, value);
}

uint8_t iob_uart16550_csrs_get_ii() {
  return iob_read(base + IOB_UART16550_CSRS_II_ADDR, IOB_UART16550_CSRS_W);
}

void iob_uart16550_csrs_set_fc(uint8_t value) {
  iob_write(base + IOB_UART16550_CSRS_FC_ADDR, IOB_UART16550_CSRS_W, value);
}

uint8_t iob_uart16550_csrs_get_lc() {
  return iob_read(base + IOB_UART16550_CSRS_LC_ADDR, IOB_UART16550_CSRS_W);
}

void iob_uart16550_csrs_set_lc(uint8_t value) {
  iob_write(base + IOB_UART16550_CSRS_LC_ADDR, IOB_UART16550_CSRS_W, value);
}

void iob_uart16550_csrs_set_mc(uint8_t value) {
  iob_write(base + IOB_UART16550_CSRS_MC_ADDR, IOB_UART16550_CSRS_W, value);
}

uint8_t iob_uart16550_csrs_get_ls() {
  return iob_read(base + IOB_UART16550_CSRS_LS_ADDR, IOB_UART16550_CSRS_W);
}

uint8_t iob_uart16550_csrs_get_ms() {
  return iob_read(base + IOB_UART16550_CSRS_MS_ADDR, IOB_UART16550_CSRS_W);
}

uint8_t iob_uart16550_csrs_get_dl1() {
  // ensure LCR bit 7 is 1
  uint8_t lcr = iob_uart16550_csrs_get_lc();
  uint8_t lcr_tmp = lcr;
  set_bit(&lcr_tmp, IOB_UART16550_LC_DL);
  iob_uart16550_csrs_set_lc(lcr_tmp);
  uint8_t div_latch1 =
      iob_read(base + IOB_UART16550_CSRS_DL1_ADDR, IOB_UART16550_CSRS_W);
  // restore LCR value
  iob_uart16550_csrs_set_lc(lcr);
  return div_latch1;
}

void iob_uart16550_csrs_set_dl1(uint8_t value) {
  // ensure LCR bit 7 is 1
  uint8_t lcr = iob_uart16550_csrs_get_lc();
  uint8_t lcr_tmp = lcr;
  set_bit(&lcr_tmp, IOB_UART16550_LC_DL);
  iob_uart16550_csrs_set_lc(lcr_tmp);
  iob_write(base + IOB_UART16550_CSRS_DL1_ADDR, IOB_UART16550_CSRS_W, value);
  // restore LCR value
  iob_uart16550_csrs_set_lc(lcr);
}

uint8_t iob_uart16550_csrs_get_dl2() {
  // ensure LCR bit 7 is 1
  uint8_t lcr = iob_uart16550_csrs_get_lc();
  uint8_t lcr_tmp = lcr;
  set_bit(&lcr_tmp, IOB_UART16550_LC_DL);
  iob_uart16550_csrs_set_lc(lcr_tmp);
  uint8_t div_latch2 =
      iob_read(base + IOB_UART16550_CSRS_DL2_ADDR, IOB_UART16550_CSRS_W);
  // restore LCR value
  iob_uart16550_csrs_set_lc(lcr);
  return div_latch2;
}

void iob_uart16550_csrs_set_dl2(uint8_t value) {
  // ensure LCR bit 7 is 1
  uint8_t lcr = iob_uart16550_csrs_get_lc();
  uint8_t lcr_tmp = lcr;
  set_bit(&lcr_tmp, IOB_UART16550_LC_DL);
  iob_uart16550_csrs_set_lc(lcr_tmp);
  iob_write(base + IOB_UART16550_CSRS_DL2_ADDR, IOB_UART16550_CSRS_W, value);
  // restore LCR value
  iob_uart16550_csrs_set_lc(lcr);
}

uint8_t iob_uart16550_csrs_get_db1() {
  return iob_read(base + IOB_UART16550_CSRS_DB1_ADDR, IOB_UART16550_CSRS_W);
}

uint8_t iob_uart16550_csrs_get_db2() {
  return iob_read(base + IOB_UART16550_CSRS_DB2_ADDR, IOB_UART16550_CSRS_W);
}
