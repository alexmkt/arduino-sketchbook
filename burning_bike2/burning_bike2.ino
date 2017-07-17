#include <bitswap.h>
#include <chipsets.h>
#include <color.h>
#include <colorpalettes.h>
#include <colorutils.h>
#include <controller.h>
#include <cpp_compat.h>
#include <dmx.h>
#include <FastLED.h>
#include <fastled_config.h>
#include <fastled_delay.h>
#include <fastled_progmem.h>
#include <fastpin.h>
#include <fastspi.h>
#include <fastspi_bitbang.h>
#include <fastspi_dma.h>
#include <fastspi_nop.h>
#include <fastspi_ref.h>
#include <fastspi_types.h>
#include <hsv2rgb.h>
#include <led_sysdefs.h>
#include <lib8tion.h>
#include <noise.h>
#include <pixelset.h>
#include <pixeltypes.h>
#include <platforms.h>
#include <power_mgt.h>

#include <Encoder.h>

#define STRIP_PIN1 12
#define STRIP_PIN2 6

#define STRIP_LENGTH1 50
#define STRIP_LENGTH2 75

CRGB leds1[STRIP_LENGTH1];

Encoder encoder(0, 10);

#define BLUE 0
#define GREEN 1
#define RED 2
#define RAINBOW 3

#define ENC_MAX 255
uint8_t encoderValue = 0;
long encPosition = 127;

void setup() {
  Serial.begin(9600);
  encoder.write(127);
  FastLED.addLeds<NEOPIXEL, STRIP_PIN1>(leds1, STRIP_LENGTH1);
}

void loop() {

 encPosition = encoder.read();
 Serial.println(encPosition);
 if (encPosition > ENC_MAX) {
  encoder.write(ENC_MAX);
  encPosition = ENC_MAX;
 }else if (encPosition < 0) {
  encPosition = 0;
  encoder.write(0);
 }

 Serial.println(encPosition);
 
for(int dot = 0; dot < STRIP_LENGTH1; dot++) { 
            leds1[dot] = CRGB::Blue;
            FastLED.show();
            // clear this led for the next time around the loop
            leds1[dot] = CRGB::Black;
            delay(30);
        }
}



