

/*
  GMSN! Pure Digit
  28th July 2018
  cc-by 4.0
  Rob Spencer

  RandomSequencer
  Writes random values to the DAC. Encoder controls the rate.
*/

#include <PureDigit.h>

PureDigit digit;

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
