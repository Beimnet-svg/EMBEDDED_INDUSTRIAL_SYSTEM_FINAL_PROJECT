#include "motor.h"
#include "sam.h"
#include <stdio.h>

void motor_init(){
    
    //aplly a PWM signal to the phase/dir pin (PB12)
    PMC->PMC_PCER1 |= (1<<4);//enable clock for PIOB
    PIOC->PIO_OER |= PIO_PC23;
    PIOC->PIO_PER |= PIO_PC23;
    PIOB->PIO_OER |= PIO_PB15;
    PIOB->PIO_PER |= PIO_PB15;
    


    

    PIOB->PIO_OER |= PIO_PB12;
    PIOB->PIO_PER |= PIO_PB12;
    PIOB->PIO_PDR |= PIO_PDR_P12; //disable PIO FOR PB13
    PIOB->PIO_ABSR |= PIO_ABSR_P12; //enable Peripheral B for PB13
    PWM->PWM_CH_NUM[0].PWM_CMR = ((0<<PWM_CMR_CPRE_Pos) & PWM_CMR_CPRE_Msk) | PWM_CMR_CPOL;
    PWM->PWM_CH_NUM[0].PWM_CMR |= PWM_CMR_CPRE_MCK_DIV_128;


    uint32_t period = 3200; 
    uint32_t duty = 0;
    PWM->PWM_CH_NUM[0].PWM_CPRD = period;//period; about 25KHz with 50% duty
    PWM->PWM_CH_NUM[0].PWM_CDTY = duty;//duty;

    PWM->PWM_ENA = (1 << 0); // enable channel 10

    //aplly a high or low signal to the Phace / dir pin.

    
}

void MakePWMToMotor(int percent, int dir){

    if(dir ==0){
        PIOC->PIO_CODR = PIO_PC23;
    }else if(dir ==1){
        PIOC->PIO_SODR = PIO_PC23;
    }else{
        PWM->PWM_CH_NUM[0].PWM_CDTYUPD = 0;
    }

    if(percent >100){percent = 100;}
    

    uint32_t minCounts  =0;
    uint32_t maxCounts = 1600;
    uint32_t dutyCycleCounts = minCounts + (percent * (maxCounts - minCounts)) / 100;
    PWM->PWM_CH_NUM[0].PWM_CDTYUPD = dutyCycleCounts;


}



