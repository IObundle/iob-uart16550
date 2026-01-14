/*
 * SPDX-FileCopyrightText: 2025 IObundle, Lda
 *
 * SPDX-License-Identifier: MIT
 */

/** @file iob_uart16550_csrs.h
 *  @brief UART16550 Control and Status Register access functions
 *
 * The present IOb-UART16550 software drivers map the Control and Status
 * Registers for direct core access.
 *
 */

#ifndef H_IOB_UART16550_CSRS_CSRS_H
#define H_IOB_UART16550_CSRS_CSRS_H

#include <stdint.h>

// used address space width
/**
 * @def IOB_UART16550_CSRS_CSRS_ADDR_W
 * @brief Address width required to access all CSRs in UART16550.
 */
#define IOB_UART16550_CSRS_CSRS_ADDR_W 5

// Addresses
/**
 * @def IOB_UART16550_CSRS_RB_ADDR
 * @brief Receiver buffer.
 */
/**
 * @def IOB_UART16550_CSRS_TR_ADDR
 * @brief Transmitter.
 */
/**
 * @def IOB_UART16550_CSRS_IE_ADDR
 * @brief Interrupt enable.
 */
/**
 * @def IOB_UART16550_CSRS_II_ADDR
 * @brief Interrupt identification.
 */
/**
 * @def IOB_UART16550_CSRS_FC_ADDR
 * @brief FIFO control.
 */
/**
 * @def IOB_UART16550_CSRS_LC_ADDR
 * @brief Line control.
 */
/**
 * @def IOB_UART16550_CSRS_MC_ADDR
 * @brief Modem control.
 */
/**
 * @def IOB_UART16550_CSRS_LS_ADDR
 * @brief Scratch register.
 */
/**
 * @def IOB_UART16550_CSRS_MS_ADDR
 * @brief Modem status.
 */
/**
 * @def IOB_UART16550_CSRS_SR_ADDR
 * @brief Scratch register.
 */
/**
 * @def IOB_UART16550_CSRS_DL1_ADDR
 * @brief Divisor latch bytes (1).
 */
/**
 * @def IOB_UART16550_CSRS_DL2_ADDR
 * @brief Divisor latch bytes (2).
 */
/**
 * @def IOB_UART16550_CSRS_DB1_ADDR
 * @brief Debug register 1.
 */
/**
 * @def IOB_UART16550_CSRS_DB2_ADDR
 * @brief Debug register 2.
 */
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

// Interrupt Enable Register bits
/**
 * @def IOB_UART16550_IE_RDA
 * @brief Received Data Available.
 */
/**
 * @def IOB_UART16550_IE_THRE
 * @brief Transmitter Holding Register Empty.
 */
/**
 * @def IOB_UART16550_IE_RLS
 * @brief Receiver Line Status.
 */
/**
 * @def IOB_UART16550_IE_MS
 * @brief Modem Status.
 */
#define IOB_UART16550_IE_RDA (0)  // Received Data Available
#define IOB_UART16550_IE_THRE (1) // Transmitter Holding Register Empty
#define IOB_UART16550_IE_RLS (2)  // Receiver Line Status
#define IOB_UART16550_IE_MS (3)   // Modem Status

// Interrupt Identification
/**
 * @def IOB_UART16550_II_PND
 * @brief Pending Interrupt.
 */
#define IOB_UART16550_II_PND (0) // Pending Interrupt
// ID Codes bits [3:1]
/**
 * @def IOB_UART16550_II_RLS
 * @brief Receiver Line Status.
 */
/**
 * @def IOB_UART16550_II_RDA
 * @brief Receiver Data Available.
 */
/**
 * @def IOB_UART16550_II_TI
 * @brief Timeout Indication.
 */
/**
 * @def IOB_UART16550_II_THRE
 * @brief Transmitter Holding Register empty.
 */
/**
 * @def IOB_UART16550_II_MS
 * @brief Modem Status.
 */
#define IOB_UART16550_II_RLS (0b011)  // Receiver Line Status
#define IOB_UART16550_II_RDA (0b010)  // Receiver Data Available
#define IOB_UART16550_II_TI (0b110)   // Timeout Indication
#define IOB_UART16550_II_THRE (0b001) // Transmitter Holding Register empty
#define IOB_UART16550_II_MS (0b000)   // Modem Status

// FIFO Control Register
/**
 * @def IOB_UART16550_FC_RF
 * @brief Clear Receive FIFO.
 */
/**
 * @def IOB_UART16550_FC_TF
 * @brief Clear Transmitter FIFO.
 */
/**
 * @def IOB_UART16550_FC_TL
 * @brief Receiver FIFO Trigger Level address.
 */
/**
 * @def IOB_UART16550_FC_TL_1
 * @brief Receiver FIFO Trigger Level: 1 byte.
 */
/**
 * @def IOB_UART16550_FC_TL_4
 * @brief Receiver FIFO Trigger Level: 4 byte.
 */
/**
 * @def IOB_UART16550_FC_TL_8
 * @brief Receiver FIFO Trigger Level: 8 byte.
 */
/**
 * @def IOB_UART16550_FC_TL_14
 * @brief Receiver FIFO Trigger Level: 14 byte.
 */
#define IOB_UART16550_FC_RF (1)       // Clear Receive FIFO
#define IOB_UART16550_FC_TF (2)       // Clear Transmitter FIFO
#define IOB_UART16550_FC_TL (6)       // Receiver FIFO Trigger Level address
#define IOB_UART16550_FC_TL_1 (0b00)  // Receiver FIFO Trigger Level: 1 byte
#define IOB_UART16550_FC_TL_4 (0b01)  // Receiver FIFO Trigger Level: 4 byte
#define IOB_UART16550_FC_TL_8 (0b10)  // Receiver FIFO Trigger Level: 8 byte
#define IOB_UART16550_FC_TL_14 (0b11) // Receiver FIFO Trigger Level: 14 byte

// Line Control Register
/**
 * @def IOB_UART16550_LC_BITS
 * @brief Bits per character (0:1).
 */
/**
 * @def IOB_UART16550_LC_SB
 * @brief Stop bits.
 */
/**
 * @def IOB_UART16550_LC_PE
 * @brief Parity enable.
 */
/**
 * @def IOB_UART16550_LC_EP
 * @brief Even parity.
 */
/**
 * @def IOB_UART16550_LC_SP
 * @brief Stick parity.
 */
/**
 * @def IOB_UART16550_LC_BC
 * @brief Break control.
 */
/**
 * @def IOB_UART16550_LC_DL
 * @brief Divisor latch access.
 */
#define IOB_UART16550_LC_BITS (0) // Bits per character (0:1)
#define IOB_UART16550_LC_SB (2)   // Stop bits
#define IOB_UART16550_LC_PE (3)   // Parity enable
#define IOB_UART16550_LC_EP (4)   // Even parity
#define IOB_UART16550_LC_SP (5)   // Stick parity
#define IOB_UART16550_LC_BC (6)   // Break control
#define IOB_UART16550_LC_DL (7)   // Divisor latch access

// Modem Control Register
/**
 * @def IOB_UART16550_MC_DTR
 * @brief Data Terminal Ready.
 */
/**
 * @def IOB_UART16550_MC_RTS
 * @brief Request To Send.
 */
/**
 * @def IOB_UART16550_MC_OUT1
 * @brief Loopback -> Ring Indicator.
 */
/**
 * @def IOB_UART16550_MC_OUT2
 * @brief Loopback -> Data Carrier Detect.
 */
/**
 * @def IOB_UART16550_MC_LB
 * @brief LoopBack mode.
 */
#define IOB_UART16550_MC_DTR (0)  // Data Terminal Ready
#define IOB_UART16550_MC_RTS (1)  // Request To Send
#define IOB_UART16550_MC_OUT1 (2) // Loopback -> Ring Indicator
#define IOB_UART16550_MC_OUT2 (3) // Loopback -> Data Carrier Detect
#define IOB_UART16550_MC_LB (4)   // LoopBack mode

// Line Status Register
/**
 * @def IOB_UART16550_LS_DR
 * @brief Data Ready.
 */
/**
 * @def IOB_UART16550_LS_OE
 * @brief Overrun Error.
 */
/**
 * @def IOB_UART16550_LS_PE
 * @brief Parity Error.
 */
/**
 * @def IOB_UART16550_LS_FE
 * @brief Framing Error.
 */
/**
 * @def IOB_UART16550_LS_BI
 * @brief Break Interrupt.
 */
/**
 * @def IOB_UART16550_LS_TFE
 * @brief Transmit FIFO empty.
 */
/**
 * @def IOB_UART16550_LS_TE
 * @brief Transmitter Empty Indicator.
 */
/**
 * @def IOB_UART16550_LS_EI
 * @brief Error indicator.
 */
#define IOB_UART16550_LS_DR (0)  // Data Ready
#define IOB_UART16550_LS_OE (1)  // Overrun Error
#define IOB_UART16550_LS_PE (2)  // Parity Error
#define IOB_UART16550_LS_FE (3)  // Framing Error
#define IOB_UART16550_LS_BI (4)  // Break Interrupt
#define IOB_UART16550_LS_TFE (5) // Transmit FIFO empty
#define IOB_UART16550_LS_TE (6)  // Transmitter Empty Indicator
#define IOB_UART16550_LS_EI (7)  // Error indicator

// Modem Status Register
/**
 * @def IOB_UART16550_MS_DCTS
 * @brief Delta Clear To Send.
 */
/**
 * @def IOB_UART16550_MS_DDSR
 * @brief Delta Data Set Ready.
 */
/**
 * @def IOB_UART16550_MS_TERI
 * @brief Trailing Edge of Ring Indicator.
 */
/**
 * @def IOB_UART16550_MS_DDCD
 * @brief Delta Data Carrier Detect.
 */
/**
 * @def IOB_UART16550_MS_CCTS
 * @brief Complement Clear To Send.
 */
/**
 * @def IOB_UART16550_MS_CDSR
 * @brief Complement Data Set Ready.
 */
/**
 * @def IOB_UART16550_MS_CRI
 * @brief Complement Ring Indicator.
 */
/**
 * @def IOB_UART16550_MS_CDCD
 * @brief Complement Data Carrier Detect.
 */
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
/**
 * @def IOB_UART16550_CSRS_W
 * @brief CSR Data Width.
 */
#define IOB_UART16550_CSRS_W 8

// Base Address
/**
 * @brief Set core base address.
 *
 * This function sets the base address for the core in the system. All other
 * accesses are offset from this base address.
 *
 * @param addr Base address for core.
 */
void iob_uart16550_csrs_init_baseaddr(uint32_t addr);

// IO read and write function prototypes
/**
 * @brief Write access function prototype.
 *
 * @param addr Address to write to.
 * @param data_w Data width in bits.
 * @param value Value to write.
 */
void iob_write(uint32_t addr, uint32_t data_w, uint32_t value);
/**
 * @brief Read access function prototype.
 *
 * @param addr Address to write to.
 * @param data_w Data width in bits.
 * @return uint32_t Read data value.
 */
uint32_t iob_read(uint32_t addr, uint32_t data_w);

// Core Setters and Getters
/**
 * @brief Get receiver buffer.
 * Receiver buffer
 * @return uint8_t receiver buffer value.
 */
uint8_t iob_uart16550_csrs_get_rb();
/**
 * @brief Write to transmitter buffer.
 * Transmitter buffer
 * @param value to write to tx buffer.
 */
void iob_uart16550_csrs_set_tr(uint8_t value);
/**
 * @brief Get interrupt enable.
 * Interrupt enable
 * @return uint8_t interrupt enable value.
 */
uint8_t iob_uart16550_csrs_get_ie();
/**
 * @brief Set interrupt enable.
 * Interrupt enable
 * @param value for interrupt enable.
 */
void iob_uart16550_csrs_set_ie(uint8_t value);
/**
 * @brief Get interrupt identification.
 * Interrupt indentification
 * @return uint8_t interrupt identification.
 */
uint8_t iob_uart16550_csrs_get_ii();
/**
 * @brief Set FIFO control.
 * FIFO control
 * @param value FIFO control.
 */
void iob_uart16550_csrs_set_fc(uint8_t value);
/**
 * @brief Get Line control.
 * Line control
 * @return uint8_t current Line control.
 */
uint8_t iob_uart16550_csrs_get_lc();
/**
 * @brief Set Line control.
 * Line control
 * @param value for Line control.
 */
void iob_uart16550_csrs_set_lc(uint8_t value);
/**
 * @brief Set Modem control.
 * Modem control
 * @param value for Modem control.
 */
void iob_uart16550_csrs_set_mc(uint8_t value);
/**
 * @brief Get Scratch register.
 * Scratch register
 * @return uint8_t Scratch register.
 */
uint8_t iob_uart16550_csrs_get_ls();
/**
 * @brief Get Modem status.
 * Modem status
 * @return uint8_t Modem status.
 */
uint8_t iob_uart16550_csrs_get_ms();
/**
 * @brief Get Divisor latch bytes (1).
 * Divisor latch bytes
 * @return uint8_t Divisor latch bytes (1).
 */
uint8_t iob_uart16550_csrs_get_dl1();
/**
 * @brief Set Divisor latch bytes (1).
 * Divisor latch bytes (1)
 * @param value for Divisor latch bytes (1).
 */
void iob_uart16550_csrs_set_dl1(uint8_t value);
/**
 * @brief Get Divisor latch bytes (2).
 * Divisor latch bytes
 * @return uint8_t Divisor latch bytes (2).
 */
uint8_t iob_uart16550_csrs_get_dl2();
/**
 * @brief Set Divisor latch bytes (2).
 * Divisor latch bytes (2)
 * @param value for Divisor latch bytes (2).
 */
void iob_uart16550_csrs_set_dl2(uint8_t value);
/**
 * @brief Get Debug register 1.
 * Debug register
 * @return uint8_t Debug register 1.
 */
uint8_t iob_uart16550_csrs_get_db1();
/**
 * @brief Get Debug register 2.
 * Debug register
 * @return uint8_t Debug register 2.
 */
uint8_t iob_uart16550_csrs_get_db2();

#endif // H_IOB_UART16550_CSRS__CSRS_H
