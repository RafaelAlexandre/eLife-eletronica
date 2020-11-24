#include "diretivas.h"
#include "display.h"
#include "dataHora.h"
#include "firebase.h"
#include "conexaoWifi.h"
#include "oximetro.h"

//Variaveis de apoio
uint32_t tsLastReport = 0;
uint32_t tsLastReportFirebase = 0;
uint32_t spo2 = 0;
uint32_t bpm = 0;
uint32_t flag = 0;

//Variável dos remédios que vem do Realtime Database
String medicine;

//Variáveis do buzzer
int buzzer_pin      = 15;
int channel         = 0;
int frequence       = 2000;
int resolution      = 10;
unsigned long timer = millis();
int flagAlarme = 0;
TaskHandle_t dobitaobyte;
eTaskState statusOf;


void sirene(void *pvParameters){
    ledcWriteTone(channel,1000);
    delay(500);
    ledcWriteTone(channel,3000);
    delay(500);
    ledcWriteTone(channel,1000);
    delay(500);
    ledcWriteTone(channel,3000);
    delay(500);
    ledcWriteTone(channel,1000);
    delay(500);
    ledcWriteTone(channel,3000);
    delay(500);
    ledcWriteTone(channel, 0);
    vTaskDelete(NULL);
}

void restartVariables() {
  //bpm = 0;
  //spo2 = 0;
  flag = 0;
  inicializaMAX30100();
  Serial.println("Variáveis resetadas!");
}

void setup() {
  ledcSetup(channel, frequence, resolution);
  ledcAttachPin(buzzer_pin, channel);
  timer = millis();
  Serial.begin(115200);
  inicializaMAX30100();
  inicializaDisplay();
  xTaskCreatePinnedToCore(sirene,"sirene", 10000, NULL, 1, &dobitaobyte,0);
  //wiFiConnection();
}

void loop() {
  atualizaMAX30100();

  if(flagAlarme == 1){
    statusOf = eTaskGetState(dobitaobyte);
    if (statusOf == eReady){
      xTaskCreatePinnedToCore(sirene,"sirene", 10000, NULL, 1, &dobitaobyte,0);
    }
    flagAlarme = 0;
  }
  
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    if (flag == 0) {
      //pega todos os valores dos sensores
      bpm = pegaBpm();
      spo2 = pegaSpo2();
      //printa valores na serial
      //printaDataHora();
      
      printaBpmSpo2(bpm, spo2);
      //Serial.println(flagAlarme);
    }
    tsLastReport = millis();
  }

    if (millis() - tsLastReportFirebase > REPORTING_PERIOD_MS_FIREBASE) {
      //Verificar se a string retornada do leDadosRealTime é  != "x"
      medicine = leDadosRealTime();
      if(!medicine.equals("x")){
        printaDisplayMedicine(medicine);
        flagAlarme = 1;
      }
      else{ 
        if(!((bpm<=80)&&(bpm>=40)) && (spo2<90)){
          flagAlarme = 1;
          printaDisplayAlarmeSpo2BPMcriticos(bpm, spo2);
        }
        else if((bpm<40)&&(spo2>=90)){
          flagAlarme = 1;
          printaDisplayAlarmeBPMbaixo(bpm, spo2);
        }
        else if(((bpm<=80)&&(bpm>=40))&&(spo2<90)){
          flagAlarme = 1;
          printaDisplayAlarmeSpo2Baixo(bpm, spo2);
        }
        else if((bpm >80)&&(spo2>=90)){
          flagAlarme = 1;
          printaDisplayAlarmeBPMalto(bpm, spo2);
        }
        if(flagAlarme == 0){
          printaDisplay(bpm, spo2);
        }
        
        flag = 1;
        desligaMAX30100();
        wiFiConnection();
        printaDataHora();
    
        if (verificaConexao()) {
          enviaDadosFirebase(bpm, spo2, flagAlarme);
          wiFiDisconnection();
        }
        restartVariables();
        tsLastReportFirebase = millis();
      }
    }
}
