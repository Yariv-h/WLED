#define FASTLED_INTERNAL //remove annoying pragma messages
#include "FX.h"
extern double fftResult[];  

uint16_t WS2812FX::mode_fft_wall() {
     
    double temp[16];
    memcpy(temp, fftResult, sizeof(fftResult[0])*16);

     for(int i=0; i<SEGLEN; i++) {
        int val =  constrain(temp[i%16], 0, 254);
        int sat = constrain(temp[i%16]+120,0,250);
        int bri = constrain(temp[i%16],0,250);
        int col = 192;
        if(pixelToSkip(i)) continue;

        CRGB newcolor = CHSV(col, sat, bri);
        setPixelColor(i, crgb_to_col(newcolor));
     }

    if(sampleAvg>150)
        addGlitterPro(sampleAvg, 192, 0, 250);

    if(sampleAvg> 70 && sampleAvg<150)
        addGlitterPro(sampleAvg, 50, 250, 250);
   
    fade_out(-30);
    return FRAMETIME + SEGMENT.speed;
 }

 void WS2812FX::addGlitterPro(fract8 chanceOfGlitter, int color, int sat, int bright) { // Let's add some glitter, thanks to Mark
  if( random8() < chanceOfGlitter) {
    CRGB newcolor = CHSV(color,sat, bright); 
    setPixelColor(random16(SEGLEN), crgb_to_col(newcolor));
  }
}

 bool WS2812FX::pixelToSkip(int pixel)  {
     if(pixel >=0 && pixel <20) return true;
     if(pixel >=110 && pixel <=148) return true;
     if(pixel >=230 && pixel <=250) return true;

     return false;
 }

