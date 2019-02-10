#include <Adafruit_SleepyDog.h>
#include <SPI.h>
#include <Artnet.h>


#include "arduino_secrets.h" 

#ifdef MAX_BUFFER_ARTNET
#undef MAX_BUFFER_ARTNET
#define MAX_BUFFER_ARTNET 2120
#endif

#include <Adafruit_NeoPixel.h>

//#define SERIAL_DEBUG true

// Neopixel settings
const int numLeds = 240; // change for your setup
const int numberOfChannels = numLeds * 4; // Total number of channels you want to receive (1 led = 4 channels)
const byte dataPin1 = 5;
const byte dataPin2 = 6;
const byte dataPin3 = 9;
const byte dataPin4 = 10;
const byte dataPin5 = 11;
const byte dataPin6 = 12;
Adafruit_NeoPixel leds1 = Adafruit_NeoPixel(numLeds/6, dataPin1, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel leds2 = Adafruit_NeoPixel(numLeds/6, dataPin2, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel leds3 = Adafruit_NeoPixel(numLeds/6, dataPin3, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel leds4 = Adafruit_NeoPixel(numLeds/6, dataPin4, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel leds5 = Adafruit_NeoPixel(numLeds/6, dataPin5, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel leds6 = Adafruit_NeoPixel(numLeds/6, dataPin6, NEO_GRBW + NEO_KHZ800);

extern const char* KNOWN_SSID[];
extern const char* KNOWN_PASSWORD[];
const int   KNOWN_SSID_COUNT = sizeof(KNOWN_SSID) / sizeof(KNOWN_SSID[0]); // number of known networks

const uint8_t PROGMEM gamma8[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };
  
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
//char ssid[] = SECRET_SSID;        // your network SSID (name)
//char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                 // your network key Index number (needed only for WEP)
const char* ssid;
const char* pass;

Artnet artnet;
const int startUniverse = 0; // CHANGE FOR YOUR SETUP most software this is 1, some software send out artnet first universe as 0.

// Check if we got all universes
const int maxUniverses = numberOfChannels / 512 + ((numberOfChannels % 512) ? 1 : 0);
bool universesReceived[maxUniverses];
bool sendFrame = 1;
int previousDataLength = 0;

// Change ip and mac address for your setup
byte ip[4];
byte mac[6];

int status = WL_IDLE_STATUS;
//WiFiServer server(80);
unsigned long lasttime = millis();
int countdownMS = Watchdog.enable(10000);
float t[] = {PI*-1, .0};

void setup() {
  boolean wifiFound = false;
  int i, n;

  WiFi.setPins(8,7,4,2);
#ifdef SERIAL_DEBUG
  Serial.begin(9600);
#endif
  pinMode(13, OUTPUT);      // set the LED pin mode

#ifdef SERIAL_DEBUG
  Serial.println(F("scan start"));
#endif
  int nbVisibleNetworks = WiFi.scanNetworks();
  Serial.println(F("scan done"));
  if (nbVisibleNetworks == 0) {
#ifdef SERIAL_DEBUG
    Serial.println(F("no networks found. Reset to try again"));
#endif
    while (true); // no need to go further, hang in there, will auto launch the Soft WDT reset
  }

  // ----------------------------------------------------------------
  // if you arrive here at least some networks are visible
  // ----------------------------------------------------------------
#ifdef SERIAL_DEBUG
  Serial.print(nbVisibleNetworks);
  Serial.println(" network(s) found");
#endif
  // ----------------------------------------------------------------
  // check if we recognize one by comparing the visible networks
  // one by one with our list of known networks
  // ----------------------------------------------------------------
  for (i = 0; i < nbVisibleNetworks; ++i) {
#ifdef SERIAL_DEBUG
    Serial.println(WiFi.SSID(i)); // Print current SSID
#endif
    for (n = 0; n < KNOWN_SSID_COUNT; n++) { // walk through the list of known SSID and check for a match
      if (strcmp(KNOWN_SSID[n], WiFi.SSID(i))) {
#ifdef SERIAL_DEBUG        
        Serial.print(F("\tNot matching "));
        Serial.println(KNOWN_SSID[n]);
#endif
      } else { // we got a match
        wifiFound = true;
        break; // n is the network index we found
      }
    } // end for each known wifi SSID
    if (wifiFound) break; // break from the "for each visible network" loop
  } // end for each visible network

  if (!wifiFound) {
#ifdef SERIAL_DEBUG
    Serial.println(F("no Known network identified. Reset to try again"));
#endif
    while (true); // no need to go further, hang in there, will auto launch the Soft WDT reset
  }

  // ----------------------------------------------------------------
  // if you arrive here you found 1 known SSID
  // ----------------------------------------------------------------
#ifdef SERIAL_DEBUG  
  Serial.print(F("\nConnecting to "));
  Serial.println(KNOWN_SSID[n]);
#endif
  // ----------------------------------------------------------------
  // We try to connect to the WiFi network we found
  // ----------------------------------------------------------------
  WiFi.begin(KNOWN_SSID[n], KNOWN_PASSWORD[n]);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
#ifdef SERIAL_DEBUG
    Serial.print(".");
#endif
  }
#ifdef SERIAL_DEBUG
  Serial.println("");
#endif
  // ----------------------------------------------------------------
  // SUCCESS, you are connected to the known WiFi network
  // ----------------------------------------------------------------
  ssid = KNOWN_SSID[n];
  pass = KNOWN_PASSWORD[n];
#ifdef SERIAL_DEBUG
  Serial.println(F("WiFi connected, your IP address is "));
  Serial.println(WiFi.localIP());
#endif
  WiFi.macAddress(mac);
  IPAddress WiFiIP = WiFi.localIP();
  for (int i = 0;i<4;i++) ip[i] = byte(WiFiIP[i]);
  
  digitalWrite(13, LOW);
  artnet.begin(mac, ip);
  leds1.begin();
  leds2.begin();
  leds3.begin();
  leds4.begin();
  leds5.begin();
  leds6.begin();
  
  artnet.setArtDmxCallback(onDmxFrame);

}

void loop() {
  if (WiFi.status() == WL_CONNECTED) artnet.read();
  else {
    WiFi.end();
    delay(2000);
    while ( status != WL_CONNECTED) {
#ifdef SERIAL_DEBUG
      Serial.print("Lost connection...");
      Serial.print("Attempting to reconnect to Network named: ");
      Serial.println(ssid);                   // print the network name (SSID);
#endif
      // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
      
      status = WiFi.begin(ssid, pass);
      // wait 10 seconds for connection:
      delay(2000);
#ifdef SERIAL_DEBUG
      Serial.println(status);
#endif
    }
    printWiFiStatus();
  }
}
