#include <stdio.h>
#include "adc.h"
#include "can.h"
#include "sam.h"
#include "encoder.h"

void encoder_init(){
    PMC->PMC_PCER0 |= PMC_PCER0_PID29;
    PMC->PMC_PCER1 |= PMC_PCER1_PID33;

    PIOC->PIO_OER |= PIO_PC26 | PIO_PC25;
    PIOC->PIO_PER |= PIO_PC26 | PIO_PC25; 
    PIOC->PIO_PDR |= PIO_PDR_P26 | PIO_PDR_P25; //disable PIO FOR PC26
    PIOC->PIO_ABSR |= PIO_ABSR_P26 | PIO_ABSR_P25; //enable Peripheral B for PC26

    
    TC2 -> TC_BMR = TC_BMR_QDEN | TC_BMR_POSEN;
    TC2 -> TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_XC0 | TC_CMR_ABETRG;
    TC2 -> TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
    int c = 2; 
    c = ((TC2->TC_CHANNEL[0].TC_CCR) & TC_CCR_SWTRG);
    
}

float ReadEncoder(){

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
    
    return c;

}


