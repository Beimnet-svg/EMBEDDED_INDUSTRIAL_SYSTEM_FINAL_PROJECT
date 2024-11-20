#include "JoyCan.h"
#include <util/delay.h>
#include "can1.h"
#include "OLED.h"

void SendJoyPos(volatile char *adc, uint8_t x_start, uint8_t y_start){
    uint8_t dir = 2;
    volatile uint8_t joyx = get_joystick_x(&dir,adc, x_start, y_start);
    volatile uint8_t joyy = get_joystick_y(adc, x_start, y_start);
    volatile uint8_t button = is_button_pressed();
    

    char joy1x[4];
    
    joy1x[0] = (char)joyx;
    
    joy1x[1] = (char)dir;
    joy1x[2] = (char)joyy;
    joy1x[3] = (char)button;
    
    //printf("%d number1\r\n",joy1x[0]);
    volatile message_t messagex = make_message(1,4,joy1x);
    
    //message_print(messagex);
    can_write(&messagex); 

}


void SendJoyPosStart(volatile char *adc, uint8_t x_start, uint8_t y_start){

    
  
    char joy1x[5];
    
    joy1x[0] = (char)0;
    joy1x[1] = (char)2;
    joy1x[2] = (char)0;
    joy1x[3] = (char)0;
    joy1x[4] = (char)1;
    //printf("%d number1\r\n",joy1x[0]);
    volatile message_t messagex = make_message(1,5,joy1x);
    
    //message_print(messagex);
    can_write(&messagex); 

}
