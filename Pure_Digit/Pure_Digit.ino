

/*
  GMSN! Pure Digit v0.1
  28th July 2018
  cc-by 4.0
  Rob Spencer

  Write various codes
  Enable EEPROM
  Trimming Inputs and Outputs and test for variation between modules

  Noise
  Clk Divider
  Micro Tuner
  Bit Crusher
  Drums
  Rhythm Player
  Swing Machine
  LFO
  Step LFO
  DC Level
  Module Testing Functions
  Frequency Counter
  DC Meter

*/

#include <Digit.h>

Digit digit;

//Setup variables
int encPos = 1;
int cvIn, cvOut;


void setup() {

 digit.begin();

}

void loop() {

   encPos = digit.encodeVal(encPos);
   digit.displayLED(encPos, 1, 0);
   digit.dacWrite(random(4095));
   if (encPos < 1) {
    encPos = 1;
   }
   delay(encPos);

}
