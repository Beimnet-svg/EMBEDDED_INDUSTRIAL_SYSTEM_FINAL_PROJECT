#include "game1.h"
#include "can1.h"
#include <util/delay.h>

extern int score;
extern int Global;
extern message_t2 Msg;

void write_score(){

    Msg = can_recieve();
    
    if(Msg.id == 6){
        Global = 0;
        }
    

}   