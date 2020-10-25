#ifndef _DATAHORA_H
#define _DATAHORA_H

#include <NTPClient.h> //Biblioteca NTPClient modificada
#include <WiFiUdp.h> //Socket UDP
#include <SPI.h>

//Struct com os dados do dia e hora
struct Date{
    int dayOfWeek;
    int day;
    int month;
    int year;
    int hours;
    int minutes;
    int seconds;
};

//Métodos:
char* pegaDiaSemana();
int pegaDia();
int pegaMes();
int pegaAno();
int pegaHora();
int pegaMinuto();
int pegaSegundo();
void printaDataHora();
void setupNTP();
Date getDate();

#endif //_DATAHORA_H
