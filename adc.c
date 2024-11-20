#include <stdio.h>

#include "adc.h"
#include "can.h"
#include "sam.h"
#include "game.h"
extern int score; 



void adc_init(){
    printf("Starting\n\r");
    PMC->PMC_PCER1 |= PMC_PCER1_PID37;//enable clock for ADC ID=37
    
    ADC->ADC_MR |= ADC_MR_FREERUN_ON;
    ADC->ADC_CHER |= (ADC_CHER_CH0);

   
   /* ADC->ADC_CHER = (1<<6); //enable channel 6
    while(ADC->ADC_CHSR != (1<<6));
    ADC->ADC_IER = (1<<6);
    ADC->ADC_MR = (0<<0);
    ADC->ADC_MR |= (0<<8);
    */
    printf("Enabled\n\r");
    /*
    ADC->ADC_WPMR = (0x414443 << 8);
    ADC->ADC_WPMR &= ~(1<<0); //These two lines is basicly set adc to config mode
    ADC->ADC_WPMR |= (0x414443 << 8);
    ADC->ADC_WPMR |= (1<<0); //Enabling back write protect*/
    


    /*PMC->PMC_PCER1 |= (1<<5);//enable clock for ADC ID=37
    

    PIOA->PIO_OER = PIO_PA2;
    PIOA->PIO_PER = PIO_PA2;
    PIOA->PIO_PDR |= PIO_PDR_P2; //disable PIO FOR PB13
    PIOA->PIO_ABSR |= PIO_ABSR_P2; //enable Peripheral B for PB13

    ADC->ADC_CHER = (1<<7); //enable channel 7*/
   

    //Need to program prescal according to Electrical Characteristics section
    //adc SWRST in ASC_CR = reset
    


    
}

uint16_t CheckADCPort7(){
    ADC->ADC_CR |= (ADC_CR_START); 
    
    //while((ADC->ADC_ISR != (1<<6))){}

    uint16_t a = ADC->ADC_CDR[0];
    
   
    //printf("ADC value: %d\n\r", a);

    return a;
    
}

uint8_t 
isscored(){
    uint16_t a = CheckADCPort7();
    
    if(a < 1100){
        
        return 1;
    }
    return 0; 
}


void Score(){
    if(isscored()){
        printf("It scored!\n\r");
        while((CheckADCPort7()<1100)){printf("It scored!\n\r");}
        
        
        send_count();
    }

}