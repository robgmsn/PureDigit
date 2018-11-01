/*
  GMSN! Pure Digit v1
  28th July 2018
  cc-by 4.0
  Rob Spencer
  gmsn.co.uk
  forum.gmsn.co.uk
  Open Source Synth Designs.

  This library is used to control the GMSN! Pure Digit. A small programmable utility module which accepts +/- 10V in,
  does some digital processing, and outputs a +/-10V signal. It has a 2 Channel 12bit ADC In, 12bit DAC Out, a 24
  position rotary encoder with switch and a 7 segment LED display.

  Examples include:   Noise, Clock Divider, Micro Tuner, Bit Crusher, Drums, Rhythm Player, Swing Machine, LFO, Step LFO,
  DC Level, Module Testing Functions, Frequency Counter, DC Meter

  See README.md for details on the available methods and calibrate routine.

  Example Sketch:

  <code>
  #include <PureDigit.h>

  PureDigit digit;

  //Setup variables
  int encPos = 1;
  int cvIn, cvOut;


  void setup() {

   digit.begin();

  }

  void loop() {

     encPos = digit.encodeVal(encPos);  //Increment or decrement the varible encPos
     digit.displayLED(encPos, 2, 0);    //Display the value of encPos as a Positional Representation with no decimal point
     digit.dacWrite(random(4095));      //Output a random value between 0 and 4095, which is analogue +/- 10V
     delay(encPos);                     //Delay for encPos milliseconds.

  }
  </code>
*/

#include <Arduino.h>
#include <SPI.h>
#include "PureDigit.h"
#include <EEPROM.h>

//Setup Digit
#define digitA 0 //30
#define digitB A5 //28
#define digitC A4 //27
#define digitD A3 //26
#define digitE 4 //2
#define digitF 2 //32
#define digitG 1 //31
#define digitDP 3 //1

//Setup Encoder
//SW
#define sw 5 //9

//Encoder
#define encA 7 //11
#define encB 6 //10


//Setup SPI Bus
#define csADC A1 //9 //13 or 24
#define csDAC 10 //14

boolean doCalibrate=true;

PureDigit::PureDigit() {

        int encAVal, encALast, encBVal = 0;
        float Ge1 = 0;
        float Ge2 = 0;
        float Oe1 = 0;
        float Oe2 = 0;
}

void PureDigit::dontCalibrate(){
     doCalibrate=false;
}

void PureDigit::begin() {

        //Setup Digit Pins
        pinMode(digitA, OUTPUT);
        pinMode(digitB, OUTPUT);
        pinMode(digitC, OUTPUT);
        pinMode(digitD, OUTPUT);
        pinMode(digitE, OUTPUT);
        pinMode(digitF, OUTPUT);
        pinMode(digitG, OUTPUT);
        pinMode(digitDP, OUTPUT);

        digitalWrite(digitA, HIGH);
        digitalWrite(digitB, HIGH);
        digitalWrite(digitC, HIGH);
        digitalWrite(digitD, HIGH);
        digitalWrite(digitE, HIGH);
        digitalWrite(digitF, HIGH);
        digitalWrite(digitG, HIGH);
        digitalWrite(digitDP, HIGH);

        //Setup Encoder Pins
        pinMode(sw, INPUT);
        pinMode(encA, INPUT);
        pinMode(encB, INPUT);
        digitalWrite(encA, HIGH);
        digitalWrite(encB, HIGH);

        //Setup DAC/ADC Pins
        pinMode(csADC, OUTPUT);
        pinMode(csDAC, OUTPUT);

        digitalWrite(csADC, HIGH);
        digitalWrite(csDAC, HIGH);
        SPI.begin();

        byte calFlag = EEPROM.read(0);
        if (doCalibrate==true){
        if (calFlag == 0) {
                EEPROM.get(1, Ge1);
                EEPROM.get(5, Ge2);
                EEPROM.get(9, Oe1);
                EEPROM.get(13, Oe2);
        } else {
                for (int x = 0; x < 10; x++) {
                        displayLED(8, 1, 0);
                        delay(200);
                        displayOff();
                        delay(100);
                }
                calibrate();
        }
        }else{
            Oe1=0.0;
            Ge1=1.0;
        }
}

int PureDigit::encodeVal(int val) {
        encAVal = digitalRead(encA);
        encBVal = digitalRead(encB);
        if (encAVal != encALast) {
                if (encAVal == encBVal) {
                        val--;
                } else {
                        val++;
                }
                encALast = encAVal;
        }
        return val;
}

bool PureDigit::getSwitchState() {
        bool swState = !digitalRead(sw);
        return swState;
}

void PureDigit::displayOff() {
        digitalWrite(digitA, HIGH);
        digitalWrite(digitB, HIGH);
        digitalWrite(digitC, HIGH);
        digitalWrite(digitD, HIGH);
        digitalWrite(digitE, HIGH);
        digitalWrite(digitF, HIGH);
        digitalWrite(digitG, HIGH);
}

void PureDigit::displayLEDChar(char c, bool dp) {
        if (dp == 1) {
                digitalWrite(digitDP, LOW);
        } else {
                digitalWrite(digitDP, HIGH);
        }

        switch (c) {
          case '-':
                digitalWrite(digitA, HIGH);
                digitalWrite(digitB, LOW);
                digitalWrite(digitC, HIGH);
                digitalWrite(digitD, HIGH);
                digitalWrite(digitE, HIGH);
                digitalWrite(digitF, HIGH);
                digitalWrite(digitG, HIGH);
                break;

          case 'M':
                digitalWrite(digitA, LOW);
                digitalWrite(digitB, HIGH);
                digitalWrite(digitC, LOW);
                digitalWrite(digitD, HIGH);
                digitalWrite(digitE, LOW);
                digitalWrite(digitF, LOW);
                digitalWrite(digitG, LOW);
                break;

          case 'm':
                digitalWrite(digitA, HIGH);
                digitalWrite(digitB, LOW);
                digitalWrite(digitC, LOW);
                digitalWrite(digitD, HIGH);
                digitalWrite(digitE, LOW);
                digitalWrite(digitF, HIGH);
                digitalWrite(digitG, HIGH);
                break;

          case 'C':
                digitalWrite(digitA, LOW);
                digitalWrite(digitB, HIGH);
                digitalWrite(digitC, LOW);
                digitalWrite(digitD, LOW);
                digitalWrite(digitE, HIGH);
                digitalWrite(digitF, HIGH);
                digitalWrite(digitG, LOW);
                break;

          case 'P':
                digitalWrite(digitA, LOW);
                digitalWrite(digitB, LOW);
                digitalWrite(digitC, LOW);
                digitalWrite(digitD, HIGH);
                digitalWrite(digitE, HIGH);
                digitalWrite(digitF, LOW);
                digitalWrite(digitG, LOW);
                break;

          case 'b': 
                digitalWrite(digitA, LOW);
                digitalWrite(digitB, LOW);
                digitalWrite(digitC, LOW);
                digitalWrite(digitD, LOW);
                digitalWrite(digitE, LOW);
                digitalWrite(digitF, HIGH);
                digitalWrite(digitG, HIGH);
                break;

          case 'H':
                digitalWrite(digitA, LOW);
                digitalWrite(digitB, LOW);
                digitalWrite(digitC, LOW);
                digitalWrite(digitD, HIGH);
                digitalWrite(digitE, LOW);
                digitalWrite(digitF, LOW);
                digitalWrite(digitG, HIGH);
                break;
        }
}

void PureDigit::displayLED(int digit, byte mode, bool dp) {
        if (dp == 1) {
                digitalWrite(digitDP, LOW);
        } else {
                digitalWrite(digitDP, HIGH);
        }
        if (mode == 1) {
                switch (digit) {
                case 0:
                        digitalWrite(digitA, LOW);
                        digitalWrite(digitB, HIGH);
                        digitalWrite(digitC, LOW);
                        digitalWrite(digitD, LOW);
                        digitalWrite(digitE, LOW);
                        digitalWrite(digitF, LOW);
                        digitalWrite(digitG, LOW);
                        break;
                case 1:
                        digitalWrite(digitA, HIGH);
                        digitalWrite(digitB, HIGH);
                        digitalWrite(digitC, HIGH);
                        digitalWrite(digitD, HIGH);
                        digitalWrite(digitE, LOW);
                        digitalWrite(digitF, LOW);
                        digitalWrite(digitG, HIGH);
                        break;
                case 2:
                        digitalWrite(digitA, HIGH);
                        digitalWrite(digitB, LOW);
                        digitalWrite(digitC, LOW);
                        digitalWrite(digitD, LOW);
                        digitalWrite(digitE, HIGH);
                        digitalWrite(digitF, LOW);
                        digitalWrite(digitG, LOW);
                        break;
                case 3:
                        digitalWrite(digitA, HIGH);
                        digitalWrite(digitB, LOW);
                        digitalWrite(digitC, HIGH);
                        digitalWrite(digitD, LOW);
                        digitalWrite(digitE, LOW);
                        digitalWrite(digitF, LOW);
                        digitalWrite(digitG, LOW);
                        break;
                case 4:
                        digitalWrite(digitA, LOW);
                        digitalWrite(digitB, LOW);
                        digitalWrite(digitC, HIGH);
                        digitalWrite(digitD, HIGH);
                        digitalWrite(digitE, LOW);
                        digitalWrite(digitF, LOW);
                        digitalWrite(digitG, HIGH);
                        break;
                case 5:
                        digitalWrite(digitA, LOW);
                        digitalWrite(digitB, LOW);
                        digitalWrite(digitC, HIGH);
                        digitalWrite(digitD, LOW);
                        digitalWrite(digitE, LOW);
                        digitalWrite(digitF, HIGH);
                        digitalWrite(digitG, LOW);
                        break;
                case 6:
                        digitalWrite(digitA, LOW);
                        digitalWrite(digitB, LOW);
                        digitalWrite(digitC, LOW);
                        digitalWrite(digitD, LOW);
                        digitalWrite(digitE, LOW);
                        digitalWrite(digitF, HIGH);
                        digitalWrite(digitG, HIGH);
                        break;
                case 7:
                        digitalWrite(digitA, HIGH);
                        digitalWrite(digitB, HIGH);
                        digitalWrite(digitC, HIGH);
                        digitalWrite(digitD, HIGH);
                        digitalWrite(digitE, LOW);
                        digitalWrite(digitG, LOW);
                        digitalWrite(digitF, LOW);
                        break;
                case 8:
                        digitalWrite(digitA, LOW);
                        digitalWrite(digitB, LOW);
                        digitalWrite(digitC, LOW);
                        digitalWrite(digitD, LOW);
                        digitalWrite(digitE, LOW);
                        digitalWrite(digitF, LOW);
                        digitalWrite(digitG, LOW);
                        break;
                case 9:
                        digitalWrite(digitA, LOW);
                        digitalWrite(digitB, LOW);
                        digitalWrite(digitC, HIGH);
                        digitalWrite(digitD, HIGH);
                        digitalWrite(digitE, LOW);
                        digitalWrite(digitF, LOW);
                        digitalWrite(digitG, LOW);
                        break;
                }
        } else if (mode = 2) {

                digit = digit % 12;

                switch (digit) {
                case 0:
                        digitalWrite(digitA, HIGH);
                        digitalWrite(digitB, HIGH);
                        digitalWrite(digitC, HIGH);
                        digitalWrite(digitD, LOW);
                        digitalWrite(digitE, HIGH);
                        digitalWrite(digitF, HIGH);
                        digitalWrite(digitG, HIGH);
                        break;
                case 1:
                        digitalWrite(digitA, HIGH);
                        digitalWrite(digitB, HIGH);
                        digitalWrite(digitC, LOW);
                        digitalWrite(digitD, LOW);
                        digitalWrite(digitE, HIGH);
                        digitalWrite(digitF, HIGH);
                        digitalWrite(digitG, HIGH);
                        break;
                case 2:
                        digitalWrite(digitA, HIGH);
                        digitalWrite(digitB, HIGH);
                        digitalWrite(digitC, LOW);
                        digitalWrite(digitD, HIGH);
                        digitalWrite(digitE, HIGH);
                        digitalWrite(digitF, HIGH);
                        digitalWrite(digitG, HIGH);
                        break;
                case 3:
                        digitalWrite(digitA, LOW);
                        digitalWrite(digitB, HIGH);
                        digitalWrite(digitC, LOW);
                        digitalWrite(digitD, HIGH);
                        digitalWrite(digitE, HIGH);
                        digitalWrite(digitF, HIGH);
                        digitalWrite(digitG, HIGH);
                        break;
                case 4:
                        digitalWrite(digitA, LOW);
                        digitalWrite(digitB, HIGH);
                        digitalWrite(digitC, HIGH);
                        digitalWrite(digitD, HIGH);
                        digitalWrite(digitE, HIGH);
                        digitalWrite(digitF, HIGH);
                        digitalWrite(digitG, HIGH);
                        break;
                case 5:
                        digitalWrite(digitA, LOW);
                        digitalWrite(digitB, HIGH);
                        digitalWrite(digitC, HIGH);
                        digitalWrite(digitD, HIGH);
                        digitalWrite(digitE, HIGH);
                        digitalWrite(digitF, HIGH);
                        digitalWrite(digitG, LOW);
                        break;
                case 6:
                        digitalWrite(digitA, HIGH);
                        digitalWrite(digitB, HIGH);
                        digitalWrite(digitC, HIGH);
                        digitalWrite(digitD, HIGH);
                        digitalWrite(digitE, HIGH);
                        digitalWrite(digitF, HIGH);
                        digitalWrite(digitG, LOW);
                        break;
                case 7:
                        digitalWrite(digitA, HIGH);
                        digitalWrite(digitB, HIGH);
                        digitalWrite(digitC, HIGH);
                        digitalWrite(digitD, HIGH);
                        digitalWrite(digitE, HIGH);
                        digitalWrite(digitF, LOW);
                        digitalWrite(digitG, LOW);
                        break;
                case 8:
                        digitalWrite(digitA, HIGH);
                        digitalWrite(digitB, HIGH);
                        digitalWrite(digitC, HIGH);
                        digitalWrite(digitD, HIGH);
                        digitalWrite(digitE, HIGH);
                        digitalWrite(digitF, LOW);
                        digitalWrite(digitG, HIGH);
                        break;
                case 9:
                        digitalWrite(digitA, HIGH);
                        digitalWrite(digitB, HIGH);
                        digitalWrite(digitC, HIGH);
                        digitalWrite(digitD, HIGH);
                        digitalWrite(digitE, LOW);
                        digitalWrite(digitF, LOW);
                        digitalWrite(digitG, HIGH);
                        break;
                case 10:
                        digitalWrite(digitA, HIGH);
                        digitalWrite(digitB, HIGH);
                        digitalWrite(digitC, HIGH);
                        digitalWrite(digitD, HIGH);
                        digitalWrite(digitE, LOW);
                        digitalWrite(digitF, HIGH);
                        digitalWrite(digitG, HIGH);
                        break;
                case 11:
                        digitalWrite(digitA, HIGH);
                        digitalWrite(digitB, HIGH);
                        digitalWrite(digitC, HIGH);
                        digitalWrite(digitD, LOW);
                        digitalWrite(digitE, LOW);
                        digitalWrite(digitF, HIGH);
                        digitalWrite(digitG, HIGH);
                        break;
                }
        }
}

void PureDigit::dacWrite(int value) {
        digitalWrite(csDAC, LOW);
        constrain(value, 0, 4095);
        value = value + 12288;
        SPI.transfer16(value);
        digitalWrite(csDAC, HIGH);
}

void PureDigit::dacWriteCal(int value) {
        value = float(value);
        value = (value - Oe1) / Ge1;
        value = constrain(value, 0, 4095);
        dacWrite(value);
}

int PureDigit::adcRead(byte channel) {

        int commandbits = 40960;
        if (channel == 2) {
                commandbits = commandbits + 16384;
        }
        unsigned int result;
        digitalWrite(csADC, LOW);
        SPI.transfer(1);
        result = SPI.transfer16(commandbits);
        result = result & 4095;
        digitalWrite(csADC, HIGH);
        return result;
}

void PureDigit::calibrate(){
        int n1, n2, i;
        int stage = 1;
        bool swState, lastSwState;

//loop
        while (stage < 6) {
                switch (stage) {
                case 1:
                        //Connect Output to Input 1 & Wait
                        flashLED(1, 1, 500);
                        break;

                case 2:
                        //Output 512 and read ADC
                        dacWrite(512);
                        delay(50);
                        n1 = adcRead(1);
                        delay(50);

                        //Output 3584 and read ADC
                        dacWrite(3584);
                        delay(50);
                        n2 = adcRead(1);

                        //Calculate Ge1 = (n2 - n1)/(3584 - 512)
                        Ge1 = (n2 - n1) / (3584.0 - 512.0);

                        //Calculate Oe1 = n1 - (Ge * 512)
                        Oe1 = n1 - (Ge1 * 512.0);

                        flashLED(1, 10, 50);
                        stage = 3;
                        break;

                case 3:
                        //Connect Output to Input 2 & Wait
                        flashLED(2, 1, 500);
                        break;

                case 4:
                        //Output 512 and read ADC
                        dacWrite(512);
                        delay(50);
                        n1 = adcRead(2);
                        delay(50);

                        //Output 3584 and read ADC
                        dacWrite(3584);
                        delay(50);
                        n2 = adcRead(2);

                        //Calculate Ge1 = (n2 - n1)/(3584 - 512)
                        Ge2 = (n2 - n1) / (3584.0 - 512.0);

                        //Calculate Oe1 = n1 - (Ge * 512)
                        Oe2 = n1 - (Ge2 * 512.0);

                        flashLED(2, 10, 50);
                        stage = 5;
                        break;

                case 5:
                        flashLED(9, 20, 5);
                        flashLED(8, 20, 5);
                        flashLED(7, 20, 5);
                        flashLED(6, 20, 5);
                        flashLED(5, 10, 5);
                        flashLED(4, 20, 5);
                        flashLED(3, 20, 5);
                        flashLED(2, 20, 5);
                        flashLED(1, 20, 5);
                        displayLED(8, 1, 0);
                        delay(200);
                        displayOff();
                        delay(100);
                        displayLED(8, 1, 0);
                        delay(200);
                        displayOff();
                        delay(100);
                        displayLED(8, 1, 0);
                        delay(200);
                        displayOff();
                        delay(100);
                        displayLED(8, 1, 0);
                        delay(200);
                        displayOff();

                        //Store Ge1, Ge2, Oe1 and Oe2
                        EEPROM.put(1, Ge1);
                        EEPROM.put(5, Ge2);
                        EEPROM.put(9, Oe1);
                        EEPROM.put(13, Oe2);
                        EEPROM.write(0, 0);
                        delay(100);
                        displayLED(8, 1, 0);
                        delay(200);
                        displayOff();
                        delay(100);
                        stage = 6;
                        break;


                }

//Poll Button

                swState = getSwitchState();
                if (swState == 1 && swState != lastSwState) {
                        switch (stage) {
                        case 1:
                                stage = 2;
                                break;
                        case 3:
                                stage = 4;
                                break;
                        case 6:
                                stage = 7;
                                break;
                        case 8:
                                stage = 9;
                                break;
                        }

                        lastSwState = 1;
                } else {
                        lastSwState = 0;
                }
        }
}

void PureDigit::flashLED(int number, int repeats, int delays) {
        int i = 0;
        while ( i < repeats) {
                displayLED(number, 1, 1);
                delay(delays);
                displayLED(number, 1, 0);
                delay(delays);
                i++;
        }
}
