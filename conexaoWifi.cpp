#include "angulo.h"
#include "conexaoWifi.h"
#include "dataHora.h"
#include "diretivas.h"
#include "display.h"
#include "firebase.h"
#include "oximetro.h"

void wiFiConnection() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  setupNTP();
  firebaseInit();
}

void wiFiDisconnection() {
  WiFi.disconnect();
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi desconectado!");
  }
}

bool verificaConexao() {
  if(WiFi.status() == WL_CONNECTED)
    return true;
  else
    return false;
}
