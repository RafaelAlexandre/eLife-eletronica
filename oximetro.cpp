#include "angulo.h"
#include "conexaoWifi.h"
#include "dataHora.h"
#include "diretivas.h"
#include "display.h"
#include "firebase.h"
#include "oximetro.h"

PulseOximeter pox;

void onBeatDetected()
{
  Serial.println("Beat!");
}

void inicializaMAX30100() {
  Serial.println(" ");
  Serial.println("Inicializando OXIMETRO");
  if(!pox.begin()){
    Serial.println("MAX30100 FAILED");
    for (;;);
  }
  else{
    Serial.println("MAX30100 SUCCESS");
  }
  pox.setOnBeatDetectedCallback(onBeatDetected);
}

uint32_t pegaBpm() {
  return pox.getHeartRate();
}

uint32_t pegaSpo2() {
  return pox.getSpO2();
}

void atualizaMAX30100() {
  pox.update();
}

void printaBpmSpo2(uint32_t bpm, uint32_t spo2) {
  Serial.println("------------BPM E SPO2-------------");
  Serial.print("Heart rate:");
  Serial.print(bpm);
  Serial.print("bpm / SpO2:");
  Serial.print(spo2);
  Serial.println("%");
}

void desligaMAX30100() {
  pox.shutdown();
}
