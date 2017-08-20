#ifndef RAINBOWCYCLESOUND_H
#define RAINBOWCYCLESOUND_H

#include "Demo.h"

#define MIC_PIN         A4  // Microphone is attached to this analog pin (A4 for circuit playground)
#define SAMPLE_WINDOW   10  // Sample window for average level
#define PEAK_HANG       24  // Time of pause before peak dot falls
#define PEAK_FALL        4  // Rate of falling peak dot
#define INPUT_FLOOR     10  // Lower range of analogRead input

// Animation speeds to loop through with mode presses.  The current milliseconds 
// are divided by this value so the smaller the value the faster the animation.
//static int speeds[] = { 5, 10, 50, 100 };

class RainbowCycleSound: public Demo {
public:
  RainbowCycleSound() { currentSpeed = 0; }
  ~RainbowCycleSound() {}
  unsigned int lastBrightness = 255;

  virtual void loop() {

    //Sound tracking
    unsigned long startMillis= millis();  // Start of sample window
    float peakToPeak = 0;   // peak-to-peak level
  
    unsigned int signalMax = 0;
    unsigned int signalMin = 1023;
    unsigned int brightness = 255;
    unsigned int sample;
  
    // collect data for length of sample window (in mS)
    while (millis() - startMillis < SAMPLE_WINDOW)
    {
      sample = analogRead(MIC_PIN);
      if (sample < 1024)  // toss out spurious readings
      {
        if (sample > signalMax)
        {
          signalMax = sample;  // save just the max levels
        }
        else if (sample < signalMin)
        {
          signalMin = sample;  // save just the min levels
        }
      }
    }
    peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
    brightness = peakToPeak;
    brightness = (brightness*brightness)/255;
    if (brightness > 255) {
      brightness = 255;
    } else if (brightness < 1) {
      brightness = 1;
    }
    if (brightness >= lastBrightness-10 && brightness <= lastBrightness+10) {
      brightness = lastBrightness;
    }
    lastBrightness = brightness;
    
    Serial.print("Peak: ");
    Serial.println(peakToPeak);
    Serial.print("Brightness: ");
    Serial.println(brightness);
    
    // Make an offset based on the current millisecond count scaled by the current speed.
    uint32_t offset = millis() / speeds[currentSpeed];
    // Loop through each pixel and set it to an incremental color wheel value.
    for(int i=0; i<10; ++i) {
      CircuitPlayground.strip.setPixelColor(i, CircuitPlayground.colorWheel(((i * 256 / 10) + offset) & 255));
    }
    // Show all the pixels.
    CircuitPlayground.strip.setBrightness(brightness);
    CircuitPlayground.strip.show();
    
  }

  virtual void modePress() {
    // Increment through the available speeds.
    currentSpeed += 1;
    if (currentSpeed >= sizeof(speeds)/sizeof(int)) {
      currentSpeed = 0;
    }
  }

private:
  int currentSpeed;
  
};


#endif
