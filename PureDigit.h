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
  getSwitchState() - Returns a bool of the switch state, either on or off. It's a good
                     idea to call this from the main loop, so it's continuously polling.
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


#ifndef PureDigit_h
#define PureDigit_h

#include <Arduino.h>
#include <SPI.h>

class PureDigit {
public:
  PureDigit();
  void dontCalibrate();
  void begin();
  void calibrate();
  int encodeVal(int val);
  bool getSwitchState();
  void displayRandom();
  void displayLED(int digit, byte mode, bool dp);
  void displayOff();
  void dacWrite(int value);
  void dacWriteCal(int value);
  int adcRead(byte channel);

private:
  void flashLED(int number, int repeats, int delays);
  int encAVal, encALast, encBVal;
  float Ge1, Ge2, Oe1, Oe2;
};

#endif
