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

  Methods:
  encodeVal(value) - Call this to increment or decrement a value with the rotary encoder.
  display(digit, mode, dp) - The display has two modes:
                                1) Standard 0 - 9 digital digit.
                                2) Positional representation with 12 values. Where the knob is pointing!
                                   This uses the result of a modulo calculation so the value can be greater
                                   than 12 and it will keep rotating. It can't go less than zero though.
  dacWrite - Write a value to the DAC. Yes, it's got a 20V P2P voltage!! :D
             0    =   +10V
             2048 =   0V
             4095 =   -10V

  adcRead(channel) - Read a 20V P2P from the DAC, channel 1 or 2. Voltages as above.

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
#define csADC 9 //13
#define csDAC 10 //14

PureDigit::PureDigit() {

  int encAVal, encALast, encBVal = 0;
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
  byte data = value >> 8;
  data = data & B00001111;
  data = data | B00110000;
  SPI.transfer(data);

  data = value;
  SPI.transfer(data);

  digitalWrite(csDAC, HIGH);
}

int PureDigit::adcRead(byte channel) {

  byte commandbits = B00001101;          //command bits - 0000, start, mode, chn, MSBF
  unsigned int b1 = 0;                   // get the return var's ready
  unsigned int b2 = 0;
  commandbits |= ((channel - 1) << 1);   // update the command bit to select either ch 1 or 2
  digitalWrite(csADC, LOW);
  SPI.transfer(commandbits);             // send out the command bits
  const int hi = SPI.transfer(b1);       // read back the result high byte
  const int lo = SPI.transfer(b2);       // then the low byte
  digitalWrite(csADC, HIGH);
  b1 = lo + (hi << 8);                   // assemble the two bytes into a word
  //return b1;
  return (b1 >> 3);                      // We have got a 12bit answer but strip LSB's if
  // required >>4 ==10 bit (0->1024), >>2 ==12bit (0->4096)
}
