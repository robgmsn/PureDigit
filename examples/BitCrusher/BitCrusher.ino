/*
  GMSN! Pure Digit
  28th July 2018
  cc-by 4.0
  Rob Spencer

  BitCrusher
  Turn the encoder to increaser the crush level.
*/

#include <PureDigit.h>

PureDigit digit;

//Setup variables

int val, bitDepth;
void setup() {

  //digit.dontCalibrate();
  digit.begin();

  // initialize timer1
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  OCR1A = 404;            // compare match register 20MHz/1/44kHz = 454
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS10);    // 256 prescaler
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
  interrupts();             // enable all interrupts

}

ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{

  val = digit.adcRead(1);
  val = val >> bitDepth;
  val = val << bitDepth;
  digit.dacWrite(val);


}

void loop() {
    bitDepth = digit.encodeVal(bitDepth);
    bitDepth = constrain(bitDepth, 0, 10);
    digit.displayLED(bitDepth, 1, 0);

}
