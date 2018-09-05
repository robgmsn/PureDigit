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
//Setup variables

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

void setup() {
  digit.dontCalibrate();
  digit.begin();

}

void setShape() {
  int an1=512; //Rate
  int an2=512; //Shape
  Rate = 1*an1 + 512;
  float divisor = 1024 / Rate;
  Shape = an2 / divisor + 1.0;

  Step1 = twoPi / Shape;
  Step2 = twoPi / ((Rate + 1) - Shape);
}

float Wave = 0;
int Square = 0;
float Triangle = 0;
float Cos = 0;
float lastRad;


void loop() {
// put your main code here, to run repeatedly:
  setShape();
  rad = rad + Square * Step1 + (1 - Square) * Step2;
  if (rad > twoPi)
    rad = 0.0;
  Wave = 0.5 * sin(rad) + 0.5;
  writeSin(Wave);
  /*
  Square = (int)(rad / Pi);
  writeSquare(Square);
  Triangle = (-1 * (Pi / 2.0) + Square * Pi + (0 - Square) * (rad - (Square * Pi)) + (1 - Square) * (rad - Square * Pi));
  writeTriangle(0.24 * Triangle + 0.5);
  Cos = sin(Triangle);
  writeCos(0.5 * Cos + 0.5);
  lastRad = rad;
  */
}
