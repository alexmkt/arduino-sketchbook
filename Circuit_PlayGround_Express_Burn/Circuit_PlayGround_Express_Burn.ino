#include <Adafruit_CircuitPlayground.h>
#include <Wire.h>
#include <SPI.h>
#include "Adafruit_SleepyDog.h"

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

#define DATA_PIN    A1
#define CP_NEO_PIN    8
#define LEFT_BUTTON_PIN 4
#define RIGHT_BUTTON_PIN 19
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
//base
#define NUM_LEDS 10
//coat
//#define NUM_LEDS    148
//hat
//#define NUM_LEDS 38
CRGB leds[NUM_LEDS];

uint8_t max_brightness=48;
uint8_t min_brightness=10;
#define FRAMES_PER_SECOND  120

#define MIC_PIN         A4  // Microphone is attached to this analog pin (A4 for circuit playground)
#define SAMPLE_WINDOW   10  // Sample window for average level
#define PEAK_HANG       24  // Time of pause before peak dot falls
#define PEAK_FALL        4  // Rate of falling peak dot
#define INPUT_FLOOR     10  // Lower range of analogRead input

#define RANDOM_TIME_MS 5000

void setup() {
  delay(3000); // 3 second delay for recovery
  Serial.begin(115200);
  CircuitPlayground.begin();
  
  // tell FastLED about the LED strip configuration
  //FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,CP_NEO_PIN,COLOR_ORDER>(leds, 10).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(max_brightness);
  

}


// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
//SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, confetti, rainbow, rainbowWithGlitter, confetti, rainbow, rainbowWithGlitter, confetti, juggle, bpm, rainbowFast, solid, gradient };
SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm, rainbowFast, solid, gradient };
#define PATTERN_COUNT 9


uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // Hue from the mag sensor
uint8_t timeHue = 0; //Hue over time

uint16_t brightness = max_brightness;

static unsigned int sample;
unsigned int lastBrightness = 255;

bool leftButtonNow = false;
bool leftButtonLast = false;
bool rightButtonNow = false;
bool rightButtonLast = false;
long leftButtonPressTime;
long rightButtonPressTime;
bool randomEnabled = false;

void loop() {
  
  Serial.print("Pattern: ");
  Serial.println(gCurrentPatternNumber);
  if (gCurrentPatternNumber >= 9) {
    Serial.println("Huge Error!");
  }

  // Audio stuff
  unsigned long startMillis= millis();  // Start of sample window
  float pressure = 0;   // peak-to-peak level

  pressure = CircuitPlayground.mic.soundPressureLevel(SAMPLE_WINDOW);
  Serial.println("Sound pressure: " + String(pressure));
  pressure = pressure - 50; //normalize to standard DB levels. 
  if (pressure <0) {
    pressure = 0;
  }
  float percentage = pressure/50;
  int difference = (max_brightness - min_brightness) * percentage;
  brightness = min_brightness + difference;
  
  if (brightness > max_brightness) {
    brightness = max_brightness;
  } else if (brightness < min_brightness) {
    brightness = min_brightness;
  }
  if (brightness >= lastBrightness-4 && brightness <= lastBrightness+4) {
    brightness = lastBrightness;
  }
  lastBrightness = brightness;
  Serial.println("brightness2: " + String(brightness));

    
  // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();

  // send the 'leds' array out to the actual LED strip
  FastLED.setBrightness(brightness);
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  if (randomEnabled) {
    EVERY_N_SECONDS( 20 ) { nextPattern(); } // change patterns periodically
  }


  //Button Handling start
  // If button is release, but was pressed last, do the action.
  leftButtonNow = CircuitPlayground.leftButton();
  rightButtonNow = CircuitPlayground.rightButton();
  Serial.println("Buttons: " + String(leftButtonNow) + " - " + String(rightButtonNow));

  if (!leftButtonNow && leftButtonLast){ //The button was pressed but is now released
    nextPattern();
    leftButtonLast = false;
    if (millis() - leftButtonPressTime > RANDOM_TIME_MS){ //If you've held the button, toggle random
      randomEnabled = !randomEnabled;
      FastLED.setBrightness(0);
      FastLED.clear();
      FastLED.show();
      delay(500);
    }
  } else if (leftButtonNow && !leftButtonLast) { //currently pressed but was released previously 
    leftButtonPressTime = millis();
  }
  leftButtonLast = leftButtonNow;

  if (!rightButtonNow && rightButtonLast){
      if (max_brightness + 24 > 96) {
          max_brightness = 0;
      }
      max_brightness += 24;
      if (max_brightness - 48 < 10){
        min_brightness = 10;
      } else {
        min_brightness = max_brightness-48;
      }
    rightButtonLast = false;
  } else {
    rightButtonLast = rightButtonNow;
  }

  Serial.print("Max Brightness: ");
  Serial.println(max_brightness);
  Serial.print("Min Brightness: ");
  Serial.println(min_brightness);
  Serial.print("Brightness: ");
  Serial.println(brightness);
}

void loop2()
{

  while (!CircuitPlayground.slideSwitch()) {
    // Turn off the pixels, then go into deep sleep for a second.
    FastLED.setBrightness(0);
    FastLED.clear();
    FastLED.show(); 
    Watchdog.sleep(1000);
  }

  Serial.print("Pattern: ");
  Serial.println(gCurrentPatternNumber);

  // Audio stuff
  unsigned long startMillis= millis();  // Start of sample window
  float peakToPeak = 0;   // peak-to-peak level

  unsigned int signalMax = 0;
  unsigned int signalMin = 1023;
  unsigned int c, y;

  peakToPeak = CircuitPlayground.mic.soundPressureLevel(SAMPLE_WINDOW);
  brightness = peakToPeak/2;
  brightness = (brightness*brightness)/(255);
  if (brightness > max_brightness) {
    brightness = max_brightness;
  } else if (brightness < min_brightness) {
    brightness = min_brightness;
  }
  if (brightness >= lastBrightness-4 && brightness <= lastBrightness+4) {
    brightness = lastBrightness;
  }
  lastBrightness = brightness;
  //
  
  
  // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();

  // send the 'leds' array out to the actual LED strip
  FastLED.setBrightness(brightness);
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  if (randomEnabled) {
    EVERY_N_SECONDS( 20 ) { nextPattern(); } // change patterns periodically
  }


  //Button Handling start
  // If button is release, but was pressed last, do the action.
  leftButtonNow = CircuitPlayground.leftButton();
  rightButtonNow = CircuitPlayground.rightButton();

  if (!leftButtonNow && leftButtonLast){ //The button was pressed but is now released
    nextPattern();
    leftButtonLast = false;
    if (millis() - leftButtonPressTime > RANDOM_TIME_MS){ //If you've held the button, toggle random
      randomEnabled = !randomEnabled;
      FastLED.setBrightness(0);
      FastLED.clear();
      FastLED.show();
      delay(500);
    }
  } else if (leftButtonNow && !leftButtonLast) { //currently pressed but was released previously 
    leftButtonPressTime = millis();
  }
  leftButtonLast = leftButtonNow;

  if (!rightButtonNow && rightButtonLast){
      if (max_brightness + 24 > 96) {
          max_brightness = 0;
      }
      max_brightness += 24;
      if (max_brightness - 48 < 10){
        min_brightness = 10;
      } else {
        min_brightness = max_brightness-48;
      }
    rightButtonLast = false;
  } else {
    rightButtonLast = rightButtonNow;
  }
  

//  // Left button will change the current demo.
//  if (leftFirst && !leftSecond) {
//    nextPattern();
//  }

//  if (rightFirst && !rightSecond) {
//      if (max_brightness + 24 > 96) {
//          max_brightness = 0;
//      }
//      max_brightness += 24;
//      if (max_brightness - 48 < 10){
//        min_brightness = 10;
//      } else {
//        min_brightness = max_brightness-48;
//      }
//  }
  Serial.print("Max Brightness: ");
  Serial.println(max_brightness);
  Serial.print("Min Brightness: ");
  Serial.println(min_brightness);
  Serial.print("Brightness: ");
  Serial.println(brightness);
  
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % PATTERN_COUNT;
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
    leds[ random16(NUM_LEDS - 1) ] += CRGB::White;
  }
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS - 1);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  //Serial.println("1");
  fadeToBlackBy( leds, NUM_LEDS, 20);
  //Serial.println("2");
  int pos = beatsin16(13,0,NUM_LEDS - 1);
  //Serial.println("pos: " + String(pos));
  leds[pos] += CHSV( gHue, 255, 192);
  //Serial.println("4");
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
  //Serial.println("5");
  fadeToBlackBy( leds, NUM_LEDS, 20);
  //Serial.println("6");
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    //Serial.println("7");
    leds[beatsin16(i+7,0,NUM_LEDS - 1)] |= CHSV(dothue, 200, 255);
    //Serial.println("8");
    dothue += 32;
  }
}
