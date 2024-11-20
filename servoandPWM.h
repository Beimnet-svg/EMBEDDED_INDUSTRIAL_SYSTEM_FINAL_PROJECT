#include <stdio.h>
#include <stdlib.h>

typedef struct Message{
    unsigned int id;
    int length;
    char data[8];
}message_t, *message_ptr;

int get_message_data(message_t message);
message_t make_message(unsigned int id, int length, char data[]);
void MakePWMToPortX(int percent);
void setupPWM();
void makeServoOutput();
void Startup();
void button();


