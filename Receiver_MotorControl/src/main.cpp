/*
 * ************** RECEIVER : RF MOTOR CONTROL PROJECT ************
 
 *CE pin is always an input with respect to the 24L01. It is used to control data transmission. 
    and reception when in TX and RX modes, respectively. 
 *CSN stands for chip select not. This is the enable pin for the SPI bus, and it is active low
  
 * nRF24L01 uses SPI interface with Atmega328p or any other microcontroller 
  
  
  
   By: Pankaj Rai
   Date: 16 November 2019
 */


#include <SPI.h>
#include <nRF24L01.h>   // Download NRF24L01 library created by TMRh20 https://github.com/TMRh20/RF24
#include <RF24.h>

#define CE_PIN   9
#define CSN_PIN 10
#define Relay 8
#define LevelLed_3 7
#define LevelLed_2 6
#define LevelLed_1 5
#define LevelLed_0 4
#define motorOnTime 10000    //for now it is in milli seconds

const byte thisSlaveAddress[5] = {'N','O','D','E','1'};

RF24 radio(CE_PIN, CSN_PIN);

char dataReceived;    // this must match dataToSend in the TX
bool newData = false;

void getData() 
{
    if ( radio.available() ) 
    {
        radio.read( &dataReceived, sizeof(dataReceived) );
        newData = true;
        
    }       
}

//**************************************************** RELAY FUNCTION *************************************************************>>>>>
void MotorOnOff()
{
  
  char x= dataReceived;
  
  if(x == '3')          
  { 
    Serial.println(" TANK IS FILLED ALREADY!!! "); 
    digitalWrite(Relay, LOW); 
    digitalWrite(LevelLed_3, HIGH);   
  }
  else if(x == '2')          
  { 
    Serial.println(" TANK IS at level 2 "); 
    digitalWrite(Relay, LOW); 
    digitalWrite(LevelLed_2, HIGH);   
  }
  else if(x== '1' )          
  { 
    Serial.println(" TANK IS at level 1 "); 
    digitalWrite(Relay, LOW); 
    digitalWrite(LevelLed_1, HIGH);   
  }

  else if(x == '0' )    
  {
    Serial.println(" STARTING THE MOTOR NOW ====>");
    digitalWrite(Relay, HIGH);
    delay(motorOnTime);              
    digitalWrite(Relay, LOW);   
    Serial.println(" MOTOR IS TURNED OFF!!! ");
  }
}
//<<<<<<<<*********************************************************************************************************************************//


void showData()
{
    if (newData == true) 
    {
        Serial.print("Data received ");
        Serial.println(dataReceived);
        newData = false;
    }
}

void setup() 
{
    Serial.begin(9600);
    pinMode(Relay, OUTPUT);
    digitalWrite(Relay, LOW);    //First, keep the motor in OFF state

    Serial.println("Rx Starting---");
    radio.begin();
    radio.setDataRate( RF24_250KBPS );
    radio.openReadingPipe(1, thisSlaveAddress);
    radio.startListening();
}

//======================================================= VOID LOOP =================================================>>>>

void loop()
{
    getData();
    showData();
    MotorOnOff();  
}

//<<<<<===================================================================================================================





//=========================================================== END OF CODE HERE ========================================================//











