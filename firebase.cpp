#include "conexaoWifi.h"
#include "dataHora.h"
#include "diretivas.h"
#include "display.h"
#include "firebase.h"
#include "oximetro.h"

String dadoMedicine;
int contadorMedicine;

void firebaseInit() {
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void enviaDadosFirebase(uint32_t bpm, uint32_t spo2, int flagAlarme) {

  //Enviando para o FIREBASE
  StaticJsonBuffer<100> jsonBufferSensorAppMedicine;
  StaticJsonBuffer<250> jsonBufferSensor;
  StaticJsonBuffer<100> jsonBufferSensorApp;
  JsonObject& sensorData = jsonBufferSensor.createObject();
  JsonObject& sensorDataApp = jsonBufferSensorApp.createObject();
  JsonObject& sensorDataAppMedicine = jsonBufferSensorAppMedicine.createObject();
  // Inserimos os atributos de temperatura e umidade no json sensorData
  sensorData["SPO2"] = spo2;
  sensorData["BPM"] = bpm;
  sensorData["Dia da semana"] = pegaDiaSemana();
  sensorData["Dia"] = pegaDia();
  sensorData["Mês"] = pegaMes();
  sensorData["Ano"] = pegaAno();
  sensorData["Hora"] = pegaHora();
  sensorData["Minutos"] = pegaMinuto();
  sensorData["Segundos"] = pegaSegundo();
  sensorData["flagAlarme"] = flagAlarme;
  sensorDataApp["BPM"] = bpm;
  sensorDataApp["SPO2"] = spo2;
  // Enviamos o json sensorData para o Firebase na pasta History
  Firebase.push("Medico/PatientsDataPerDeviceId/MRA0001UI", sensorData);
  Firebase.set("/Users/MRA0001UI", sensorDataApp);

}

String leDadosRealTime(){
  //verificar se o caminho está correto
  contadorMedicine = Firebase.getInt("/Medicines/MRA0001UI/Medicines/count");
  
  if(contadorMedicine == 1){
    dadoMedicine = Firebase.getString("/Medicines/MRA0001UI/Medicines/name");
    return dadoMedicine;
  }
  else if (contadorMedicine > 1){
    return "Tomar remédios";
  }
  else{
    return "x";
  }
  
}

void enviaNuloRealTime(){
  Firebase.push("/Medicines/MRA0001UI/Medicines/count", 0);
  Firebase.push("/Medicines/MRA0001UI/Medicines/name", "x");
}
