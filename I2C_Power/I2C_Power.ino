// This file is ConfigureExample -- overwritten.
// I2C interface by default

#include "Wire.h"
#include "SparkFunIMU.h"
#include "SparkFunLSM303C.h"
#include "LSM303CTypes.h"
#include "DebugMacros.h"
#include "FreeRTOS_ARM.h"
#include "PowerDueLED.h"

/*
   define DEBUG 1 in SparkFunLSM303C.cpp turns on debugging statements.
   Redefine to 0 to turn them off.
*/

/*
   SPI pins defined in SparkFunLSM303C.h for Pro Mini
   D10 -> SDI/SDO
   D11 -> SCLK
   D12 -> CS_XL
   D13 -> CS_MAG
*/

LSM303C myIMU;
int loopCtr, maxLoops;
int mag_id;

void setup() {

  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);

  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);

  while(!SerialUSB);
  Serial.begin(9600);
  SerialUSB.println("Testing - Serial running");

  Wire1.begin();
  Wire1.setClock(400000L);
  
  if (myIMU.begin() != IMU_SUCCESS)
  {
    SerialUSB.println("Failed setup.");
    SerialUSB.println(myIMU.begin());
    SerialUSB.println(IMU_SUCCESS);
    while (1);
  }

  loopCtr = 0;
  maxLoops = 10;
}

void loop()
{
  SerialUSB.println("Start loop");
  uint8_t who;

  digitalWrite(6, HIGH);
  
  if (loopCtr < maxLoops) {
    //myIMU.I2C_ByteRead(MAG_I2C_ADDR, MAG_WHO_AM_I, who);
    myIMU.readAccelX();
  } else {
    pd_rgb_led(PD_RED);
  }
  digitalWrite(6, LOW);
  loopCtr++;

  SerialUSB.println("End loop");
  delay(500);
}
