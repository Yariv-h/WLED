#include "FX.h"
extern double fftResultLogarithmicNoiseless[];
int mode = 0;
int timersMode = 14; // starting effect
uint32_t delayTimer = 0;

extern byte effectSpeed;
extern byte effectIntensity;
extern byte effectFFT1;
extern byte effectFFT2;
extern byte effectFFT3;
extern byte effectPalette;

//uint16_t SEGMENT.BASE_TIME = 15000;
//uint16_t BASE_TIME = 30000;
//uint16_t FADE_TIME = 10000;
#define TIMER_MODES 55 //should be max mode +1
int haanSpeed =0;
int haanIntensity=0;
uint32_t haanSEG0 = 0;
uint32_t haanSEG1 = 0;
uint32_t haanSEG2 = 0;
int haanFFTlow = 0;
int haanFFTHigh = 0;
int haanFFTCustom =0;
int haanPallet=0;
int haanBlandingPallet =0;





#define FFT_WALL 0
#define FFT_SWIPE 1
#define RANDOM_FFT 2
#define FFT_FIB 3
#define FFT_BLINK 4
#define DANCING_BAR 5
#define MODE_JUGGLE 6
#define MODE_MIDNOISE 7
#define MODE_PIXELS 8
#define MODE_PLASMOID 9
#define MODE_PUDDLEPEAK 10
#define MODE_RIPPLEPEAK 11
#define MODE_FREQPIXEL 12
#define MODE_NOISEPEAK 13
#define MODE_2DDNA 14
#define MODE_2DMATRIX 15
#define MODE_2DPLASMA 16
#define MODE_2DMEATBALLS 17
#define MODE_2DSQUAREDSWIRL 18
#define MODE_FLOW 19
#define MODE_CHASE 20
#define MODE_CANDLE 21
#define MODE_BLINK_RAINBOW 22
#define MODE_CIRCUS_COMBUSTUS 23
#define MODE_COLORWAVES 24
#define MODE_DRIP 25
#define MODE_DYNAMIC 26
#define MODE_FIREWORKS 27
#define MODE_GRADIENT 28
#define MODE_HEARTBEAT 29
#define MODE_COMET 30
#define MODE_PACIFICA 31
#define MODE_PERLINMOVE 32
#define MODE_PHASED 33
#define MODE_PHASED_NOISE 34
#define MODE_PLASMA 35
#define MODE_RIPPLE 36
#define MODE_RIPPLE_RAINBOW 37
#define MODE_RUNNING_BASE 38
#define MODE_RUNNING 39
#define MODE_SAW 40
#define MODE_SCAN 41
#define MODE_DUAL_SCAN 42
#define MODE_SINEWAVE 43
#define MODE_SOLID_GLITTER 44
#define MODE_SPARKLE 45
#define MODE_TWO_DOTS 46
#define MODE_TWO_AREAS 47
#define MODE_HAAN_FADE 48
#define MODE_SPARKLE_DARK 49
#define MODE_SPARKLE_PLUS 50
#define SET_1 51
#define SET_2 52
#define SET_3 53
#define SET_4 54
#define SET_5 55
#define SET_6 56
#define SET_7 57
#define SET_8 58
#define SET_9 59
#define SET_10 60
#define SET_11 61
#define SET_12 62
#define MODE_COLORWINKLES 63
#define AUTO_HAAN_MIX 64
#define AUTOH_HAAN_MIX_COLLOR 65







bool wallFFTDisabled = false;
uint16_t WS2812FX::mode_fft_wall() {
    printSetNSettings();
    //int col = 192;

    //uint16_t col = SEGCOLOR(0);
    uint16_t col = 0;
    if(haanPalettes) {
       BlandingPallets();       
    } else {
      //col = CRGBColor;
      col = SEGMENT.fastledColor;
    }
    

    double scaleVal = SEGMENT.fft3 /100;  //TODO - Yariv to fix!!
    for(int i=0; i<16; i++) {
      fftResultLogarithmicNoiseless[i]*=scaleVal;
    }

    for(int i=0; i<SEGLEN; i++) {
        int val = constrain(fftResultLogarithmicNoiseless[i%16], 0, 254);
        int sat = constrain(fftResultLogarithmicNoiseless[i%16]+120,0,250);
        int bri = constrain(fftResultLogarithmicNoiseless[i%16],0,250);
        
        if(wallFFTDisabled) {
          val = 0;
          sat = 0;
          bri = 0;
        }
    //    col = 192;
        if(pixelToSkip(i)) continue;
        CRGB newcolor;
        
        if(haanPalettes) {
          setColorFromPaletts(bri, sat, i);
          // BlandingPallets(); 
          // newcolor = ColorFromPalette(currentPalette1, sat, bri, LINEARBLEND);
          // Serial.println(newcolor);
          // CRGB fastled_col = col_to_crgb(getPixelColor(i));
          // nblend(fastled_col, newcolor, 128);
          // setPixelColor(i, newcolor.red, newcolor.green, newcolor.blue);
        } else {
          newcolor = CHSV(col, sat, bri);
          setPixelColor(i, crgb_to_col(newcolor));
        }
        
        
     }

     addGlitterPro(sampleAvg, 48, 250, sampleAvg);

     if(sampleAvg>170) {
         addGlitterPro(sampleAvg, 192, 0, 254);
         
     }

     if(sampleAvg> 70 && sampleAvg < 170) {
         addGlitterPro(sampleAvg, 38, 150, constrain(sampleAvg+100,0,254));
         
     }
     
     if(sampleAvg< 70 && sampleAvg > 40) {
        addGlitterPro(sampleAvg, 32, 250, 250);
     }
   
    
    fade_out(SEGMENT.intensity);
    return FRAMETIME + SEGMENT.speed;
 }

 void WS2812FX::addGlitterPro(fract8 chanceOfGlitter, int color, int sat, int bright) { // Let's add some glitter, thanks to Mark
  if( random8() < chanceOfGlitter) {
    CRGB newcolor = CHSV(color,sat, bright); 
    setPixelColor(random16(SEGLEN), crgb_to_col(newcolor));
  }
}

void WS2812FX::setColorFromPaletts(int bri, int sat, int i) {
      BlandingPallets(); 
      CRGB newcolor = ColorFromPalette(currentPalette1, sat, bri, LINEARBLEND);
      //Serial.println(newcolor);
      CRGB fastled_col = col_to_crgb(getPixelColor(i));
      nblend(fastled_col, newcolor, 128);
      setPixelColor(i, newcolor.red, newcolor.green, newcolor.blue);
}

bool WS2812FX::pixelToSkip(int pixel)  {
     //if(pixel >=0 && pixel <20) return true;
     //if(pixel >=110 && pixel <=148) return true;
     //if(pixel >=230 && pixel <=250) return true;
        
        
        
        //if(pixel >=40 && pixel <86) return true;
        if(pixel >=127 && pixel <187) return true;
        //if(pixel >=234 && pixel <278) return true;
        if(pixel >=313 && pixel <373) return true;
     return false;
 } //mode_fft_wall

uint16_t WS2812FX::mode_haan_fade() {
  fade_out(0);
  return FRAMETIME;
}

int fftSwpieLoop = 0;
byte singleColorMode = 0;
byte BLOCK_SIZE = 20;


uint16_t WS2812FX::mode_fft_swiping() {
  printSetNSettings();
  double scaleVal = SEGMENT.fft3 /100;
  for(int i=0; i<16; i++) {
    fftResultLogarithmicNoiseless[i]*=scaleVal;
  }

 //int color = 192; 
 
 fract8 chanceOfGlitter = sampleAvg;
 

 if( random8() < chanceOfGlitter-SEGMENT.speed) {


    EVERY_N_MILLIS(5000) {
       singleColorMode++;
       singleColorMode=singleColorMode%5;
      }

    

    switch(singleColorMode) {
          case 0:
            if(fftSwpieLoop >= SEGLEN-BLOCK_SIZE) {
                 fftSwpieLoop-=BLOCK_SIZE;
                 singleColorMode=1;
            }
            fftSwpieLoop+=BLOCK_SIZE;
            break;

          case 1:
           if(fftSwpieLoop-BLOCK_SIZE < 0) {
                fftSwpieLoop+=BLOCK_SIZE;
                singleColorMode=0;
            }
            fftSwpieLoop-=BLOCK_SIZE;
            break;

          case 2:
            fftSwpieLoop+=BLOCK_SIZE;
            fftSwpieLoop=fftSwpieLoop%SEGLEN;
            break;

          case 3:

            if(fftSwpieLoop-BLOCK_SIZE < 0) {
              fftSwpieLoop= SEGLEN-BLOCK_SIZE;
            }
            fftSwpieLoop-=BLOCK_SIZE;
            break;

          case 4:
            if(fftSwpieLoop-BLOCK_SIZE < 0 && fftSwpieLoop-20> -20) {
              fftSwpieLoop=BLOCK_SIZE;
            }
            else if(fftSwpieLoop-BLOCK_SIZE < 0) {
              fftSwpieLoop= SEGLEN-BLOCK_SIZE-1; // TODO - fix this - not reaching 0...
            }
            fftSwpieLoop-=BLOCK_SIZE;
            break;
    }


    for(int i = fftSwpieLoop; i < fftSwpieLoop+BLOCK_SIZE; i++) {


        int sat = constrain(fftResultLogarithmicNoiseless[i%16]+160,0,254);
        int bri = constrain(fftResultLogarithmicNoiseless[i%16]+100,0,254);
        int col = constrain(fftResultLogarithmicNoiseless[i%16],0,254);
        col = SEGMENT.fastledColor;



         if(sat<255 && sat >0 && bri<255 && bri >0 && col<255 && col >0){
            
            if(haanPalettes) {
                 setColorFromPaletts(bri, sat, i);
            } else {

              //CRGB newcolor = ColorFromPalette(currentPalette, sat, bri, LINEARBLEND);
              CRGB newcolor = CHSV(col, sat, bri);
              CRGB fastled_col = col_to_crgb(getPixelColor(i));

              nblend(fastled_col, newcolor, 128);
              setPixelColor(i, fastled_col.red, fastled_col.green, fastled_col.blue);
            }

          // CRGB newcolor = CHSV(col, sat, bri);
          // setPixelColor(i, crgb_to_col(newcolor));
         } else {
           CRGB newcolor = CHSV(col, 0, 0);
           setPixelColor(i, crgb_to_col(newcolor));
         }
    }

   } else {
    fade_out(SEGMENT.intensity);
   }

  //return FRAMETIME;
  return 30+sampleAvg/2;

} //fft swipe



uint16_t WS2812FX::mode_random_fft() {

    printSetNSettings();
    //uint16_t col = CRGBColor;
    uint16_t col = SEGMENT.fastledColor;
    

    double scaleVal = SEGMENT.fft3 /100;
    for(int i=0; i<16; i++) {
      fftResultLogarithmicNoiseless[i]*=scaleVal;
    }

    int pixel = random(0,SEGLEN-30);
    for(int i=0; i<16; i++) {
      
      int sat = constrain(fftResultLogarithmicNoiseless[i%16]+160,0,254);
      int bri = constrain(fftResultLogarithmicNoiseless[i%16]+100,0,254);
      //int col = constrain(fftResultLogarithmicNoiseless[i%16],0,254);

      if(haanPalettes) {
        setColorFromPaletts(bri, sat, pixel+i);
        // CRGB newcolor = ColorFromPalette(currentPalette1, sat, bri, LINEARBLEND);
        // CRGB fastled_col = col_to_crgb(getPixelColor(i));

        // nblend(fastled_col, newcolor, 128);
        // setPixelColor(pixel+i, fastled_col.red, fastled_col.green, fastled_col.blue);
      } else {
      
        CRGB newcolor = CHSV(col, sat, bri);
        setPixelColor(pixel+i, crgb_to_col(newcolor));        
      }
    }

    if(sampleAvg>170) {
       //addGlitterPro(sampleAvg, 48, 250, 250);
       setPixelColor(random16(SEGLEN), ULTRAWHITE);
    }

    fade_out(SEGMENT.intensity);
    return 30+sampleAvg/2;
} // mode_random_fft


 uint16_t WS2812FX::dancingBar2() {
   printSetNSettings();

   
   double scaleVal = SEGMENT.fft3 /100;
    for(int i=0; i<16; i++) {
      fftResultLogarithmicNoiseless[i]*=scaleVal;
    }
   
   int offset = 0;     
   for (int i = 0; i < 16; i++) {  // goes through each octave. skip the first 1, which is not useful
     
      int j = fftResultLogarithmicNoiseless[i];
      int saturation=constrain(j+30, 0,255);
      int brightness=constrain(j+30, 0,255);

      if (brightness==255) {
          saturation=50;
          brightness=200;
      }    

    int color = SEGCOLOR(0);          

    //if(j < 40 && j > 0) {
      if(j > 40) {
      for(int x=0; x < 5; x++) {
          
          CRGB newcolor = CHSV(color+(i*5),saturation, brightness); 
          setPixelColor(0+i+x, crgb_to_col(newcolor));
      }      
    } else {
        CRGB newcolor = CHSV(color+(i*5),saturation, brightness); 
        setPixelColor(0+i, crgb_to_col(newcolor));
    }
     
   }
   fade_out(SEGMENT.intensity);
   return FRAMETIME;
 } // dancingBar2


 uint16_t WS2812FX::fft_blink() {
   printSetNSettings();
  

   double scaleVal = SEGMENT.fft3 /200;
   for(int i=0; i<16; i++) {
      fftResultLogarithmicNoiseless[i]*=scaleVal;
    }

    
    //uint16_t col = CRGBColor;
    uint16_t col = SEGMENT.fastledColor;
    //col = SEGMENT.fastledColor;
    //Serial.print("blink col"); Serial.println(col);
    for(int i=0; i<SEGLEN; i++) {
      
      int j = fftResultLogarithmicNoiseless[i%16];
      int saturation=constrain(j+170, 0,255);
      int brightness=constrain(j+130, 0,255);
      //color = constrain(color+j,0,254);
       if(haanPalettes) {
         setColorFromPaletts(brightness, saturation, i);
       } else {

        CRGB newcolor = CHSV(col,saturation, brightness); 
        setPixelColor(i, crgb_to_col(newcolor));
     }

      
    }
    uint16_t tempo = constrain(60-sampleAvg,0,254);
    return tempo;
 } //fft blink


uint16_t WS2812FX::mode_fft_fib() {
  printSetNSettings();
  //BlandingPallets();
  BlandingPalletsFib();
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;
 
  uint8_t sat8 = beatsin88( 87, 220, 250);
  uint8_t brightdepth = beatsin88( 341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;//gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 300, 1500);
  
  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis ;
  sLastMillis  = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88( 400, 5,9);
  uint16_t brightnesstheta16 = sPseudotime;
  
  for( uint16_t i = 0 ; i < SEGLEN; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;
    uint16_t h16_128 = hue16 >> 7;
    if( h16_128 & 0x100) {
      hue8 = 255 - (h16_128 >> 1);
    } else {
      hue8 = h16_128 >> 1;
    }

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16( brightnesstheta16  ) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);
    
    uint8_t index = hue8;
    //index = triwave8( index);
    index = scale8( index, 240);

    //CRGB newcolor = ColorFromPalette(currentPalette1, index, bri8, LINEARBLEND);
    CRGB newcolor = ColorFromPalette(currentPalette1, index, bri8, LINEARBLEND);
   

    uint16_t pixelnumber = i;
    pixelnumber = (SEGLEN-1) - pixelnumber;
    
    CRGB fastled_col = col_to_crgb(getPixelColor(i));
    nblend(fastled_col, newcolor, 128);
    setPixelColor(i, fastled_col.red, fastled_col.green, fastled_col.blue);
  }
  return FRAMETIME;
} //fft fib


#define PALETTE_SOLID_WRAP_FFT (paletteBlend == 1 || paletteBlend == 3)
int cycles =0;
uint16_t WS2812FX::mode_puddles_fft(void) {                                   // Puddles. By Andrew Tuline.
  printSetNSettings();
  uint16_t size = 0;
  uint8_t fadeVal = map(SEGMENT.speed,0,255, 224, 255);
  uint16_t pos = random(SEGLEN);                                          // Set a random starting position.

  fade_out(fadeVal);

  if (samplePeak == 1 ) {
    size = sample * SEGMENT.intensity /256 + 1;  
    size = sampleAvg*2;                      // Determine size of the flash based on the volume.
    if (pos+size>= SEGLEN) size=SEGLEN-pos;
    samplePeak = 0;
  }


  cycles++;
  cycles%=5;
  if(cycles ==4) {
    for(int i=0; i<size; i++) {                                             // Flash the LED's.
      if(haanPalettes) {
        setColorFromPaletts(255, sampleAvg, pos+i);
      } else {
        setPixelColor(pos+i, color_blend(SEGCOLOR(1), color_from_palette(millis(), false, PALETTE_SOLID_WRAP_FFT, 0), 255));
      }

    }
  }

  return FRAMETIME+SEGMENT.fft3; //haan change
} // puddle yariv


uint8_t gCurrentPaletteNumber = 0;
void WS2812FX::BlandingPallets() {
  EVERY_N_MILLISECONDS( 5000 ) {
    //gCurrentPaletteNumber = addmod8( gCurrentPaletteNumber, 1, 54);
    //targetPalette1 = gGradientPalettes[ gCurrentPaletteNumber ];
    gCurrentPaletteNumber = addmod8( gCurrentPaletteNumber, 1, 36);
    targetPalette1 = gGradientPalettesHaan[ gCurrentPaletteNumber ];
    
  }

  EVERY_N_MILLISECONDS(40) {
    nblendPaletteTowardPalette( currentPalette1, targetPalette1, 16);
  }
} //pallet planding


void WS2812FX::BlandingPalletsFib() {
  EVERY_N_MILLISECONDS( 5000 ) {
    //gCurrentPaletteNumber = addmod8( gCurrentPaletteNumber, 1, 54);
    //targetPalette1 = gGradientPalettes[ gCurrentPaletteNumber ];
    gCurrentPaletteNumber = addmod8( gCurrentPaletteNumber, 1, 78);
    targetPalette1 = gGradientPalettes[ gCurrentPaletteNumber ];
    
  }

  EVERY_N_MILLISECONDS(40) {
    nblendPaletteTowardPalette( currentPalette1, targetPalette1, 16);
  }
} 

void WS2812FX::setNSettings(int speed, int intensity, uint32_t seg0, uint32_t seg1, uint32_t seg2, int fftlow, int ffthigh, int fftcustom, int pallet, int palletblanding) {
  effectSpeed = speed;
  SEGMENT.speed = speed;
  effectIntensity = intensity;
  SEGMENT.intensity = intensity;
  effectFFT1 = fftlow;
  SEGMENT.fft1 = fftlow;
  effectFFT2 = ffthigh;
  SEGMENT.fft2 = fftlow;
  effectFFT3 = fftcustom;
  SEGMENT.fft3 = fftcustom;
  _segments[_segment_index].colors[0] =seg0;
  _segments[_segment_index].colors[1] =seg1;
  _segments[_segment_index].colors[2] =seg2;
  effectPalette = pallet;
  SEGMENT.palette = pallet;
  //currentPalette1 = gGradientPalettes[5];
  //pallet blanding
}
void WS2812FX::setHaanSettings(int speed, int intensity, uint32_t seg0, uint32_t seg1, uint32_t seg2,  int fftlow, int ffthigh, int fftcustom, int pallet, int palletblanding, int col) {
  haanSpeed = speed;
  haanIntensity = intensity;
  haanSEG0 = seg0;
  haanSEG1 = seg1;
  haanSEG2 = seg2;
  haanFFTlow = fftlow;
  haanFFTHigh = ffthigh;
  haanFFTCustom = fftcustom;
  haanPallet = pallet;
  haanBlandingPallet = palletblanding;
  CRGBColor = col;
  SEGMENT.fastledColor = col;
}

void WS2812FX::printSetNSettings() {
  EVERY_N_MILLIS(5000) {
    Serial.print("Speed: ");
    Serial.print(SEGMENT.speed);
    Serial.print(", Intensity: ");
    Serial.print(SEGMENT.intensity);
    Serial.print(", COLOR0: ");
    Serial.print(_segments[_segment_index].colors[0]);
    Serial.print(", COLOR1: ");
    Serial.print(_segments[_segment_index].colors[1]);
    Serial.print(", COLOR2: ");
    Serial.print(_segments[_segment_index].colors[2]);
    Serial.print(", FFT_LOW: ");
    Serial.print(SEGMENT.fft1);
    Serial.print(", FFT_HIGH: ");
    Serial.print(SEGMENT.fft2);
    Serial.print(", FFT_CUSTOM: ");
    Serial.print(SEGMENT.fft3);
    Serial.print(", PALLET: ");
    Serial.print(SEGMENT.palette);
    Serial.print(", PALLET_BLENDING: ");
    Serial.println(gCurrentPaletteNumber);
    Serial.print(", FastLed coll: ");
    Serial.print(SEGMENT.fastledColor);
     Serial.print(", BASE_Time: ");
    Serial.print(SEGMENT.BASE_TIME);
     Serial.print(", FADE_TIME: ");
    Serial.println(SEGMENT.FADE_TIME);
  }
}

/***** SET 0 *********/
uint32_t delayTimerSet0 = 0;
int timersModeSet0 = 0;
#define TIMER_MODES_SET0 2 //should be max mode +1
int modeSet0 = 0;
/**********************/
uint16_t WS2812FX::set0() {
  EVERY_N_MILLISECONDS_I(pixTimerSet0, delayTimerSet0) { 
      switch (timersModeSet0)
      {
        case 0 :
          delayTimerSet0 = SEGMENT.BASE_TIME;
          modeSet0 = FFT_FIB;
          break;
        case 1:
          delayTimerSet0 = SEGMENT.FADE_TIME;
          modeSet0 = MODE_HAAN_FADE;
          break;
        default:
        break;
      }
      setNSettings(haanSpeed, haanIntensity, haanSEG0, haanSEG1, haanSEG2, haanFFTlow, haanFFTHigh, haanFFTCustom, haanPallet, haanBlandingPallet);
      Serial.print("Set0 Mode: ");Serial.print(modeSet0); Serial.print(", Timer Mode:");
      Serial.println(timersModeSet0);
      pixTimerSet0.setPeriod(delayTimerSet0);
      timersModeSet0++;
      timersModeSet0%=TIMER_MODES_SET0;
  }
  
  switch (modeSet0)
  {
  case FFT_FIB:
    return mode_fft_fib();
  case MODE_HAAN_FADE:
    return mode_haan_fade();
  default:
    break;
  }
}




/***** SET 1 *********/
uint32_t delayTimerSet1 = 0;
int timersModeSet1 = 0;
#define TIMER_MODES_SET1 8 //should be max mode +1
int modeSet1 = 0;
/**********************/
uint16_t WS2812FX::set1_color() {
  haanPalettes = true;
  uint16_t rate = set1();
  haanPalettes = false;
  return rate;
}



uint16_t WS2812FX::set1() {
  EVERY_N_MILLISECONDS_I(pixTimerSet1, delayTimerSet1) { 
      switch (timersModeSet1)
      {
        case 0:
          delayTimerSet1 = SEGMENT.BASE_TIME;
          modeSet1 = FFT_WALL;
          wallFFTDisabled = true;
          setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
          break;
        case 1:
          delayTimerSet1 = SEGMENT.BASE_TIME;
          modeSet1 = FFT_WALL;
          wallFFTDisabled = false;
          setHaanSettings(0,288,16752640, 0, 0,0,0,228,0,0,192);
          break;
        case 2:
          delayTimerSet1 = SEGMENT.BASE_TIME;
          modeSet1 = FFT_WALL;
          wallFFTDisabled = true;
          setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
          break;
        case 3:
          delayTimerSet1 = SEGMENT.FADE_TIME;
          modeSet1 = MODE_HAAN_FADE;
          wallFFTDisabled = true;
          setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
          break;
        case 4:
          delayTimerSet1 = SEGMENT.BASE_TIME;
          modeSet1 = FFT_SWIPE;
          wallFFTDisabled = true;
          setHaanSettings(0,288,8913151, 0, 0,0,0,228,0,0,192);  //To remove - just for init testing
          break;
        case 5:
          delayTimerSet1 = SEGMENT.BASE_TIME;
          modeSet1 = RANDOM_FFT;
          wallFFTDisabled = true;
          setHaanSettings(0,288,8913151, 0, 0,0,0,228,0,0,192);  //To remove - just for init testing
          break;
        case 6:
          delayTimerSet1 = SEGMENT.FADE_TIME;
          modeSet1 = MODE_HAAN_FADE;
          wallFFTDisabled = true;
          setHaanSettings(0,288,8913151, 0, 0,0,0,228,0,0,192);  //To remove - just for init testing
          break;
        case 7:
          delayTimerSet1 = SEGMENT.BASE_TIME;
          modeSet1 = FFT_BLINK;
          wallFFTDisabled = true;
          setHaanSettings(0,288,8913151, 0, 0,0,0,228,0,0,192);  //To remove - just for init testing
          break;
        default:
          break;
      }
      setNSettings(haanSpeed, haanIntensity, haanSEG0, haanSEG1, haanSEG2, haanFFTlow, haanFFTHigh, haanFFTCustom, haanPallet, haanBlandingPallet);
      Serial.print("Set1 Mode: ");Serial.print(modeSet1); Serial.print(", Timer Mode:");
      Serial.println(timersModeSet1);
      pixTimerSet1.setPeriod(delayTimerSet1);
      timersModeSet1++;
      timersModeSet1%=TIMER_MODES_SET1;
  }

  switch (modeSet1)
  {
  case FFT_WALL:
    return mode_fft_wall();
  case MODE_HAAN_FADE:
    return mode_haan_fade();
  case FFT_SWIPE:
    return mode_fft_swiping();
  case RANDOM_FFT:
    return mode_random_fft();
  case FFT_BLINK:
    return fft_blink();
  default:
    break;
  }
}

/***** SET 2 *********/
uint32_t delayTimerSet2 = 0;
int timersModeSet2 = 0;
#define TIMER_MODES_SET2 3 //should be max mode +1
int modeSet2 = 0;
/**********************/

uint16_t WS2812FX::set2_color() {
  haanPalettes = true;
  uint16_t rate = set2();
  haanPalettes = false;
  return rate;
}
uint16_t WS2812FX::set2() {
  EVERY_N_MILLISECONDS_I(pixTimerSet2, delayTimerSet2) { 
      switch (timersModeSet2)
      {
        case 0:
          delayTimerSet2 = SEGMENT.BASE_TIME;
          modeSet2 = MODE_2DDNA;
          setHaanSettings(0, 255, 8913151, 3868159, 16715587, 255, 255, 143, 42, 0,192);
          break;

        case 1:
          delayTimerSet2 = SEGMENT.BASE_TIME;
          modeSet2 = MODE_2DSQUAREDSWIRL;
          setHaanSettings(27, 255, 8913151, 3868159, 16715587, 43, 64, 81,  0, 0 ,192);
          break;

        case 2:
          delayTimerSet2 = SEGMENT.FADE_TIME;
          modeSet2 = MODE_HAAN_FADE;
          setHaanSettings(0,255,13107455,0, 0, 255,  255,  255, 0, 0, 0);
          break;

        default:
          break;
      }
      setNSettings(haanSpeed, haanIntensity, haanSEG0, haanSEG1, haanSEG2, haanFFTlow, haanFFTHigh, haanFFTCustom, haanPallet, haanBlandingPallet);
      Serial.print("Set2 Mode: ");Serial.print(modeSet2); Serial.print(", Timer Mode:");
      Serial.println(timersModeSet2);
      pixTimerSet2.setPeriod(delayTimerSet2);
      timersModeSet2++;
      timersModeSet2%=TIMER_MODES_SET2;
  }

  switch (modeSet2)
  {
  case 14:
    return mode_2Ddna();
  case 18:
    return mode_2Dsquaredswirl();
  case 48:
    return mode_haan_fade();
  default:
    break;
  }
}


/***** SET 3 *********/
uint32_t delayTimerSet3 = 0;
int timersModeSet3 = 0;
#define TIMER_MODES_SET3 8 //should be max mode +1
int modeSet3 = 0;
/**********************/

uint16_t WS2812FX::set3_color() {
  haanPalettes = true;
  uint16_t rate = set3();
  haanPalettes = false;
  return rate;
}

uint16_t WS2812FX::set3() {
  EVERY_N_MILLISECONDS_I(pixTimerSet3, delayTimerSet3) { 
      switch (timersModeSet3)
      {
        case 0:
          delayTimerSet3 = SEGMENT.BASE_TIME;      
          modeSet3 = MODE_2DPLASMA;
          //blue purpul
          //FFT custom can scale to 248
          setHaanSettings(27,255,13107455,7536895, 0, 255,  255,  255, 0, 0, 192);

          break;

        case 1:
          delayTimerSet3 = SEGMENT.BASE_TIME;
          modeSet3 = MODE_JUGGLE;
          setHaanSettings(18,255,13107455,0, 0, 255,  255,  42, 0, 0, 0);
          break; 
        
        case 2:
          delayTimerSet3 = SEGMENT.BASE_TIME;
          setHaanSettings(18,69,13107455,0, 0, 255,  255,  42, 0, 0, 0);
          modeSet3 = MODE_PIXELS;
          break; 

        case 3:
          delayTimerSet3 = SEGMENT.FADE_TIME;
          modeSet3 = MODE_HAAN_FADE;
          setHaanSettings(0,255,13107455,0, 0, 255,  255,  255, 0, 0, 0);
          break;  
        
        case 4:
          delayTimerSet3 = SEGMENT.BASE_TIME;
          //high intencity in high vol - static color
          setHaanSettings(18,17,13107455,0, 0, 255,  255,  42, 0, 0, 0);
          modeSet3 = MODE_PLASMOID;
          break; 

        case 5:
          delayTimerSet3 = SEGMENT.BASE_TIME;
          modeSet3 = MODE_FREQPIXEL;
          
          setHaanSettings(3,17,13107455,0, 0, 255,  255,  42, 0, 0, 0);
          break;
        
        case 6:
          delayTimerSet3 = SEGMENT.BASE_TIME;
          modeSet3 = MODE_PUDDLEPEAK;
           setHaanSettings(0,255,13107455,0, 0, 255,  255,  255, 0, 0, 0);
          break;

        case 7:
          delayTimerSet3 = SEGMENT.FADE_TIME;
          modeSet3 = MODE_HAAN_FADE;
          setHaanSettings(0,255,13107455,0, 0, 255,  255,  255, 0, 0, 0);
          break;
        
        default:
          break;
      }

      setNSettings(haanSpeed, haanIntensity, haanSEG0, haanSEG1, haanSEG2, haanFFTlow, haanFFTHigh, haanFFTCustom, haanPallet, haanBlandingPallet);
      Serial.print("Set3 Mode: ");Serial.print(modeSet3); Serial.print(", Timer Mode:");
      Serial.println(timersModeSet3);
      pixTimerSet3.setPeriod(delayTimerSet3);
      timersModeSet3++;
      timersModeSet3%=TIMER_MODES_SET3;
  }

  switch (modeSet3)
  {
  case 16:
    return mode_2Dplasma();
  case 6:
    return mode_juggles();
  case 8:
    return mode_pixels();
  case 9:
    return mode_plasmoid();
  case 12:
    return mode_freqpixel();
  case 10: 
    return mode_puddlepeak();      
  case 48:
    return mode_haan_fade();   
  default:
    break;
  }
  
}


/***** SET 4 *********/
uint32_t delayTimerSet4 = 0;
int timersModeSet4 = 0;
#define TIMER_MODES_SET4 2 //should be max mode +1
int modeSet4 = 0;
/**********************/

uint16_t WS2812FX::set4() {
  EVERY_N_MILLISECONDS_I(pixTimerSet4, delayTimerSet4) { 
      switch (timersModeSet4)
      {
        case 0:
          delayTimerSet4 = SEGMENT.BASE_TIME;
          modeSet4 = MODE_2DMATRIX;
           setHaanSettings(18,255,13107455,7536895, 0, 255,  255,  129, 0, 0, 0);
          break;
        case 1:
            delayTimerSet4 = SEGMENT.FADE_TIME;
            setHaanSettings(18,255,13107455,0, 0, 255,  255,  42, 0, 0, 0);
            modeSet4 = MODE_HAAN_FADE;
          break;
        default:
          break;
      }
      setNSettings(haanSpeed, haanIntensity, haanSEG0, haanSEG1, haanSEG2, haanFFTlow, haanFFTHigh, haanFFTCustom, haanPallet, haanBlandingPallet);
      Serial.print("Set4 Mode: ");Serial.print(modeSet4); Serial.print(", Timer Mode:");
      Serial.println(timersModeSet4);
      pixTimerSet4.setPeriod(delayTimerSet4);
      timersModeSet4++;
      timersModeSet4%=TIMER_MODES_SET4;
  }

  switch (modeSet4)
  {
  case MODE_2DMATRIX:
    return mode_2Dmatrix();
  case MODE_HAAN_FADE:
    return mode_haan_fade();
  
  default:
    break;
  }
}


/***** SET 5 *********/
uint32_t delayTimerSet5 = 0;
int timersModeSet5 = 0;
#define TIMER_MODES_SET5 13 //should be max mode +1
int modeSet5 = 0;
/**********************/
uint16_t WS2812FX::set5_color() {
  haanPalettes = true;
  uint16_t rate = set5();
  haanPalettes = false;
  return rate;
}

uint16_t WS2812FX::set5() {
  EVERY_N_MILLISECONDS_I(pixTimerSet5, delayTimerSet5) { 
      switch (timersModeSet5)
      {
        case 0:
          delayTimerSet5 = SEGMENT.BASE_TIME;
          modeSet5 = MODE_CHASE;
          setHaanSettings(73,255,13107455,7536895, 0, 255,  255,  42, 0, 0, 0);
          break;
        case 1:
          delayTimerSet5 = SEGMENT.BASE_TIME;
          modeSet5 = MODE_COLORWAVES;
          setHaanSettings(5,3,13107455,7536895, 0, 255,  255,  42, 0, 0, 0);
          break;

        case 2:
          delayTimerSet5 = SEGMENT.BASE_TIME;
          modeSet5 = MODE_COLORWAVES;
          setHaanSettings(5,3,13107455,7536895, 0, 255,  255,  42, 0, 0, 0);
          break;
        
        case 3: 
          delayTimerSet5 = SEGMENT.BASE_TIME;
          modeSet5 = MODE_COLORWINKLES;
          setHaanSettings(5,3,13107455,7536895, 0, 255,  255,  42, 0, 0, 0);
          break;
        case 4:
          delayTimerSet5 = SEGMENT.BASE_TIME;
          modeSet5 = MODE_FIREWORKS;
          setHaanSettings(0, 0, 6038015, 0, 0,  255, 255, 255, 10,  0, 0);
          break;
        case 5:
          delayTimerSet5 = SEGMENT.FADE_TIME;
          modeSet5 = MODE_HAAN_FADE;
          setHaanSettings(0, 0, 6038015, 0, 0,  255, 255, 255, 10,  0, 0);
          break;
        case 6:
          delayTimerSet5 = SEGMENT.BASE_TIME;
          modeSet5 = MODE_DRIP;
          setHaanSettings(0, 211, 6038015,0,  0, 83, 255,  255,  28,  0, 0);
          break;
        case 7:
          delayTimerSet5 = SEGMENT.FADE_TIME;
          modeSet5 = MODE_HAAN_FADE;
          setHaanSettings(0, 0, 6038015, 0, 0,  255, 255, 255, 10,  0, 0);
          break;
        case 8:
          delayTimerSet5 = SEGMENT.BASE_TIME;
          modeSet5 = MODE_FLOW;
          setHaanSettings(27,255, 7618047,  0, 0,  43, 64, 81,  0,  0, 0);
          break;
        case 9:
          delayTimerSet5 = SEGMENT.BASE_TIME;
          modeSet5 = MODE_SOLID_GLITTER;
          setHaanSettings(0, 44, 16731549, 10162175,  0, 208,  255,  252, 66, 0, 0);
          break;
        case 10:
          delayTimerSet5 = SEGMENT.FADE_TIME;
          modeSet5 = MODE_HAAN_FADE;
          setHaanSettings(0, 0, 6038015, 0, 0,  255, 255, 255, 10,  0, 0);
          break;
        case 11:
          delayTimerSet5 = SEGMENT.BASE_TIME;
          modeSet5 = MODE_PACIFICA;
          setHaanSettings( 0, 2,  16727431,  0, 0,  162,  255,  252,  44,  0, 0);
          break;
        case 12:
          delayTimerSet5 = SEGMENT.FADE_TIME;
          modeSet5 = MODE_HAAN_FADE;
          setHaanSettings(0, 0, 6038015, 0, 0,  255, 255, 255, 10,  0, 0);
          break;
        default:
          break;
      }
      setNSettings(haanSpeed, haanIntensity, haanSEG0, haanSEG1, haanSEG2, haanFFTlow, haanFFTHigh, haanFFTCustom, haanPallet, haanBlandingPallet);
      Serial.print("Set5 Mode: ");Serial.print(modeSet5); Serial.print(", Timer Mode:");
      Serial.println(timersModeSet5);
      pixTimerSet5.setPeriod(delayTimerSet5);
      timersModeSet5++;
      timersModeSet5%=TIMER_MODES_SET5;
  }

  switch (modeSet5)
  {
    case MODE_CHASE:
      return mode_chase_color();
    case MODE_COLORWAVES:
      return mode_colorwaves();
    case MODE_COLORWINKLES:
      return mode_colortwinkle();
    case MODE_FIREWORKS:
      return mode_fireworks();
    case MODE_HAAN_FADE:
      return mode_haan_fade();
    case MODE_DRIP:
      return mode_drip();
    case MODE_FLOW:
      return mode_flow();
    case MODE_SOLID_GLITTER:
      return mode_solid_glitter();
    case MODE_PACIFICA:
      return mode_pacifica();
    default:
      break;
  }
}

/***** SET 6 *********/
uint32_t delayTimerSet6 = 0;
int timersModeSet6 = 0;
#define TIMER_MODES_SET6 2 //should be max mode +1
int modeSet6 = 0;
/**********************/

uint16_t WS2812FX::set6_color() {
  haanPalettes = true;
  uint16_t rate = set6();
  haanPalettes = false;
  return rate;
}

uint16_t WS2812FX::set6() {
  EVERY_N_MILLISECONDS_I(pixTimerSet6, delayTimerSet6) { 
      switch (timersModeSet6)
      {
        case 0:
          delayTimerSet6 = SEGMENT.BASE_TIME;
          modeSet6 = MODE_PHASED;
          setHaanSettings( 113, 50,  13107455,  0,  0,  255,  255,  255,  39,  0,0);
          break;
        case 1:
          delayTimerSet6 = SEGMENT.BASE_TIME;
          modeSet6 = MODE_PHASED_NOISE;
          setHaanSettings( 113, 50,  13107455,  0,  0,  255,  255,  255,  39,  0,0);
          break;
        default:
          break;
      }
      setNSettings(haanSpeed, haanIntensity, haanSEG0, haanSEG1, haanSEG2, haanFFTlow, haanFFTHigh, haanFFTCustom, haanPallet, haanBlandingPallet);
      Serial.print("Set6 Mode: ");Serial.print(modeSet6); Serial.print(", Timer Mode:");
      Serial.println(timersModeSet6);
      pixTimerSet6.setPeriod(delayTimerSet6);
      timersModeSet6++;
      timersModeSet6%=TIMER_MODES_SET6;
  }

  switch (modeSet6)
  {
  case MODE_PHASED:
    return mode_phased();
  case MODE_PHASED_NOISE:
    return mode_phased_noise();
  
  default:
    break;
  }
}

/***** SET 7 *********/
uint32_t delayTimerSet7 = 0;
int timersModeSet7 = 0;
#define TIMER_MODES_SET7 2 //should be max mode +1
int modeSet7 = 0;
/**********************/

uint16_t WS2812FX::set7_color() {
  haanPalettes = true;
  uint16_t rate = set7();
  haanPalettes = false;
  return rate;
}
uint16_t WS2812FX::set7() {
  EVERY_N_MILLISECONDS_I(pixTimerSet7, delayTimerSet7) { 
      switch (timersModeSet7)
      {
        case 0:
          delayTimerSet7 = SEGMENT.BASE_TIME;
          modeSet7 = MODE_PLASMA;
          setHaanSettings(  0,  103,  9175295,  0,  0,  255,  255,  255,  0,  0,192);
          break;
        case 1:
          delayTimerSet7 = SEGMENT.FADE_TIME;
          modeSet7 = MODE_HAAN_FADE;
          setHaanSettings(  0,  103,  9175295,  0,  0,  255,  255,  255,  0,  0,192);
          break;
        default:
          break;
      }
      setNSettings(haanSpeed, haanIntensity, haanSEG0, haanSEG1, haanSEG2, haanFFTlow, haanFFTHigh, haanFFTCustom, haanPallet, haanBlandingPallet);
      Serial.print("Set7 Mode: ");Serial.print(modeSet7); Serial.print(", Timer Mode:");
      Serial.println(timersModeSet7);
      pixTimerSet7.setPeriod(delayTimerSet7);
      timersModeSet7++;
      timersModeSet7%=TIMER_MODES_SET7;
  }

  switch (modeSet7)
  {
  case MODE_PLASMA:
    return mode_plasma();
  case MODE_HAAN_FADE:
    return mode_haan_fade();
  
  default:
    break;
  }
}

/***** SET 8 *********/
uint32_t delayTimerSet8 = 0;
int timersModeSet8 = 0;
#define TIMER_MODES_SET8 3 //should be max mode +1
int modeSet8 = 0;
/**********************/

uint16_t WS2812FX::set8_color() {
  haanPalettes = true;
  uint16_t rate = set8();
  haanPalettes = false;
  return rate;
}

uint16_t WS2812FX::set8() {
  EVERY_N_MILLISECONDS_I(pixTimerSet8, delayTimerSet8) { 
      switch (timersModeSet8)
      {
        case 0:
          delayTimerSet8 = SEGMENT.BASE_TIME;
          modeSet8 = MODE_RUNNING;
          setHaanSettings(101, 58,  8913151,  0,  0,  198,  128,  252,  0, 0, 0);
          break;

        case 1:
          delayTimerSet8 = SEGMENT.FADE_TIME;
          modeSet8 = MODE_HAAN_FADE;
          setHaanSettings(101, 58,  8913151,  0,  0,  198,  128,  252,  0, 0, 0);
          break;

        case 2:
          delayTimerSet8 = SEGMENT.BASE_TIME;
          modeSet8 = MODE_SAW;
          setHaanSettings( 128, 128,  8913151,  0,  0,  198,  128,  252,  0, 0, 0);
          break;

        default:
          break;
      }
      setNSettings(haanSpeed, haanIntensity, haanSEG0, haanSEG1, haanSEG2, haanFFTlow, haanFFTHigh, haanFFTCustom, haanPallet, haanBlandingPallet);
      Serial.print("Set8 Mode: ");Serial.print(modeSet8); Serial.print(", Timer Mode:");
      Serial.println(timersModeSet8);
      pixTimerSet8.setPeriod(delayTimerSet8);
      timersModeSet8++;
      timersModeSet8%=TIMER_MODES_SET8;
  }

  switch (modeSet8)
  {
  case MODE_RUNNING:
    return mode_running_lights();
  case MODE_SAW:
    return mode_saw();
  case MODE_HAAN_FADE:
    return mode_haan_fade();
  
  default:
    break;
  }
}

/***** SET 9 *********/
uint32_t delayTimerSet9 = 0;
int timersModeSet9 = 0;
#define TIMER_MODES_SET9 3 //should be max mode +1
int modeSet9 = 0;
/**********************/
uint16_t WS2812FX::set9() {
  EVERY_N_MILLISECONDS_I(pixTimerSet9, delayTimerSet9) { 
      switch (timersModeSet9)
      {
        case 0:
          delayTimerSet9 = SEGMENT.BASE_TIME;
          modeSet9 = MODE_SCAN;
          setHaanSettings(101,  58,  12517631,  2508799,  0,  255,  255,  252,  66,  0,0);
          break;

        case 1:
          delayTimerSet9 = SEGMENT.BASE_TIME;
          modeSet9 = MODE_DUAL_SCAN;
          setHaanSettings(101,  58,  12517631,  2508799,  0,  255,  255,  252,  66,  0,0);
          break;

        case 2:
          delayTimerSet9 = SEGMENT.FADE_TIME;
          modeSet9 = MODE_HAAN_FADE;
          setHaanSettings(101,  58,  9175295,  2508799,  0,  255,  255,  252,  66,  0,0);
          break;
        default:
          break;
      }
      setNSettings(haanSpeed, haanIntensity, haanSEG0, haanSEG1, haanSEG2, haanFFTlow, haanFFTHigh, haanFFTCustom, haanPallet, haanBlandingPallet);
      Serial.print("Set9 Mode: ");Serial.print(modeSet9); Serial.print(", Timer Mode:");
      Serial.println(timersModeSet9);
      pixTimerSet9.setPeriod(delayTimerSet9);
      timersModeSet9++;
      timersModeSet9%=TIMER_MODES_SET9;
  }

  switch (modeSet9)
  {
  case MODE_SCAN:
    return mode_scan();
  case MODE_DUAL_SCAN:
    return mode_dual_scan();
  case MODE_HAAN_FADE:
    return mode_haan_fade();
  
  default:
    break;
  }
}

/***** SET 10 *********/
uint32_t delayTimerSet10 = 0;
int timersModeSet10 = 0;
#define TIMER_MODES_SET10 4 //should be max mode +1
int modeSet10 = 0;
/**********************/
uint16_t WS2812FX::set10() {
  EVERY_N_MILLISECONDS_I(pixTimerSet10, delayTimerSet10) { 
      switch (timersModeSet10)
      {
        case 0:
          delayTimerSet10= SEGMENT.BASE_TIME;
          modeSet10 = MODE_SOLID_GLITTER;
          setHaanSettings( 0, 44,  16731549,  10162175,  0,  208,  255,  252,  0,  0, 0);
          break;

        case 1:
          delayTimerSet10 = SEGMENT.BASE_TIME;
          modeSet10 = MODE_SPARKLE_DARK;
          setHaanSettings(255,  58,  9175295,  0,  0,  255,  255,  252,  0,  0,0);
          break;

        case 2:
          delayTimerSet10 = SEGMENT.BASE_TIME;
          modeSet10 = MODE_SPARKLE_PLUS;
          setHaanSettings(255,  255,  9175295,  0,  0,  255,  255,  252,  0,  0,0);
          break;

        case 3:
          delayTimerSet10 = SEGMENT.FADE_TIME;
          modeSet10 = MODE_HAAN_FADE;
          setHaanSettings(101,  58,  9175295,  0,  0,  255,  255,  252,  0,  0,0);
          break;
        default:
          break;
      }
      setNSettings(haanSpeed, haanIntensity, haanSEG0, haanSEG1, haanSEG2, haanFFTlow, haanFFTHigh, haanFFTCustom, haanPallet, haanBlandingPallet);
      Serial.print("Set10 Mode: ");Serial.print(modeSet10); Serial.print(", Timer Mode:");
      Serial.println(timersModeSet10);
      pixTimerSet10.setPeriod(delayTimerSet10);
      timersModeSet10++;
      timersModeSet10%=TIMER_MODES_SET10;
  }

  switch (modeSet10)
  {
  case MODE_SOLID_GLITTER: 
    return mode_solid_glitter();
  case MODE_SPARKLE_DARK:
    return mode_flash_sparkle();
  case MODE_SPARKLE_PLUS:
    return mode_hyper_sparkle();
  case MODE_HAAN_FADE:
    return mode_haan_fade();
  
  default:
    break;
  }
}

/***** SET 11 *********/
uint32_t delayTimerSet11 = 0;
int timersModeSet11 = 0;
#define TIMER_MODES_SET11 3 //should be max mode +1
int modeSet11 = 0;
/**********************/
uint16_t WS2812FX::set11() {
   EVERY_N_MILLISECONDS_I(pixTimerSet11, delayTimerSet11) { 
      switch (timersModeSet11)
      {
        case 0:
          delayTimerSet11 = SEGMENT.BASE_TIME;
          modeSet11 = MODE_SPARKLE_DARK;
          setHaanSettings(255,  255,  9175295,  0,  0,  255,  255,  252,  0,  0,0);
          break;
        case 1:
          delayTimerSet11 = SEGMENT.FADE_TIME;
          modeSet11 = MODE_HAAN_FADE;
          setHaanSettings(101,  58,  9175295,  0,  0,  255,  255,  252,  0,  0,0);
          break;
        case 2:
          delayTimerSet11 = SEGMENT.BASE_TIME;
          modeSet11 = MODE_SOLID_GLITTER;
          setHaanSettings( 0, 44,  16731549,  10162175,  0,  208,  255,  252,  0,  0, 0);
          break;
        default:
          break;
      }
      setNSettings(haanSpeed, haanIntensity, haanSEG0, haanSEG1, haanSEG2, haanFFTlow, haanFFTHigh, haanFFTCustom, haanPallet, haanBlandingPallet);
      Serial.print("Set11 Mode: ");Serial.print(modeSet11); Serial.print(", Timer Mode:");
      Serial.println(timersModeSet11);
      pixTimerSet11.setPeriod(delayTimerSet11);
      timersModeSet11++;
      timersModeSet11%=TIMER_MODES_SET11;
  }

  switch (modeSet11)
  {
   case MODE_SPARKLE_DARK:
    return mode_flash_sparkle();
  case MODE_SOLID_GLITTER:
    return mode_solid_glitter();
  case MODE_HAAN_FADE:
    return mode_haan_fade();
  
  default:
    break;
  }
}

/***** SET 12 *********/
uint32_t delayTimerSet12 = 0;
int timersModeSet12 = 0;
#define TIMER_MODES_SET12 3 //should be max mode +1
int modeSet12 = 0;
/**********************/
uint16_t WS2812FX::set12() {
  EVERY_N_MILLISECONDS_I(pixTimerSet12, delayTimerSet12) { 
      switch (timersModeSet12)
      {
        case 0:
          delayTimerSet12 = SEGMENT.BASE_TIME;
          modeSet12 = MODE_TWO_AREAS;
          setHaanSettings(  101,  255,  8913151,  0,  0,  208,  255,  252,  0,  0, 0);
          break;

        case 1:
          delayTimerSet12 = SEGMENT.BASE_TIME;
          modeSet12 = MODE_TWO_DOTS;
          setHaanSettings(  101,  255,  8913151,  0,  0,  208,  255,  252,  0,  0, 0);
          break;

        case 2:
          delayTimerSet12 = SEGMENT.FADE_TIME;
          modeSet12 = MODE_HAAN_FADE;
          setHaanSettings(101,  58,  8913151,  0,  0,  255,  255,  252,  66,  0,0);
          break;
        default:
          break;
      }
      setNSettings(haanSpeed, haanIntensity, haanSEG0, haanSEG1, haanSEG2, haanFFTlow, haanFFTHigh, haanFFTCustom, haanPallet, haanBlandingPallet);
      Serial.print("Set12 Mode: ");Serial.print(modeSet12); Serial.print(", Timer Mode:");
      Serial.println(timersModeSet12);
      pixTimerSet12.setPeriod(delayTimerSet12);
      timersModeSet12++;
      timersModeSet12%=TIMER_MODES_SET12;
  }

  switch (modeSet12)
  {
  case MODE_TWO_AREAS:
    return mode_two_areas();
  case MODE_TWO_DOTS:
    return mode_two_dots();
  case MODE_HAAN_FADE:
    return mode_haan_fade();  
  
  default:
    break;
  }
}

/***** SET HAAN MIX *********/
uint32_t delayTimerSetHAANMix = 0;
int timersModeSetHAANMix = 0;
#define TIMER_MODES_SETHAANMix 68 //should be max mode +1
int modeSetHaanMix = 0;
/**********************/

uint16_t WS2812FX::set_haan_mix_color() {
  haanPalettes = true;
  uint16_t rate = set_haan_mix();
  haanPalettes = false;
  return rate;
}

uint16_t WS2812FX::set_haan_mix() {
  EVERY_N_MILLISECONDS_I(pixTimerSetHaanMix, delayTimerSetHAANMix) { 
    switch (timersModeSetHAANMix)
    {

    //SET1
    case 0:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = FFT_WALL;
      wallFFTDisabled = true;
      haanPalettes = false;
      setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
      break;
    
    case 1:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = FFT_WALL;
      wallFFTDisabled = false;
      haanPalettes = false;
      setHaanSettings(0,288,16752640, 0, 0,0,0,228,0,0,194);
      break;
    case 2:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = FFT_WALL;
      wallFFTDisabled = true;
      haanPalettes = false;
      setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
      break;
    
    case 3:
      delayTimerSetHAANMix = SEGMENT.FADE_TIME;
      modeSetHaanMix = MODE_HAAN_FADE;
      wallFFTDisabled = true;
      haanPalettes = false;
      setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
     break;

    case 4:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = FFT_SWIPE;
      wallFFTDisabled = true;
      haanPalettes = false;
      setHaanSettings(0,288,8913151, 0, 0,0,0,228,0,0,196);  //To remove - just for init testing
     break;
    
    case 5:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = RANDOM_FFT;
      wallFFTDisabled = true;
      haanPalettes = false;
      setHaanSettings(0,288,8913151, 0, 0,0,0,228,0,0,198);  //To remove - just for init testing
     break;

    case 6:
      delayTimerSetHAANMix = SEGMENT.FADE_TIME;
      modeSetHaanMix = MODE_HAAN_FADE;
      wallFFTDisabled = true;
      haanPalettes = false;
      setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
     break;

    case 7:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = FFT_BLINK;
      wallFFTDisabled = true;
      haanPalettes = false;
      setHaanSettings(0,288,8913151, 0, 0,0,0,228,0,0,198); //To remove - just for init testing
     break;

    case 8:
      delayTimerSetHAANMix = SEGMENT.FADE_TIME;
      modeSetHaanMix = MODE_HAAN_FADE;
      wallFFTDisabled = true;
      haanPalettes = false;
      setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
     break;

//SET0
    case 9:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME*3;
      modeSetHaanMix = FFT_FIB;
      haanPalettes = false;
     break;
//SET2

    case 10:
      delayTimerSetHAANMix = SEGMENT.FADE_TIME;
      modeSetHaanMix = MODE_HAAN_FADE;
      wallFFTDisabled = true;
      setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
      haanPalettes = false;
     break;

    case 11:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_2DDNA;
      haanPalettes = false;
      setHaanSettings(0, 255, 8913151, 3868159, 16715587, 255, 255, 143, 42, 0,198);
     break;

    case 12:
      delayTimerSetHAANMix = SEGMENT.FADE_TIME;
      modeSetHaanMix = MODE_HAAN_FADE;
      wallFFTDisabled = true;
      haanPalettes = false;
      setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
     break;
    
    case 13:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_2DSQUAREDSWIRL;
      haanPalettes = false;
      setHaanSettings(27, 255, 8913151, 3868159, 16715587, 43, 64, 81,  0, 0 ,200);
     break;

     case 14:
      delayTimerSetHAANMix = SEGMENT.FADE_TIME;
      modeSetHaanMix = MODE_HAAN_FADE;
      haanPalettes = false;
      setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
     break;
    
    //SET3
    case 15:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_2DPLASMA;
      haanPalettes = false;
      setHaanSettings(27,255,13107455,7536895, 0, 255,  255,  255, 0, 0, 202);
     break;

    case 16:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_JUGGLE;
      haanPalettes = false;
      //setHaanSettings(18,255,13107455,0, 0, 255,  255,  42, 0, 0, 0);
      setHaanSettings(18,255,16711931,0, 0, 255,  255,  42, 0, 0, 0);
     break;

    case 17:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_PIXELS;
      haanPalettes = false;
      setHaanSettings(18,69,15859967,0, 0, 255,  255,  42, 0, 0, 0);
     break;

    case 18:
      delayTimerSetHAANMix = SEGMENT.FADE_TIME;
      modeSetHaanMix = MODE_HAAN_FADE;
      haanPalettes = false;
      setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
     break;

    case 19:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_PLASMOID;
      haanPalettes = false;
      setHaanSettings(18,17,15926783,0, 0, 255,  255,  42, 0, 0, 0);
     break;

    case 20:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_FREQPIXEL;
      haanPalettes = false;
      setHaanSettings(3,17,13107455,0, 0, 255,  255,  42, 0, 0, 0);
     break;

    case 21:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_PUDDLEPEAK;
      haanPalettes = false;
      setHaanSettings(0,255,15598591,0, 0, 255,  255,  255, 0, 0, 0);
     break;

    case 22:
      delayTimerSetHAANMix = SEGMENT.FADE_TIME;
      modeSetHaanMix = MODE_HAAN_FADE;
      haanPalettes = false;
      setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
     break;
    //SET4
    case 23:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_2DMATRIX;
      haanPalettes = false;
      setHaanSettings(18,255,13107455,7536895, 0, 255,  255,  129, 0, 0, 0);
     break;
    
    case 24:
      delayTimerSetHAANMix = SEGMENT.FADE_TIME;
      modeSetHaanMix = MODE_HAAN_FADE;
      haanPalettes = false;
      setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
     break;

    //SET5
    case 25:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_CHASE;
      haanPalettes = false;
      setHaanSettings(73,255,15598591,15615, 0, 255,  255,  42, 0, 0, 0);
     break;

    case 26:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_HAAN_FADE;
      haanPalettes = false;
      setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
     break;

    case 27:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_COLORWAVES;
      haanPalettes = false;
      setHaanSettings(5,3,13107455,7536895, 0, 255,  255,  42, 58, 0, 0);
     break;

    case 28:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_COLORWINKLES;
      haanPalettes = false;
      setHaanSettings(5,3,13107455,7536895, 0, 255,  255,  42, 0, 0, 0);
     break;

    case 29:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_FIREWORKS;
      haanPalettes = false;
      setHaanSettings(0, 0, 6038015, 0, 0,  255, 255, 255, 10,  0, 0);
     break;

    case 30:
      delayTimerSetHAANMix = SEGMENT.FADE_TIME;
      modeSetHaanMix = MODE_HAAN_FADE;
      haanPalettes = false;
      setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
     break;

    case 31:
      delayTimerSetHAANMix = 1;
      modeSetHaanMix = MODE_HAAN_FADE;
      haanPalettes = false;
      setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
     break;

    case 32:
      delayTimerSetHAANMix = 1;
      modeSetHaanMix = MODE_HAAN_FADE;
      haanPalettes = false;
      setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
     break;

    case 33:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_FLOW;
      haanPalettes = false;
      setHaanSettings(27,255, 11403519,  0, 0,  43, 64, 81,  0,  0, 0);
     break;

    case 34:
      delayTimerSetHAANMix = 1;
      modeSetHaanMix = MODE_HAAN_FADE;
      haanPalettes = false;
      setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
     break;

    case 35:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_SOLID_GLITTER;
      haanPalettes = false;
      setHaanSettings(0, 44, 11403519, 10162175,  0, 208,  255,  252, 66, 0, 0);
     break;

    case 36:
      delayTimerSetHAANMix = SEGMENT.FADE_TIME;
      modeSetHaanMix = MODE_HAAN_FADE;
      haanPalettes = false;
      setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
     break;

    case 37:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_PACIFICA;
      haanPalettes = false;
      setHaanSettings( 0, 2,  16727431,  0, 0,  162,  255,  252,  39,  0, 0);
     break;

    case 38:
      delayTimerSetHAANMix = SEGMENT.FADE_TIME;
      modeSetHaanMix = MODE_HAAN_FADE;
      haanPalettes = false;
      setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
     break;
    //SET6
    case 39:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_PHASED;
      haanPalettes = false;
      setHaanSettings( 113, 50,  13107455,  0,  0,  255,  255,  255,  39,  0,0);
     break;
    case 40:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_PHASED;
      haanPalettes = false;
      setHaanSettings( 113, 50,  13107455,  0,  0,  255,  255,  255,  39,  0,0);
     break;
    
    case 41:
      delayTimerSetHAANMix = SEGMENT.FADE_TIME;
      modeSetHaanMix = MODE_HAAN_FADE;
      haanPalettes = false;
      setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
     break;
    
    //SET7

     case 42:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_PLASMA;
      haanPalettes = false;
      setHaanSettings(  0,  103,  9175295,  0,  0,  255,  255,  255,  0,  0,167);
     break;

    case 43:
      delayTimerSetHAANMix = SEGMENT.FADE_TIME;
      modeSetHaanMix = MODE_HAAN_FADE;
      haanPalettes = false;
      setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
     break;

    case 44:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_PLASMA;
      haanPalettes = false;
      setHaanSettings(  0,  103,  9175295,  0,  0,  255,  255,  255,  0,  0,192);
     break;

    case 45:
      delayTimerSetHAANMix = SEGMENT.FADE_TIME;
      modeSetHaanMix = MODE_HAAN_FADE;
      haanPalettes = false;
      setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
     break;

    case 46:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_PLASMA;
      haanPalettes = false;
      setHaanSettings(  0,  103,  9175295,  0,  0,  255,  255,  255,  0,  0,192);
     break;

    case 47:
      delayTimerSetHAANMix = SEGMENT.FADE_TIME;
      modeSetHaanMix = MODE_HAAN_FADE;
      haanPalettes = false;
      setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
     break;

    case 48:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_PLASMA;
      haanPalettes = false;
      setHaanSettings(  0,  103,  9175295,  0,  0,  255,  255,  255,  0,  0,235);
     break;

    case 49:
      delayTimerSetHAANMix = SEGMENT.FADE_TIME;
      modeSetHaanMix = MODE_HAAN_FADE;
      haanPalettes = false;
      setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
     break;
    //SET8

    case 50:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_RUNNING;
      haanPalettes = false;
      setHaanSettings(101, 58,  12255487,  0,  0,  198,  128,  252,  0, 0, 0);
     break;

    case 51:
      delayTimerSetHAANMix = SEGMENT.FADE_TIME;
      modeSetHaanMix = MODE_HAAN_FADE;
      haanPalettes = false;
      setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
     break;

     case 52:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_SAW;
      haanPalettes = false;
      setHaanSettings( 128, 128,  16711905,  0,  0,  198,  128,  252,  0, 0, 0);
     break;

    case 53:
      delayTimerSetHAANMix = SEGMENT.FADE_TIME;
      modeSetHaanMix = MODE_HAAN_FADE;
      haanPalettes = false;
      setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
     break;

    case 54:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_SAW;
      haanPalettes = false;
      setHaanSettings( 128, 128,  8913151,  0,  0,  198,  128,  252,  65, 0, 0);
     break;
    
    //SET9

    case 55:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_SCAN;
      haanPalettes = false;
      setHaanSettings(101,  156,  12517631,  2508799,  0,  255,  255,  252,  66,  0,0);
     break;

    case 56:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_DUAL_SCAN;
      haanPalettes = false;
      setHaanSettings(101,  120,  12517631,  2508799,  0,  255,  255,  252,  66,  0,0);
     break;

    case 57:
      delayTimerSetHAANMix = SEGMENT.FADE_TIME;
      modeSetHaanMix = MODE_HAAN_FADE;
      haanPalettes = false;
      setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
     break;

    //SET10

    case 58:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_SOLID_GLITTER;
      haanPalettes = false;
      setHaanSettings( 0, 44,  16731549,  10162175,  0,  208,  255,  252,  0,  0, 0);
     break;

    case 59:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_SPARKLE_DARK;
      haanPalettes = false;
      setHaanSettings(255,  58,  9175295,  0,  0,  255,  255,  252,  0,  0,0);
     break;

    case 60:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_SPARKLE_PLUS;
      haanPalettes = false;
      setHaanSettings(255,  255,  9175295,  0,  0,  255,  255,  252,  0,  0,0);
     break;

     case 61:
      delayTimerSetHAANMix = SEGMENT.FADE_TIME;
      modeSetHaanMix = MODE_HAAN_FADE;
      setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
     break;
    //SET11

    case 62:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_SPARKLE_DARK;
      haanPalettes = false;
      setHaanSettings(255,  255,  15335679,  0,  0,  255,  255,  252,  0,  0,0);
     break;

    case 63:
      delayTimerSetHAANMix = SEGMENT.FADE_TIME;
      modeSetHaanMix = MODE_HAAN_FADE;
      haanPalettes = false;
      setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
     break;

    case 64:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_SOLID_GLITTER;
      haanPalettes = false;
      setHaanSettings( 0, 44,  15335679,  10162175,  0,  208,  255,  252,  0,  0, 0);
     break;

    
    //SET12

    case 65:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_TWO_AREAS;
      haanPalettes = false;
      setHaanSettings(  101,  255,  16720265,  0,  0,  208,  255,  252,  0,  0, 0);
     break;

    case 66:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_TWO_DOTS;
      haanPalettes = false;
      setHaanSettings(  101,  255,  16720265,  0,  0,  208,  255,  252,  0,  0, 0);
     break;

    case 67:
      delayTimerSetHAANMix = SEGMENT.FADE_TIME;
      modeSetHaanMix = MODE_HAAN_FADE;
      haanPalettes = false;
      setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
     break;
    default:
      break;
    }
    setNSettings(haanSpeed, haanIntensity, haanSEG0, haanSEG1, haanSEG2, haanFFTlow, haanFFTHigh, haanFFTCustom, haanPallet, haanBlandingPallet);
    Serial.print("SetHaanMix Mode: ");Serial.print(modeSetHaanMix); Serial.print(", Timer Mode:");
    Serial.println(timersModeSetHAANMix);
    pixTimerSetHaanMix.setPeriod(delayTimerSetHAANMix);
    timersModeSetHAANMix++;
    timersModeSetHAANMix%=TIMER_MODES_SETHAANMix;
  }

  switch (modeSetHaanMix)
  {
  case FFT_WALL:
    return mode_fft_wall();
  case MODE_HAAN_FADE:
    return mode_haan_fade();
  case FFT_SWIPE:
    return mode_fft_swiping();
  case RANDOM_FFT:
    return mode_random_fft();
  case FFT_BLINK:
    return fft_blink();
  case FFT_FIB:
    return mode_fft_fib();
  case MODE_2DSQUAREDSWIRL:
    return mode_2Dsquaredswirl();
  case MODE_2DDNA:
    return mode_2Ddna();
  case MODE_2DPLASMA:
    return mode_2Dplasma();
  case MODE_JUGGLE:
    return mode_juggles();
  case MODE_PIXELS:
    return mode_pixels();
  case MODE_PLASMOID:
    return mode_plasmoid();
  case MODE_FREQPIXEL:
    return mode_freqpixel();
  case MODE_PUDDLEPEAK:
    return mode_puddlepeak();
  case MODE_2DMATRIX:
    return mode_2Dmatrix();
  case MODE_TWO_AREAS:
    return mode_two_areas();
  case MODE_TWO_DOTS:
    return mode_two_dots();
  case MODE_SPARKLE_DARK:
    return mode_flash_sparkle();
  case MODE_SOLID_GLITTER:
    return mode_solid_glitter();
  case MODE_SPARKLE_PLUS:
    return mode_hyper_sparkle();  
  case MODE_SCAN:
    return mode_scan();
  case MODE_DUAL_SCAN:
    return mode_dual_scan();
  case MODE_RUNNING:
    return mode_running_lights();
  case MODE_SAW:
    return mode_saw();
  case MODE_PLASMA:
    return mode_plasma();
  case MODE_PHASED:
    return mode_phased();
  case MODE_PHASED_NOISE:
    return mode_phased_noise();
  case MODE_CHASE:
    return mode_chase_color();
  case MODE_COLORWAVES:
    return mode_colorwaves();
  case MODE_COLORWINKLES:
    return mode_colortwinkle();
  case MODE_FIREWORKS:
    return mode_fireworks();
  case MODE_FLOW:
    return mode_flow();
  case MODE_DRIP:
    return mode_drip();
  case MODE_PACIFICA:
    return mode_pacifica();

    
  default:
    break;
  }
}




/***** SET AUTO HAAN *********/
uint32_t delayTimerSetAutoHAAN = 0;
int timersModeSetAutoHAAN = 0;
#define TIMER_MODES_SET_AUTO_HAAN 4 //should be max mode +1
int modeSetAutoHaan = 0;
bool firstTimeSettings = true;
/**********************/
uint16_t WS2812FX::auto_haan() {

  if (firstTimeSettings) {
    firstTimeSettings = false;
    SEGMENT.BASE_TIME = DEFAULT_BASE_TIME;
    SEGMENT.FADE_TIME = DEFAULT_FADE_TIME;
  }

  EVERY_N_MILLISECONDS_I(pixTimerAutoMix, delayTimerSetAutoHAAN) { 
    switch (timersModeSetAutoHAAN)
    {
    case 0:
      delayTimerSetAutoHAAN = SEGMENT.BASE_TIME*44 + SEGMENT.FADE_TIME*23; 
      modeSetAutoHaan = AUTO_HAAN_MIX;
      break;
    case 1:
      delayTimerSetAutoHAAN = SEGMENT.BASE_TIME*52 + SEGMENT.FADE_TIME*22 + 15000 +4000;
      modeSetAutoHaan = FX_AUTO_HAAN_INTENCE;
      break;

    case 2:
      delayTimerSetAutoHAAN = SEGMENT.BASE_TIME*44 + SEGMENT.FADE_TIME*23;;
      modeSetAutoHaan = AUTOH_HAAN_MIX_COLLOR;
      break;  
    
    case 3:
      delayTimerSetAutoHAAN = SEGMENT.BASE_TIME*52 + SEGMENT.FADE_TIME*22 + 15000 +4000;
      modeSetAutoHaan = FX_AUTO_HAAN_INTENCE;
      break;  
    
    default:
      break;
    }
    setNSettings(haanSpeed, haanIntensity, haanSEG0, haanSEG1, haanSEG2, haanFFTlow, haanFFTHigh, haanFFTCustom, haanPallet, haanBlandingPallet);
    Serial.print("Auto Haan Mode: ");Serial.print(modeSetAutoHaan); Serial.print(", Timer Mode:");
    Serial.println(timersModeSetAutoHAAN);
    pixTimerAutoMix.setPeriod(delayTimerSetAutoHAAN);
    timersModeSetAutoHAAN++;
    timersModeSetAutoHAAN%=TIMER_MODES_SET_AUTO_HAAN;
  }
  switch (modeSetAutoHaan)
  {
  case AUTO_HAAN_MIX:
    return set_haan_mix();
  case AUTOH_HAAN_MIX_COLLOR:
    return set_haan_mix_color();  
  case FX_AUTO_HAAN_INTENCE:
    return auto_haan_intence();
  default:
    break;
  }

}


/***** SET AUTO HAAN INTENCE*********/
uint32_t delayTimerSetAutoHAANIntence = 0;
int timersModeSetAutoHAANIntence = 0;
#define TIMER_MODES_SET_HAAN_INTENCE 55 //should be max mode +1
int modeSetAutoHaanIntence = 0;
/**********************/
uint16_t WS2812FX::auto_haan_intence() {
   EVERY_N_MILLISECONDS_I(pixTimerAutoIntence, delayTimerSetAutoHAANIntence) { 
     switch (timersModeSetAutoHAANIntence)
     {
      case 0:
          delayTimerSetAutoHAANIntence = SEGMENT.BASE_TIME;
          modeSetAutoHaanIntence = FX_FFT_PUDDLES;
          //find best pallet
          haanPalettes = false;
          setHaanSettings(5,249,16753152, 0, 0,255,255,19,19,0,0);
         break;
      case 1:
          delayTimerSetAutoHAANIntence = SEGMENT.FADE_TIME;
          modeSetAutoHaanIntence = FX_HAAN_FADE;
          haanPalettes = false;
          setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
          break;
      case 2:
          delayTimerSetAutoHAANIntence = SEGMENT.BASE_TIME;
          modeSetAutoHaanIntence = FFT_WALL;
          wallFFTDisabled = true;
          haanPalettes = false;
          setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
          break;
      case 3:
          delayTimerSetAutoHAANIntence = SEGMENT.BASE_TIME;
          modeSetAutoHaanIntence = FFT_WALL;
          wallFFTDisabled = false;
          haanPalettes = false;
          setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
          break;
      case 4:
          delayTimerSetAutoHAANIntence = SEGMENT.FADE_TIME;
          modeSetAutoHaanIntence = FX_HAAN_FADE;
          haanPalettes = false;
          setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
          break;
      case 5:
          delayTimerSetAutoHAANIntence = SEGMENT.BASE_TIME;
          modeSetAutoHaanIntence = MODE_PLASMA;
          haanPalettes = false;
          setHaanSettings(  0,  103,  9175295,  0,  0,  255,  255,  255,  0,  0,192);
          break;
      case 6:
          delayTimerSetAutoHAANIntence = SEGMENT.FADE_TIME;
          modeSetAutoHaanIntence = FX_HAAN_FADE;
          haanPalettes = false;
          setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
          break;
      case 7:
          delayTimerSetAutoHAANIntence = SEGMENT.BASE_TIME;
          modeSetAutoHaanIntence = MODE_PLASMA;
          haanPalettes = false;
          setHaanSettings(  0,  103,  9175295,  0,  0,  255,  255,  255,  0,  0,192);
          break;
      case 8:
          delayTimerSetAutoHAANIntence = SEGMENT.FADE_TIME;
          modeSetAutoHaanIntence = FX_HAAN_FADE;
          haanPalettes = false;
          setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
          break;
      case 9:
          delayTimerSetAutoHAANIntence = SEGMENT.BASE_TIME;
          modeSetAutoHaanIntence = MODE_PLASMA;
          haanPalettes = false;
          setHaanSettings(  0,  103,  9175295,  0,  0,  255,  255,  255,  0,  0,192);
          break;
      case 10:
          delayTimerSetAutoHAANIntence = SEGMENT.FADE_TIME;
          modeSetAutoHaanIntence = FX_HAAN_FADE;
          haanPalettes = false;
          setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
          break;
      case 11:
          delayTimerSetAutoHAANIntence = SEGMENT.BASE_TIME;
          modeSetAutoHaanIntence = MODE_PLASMA;
          haanPalettes = false;
          setHaanSettings(  0,  103,  9175295,  0,  0,  255,  255,  255,  0,  0,192);
          break;
      case 12:
          delayTimerSetAutoHAANIntence = SEGMENT.FADE_TIME;
          modeSetAutoHaanIntence = FX_HAAN_FADE;
          haanPalettes = false;
          setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
          break;
      case 13:
          // delayTimerSetAutoHAANIntence = 1;
          // modeSetAutoHaanIntence = FX_HAAN_FADE;
          // haanPalettes = false;
          // setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
          break;
      case 14:
          // delayTimerSetAutoHAANIntence = 1;
          // modeSetAutoHaanIntence = FX_HAAN_FADE;
          // haanPalettes = false;
          // setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
          break;
      case 15:
          delayTimerSetAutoHAANIntence = 15000;
          modeSetAutoHaanIntence = MODE_DRIP;
          haanPalettes = false;
          setHaanSettings(128, 255, 16750600,0,  0, 255, 255,  9,  13,  33, 0); //Verify its working
          break;
      case 16:
          delayTimerSetAutoHAANIntence = SEGMENT.FADE_TIME;
          modeSetAutoHaanIntence = FX_HAAN_FADE;
          haanPalettes = false;
          setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
          break;
      case 17:
          delayTimerSetAutoHAANIntence = SEGMENT.BASE_TIME;
          modeSetAutoHaanIntence = MODE_FIREWORKS;
          haanPalettes = false;
          setHaanSettings(0, 0, 6038015, 0, 0,  255, 255, 255, 10,  0, 0);
          break;
      case 18:
          delayTimerSetAutoHAANIntence = SEGMENT.FADE_TIME;
          modeSetAutoHaanIntence = FX_HAAN_FADE;
          haanPalettes = false;
          setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
          break;
      case 19:
          delayTimerSetAutoHAANIntence = SEGMENT.BASE_TIME;
          modeSetAutoHaanIntence = MODE_SOLID_GLITTER;
          haanPalettes = false;
          setHaanSettings(0,104,9175295, 0, 0,255,255,252,0,0,0);
          break;
      case 20:
          delayTimerSetAutoHAANIntence = SEGMENT.FADE_TIME;
          modeSetAutoHaanIntence = FX_HAAN_FADE;
          haanPalettes = false;
          setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
          break;
      case 21:
          delayTimerSetAutoHAANIntence = SEGMENT.BASE_TIME;
          modeSetAutoHaanIntence = MODE_2DMATRIX;
          //horizonatl
          haanPalettes = false;
         setHaanSettings(18,255,13107455,7536895, 0, 255,  255,  105, 0, 0, 0);
          break;
      case 22:
          delayTimerSetAutoHAANIntence = SEGMENT.BASE_TIME;
          modeSetAutoHaanIntence = MODE_2DMATRIX;
          haanPalettes = false;
          //vertical
         setHaanSettings(18,255,13107455,7536895, 0, 255,  255,  173, 0, 0, 0);
          break;

      case 23:
          delayTimerSetAutoHAANIntence = SEGMENT.BASE_TIME;
          modeSetAutoHaanIntence = MODE_2DMATRIX;
          haanPalettes = false;
          //horizontal
          setHaanSettings(18,255,13107455,7536895, 0, 255,  255,  105, 0, 0, 0);
          break;
       
       case 24:
          delayTimerSetAutoHAANIntence = SEGMENT.BASE_TIME;
          modeSetAutoHaanIntence = MODE_2DMATRIX;
          haanPalettes = false;
          //vertical
          setHaanSettings(18,255,13107455,7536895, 0, 255,  255,  173, 0, 0, 0);
          break;


      case 25:
          delayTimerSetAutoHAANIntence = SEGMENT.FADE_TIME;
          modeSetAutoHaanIntence = FX_HAAN_FADE;
          haanPalettes = false;
          setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
          break;

      case 26:
          delayTimerSetAutoHAANIntence = SEGMENT.BASE_TIME*3;
          modeSetAutoHaanIntence = MODE_2DSQUAREDSWIRL;
          haanPalettes = true;
          setHaanSettings(27, 255, 8913151, 3868159, 16715587, 43, 64, 81,  0, 0 ,32);
          break;
      
      case 27:
          delayTimerSetAutoHAANIntence = SEGMENT.FADE_TIME;
          modeSetAutoHaanIntence = FX_HAAN_FADE;
          haanPalettes = false;
          setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
          break;    

      case 28:
          delayTimerSetAutoHAANIntence = 4000;
          modeSetAutoHaanIntence = FX_MODE_HEARTBEAT;
          haanPalettes = false;
          setHaanSettings(0,192,4798719, 0, 0,0,0,228,0,0,0);
          break;  

      case 29:
          delayTimerSetAutoHAANIntence = SEGMENT.FADE_TIME;
          modeSetAutoHaanIntence = FX_HAAN_FADE;
          haanPalettes = false;
          setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
          break;  

      case 30:
          delayTimerSetAutoHAANIntence = SEGMENT.BASE_TIME;
          modeSetAutoHaanIntence = FX_MODE_RIPPLE;
          haanPalettes = false;
          setHaanSettings(0,255,11403519, 0, 0,255,255,255,0,0,0);
          break; 

      case 31:
          delayTimerSetAutoHAANIntence = SEGMENT.FADE_TIME;
          modeSetAutoHaanIntence = FX_HAAN_FADE;
          haanPalettes = false;
          setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
          break;  

      case 32:
          delayTimerSetAutoHAANIntence = SEGMENT.BASE_TIME;
          modeSetAutoHaanIntence = MODE_TWO_AREAS;
          haanPalettes = false;
          setHaanSettings(133,  243,  1638655,  16000767,  0,  208,  255,  252,  0,  0, 0);
          break;  


      case 33:
          delayTimerSetAutoHAANIntence = SEGMENT.FADE_TIME;
          modeSetAutoHaanIntence = FX_HAAN_FADE;
          haanPalettes = false;
          setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
          break;

      case 34:
          delayTimerSetAutoHAANIntence = SEGMENT.BASE_TIME;
          modeSetAutoHaanIntence = MODE_TWO_DOTS;
          haanPalettes = false;
          setHaanSettings(133,  243,  1638655,  16000767,  0,  208,  255,  252,  0,  0, 0);
          break; 

      case 35:
          delayTimerSetAutoHAANIntence = SEGMENT.FADE_TIME;
          modeSetAutoHaanIntence = FX_HAAN_FADE;
          haanPalettes = false;
          setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
          break;

      case 36:
          delayTimerSetAutoHAANIntence = SEGMENT.BASE_TIME;
          modeSetAutoHaanIntence = MODE_SPARKLE;
          haanPalettes = false;
          //sparkle with yello
          setHaanSettings(255,  128,  16752640,  0,  0,  152,  181,  143,  0,  0, 0);
          break;

      case 37:
          delayTimerSetAutoHAANIntence = SEGMENT.BASE_TIME;
          modeSetAutoHaanIntence = MODE_SPARKLE;
          shouldAddGlitterPro = true;
          haanPalettes = false;
          setHaanSettings(255,  128,  16752640,  0,  0,  152,  181,  143,  0,  0, 218);
          break;

      case 38:
          delayTimerSetAutoHAANIntence = SEGMENT.BASE_TIME;
          modeSetAutoHaanIntence = MODE_SPARKLE;
          shouldAddGlitterPro = true; // - to disable prev state
          //sparkle with yello
          haanPalettes = false;
          setHaanSettings(255,  128,  13631743,  0,  0,  152,  181,  143,  0,  0, 218);
          break;

      case 39:
          delayTimerSetAutoHAANIntence = SEGMENT.FADE_TIME;
          modeSetAutoHaanIntence = FX_HAAN_FADE;
             shouldAddGlitterPro = false; // - to disable prev state
          haanPalettes = false;
          setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
          break;

      case 40:
          delayTimerSetAutoHAANIntence = SEGMENT.BASE_TIME*5;
          modeSetAutoHaanIntence = FFT_FIB;
          haanPalettes = false;
          //setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
          break;
      case 41:
          delayTimerSetAutoHAANIntence = SEGMENT.FADE_TIME;
          modeSetAutoHaanIntence = FX_HAAN_FADE;
          haanPalettes = false;
          setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
          break;    
      case 42:
          delayTimerSetAutoHAANIntence = 4000;
          modeSetAutoHaanIntence = FX_MODE_HEARTBEAT;
          haanPalettes = false;
          setHaanSettings(0,192,13631743, 0, 0,0,0,228,0,0,0);
          break;
      case 43:
          delayTimerSetAutoHAANIntence = SEGMENT.FADE_TIME;
          modeSetAutoHaanIntence = FX_HAAN_FADE;
          haanPalettes = false;
          setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
          break;   
      case 44:
          delayTimerSetAutoHAANIntence = SEGMENT.BASE_TIME;
          modeSetAutoHaanIntence = MODE_PLASMA;
          haanPalettes = false;
          setHaanSettings(164,167,16755200, 0, 0,65,109,65,0,0,0);
          break; 
      case 45:
          delayTimerSetAutoHAANIntence = SEGMENT.FADE_TIME;
          modeSetAutoHaanIntence = FX_HAAN_FADE;
          haanPalettes = false;
          setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
          break;
      case 46:
          delayTimerSetAutoHAANIntence = SEGMENT.BASE_TIME;
          modeSetAutoHaanIntence =  MODE_SPARKLE;
          haanPalettes = false;
          setHaanSettings(255,  128,  16755200,  255,  0,  152,  181,  143,  0,  0, 0);
          shouldAddGlitterPro = true;
          break;               
      case 47:
          delayTimerSetAutoHAANIntence = SEGMENT.FADE_TIME;
          modeSetAutoHaanIntence = FX_HAAN_FADE;
          haanPalettes = false;
          shouldAddGlitterPro = false; //to disable prev stage
          setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
          break;   
      case 48:
          // delayTimerSetAutoHAANIntence = 1;
          // modeSetAutoHaanIntence = FX_HAAN_FADE;
          // haanPalettes = false;
          // shouldAddGlitterPro = false; //to disable prev stage
          // setHaanSettings(12,255,16755200, 0, 0,205,200,228,223,0,0);
          break;    
      case 49:
          // delayTimerSetAutoHAANIntence = SEGMENT.FADE_TIME;
          // modeSetAutoHaanIntence = FX_HAAN_FADE;
          // haanPalettes = false;
          // shouldAddGlitterPro = false; //to disable prev stage
          // setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
           break; 
      case 50:
          delayTimerSetAutoHAANIntence = SEGMENT.BASE_TIME;
          modeSetAutoHaanIntence = FFT_BLINK;
          wallFFTDisabled = true;
          haanPalettes = false;
          setHaanSettings(0,288,8913151, 0, 0,0,0,228,0,0,198); //To remove - just for init testing
          break;     
      case 51:
          delayTimerSetAutoHAANIntence = SEGMENT.FADE_TIME;
          modeSetAutoHaanIntence = FX_HAAN_FADE;
          haanPalettes = false;
          shouldAddGlitterPro = true; //to disable prev stage
          setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
          break;  
       case 52:
          delayTimerSetAutoHAANIntence = SEGMENT.BASE_TIME;
          modeSetAutoHaanIntence = FFT_SWIPE;
          wallFFTDisabled = true;
          haanPalettes = false;
          setHaanSettings(0,288,8913151, 0, 0,0,0,228,0,0,196);  //To remove - just for init testing
        break;
        
        case 53:
          delayTimerSetAutoHAANIntence = SEGMENT.BASE_TIME;
          modeSetAutoHaanIntence = RANDOM_FFT;
          wallFFTDisabled = true;
          haanPalettes = false;
          setHaanSettings(0,288,8913151, 0, 0,0,0,228,0,0,198);  //To remove - just for init testing
        break;    
      
      case 54:
          delayTimerSetAutoHAANIntence = SEGMENT.FADE_TIME;
          modeSetAutoHaanIntence = FX_HAAN_FADE;
          haanPalettes = false;
          shouldAddGlitterPro = true; //to disable prev stage
          setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
          break;
     default:
       break;
     }
    setNSettings(haanSpeed, haanIntensity, haanSEG0, haanSEG1, haanSEG2, haanFFTlow, haanFFTHigh, haanFFTCustom, haanPallet, haanBlandingPallet);
    Serial.print("Auto Haan Intence Mode: ");Serial.print(modeSetAutoHaanIntence); Serial.print(", Timer Mode:");
    Serial.println(timersModeSetAutoHAANIntence);
    pixTimerAutoIntence.setPeriod(delayTimerSetAutoHAANIntence);
    timersModeSetAutoHAANIntence++;
    timersModeSetAutoHAANIntence%=TIMER_MODES_SET_HAAN_INTENCE;
  }

  switch (modeSetAutoHaanIntence)
  {
    case FX_FFT_PUDDLES:
      return mode_puddles_fft();
    case FX_HAAN_FADE:
      return mode_haan_fade();
    case FFT_WALL: 
      return mode_fft_wall();
    case MODE_PLASMA:
      return mode_plasma();
    case MODE_DRIP:
      return mode_drip();
    case MODE_SOLID_GLITTER:
      return mode_solid_glitter();
    case MODE_2DMATRIX:
      return mode_2Dmatrix();
    case MODE_2DSQUAREDSWIRL:
      return mode_2Dsquaredswirl();
    case FX_MODE_HEARTBEAT:
      return mode_heartbeat();
    case FX_MODE_RIPPLE:
      return mode_ripple();
    case MODE_TWO_AREAS:
      return mode_two_areas();
    case MODE_TWO_DOTS:
      return mode_two_dots();
    case MODE_SPARKLE:
      return mode_sparkle();
    case FFT_FIB:
      return mode_fft_fib();
    case MODE_FIREWORKS:
      return mode_fireworks();
    case FX_MODE_BOUNCINGBALLS:
      return mode_bouncing_balls();
    case FFT_SWIPE:
      return mode_fft_swiping();
    case FFT_BLINK:
      return fft_blink();
    case RANDOM_FFT:
      return mode_random_fft();

  default:
    break;
  }
  
}


///////////////////////////////////////////////////////////////////////////////////
            // Haan Blue settings copy//

/***** SET HAAN MIX *********/
//uint32_t delayTimerSetHAANMix = 0;
//int timersModeSetHAANMix = 0;
//#define TIMER_MODES_SETHAANMix 64 //should be max mode +1
//int modeSetHaanMix = 0;
/**********************/
/*
uint16_t WS2812FX::set_haan_mix() {
  EVERY_N_MILLISECONDS_I(pixTimerSetHaanMix, delayTimerSetHAANMix) { 
    switch (timersModeSetHAANMix)
    {

    //SET1
    case 0:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = FFT_WALL;
      wallFFTDisabled = true;
      setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
      break;
    
    case 1:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = FFT_WALL;
      wallFFTDisabled = false;
      setHaanSettings(0,288,16752640, 0, 0,0,0,228,0,0,192);
      break;
    case 2:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = FFT_WALL;
      wallFFTDisabled = true;
      setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
      break;
    
    case 3:
      delayTimerSetHAANMix = 2000;
      modeSetHaanMix = MODE_HAAN_FADE;
      wallFFTDisabled = true;
      setHaanSettings(0,192,0, 0, 0,0,0,228,0,0,0);
     break;

    case 4:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = FFT_SWIPE;
      wallFFTDisabled = true;
      setHaanSettings(0,288,8913151, 0, 0,0,0,228,0,0,192);  //To remove - just for init testing
     break;
    
    case 5:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = RANDOM_FFT;
      wallFFTDisabled = true;
      setHaanSettings(0,288,8913151, 0, 0,0,0,228,0,0,192);  //To remove - just for init testing
     break;

    case 6:
      delayTimerSetHAANMix = 2000;
      modeSetHaanMix = MODE_HAAN_FADE;
      wallFFTDisabled = true;
      setHaanSettings(0,288,8913151, 0, 0,0,0,228,0,0,192); //To remove - just for init testing
     break;

    case 7:
      delayTimerSetHAANMix = 2000;
      modeSetHaanMix = FFT_BLINK;
      wallFFTDisabled = true;
      setHaanSettings(0,288,8913151, 0, 0,0,0,228,0,0,192); //To remove - just for init testing
     break;

//SET0
    case 8:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = FFT_FIB;
     break;
//SET2

    case 9:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_2DDNA;
      setHaanSettings(0, 255, 8913151, 3868159, 16715587, 255, 255, 143, 42, 0,192);
     break;
    
    case 10:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_2DSQUAREDSWIRL;
      setHaanSettings(27, 255, 8913151, 3868159, 16715587, 43, 64, 81,  0, 0 ,192);
     break;

     case 11:
      delayTimerSetHAANMix = 2000;
      modeSetHaanMix = MODE_HAAN_FADE;
      setHaanSettings(0,255,13107455,0, 0, 255,  255,  255, 0, 0, 0);
     break;
    
    //SET3
    case 12:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_2DPLASMA;
      setHaanSettings(27,255,13107455,7536895, 0, 255,  255,  255, 0, 0, 192);
     break;

    case 13:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_JUGGLE;
      setHaanSettings(18,255,13107455,0, 0, 255,  255,  42, 0, 0, 0);
     break;

    case 14:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_PIXELS;
      setHaanSettings(18,69,13107455,0, 0, 255,  255,  42, 0, 0, 0);
     break;

    case 15:
      delayTimerSetHAANMix = 2000;
      modeSetHaanMix = MODE_HAAN_FADE;
      setHaanSettings(0,255,13107455,0, 0, 255,  255,  255, 0, 0, 0);
     break;

    case 16:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_PLASMOID;
      setHaanSettings(18,17,13107455,0, 0, 255,  255,  42, 0, 0, 0);
     break;

    case 17:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_FREQPIXEL;
      setHaanSettings(3,17,13107455,0, 0, 255,  255,  42, 0, 0, 0);
     break;

    case 18:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_PUDDLEPEAK;
      setHaanSettings(0,255,13107455,0, 0, 255,  255,  255, 0, 0, 0);
     break;

    case 19:
      delayTimerSetHAANMix = 2000;
      modeSetHaanMix = MODE_HAAN_FADE;
      setHaanSettings(0,255,13107455,0, 0, 255,  255,  255, 0, 0, 0);
     break;
    //SET4
    case 20:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_2DMATRIX;
      setHaanSettings(18,255,13107455,7536895, 0, 255,  255,  129, 0, 0, 0);
     break;
    
    case 21:
      delayTimerSetHAANMix = 2000;
      modeSetHaanMix = MODE_HAAN_FADE;
      setHaanSettings(18,255,13107455,0, 0, 255,  255,  42, 0, 0, 0);
     break;

    //SET5
    case 22:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_CHASE;
      setHaanSettings(73,255,13107455,7536895, 0, 255,  255,  42, 0, 0, 0);
     break;

    case 23:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_CHASE;
      setHaanSettings(73,255,13107455,7536895, 0, 255,  255,  42, 0, 0, 0);
     break;

    case 24:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_COLORWAVES;
      setHaanSettings(5,3,13107455,7536895, 0, 255,  255,  42, 0, 0, 0);
     break;

    case 25:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_COLORWINKLES;
      setHaanSettings(5,3,13107455,7536895, 0, 255,  255,  42, 0, 0, 0);
     break;

    case 26:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_FIREWORKS;
      setHaanSettings(0, 0, 6038015, 0, 0,  255, 255, 255, 10,  0, 0);
     break;

    case 27:
      delayTimerSetHAANMix = 2000;
      modeSetHaanMix = MODE_HAAN_FADE;
      setHaanSettings(0, 0, 6038015, 0, 0,  255, 255, 255, 10,  0, 0);
     break;

    case 28:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_DRIP;
      setHaanSettings(0, 211, 6038015,0,  0, 83, 255,  255,  28,  0, 0);
     break;

    case 29:
      delayTimerSetHAANMix = 2000;
      modeSetHaanMix = MODE_HAAN_FADE;
      setHaanSettings(0, 211, 6038015,0,  0, 83, 255,  255,  28,  0, 0);
     break;

    case 30:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_FLOW;
      setHaanSettings(27,255, 7618047,  0, 0,  43, 64, 81,  0,  0, 0);
     break;

    case 31:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_SOLID_GLITTER;
      setHaanSettings(0, 44, 16731549, 10162175,  0, 208,  255,  252, 66, 0, 0);
     break;

    case 32:
      delayTimerSetHAANMix = 2000;
      modeSetHaanMix = MODE_HAAN_FADE;
      setHaanSettings(0, 0, 6038015, 0, 0,  255, 255, 255, 10,  0, 0);
     break;

    case 33:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_PACIFICA;
      setHaanSettings( 0, 2,  16727431,  0, 0,  162,  255,  252,  44,  0, 0);
     break;

    case 34:
      delayTimerSetHAANMix = 2000;
      modeSetHaanMix = MODE_HAAN_FADE;
      setHaanSettings( 0, 2,  16727431,  0, 0,  162,  255,  252,  44,  0, 0);
     break;
    //SET6
    case 35:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_PHASED;
      setHaanSettings( 113, 50,  13107455,  0,  0,  255,  255,  255,  39,  0,0);
     break;
    case 36:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_PHASED;
      setHaanSettings( 113, 50,  13107455,  0,  0,  255,  255,  255,  39,  0,0);
     break;
    
    case 37:
      delayTimerSetHAANMix = 2000;
      modeSetHaanMix = MODE_HAAN_FADE;
      setHaanSettings( 0, 2,  16727431,  0, 0,  162,  255,  252,  44,  0, 0);
     break;
    
    //SET7

     case 38:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_PLASMA;
      setHaanSettings(  0,  103,  9175295,  0,  0,  255,  255,  255,  0,  0,192);
     break;

    case 39:
      delayTimerSetHAANMix = 2000;
      modeSetHaanMix = MODE_HAAN_FADE;
      setHaanSettings( 0, 2,  16727431,  0, 0,  162,  255,  252,  44,  0, 0);
     break;

    case 40:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_PLASMA;
      setHaanSettings(  0,  103,  9175295,  0,  0,  255,  255,  255,  0,  0,192);
     break;

    case 41:
      delayTimerSetHAANMix = 2000;
      modeSetHaanMix = MODE_HAAN_FADE;
      setHaanSettings( 0, 2,  16727431,  0, 0,  162,  255,  252,  44,  0, 0);
     break;

    case 42:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_PLASMA;
      setHaanSettings(  0,  103,  9175295,  0,  0,  255,  255,  255,  0,  0,192);
     break;

    case 43:
      delayTimerSetHAANMix = 2000;
      modeSetHaanMix = MODE_HAAN_FADE;
      setHaanSettings( 0, 2,  16727431,  0, 0,  162,  255,  252,  44,  0, 0);
     break;

    case 44:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_PLASMA;
      setHaanSettings(  0,  103,  9175295,  0,  0,  255,  255,  255,  0,  0,192);
     break;

    case 45:
      delayTimerSetHAANMix = 2000;
      modeSetHaanMix = MODE_HAAN_FADE;
      setHaanSettings( 0, 2,  16727431,  0, 0,  162,  255,  252,  44,  0, 0);
     break;
    //SET8

    case 46:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_RUNNING;
      setHaanSettings(101, 58,  8913151,  0,  0,  198,  128,  252,  0, 0, 0);
     break;

    case 47:
      delayTimerSetHAANMix = 2000;
      modeSetHaanMix = MODE_HAAN_FADE;
      setHaanSettings(101, 58,  8913151,  0,  0,  198,  128,  252,  0, 0, 0);
     break;

     case 48:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_SAW;
      setHaanSettings( 128, 128,  8913151,  0,  0,  198,  128,  252,  0, 0, 0);
     break;

    case 49:
      delayTimerSetHAANMix = 2000;
      modeSetHaanMix = MODE_HAAN_FADE;
      setHaanSettings(101, 58,  8913151,  0,  0,  198,  128,  252,  0, 0, 0);
     break;

    case 50:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_SAW;
      setHaanSettings( 128, 128,  8913151,  0,  0,  198,  128,  252,  0, 0, 0);
     break;
    
    //SET9

    case 51:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_SCAN;
      setHaanSettings(101,  58,  12517631,  2508799,  0,  255,  255,  252,  66,  0,0);
     break;

    case 52:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_DUAL_SCAN;
      setHaanSettings(101,  58,  12517631,  2508799,  0,  255,  255,  252,  66,  0,0);
     break;

    case 53:
      delayTimerSetHAANMix = 2000;
      modeSetHaanMix = MODE_HAAN_FADE;
      setHaanSettings(101,  58,  9175295,  2508799,  0,  255,  255,  252,  66,  0,0);
     break;

    //SET10

    case 54:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_SOLID_GLITTER;
      setHaanSettings( 0, 44,  16731549,  10162175,  0,  208,  255,  252,  0,  0, 0);
     break;

    case 55:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_SPARKLE_DARK;
      setHaanSettings(255,  58,  9175295,  0,  0,  255,  255,  252,  0,  0,0);
     break;

    case 56:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_SPARKLE_PLUS;
      setHaanSettings(255,  255,  9175295,  0,  0,  255,  255,  252,  0,  0,0);
     break;

     case 57:
      delayTimerSetHAANMix = 2000;
      modeSetHaanMix = MODE_HAAN_FADE;
      setHaanSettings(101,  58,  9175295,  0,  0,  255,  255,  252,  0,  0,0);
     break;
    //SET11

    case 58:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_SPARKLE_DARK;
      setHaanSettings(255,  255,  9175295,  0,  0,  255,  255,  252,  0,  0,0);
     break;

    case 59:
      delayTimerSetHAANMix = 2000;
      modeSetHaanMix = MODE_HAAN_FADE;
      setHaanSettings(101,  58,  9175295,  0,  0,  255,  255,  252,  0,  0,0);
     break;

    case 60:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_SOLID_GLITTER;
      setHaanSettings( 0, 44,  16731549,  10162175,  0,  208,  255,  252,  0,  0, 0);
     break;

    
    //SET12

    case 61:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_TWO_AREAS;
      setHaanSettings(  101,  255,  8913151,  0,  0,  208,  255,  252,  0,  0, 0);
     break;

    case 62:
      delayTimerSetHAANMix = SEGMENT.BASE_TIME;
      modeSetHaanMix = MODE_TWO_DOTS;
      setHaanSettings(  101,  255,  8913151,  0,  0,  208,  255,  252,  0,  0, 0);
     break;

    case 63:
      delayTimerSetHAANMix = 2000;
      modeSetHaanMix = MODE_HAAN_FADE;
      setHaanSettings(101,  58,  8913151,  0,  0,  255,  255,  252,  66,  0,0);
     break;
    default:
      break;
    }
    setNSettings(haanSpeed, haanIntensity, haanSEG0, haanSEG1, haanSEG2, haanFFTlow, haanFFTHigh, haanFFTCustom, haanPallet, haanBlandingPallet);
    Serial.print("SetHaanMix Mode: ");Serial.print(modeSetHaanMix); Serial.print(", Timer Mode:");
    Serial.println(timersModeSetHAANMix);
    pixTimerSetHaanMix.setPeriod(delayTimerSetHAANMix);
    timersModeSetHAANMix++;
    timersModeSetHAANMix%=TIMER_MODES_SETHAANMix;
  }

  switch (modeSetHaanMix)
  {
  case FFT_WALL:
    return mode_fft_wall();
  case MODE_HAAN_FADE:
    return mode_haan_fade();
  case FFT_SWIPE:
    return mode_fft_swiping();
  case RANDOM_FFT:
    return mode_random_fft();
  case FFT_BLINK:
    return fft_blink();
  case FFT_FIB:
    return mode_fft_fib();
  case MODE_2DSQUAREDSWIRL:
    return mode_2Dsquaredswirl();
  case MODE_2DDNA:
    return mode_2Ddna();
  case MODE_2DPLASMA:
    return mode_2Dplasma();
  case MODE_JUGGLE:
    return mode_juggles();
  case MODE_PIXELS:
    return mode_pixels();
  case MODE_PLASMOID:
    return mode_plasmoid();
  case MODE_FREQPIXEL:
    return mode_freqpixel();
  case MODE_PUDDLEPEAK:
    return mode_puddlepeak();
  case MODE_2DMATRIX:
    return mode_2Dmatrix();
  case MODE_TWO_AREAS:
    return mode_two_areas();
  case MODE_TWO_DOTS:
    return mode_two_dots();
  case MODE_SPARKLE_DARK:
    return mode_flash_sparkle();
  case MODE_SOLID_GLITTER:
    return mode_solid_glitter();
  case MODE_SPARKLE_PLUS:
    return mode_hyper_sparkle();  
  case MODE_SCAN:
    return mode_scan();
  case MODE_DUAL_SCAN:
    return mode_dual_scan();
  case MODE_RUNNING:
    return mode_running_lights();
  case MODE_SAW:
    return mode_saw();
  case MODE_PLASMA:
    return mode_plasma();
  case MODE_PHASED:
    return mode_phased();
  case MODE_PHASED_NOISE:
    return mode_phased_noise();
  case MODE_CHASE:
    return mode_chase_color();
  case MODE_COLORWAVES:
    return mode_colorwaves();
  case MODE_COLORWINKLES:
    return mode_colortwinkle();
  case MODE_FIREWORKS:
    return mode_fireworks();
  case MODE_FLOW:
    return mode_flow();
  case MODE_DRIP:
    return mode_drip();
  case MODE_PACIFICA:
    return mode_pacifica();

    
  default:
    break;
  }
}*/


///////////////////////////////////////////////////////////////////////////////////













uint16_t WS2812FX::mode_haan_mix() {
  
  EVERY_N_MILLISECONDS_I(pixTimer, delayTimer) { 
      
      switch (timersMode)
      {
        
        case 0:
          delayTimer = SEGMENT.BASE_TIME;
          mode = FFT_WALL;
          //setHaanSettings(0,128,16752640, 16752640, 16752640,0,0,0,0,0);
          wallFFTDisabled = false;
          setHaanSettings(0,288,0, 0, 0,0,0,228,0,0,0);
          //RED- need to change color
          //Speed: 44, Intensity: 228, COLOR0: 16752640, COLOR1: 0, COLOR2: 0, FFT_LOW: 255, FFT_HIGH: 255, FFT_CUSTOM: 218, PALLET: 0, PALLET_BLENDING: 0
          //Speed: 94, Intensity: 169, COLOR0: 16752640, COLOR1: 0, COLOR2: 0, FFT_LOW: 255, FFT_HIGH: 255, FFT_CUSTOM: 142, PALLET: 0, PALLET_BLENDING: 0
          //YELLOW- need to change color
          //speed: 0, Intensity: 128, COLOR0: 16712589, COLOR1: 0, COLOR2: 0, FFT_LOW: 255, FFT_HIGH: 255, FFT_CUSTOM: 228, PALLET: 0, PALLET_BLENDING: 0

          //Perpule - need to change color
          //speed: 0, Intensity: 128, COLOR0: 16712589, COLOR1: 0, COLOR2: 0, FFT_LOW: 255, FFT_HIGH: 255, FFT_CUSTOM: 228, PALLET: 0, PALLET_BLENDING: 0
          break;
        case 1:
          delayTimer = SEGMENT.BASE_TIME;
          wallFFTDisabled = true;
          setHaanSettings(0,288,16752640, 0, 0,0,0,228,0,0,192);
          break;
        case 2:
          delayTimer = SEGMENT.BASE_TIME;
          mode = FFT_WALL;
          wallFFTDisabled = false;
          setHaanSettings(0,288,192, 0, 0,0,0,228,0,0,0);
          break;
        case 3:
          delayTimer = SEGMENT.BASE_TIME;
          mode = FFT_WALL;
          wallFFTDisabled = true;
          setHaanSettings(0,288,16252040, 0, 0,0,0,228,0,0,0);
          break;
        case 4:
          delayTimer = SEGMENT.BASE_TIME;
          mode = FFT_WALL;
          wallFFTDisabled = false;
          setHaanSettings(0,288,16252040, 0, 0,0,0,228,0,0,0);
          break;
        case 5:
          delayTimer = SEGMENT.FADE_TIME;
          mode = MODE_HAAN_FADE;
          break;
        case 6:
        
          setHaanSettings(0,288,16752640, 0, 0,0,0,228,0,0,192);  //To remove - just for init testing
          delayTimer = SEGMENT.BASE_TIME;
          mode = FFT_SWIPE;
          break;
        case 7:
          delayTimer = SEGMENT.BASE_TIME;
          mode = RANDOM_FFT;
          break;
        case 8:
          delayTimer = SEGMENT.FADE_TIME;
          mode = MODE_HAAN_FADE;
          break;
        case 9:
          delayTimer = SEGMENT.BASE_TIME;
          mode = FFT_BLINK;
          break;
        
        
        case 10:
          delayTimer = SEGMENT.BASE_TIME;
          mode = FFT_FIB;
          break;
        
        
        
        case 11:
          delayTimer = SEGMENT.BASE_TIME;
          mode = MODE_2DDNA;
          break;
        case 12:
          delayTimer = SEGMENT.BASE_TIME;
          mode = MODE_2DSQUAREDSWIRL;
          break;
        case 13:
          delayTimer = SEGMENT.FADE_TIME;
          mode = MODE_HAAN_FADE;
          break;

        


       //SET3


       case 14:
          delayTimer = SEGMENT.BASE_TIME*7;
          mode = SET_2;
          break; 
        // case 14:
        //   delayTimer = SEGMENT.BASE_TIME;
        //   mode = MODE_2DPLASMA;
        //   break;

        case 15:
          delayTimer = SEGMENT.BASE_TIME;
          mode = MODE_JUGGLE;
          break; 
        
        case 16:
          delayTimer = SEGMENT.BASE_TIME;
          mode = MODE_PIXELS;
          break; 
        
        case 17:
          delayTimer = SEGMENT.BASE_TIME;
          mode = MODE_PLASMOID;
          break; 

        case 18:
          delayTimer = SEGMENT.BASE_TIME;
          mode = MODE_FREQPIXEL;
          break;
        
        case 19:
          delayTimer = SEGMENT.BASE_TIME;
          mode = MODE_PUDDLEPEAK;
          break;

        case 20:
          delayTimer = SEGMENT.FADE_TIME;
          mode = MODE_HAAN_FADE;
          break;

        
        
        
        case 21:
          delayTimer = SEGMENT.BASE_TIME;
          mode = MODE_2DMATRIX;
          break;  
        
        case 22:
          delayTimer = SEGMENT.FADE_TIME;
          mode = MODE_HAAN_FADE;
          break;




        case 23:
          delayTimer = SEGMENT.BASE_TIME;
          mode = MODE_CHASE;
          break;

        case 24:
          delayTimer = SEGMENT.BASE_TIME;
          mode = MODE_COLORWAVES;
          break;

        case 25:
          delayTimer = SEGMENT.BASE_TIME;
          mode = MODE_FIREWORKS;
          break;
        
        case 26:
          delayTimer = SEGMENT.FADE_TIME;
          mode = MODE_HAAN_FADE;
          break;

        case 27:
          delayTimer = SEGMENT.BASE_TIME;
          mode = MODE_DRIP;
          break;

        case 28:
          delayTimer = SEGMENT.FADE_TIME;
          mode = MODE_HAAN_FADE;
          break;

        case 29:
          delayTimer = SEGMENT.BASE_TIME;
          mode = MODE_FLOW;
          break;

        case 30:
          delayTimer = SEGMENT.BASE_TIME;
          mode = MODE_SOLID_GLITTER;
          break;
      
        case 31:
          delayTimer = SEGMENT.FADE_TIME;
          mode = MODE_HAAN_FADE;
          break;

        case 32:
          delayTimer = SEGMENT.BASE_TIME;
          mode = MODE_PACIFICA;
          break;

        case 33:
          delayTimer = SEGMENT.FADE_TIME;
          mode = MODE_HAAN_FADE;
          break;





        case 34:
          delayTimer = SEGMENT.BASE_TIME;
          mode = MODE_PHASED;
          break;

        case 35:
          delayTimer = SEGMENT.BASE_TIME;
          mode = MODE_PHASED_NOISE;
          break;

        case 36:
          delayTimer = SEGMENT.FADE_TIME;
          mode = MODE_HAAN_FADE;
          break;



        case 37:
          delayTimer = SEGMENT.BASE_TIME;
          mode = MODE_PLASMA;
          break;

        case 38:
          delayTimer = SEGMENT.FADE_TIME;
          mode = MODE_HAAN_FADE;
          break;




        case 39:
          delayTimer = SEGMENT.BASE_TIME;
          mode = MODE_RUNNING;
          break;

        case 40:
          delayTimer = SEGMENT.BASE_TIME;
          mode = MODE_SAW;
          break;




        case 41:
          delayTimer = SEGMENT.BASE_TIME;
          mode = MODE_SCAN;
          break;

        case 42:
          delayTimer = SEGMENT.BASE_TIME;
          mode = MODE_DUAL_SCAN;
          break;

        case 43:
          delayTimer = SEGMENT.FADE_TIME;
          mode = MODE_HAAN_FADE;
          break;



        case 44:
          delayTimer = SEGMENT.BASE_TIME;
          mode = MODE_SOLID_GLITTER;
          break;

        case 45:
          delayTimer = SEGMENT.BASE_TIME;
          mode = MODE_SPARKLE_DARK;
          break;

        case 46:
          delayTimer = SEGMENT.BASE_TIME;
          mode = MODE_SPARKLE_PLUS;
          break;

        case 47:
          delayTimer = SEGMENT.FADE_TIME;
          mode = MODE_HAAN_FADE;
          break;

        
        
        case 48:
          delayTimer = SEGMENT.BASE_TIME;
          mode = MODE_SPARKLE;
          break;

        case 49:
          delayTimer = 1000;
          mode = MODE_HAAN_FADE;
          break;

        case 50:
          delayTimer = SEGMENT.BASE_TIME;
          mode = MODE_SOLID_GLITTER;
          break;


        case 51:
          delayTimer = SEGMENT.BASE_TIME;
          mode = MODE_TWO_AREAS;
          break;


        case 52:
          delayTimer = SEGMENT.BASE_TIME;
          mode = MODE_TWO_DOTS;
          break;

        case 53:
          delayTimer = SEGMENT.BASE_TIME;
          mode = MODE_TWO_AREAS;
          break;

        case 54:
          delayTimer = SEGMENT.BASE_TIME;
          mode = MODE_TWO_DOTS;
          break;

        case 55:
          delayTimer = 1000;
          mode = MODE_HAAN_FADE;
          break;

        // case 1:
        //   delayTimer = 1000;
        //   mode = MODE_HAAN_FADE;
        //   break;
        // case 2:
        //   delayTimer = 3000;
        //   mode = FFT_SWIPE;
        //   //Blue to purpul to pink
        //   //speed: 0, Intensity: 103, COLOR0: 331263, COLOR1: 0, COLOR2: 0, FFT_LOW: 255, FFT_HIGH: 255, FFT_CUSTOM: 136, PALLET: 0, PALLET_BLENDING: 0
        //   //Speed: 0, Intensity: 103, COLOR0: 10617087, COLOR1: 0, COLOR2: 0, FFT_LOW: 255, FFT_HIGH: 255, FFT_CUSTOM: 213, PALLET: 0, PALLET_BLENDING: 0
        //   //Speed: 0, Intensity: 103, COLOR0: 14483711, COLOR1: 0, COLOR2: 0, FFT_LOW: 255, FFT_HIGH: 255, FFT_CUSTOM: 213, PALLET: 0, PALLET_BLENDING: 0
        //   //Speed: 0, Intensity: 103, COLOR0: 10879231, COLOR1: 0, COLOR2: 0, FFT_LOW: 255, FFT_HIGH: 255, FFT_CUSTOM: 213, PALLET: 0, PALLET_BLENDING: 0

        //   //Red to yellow
        //   //Speed: 0, Intensity: 103, COLOR0: 16737792, COLOR1: 0, COLOR2: 0, FFT_LOW: 255, FFT_HIGH: 255, FFT_CUSTOM: 213, PALLET: 0, PALLET_BLENDING: 0
        //   //Speed: 0, Intensity: 103, COLOR0: 16769280, COLOR1: 0, COLOR2: 0, FFT_LOW: 255, FFT_HIGH: 255, FFT_CUSTOM: 213, PALLET: 0, PALLET_BLENDING: 0
        //   //Speed: 0, Intensity: 103, COLOR0: 16773148, COLOR1: 0, COLOR2: 0, FFT_LOW: 255, FFT_HIGH: 255, FFT_CUSTOM: 213, PALLET: 0, PALLET_BLENDING: 0


        //   break;
        // case 3:
        //   delayTimer = 1000;
        //   mode = MODE_HAAN_FADE;
        //   break;
        // case 4:
        //   delayTimer = 1000;
        //   mode = RANDOM_FFT;
        //   //Blue to purpul to pink
        //   //speed: 0, Intensity: 103, COLOR0: 331263, COLOR1: 0, COLOR2: 0, FFT_LOW: 255, FFT_HIGH: 255, FFT_CUSTOM: 136, PALLET: 0, PALLET_BLENDING: 0
        //   //Speed: 0, Intensity: 103, COLOR0: 10617087, COLOR1: 0, COLOR2: 0, FFT_LOW: 255, FFT_HIGH: 255, FFT_CUSTOM: 213, PALLET: 0, PALLET_BLENDING: 0
        //   //Speed: 0, Intensity: 103, COLOR0: 14483711, COLOR1: 0, COLOR2: 0, FFT_LOW: 255, FFT_HIGH: 255, FFT_CUSTOM: 213, PALLET: 0, PALLET_BLENDING: 0
        //   //Speed: 0, Intensity: 103, COLOR0: 10879231, COLOR1: 0, COLOR2: 0, FFT_LOW: 255, FFT_HIGH: 255, FFT_CUSTOM: 213, PALLET: 0, PALLET_BLENDING: 0

        //   //Red to yellow
        //   //Speed: 0, Intensity: 103, COLOR0: 16737792, COLOR1: 0, COLOR2: 0, FFT_LOW: 255, FFT_HIGH: 255, FFT_CUSTOM: 213, PALLET: 0, PALLET_BLENDING: 0
        //   //Speed: 0, Intensity: 103, COLOR0: 16769280, COLOR1: 0, COLOR2: 0, FFT_LOW: 255, FFT_HIGH: 255, FFT_CUSTOM: 213, PALLET: 0, PALLET_BLENDING: 0
        //   //Speed: 0, Intensity: 103, COLOR0: 16773148, COLOR1: 0, COLOR2: 0, FFT_LOW: 255, FFT_HIGH: 255, FFT_CUSTOM: 213, PALLET: 0, PALLET_BLENDING: 0

        //   //pallets
        //   break;
        // case 5:
        //   delayTimer = 1000;
        //   mode = MODE_HAAN_FADE;
        //   break;
        // case 6:
        //   delayTimer = 3000;
        //   mode = FFT_FIB;
        //   break;
        // case 7:
        //   delayTimer = 1000;
        //   mode = MODE_HAAN_FADE;
        //   break;
        // case 8:
        //   delayTimer = 1000;
        //   mode = FFT_BLINK;
        //   break;
        // case 9:
        //   delayTimer = 1000;
        //   mode = MODE_HAAN_FADE;
        //   break;
        // case 10:
        //   delayTimer = 3000;
        //   mode = DANCING_BAR;
        //   break;
        // default:
        //   break;
         default:
           break;
      }

      mode = SET_12;

      Serial.print("Mode: ");Serial.print(mode); Serial.print(", Timer Mode:");
      Serial.println(timersMode);
      pixTimer.setPeriod(delayTimer);
      timersMode++;
      timersMode%=TIMER_MODES;
      
  }

  setNSettings(haanSpeed,haanIntensity,haanSEG0, haanSEG1, haanSEG2,haanFFTlow,haanFFTHigh,haanFFTCustom,haanPallet,haanBlandingPallet);
  switch (mode)
  {
  case 0:
    //Serial.println("fft wall");
    return mode_fft_wall();
  case 1:
    //Serial.println("fft swiping");
    return mode_fft_swiping();
  case 2:
    //Serial.println("fft random");
    return mode_random_fft();
  case 3:
    //Serial.println("fft fib");
    return mode_fft_fib();
  case 4:
    //Serial.println("fft blink");
    return fft_blink();
  case 5: 
    //Serial.println("fft dancingbar");
    return dancingBar2();
  case 6:
    //Serial.println("mode_juggle");
    return mode_juggles();
  case 7:
    //Serial.println("mode_midnoise");
    return mode_midnoise();
  case 8:
    //Serial.println("mode_pixels");
    return mode_pixels();
  case 9:
    //Serial.println("mode_plasmoid");
    return mode_plasmoid();
  case 10:
    //Serial.println("mode_puddlepeak");
    return mode_puddlepeak();
  case 11:
    //Serial.println("mode_ripplepeak");
    return mode_ripplepeak();
  case 12:
    //Serial.println("mode_freqpixel");
    return mode_freqpixel();
  case 13:
    //Serial.println("mode_noisepeak");
    return mode_noisepeak();
  case 14:
    //Serial.println("mode_2Ddna");
    //Speed: 255, Intensity: 95, COLOR0: 16763663, COLOR1: 0, COLOR2: 0, FFT_LOW: 57, FFT_HIGH: 66, FFT_CUSTOM: 255, PALLET: 0, PALLET_BLENDING: 0
    //Speed: 255, Intensity: 95, COLOR0: 12517631, COLOR1: 0, COLOR2: 0, FFT_LOW: 57, FFT_HIGH: 66, FFT_CUSTOM: 255, PALLET: 0, PALLET_BLENDING: 0
    
    //Pallets blue white
    //Speed: 255, Intensity: 95, COLOR0: 12517631, COLOR1: 0, COLOR2: 0, FFT_LOW: 57, FFT_HIGH: 66, FFT_CUSTOM: 255, PALLET: 0, PALLET_BLENDING: 0
    //Speed: 255, Intensity: 95, COLOR0: 8913151, COLOR1: 3868159, COLOR2: 16715587, FFT_LOW: 57, FFT_HIGH: 66, FFT_CUSTOM: 255, PALLET: 35, PALLET_BLENDING: 

    //pallets red yelo
    //Speed: 255, Intensity: 95, COLOR0: 8913151, COLOR1: 3868159, COLOR2: 16715587, FFT_LOW: 57, FFT_HIGH: 66, FFT_CUSTOM: 255, PALLET: 8, PALLET_BLENDING: 0
    
    //pallets green white
    //Speed: 255, Intensity: 95, COLOR0: 8913151, COLOR1: 3868159, COLOR2: 16715587, FFT_LOW: 57, FFT_HIGH: 66, FFT_CUSTOM: 255, PALLET: 14, PALLET_BLENDING: 0

    //pallet vintage  semi red white yello black
    //Speed: 255, Intensity: 95, COLOR0: 8913151, COLOR1: 3868159, COLOR2: 16715587, FFT_LOW: 57, FFT_HIGH: 66, FFT_CUSTOM: 255, PALLET: 23, PALLET_BLENDING: 0
    
    //pallet yellow to red
    //Speed: 255, Intensity: 95, COLOR0: 8913151, COLOR1: 3868159, COLOR2: 16715587, FFT_LOW: 57, FFT_HIGH: 66, FFT_CUSTOM: 255, PALLET: 65, PALLET_BLENDING: 0

    return mode_2Ddna();
  case 15:
   //Serial.println("mode_2Dmatrix");
   //Speed: 0, Intensity: 255, COLOR0: 8913151, COLOR1: 3868159, COLOR2: 16715587, FFT_LOW: 255, FFT_HIGH: 255, FFT_CUSTOM: 143, PALLET: 42, PALLET_BLENDING: 0

    return mode_2Dmatrix();
  case 16:
    //Serial.println("mode_2Dplasma");
    //blue purpul
    //Speed: 18, Intensity: 255, COLOR0: 13107455, COLOR1: 7536895, COLOR2: 0, FFT_LOW: 255, FFT_HIGH: 255, FFT_CUSTOM: 42, PALLET: 0, PALLET_BLENDING: 0
    //orange
    //Speed: 18, Intensity: 255, COLOR0: 16751872, COLOR1: 0, COLOR2: 0, FFT_LOW: 255, FFT_HIGH: 255, FFT_CUSTOM: 42, PALLET: 0, PALLET_BLENDING: 0
    //organ red white
    //Speed: 18, Intensity: 255, COLOR0: 16763788, COLOR1: 0, COLOR2: 0, FFT_LOW: 255, FFT_HIGH: 255, FFT_CUSTOM: 42, PALLET: 0, PALLET_BLENDING: 0
    //purpule blue
    //Speed: 18, Intensity: 255, COLOR0: 9765119, COLOR1: 0, COLOR2: 0, FFT_LOW: 255, FFT_HIGH: 255, FFT_CUSTOM: 42, PALLET: 0, PALLET_BLENDING: 0
    //blue very slow speed nice!!
    //Speed: 0, Intensity: 255, COLOR0: 9765119, COLOR1: 0, COLOR2: 0, FFT_LOW: 255, FFT_HIGH: 255, FFT_CUSTOM: 255, PALLET: 0, PALLET_BLENDING: 0
    //should check pallets
    return mode_2Dplasma();
  case 17:
  //Serial.println("mode_2Dmeatballs");

  //Too fast, need to slow down
    return mode_2Dmeatballs();
  case 18:
  //Serial.println("mode_2Dsquaredswirl");
  //Red yello, need to slow down the effect!
  //Speed: 255, Intensity: 255, COLOR0: 16711705, COLOR1: 16759552, COLOR2: 0, FFT_LOW: 255, FFT_HIGH: 255, FFT_CUSTOM: 255, PALLET: 0, PALLET_BLENDING: 0
    return mode_2Dsquaredswirl();
  case 19:
  //Serial.println("mode_flow");

  //Use only with pallets!! dont take the following pallets, just the ones above
  //Speed: 27, Intensity: 255, COLOR0: 7618047, COLOR1: 0, COLOR2: 0, FFT_LOW: 43, FFT_HIGH: 64, FFT_CUSTOM: 81, PALLET: 0, PALLET_BLENDING: 0
    return mode_flow();
  case 20:
    //return chase(); //nice to have?
    return 40;
  case 21:
  //not that nice
    return mode_candle_multi();
  case 22:
  //not that nice
    return mode_blink_rainbow();
  case 23:
  //pallets
  //Speed: 239, Intensity: 223, COLOR0: 6038015, COLOR1: 0, COLOR2: 0, FFT_LOW: 255, FFT_HIGH: 83, FFT_CUSTOM: 62, PALLET: 65, PALLET_BLENDING: 0
    return mode_circus_combustus();
  case 24:
  //Very nice!! 
  //Speed: 18, Intensity: 3, COLOR0: 16752640, COLOR1: 0, COLOR2: 0, FFT_LOW: 83, FFT_HIGH: 255, FFT_CUSTOM: 238, PALLET: 46, PALLET_BLENDING: 0
  //Intensity: 248 can scale up to  - change the way it moves on the bord- super nice!!
  //Speed can scale from 5-18
  //Should use pallets, maybe balnd as well, can be long run
    return mode_colorwaves();
  case 25:
    //not working with pallets, only color all works should slow down the speed via code
    //Speed: 0, Intensity: 211, COLOR0: 6038015, COLOR1: 0, COLOR2: 0, FFT_LOW: 83, FFT_HIGH: 255, FFT_CUSTOM: 255, PALLET: 28, PALLET_BLENDING: 0
    return mode_drip();
  case 26:
  //not that nice
    return mode_dynamic();
  case 27:
  //Speed: 0, Intensity: 0, COLOR0: 6038015, COLOR1: 0, COLOR2: 0, FFT_LOW: 255, FFT_HIGH: 255, FFT_CUSTOM: 255, PALLET: 10, PALLET_BLENDING: 0
  //nice with pallets, speed should scale from 0-256 very nice!
    return mode_fireworks();
  case 28:
    //nice with pallets, should work with fib?
    //Speed: 116, Intensity: 255, COLOR0: 6038015, COLOR1: 0, COLOR2: 0, FFT_LOW: 255, FFT_HIGH: 255, FFT_CUSTOM: 255, PALLET: 10, PALLET_BLENDING: 0
    return mode_gradient();
  case 29:
    //not that nice
    return mode_heartbeat();
  case 30:
  //not that nice
    return mode_comet();
  case 31:
    //not that nice
    //speed: 0, Intensity: 2, COLOR0: 16727431, COLOR1: 0, COLOR2: 0, FFT_LOW: 162, FFT_HIGH: 255, FFT_CUSTOM: 252, PALLET: 44, PALLET_BLENDING: 0
    //intencity can scale 0-250 
    // good with pallets, maybe comined with fib

    return mode_pacifica();
  case 32:
  //nice 
  //blue pink
  //Speed: 96, Intensity: 255, COLOR0: 13277695, COLOR1: 0, COLOR2: 0, FFT_LOW: 255, FFT_HIGH: 255, FFT_CUSTOM: 255, PALLET: 0, PALLET_BLENDING: 0
  //blue purple
  //Speed: 96, Intensity: 255, COLOR0: 9175295, COLOR1: 0, COLOR2: 0, FFT_LOW: 255, FFT_HIGH: 255, FFT_CUSTOM: 255, PALLET: 0, PALLET_BLENDING: 0
  //speed can scale - Speed: 195,

  //nice with pallets
    return mode_perlinmove();
  case 33:
  //low intensiy- high spped
  //spped 66 - 113

  //Speed: 113, Intensity: 50, COLOR0: 9175295, COLOR1: 0, COLOR2: 0, FFT_LOW: 255, FFT_HIGH: 255, FFT_CUSTOM: 255, PALLET: 39, PALLET_BLENDING: 0

  //nice with pallets
    return mode_phased();
  case 34:
  //amost the same as phased mode but nice, can be combind
    return mode_phased_noise();
  case 35:
  //spped can scale up to 255
  //intensity change 31- 255
  //Speed: 0, Intensity: 103, COLOR0: 9175295, COLOR1: 0, COLOR2: 0, FFT_LOW: 255, FFT_HIGH: 255, FFT_CUSTOM: 255, PALLET: 39, PALLET_BLENDING: 0
  // colllor + pallets
    return mode_plasma();
  case 36:
  //Speed: 31, Intensity: 54, COLOR0: 9175295, COLOR1: 0, COLOR2: 0, FFT_LOW: 255, FFT_HIGH: 255, FFT_CUSTOM: 252, PALLET: 0, PALLET_BLENDING: 0
  //intesity 54- 255
  //color + pallets
    return mode_ripple();
  case 37:
  //same as above, can be cominded
    return mode_ripple_rainbow();
  case 38:
    //return running_base();
    return 10;
  case 39:
    //return running();
    return 10;
  case 40:
  //speed: 101, Intensity: 58, COLOR0: 9175295, COLOR1: 0, COLOR2: 0, FFT_LOW: 255, FFT_HIGH: 255, FFT_CUSTOM: 252, PALLET: 0, PALLET_BLENDING: 0
  //spped 20-101
  //intencity - 255
    return mode_saw();
  case 41:
  //nice!! need to check collros
  //speed: 101, Intensity: 58, COLOR0: 9175295, COLOR1: 0, COLOR2: 0, FFT_LOW: 255, FFT_HIGH: 255, FFT_CUSTOM: 252, PALLET: 66, PALLET_BLENDING: 0

    return mode_scan();
  case 42:
  //nice!! need to check collros
  //speed: 101, Intensity: 58, COLOR0: 9175295, COLOR1: 0, COLOR2: 0, FFT_LOW: 255, FFT_HIGH: 255, FFT_CUSTOM: 252, PALLET: 66, PALLET_BLENDING: 0
    return mode_dual_scan();
  case 43:
  //Speed: 99, Intensity: 51, COLOR0: 4541439, COLOR1: 10162175, COLOR2: 0, FFT_LOW: 208, FFT_HIGH: 255, FFT_CUSTOM: 252, PALLET: 66, PALLET_BLENDING: 0
  //speed 68-99
    return mode_sinewave();
  case 44:
    //nice only colors
   //Speed: 0, Intensity: 44, COLOR0: 16731549, COLOR1: 10162175, COLOR2: 0, FFT_LOW: 208, FFT_HIGH: 255, FFT_CUSTOM: 252, PALLET: 66, PALLET_BLENDING: 0

    return mode_solid_glitter();
  case 45:
  //flicker

  //only single color
  //Speed: 255, Intensity: 44, COLOR0: 16731549, COLOR1: 0, COLOR2: 0, FFT_LOW: 208, FFT_HIGH: 255, FFT_CUSTOM: 252, PALLET: 0, PALLET_BLENDING: 0

    return mode_sparkle();
  case 46:
  //Speed: 101, Intensity: 255, COLOR0: 16731549, COLOR1: 0, COLOR2: 0, FFT_LOW: 208, FFT_HIGH: 255, FFT_CUSTOM: 252, PALLET: 0, PALLET_BLENDING: 0

    return mode_two_dots();
  case 47:
  //Speed: 101, Intensity: 255, COLOR0: 16731549, COLOR1: 0, COLOR2: 0, FFT_LOW: 208, FFT_HIGH: 255, FFT_CUSTOM: 252, PALLET: 0, PALLET_BLENDING: 0

    return mode_two_areas(); //should be combined with two dots
  case 48:
    return mode_haan_fade();

  case 49:
    return mode_flash_sparkle();

  case 50:
    return mode_hyper_sparkle();

  case 51:
    return set1();
  case 52:
    return set2();
  case 53: 
    return set3();  
  case 54:
    return set4();
  case 55:
    return set5();
  case 56:
    return set6();
  case 57:
    return set7();
  case 58:
    return set8();
  case 59:
    return set9();
  case 60:
    return set10();
  case 61:
    return set11();
  case 62:
    return set12();
  default:
    break;
  }

} //haan mix

//List of non sound effects which should be used.

/*
 
Juggels -  Max Brithness, spped 60%

midnose ?
pixles - choose pallets  60% spped
plasmoid -
puddlepeak 30% spped + choose pallets
ripplepeak - shold tweek sepped - not working correctlty
freqpixel - 40% speed + pallets 
noisepeek- should be short + pallets  80% speed 

 Solid  - ? might be used with different color

 2D DNA - static color are nice!, pallets - need to choose 
 2D Matrix - nice, need to tweek the speed
 2D Palasma - static color are nice! , pallets - need to choose, speed needs to tweak, should be low
 2D Meta ball - should be short
 2D Squared Swirl -  nice, 
 
 
 Flow -  should used combined with low intencity then move to high intencity - spped from 20-60% colors pallets need to choose
 Chase - nice to have, pallets + should tweek colors + spped+ intencicy 
 Candle - multi - nice with static colors + pallets - need to choose. intencity - max, speed should tweek
 Blink rainbows - same as blink
 Blink - not a must - could be nice if music will be tweekes into the pattern
 Android - not a must !nice to have, should tweek to support audio as speed, + colros - n
 Circus - nice, almost max speed + max intencity + SEGCOLOR(2)
 ColorTwinkles - NTH - 40% speed
 ColorWave - 10% speed + pallets need to choose  
 Drip - SEG(0) - should be black, seg(2) - should be colored -
 Dynamic -  max speed, intencty - 10% , pallets only
 Fireworks - max speed, max intencity - single color, + pallets
 Gradient 
 Heartbeat -  nice with pallets
 Light house (mode_comet)-  nice, seg(2) need to have color + palets
 Pacifica - 50% speed 
 Perlin Move -  should choose pallets
 Phased -  30% speed - should choose colors + pallets
 Pahsed Noice -  Seg(2) black, choose pallets 30% speed
 Plasma -  60% speed, + choose pallets
 Ripple - 10% speed +  choose pallets
 Rainbow Ripple - 10% speed +  choose pallets
 Running - 60% speed, max intencity + choose pallets
 Running 2 - 60% speed, max intencity + choose pallets
 SAW - 60% speed, max intencity + choose pallets
 SCAN - 80% speed, max intencity + choose pallets
 SCANNER DUal -  0 spped max intencity 
 SINe (mode_sinewave) - 50% speed - max intencity - choose pallets 
 Solid Glitter -  should be short + sen(0) color or none.
 Spark - SEG(0) balck SEG(2) color max all

 Two Areas + Two Dots - should fix color

 Glitter ? 

 bouncing balls - to havvy
 bpm? 
 breath - not working
 candle - not working
fire2012?










Seens 
------

??Noisepeak  not sure if needed

?Android
Blink - should mixed with sound
?Blink Rainbow

------------------------------------
//SET 1
FFT FIB
FFT Blink
FFT RANDOM
FFT WALL
FFT SWIPE

//SET 2
2D DNA <-> 2D Squard Swirl -> Fade out

//SET 3
2D Plasma <-> Juggles
Juggles <-> Pixles
Pixles <-> Plasmoid
Plasmoid <-> FreqPixel
FreqPixel <-> Puddlepeak

//SET 4
2D Matrix -> Fadeout

//SET 5
Chase  -> ColorWaves -> Colorwinkles -> fireworks -> fade -> Drip -> fade -> flow -> glitter -> fade -> pacifica

//SET6
Phased -> Phased Noise -> fade
//SET7
Plasma -> fade
//SET8
Running 2 -> SAW -> fade
//SET9
Scan -> Scan Dual 
//SET10
Solid Glitter -> Sparkle Dark(FX_MODE_FLASH_SPARKLE) -> Sparkele+ (FX_MODE_HYPER_SPARKLE)(should convert to sound!!!!)
//Set11
Sparkle -> Fade -> Solid Gliter
//SET12
Two Area -> Two Dots


37 Effects 








ib_jul01_gp -  RED -> DARK GREEN -> GREEN -> LIGHT GREEN -> RED 
es_vintage_57_gp ->




*/