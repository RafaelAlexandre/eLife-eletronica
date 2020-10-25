#include "angulo.h"
#include "conexaoWifi.h"
#include "dataHora.h"
#include "diretivas.h"
#include "display.h"
#include "firebase.h"
#include "oximetro.h"

TwoWire I2Ctwo = TwoWire(1);
MPU6050 mpu6050(I2Ctwo);

void InicializaMPU6050(){
  I2Ctwo.begin(SDA_2, SCL_2, 100000);
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
}

void atualizaMPU6050(){
  mpu6050.update();  
}

void printaAngulos(int angulox, int anguloy, int anguloz){
      Serial.println("-------------Posição---------------");
      Serial.print("angleX : ");
      Serial.print(angulox);
      Serial.print("\tangleY : ");
      Serial.print(anguloy);
      Serial.print("\tangleZ : ");
      Serial.println(anguloz);
      Serial.println(" ");
}

int pegaAnguloX(){
  return mpu6050.getAngleX();
}
int pegaAnguloY(){
  return mpu6050.getAngleY();
}
int pegaAnguloZ(){
  return mpu6050.getAngleZ();
}
