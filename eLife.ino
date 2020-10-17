//----------------INCLUDES E DEFINES DO DISPLAY OLED-------------------
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   15
#define LOGO_WIDTH    15


//-----------------INCLUDES PARA DATA E HORA --------------------------
#include <NTPClient.h> //Biblioteca NTPClient modificada
#include <WiFiUdp.h> //Socket UDP
#include <SPI.h>

//-----------------FIREBASE IMPORTS E DEFINES--------------------------
#include <WiFi.h>
//#include <FirebaseESP32.h>
#include <IOXhop_FirebaseESP32.h> // Lib Firebase
#include <ArduinoJson.h> // Lib para a manipulação de Json


#define FIREBASE_HOST "https://tfg2020-7ccf6.firebaseio.com/"
#define FIREBASE_AUTH "5ntxgIgHw3BNJqKsbEWCL6HqXUUnTIKFF7JJ68Ko"
#define WIFI_SSID "MAIARA"
#define WIFI_PASSWORD "31082063"
//FirebaseData firebaseData;
//---------------------------------------------------------------------

//---------------------DEFINIÇÕES DO MPU6050---------------------------
#include <MPU6050_tockn.h>

#define SDA_2 33
#define SCL_2 32

TwoWire I2Ctwo = TwoWire(1);
MPU6050 mpu6050(I2Ctwo);

//--------------------MAX30100 IMPORTS E DEFINES-----------------------
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

#define REPORTING_PERIOD_MS 1000
#define REPORTING_PERIOD_MS_FIREBASE 9000
// PulseOximeter is the higher level interface to the sensor
// it offers:
// * beat detection reporting
// * heart rate calculation
// * SpO2 (oxidation level) calculation
//---------------------------------------------------------------------

PulseOximeter pox;

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

uint32_t tsLastReport = 0;
uint32_t tsLastReportFirebase = 0;
uint32_t spo2 = 0;
uint32_t bpm = 0;
uint32_t flag = 0;

int angulox, anguloy, anguloz;


//Fuso Horário, no caso horário de verão de Brasília 
int timeZone = -3;

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



void restartVariables(){

  //bpm = 0;
  //spo2 = 0;  
  flag = 0;
  Serial.println("Variáveis resetadas!");

}

// Callback (registered below) fired when a pulse is detected
void onBeatDetected()
{
  
  Serial.println("Beat!");  

}

void InicializaMPU6050(){
  I2Ctwo.begin(SDA_2, SCL_2, 100000);
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
}

void setup() {
  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  // Clear the buffer
  display.clearDisplay();

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  
  Initialization();
  InicializaMPU6050();
  //WiFiConnection();
    
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
    Date date;
    sscanf(strDate, "%d-%d-%dT%d:%d:%dZ", 
                    &date.year, 
                    &date.month, 
                    &date.day, 
                    &date.hours, 
                    &date.minutes,
                    &date.seconds);

    //Dia da semana de 0 a 6, sendo 0 o domingo
    date.dayOfWeek = ntpClient.getDay();
    return date;
}



void Initialization() {

  Serial.print("Initializing pulse oximeter..");

  // Initialize the PulseOximeter instance
  // Failures are generally due to an improper I2C wiring, missing power supply
  // or wrong target chip
  if (!pox.begin()) {
    Serial.println("FAILED");
    for (;;);
  } else {
    Serial.println("SUCCESS");
  }

  // The default current for the IR LED is 50mA and it could be changed
  // by uncommenting the following line. Check MAX30100_Registers.h for all the
  // available options.
  // pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);

  // Register a callback for the beat detection
  pox.setOnBeatDetectedCallback(onBeatDetected);
}

void WiFiConnection() {
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

  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

}

void WiFiDisconnection(){
  WiFi.disconnect();
  if(WiFi.status() != WL_CONNECTED){
    Serial.println("WiFi desconectado!");
  }
  //restarta todas as variáveis
  restartVariables();
  Initialization();
}

void loop() {

  // Make sure to call update as fast as possible
  pox.update();
  mpu6050.update();

  // Asynchronously dump heart rate and oxidation levels to the serial
  // For both, a value of 0 means "invalid"
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    if(flag == 0){
      bpm = pox.getHeartRate();
      spo2 = pox.getSpO2();
      angulox = mpu6050.getAngleX();
      anguloy = mpu6050.getAngleY();
      anguloz = mpu6050.getAngleZ();
      Serial.println("------------BPM E SPO2-------------");
      Serial.print("Heart rate:");
      Serial.print(bpm);
      Serial.print("bpm / SpO2:");
      Serial.print(spo2);
      Serial.println("%");
      Serial.println("-------------Posição---------------");
      Serial.print("angleX : ");
      Serial.print(angulox);
      Serial.print("\tangleY : ");
      Serial.print(anguloy);
      Serial.print("\tangleZ : ");
      Serial.println(anguloz);
      Serial.println(" ");
    }
    tsLastReport = millis();      
  }

  if (millis() - tsLastReportFirebase > REPORTING_PERIOD_MS_FIREBASE) {
    flag = 1;
    printaDisplay(bpm, spo2);
    pox.shutdown();
    WiFiConnection();
    
    Date date = getDate();
    Serial.print(dayOfWeekNames[date.dayOfWeek]);
    Serial.print(" ");
    Serial.print(date.day);
    Serial.print("/"); 
    Serial.print(date.month);
    Serial.print("/");
    Serial.print(date.year);
    Serial.print(" "); 
    Serial.print(date.hours);
    Serial.print(":");
    Serial.print(date.minutes);
    Serial.print(":");
    Serial.println(date.seconds);
    
    
    if(WiFi.status() == WL_CONNECTED){
      
      
      //Enviando para o FIREBASE
      StaticJsonBuffer<200> jsonBufferSensor;
      StaticJsonBuffer<100> jsonBufferSensorApp;
      JsonObject& sensorData = jsonBufferSensor.createObject();
      JsonObject& sensorDataApp = jsonBufferSensorApp.createObject();
      // Inserimos os atributos de temperatura e umidade no json sensorData
      sensorData["SPO2"] = spo2;
      sensorData["BPM"] = bpm;
      sensorData["Dia da semana"] = dayOfWeekNames[date.dayOfWeek];
      sensorData["Dia"] = date.day;
      sensorData["Mês"] = date.month;
      sensorData["Ano"] = date.year;
      sensorData["Hora"] = date.hours;
      sensorData["Minutos"] = date.minutes;
      sensorData["Segundos"] = date.seconds;
      sensorData["ÂnguloX"] = angulox;
      sensorData["ÂnguloY"] = anguloy;
      sensorData["ÂnguloZ"] = anguloz;
      sensorDataApp["BPM"] = bpm;
      sensorDataApp["SPO2"] = spo2;
      // Enviamos o json sensorData para o Firebase na pasta History
      Firebase.push("Medico/Users/ESPID", sensorData);
      Firebase.set("/Users/ESPID", sensorDataApp);
      
     // Firebase.setInt("/Users/ESPID/BPM", bpm);
      //Firebase.setInt("/Users/ESPID/SPO2", spo2);

      
      WiFiDisconnection();
    }

    tsLastReportFirebase = millis(); 
  }  
  


  


  



  /*if(Firebase.pushInt(firebaseData, "/Users/Count",counter++)){
       //Serial.println(counter);
     }

    if(Firebase.setInt(firebaseData, "/Users/CountMobile",counter++)){
       //Serial.println(counter);
     }

    if(Firebase.setString(firebaseData, "/Count3","ACTIVE")){
     }


    if(Firebase.getInt(firebaseData, "/Count")){
     if(firebaseData.dataType() == "int"){
       Serial.print("data = ");
       Serial.println(firebaseData.intData());
     }
    }

    if(Firebase.getFloat(firebaseData, "/Count2")){
     if(firebaseData.dataType() == "float"){
       Serial.print("data = ");
       Serial.println(firebaseData.floatData());
     }
    }

     if(Firebase.getString(firebaseData, "/Count3")){
     if(firebaseData.dataType() == "string"){
       Serial.print("data = ");
       Serial.println(firebaseData.stringData());
     }
    }*/

}
