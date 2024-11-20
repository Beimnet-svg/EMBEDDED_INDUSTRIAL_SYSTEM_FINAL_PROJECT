#pragma once
#include <stdlib.h>
#include <avr/io.h>
#include <stdio.h>
#include "ADC.h"
#include "can1.h"



void SendJoyPos(volatile char *adc, uint8_t x_start, uint8_t y_start);
void SendJoyPosStart(volatile char *adc, uint8_t x_start, uint8_t y_start);