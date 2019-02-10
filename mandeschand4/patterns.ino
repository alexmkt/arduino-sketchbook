// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<leds1.numPixels(); i++) {
      leds1.setPixelColor(i, c);
      leds1.show();
      delay(wait);
  }
}

void colorAll(uint8_t _length, float r, float g, float b, float w) {
  for (uint16_t i=0; i< leds1.numPixels(); i+=1) {
    leds1.setPixelColor(i,leds1.Color((int) r*255,  g*255, b*255, w*255));
  }
  leds1.show();
}

void redPulse(float t) {
  float f;
  f = 255.0*(sin(t));
  for (uint16_t i=0; i< leds1.numPixels(); i+=1) {
    if (f < 0) f = 0;
    leds1.setPixelColor(i,leds1.Color((int) f,   0, 0));
  }
  leds1.show();
}

void redPulseAbs(float t) {
  float f;
  f = 255.0*(sin(t));
  for (uint16_t i=0; i< leds1.numPixels(); i+=1) {
    leds1.setPixelColor(i,leds1.Color((int) abs(f),   0, 0));
  }
  leds1.show();
}

void quads(float t, int quadsize) {
  float f;
  float piquad = TWO_PI/4.0;
  for (uint16_t j=0; j <4; j++) {
    f = 255.0*(sin(t+piquad*j));
    for (uint16_t i=quadsize*j; i< quadsize*j+quadsize; i+=1) {
      if (f < 0) f = 0;
      leds1.setPixelColor(i,leds1.Color((int) f,   0, 0));
    }
  }
  leds1.show();
}

void quads2(float t, int quadsize) {
  float f;
  float piquad = TWO_PI/4.0;
  for (uint16_t j=0; j <4; j++) {
    f = 255.0*(sin(t+piquad*j));
    for (uint16_t i=quadsize*j; i< quadsize*j+quadsize; i+=1) {
      if (f < 0) f = 0;
      leds1.setPixelColor(i,leds1.Color((int) f, 0, j%2*f));
    }
  }
  leds1.show();
}

void quads3(float t, int quadsize) {
  float f;
  float g;
  float piquad = TWO_PI/4.0;
  for (uint16_t j=0; j <4; j++) {
    f = 255.0*(sin(t+piquad*j));
    g = 255.0 *(sin(t+piquad*(j+1)));
    for (uint16_t i=quadsize*j; i< quadsize*j+quadsize; i+=1) {
      if (f < 0) f = 0;
      if (g < 0) g = 0;
      leds1.setPixelColor(i,leds1.Color((int) f, 0, g));
    }
  }
  leds1.show();
}

void quads4(float t, int quadsize) {
  float f;
  float g;
  float piquad = TWO_PI/4.0;
  for (uint16_t j=0; j <4; j++) {
    f = 255.0*(sin(t+piquad*j));
    g = 255.0 *(sin(-t+piquad*(j+1)));
    for (uint16_t i=quadsize*j; i< quadsize*j+quadsize; i+=1) {
      if (f < 0) f = 0;
      if (g < 0) g = 0;
      leds1.setPixelColor(i,leds1.Color((int) f, 0, g));
    }
  }
  leds1.show();
}

void fadeWave(float t, uint8_t _length, float r, float g, float b, float w) {
  float f;
  float halflength = _length/2.0;
  for (uint16_t i=0; i< _length; i+=1) {
    f = 255.0*(sin(t+((TWO_PI/halflength)*float(i))));
    if (f < 0) f = 0;
    leds1.setPixelColor(i,leds1.Color((int) f*r,   f*g,   f*b, f*w));   

  }
  leds1.show();
}

void fadeDot(float t, uint8_t _length, float r, float g, float b, float w) {
  float f;
  //float pilength = _length/2.0;
  float trate = t*1.5708;
  for (uint16_t i=0; i< _length; i+=1) {
    if (i > 6+trate || i< trate-1) continue;
    else {
      f = 255.0*(sin((float(i)/HALF_PI)-t)); //40 pixels over the space of TWO_PI
      if (f < 0) f = 0;
      leds1.setPixelColor(i,leds1.Color((int) f*r,   f*g,   f*b, f*w));
    }
  }
  leds1.show();
}

void midoutDot(float t, uint8_t _length, float r, float g, float b, float w) {
  float f;
  //float pilength = _length/2.0;
  float trate = t*1.5708;
  for (uint16_t i=0; i< _length/2; i+=1) {
    if (i > 6+trate || i< trate-1) leds1.setPixelColor(i+30,leds1.Color( 0,0,0,0));
    else {
      f = 255.0*(sin((float(i)/HALF_PI)-t)); //40 pixels over the space of TWO_PI
      if (f < 0) f = 0;
      leds1.setPixelColor(i+20,leds1.Color((int) f*r,   f*r,   f*b, f*w));
      leds1.setPixelColor(19-i,leds1.Color((int) f*r,   f*g,   f*b, f*w));   
    }
  }
  leds1.show();
}

void colorHigh(float t, uint8_t _length, float _r, float _g, float _b, float _w) {
  float f;
  uint8_t r,g,b;
  r = _r *255;
  g = _g *255;
  b = _b *255;
  //float pilength = _length/2.0;
  float trate = t*1.5708;
  for (uint16_t i=0; i< _length; i+=1) {
    if (i > 6+trate) leds1.setPixelColor(i,leds1.Color( r,g,b,0)); //leading edge color
    else if (i< trate) leds1.setPixelColor(i,leds1.Color( r,g,b,0)); //trailing edge color
    else {
      f = 255.0*(sin((float(i)/HALF_PI)-t)); //40 pixels over the space of TWO_PI
      if (f < 0) f = 0;
      leds1.setPixelColor(i,leds1.Color((int) r,   g,   b, f*_w));   
    }
  }
  leds1.show();
}

/*void color2High(float t, uint8_t _length, float _r, float _g, float _b, float _w,float _r2, float _g2, float _b2, float _w2) {
  float f;
  uint8_t r,g,b,w,r2,g2,b2,w2;
  r = _r *255;
  g = _g *255;
  b = _b *255;
  w = _w *255;
  r2 = _r2 *255;
  g2 = _g2 *255;
  b2 = _b2 *255;
  w2 = _w2 *255;

  //float pilength = _length/2.0;
  float trate = t*1.5708;
  for (uint16_t i=0; i< _length; i+=1) {
    if (i > 6+trate) leds1.setPixelColor(i,leds1.Color( r,g,b,0)); //leading edge color
    else if (i< trate) leds1.setPixelColor(i,leds1.Color( r,g,b,0)); //trailing edge color
    else {
      f = (sin((float(i)/HALF_PI)-t)); //40 pixels over the space of TWO_PI
      if (f < 0) f = 0;
      leds1.setPixelColor(i,leds1.Color((int) lerp(r,r2,f), lerp(g,g2,f), lerp(b,b2,f), lerp(w,w2,f)));   
    }
  }
  leds1.show();
}*/

void goldCycle(uint8_t wait) {
  uint16_t i,j;  
  for (j=15; j <128;j++) {
    for (i=0; i<leds1.numPixels(); i++) {
      leds1.setPixelColor(i,leds1.Color(255,   220+30*int((j/127.0)),   j*2));
    }
    leds1.show();
    delay(wait);
  }
  for (j=127; j >15;j--) {
    for (i=0; i<leds1.numPixels(); i++) {
      leds1.setPixelColor(i,leds1.Color(255,   220+30*int((j/127.0)),   j*2));
    }
    leds1.show();
    delay(wait);
  }
}

void goldChase(uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < leds1.numPixels(); i=i+3) {
        leds1.setPixelColor(i+q, leds1.Color(255, 220, 30));    //turn every third pixel on
      }
      leds1.show();
     
      delay(wait);
     
      for (int i=0; i < leds1.numPixels(); i=i+3) {
        leds1.setPixelColor(i+q, leds1.Color(255, 255, 255));        //turn every third pixel off
      }
    }
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<leds1.numPixels(); i++) {
      leds1.setPixelColor(i, Wheel((i+j) & 255));
    }
    leds1.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< leds1.numPixels(); i++) {
      leds1.setPixelColor(i, Wheel(((i * 256 / leds1.numPixels()) + j) & 255));
    }
    leds1.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasingd
    for (int q=0; q < 3; q++) {
      for (int i=0; i < leds1.numPixels(); i=i+3) {
        leds1.setPixelColor(i+q, c);    //turn every third pixel on
      }
      leds1.show();
     
      delay(wait);
     
      for (int i=0; i < leds1.numPixels(); i=i+3) {
        leds1.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
        for (int i=0; i < leds1.numPixels(); i=i+3) {
          leds1.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
        }
        leds1.show();
       
        delay(wait);
       
        for (int i=0; i < leds1.numPixels(); i=i+3) {
          leds1.setPixelColor(i+q, 0);        //turn every third pixel off
        }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return leds1.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return leds1.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return leds1.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}

