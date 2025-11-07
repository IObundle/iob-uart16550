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

// Interrupt Identification codes bits [3:1]
#define IOB_UART16550_II_RLS (0b011)  // Receiver Line Status
#define IOB_UART16550_II_RDA (0b010)  // Receiver Data Available
#define IOB_UART16550_II_TI (0b110)   // Timeout Indication
#define IOB_UART16550_II_THRE (0b001) // Transmitter Holding Register empty
#define IOB_UART16550_II_MS (0b000)   // Modem Status

// FIFO Control Register
#define IOB_UART16550_FC_RF (1)       // Clear Receive FIFO
#define IOB_UART16550_FC_TF (2)       // Clear Transmitter FIFO
#define IOB_UART16550_FC_TL (6)       // Receiver FIFO Trigger Level address
#define IOB_UART16550_FC_TL_1 (0b00)  // Receiver FIFO Trigger Level: 1 byte
#define IOB_UART16550_FC_TL_4 (0b01)  // Receiver FIFO Trigger Level: 4 byte
#define IOB_UART16550_FC_TL_8 (0b10)  // Receiver FIFO Trigger Level: 8 byte
#define IOB_UART16550_FC_TL_14 (0b11) // Receiver FIFO Trigger Level: 14 byte

// Line Control Register
#define IOB_UART16550_LC_BITS (0) // Bits per character (0:1)
#define IOB_UART16550_LC_SB (2)   // Stop bits
#define IOB_UART16550_LC_PE (3)   // Parity enable
#define IOB_UART16550_LC_EP (4)   // Even parity
#define IOB_UART16550_LC_SP (5)   // Stick parity
#define IOB_UART16550_LC_BC (6)   // Break control
#define IOB_UART16550_LC_DL (7)   // Divisor latch access

// Modem Control Register
#define IOB_UART16550_MC_DTR (0)  // Data Terminal Ready
#define IOB_UART16550_MC_RTS (1)  // Request To Send
#define IOB_UART16550_MC_OUT1 (2) // Loopback -> Ring Indicator
#define IOB_UART16550_MC_OUT2 (3) // Loopback -> Data Carrier Detect
#define IOB_UART16550_MC_LB (4)   // LoopBack mode

// Line Status Register
#define IOB_UART16550_LS_DR (0)  // Data Ready
#define IOB_UART16550_LS_OE (1)  // Overrun Error
#define IOB_UART16550_LS_PE (2)  // Parity Error
#define IOB_UART16550_LS_FE (3)  // Framing Error
#define IOB_UART16550_LS_BI (4)  // Break Interrupt
#define IOB_UART16550_LS_TFE (5) // Transmit FIFO empty
#define IOB_UART16550_LS_TE (6)  // Transmitter Empty Indicator
#define IOB_UART16550_LS_EI (7)  // Error indicator

// Modem Status Register
#define IOB_UART16550_MS_DCTS (0) // Delta Clear To Send
#define IOB_UART16550_MS_DDSR (1) // Delta Data Set Ready
#define IOB_UART16550_MS_TERI (2) // Trailing Edge of Ring Indicator
#define IOB_UART16550_MS_DDCD (3) // Delta Data Carrier Detect
#define IOB_UART16550_MS_CCTS (4) // Complement signals
#define IOB_UART16550_MS_CDSR (5)
#define IOB_UART16550_MS_CRI (6)
#define IOB_UART16550_MS_CDCD (7)

// Data widths (bit)
// all CSRS have 8bit
#define IOB_UART16550_CSRS_W 8

// Base Address
void iob_uart16550_csrs_init_baseaddr(uint32_t addr);

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
