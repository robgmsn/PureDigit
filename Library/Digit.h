/*
  GMSN! Pure Digit v0.1
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
  #include <Digit.h>

  Digit digit;

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


#ifndef Digit_h
#define Digit_h

#include <Arduino.h>
#include <SPI.h>

class Digit {
public:
  Digit();
  void begin();
  int encodeVal(int val);
  void displayLED(int digit, byte mode, bool dp);
  void dacWrite(int value);
  int adcRead(byte channel);

private:
  int encAVal, encALast, encBVal;
};

#endif
