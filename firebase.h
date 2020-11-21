#ifndef _FIREBASE_H
#define _FIREBASE_H

#include <IOXhop_FirebaseESP32.h> // Lib Firebase
#include <ArduinoJson.h> // Lib para a manipulação de Json


#define FIREBASE_HOST "https://tfg2020-7ccf6.firebaseio.com/"
#define FIREBASE_AUTH "5ntxgIgHw3BNJqKsbEWCL6HqXUUnTIKFF7JJ68Ko"

//Métodos:
void firebaseInit();
void enviaDadosFirebase(uint32_t bpm, uint32_t spo2, int flagAlarme);

#endif //_FIREBASE_H
