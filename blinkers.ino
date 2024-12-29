#include <Wire.h>
#include <FastLED.h>

#define LED_PIN D1
#define NUM_LEDS 6
#define BRIGHTNESS 255
#define ORANGE CRGB(255, 165, 0)
#define BLINK_DELAY 300 // Milliseconds between blinks

CRGB leds[NUM_LEDS];

// ADXL345 I2C address
#define ADXL345_ADDRESS 0x53

// On/Off switch pin
#define SWITCH_PIN D2

void setupADXL345() {
  Wire.begin();
  // Set ADXL345 to measure mode
  Wire.beginTransmission(ADXL345_ADDRESS);
  Wire.write(0x2D); // Power control register
  Wire.write(0x08); // Measure mode
  Wire.endTransmission();
}

void readADXL345(int &x, int &y, int &z) {
  Wire.beginTransmission(ADXL345_ADDRESS);
  Wire.write(0x32); // Start with data register 0x32
  Wire.endTransmission(false);
  Wire.requestFrom(ADXL345_ADDRESS, 6, true);

  x = Wire.read() | (Wire.read() << 8);
  y = Wire.read() | (Wire.read() << 8);
  z = Wire.read() | (Wire.read() << 8);
}

void setup() {
  pinMode(SWITCH_PIN, INPUT_PULLUP); // Configure the switch pin with an internal pull-up resistor

  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();

  Wire.begin();
  setupADXL345();
}

void blinkLeft() {
  for (int i = 0; i < 4; i++) {
    leds[0] = ORANGE;
    leds[1] = ORANGE;
    leds[2] = ORANGE;
    FastLED.show();
    delay(BLINK_DELAY);

    leds[0] = CRGB::Black;
    leds[1] = CRGB::Black;
    leds[2] = CRGB::Black;
    FastLED.show();
    delay(BLINK_DELAY);
  }
}

void blinkRight() {
  for (int i = 0; i < 4; i++) {
    leds[3] = ORANGE;
    leds[4] = ORANGE;
    leds[5] = ORANGE;
    FastLED.show();
    delay(BLINK_DELAY);

    leds[3] = CRGB::Black;
    leds[4] = CRGB::Black;
    leds[5] = CRGB::Black;
    FastLED.show();
    delay(BLINK_DELAY);
  }
}

void loop() {
  // Check if the switch is ON
  if (digitalRead(SWITCH_PIN) == LOW) { // LOW means the switch is ON
    int x, y, z;
    readADXL345(x, y, z);

    // Determine lean direction
    if (x > 200) { // Adjust threshold based on sensitivity
      blinkLeft();
    } else if (x < -200) {
      blinkRight();
    }
  } else {
    // Turn off LEDs when the switch is OFF
    FastLED.clear();
    FastLED.show();
  }
}
