#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   15
#define LOGO_WIDTH    15

//BITMAP DO ÍCONE DE CORAÇÃO
static const unsigned char PROGMEM logo_bmp[] =
{ B00111100,B01111000,
  B01111110,B11111100,
  B01111111,B11111100,
  B11111111,B11111110,
  B11111111,B11111110,
  B11111111,B11111110,
  B11111111,B11111110,
  B01111111,B11111100,
  B01111111,B11111000,
  B00111111,B11111000,
  B00011111,B11110000,
  B00001111,B11100000,
  B00000111,B11000000,
  B00000011,B10000000,
  B00000001,B00000000 };


//MÉTODOS:
  void inicializaDisplay();
  void printaDisplay(int bpm, int spo2);
  void printaDisplayAlarmeBPMalto(int bpm, int spo2);
  void printaDisplayAlarmeBPMbaixo(int bpm, int spo2);
  void printaDisplayAlarmeSpo2Baixo(int bpm, int spo2);
  void printaDisplayAlarmeSpo2BPMcriticos(int bpm, int spo2);
#endif // _DISPLAY_H
