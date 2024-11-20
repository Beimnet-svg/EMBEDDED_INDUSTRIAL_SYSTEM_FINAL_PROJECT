#ifndef UART_H
#define UART_H

void uart_Init(unsigned int ubrr);
void uart_Transmit(unsigned char data);
unsigned char uart_Receive(void);


#endif
