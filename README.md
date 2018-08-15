  # GMSN! Pure Digit
  28th July 2018
  
  cc-by 4.0
  
  Rob Spencer
  
  [gmsn.co.uk](https://gmsn.co.uk)
  
  [forum.gmsn.co.uk](https://forum.gmsn.co.uk)
  
  Open Source Synth Designs.
  
  ### Intro
  This repo contains both the Library and Example Sketches for the GMSN! Pure Digit.
  
  A small programmable utility module which accepts +/- 10V in,
  does some digital processing, and outputs a +/-10V signal.
  
  It has a 2 Channel 12bit ADC In, 12bit DAC Out, a 24
  position rotary encoder with switch and a 7 segment LED display.
  
  To install the library
  
  1. [Download the zip](https://github.com/robgmsn/PureDigit/archive/master.zip)
  
  2. Follow the [Importing a .zip library how to](https://www.arduino.cc/en/Guide/Libraries#toc4).
  
  ### Sketches   
  - [x] Random Sequencer
  - [x] EmptySketch
  - [ ] Clock Divider
  - [ ] Micro Tuner
  - [ ] Bit Crusher
  - [ ] Drums
  - [ ] Rhythm Player
  - [ ] Swing Machine
  - [ ] LFO
  - [ ] Step LFO
  - [x] DC Level
  - [ ] Module Testing Functions
  - [ ] Frequency Counter
  - [ ] DC Meter
  
  ### Methods
  
  #### calibrate()
  
  If the module hasn't been calibrated, it will run through a self calibration routine. You'll need a patch cable. 
  
   1. When it displays "1" patch from the output back into input 1. 
   2. Long Press the encoder.
   3. Patch from output into input 2.
   4. Long Press the encoder.
                              
   It will then count down to zero and flash.
   The module is now calibrated.
                              
   N.B. The calibration settings are stored in EEPROM, which is overwritten every time the module is flashed, therefor the calibration routing will run everytime the module is flashed.
                              
  #### encodeVal(value)
  
  Call this to increment or decrement a value with the rotary encoder.
  
  #### display(digit, mode, dp)
  
  The display has two modes:
    
  1. Standard 0 - 9 digital digit.
  2. Positional representation with 12 values. Where the knob is pointing!

  This uses the result of a modulo calculation so the value can be greater than 12 and it will keep rotating. It can't go less than zero though.
                                
  #### displayOff()
  
  Turns the display off. Useful if you want it to flash.
  
  #### dacWrite(value)
  
  Write a value to the DAC. Yes, it's got a 20V P2P voltage!! :D
             0    =   +10V
             2048 =   0V
             4095 =   -10V
  
  #### dacWriteCal(value)
    
  Same as above, but uses the calibration settings. If you want the output to match the input, this is the method to use.
  
  #### adcRead(channel)
    
  Read a 20V P2P from the DAC, channel 1 or 2. Voltages as above.
  
  
  ### Example Sketch
  ```
  // A random sequencer with variable delay 
  
  #include <Digit.h>                    //Include the Digit library
  
  Digit digit;                          //Create a Digit called digit. This object now has the methods above available to it.
  
  //Setup variables
  int encPos = 1;                       //Just some variables for us to use in the code
  
  void setup() {
   digit.begin();                       //Tell digit to configure its self.
  }
  
  void loop() {
     encPos = digit.encodeVal(encPos);  //Increment or decrement the varible encPos
     digit.displayLED(encPos, 2, 0);    //Display the value of encPos as a Positional Representation with no decimal point
     digit.dacWrite(random(4095));      //Output a random value between 0 and 4095, which is analogue +/- 10V
     delay(encPos);                     //Delay for encPos milliseconds.
  }
 ```
