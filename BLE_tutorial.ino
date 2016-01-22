//This will run the tutorial laid out at
// http://www.raywenderlich.com/85900/arduino-tutorial-integrating-bluetooth-le-ios-swift


#include <SPI.h>
#include "Adafruit_BLE_UART.h"

//variables
String incoming = "";      //Creates a string that will hold incoming messages

// Connect CLK/MISO/MOSI to hardware SPI
// e.g. On UNO & compatible: CLK = 13, MISO = 12, MOSI = 11
#define ADAFRUITBLE_REQ 10
#define ADAFRUITBLE_RDY 2     // This should be an interrupt pin, on Uno thats #2 or #3
#define ADAFRUITBLE_RST 9
#define SPEAKER 6             //Speaker needs to be on a PWM pin, here I chose pin 6
#define FREQ 500              //Frequency in Hz of the tone played

Adafruit_BLE_UART BTLEserial = Adafruit_BLE_UART(ADAFRUITBLE_REQ, ADAFRUITBLE_RDY, ADAFRUITBLE_RST);
/**************************************************************************/
/*
    Configure the Arduino and start advertising with the radio
*/
/**************************************************************************/
void setup(void)
{ 
  Serial.begin(9600);
  while(!Serial); // Leonardo/Micro should wait for serial init
  Serial.println(F("Apollo Hearing beep on command demo"));
  tone(SPEAKER, FREQ, 1000);   //Plays a brief tone as a test of the system when it starts up

  //BTLEserial.setDeviceName("NEWNAME"); /* 7 characters max! */

  BTLEserial.begin();
}

/**************************************************************************/
/*!
    Constantly checks for new events on the nRF8001
*/
/**************************************************************************/
aci_evt_opcode_t laststatus = ACI_EVT_DISCONNECTED;

void loop()
{
  // Tell the nRF8001 to do whatever it should be working on.
  BTLEserial.pollACI();

  // Ask what is our current status
  aci_evt_opcode_t status = BTLEserial.getState();
  // If the status changed....
  if (status != laststatus) {
    // print it out!
    if (status == ACI_EVT_DEVICE_STARTED) {
        Serial.println(F("* Advertising started"));
    }
    if (status == ACI_EVT_CONNECTED) {
        Serial.println(F("* Connected!"));
    }
    if (status == ACI_EVT_DISCONNECTED) {
        Serial.println(F("* Disconnected or advertising timed out"));
    }
    // OK set the last status change to this one
    laststatus = status;
  }

  if (status == ACI_EVT_CONNECTED) {
    // Lets see if there's any data for us!
    if (BTLEserial.available()) {
      incoming = "";        //Clear out the string
      Serial.print("* "); Serial.print(BTLEserial.available()); Serial.println(F(" bytes available from BTLE"));
      // OK while we still have something to read, get a character and print it out
      while (BTLEserial.available()) {
        char c = BTLEserial.read();
        incoming.concat(c);           //Build up the string of characters
      }
     Serial.println(incoming);
    }

   if (incoming == "beep") {
      tone(SPEAKER, FREQ, 100);   //Plays a brief tone if the phone tells it to
   }
    

    // Next up, see if we have any data to get from the Serial console

    if (Serial.available()) {
      // Read a line from Serial
      Serial.setTimeout(100); // 100 millisecond timeout
      String s = Serial.readString();

      // We need to convert the line to bytes, no more than 20 at this time
      uint8_t sendbuffer[20];
      s.getBytes(sendbuffer, 20);
      char sendbuffersize = min(20, s.length());

      Serial.print(F("\n* Sending -> \"")); Serial.print((char *)sendbuffer); Serial.println("\"");

      // write the data
      BTLEserial.write(sendbuffer, sendbuffersize);
    }

    //clear the incoming message
      incoming = "";
  }
}
