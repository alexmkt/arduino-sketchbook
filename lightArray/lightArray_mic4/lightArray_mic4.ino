#include <TimerOne.h>

#include "SPI.h"
#include "Adafruit_WS2801.h"


/*****************************************************************************
Example sketch for driving Adafruit WS2801 pixels!


  Designed specifically to work with the Adafruit RGB Pixels!
  12mm Bullet shape ----> https://www.adafruit.com/products/322
  12mm Flat shape   ----> https://www.adafruit.com/products/738
  36mm Square shape ----> https://www.adafruit.com/products/683

  These pixels use SPI to transmit the color data, and have built in
  high speed PWM drivers for 24 bit color per pixel
  2 pins are required to interface

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution

*****************************************************************************/

// Choose which 2 pins you will use for output.
// Can be any valid output pins.
// The colors of the wires may be totally different so
// BE SURE TO CHECK YOUR PIXELS TO SEE WHICH WIRES TO USE!
int dataPin  = 11;    // Yellow wire on Adafruit Pixels
int clockPin = 13;    // Green wire on Adafruit Pixels
//Blue wire is ground
int buttonPin = 2;
int val = 0;
volatile int pattern=0;
volatile unsigned long button_time = 0;  
volatile unsigned long last_button_time = 0; 
volatile int audiobright=0;
volatile int audiobright1=0;
volatile int audiotemp=0;
volatile int lastaudiobright = 0;
volatile int avbrights = 0;
volatile int avindex = 0;

// Don't forget to connect the ground wire to Arduino ground,
// and the +5V wire to a +5V supply

// Set the first variable to the NUMBER of pixels. 25 = 25 pixels in a row
Adafruit_WS2801 strip = Adafruit_WS2801(25, dataPin, clockPin);

// Optional: leave off pin numbers to use hardware SPI
// (pinout is then specific to each board and can't be changed)
//Adafruit_WS2801 strip = Adafruit_WS2801(25);

// For 36mm LED pixels: these pixels internally represent color in a
// different format.  Either of the above constructors can accept an
// optional extra parameter: WS2801_RGB is 'conventional' RGB order
// WS2801_GRB is the GRB order required by the 36mm pixels.  Other
// than this parameter, your code does not need to do anything different;
// the library will handle the format change.  Examples:
//Adafruit_WS2801 strip = Adafruit_WS2801(25, dataPin, clockPin, WS2801_GRB);
//Adafruit_WS2801 strip = Adafruit_WS2801(25, WS2801_GRB);

void setup() {
    
  Serial.begin(9600);
  randomSeed(analogRead(1));
  //pinMode(buttonPin,INPUT);
  attachInterrupt(0, changePattern, RISING);
  Timer1.initialize(10000); // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
  Timer1.attachInterrupt( relativeVolume );
  
  
  strip.begin();

  // Update LED contents, to start they are all 'off'
  strip.show();
}


void loop() {
  // Some example procedures showing how to display to the pixels
  
 
  //Serial.println(pattern);
  //randomfades();
  
  if (pattern==0){
    clearStrip();
  } else if (pattern==1){
    //sideToSide();
    //fullOnMic();
    vuMeter();
  }else if (pattern==2){
    multiSides();
  }else if (pattern==3){
    multiSidesColor();
  }else if (pattern==4){
    randomfades();
  }else if (pattern==5){
    randomness();
  }else if (pattern==6){
    colorWipe(Color(255, 0, 0), 50);
  }else if (pattern==7){
    rainbow(analogRead(0));
  }else {
    pattern=0;
  }
  
  
  
  //randomness();
  //colorWipe(Color(255, 0, 0), 50);
  //colorWipe(Color(0, 255, 0), 50);
  //colorWipe(Color(0, 0, 255), 50);
  //rainbow(analogRead(0));
  //rainbowCycle(20);
}

void readMicBright(){
  
  
 int i=0;

  audiobright=0;

 for (i=0; i < 10; i++) {
   audiotemp = analogRead(5); 
 
 
   audiotemp = audiotemp-512;
    if (audiotemp<0){
      audiotemp=audiotemp*-1;
    }

    if (audiotemp>audiobright){
      audiobright=audiotemp;
    }
  
 }
  
 //audiobright=audiobright/2;
 if (audiobright>100){
    audiobright=255;
 }else{
     audiobright=50;
 }
 

  
}


void readMicAddBright(){
  
  
 int i=0;

  audiobright=0;

 for (i=0; i < 10; i++) {
   audiotemp = analogRead(5);
  //Serial.println(audiotemp); 
   audiotemp = audiotemp-512;
    if (audiotemp<0){
      audiotemp=audiotemp*-1;
    }

  audiobright+=audiotemp;
 }
  
 //audiobright=audiobright/2;
 if (audiobright>1000){
    audiobright=255;
 }else{
     audiobright=50;
 }
 

  
}

void relativeVolume(){
  /*
  volatile int lastaudiobright = 0;
volatile int avbrights[10];
volatile int avindex = 0;
  */
 int i=0;

  audiobright1=0;

 for (i=0; i < 20; i++) {
   audiotemp = analogRead(5);
  //Serial.println(audiotemp); 
   audiotemp = audiotemp-512;
    if (audiotemp<0){
      audiotemp=audiotemp*-1;
    }
   if (audiobright1<audiotemp){
    audiobright1=audiotemp;
   }
 }
  /*
 //audiobright=audiobright/2;
 if (audiobright>1000){
    audiobright=255;
 }else{
     audiobright=50;
 }
 
*/
  avbrights += audiobright1;
  avindex++;
  if (avindex==10){
    avindex=0;
   
     
    audiobright = avbrights/20.1;
    avbrights=0;
  }
  
  
  
/*
  if ((audiobright-lastaudiobright)>51){
    audiobright=lastaudiobright+51;
  }else if ((audiobright-lastaudiobright)<-51){
    audiobright=lastaudiobright-51;
  }else{
     audiobright=lastaudiobright;
  }
*/

  
}




void changePattern(){
  
  button_time = millis();
  if (button_time - last_button_time > 250)
  {
    pattern++;
    last_button_time = button_time;
  }
}

void clearStrip(){
  int i;
  for (i=0; i < strip.numPixels(); i++) {
          strip.setPixelColor(i, 0);       
    }
    strip.show();
}


void fullOnMic(){
  int i;
  for (i=0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, Color(0,0,audiobright));
  }
  strip.show();
  
}

void vuMeter(){
  int i;
 
  //Serial.println(audiobright);
  
  int brightness;
  
  brightness = audiobright/10;
  
  for (i=0; i < strip.numPixels(); i++) {
    
    //Serial.print("i:");
    //Serial.println(i);
    //Serial.println(audiobright);
    

      
    if ((i+1)<(brightness ) ){
      
          
        //if (brightness-i >= 3 ){
          strip.setPixelColor(i, Color(0,0,127));
        //}else{
          //strip.setPixelColor(i, Color(0,0,255/(brightness-(i+1) ) ) );
        //}
      
    }else{
      strip.setPixelColor(i, 0);
    }
        
  }
  strip.show();
  delay(20);
  
}

void sideToSide(){
  int i;
  
  int rows = 5;
  int cols = 5;
  uint32_t color;

  Serial.println("Analog");
  Serial.println(analogRead(5));
  Serial.println("Bright");
  Serial.println(audiobright);

  int offset = 0;
  while (offset<(cols)){
  
    for (i=0; i < strip.numPixels(); i++) {
        if ( (i%(rows*2)<rows && i%rows==offset) || (i%(rows*2)>=rows && ((rows-1)-(i%rows))==offset)) {
          //strip.setPixelColor(i, Wheel(analogRead(0)/4)); 
          color = Wheel(analogRead(0)/4);
          strip.setPixelColor(i, Color(RedPart(color)/(257-audiobright),GreenPart(color)/(257-audiobright),BluePart(color)/(257-audiobright) ));
        }else{
          strip.setPixelColor(i, 0); 
        }
       
    }
    
    strip.show();
    offset++;
    
    delay(100);
  }
  
}



void multiSides(){
  int i;
  
  int rows = 5;
  int cols = 5;
  

//left to right
  int offset = 0;
  while (offset<(cols)){
  
    for (i=0; i < strip.numPixels(); i++) {
        if ( (i%(rows*2)<rows && i%rows==offset) || (i%(rows*2)>=rows && ((rows-1)-(i%rows))==offset)) {
          strip.setPixelColor(i, Wheel(analogRead(0)/4)); 
        }else{
          strip.setPixelColor(i, 0); 
        }
        
    }
    strip.show();
  
    offset++;
    
    delay(100);
  }
 
  offset = 0;
  while (offset<(rows)){
  
    for (i=0; i < strip.numPixels(); i++) {
        if ( i/cols==offset ) {
          strip.setPixelColor(i, Wheel(analogRead(0)/4)); 
        }else{
          strip.setPixelColor(i, 0); 
        }
        
    }
    
    strip.show();
  
    offset++;
    
    delay(100);
  }
  
  offset = cols-1;
  while (offset>=0){
  
    for (i=0; i < strip.numPixels(); i++) {
        if ( (i%(rows*2)<rows && i%rows==offset) || (i%(rows*2)>=rows && ((rows-1)-(i%rows))==offset)) {
          strip.setPixelColor(i, Wheel(analogRead(0)/4)); 
        }else{
          strip.setPixelColor(i, 0); 
        }
        
    }
    strip.show();
  
    offset--;
    
    delay(100);
  }
 
  offset = cols-1;
  while (offset>=0){
  
    for (i=0; i < strip.numPixels(); i++) {
        if ( i/cols==offset ) {
          strip.setPixelColor(i, Wheel(analogRead(0)/4)); 
        }else{
          strip.setPixelColor(i, 0); 
        }
        
    }
    
    strip.show();
  
    offset--;
    
    delay(100);
  }
  
}


void multiSidesColor(){
  int i;
  
  int rows = 5;
  int cols = 5;
  
  uint32_t color = Wheel(random(0,256));

//left to right
  int offset = 0;
  while (offset<(cols)){
  
    for (i=0; i < strip.numPixels(); i++) {
        if ( (i%(rows*2)<rows && i%rows==offset) || (i%(rows*2)>=rows && ((rows-1)-(i%rows))==offset)) {
          strip.setPixelColor(i, color); 
        }else{
          strip.setPixelColor(i, 0); 
        }
        
    }
    strip.show();
  
    offset++;
    
    delay(100);
  }
 
 color = Wheel(random(0,256));
  offset = 0;
  while (offset<(rows)){
  
    for (i=0; i < strip.numPixels(); i++) {
        if ( i/cols==offset ) {
          strip.setPixelColor(i, color); 
        }else{
          strip.setPixelColor(i, 0); 
        }
        
    }
    
    strip.show();
  
    offset++;
    
    delay(100);
  }
  
  color = Wheel(random(0,256));
  offset = cols-1;
  while (offset>=0){
  
    for (i=0; i < strip.numPixels(); i++) {
        if ( (i%(rows*2)<rows && i%rows==offset) || (i%(rows*2)>=rows && ((rows-1)-(i%rows))==offset)) {
          strip.setPixelColor(i, color); 
        }else{
          strip.setPixelColor(i, 0); 
        }
        
    }
    strip.show();
  
    offset--;
    
    delay(100);
  }
 
  color = Wheel(random(0,256));
  offset = cols-1;
  while (offset>=0){
  
    for (i=0; i < strip.numPixels(); i++) {
        if ( i/cols==offset ) {
          strip.setPixelColor(i, color); 
        }else{
          strip.setPixelColor(i, 0); 
        }
        
    }
    
    strip.show();
  
    offset--;
    
    delay(100);
  }
  
}



void randomness(){
  
  
  
  strip.setPixelColor(random(0,25), Color(random(0,256), random(0,256), random(0,256))); 
  strip.show();
  
  delay(50);
  
}

void randomfades(){
 
  uint32_t colors[25];  //color value
  int reds[25];
  int greens[25];
  int blues[25];
  int states[25];  //up=1, down=2, or off=0
  int brightness[25]; //0-255
  int waits[25]; //random time to wait before next cycle
  int i;
  
  
  for (i=0; i < strip.numPixels(); i++) {
    
     states[i] = 0;
     brightness[i] = 0;
     waits[i] = random(0,255);
     //waits[i] = 0;
     reds[i] = 0;
     greens[i] = 0;
     blues[i] = 0;
  }
  
  
  while (pattern==4){
    
    for (i=0; i < strip.numPixels(); i++) {
      
      
      
      if (waits[i]>0){ //still counting down
        waits[i]--;
        
        colors[i] = 0;
        reds[i] = 0;
        greens[i] = 0;
        blues[i] = 0;
        
      }else if (waits[i]==0 && states[i]==0){  //out of wait time, but needs a new color and needs to start going up.  
        //Serial.println("HERE");
        states[i]=1;
        brightness[i] = 1;
        colors[i] = Wheel(random(0,256));
        reds[i] = random(0,256);
        greens[i] = random(0,256);
        blues[i] = random(0,256);

      }else if (states[i]==1){  //Going up
      
         if (brightness[i]==245){
           
           brightness[i]--;
           states[i]=2;
         }else{
           brightness[i]++;
         }
      }else if (states[i]==2){
        
        brightness[i]--;
        
        if (brightness[i]==0){
         states[i] = 0;
         waits[i] = random(0,255);
          
        }

      }
      
      
      //strip.setPixelColor(i, Color(reds[i]/(255-brightness[i]),greens[i]/(255-brightness[i]),blues[i]/(255-brightness[i]) )); 
      
      
      //strip.setPixelColor(i,  colors[i]); 
      if (states[i]==0){
        strip.setPixelColor(i, 0);
      }else{
        strip.setPixelColor(i, Color(RedPart(colors[i])/(255-brightness[i]),GreenPart(colors[i])/(255-brightness[i]),BluePart(colors[i])/(255-brightness[i]) )); 
      }
      
      //Serial.print("Light:");
      //Serial.println(i);
      /*Serial.println("Wait:"+waits[i]);
      Serial.println(waits[i]);
      Serial.println("Red:"+reds[i]);
      Serial.println(reds[i]);
      Serial.println("Green:"+greens[i]);
      Serial.println(greens[i]);
      Serial.println("Blue:"+blues[i]);
      Serial.println(blues[i]);
      Serial.println("State:");
      Serial.println(states[i]);
      */
      /*
      Serial.println("Red:");
      Serial.println(RedPart(colors[i]));
      Serial.println("Green:");
      Serial.println(GreenPart(colors[i]));
      Serial.println("Blue:");
      Serial.println(BluePart(colors[i]));
      
      delay(1000);
      */
      
      
      
    }
    
    strip.show();
    
  }
  
}



void rainbow(uint8_t wait) {
  int i, j;
   
  for (j=0; j < 256; j+=8) {     // 3 cycles of all 256 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel( (i + j) % 255));
      strip.show();
      delay(analogRead(0));
      strip.setPixelColor(i, 0);
      strip.show();
      
    }  
    //strip.show();   // write all the pixels out
    //delay(wait);
  }
}

// Slightly different, this one makes the rainbow wheel equally distributed 
// along the chain
void rainbowCycle(uint8_t wait) {
  int i, j;
  
  for (j=0; j < 256 * 5; j++) {     // 5 cycles of all 25 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      // tricky math! we use each pixel as a fraction of the full 96-color wheel
      // (thats the i / strip.numPixels() part)
      // Then add in j which makes the colors go around per pixel
      // the % 96 is to make the wheel cycle around
      strip.setPixelColor(i, Wheel( ((i * 256 / strip.numPixels()) + j) % 256) );
    }  
    strip.show();   // write all the pixels out
    delay(wait);
  }
}

// fill the dots one after the other with said color
// good for testing purposes
void colorWipe(uint32_t c, uint8_t wait) {
  int i;
  
  for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

/* Helper functions */

// Create a 24 bit color value from R,G,B
uint32_t Color(byte r, byte g, byte b)
{
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}

//Input a value 0 to 255 to get a color value.
//The colours are a transition r - g -b - back to r
uint32_t Wheel(byte WheelPos)
{
  if (WheelPos < 85) {
   return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
   WheelPos -= 85;
   return Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170; 
   return Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

byte RedPart(uint32_t color){
  byte r;
  
  color>>=16;
  
  r = color;
  
  return r;

}

byte GreenPart(uint32_t color){
  byte g;
  
  color<<=8;
  color>>=16;
  
  g = (byte)color;
  
  return g;

}

byte BluePart(uint32_t color){
  byte b;
  
  color<<=16;
  color>>=16;
  
  b = (byte)color;
  
  return b;

}

  

