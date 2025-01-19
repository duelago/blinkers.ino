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

// Adjusted G-force threshold for more accurate turn detection
#define TURN_THRESHOLD 1.4   // G-force to register a turn
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
  for (int i = 0; i < 3; i++) {  // Reduced blinks to 3
    leds[0] = ORANGE;
    leds[1] = ORANGE;
    leds[2] = ORANGE;
    leds[3] = ORANGE;
    FastLED.show();
    delay(BLINK_DELAY);
    FastLED.clear();
    FastLED.show();
    delay(BLINK_DELAY);
  }
}

void blinkRight() {
  for (int i = 0; i < 3; i++) {  // Reduced blinks to 3
    leds[4] = ORANGE;
    leds[5] = ORANGE;
    leds[6] = ORANGE;
    leds[7] = ORANGE;
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

  // Adjusted turn detection based on X-axis
  if (x > TURN_THRESHOLD && (currentTime - lastTurnTime) > DEBOUNCE_TIME) {
    Serial.println("Left turn detected!");
    blinkLeft();
    lastTurnTime = currentTime;
  }

  if (x < -TURN_THRESHOLD && (currentTime - lastTurnTime) > DEBOUNCE_TIME) {
    Serial.println("Right turn detected!");
    blinkRight();
    lastTurnTime = currentTime;
  }
}
