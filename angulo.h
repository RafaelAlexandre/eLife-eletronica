#ifndef _ANGULO_H
#define _ANGULO_H

#include <MPU6050_tockn.h>
#include <Wire.h>

#define SDA_2 33
#define SCL_2 32

//Métodos
void InicializaMPU6050();
void atualizaMPU6050();
void printaAngulos(int angulox, int anguloy, int anguloz);
int pegaAnguloX();
int pegaAnguloY();
int pegaAnguloZ();

#endif //_ANGULO_H
