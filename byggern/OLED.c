#include "OLED.h"
#include "ADC.h"
#include "font.h"
#include <util/delay.h>

extern int Startgame;
void oled_write_command(uint8_t command)
{
    volatile char *ext_oled_command = (char *) 0x1000;
    ext_oled_command[0] = command;
}

void oled_write_data(uint8_t data)
{
    volatile char *ext_oled_data = (char *) 0x1200;
    ext_oled_data[0] = data;
}

void oled_init()
{
    oled_write_command(0xae); // display off
    oled_write_command(0xa1); // segment remap
    oled_write_command(0xda); // common pads hardware: alternative
    oled_write_command(0x12); 
    oled_write_command(0xc8); // common output scan direction: com63~com0
    oled_write_command(0xa8); // multiplex ration mode:63
    oled_write_command(0x3f);
    oled_write_command(0xd5); // display divide ration/osc. freq. mode
    oled_write_command(0x80);
    oled_write_command(0x81); // contrast control
    oled_write_command(0x50);
    oled_write_command(0xd9); // set pre-charge period
    oled_write_command(0x21);
    oled_write_command(0x20); // Set Memory Addressing Mode
    oled_write_command(0x02);
    oled_write_command(0xdb); // VCOM deselect level mode
    oled_write_command(0x30);
    oled_write_command(0xad); // master configuration
    oled_write_command(0x00);
    oled_write_command(0xa4); // out follows RAM content
    oled_write_command(0xa6); // set normal display
    oled_write_command(0xaf); // display on
}

void clear_OLED()
{

    for(uint8_t i = 0; i < 8; i++){
        selectpage(i);
        selectcolumn(0);
        for(int k = 0; k<128; k++){
            oled_write_data(0x00);
        }
    }
}

void selectpage(uint8_t line){
    oled_write_command(0xB0 + line);
        
}

//Data on the MCP is beng clocked on the rising egde of the clock signal this may affect the mode we need to use (and out on the falling edge)
//CD must be low for this to work obiusly

void selectcolumn(uint8_t column){
    oled_write_command(0x00 + (column % 16));
    oled_write_command(0x10 + (column / 16));
    
}

//For printing characters
void oled_print_char(char c)
{
    c = c - 32;
    for(int i = 0 ; i < 8 ; i++)
    {
        int byte = pgm_read_byte(&font8[c][i]);
        oled_write_data(byte);
    }
}

//For printing Strings(as strings are considered as sequence of characters)
void oled_print(char c[])
{
    for(int i = 0; i < strlen(c); i++)
    {
        oled_print_char(c[i]);
    }
}

uint8_t displayMenu(uint8_t i)
{
    oled_init();

    uint8_t pagePointer =  i;
    char pointer[] = "->";
    char title[] = "MENU";
    char subMenu1[] = "PLAY GAME1";
    char subMenu2[] = "PLAY GAME2";
    char subMenu3[] = "PLAY GAME3";
    //choose a page(0-7)
    clear_OLED();
   
    //choose line/page: 
    selectpage(1);

    //Select column
    selectcolumn(25);

    oled_print(title);

    selectpage(2);
    selectcolumn(25);
    oled_print(subMenu1);

    selectpage(3);
    selectcolumn(25);
    oled_print(subMenu2);

    selectpage(4);
    selectcolumn(25);
    oled_print(subMenu3);

    selectpage(pagePointer);
    selectcolumn(5);
    oled_print(pointer);

    return pagePointer;
}

void subMenu1()
{
    while(is_button_pressed()){
        _delay_ms(100);
    }
    oled_init();

    char title[] = "POSITION 2";
    char exit[] = "START GAME";

    char pointer[] = "->";

    //choose a page(0-7)
    clear_OLED();
   
    //choose line/page: 
    selectpage(1);

    //Select column
    selectcolumn(25);

    oled_print(title);
    selectpage(2);
    selectcolumn(25);
    oled_print(exit);

    selectpage(2);
    selectcolumn(5);
    oled_print(pointer);

while(1)
{
    
    if(is_button_pressed()){
    while(is_button_pressed()){
        _delay_ms(100);
    }
    Startgame=1;
    displayMenu(2);
    break;

    }

}
}
void subMenu2()
{
    while(is_button_pressed()){
        _delay_ms(100);
    }
    oled_init();

    char title[] = "POSITION 3";
    char exit[] = "EXIT";

    char pointer[] = "->";

    //choose a page(0-7)
    clear_OLED();
   
    //choose line/page: 
    selectpage(1);

    //Select column
    selectcolumn(25);

    oled_print(title);
    selectpage(2);
    selectcolumn(25);
    oled_print(exit);

    selectpage(2);
    selectcolumn(5);
    oled_print(pointer);

while(1)
{
    
    if(is_button_pressed()){
    while(is_button_pressed()){
        _delay_ms(100);
    }
    displayMenu(3);
    break;

    }

}
}

void subMenu3()
{
    while(is_button_pressed()){
        _delay_ms(100);
    }
    oled_init();

    char title[] = "POSITION 4";
    char exit[] = "EXIT";

    char pointer[] = "->";

    //choose a page(0-7)
    clear_OLED();
   
    //choose line/page: 
    selectpage(1);

    //Select column
    selectcolumn(25);

    oled_print(title);
    selectpage(2);
    selectcolumn(25);
    oled_print(exit);

    selectpage(2);
    selectcolumn(5);
    oled_print(pointer);

while(1)
{
    
    if(is_button_pressed()){
    while(is_button_pressed()){
        _delay_ms(100);
    }
    displayMenu(4);
    break;

    }

}
}

int is_button_pressed()
    {
        //Read the button state
        if(!(PINB & (1 << PB2)))
        {
            return 1;
        }
        return 0;
    }
void init_menu(volatile char *adc, uint8_t x_start, uint8_t y_start){
    // Set PB2 as input for joystick button
    DDRB &= ~(1 << PB2);
    //Enable internal pull-up resistor on PB2
    PORTB |= (1 << PB2);
    uint8_t pagePointerI=2;
    displayMenu(pagePointerI);
    uint8_t pagePointerJ;

while(1)
{
    pagePointerJ = change_oled_ptr(adc, pagePointerI, x_start, y_start);

    if(pagePointerJ != pagePointerI)
    {
        displayMenu(pagePointerJ);
        pagePointerI = pagePointerJ;
        _delay_ms(300);
    }else if(is_button_pressed() && pagePointerI==2){
        subMenu1();
        break;
    }
    else if(is_button_pressed() && pagePointerI==3){

    
        subMenu2();
    }
    else if(is_button_pressed() && pagePointerI==4){
        subMenu3();
    }
        _delay_ms(200);

}
    
}


uint8_t change_oled_ptr(volatile char *adc, uint8_t pagePointer, uint8_t x_start, uint8_t y_start){

    uint8_t dir= Joy_Direction(adc, x_start, y_start);
    switch(dir)
        {
            case 0:
                //LEFT
                break;
            case 1:
                //RIGHT
                break;
            case 2:
                //DOWN
                if(pagePointer<4){
                    pagePointer++;
                }
                    
                break;
            case 3:
                //UP
                if(pagePointer > 2){
                    pagePointer--;
                }
                    
                break;
            case 4:
                break;
        }
        return pagePointer;
}

void subMenu4(){

    oled_init();

    char title[] = "THE END";
    
    //choose a page(0-7)
    clear_OLED();
   
    //choose line/page: 
    selectpage(1);

    //Select column
    selectcolumn(25);

    oled_print(title);
    
}


