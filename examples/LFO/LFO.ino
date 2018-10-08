/*
  GMSN! Pure Digit
  28th July 2018
  cc-by 4.0
  Rob Spencer

  
  LFO 
  This code by Andy Cobley
  Rate is controlled by CV1, Shape by CV2. The shape changes the duty cycle, for a Square wave this gives PWM, Triangle from +saw to triangle to -Saw. There others similarly.

Encoder position

0: Sine
1: Square
2: Triangle
3: Sine based on Triangle (can produce a nice output)

4,5,6,7 the same but slower cycles
Higher encoder positons give slower cycles still .
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
int encPos = 0;
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
  digit.displayLED(encPos, 1, 0);
}


float divisor=0.0;
void setShape() {
  int an1 = digit.adcRead(1)+RateMultiplier+100; //Rate

  if (an1<=0){
    an1=1;
  }
  int an2 = digit.adcRead(2); //Shape
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
    if (encPos <0)
       encPos=19;
    if (encPos>19)
       encPos=0;
    if (encPos <10)
       digit.displayLED(encPos, 1, 0);
    else 
       digit.displayLED(encPos-10, 1, 1);
    lastEnc = encPos;
  }
  
  switch (encPos) {
    case 0:
    case 1:
    case 2:
    case 3: RateMultiplier = 0; break;
    case 4:
    case 5:
    case 6:
    case 7: RateMultiplier = newRange; break;
    case 8: 
    case 9:
    case 10:
    case 11:RateMultiplier = 2*newRange; break;
    case 12: 
    case 13:
    case 14:
    case 15:RateMultiplier = 3*newRange; break;
    case 16: 
    case 17:
    case 18:
    case 19:RateMultiplier = 4*newRange; break;
    default: RateMultiplier = 0; break;

  }
  setShape();
  rad = rad + Square * Step1 + (1 - Square) * Step2;
  if (rad > twoPi)
    rad = 0.0;

  Square = (int)(rad / Pi);
  Triangle = (-1 * (Pi / 2.0) + Square * Pi + (0 - Square) * (rad - (Square * Pi)) + (1 - Square) * (rad - Square * Pi));

  switch (encPos) {
    case 0:
    case 4:
    case 8:
    case 12:
    case 16:
      Wave = 0.5 * sin(rad) + 0.5;
      writeSin(Wave);
      break;
   case 1:
    case 5:
    case 9:
    case 13:
    case 17:
      writeSquare(Square);
      break;
   case 2:
    case 6:
    case 10:
    case 14:
    case 18:
      writeTriangle(0.24 * Triangle + 0.5);
      break;
   case 3:
    case 7:
    case 11:
    case 15:
    case 19:
      Cos = sin(Triangle);
      writeCos(0.5 * Cos + 0.5);
    default:
      break;
  }


}
