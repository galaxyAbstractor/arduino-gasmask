#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// MSGEQ7
#include "MSGEQ7.h"
#define pinAnalogLeft A1
#define pinReset 6
#define pinStrobe 4
#define MSGEQ7_INTERVAL ReadsPerSecond(50)
#define MSGEQ7_SMOOTH false

// LED
#define pinLed 5
Adafruit_NeoPixel ring = Adafruit_NeoPixel(16, pinLed, NEO_GRB + NEO_KHZ800);

uint8_t _volume = 0;
uint8_t _effect = 1;
uint8_t _minBright = 20;

CMSGEQ7<MSGEQ7_SMOOTH, pinReset, pinStrobe, pinAnalogLeft> MSGEQ7;

void setup() {
  // This will set the IC ready for reading
  MSGEQ7.begin();

  ring.begin();
  ring.setBrightness(_minBright);
  
  fillWithColor(_color);
  
  ring.show(); // Initialize all pixels to 'off'
}

void loop() {
  // Analyze without delay every interval
  bool newReading = MSGEQ7.read(MSGEQ7_INTERVAL);

  // Serial raw debug output
  if (newReading)
    processSound();

  switch(_effect) {
    case 0:
      handleBrightness();
      fillWithColor(_color);
      break;
    case 1:
      handleBlink();
      break;  
  }

  handleColor();
  
  ring.show();
}

void processSound() {
  _volume = MSGEQ7.getVolume(); 
}

uint8_t _brightness;
void setBrightness(uint8_t brightness) {

  if(MSGEQ7.getVolume() < 50) {
    _brightness = _minBright;
  } else {
    _brightness = brightness;
  }
}

void handleBrightness() {
   ring.setBrightness(_brightness);
}

uint8_t frame = 0;
void handleBlink() {

  setBrightness(MSGEQ7.getVolume());
  
  handleBrightness();

  if (frame == 0)
    ring.setPixelColor(rand() % 16, ring.Color(rand() % 256, rand() % 256 , rand() % 256));

  frame++;

  if (frame > 120) {
    for (int i = 0; i < 16; i++) {
      fillWithColor(ring.Color(0,0,0));
    }
    frame = 0;
  }
}

void fillWithColor(Color color) {
  for (int i = 0; i < 16; i++) {
      ring.setPixelColor(i, color);
    }
}

Color _color;
Color _standardColor = ring.Color(255, 0, 0);
bool _shouldRandom = false;
void handleColor() {
  if (_shouldRandom) {
    _color = ring.Color(rand() % 256, rand() % 256 , rand() % 256);  
  } else {
    _color = _standardColor;
    fillWithColor(_color);
  }

}

