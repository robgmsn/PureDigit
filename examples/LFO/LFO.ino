/*
  GMSN! Pure Digit
  28th July 2018
  cc-by 4.0
  Rob Spencer

  
  LFO 
  This code by Andy Cobley
  Rate is controlled by CV1, Shape by CV2. The shape changes the duty cycle, for a Square wave this gives PWM, Triangle from +saw to triangle to -Saw. There others similarly.

Encoder position

1: Sine
2: Square
3: Triangle
4: Sine based on Triangle (can produce a nice output)

5,6,7,8 the same but slower cycles
9 Same as 4 but even slower.
*/

#include <PureDigit.h>

PureDigit digit;
float rad = 0;
const float Pi = 3.14159;
float step = Pi / (256.0);
int Value = 0;
float Range = 4094.0;

int Shape = 128.0;
float Step1 = 2 * Pi / Shape;
float Step2 = 2 * Pi / (512 - Shape);
float twoPi = 2.0 * Pi;
float Rate = 0;
float RateMultiplier = 1;
//Setup variables
int encPos = 1;
int cvIn, cvOut;

void writeSin(float Value) {
  int Out = (int)(Range * Value);
  digit.dacWrite(Out);

}

void writeSquare(float Value) {
  int Out = (int)(Range * Value);
  digit.dacWrite(Out);

}

void writeTriangle(float Value) {
  int Out = (int)(Range * Value);
  digit.dacWrite(Out);

}

void writeCos(float Value) {
  int Out = (int)(Range * Value);
  digit.dacWrite(Out);

}

float dv=0; //These nembers where used for offsets if the input resistors are not correct
float dv2=0;
float newRange=4096-dv;
float newRange2=4096-dv2;
void setup() {
  digit.dontCalibrate();
  digit.begin();
  int i=0;

}


float divisor=0.0;
void setShape() {
  int an1 = digit.adcRead(1)-dv+RateMultiplier; //Rate

  if (an1<=0){
    an1=1;
  }
  int an2 = digit.adcRead(2)-dv2; //Shape
  if (an2<=0){
    an2=1;
  }
  float Percent=an2/newRange2;
  float Percent2=1.0-Percent;
  

  Step1 = twoPi / (an1*Percent);
  Step2 = twoPi/(an1*Percent2);
}

float Wave = 0;
int Square = 0;
float Triangle = 0;
float Cos = 0;
float lastRad;

int lastEnc = -1;
void loop() {
  // put your main code here, to run repeatedly:
  encPos = digit.encodeVal(encPos);
  if (encPos != lastEnc) {
    digit.displayLED(encPos, 1, 0);
    lastEnc = encPos;
  }
  
  switch (encPos) {
    case 1:
    case 2:
    case 3:
    case 4: RateMultiplier = 1; break;
    case 5:
    case 6:
    case 7:
    case 8: RateMultiplier = newRange; break;
    case 9: RateMultiplier = 2*newRange; break;
    default: RateMultiplier = 1; break;

  }
  setShape();
  rad = rad + Square * Step1 + (1 - Square) * Step2;
  if (rad > twoPi)
    rad = 0.0;

  Square = (int)(rad / Pi);
  Triangle = (-1 * (Pi / 2.0) + Square * Pi + (0 - Square) * (rad - (Square * Pi)) + (1 - Square) * (rad - Square * Pi));

  switch (encPos) {
    case 0:
    case 1:
    case 5:
      Wave = 0.5 * sin(rad) + 0.5;
      writeSin(Wave);
      break;
    case 2:
    case 6:
      writeSquare(Square);
      break;
    case 3:
    case 7:
      writeTriangle(0.24 * Triangle + 0.5);
      break;
    case 4:
    case 8:
    case 9:
      Cos = sin(Triangle);
      writeCos(0.5 * Cos + 0.5);
    default:
      break;
  }


}
