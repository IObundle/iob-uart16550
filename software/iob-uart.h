#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>

#define UART_PROGNAME "IOb-UART"

//UART commands
#define STX 2 //start text
#define ETX 3 //end text
#define EOT 4 //end of transission
#define ENQ 5 //enquiry
#define ACK 6 //acklowledge
#define FTX 7 //transmit file
#define FRX 8 //receive file

static int base;

//UART functions

//Reset UART and set the division factor
void uart_init(int base_address, uint16_t div);

//Close transmission
void uart_finish();

//TX FUNCTIONS
//Check if tx is ready
char uart_txready();
//Wait for tx to be ready
void uart_txwait();

//RX FUNCTIONS
//Check if rx is ready
char uart_rxready();
//Wait for rx to be ready
void uart_rxwait();


//Print char
void uart_putc(char c);

//Print string
void uart_puts(const char *s);

//Send file
void uart_sendfile(char* file_name, int file_size, char *mem);

//Get char
char uart_getc();

//Receive file
int uart_recvfile(char* file_name, char *mem);
