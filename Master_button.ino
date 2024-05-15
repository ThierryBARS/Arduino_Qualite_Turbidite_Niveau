
/*
 * FM.h
 * A library for SeeedStudio Grove FM
 *
 * Copyright (c) 2012 seeed technology inc.
 * Website    : www.seeed.cc
 * Author     : Steve Chang
 * Create Time: JULY 2014
 * Change Log : Modified by loovee 2013-10-29  ,   Modified by jacob yan 2014-7-29 
 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <SoftwareSerial.h>                         // Software Serial Port

#define TxD      7   //1
#define RxD      6   //0
#define LED     10
#define PINBUTTON   9                               // pin of button

#define DEBUG_ENABLED  1

const char MSG_ON[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char MSG_OFF[] = "ZYXWVUTSRQPONMLKJHGFEDCBA";

SoftwareSerial blueToothSerial(RxD,TxD);

void setup()
{
    Serial.begin(9600);
    pinMode(RxD, INPUT);
    pinMode(TxD, OUTPUT);
    pinMode(PINBUTTON, INPUT);
    pinMode(LED, OUTPUT);
    
    setupBlueToothConnection();
    //wait 1s and flush the serial buffer
    delay(1000);
    Serial.flush();
    blueToothSerial.flush();
}

void loop()
{
  String ChaineRecue;

    static unsigned char state = 0;             // led off
    
    if(digitalRead(PINBUTTON))
    {
        state = 1-state;
        
       // Serial.println("button on");
        
        blueToothSerial.write("button arduino on");
        delay(10);
        while(digitalRead(PINBUTTON))       // until button release
        {
            delay(10);
        }
        
       // Serial.println("button off");
        
        blueToothSerial.write("button arduino off");

    }

    ChaineRecue = blueToothSerial.readString();
    if (ChaineRecue == MSG_OFF) {
      Serial.println("MIT app OFF");
      digitalWrite(LED,LOW);
    }
    if (ChaineRecue == MSG_ON) {
      Serial.println("MIT app ON");  
      digitalWrite(LED,HIGH);
    }
 
}
//-------------------------------------------------------------------------
// initilizing bluetooth connction
//-------------------------------------------------------------------------
void setupBlueToothConnection()
{
  blueToothSerial.begin(9600);  
	blueToothSerial.print("AT"); delay(400); 
	blueToothSerial.print("AT+DEFAULT"); delay(2000);         // Restore all setup value to factory setup
	blueToothSerial.print("AT+NAMESeeedMaster"); delay(400);  // set the bluetooth name as "SeeedMaster" ,the length of bluetooth name must less than 12 characters.
	blueToothSerial.print("AT+ROLES"); delay(400);            // set the bluetooth work in master mode
	blueToothSerial.print("AT+AUTH1"); delay(400);    
	blueToothSerial.print("AT+CLEAR"); delay(400);            // Clear connected device mac address
  blueToothSerial.flush();
}