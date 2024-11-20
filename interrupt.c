#include "sam.h"
#include "interrupt.h"
#include "can.h"

extern CanMsg m;
extern uint8_t gain;
extern uint8_t dir;
extern uint8_t x;
extern uint8_t y;
extern float ref;
extern float Kp;
extern float Ki;
extern float integral;
extern int Start_flag;


void interrupt_init_PID(uint32_t timer_ms){
    PMC->PMC_PCER0 |= (1<<ID_TC0);
 
    /*NVIC->ISER[0] |= (0<<1);
    NVIC->IABR[0] |= (0<<1);
    NVIC_EnableIRQ(27);*/

    //Select prescalar and waveform mode
    TC0->TC_CHANNEL[0].TC_CMR |= TC_CMR_TCCLKS_TIMER_CLOCK4 | TC_CMR_WAVSEL_UP_RC;

    //Calculate the RC compare value
    uint32_t rc_value = (84000000 / 128) * timer_ms /1000;
    TC0 -> TC_CHANNEL[0].TC_RC = rc_value;

    //Enable the interrupt
    TC0->TC_CHANNEL[0].TC_IER = TC_IER_CPCS; //Enables RC compare Interrupt. 
    NVIC_EnableIRQ(TC0_IRQn);  //Enable TC0 interrupt in NVIC

    //Start the timer
    TC0-> TC_CHANNEL[0].TC_CCR = TC_CCR_SWTRG | TC_CCR_CLKEN;
}

//Timer interrupt handler(for TC0 cHANNEL 0)
void TC0_Handler(void)
{
    //Check if the interrupt is for RC compare match
    if((TC0 -> TC_CHANNEL[0].TC_SR & TC_SR_CPCS)!=0)
    {   
        float U = 0.0f;
        getvalue(&m,&x,&dir,&y);

        //Reading encoder:
    uint16_t a = 0; 

    a = TC2->TC_CHANNEL[0].TC_CV;
    a -= 65534;
    float b  = 0.0f; 


    float c = (float) a;

    b = (c/2816.0f)*100.0f;
    if(b >100.0f){
        c = 100.0f;
    }else{
        c = b;
    }
        //done reading encoder

        
        updateref();

        float T = 0.05f;

        
        float error = ref - c;

        
        
        
        int flag = 0; 
        if(error > 2.0f){
            dir = 0;
            
        }else if(error < -2.0f){
            dir = 1;
        }else{
            gain = 0; 
            flag = 1;
        }
        
        

        if((!flag) & Start_flag){

            integral += error * T;
            
            if(integral >40.0f){
                integral = 10.0f; 
            }
            if(integral <-40.0f){
                integral = -10.0f; 
            }
            

            U = (Kp * error) + (Ki * integral);

            U = abs(U);

            
            gain = (uint8_t) U;
            
            

        }
        


       
        
    }
}

void updateref(){
    float r = (float) x;
    if(dir == 1 ){ 
        ref += r*0.05;
    }else if(dir ==0){
        ref -=r*0.05;
    }
    if (ref > 100){ref = 100;}
    if (ref < 0){ref = 0;}

}