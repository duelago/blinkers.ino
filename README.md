<h1>Install Libraries:</h1>


Adafruit Unified Sensor (optional but helpful for debugging).
<br>
Wire (built-in for I2C communication).
<br>

Power On: Once powered, the ADXL345 will start detecting motion.
<br>
<b>Tilt Left:</b>
Lean the ADXL345 left; LEDs 1, 2, and 3 will blink orange 4 times.
<br>
<b>Tilt Right:</b>
Lean the ADXL345 right; LEDs 4, 5, and 6 will blink orange 4 times.
Transition: If you quickly transition from left to right, the LEDs will interrupt and start blinking for the new direction.

Flickering LEDs:
<p></p>
Add a <b></b>330Ω resistor</b> in series with the data line.<br>


<b>Connecting the Switch</b>
Use a simple SPST (Single Pole Single Throw) switch.<br>
Connect one terminal of the switch to GND.<br>
Connect the other terminal to D2 (GPIO 4) on the Wemos D1 Mini.</br>
Note: The code uses an internal pull-up resistor on D2, so no external resistor is needed.
<p>
How It Works<br>
When the switch is closed (connected to GND), digitalRead(SWITCH_PIN) returns LOW, and the program runs the LED blinking logic.<br>
When the switch is open (disconnected from GND), digitalRead(SWITCH_PIN) returns HIGH, and the LEDs are turned off, and the program does nothing.
