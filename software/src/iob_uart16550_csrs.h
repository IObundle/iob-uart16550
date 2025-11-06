/*
 * SPDX-FileCopyrightText: 2025 IObundle, Lda
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef H_IOB_UART16550_CSRS_CSRS_H
#define H_IOB_UART16550_CSRS_CSRS_H

#include <stdint.h>

// used address space width
#define IOB_UART16550_CSRS_CSRS_ADDR_W 5

// Addresses
#define IOB_UART16550_CSRS_RB_ADDR (0)   // Receiver buffer
#define IOB_UART16550_CSRS_TR_ADDR (0)   // Transmitter
#define IOB_UART16550_CSRS_IE_ADDR (1)   // Interrupt enable
#define IOB_UART16550_CSRS_II_ADDR (2)   // Interrupt identification
#define IOB_UART16550_CSRS_FC_ADDR (2)   // FIFO control
#define IOB_UART16550_CSRS_LC_ADDR (3)   // Line control
#define IOB_UART16550_CSRS_MC_ADDR (4)   // Modem control
#define IOB_UART16550_CSRS_LS_ADDR (5)   // Line status
#define IOB_UART16550_CSRS_MS_ADDR (6)   // Modem status
#define IOB_UART16550_CSRS_SR_ADDR (7)   // Scratch register
#define IOB_UART16550_CSRS_DL1_ADDR (0)  // Divisor latch bytes (1)
#define IOB_UART16550_CSRS_DL2_ADDR (1)  // Divisor latch bytes (2)
#define IOB_UART16550_CSRS_DB1_ADDR (8)  // Debug register 1
#define IOB_UART16550_CSRS_DB2_ADDR (12) // Debug register 2

// TODO: mode register bit defines
// Interrupt Enable Register bits
#define IOB_UART16550_IE_RDA (0)  // Received Data Available
#define IOB_UART16550_IE_THRE (1) // Transmitter Holding Register Empty
#define IOB_UART16550_IE_RLS (2)  // Receiver Line Status
#define IOB_UART16550_IE_MS (3)   // Modem Status

// Data widths (bit)
// all CSRS have 8bit
#define IOB_UART16550_CSRS_W 8

// Base Address
void iob_uart_csrs_init_baseaddr(uint32_t addr);

// IO read and write function prototypes
void iob_write(uint32_t addr, uint32_t data_w, uint32_t value);
uint32_t iob_read(uint32_t addr, uint32_t data_w);

// Core Setters and Getters
uint8_t iob_uart16550_csrs_get_rb();
void iob_uart16550_csrs_set_tr(uint8_t value);
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

#endif // H_IOB_UART16550_CSRS__CSRS_H
