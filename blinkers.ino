#include <Wire.h>
#include <FastLED.h>

#define LED_PIN D6
#define NUM_LEDS 8
#define BRIGHTNESS 255
#define ORANGE CRGB(255, 165, 0)
#define BLINK_DELAY 300 // Milliseconds between blinks

CRGB leds[NUM_LEDS];

// ADXL345 I2C address
#define ADXL345_ADDRESS 0x53

#define TURN_THRESHOLD 1.6   // G-force to register a turn
#define DEBOUNCE_TIME 500    // Milliseconds to avoid false turns

// Timing for debounce
unsigned long lastTurnTime = 0;

void setupADXL345() {
  Wire.begin();
  Wire.beginTransmission(ADXL345_ADDRESS);
  Wire.write(0x2D);  // Power control register
  Wire.write(0x08);  // Measure mode
  Wire.endTransmission();
}

void readADXL345(float &x, float &y, float &z) {
  Wire.beginTransmission(ADXL345_ADDRESS);
  Wire.write(0x32);  // Start with data register 0x32
  Wire.endTransmission(false);
  Wire.requestFrom(ADXL345_ADDRESS, 6, true);

  int16_t rawX = Wire.read() | (Wire.read() << 8);
  int16_t rawY = Wire.read() | (Wire.read() << 8);
  int16_t rawZ = Wire.read() | (Wire.read() << 8);

  // Convert raw data to G values
  x = rawX * 0.0039;  // 3.9 mg/LSB
  y = rawY * 0.0039;
  z = rawZ * 0.0039;

  Serial.print("X: ");
  Serial.print(x, 2);
  Serial.print("g, Y: ");
  Serial.print(y, 2);
  Serial.print("g, Z: ");
  Serial.print(z, 2);
  Serial.println("g");
}

void setup() {
  Serial.begin(115200);
  Serial.println("Initializing...");

  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();

  for (int i = 0; i < 2; i++) {
    fill_solid(leds, NUM_LEDS, ORANGE);
    FastLED.show();
    delay(BLINK_DELAY);
    FastLED.clear();
    FastLED.show();
    delay(BLINK_DELAY);
  }

  Wire.begin();
  setupADXL345();

  Serial.println("ADXL345 initialized.");
}

void blinkLeft() {
  for (int i = 0; i < 3; i++) {
    fill_solid(leds, NUM_LEDS / 2, ORANGE);
    FastLED.show();
    delay(BLINK_DELAY);
    FastLED.clear();
    FastLED.show();
    delay(BLINK_DELAY);
  }
}

void blinkRight() {
  for (int i = 0; i < 3; i++) {
    fill_solid(&leds[NUM_LEDS / 2], NUM_LEDS / 2, ORANGE);
    FastLED.show();
    delay(BLINK_DELAY);
    FastLED.clear();
    FastLED.show();
    delay(BLINK_DELAY);
  }
}

void loop() {
  float x, y, z;
  readADXL345(x, y, z);

  unsigned long currentTime = millis();

  // Determine the axis with the maximum G-force, excluding Z-axis
  float maxG = max(abs(x), abs(y));
  if ((maxG > TURN_THRESHOLD) && (currentTime - lastTurnTime) > DEBOUNCE_TIME) {
    if (x > 0 || y > 0) {
      Serial.println("Positive G-force on X or Y detected: Blinking left");
      blinkLeft();
    } else {
      Serial.println("Negative G-force on X or Y detected: Blinking right");
      blinkRight();
    }
    lastTurnTime = currentTime;
  }
}
