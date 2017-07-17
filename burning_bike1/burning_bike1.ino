#define ENCODER_DO_NOT_USE_INTERRUPTS
#include <Encoder.h>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define STRIP_PIN1 12
#define STRIP_PIN2 6

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(200, STRIP_PIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(100, STRIP_PIN2, NEO_GRB + NEO_KHZ800);

Encoder encoder(0, 1);

#define BLUE 0
#define GREEN 1
#define RED 2
#define RAINBOW 3

#define ENC_MAX 3
uint8_t encoderValue = 0;

void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  strip2.begin();
  strip2.show(); // Initialize all pixels to 'off'
}

void loop() {

 long encPosition = encoder.read();
 Serial.println(encPosition);
 if (encPosition > ENC_MAX) {
  encoder.write(0);
  encPosition = 0;
 }else if (encPosition < 0) {
  encPosition = ENC_MAX;
  encoder.write(ENC_MAX);
 }

 Serial.println(encPosition);
 
 switch (encPosition) {
//   case 0:
//     colorWipe(strip, strip.Color(0, 0, 255), 0); // Blue
//     colorWipe(strip2, strip2.Color(0, 0, 255), 0); // Blue
//     break;
//   case BLUE:
//     colorWipe(strip, strip.Color(0, 0, 255), 5); // Blue
//     colorWipe(strip2, strip2.Color(0, 0, 255), 5); // Blue
//     break;

   case RED:
      strip.setPixelColor(0, strip.Color(255, 0, 0));
    strip.show();
    strip2.setPixelColor(0, strip2.Color(255, 0, 0));
    strip2.show();
//     colorWipe(strip, strip.Color(255, 0, 0), 0); 
//     colorWipe(strip2, strip2.Color(255, 0, 0), 0); 
     break;

   case GREEN:
      strip.setPixelColor(0, strip.Color(0, 255, 0));
    strip.show();
    strip2.setPixelColor(0, strip2.Color(0, 255, 0));
    strip2.show();
//     colorWipe(strip, strip.Color(0, 255, 0), 0); // Green
//     colorWipe(strip2, strip2.Color(0, 255, 0), 0); // Green
     break;
//
//   case RAINBOW:
//     rainbowCycle(20);
//     break;

   default:
   strip.setPixelColor(0, strip.Color(255, 0, 0));
    strip.show();
    strip2.setPixelColor(0, strip2.Color(255, 0, 0));
    strip2.show();
//     colorWipe(strip, strip.Color(255, 0, 0), 0); 
//     colorWipe(strip2, strip2.Color(255, 0, 0), 0); 
     break;
 }
 
  // Some example procedures showing how to display to the pixels:
  //colorWipe(strip2.Color(255, 0, 0), sensorValue); // Red
//  

////colorWipe(strip.Color(0, 0, 0, 255), 50); // White RGBW
//  // Send a theater pixel chase in...
//  theaterChase(strip.Color(127, 127, 127), 50); // White
//  theaterChase(strip.Color(127, 0, 0), 50); // Red
//  theaterChase(strip.Color(0, 0, 127), 50); // Blue
//
//  rainbow(20);
//  rainbowCycle(20);
//  theaterChaseRainbow(50);
}


// Fill the dots one after the other with a color
void colorWipe(Adafruit_NeoPixel strip, uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
