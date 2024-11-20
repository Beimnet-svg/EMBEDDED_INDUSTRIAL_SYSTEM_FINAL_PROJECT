#include "ADC.h"
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "OLED.h"

void init_clock(){

    //DDRD |= (1<<PD5);
    DDRD |= (1<<PD4);
    //DDRB |= (1<<PB0);

    TCCR3A |= (1<<WGM30) | (1<<WGM31); //Using 8 bit fast mode based on table 56 page 
    TCCR3B |= (1<<CS30) | (1<<WGM32) | (1<<WGM33); //also 8 bit fast mode but also table 57 so clk /1 
    TCCR3A |= (0<<COM3A1) | (1<<COM3A0); // (1<<COM3B1) | (1<<COM3B0); //Table 55 set OCnA/OcnB when counting up and clear when counting down.

    OCR3A = 0;

}


uint8_t get_joystick_x(uint8_t *dir,volatile char *adc, uint8_t x_start, uint8_t y_start)
{
  
    
    x_start = (float) x_start;

    float value;

    uint8_t x = check_ADC(2,adc);

    x = (float) x;
   
    float bottom = x_start-155.0;
    x_start = (float) x_start;
    bottom = (float) bottom;
   
    if(x <x_start){
        value = 100 - (((x-bottom)/(x_start-bottom))*100); 
        *dir = 0; //left
    }
    else{
        value = (((x-x_start)/(255.0-x_start))*100.0);
        *dir = 1; //right

    }
    
    if(value <5){
        *dir=2;
    }
    
    if(value<5.0){value = 0.0;}
    if(value>95.0){value = 100.0;}

    value = (uint8_t) value;
    
    return value;
}


uint8_t get_joystick_y(volatile char *adc, uint8_t x_start, uint8_t y_start){

    float value;
    uint8_t y = check_ADC(3,adc);
    



    y = (float) y;
    y_start = (float) y_start;

    float bottom = y_start-155.0;
    

   



    if(y < y_start){
        value = ((y-bottom)/(y_start-bottom))*50.0; //there was *50
    }
    else if(y>y_start){
        value = 50.0 + ((y-y_start)/(255.0-y_start))*50.0;
    }
    else
    {
        value = 50.0;
    }

    if(value<5.0){value = 0.0;}
    if(value>95.0){value = 100.0;}

    value = (uint8_t) value;

    return value;
}


uint8_t get_leftslider(volatile char *adc){
    uint8_t value;
    uint8_t left = check_ADC(0,adc);
    if(left <=180){
        value = 50*(left-24)/156;
    }
    else{
        value = 50 + ((left-180)/1.5);
    }
    return value;
}

uint8_t get_rightslider(volatile char *adc){
    uint8_t value;
    uint8_t right = check_ADC(1,adc);
    if(right <=180){
        value = 50*(right-24)/156;
    }
    else{
        value = 50 + ((right-180)/1.5);
    }
    return value;
}



uint8_t check_ADC(uint8_t a, volatile char *adc){
    
    adc[0] = 0x04; 

    _delay_us(100); 

    uint8_t value_right =adc[0]; 
    uint8_t value_left =adc[0]; 
    uint8_t Joystick_y =adc[0]; 
    uint8_t Joystick_x = adc[0];

    _delay_ms(100); 

    switch(a)
        {
            case 0:
                return value_left;
                break;
            case 1:
                return value_right;
                break;
            case 2:
                return Joystick_x;
                break;
            case 3:
                return Joystick_y;
                break;
            
        }
}

uint8_t Joy_Direction(volatile char *adc, uint8_t x_start, uint8_t y_start){
    uint8_t dir;
    uint8_t x = get_joystick_x(&dir, adc, x_start, y_start);
    uint8_t y = get_joystick_y(adc, x_start, y_start);
    if((abs(x-50))>abs(y-50)){
        if(x<40){
            return 0;
        }else if(x>80){
            return 1;
        }else{
            return 4;
        }
    }else{
        if(y<40){
            return 2;
        }else if((y>80)){
            return 3;
        }else{
            return 4;
        }
    }
}


void print_Joy_dir(volatile char *adc, uint8_t x_start, uint8_t y_start){
    uint8_t dir= Joy_Direction(adc, x_start, y_start);
    switch(dir)
        {
            case 0:
                //LEFT
                printf("Left\n\r");
                break;
            case 1:
                //RIGHT
                printf("Right\n\r");
                break;
            case 2:
                //DOWN
                printf("Down\n\r");
                    
                break;
            case 3:
                //UP
                printf("Up\n\r");
                break;
            case 4:
                printf("Neutral\n\r");
                break;
        }
        
}










  /*
    uint8_t value = check_ADC(2,adc);

    value = (float) value;

    if (value > 185)
    {
      value = (50.0f/95.0f)*value - 34.2f;
    }
    else{
      value =(5.0f/16.0f)*value;
    };

    value = (uint8_t) value;


    return value;
    */