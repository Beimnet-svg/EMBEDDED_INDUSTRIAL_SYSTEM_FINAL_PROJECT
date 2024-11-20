#include"game.h"
#include"can.h"
#include <stdio.h>
#include "sam.h"
extern  CanMsg scoreMsg;
extern  CanMsg initMsg;
extern int score; 
extern uint8_t Startsignal;
extern CanMsg m;


void send_count(){
    scoreMsg.id = 5;
    scoreMsg.byte[0]  = 2;
    for(int L = 0; L < 10; L++){
        can_tx(scoreMsg);
        printf("sending!\n\r");
    }
}

void makeMsg(uint8_t id, uint8_t length, uint8_t word){
    scoreMsg.id = id;
    scoreMsg.length = length;
    
    scoreMsg.byte[0] = word;
    
}
void makeMsginit(uint8_t id, uint8_t length, uint8_t word){
    initMsg.id = id;
    initMsg.length = length;
    
    initMsg.byte[0] = word;
    
}

updateStart(){
    can_rx1(&m);
    if(m.id==1){
        getvalueStart(&m, &Startsignal);
    }
    
}






