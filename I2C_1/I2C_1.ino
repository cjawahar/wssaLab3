// This file is ConfigureExample -- overwritten.
// I2C interface by default

#include "Wire.h"
#include "SparkFunIMU.h"
#include "SparkFunLSM303C.h"
#include "LSM303CTypes.h"
#include "DebugMacros.h"
#include "FreeRTOS_ARM.h"

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
float magX, magY, magZ;

SemaphoreHandle_t Read, Write;

static void Read_Val(void* arg){
  
  while(1){

    xSemaphoreTake(Read, portMAX_DELAY);
    
    magX = myIMU.readMagX();
    magY = myIMU.readMagY();
    magZ = myIMU.readMagZ();
 
    xSemaphoreGive(Write);
  }
}

static void Write_Val(void* arg){
  
  while(1){
    xSemaphoreTake(Write, portMAX_DELAY);
    
    SerialUSB.println("Magnetometer X: ");
    SerialUSB.println(magX, 4);
    SerialUSB.println("Magnetometer Y: ");
    SerialUSB.println(magY, 4);
    SerialUSB.println("Magnetometer Z: ");
    SerialUSB.println(magZ, 4);
    xSemaphoreGive(Read);
    SerialUSB.println("");
    delay(2000);
  }
}

/* Answers to Part 1 Questions.
 * 
 * 1. The default I2C clockspeed is 100 kHz for the standard mode.
 * 2. The LSM303C's maximum clock speed is 400 kHz.
 * 3. The code is using a 400000 Hz frequency, as seen in Wire.setClock.
 * 4. Looks like 3 bytes are required. 2 to denote the address of device/receiver
 *    and then 1 bytes for the data itself.
 * 5. 
 * 
 */

uint8_t mag_id;
void setup() {

  // Commented out as we are using Wire1 instead of Wire.
  
  //Wire.begin();//set up I2C bus, comment out if using SPI mode
  //Wire.setClock(400000L);//clock stretching, comment out if using SPI mode

  portBASE_TYPE s1, s2;

  while(!SerialUSB);
  Serial.begin(115200);//initialize serial monitor, maximum reliable baud for 3.3V/8Mhz ATmega328P is 57600

  Wire1.begin();
  Wire1.setClock(400000L);
  
  if (myIMU.begin() != IMU_SUCCESS)
  {
    SerialUSB.println("Failed setup.");
    while (1);
  }

  // WHO_AM_I Section

  if (myIMU.mag_whoami() != IMU_SUCCESS) {
    SerialUSB.println("Failure to read: MAG_WHO_AM_I");
  }
  else {
    SerialUSB.println("MAG WHO AM I id = ");
    SerialUSB.println(mag_id, 1);
  }

  //Semaphores
  Read = xSemaphoreCreateCounting(1, 1);
  Write = xSemaphoreCreateCounting(1, 0);

  s1 = xTaskCreate(Read_Val, NULL, configMINIMAL_STACK_SIZE, NULL, 1, NULL);
  s2 = xTaskCreate(Write_Val, NULL, configMINIMAL_STACK_SIZE, NULL, 1, NULL);

  vTaskStartScheduler();
  while(1);
}

void loop()
{
  // Removed everything that was present here from ConfigureExample.
}
