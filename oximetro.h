#ifndef _OXIMETRO_H
#define _OXIMETRO_H

#define SDA_1 34
#define SCL_1 35

#include "MAX30100_PulseOximeter.h"


//Métodos:
void onBeatDetected();
void inicializaMAX30100();
uint32_t pegaBpm();
uint32_t pegaSpo2();
void atualizaMAX30100();
void printaBpmSpo2(uint32_t bpm, uint32_t spo2);
void desligaMAX30100();

#endif //_OXIMETRO_H
