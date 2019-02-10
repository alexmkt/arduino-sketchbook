void discardFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data, IPAddress remoteIP) {
  
}

void onDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data, IPAddress remoteIP)
{
  // set brightness of the whole strip
  
  // Store which universe has got in
  if ((universe - startUniverse) < maxUniverses)
    universesReceived[universe - startUniverse] = 1;

/*  for (int i = 0 ; i < maxUniverses ; i++)
  {
    if (universesReceived[i] == 0)
    {
      Serial.println("Broke");
      digitalWrite(13, HIGH);
      sendFrame = 0;
      break;
    }
  }*/
  //int pixnum = (universe - startUniverse) * (previousDataLength / 4);
  int pixnum = 0;
   //read universe and put into the right part of the display buffer
  //Serial.println("Settings pixels...");
  for (int i = 0; i < 120; i+=6) //number of pixels in DMX packet
  {
      //int led = i + (universe - startUniverse) * (previousDataLength / 4);
      leds1.setPixelColor(universe*20+pixnum, pgm_read_byte(&gamma8[data[i * 4]]), pgm_read_byte(&gamma8[data[i * 4 + 1]]), pgm_read_byte(&gamma8[data[i * 4 + 2]]), pgm_read_byte(&gamma8[data[i * 4 + 3]]));
      //leds1.setPixelColor(universe*20+pixnum, 255,0,0,0);
      leds2.setPixelColor(universe*20+pixnum, pgm_read_byte(&gamma8[data[(i+1) * 4]]), pgm_read_byte(&gamma8[data[(i+1) * 4 + 1]]), pgm_read_byte(&gamma8[data[(i+1) * 4 + 2]]), pgm_read_byte(&gamma8[data[(i+1) * 4 + 3]]));
      
      leds3.setPixelColor(universe*20+pixnum, pgm_read_byte(&gamma8[data[(i+2) * 4]]), pgm_read_byte(&gamma8[data[(i+2) * 4 + 1]]), pgm_read_byte(&gamma8[data[(i+2) * 4 + 2]]), pgm_read_byte(&gamma8[data[(i+2) * 4 + 3]]));
      
      leds4.setPixelColor(universe*20+pixnum, pgm_read_byte(&gamma8[data[(i+3) * 4]]), pgm_read_byte(&gamma8[data[(i+3) * 4 + 1]]), pgm_read_byte(&gamma8[data[(i+3) * 4 + 2]]), pgm_read_byte(&gamma8[data[(i+3) * 4 + 3]]));
      leds5.setPixelColor(universe*20+pixnum, pgm_read_byte(&gamma8[data[(i+4) * 4]]), pgm_read_byte(&gamma8[data[(i+4) * 4 + 1]]), pgm_read_byte(&gamma8[data[(i+4) * 4 + 2]]), pgm_read_byte(&gamma8[data[(i+4) * 4 + 3]]));
      leds6.setPixelColor(universe*20+pixnum, pgm_read_byte(&gamma8[data[(i+5) * 4]]), pgm_read_byte(&gamma8[data[(i+5) * 4 + 1]]), pgm_read_byte(&gamma8[data[(i+5) * 4 + 2]]), pgm_read_byte(&gamma8[data[(i+5) * 4 + 3]]));
      pixnum++;
  }
  previousDataLength = length;

  leds1.show();
  leds2.show();
  leds3.show();
  leds4.show();
  leds5.show();
  leds6.show();
  if (millis()-lasttime > 4000){
    Watchdog.reset();
    lasttime = millis();
  }
    
    // Reset universeReceived to 0
  memset(universesReceived, 0, maxUniverses);
}

void initTest()
{
  for (int i = 0 ; i < numLeds ; i++)
    leds1.setPixelColor(i, 127, 0, 0, 0);
  leds1.show();
  delay(5500);
  for (int i = 0 ; i < numLeds ; i++)
    leds1.setPixelColor(i, 0, 127, 0, 0);
  leds1.show();
  delay(500);
  for (int i = 0 ; i < numLeds ; i++)
    leds1.setPixelColor(i, 0, 0, 127, 0);
  leds1.show();
  delay(500);
    for (int i = 0 ; i < numLeds ; i++)
    leds1.setPixelColor(i, 0, 0, 0, 127);
  leds1.show();
  delay(500);
  for (int i = 0 ; i < numLeds ; i++)
    leds1.setPixelColor(i, 0, 0, 0, 0);
  leds1.show();
}
