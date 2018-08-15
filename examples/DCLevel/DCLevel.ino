/*
  GMSN! Pure Digit
  28th July 2018
  cc-by 4.0
  Rob Spencer

  DC Level
  Provides a DC Voltage from 9 to -9V at 1V increments.
*/

#include <PureDigit.h>

PureDigit digit;

//Setup variables
int encPos = 1;
int cvIn, cvOut;
bool dp = 0;
int dcLookUp[19] = {3873, 3669, 3464, 3263,
                    3059, 2857, 2654, 2450,
                    2248, 2040, 1839, 1638,
                    1435, 1234, 1031, 830,
                    628, 428, 227
                   };
int lookupIndex;
void setup() {

  digit.begin();

}

void loop() {

  encPos = digit.encodeVal(encPos);
  encPos = constrain(encPos, -9, 9);

  if (encPos < 0) {
    digit.displayLED(abs(encPos), 1, 1);
  } else {
    digit.displayLED(encPos, 1, 0);
  }

  lookupIndex = encPos + 9;
  digit.dacWrite(dcLookUp[lookupIndex]);

}
