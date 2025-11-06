/*
 * SPDX-FileCopyrightText: 2025 IObundle, Lda
 *
 * SPDX-License-Identifier: MIT
 */

#include "iob_uart16550_csrs.h"
#include <stdint.h>

// Base Address
static uint32_t base;
void iob_uart16550_csrs_init_baseaddr(uint32_t addr) { base = addr; }

// Core Setters and Getters
uint8_t iob_uart16550_csrs_get_rb(){
  // TODO: ensure LCR bit 7 is 0
  uint8_t lcr = iob_uart16550_csrs_get_lc();
  return iob_read(base + IOB_UART16550_CSRS_RB_ADDR, IOB_UART16550_CSRS_W);
  // TODO: restore LCR value
}

void iob_uart16550_csrs_set_tr(uint8_t value){
  // TODO: ensure LCR bit 7 is 0
  iob_write(base + IOB_UART16550_CSRS_TR_ADDR, IOB_UART16550_CSRS_W, value);
  // TODO: restore LCR value
}

uint8_t iob_uart16550_csrs_get_ie();
void iob_uart16550_csrs_set_ie(uint8_t value);
uint8_t iob_uart16550_csrs_get_ii();
void iob_uart16550_csrs_set_fc(uint8_t value);
uint8_t iob_uart16550_csrs_get_lc();
void iob_uart16550_csrs_set_lc(uint8_t value);
void iob_uart16550_csrs_set_mc(uint8_t value);
uint8_t iob_uart16550_csrs_get_ls();
uint8_t iob_uart16550_csrs_get_ms();
uint8_t iob_uart16550_csrs_get_dl1();
void iob_uart16550_csrs_set_dl1(uint8_t value);
uint8_t iob_uart16550_csrs_get_dl2();
void iob_uart16550_csrs_set_dl2(uint8_t value);
uint8_t iob_uart16550_csrs_get_db1();
uint8_t iob_uart16550_csrs_get_db2();
