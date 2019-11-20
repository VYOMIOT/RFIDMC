/*
 * **************TRANSMITTER : MOTOR CONTROL PROJECT************
 * 
 *CE pin is always an input with respect to the 24L01. It is used to control data transmission. 
 *   and reception when in TX and RX modes, respectively. 
 *CSN stands for chip select not. This is the enable pin for the SPI bus, and it is active low
 * 
 * nRF24L01 uses SPI interface with Atmega328p or any other microcontroller 
 * 
 * Developer: Pankaj Rai (pankaj200497@gmail.com)
 * Date: 13 November 2019
 */



#include <SPI.h>
#include <nRF24L01.h>    // Download NRF24L01 library created by TMRh20 https://github.com/TMRh20/RF24
#include <RF24.h>


#define CE_PIN   9
#define CSN_PIN 10
#define SensorPin0 8
#define SensorPin1 7
#define SensorPin2 6
#define SensorPin3 5
#define SensorPin4 4

const byte slaveAddress[5] = {'N','O','D','E','1'};


RF24 radio(CE_PIN, CSN_PIN); // Create a Radio
char txNum = '0';

unsigned long currentMillis;
unsigned long prevMillis;
unsigned long txIntervalMillis = 3000; // send once per 3 second


//=============== Sensor Function ==================>>>>>>>
char WaterLevel()
{
    int level0= digitalRead(SensorPin0);  // 0%  Water Level
    int level1= digitalRead(SensorPin1);  // 25% Water Level
    int level2= digitalRead(SensorPin2);  // 50% water Level
    int level3= digitalRead(SensorPin3);  // 75% Water Level
    int level4= digitalRead(SensorPin4);  // 100% Water Level
     
  if(level4 == HIGH && level3 == HIGH && level2 == HIGH && level1 == HIGH && level0 == HIGH)
  {
    Serial.println("Tank is Completely Filled !!");
    return 52;      // [4] ascii equivalent of 0 is 48 , 100% Water Level
  }
  else if(level3 == HIGH && level2 == HIGH && level1 == HIGH && level0 == HIGH)
  {
    Serial.println("Tank is filled upto level 3 ");
    return 51;      // [3] 
  }
  else if(level2 == HIGH && level1 == HIGH && level0 == HIGH)
  {
    Serial.println("Tank is filled upto level 2 ");
    return 50;      // [2] 
  }
  else if(level1 == HIGH && level0 == HIGH)
  {
    Serial.println("Tank is Empty, Start the Motor...");
    return 49;      // [1] 
  }
  else if(level0 == HIGH)
  {
    Serial.println("Tank is Empty, Start the Motor...");
    return 48;      // [0] motor ON for motorOnTime, 0% Water Level
  }

}

//<<<<<<=================================================

char dataToSend = {(WaterLevel())};

//=============== Data to be sent Function ==================

void send() 
{

    bool rslt;
    rslt = radio.write( &dataToSend, sizeof(dataToSend) );
        // Always use sizeof() as it gives the size as the number of bytes.
        // For example if dataToSend was an int sizeof() would correctly return 2

    Serial.print("Data Sent ");
    Serial.print(dataToSend);
    if (rslt) 
    {
        Serial.println("  Acknowledge received");
    }
    else 
    {
        Serial.println("  Tx failed");
    }
}

//========================================================

void setup()
{

    Serial.begin(9600);
    Serial.println("SimpleTx Starting");

    pinMode(SensorPin0, INPUT);
    pinMode(SensorPin1, INPUT);
    pinMode(SensorPin2, INPUT);
    pinMode(SensorPin3, INPUT);
    pinMode(SensorPin4, INPUT);


    radio.begin();
    radio.setDataRate( RF24_250KBPS );
    radio.setRetries(3,5); // delay, count
    radio.openWritingPipe(slaveAddress);
}


void loop() 
{
    currentMillis = millis();
    if (currentMillis - prevMillis >= txIntervalMillis) {
        send();
        prevMillis = millis();
    }
}

//========================= CODE ENDS HERE ==========================//
