#include "angulo.h"
#include "conexaoWifi.h"
#include "dataHora.h"
#include "diretivas.h"
#include "display.h"
#include "firebase.h"
#include "oximetro.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void inicializaDisplay(){
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Endereço do barramento I2C para o modelo 128x32: 0x3C
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  // Clear the buffer
  display.clearDisplay();

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
}

void printaDisplay(int bpm, int spo2){
  display.clearDisplay();

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(20, 10);             // Start at top-left corner
  display.print(F("BPM: "));
  display.println(bpm);
  display.setCursor(20, 20);
  display.print(F("SpO2: "));
  display.println(spo2);
  display.drawBitmap(
    100,
    (display.height() - LOGO_HEIGHT) / 2,
    logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);

  display.display();  
}
