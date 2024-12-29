Install Libraries:

Go to Sketch > Include Library > Manage Libraries and install:
FastLED for LED control.
Adafruit Unified Sensor (optional but helpful for debugging).
Wire (built-in for I2C communication).
Upload the Code:

Connect the Wemos D1 Mini to your computer via USB.
Select the appropriate COM Port under Tools > Port.
Choose Wemos D1 Mini as the board under Tools > Board.
Copy and paste the provided code into the Arduino IDE.
Click Upload to flash the code.
Testing the Project
Power On: Once powered, the ADXL345 will start detecting motion.
Tilt Left:
Lean the ADXL345 left; LEDs 1, 2, and 3 will blink orange 4 times.
Tilt Right:
Lean the ADXL345 right; LEDs 4, 5, and 6 will blink orange 4 times.
Transition: If you quickly transition from left to right, the LEDs will interrupt and start blinking for the new direction.
Troubleshooting
LEDs Not Lighting Up:

Check the WS2812B data pin connection.
Ensure the FastLED library is correctly installed.
Verify the power supply to the LEDs (5V recommended).
No Response from ADXL345:

Confirm the I2C address and wiring.
Use an I2C scanner sketch to verify the sensor is detected.
Flickering LEDs:

Add a 330Ω resistor in series with the data line.
Place a 1000µF capacitor across the LED strip’s power lines.
