#include "servoandPWM.h"
#include "sam/sam3x/include/sam.h"
#include <stdio.h>
#include <math.h>
#include "can.h"
#include "motor.h"

extern CanMsg m;
extern uint8_t x;
extern uint8_t gain;
extern uint8_t dir;
extern uint8_t y;
extern uint8_t thebutton; 
extern uint8_t flag1; 


int get_message_data(message_t message){
    char data1[message.length+1];
    for(int i = 0; i<message.length; i++){
        data1[i] = message.data[i];
    }
    data1[message.length+1]= '\0';
    return atoi(data1);
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

void message_print(message_t message){
    printf("Message id: %d\n\r", message.id);
    printf("Message length: %d\n\r", message.length);
    printf("Data: %d", message.data[0]);
    printf("\n\n\r\r");
}
void setupPWM(){
    // uint8_t prescaler = 1; 
    // double period = 20;
    // double duty = 2.1; 
    

    //uint32_t input1  =period*84000 /2;
    //uint32_t input2  =duty *84000/2;
    

    


    PMC->PMC_PCER1 = (1<<4);//enable clock for PIOB
    

    PIOB->PIO_OER = PIO_PB13;
    PIOB->PIO_PER = PIO_PB13;
    PIOB->PIO_PDR |= PIO_PDR_P13; //disable PIO FOR PB13
    PIOB->PIO_ABSR |= PIO_ABSR_P13; //enable Peripheral B for PB13
    PWM->PWM_CH_NUM[1].PWM_CMR = ((0<<PWM_CMR_CPRE_Pos) & PWM_CMR_CPRE_Msk) | PWM_CMR_CPOL;
    PWM->PWM_CH_NUM[1].PWM_CMR |= PWM_CMR_CPRE_MCK_DIV_128;


    
    PWM->PWM_CH_NUM[1].PWM_CPRD = 13125;//period; 
    PWM->PWM_CH_NUM[1].PWM_CDTY = 1378;//duty;

    PWM->PWM_ENA = (1 << 1); // enable channel 1 

    
    
    /*while  (!(PMC->PMC_PCSR0 & (1<<13)));
    printf("Power enablCanMsg m = {};

    CanMsg m2 = {};

    int i = 0;

    while (1)
    {

        can_rx1(&m);
        can_rx2(&m2);
        printf("\n\rMsgx : \n\r");
        can_printmsg(m);
        printf("\n\r");
        printf("Msgy : \n\r");
        can_printmsg(m2);
        printf("\n\r++++++++++++++++++++++++++++++++++++++++++++\n\r");
        for (int i = 0; i < 1000000; i++){}
    }ed for pwm\n\r");


    

    
    while  (!(PWM->PWM_SR));
    printf("finished\n\r new register value: 0x%lX\n\r", PWM->PWM_SR);
    */
}


void MakePWMToPortX(int percent){

    if(percent >100){percent = 100;}

    uint32_t minCounts  =591;
    uint32_t maxCounts = 1378;
    uint32_t dutyCycleCounts = minCounts + (percent * (maxCounts - minCounts)) / 100;
    PWM->PWM_CH_NUM[1].PWM_CDTYUPD = dutyCycleCounts;
}



void makeServoOutput(){
    
    
    
    // int i = 0;

    can_rx1(&m);
    for (int i = 0; i < 250; i++){}
    
    if(m.id==1){
        getvaluey(&m, &y, &thebutton);
        
        MakePWMToPortX(y);
        
        if(flag1){
            button();
        }
        
        MakePWMToMotor(gain, dir);
    }
    

    


}

void button(){

    
    if(thebutton ==0){
        PIOB->PIO_CODR = PIO_PB15;
    }else if(thebutton ==1){
        PIOB->PIO_SODR = PIO_PB15;
        flag1 = 0;
    }


}

 /*while (1)
    {

        
    }*/

void Startup(){
    for(int i = 0; i< 1000000; i++){
        MakePWMToMotor(80, 1);
    }
}