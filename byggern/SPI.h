#include <stdlib.h>
#include <avr/io.h>
#include <stdio.h>



// Send-buffer 0
#define MCP_TXB0SIDH 0x31 // aka 0b0011 0001

//Send-buffer 1
#define MCP_TXB1SIDH 0x41

//Send-buffer 2
#define MCP_TXB2SIDH 0x51

// Motta-buffer 0
#define MCP_RXB0SIDH 0x61

// Motta-buffer 1
#define MCP_RXB1SIDH 0x71

//Send-buffer 0.1
#define MCP_TXB0SIDL 0x32
//Send-buffer 1.1
#define MCP_TXB1SIDL 0x42
//Send-buffer 2.1
#define MCP_TXB2SIDL 0x52
//transmit buffer 0.1 length
#define MCP_TXB0DLC 0x35
//transmit buffer 0.1 length
#define MCP_TXB1DLC 0x45
//transmit buffer 0.1 length
#define MCP_TXB2DLC 0x55
//Transmit databyte 0.1
#define MCP_TXB0D0 0x36
//Transmit databyte 1.1
#define MCP_TXB1D0 0x46
//Transmit databyte 2.1
#define MCP_TXB2D0 0x56
//resieve buffer 0.2
#define MCP_RXB0SIDL 0x62
//resieve buffer 0.2
#define MCP_RXB1SIDL 0x72
//Length resieve 
#define MCP_RXB0DLC 0x65
//Data resieve
#define MCP_RXB0D0 0x66


void SPI_MasterTransmit(char cData);
void SPI_MasterInit(void);
char SPI_SlaveReceive(void);
void SPI_set_SS();
void SPI_clear_SS();
void mcp_init();
void mcp_reset();
uint8_t mcp_read(uint8_t andress);
void mcp_write(uint8_t address, uint8_t data);
char mcp_read_status();
void mcp_bit_modify(uint8_t adress, uint8_t mask, uint8_t data);
void mcp_request_to_send(int buffer_number);
void mcp_set_mode(uint8_t mode);

void set_cnf_reg();