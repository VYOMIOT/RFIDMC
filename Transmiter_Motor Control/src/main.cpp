/*
 * ************** TRANSMITTER : MOTOR CONTROL PROJECT ************
  
 *CE pin is always an input with respect to the 24L01. It is used to control data transmission. 
    and reception when in TX and RX modes, respectively. 
 *CSN stands for chip select not. This is the enable pin for the SPI bus, and it is active low
  
 * nRF24L01 uses SPI interface with Atmega328p or any other microcontroller 
  
  
  
   By: Pankaj Rai
   Date: 13 November 2019
 */



#include <SPI.h>
#include <nRF24L01.h>    // Download NRF24L01 library created by TMRh20 https://github.com/TMRh20/RF24
#include <RF24.h>


#define CE_PIN   9
#define CSN_PIN 10
// #define Sensor 8
const byte slaveAddress[5] = {'N','O','D','E','1'};

RF24 radio(CE_PIN, CSN_PIN); // Create a Radio from RF24
char txNum = '0';

unsigned long currentMillis;
unsigned long prevMillis;
unsigned long txIntervalMillis = 3000; // !!!!!send once per 3 second (Adjust according to requirement)


//=============== Sensor Function ==================>>>>>>
char WaterLevel()
{
  int level=9;    //this level will be taken by using water level SENSOR (here Im giving manually to check)
  if(level>=8)
  {
    return 48;  //  ascii equivalent of 0 is 48 and of 1 is 49
  }
  else if(level <=3)
  {
    return 49;      //motor ON for motorOnTime
  }
}
//<<<<<<<<<=================================================

char dataToSend= {(WaterLevel())};

void updateMessage() 
{
    // so you can see that new data is being sent
    txNum += 1;
    if (txNum > '9') {
        txNum = '0';
    }
    dataToSend = txNum;
}


//=============== Function for : Data to be sent  ==================>>>>>>
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
        updateMessage();
    }
    else 
    {
        Serial.println("  Tx failed");
    }
}
//<<<<<<<<<==============================================================


//======================================= SETUP & LOOP FUNCTION =====================>>>>
void setup()
{

    Serial.begin(9600);
    Serial.println("SimpleTx Starting");

    //pinMode(Sensor, INPUT);    //Uncomment it when sensor is connected

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

//========================= TRANSMITTER CODE ENDS HERE ==========================//