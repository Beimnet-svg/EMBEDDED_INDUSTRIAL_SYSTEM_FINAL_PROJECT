#include <stdio.h>
#include <stdarg.h>

#include "sam/sam3x/include/sam.h"
#include "time.h"
#include "uart.h"
#include "can.h"
#include "servoandPWM.h"
#include "adc.h"
#include "encoder.h"
#include "motor.h"
#include "interrupt.h"
#include "game.h"

/*
 * Remember to update the Makefile with the (relative) path to the uart.c file.
 * This starter code will not compile until the UART file has been included in the Makefile.
 * If you get somewhat cryptic errors referencing functions such as _sbrk,
 * _close_r, _write_r, _fstat etc, you have most likely not done that correctly.

 * If you get errors such as "arm-none-eabi-gcc: no such file", you may need to reinstall the arm gcc packages using
 * apt or your favorite package manager.
 */
// #include "../path_to/uart.h"

/*
pin A7
cpu pin PA2, A7, D61

*/
    CanMsg m = {};
    uint8_t gain;
    uint8_t dir;
    uint8_t y;
    uint8_t x;
    uint8_t thebutton = 0; 
    float Kp = 10.0f;
    float Ki = 0.1f;
    float integral = 0.0f;
    int score = 0; 
    CanMsg initMsg;
    CanMsg scoreMsg;
    int Start_flag = 0;
    float ref = 5.0f;
    uint8_t flag1 =1; 
    uint8_t Startsignal = 0;
    uint8_t Notstoppet = 1;
    


int main()
{

    SystemInit();
    WDT->WDT_MR = WDT_MR_WDDIS; // Disable Watchdog Timer
    uart_init(84000000, 9600);


    setupPWM();
    can_init1();
    adc_init();
    motor_init();
    encoder_init();
    interrupt_init_PID(50);
    while(!Startsignal){
    updateStart();
    }
    printf("startsignal: %d\n\r", Startsignal);
    Startup();
    printf("Ready for use\n\r");
    Start_flag = 1;
    int i = 0; 
    int K = 0;
    makeMsginit(1, 1, 1);
    //can_tx(initMsg);
    
    while (Notstoppet){

        makeServoOutput();
        
        
        if(isscored()){
            K++;}

        if(K>10){
        
        
        for(int L = 0; L < 10; L++){
            makeMsg(6,1,2);
            can_tx(scoreMsg);
            

        }
            printf("Stopped\n\r");
            Notstoppet =0;
            
        }
        if(!isscored()){
            K=0;
        }
            
         

       
        if(!flag1){
            i++;
            
        }
        if(i > 10){
            flag1 = 1;
    
            i = 0;
        }
    
    }
    
}







/*
    m.id=2;
    m.length=50;
    m.byte[0]=1;
    m.byte[1]=2;
    uint8_t x= getvalue(&m);
    printf("X: %d \n\r",x);*/

    /*
    */
    /**/



/*MakePWMToMotor(40, 0);
    

    
    
    MakePWMToMotor(40, 1);
        
    for(int j  = 0; j < 10000000; j++){}*/






/*message_t message1 = make_message(1,1,"3");
    int a = get_message_data(message1);
    printf("a: %d\n", a);*/
/*
PMC->PMC_PCER0 |= (1<<ID_PIOB);

    PIOB->PIO_OER = PIO_PB13;x
        printf("\n\r++++++++++++++++++++++++++++++++++++++++++++\n\r");
        for (int i = 0; i < 100000; i++){}

    printf("\rThis is a test\n\r");
   while(1){
        PIOB->PIO_CODR = PIO_PB13;}*/

/*
PMC->PMC_PCER0 |= (1<<ID_PIOB);
PIOB->PIO_OER = PIO_PB13;
PIOB->PIO_PER = PIO_PB13;

PIOB->PIO_CODR = PIO_PB13;





printf("\rThis is a test\n\r");
while(1){
PIOB->PIO_CODR = PIO_PB13;
for(int i =0; i< 1000000; i++){

}
PIOB->PIO_SODR = PIO_PB13;
for(int i =0; i< 1000000; i++){

}

    }*/