/*
SparkFun Inventor's Kit
Example sketch 01-B

BLINKING LED FEEDBACK LOOP

  Turn one or two LEDs on for one half second, off for one half 
  second. Keep a history of the last N loop durations. If the last 
  loop duration was less than the average duration over the last N 
  iterations, blink the red light. If the last loop duration was 
  greater than the average duration blink the yellow LED. If the 
  last loop duration was equal to the average, blink both LEDs.
  Repeat forever.

Hardware connections:

  Most Arduinos already have an LED and resistor connected to
  pin 13, so you may not need any additional circuitry.

  But if you'd like to connect a second LED to pin 13, or use
  a different pin, follow these steps:

    Connect the positive side of your LED (longer leg) to Arduino
    digital pin 13 (or another digital pin, don't forget to change
    the code to match).
  
    Connect the negative side of your LED (shorter leg) to a 
    330 Ohm resistor (orange-orange-brown). Connect the other side
    of the resistor to ground.

    pin 12 _____ + RED LED - _____ 330 Ohm _____ GND
    pin 13 _____ + GREEN LED - _____ 330 Ohm _____ GND

	
    (We always use resistors between the Arduino and and LEDs
    to keep the LEDs from burning out due to too much current.)

This sketch was written by SparkFun Electronics,
with lots of help from the Arduino community.
This code is completely free for any use.
Visit http://learn.sparkfun.com/products/2 for SIK information.
Visit http://www.arduino.cc to learn about the Arduino.

Version 2.0 6/2012 MDG
*/

#define usbBaudRate 9600
#define redLED 12
#define yellowLED 13
#define history 256
#define seedDuration 1001588
unsigned long loopTime = 0;
unsigned long loopLast = 0;
unsigned long loopDuration = seedDuration;
unsigned long loopDurations[history];
unsigned long loopAvg = seedDuration;

// start the counter at 1000 to minimize the 50 microsecond delay
// for outputing a digit to the modem
unsigned long counter = 1000;

/**
 * returns an average loop duration over the last N iterations
 */
unsigned long avgDuration()
{
  unsigned long total = 0;
  for(int i=0; i<history; i++) {
    total += loopDurations[i];
  }
  return (total/history);
}

/**
 * Program entry point
 */
void setup()
{
  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);

  // Initialize the loopDuration array
  for(int i=0; i<history; i++) {
    loopDurations[i] = seedDuration;
  }

  Serial.begin(usbBaudRate);
}


/**
 * Program loop
 */
void loop()
{
  // Condition for lighting the Red LED
  if (loopDuration <= loopAvg) {
    digitalWrite(redLED, HIGH);
  }

  // Condition for lighting the Yellow LED
  if (loopDuration >= loopAvg) {
    digitalWrite(yellowLED, HIGH);
  }
  
  // Stay lit for half second
  delay(500);
  
  // Switch off LEDs for half second
  digitalWrite(redLED, LOW);
  digitalWrite(yellowLED, LOW);
  delay(500);

  // State Maintenance
  loopLast = loopTime;
  loopTime = micros();
  loopDuration = loopTime - loopLast;
  // Skip the first run, since it throws off the avg
  if (loopLast > 0) {
    loopDurations[counter%history] = loopDuration;
  }
  loopAvg = avgDuration();
  counter++;
  Serial.print("Loop Duration: ");
  Serial.print(loopDuration);
  Serial.print(" (avg: ");
  Serial.print(loopAvg);
  Serial.print(") at ");
  Serial.println(counter);
}

