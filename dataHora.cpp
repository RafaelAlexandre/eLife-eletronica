#include "conexaoWifi.h"
#include "dataHora.h"
#include "diretivas.h"
#include "display.h"
#include "firebase.h"
#include "oximetro.h"

//Fuso Horário, no caso horário de verão de Brasília 
int timeZone = -3;

//Variável para pegar a data
Date date;

//Socket UDP que a lib utiliza para recuperar dados sobre o horário
WiFiUDP udp;
//Objeto responsável por recuperar dados sobre horário
NTPClient ntpClient(
    udp,                    //socket udp
    "0.br.pool.ntp.org",    //URL do servwer NTP
    timeZone*3600,          //Deslocamento do horário em relacão ao GMT 0
    60000);                 //Intervalo entre verificações online

//Nomes dos dias da semana
char* dayOfWeekNames[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setupNTP()
{
    //Inicializa o client NTP
    ntpClient.begin();
    
    //Espera pelo primeiro update online
    Serial.println("Waiting for first update");
    while(!ntpClient.update())
    {
        Serial.print(".");
        ntpClient.forceUpdate();
        delay(50);
    }

    Serial.println();
    Serial.println("First Update Complete");
}


Date getDate()
{
    //Recupera os dados de data e horário usando o client NTP
    char* strDate = (char*)ntpClient.getFormattedDate().c_str();

    //Passa os dados da string para a struct
    Date dataAtual;
    sscanf(strDate, "%d-%d-%dT%d:%d:%dZ", 
                    &dataAtual.year, 
                    &dataAtual.month, 
                    &dataAtual.day, 
                    &dataAtual.hours, 
                    &dataAtual.minutes,
                    &dataAtual.seconds);

    //Dia da semana de 0 a 6, sendo 0 o domingo
    dataAtual.dayOfWeek = ntpClient.getDay();
    return dataAtual;
}

void printaDataHora(){
    date = getDate();
    Serial.print(pegaDiaSemana());
    Serial.print(" ");
    Serial.print(pegaDia());
    Serial.print("/"); 
    Serial.print(pegaMes());
    Serial.print("/");
    Serial.print(pegaAno());
    Serial.print(" "); 
    Serial.print(pegaHora());
    Serial.print(":");
    Serial.print(pegaMinuto());
    Serial.print(":");
    Serial.println(pegaSegundo());
}

char* pegaDiaSemana(){
  return dayOfWeekNames[date.dayOfWeek];  
}
int pegaDia(){
  return date.day;  
}
int pegaMes(){
  return date.month;  
}
int pegaAno(){
  return date.year;  
}
int pegaHora(){
  return date.hours;  
}
int pegaMinuto(){
  return date.minutes;  
}
int pegaSegundo(){
  return date.seconds;  
}
