
#include "sam.h"
//#include "../../../lib/can/can.h"
#include <stdio.h>
#include "can.h"

void can_printmsg(CanMsg m){
    printf("CanMsg(id:%d, length:%d, data:{", m.id, m.length);
    if(m.length){
        printf("%d", m.byte[0]);
    }
    for(uint8_t i = 1; i < m.length; i++){
        printf(", %d", m.byte[i]);
    }
    printf("})\n");
}
uint8_t getid(CanMsg m){
    return m.id;
    
}
void getvalue(CanMsg *m, uint8_t *x, uint8_t *dir,uint8_t *y){
   
     *x=  m->byte[0];
     *dir=  m->byte[1];
     *y=  m->byte[2];
    
}
void getvaluey(CanMsg *m, uint8_t *y, uint8_t *button){
    
     *y=  m->byte[2];
     *button = m->byte[3];

    
}

void getvalueStart(CanMsg *m, uint8_t *StartValue){
    
     *StartValue = m->byte[4];

    
}

#define txMailbox 0
#define rxMailbox 1
#define rx2Mailbox 2
#define rx3Mailbox 3



  scoreMsg.id = id;
    scoreMsg.length = length;
    
    scoreMsg.byte[0] = word;
void can_init(CanInit init, uint8_t rxInterrupt){
    // Disable CAN
    CAN0->CAN_MR &= ~CAN_MR_CANEN; 
    
    // Clear status register by reading it
    __attribute__((unused)) uint32_t ul_status = CAN0->CAN_SR;     
    
    // Disable interrupts on CANH and CANL pins
    PIOA->PIO_IDR = PIO_PA8A_URXD | PIO_PA9A_UTXD;
    
    // Select CAN0 RX and TX in PIOA
    PIOA->PIO_ABSR &= ~(PIO_PA1A_CANRX0 | PIO_PA0A_CANTX0);
    
    // Disable the Parallel IO (PIO) of the Rx and Tx pins so that the peripheral controller can use them
    PIOA->PIO_PDR = PIO_PA1A_CANRX0 | PIO_PA0A_CANTX0;
    
    // Enable pull up on CANH and CANL pin
    PIOA->PIO_PUER = (PIO_PA1A_CANRX0 | PIO_PA0A_CANTX0);    
    
    // Enable Clock for CAN0 in PMC
    // DIV = 1 (can clk = MCK/2), CMD = 1 (write), PID = 2B (CAN0)
    PMC->PMC_PCR = PMC_PCR_EN | (0/*??*/ << PMC_PCR_DIV_Pos) | PMC_PCR_CMD | (ID_CAN0 << PMC_PCR_PID_Pos); 
    PMC->PMC_PCER1 |= 1 << (ID_CAN0 - 32);
    
    //Set baudrate, Phase1, phase2 and propagation delay for can bus. Must match on all nodes!
    CAN0->CAN_BR = init.reg; 
    


    // Configure mailboxes
    // transmit
    CAN0->CAN_MB[txMailbox].CAN_MID = CAN_MID_MIDE;
    CAN0->CAN_MB[txMailbox].CAN_MMR = CAN_MMR_MOT_MB_TX;
    
    // receive
    CAN0->CAN_MB[rxMailbox].CAN_MAM = 0; // Accept all messages
    CAN0->CAN_MB[rxMailbox].CAN_MID = CAN_MID_MIDE;
    CAN0->CAN_MB[rxMailbox].CAN_MMR = CAN_MMR_MOT_MB_RX;
    CAN0->CAN_MB[rxMailbox].CAN_MCR |= CAN_MCR_MTCR;
    if(rxInterrupt){
        // Enable interrupt on receive
        CAN0->CAN_IER |= (1 << rxMailbox); 
        // Enable interrupt in NVIC 
        NVIC_EnableIRQ(ID_CAN0);
    }
    //testing: 
    
    // receive
    CAN0->CAN_MB[rx2Mailbox].CAN_MAM = 0x7FF; // Accept all messages
    CAN0->CAN_MB[rx2Mailbox].CAN_MID = 1;
    CAN0->CAN_MB[rx2Mailbox].CAN_MMR = CAN_MMR_MOT_MB_RX;
    CAN0->CAN_MB[rx2Mailbox].CAN_MCR |= CAN_MCR_MTCR;
    if(rxInterrupt){
        // Enable interrupt on receive
        CAN0->CAN_IER |= (1 << rx2Mailbox); 
        // Enable interrupt in NVIC 
        NVIC_EnableIRQ(ID_CAN0);
    }
    CAN0->CAN_MB[rx3Mailbox].CAN_MAM = 0x7FF; // Accept all messages
    CAN0->CAN_MB[rx3Mailbox].CAN_MID = 2;  scoreMsg.id = id;
    scoreMsg.length = length;
    
    scoreMsg.byte[0] = word;
    CAN0->CAN_MB[rx3Mailbox].CAN_MMR = CAN_MMR_MOT_MB_RX;
    CAN0->CAN_MB[rx3Mailbox].CAN_MCR |= CAN_MCR_MTCR;
    if(rxInterrupt){
        // Enable interrupt on receive
        CAN0->CAN_IER |= (1 << rx3Mailbox); 
        // Enable interrupt in NVIC 
        NVIC_EnableIRQ(ID_CAN0);
    }

    // Enable CAN
    CAN0->CAN_MR |= CAN_MR_CANEN;
}


void can_tx(CanMsg m){
    while(!(CAN0->CAN_MB[txMailbox].CAN_MSR & CAN_MSR_MRDY)){}
    
    // Set message ID and use CAN 2.0B protocol
    CAN0->CAN_MB[txMailbox].CAN_MID = CAN_MID_MIDvA(m.id) | CAN_MID_MIDE ;
        
    // Coerce maximum 8 byte length
    m.length = m.length > 8 ? 8 : m.length;
    
    //  Put message in can data registers
    CAN0->CAN_MB[txMailbox].CAN_MDL = m.dword[0];
    CAN0->CAN_MB[txMailbox].CAN_MDH = m.dword[1];
        
    // Set message length and mailbox ready to send
    CAN0->CAN_MB[txMailbox].CAN_MCR = (m.length << CAN_MCR_MDLC_Pos) | CAN_MCR_MTCR;
}

uint8_t can_rx(CanMsg* m){
    if(!(CAN0->CAN_MB[rxMailbox].CAN_MSR & CAN_MSR_MRDY)){
        return 0;
    }

    // Get message ID
    m->id = (uint8_t)((CAN0->CAN_MB[rxMailbox].CAN_MID & CAN_MID_MIDvA_Msk) >> CAN_MID_MIDvA_Pos);
        
    // Get data length
    m->length = (uint8_t)((CAN0->CAN_MB[rxMailbox].CAN_MSR & CAN_MSR_MDLC_Msk) >> CAN_MSR_MDLC_Pos);
    
    // Get data from CAN mailbox
    m->dword[0] = CAN0->CAN_MB[rxMailbox].CAN_MDL;
    m->dword[1] = CAN0->CAN_MB[rxMailbox].CAN_MDH;
                
    // Reset for new receive
    CAN0->CAN_MB[rxMailbox].CAN_MMR = CAN_MMR_MOT_MB_RX;
    CAN0->CAN_MB[rxMailbox].CAN_MCR |= CAN_MCR_MTCR;
    return 1;
}
uint8_t can_rx1(CanMsg* m){
    if(!(CAN0->CAN_MB[rx2Mailbox].CAN_MSR & CAN_MSR_MRDY)){
        return 0;
    }

    // Get message ID
    m->id = (uint8_t)((CAN0->CAN_MB[rx2Mailbox].CAN_MID & CAN_MID_MIDvA_Msk) >> CAN_MID_MIDvA_Pos);
        
    // Get data length
    m->length = (uint8_t)((CAN0->CAN_MB[rx2Mailbox].CAN_MSR & CAN_MSR_MDLC_Msk) >> CAN_MSR_MDLC_Pos);
    
    // Get data from CAN mailbox
    m->dword[0] = CAN0->CAN_MB[rx2Mailbox].CAN_MDL;
    m->dword[1] = CAN0->CAN_MB[rx2Mailbox].CAN_MDH;
                
    // Reset for new receive
    CAN0->CAN_MB[rx2Mailbox].CAN_MMR = CAN_MMR_MOT_MB_RX;
    CAN0->CAN_MB[rx2Mailbox].CAN_MCR |= CAN_MCR_MTCR;
    return 1;
}
uint8_t can_rx2(CanMsg* m){
    if(!(CAN0->CAN_MB[rx3Mailbox].CAN_MSR & CAN_MSR_MRDY)){
        return 0;
    }

    // Get message ID
    m->id = (uint8_t)((CAN0->CAN_MB[rx3Mailbox].CAN_MID & CAN_MID_MIDvA_Msk) >> CAN_MID_MIDvA_Pos);
        
    // Get data length
    m->length = (uint8_t)((CAN0->CAN_MB[rx3Mailbox].CAN_MSR & CAN_MSR_MDLC_Msk) >> CAN_MSR_MDLC_Pos);
    
    // Get data from CAN mailbox
    m->dword[0] = CAN0->CAN_MB[rx3Mailbox].CAN_MDL;
    m->dword[1] = CAN0->CAN_MB[rx3Mailbox].CAN_MDH;
                
    // Reset for new receive
    CAN0->CAN_MB[rx3Mailbox].CAN_MMR = CAN_MMR_MOT_MB_RX;
    CAN0->CAN_MB[rx3Mailbox].CAN_MCR |= CAN_MCR_MTCR;
    return 1;
}
    
    

    
/*
// Example CAN interrupt handler
void CAN0_Handler(void){
    char can_sr = CAN0->CAN_SR; 
    
    // RX interrupt
    if(can_sr & (1 << rxMailbox)){
        // Add your message-handling code here
        can_printmsg(can_rx());
    } else {
        printf("CAN0 message arrived in non-used mailbox\n\r");
    }
    
    if(can_sr & CAN_SR_MB0){
        // Disable interrupt
        CAN0->CAN_IDR = CAN_IER_MB0;
    }
    
    NVIC_ClearPendingIRQ(ID_CAN0);
} 
*/
void can_init1(){
    CanInit init;
    init.phase2 = 5;
    init.propag = 3;
    init.phase1 = 5;
    init.sjw = 1;
    init.brp = 20; 
    init.smp = 0;
    can_init(init,0);

}



