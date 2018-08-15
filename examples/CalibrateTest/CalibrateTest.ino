/*
  GMSN! Pure Digit
  28th July 2018
  cc-by 4.0
  Rob Spencer

  Empty Skietch
  Bare minimum needed for Pure Digit
*/

#include <PureDigit.h>

PureDigit digit;

//Setup variables
bool swState, lastSwState;
int stage = 1;

void setup() {

  digit.begin();
  digit.displayLED(0, 1, 0);

}

void loop() {

  swState = digit.getSwitchState();

  if (swState == 1) {
  digit.dacWriteCal(digit.adcRead(2));
  digit.displayLED(1,1,0);
  }
  if (swState == 0) {
  digit.dacWrite(digit.adcRead(2));
  digit.displayLED(0,1,0);
  }

}
