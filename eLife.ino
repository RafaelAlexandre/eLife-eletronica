#include "diretivas.h"
#include "display.h"
#include "dataHora.h"
#include "firebase.h"
#include "angulo.h"
#include "conexaoWifi.h"
#include "oximetro.h"

//Variaveis de apoio
uint32_t tsLastReport = 0;
uint32_t tsLastReportFirebase = 0;
uint32_t spo2 = 0;
uint32_t bpm = 0;
uint32_t flag = 0;
int angulox, anguloy, anguloz;



void restartVariables() {
  //bpm = 0;
  //spo2 = 0;
  flag = 0;
  inicializaMAX30100();
  Serial.println("Variáveis resetadas!");
}

void setup() {
  Serial.begin(115200);
  InicializaMPU6050();
  inicializaMAX30100();
  inicializaDisplay();
  //wiFiConnection();
}

void loop() {
  atualizaMAX30100();
  atualizaMPU6050();
  
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    if (flag == 0) {
      //pega todos os valores dos sensores
      bpm = pegaBpm();
      spo2 = pegaSpo2();
      angulox = pegaAnguloX();
      anguloy = pegaAnguloY();
      anguloz = pegaAnguloZ();
      printaDisplay(bpm, spo2);
      //printa valores na serial
      //printaDataHora();
      printaAngulos(angulox, anguloy, anguloz);
      printaBpmSpo2(bpm, spo2);
    }
    tsLastReport = millis();
  }

  if (millis() - tsLastReportFirebase > REPORTING_PERIOD_MS_FIREBASE) {
    flag = 1;
    desligaMAX30100();
    wiFiConnection();
    printaDataHora();

    if (verificaConexao()) {
      enviaDadosFirebase(bpm, spo2, angulox, anguloy, anguloz);
      wiFiDisconnection();
    }
    restartVariables();
    tsLastReportFirebase = millis();
  }
}
