#include "SPI.h"
#include "mcp.h"
#include <util/delay.h>
#include "can1.h"



void SPI_MasterInit(void)
{
//Set MOSI and SCK output, all others input
//DDR_SPI = (1<<DD_MOSI) | (1<<DD_SCK);
DDRB = (1<<PB4);
DDRB |= (1<<PB5) | (1<<PB7);


//Enable SPI, Master, set clock rate fck/16
SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0) |(1<<SPIE);
SPI_set_SS();

}

void SPI_MasterTransmit(char cData) //MOSI
{
    //Start transmission
    SPDR = cData;
    //Wait for transmission complete
    while(!(SPSR & (1<<SPIF))){ //SPIF is the register that is set when data transfer is completed
        
    }
}


// SPI-mode is initially set to mode 0; 

char SPI_Receive(void)
{   
    SPDR = 0xFF;

    //Wait for reception complete 
    while(!(SPSR & (1<<SPIF))){

    }
    //return data register
    return SPDR;
}

void SPI_set_SS(){
    
    PORTB |= (1<<PB4);
}

void SPI_clear_SS(){
    PORTB &= ~(1<<PB4);
}

void mcp_init(){
    SPI_MasterInit();
    mcp_reset();

    _delay_ms(10);

    uint8_t value = mcp_read(MCP_CANSTAT);
    if((value & MODE_MASK) != MODE_CONFIG){
        printf("CANSTAT ERROR: %x \n\r", value);
        printf("\n\r");
        printf("\n\r");
    }
    else
        printf("It is in the config mode\n\r");
}


void mcp_reset() {
    SPI_clear_SS();
    SPI_MasterTransmit(MCP_RESET);
    SPI_set_SS();

}

uint8_t mcp_read(uint8_t address){
    SPI_clear_SS();
    SPI_MasterTransmit(MCP_READ);
    SPI_MasterTransmit(address);
    uint8_t data = SPI_Receive();
    SPI_set_SS();

    return data;
}
void mcp_write(uint8_t address, uint8_t data){
    SPI_clear_SS();
    SPI_MasterTransmit(MCP_WRITE);
    SPI_MasterTransmit(address);
    SPI_MasterTransmit(data);
    SPI_set_SS();
}

char mcp_read_status()
{
    SPI_clear_SS();
    SPI_MasterTransmit(MCP_READ_STATUS);
    char data = SPI_Receive();
    SPI_set_SS();
    return data;
    
}

void mcp_bit_modify(uint8_t address, uint8_t mask, uint8_t data){
    SPI_clear_SS();

    SPI_MasterTransmit(MCP_BITMOD);
    SPI_MasterTransmit(address);
    SPI_MasterTransmit(mask); //f.eks 0b00011000 this means that we want to modefy the bits with 1. 
    SPI_MasterTransmit(data); 
    SPI_set_SS();
}

void mcp_request_to_send(int buffer_number)
{
SPI_clear_SS();
buffer_number = buffer_number %3;
char data = MCP_RTS_TX0;
if (buffer_number ==0){
    data = MCP_RTS_TX0;
}else if(buffer_number ==1){
    data  = MCP_RTS_TX1;
}else{ 
    data = MCP_RTS_TX2;
}
SPI_MasterTransmit(data);
SPI_set_SS();

}
void mcp_set_mode(uint8_t mode){
    mcp_bit_modify(MCP_CANCTRL, MODE_MASK, mode); 
}

void set_cnf_reg()
{
    uint8_t cnf1 = 0;

    cnf1 |= (1<<6); //sync Jump Width length bits = 2 * TQ
    cnf1 |= (0<<0); //Set BRP(Baud Rate Prescalar) 125kb/s

    uint8_t cnf2 = 0;
    cnf2 |= (1 << 7); //Set BTLMODE (PSP, Phase Segment 2 length)
    cnf2 |= (0 << 6); //Set SAM(Sample Point)
    cnf2 |= (7 << 3); //Set PHSEG1(Phase Segment 1)
    cnf2 |= (4 << 0); //SET ProPag to 3

    uint8_t cnf3 = 0;
    cnf3 |= (4 << 0); //Set PHSEG2(Phase Segment 2 length)

    mcp_write(CNF1, 0b01000001);
    mcp_write(CNF2, 0b10111101); 
    mcp_write(CNF3, 0b00000001);
    
}

    



  