/*
 * SPDX-FileCopyrightText: 2025 IObundle
 *
 * SPDX-License-Identifier: MIT
 */

/** @file iob_uart16550.h
 *  @brief High level iob_uart16550 core functions
 *
 * The present IOb-UART16550 software drivers implement a way to interface with
 * the IOb-UART16550 peripheral for serial communication.
 *
 * The present drivers provide base functionalities such as:
 *      - initialization and setup
 *      - basic control functions
 *      - single character send and receive functions
 *      - simple protocol for multi byte transfers
 *
 */

#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>

/**
 * @def UART_PROGNAME
 * @brief Prefix to IOb-UART16550 specific prints.
 */
#define UART_PROGNAME "IOb-UART"

// UART16550 commands
/**
 * @def STX
 * @brief Start text.
 * Signal start of data sequence to be printed.
 */
/**
 * @def ETX
 *
 * @brief End text.
 * Signal end of data sequence to be printed.
 */
/**
 * @def EOT
 *
 * @brief End of transmission.
 * Signal end of UART16550 connection.
 */
/**
 * @def ENQ
 *
 * @brief Enquiry.
 * Signal start of UART16550 connection.
 */
/**
 * @def ACK
 *
 * @brief Acknowledge.
 * Signal reception of incoming message.
 */
/**
 * @def FTX
 *
 * @brief File transfer.
 * Signal file transfer request.
 */
/**
 * @def FRX
 *
 * @brief File reception.
 * Signal file reception request.
 */
#define STX 2 // start text
#define ETX 3 // end text
#define EOT 4 // end of transission
#define ENQ 5 // enquiry
#define ACK 6 // acklowledge
#define FTX 7 // transmit file
#define FRX 8 // receive file

// UART16550 functions

/** @brief Initialize UART16550.
 *
 * Reset UART16550, set IOb-UART16550 base address and set the division factor.
 * The division factor is the number of clock cycles per symbol transfered.
 * RTS is asserted by default so the remote device can send data.
 *
 * For example, for a case with fclk = 100 Mhz for a baudrate of 115200 we
 * should have `div=(100*10^6/115200) = (868)`.
 *
 * @param base_address IOb-UART16550 instance base address in the system.
 * @param div Equal to round (fclk/baudrate).
 * @return void.
 */
void uart16550_init(int base_address, uint16_t div);

/** @brief Change UART16550 base
 *
 * Set a new IOb-UART16550 base address.
 *
 * @return Previous base
 */
int uart16550_base(int base_address);

/** @brief Close transmission.
 *
 * Send end of transmission (EOT) command via UART16550.
 * Active wait until TX transfer is complete.
 * Use this function to close console program.
 *
 * @return void.
 */
void uart16550_finish();

/** @brief Check if TX is ready
 *
 * Check if UART16550 has data to send
 *
 * @return TX ready flag
 */
char uart16550_txready();

/** @brief Wait for TX.
 *
 * Active wait until TX is ready to process new byte to send.
 *
 * @return void.
 */
void uart16550_txwait();

/** @brief Check if RX is ready
 *
 * Check if UART16550 has received data
 *
 * @return RX ready flag
 */
char uart16550_rxready();

/** @brief Wait for RX Data.
 *
 * Active wait for RX incoming data.
 *
 * @return void.
 */
void uart16550_rxwait();

/** @brief Print char.
 *
 * Send character via UART16550 to be printed by in console program.
 *
 * @param c Character to print.
 * @return void.
 */
void uart16550_putc(char c);

/** @brief Print string.
 *
 * Send string via UART16550 to be printed by in console program.
 *
 * @param s Pointer to char array to be printed.
 * @return void.
 */
void uart16550_puts(const char *s);

/** @brief Send file.
 *
 * Send variable size file via UART16550.
 * Order of commands:
 *  1. Send file transmit (FTX) command.
 *  2. Send file_name.
 *  3. Send file_size (in little endian format).
 *  4. Send file.
 *
 * @param file_name Pointer to file name string.
 * @param file_size Size of file to be sent.
 * @param mem Pointer to file.
 * @return void.
 */
void uart16550_sendfile(char *file_name, int file_size, char *mem);

/** @brief Get char.
 *
 * Active wait and receive char/byte from UART16550.
 *
 * @return received byte from UART16550.
 */
char uart16550_getc();

// FLOW CONTROL / MODEM FUNCTIONS

/** @brief Assert or de-assert RTS (Request To Send).
 *
 * When asserted (1), signals the remote device that we are ready to receive
 * data. RTS is asserted by default after uart16550_init().
 *
 * @param assert 1 to assert, 0 to de-assert.
 * @return void.
 */
void uart16550_set_rts(int assert);

/** @brief Read CTS (Clear To Send) signal status.
 *
 * Reads the modem status register and returns the complement of the CTS
 * input. Returns 1 when the remote device is ready to receive data.
 *
 * @return 1 if CTS is asserted, 0 otherwise.
 */
int uart16550_get_cts();

/** @brief Read full Modem Status Register.
 *
 * Returns the raw 8-bit MSR value.
 *
 * @return MSR register value.
 */
uint8_t uart16550_get_msr();

/** @brief Enable hardware flow control.
 *
 * When enabled, uart16550_putc() will wait for CTS to be asserted before
 * transmitting each byte. RTS is also asserted automatically.
 */
void uart16550_flow_ctrl_enable();

/** @brief Disable hardware flow control.
 *
 * uart16550_putc() reverts to original behavior (no CTS check).
 * Does not de-assert RTS.
 */
void uart16550_flow_ctrl_disable();

/** @brief Receive file.
 *
 * Request variable size file via UART16550.
 * Order of commands:
 *  1. Send file receive (FRX) command.
 *  2. Send file_name.
 *  3. Receive file_size (in little endian format).
 *  4. Send ACK command.
 *  5. Receive file.
 *
 * If memory pointer is not initialized, allocates memory for incoming file.
 *
 * @param file_name Pointer to file name string.
 * @param mem Pointer in memory to store incoming file.
 * @return Size of received file.
 */
int uart16550_recvfile(char *file_name, char *mem);
