#include <Adafruit_LSM303_U.h>
#include <Adafruit_Sensor.h>

#include <LiquidCrystal.h>

#include "FastLED.h"

FASTLED_USING_NAMESPACE

// FastLED "100-lines-of-code" demo reel, showing just a few 
// of the kinds of animation patterns you can quickly and easily 
// compose using FastLED.  
//
// This example also shows one easy way to define multiple 
// animations patterns and have them automatically rotate.
//
// -Mark Kriegsman, December 2014

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    12
//#define CLK_PIN   4
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    152
CRGB leds[NUM_LEDS];

#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  120

#define POT_DATA 10


//Mag sensor stuff
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);

void setup() {
  delay(3000); // 3 second delay for recovery
  Serial.begin(115200);
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);

  mag.enableAutoRange(true);
  uint8_t count = 0;
  if(!mag.begin() && count <10)
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
    count++;
  }
}


// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm, rainbowFast, solid, gradient, sinelon, juggle, bpm };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // Hue from the mag sensor
uint8_t timeHue = 0; //Hue over time


uint16_t knob = 0;

uint16_t knobValues[] = {0, 40, 158, 280, 398, 514, 626, 746, 867, 985, 1023};


  
void loop()
{

  knob = analogRead(POT_DATA);
  
  Serial.println(knob);

  gCurrentPatternNumber = getPatternNumber(knob);
  Serial.print("Pattern: ");
  Serial.println(gCurrentPatternNumber);


 //Mag
 /* Get a new sensor event */
  sensors_event_t event;
  mag.getEvent(&event);

  uint32_t magTotal = abs(event.magnetic.x + event.magnetic.y + event.magnetic.z);
  magTotal = magTotal*4;
  gHue = magTotal;
  

  /* Display the results (magnetic vector values are in micro-Tesla (uT)) */
  Serial.print("X: "); Serial.print(event.magnetic.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.magnetic.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.magnetic.z); Serial.print("  ");Serial.println("uT");
  Serial.print("Mag Total: "); Serial.print(magTotal); Serial.print("  ");Serial.println("uT");
  
  // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();

  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { timeHue++; } // slowly cycle the "base color" through the rainbow
  //EVERY_N_SECONDS( 10 ) { nextPattern(); } // change patterns periodically
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void rainbowFast() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue+timeHue, 7);
}

void solid() 
{
  // FastLED's built-in rainbow generator
  fill_solid( leds, NUM_LEDS, CHSV(gHue,255,255));
}

void gradient() 
{
  // FastLED's built-in rainbow generator
  fill_gradient( leds, NUM_LEDS, CHSV(gHue,255,255), CHSV(timeHue,255,255), SHORTEST_HUES);
}


void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16(13,0,NUM_LEDS);
  leds[pos] += CHSV( gHue, 255, 192);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16(i+7,0,NUM_LEDS)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

uint8_t getPatternNumber(uint16_t knob) {
  uint16_t nextVal = 0;
  uint16_t lastVal = 0;
  for (int i = 0; i<11; i++) {
    if (i==0) {
      lastVal = 0;
    } else {
      lastVal = knobValues[i] - (knobValues[i] - knobValues[i-1])/2;
    }

    if (i==10) {
      nextVal = 2000;
    } else {
      nextVal = ((knobValues[i+1] - knobValues[i])/2) + knobValues[i];
    }

    Serial.print("lastVal:");
    Serial.println(lastVal);
    Serial.print("nextVal:");
    Serial.println(nextVal);
    
    if (knob >= lastVal && knob < nextVal){
      return i;
    }
  }
  //catchall
  return 0;
}



