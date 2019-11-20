/*
 * ************** RECEIVER : RF MOTOR CONTROL PROJECT ************
 
 *CE pin is always an input with respect to the 24L01. It is used to control data transmission. 
    and reception when in TX and RX modes, respectively. 
 *CSN stands for chip select not. This is the enable pin for the SPI bus, and it is active low
  
 * nRF24L01 uses SPI interface with Atmega328p or any other microcontroller 
  
  
  
   Developer: Pankaj Rai (pankaj200497@gmail.com)
   Date: 16 November 2019
 */



#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN   9
#define CSN_PIN 10
#define Relay 8
#define LEDLevel_100 7
#define LEDLevel_75  6
#define LEDLevel_50  5
#define LEDLevel_25  4
#define LEDLevel_0   3
#define motorOnTime 10000   //for now it is in milli seconds

const byte thisSlaveAddress[5] = {'N','O','D','E','1'};

RF24 radio(CE_PIN, CSN_PIN);

char dataReceived;    // this must match dataToSend in the TX
bool newData = false;


void setup() 
{

    Serial.begin(9600);
    pinMode(Relay, OUTPUT);
    digitalWrite(Relay, LOW);    //First, keep the motor in OFF state
    
    pinMode(LEDLevel_0, OUTPUT);
    pinMode(LEDLevel_25, OUTPUT);
    pinMode(LEDLevel_50, OUTPUT);
    pinMode(LEDLevel_75, OUTPUT);
    pinMode(LEDLevel_100, OUTPUT);

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

void getData() 
{
    if ( radio.available() ) 
    {
        radio.read( &dataReceived, sizeof(dataReceived) );
        newData = true;
        
    }

}


//**************************************************** RELAY and LEVEL INDICATOR  FUNCTION *************************************************************>>>>>
void MotorOnOff()
{
  
  char x= dataReceived;
  if(x == '4')          
  { 
    Serial.println(" TANK IS FULL!!! "); 
    digitalWrite(Relay, LOW); 
    digitalWrite(LEDLevel_100, HIGH);   
  }
  if(x == '3')          
  { 
    Serial.println(" TANK IS at 75% Water Level "); 
    digitalWrite(Relay, LOW); 
    digitalWrite(LEDLevel_75, HIGH);   
  }
  else if(x == '2')          
  { 
    Serial.println(" TANK IS at level 50% Water Level "); 
    digitalWrite(Relay, LOW); 
    digitalWrite(LEDLevel_50, HIGH);   
  }
  else if(x== '1' )          
  { 
    Serial.println(" TANK IS at level 25% Water Level"); 
    digitalWrite(Relay, LOW); 
    digitalWrite(LEDLevel_25, HIGH);   
  }

  else if(x == '0' )    
  {
    Serial.println(" TANK IS at level 0% Water Level, STARTING THE MOTOR NOW ====>");
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
//<<<<<<<<*********************************************************************************************************************************//



//=========================================================== END OF CODE HERE ========================================================//
