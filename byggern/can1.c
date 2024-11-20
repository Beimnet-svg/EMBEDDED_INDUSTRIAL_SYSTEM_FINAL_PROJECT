#include "SPI.h"
#include "mcp.h"
#include "can1.h"



void can_write(message_ptr message){
    mcp_write(MCP_TXB0SIDH, message->id /8);
    mcp_write(MCP_TXB0SIDL, (message->id %8) <<5);

    mcp_write(MCP_TXB0DLC, message->length);
    for(int i = 0; i<(message->length);i++){
         mcp_write(MCP_TXB0D0 +i,message->data[i]);
    }
    
    mcp_request_to_send(0);
   
}

message_t2 can_recieve(){
    message_t2 message = {};
    uint8_t id_low = mcp_read(MCP_RXB0SIDL)/0b100000; 
    uint8_t id_high = mcp_read(MCP_RXB0SIDH);
    message.id = id_high * 0b1000 + id_low;

    message.length = mcp_read(MCP_RXB0DLC);
    for(int i = 0; i < message.length;i++){
        message.data[i] = mcp_read(MCP_RXB0D0+i);
    }
    printf("Message id: %d\n\r", message.id);
    return message;
}

void message_print(message_t message){
    printf("Message id: %d\n\r", message.id);
    printf("Message length: %d\n\r", message.length);
    printf("Data: %d", message.data[0]);
    printf("\n\n\r\r");
}

message_t make_message(unsigned int id, int length, char data[]){
    message_t message= {};
    message.id= id;
    message.length = length;
    for(int j=0;j<message.length;j++){
        message.data[j] = data[j];
    }
    return message;
}