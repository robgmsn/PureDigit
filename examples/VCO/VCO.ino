/*
  GMSN! Pure Digit
  28th July 2018
  cc-by 4.0
  Rob Spencer

  VCO
  Yes!! Huge personal technological breakthrough!! Timer controlled writes to the DAC.
  It started off with 44kHz sample rate,. but this had to be reduced so the other processing in the main loop could run.
  It's now down to 15kHz with no real impact on the quality. This is just a tiny module to learn SMD soldering,
  so I'm pretty happy with the result. Obviously things would be pretty different with a fast DSP chip through 16bit DACs :)
  The frequency runs from about 10Hz, up to just over 2Khz, so plenty of subbass and bass goodness to play with.

  Encoder controls the frequency.
  Push and turn to change the waveform: Sine & Saw. Should really add Square too....
*/

#include <PureDigit.h>

PureDigit digit;
int waveForm = 2, freqEnc = 800, cyclePhase = 1, val, cvIn, cvInFreq, freq; //11.53Hz
float ptr = 0, inc = 1;
int sineTable[512] = {2048, 2054, 2060, 2066, 2073, 2079, 2085, 2091, 2098, 2104, 2110, 2117, 2123, 2129, 2135, 2142,
                      2148, 2154, 2161, 2167, 2173, 2179, 2186, 2192, 2198, 2204, 2211, 2217, 2223, 2229, 2236, 2242,
                      2248, 2254, 2261, 2267, 2273, 2279, 2286, 2292, 2298, 2304, 2311, 2317, 2323, 2329, 2335, 2342,
                      2348, 2354, 2360, 2367, 2373, 2379, 2385, 2391, 2398, 2404, 2410, 2416, 2422, 2428, 2435, 2441,
                      2447, 2453, 2459, 2465, 2472, 2478, 2484, 2490, 2496, 2502, 2508, 2514, 2521, 2527, 2533, 2539,
                      2545, 2551, 2557, 2563, 2569, 2575, 2581, 2588, 2594, 2600, 2606, 2612, 2618, 2624, 2630, 2636,
                      2642, 2648, 2654, 2660, 2666, 2672, 2678, 2684, 2690, 2696, 2702, 2708, 2714, 2720, 2725, 2731,
                      2737, 2743, 2749, 2755, 2761, 2767, 2773, 2779, 2784, 2790, 2796, 2802, 2808, 2814, 2819, 2825,
                      2831, 2837, 2843, 2848, 2854, 2860, 2866, 2871, 2877, 2883, 2889, 2894, 2900, 2906, 2912, 2917,
                      2923, 2929, 2934, 2940, 2946, 2951, 2957, 2962, 2968, 2974, 2979, 2985, 2990, 2996, 3002, 3007,
                      3013, 3018, 3024, 3029, 3035, 3040, 3046, 3051, 3057, 3062, 3068, 3073, 3078, 3084, 3089, 3095,
                      3100, 3106, 3111, 3116, 3122, 3127, 3132, 3138, 3143, 3148, 3154, 3159, 3164, 3169, 3175, 3180,
                      3185, 3190, 3195, 3201, 3206, 3211, 3216, 3221, 3226, 3232, 3237, 3242, 3247, 3252, 3257, 3262,
                      3267, 3272, 3277, 3282, 3287, 3292, 3297, 3302, 3307, 3312, 3317, 3322, 3327, 3332, 3337, 3342,
                      3346, 3351, 3356, 3361, 3366, 3371, 3375, 3380, 3385, 3390, 3394, 3399, 3404, 3408, 3413, 3418,
                      3423, 3427, 3432, 3436, 3441, 3446, 3450, 3455, 3459, 3464, 3468, 3473, 3477, 3482, 3486, 3491,
                      3495, 3500, 3504, 3509, 3513, 3517, 3522, 3526, 3530, 3535, 3539, 3543, 3548, 3552, 3556, 3560,
                      3565, 3569, 3573, 3577, 3581, 3586, 3590, 3594, 3598, 3602, 3606, 3610, 3614, 3618, 3622, 3626,
                      3630, 3634, 3638, 3642, 3646, 3650, 3654, 3658, 3662, 3665, 3669, 3673, 3677, 3681, 3685, 3688,
                      3692, 3696, 3700, 3703, 3707, 3711, 3714, 3718, 3722, 3725, 3729, 3732, 3736, 3739, 3743, 3746,
                      3750, 3753, 3757, 3760, 3764, 3767, 3771, 3774, 3777, 3781, 3784, 3787, 3791, 3794, 3797, 3800,
                      3804, 3807, 3810, 3813, 3816, 3820, 3823, 3826, 3829, 3832, 3835, 3838, 3841, 3844, 3847, 3850,
                      3853, 3856, 3859, 3862, 3865, 3868, 3871, 3874, 3876, 3879, 3882, 3885, 3888, 3890, 3893, 3896,
                      3898, 3901, 3904, 3906, 3909, 3912, 3914, 3917, 3919, 3922, 3924, 3927, 3929, 3932, 3934, 3937,
                      3939, 3942, 3944, 3946, 3949, 3951, 3953, 3956, 3958, 3960, 3962, 3965, 3967, 3969, 3971, 3973,
                      3975, 3977, 3980, 3982, 3984, 3986, 3988, 3990, 3992, 3994, 3996, 3997, 3999, 4001, 4003, 4005,
                      4007, 4009, 4010, 4012, 4014, 4016, 4017, 4019, 4021, 4022, 4024, 4026, 4027, 4029, 4031, 4032,
                      4034, 4035, 4037, 4038, 4040, 4041, 4042, 4044, 4045, 4047, 4048, 4049, 4051, 4052, 4053, 4054,
                      4056, 4057, 4058, 4059, 4060, 4062, 4063, 4064, 4065, 4066, 4067, 4068, 4069, 4070, 4071, 4072,
                      4073, 4074, 4075, 4076, 4076, 4077, 4078, 4079, 4080, 4080, 4081, 4082, 4083, 4083, 4084, 4085,
                      4085, 4086, 4086, 4087, 4087, 4088, 4088, 4089, 4089, 4090, 4090, 4091, 4091, 4092, 4092, 4092,
                      4093, 4093, 4093, 4093, 4094, 4094, 4094, 4094, 4094, 4095, 4095, 4095, 4095, 4095, 4095, 4095
                     };

//Setup variables

void setup() {

  //digit.dontCalibrate();
  digit.begin();

  // initialize timer1
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  OCR1A = 1333;            // compare match register 20MHz/1/44kHz = 454  | 20Mhz/1/15kHz = 1333
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS10);    // 1 prescaler
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
  interrupts();             // enable all interrupts

}

ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{
  switch (waveForm) {
    case 1: //sine

      switch (cyclePhase) {
        case 1:
          ptr = ptr + inc;
          if (ptr > 512) {
            ptr = 512 - (ptr - 512);
            cyclePhase = 2;
          }
          val = sineTable[int(ptr)];
          break;
        case 2:
          ptr = ptr - inc;
          if (ptr < 1) {
            ptr = abs(ptr);
            val = sineTable[int(ptr)];
            val = 2048 - (val - 2048);
            cyclePhase = 3;
          } else {
            val = sineTable[int(ptr)];
          }
          break;
        case 3:
          ptr = ptr + inc;
          if (ptr > 512) {
            ptr = 512 - (ptr - 512);
            cyclePhase = 4;
          }
          val = sineTable[int(ptr)];
          val = 2048 - (val - 2048);
          break;
        case 4:
          ptr = ptr - inc;
          if (ptr < 1) {
            ptr = abs(ptr);
            val = sineTable[int(ptr)];
            cyclePhase = 1;
          } else {
            val = sineTable[int(ptr)];
            val = 2048 - (val - 2048);
          }
          break;
      }
      break;

    case 2: //saw
      ptr = ptr - (inc * 2.0);
      if (ptr < 1) {
        ptr = 4096;
      }
      val = abs(ptr);
      break;
  }
  digit.dacWrite(val);
  cvIn = digit.adcRead(1);


}

void loop() {
  if (digit.getSwitchState() == 0) {
    freqEnc = digit.encodeVal(freqEnc);
    digit.displayLED(freqEnc, 2, 0);
  } else {
    waveForm = digit.encodeVal(waveForm);
    waveForm = constrain(waveForm, 1, 2);
    digit.displayLED(waveForm, 1, 0);
  }

  //noInterrupts();
  //interrupts();
  //freqEnc = constrain(freqEnc, 1, 800);
  //cvInFreq = 4096 - (cvIn - 4096);
  cvInFreq = map(cvIn, 0, 4096, 800, 0);
  freq =  cvInFreq + (freqEnc-1000);
  inc = fscale( 0, 800, 1, 200, freq, -4);


  //cvInFreq = map(cvIn, 0, 4096, 1, 800);



}
