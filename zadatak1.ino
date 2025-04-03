/**
 * @file ButtonLedSensorSystem.ino
 *
 * @mainpage Button-LED-Sensor System Example
 *
 * @section description Description
 * This Arduino project controls LEDs based on button presses and sensor data,
 * while using interrupts and debounce logic to ensure accurate button presses.
 *
 * @section hardware Hardware Setup
 * - Button 0 connected to pin 3.
 * - Button 1 connected to pin 2.
 * - Button 2 connected to pin 4.
 * - LEDs connected to pins 13, 12, 11, 10, and 9.
 * - Distance sensor connected to pins 5 (Echo) and 6 (Trig).
 *
 * @section libraries Libraries
 * - Arduino (default Arduino library).
 *
 * @section notes Notes
 * - The project uses debouncing logic for button presses to avoid false triggers.
 * - Timer interrupt is used for periodic actions like LED blinking.
 *
 * @section author Author
 * - Created by Renato Rak on 04/03/2025.
 */

// Libraries
#include <Arduino.h>

// Pin Mapping
const int buttonPin0 = 3;       ///< Pin connected to button 0.
const int buttonPin1 = 2;       ///< Pin connected to button 1.
const int buttonPin2 = 4;       ///< Pin connected to button 2.

const int buttonLedPin0 = 13;   ///< Pin connected to LED for button 0.
const int buttonLedPin1 = 12;   ///< Pin connected to LED for button 1.
const int buttonLedPin2 = 11;   ///< Pin connected to LED for button 2.
const int timerLedPin = 10;     ///< Pin connected to timer LED.
const int sensorLedPin = 9;     ///< Pin connected to sensor LED.

volatile bool isButtonPressed0 = false;  ///< Flag for button 0 press.
volatile bool isButtonPressed1 = false;  ///< Flag for button 1 press.
volatile bool isButtonPressed2 = false;  ///< Flag for button 2 press.

// Debounce configuration
unsigned long lastDebounceTime0 = 0;   ///< Last debounce time for button 0.
unsigned long lastDebounceTime1 = 0;   ///< Last debounce time for button 1.
unsigned long lastDebounceTime2 = 0;   ///< Last debounce time for button 2.
const unsigned long debounceDelay = 3000; ///< Delay for debounce in milliseconds.

// Timer configuration
volatile bool isTimerTriggered = false;  ///< Flag for timer interrupt.

/// Distance sensor configuration
const int echoPin = 5;                ///< Pin connected to Echo of distance sensor.
const int trigPin = 6;                ///< Pin connected to Trig of distance sensor.
const int distanceThreshold = 100;     ///< Distance threshold in cm for sensor.
const int sensorLedBlinkTimeInterval = 200; ///< Blink interval for sensor LED in milliseconds.
volatile long lastSensorBlinkTime = 0; ///< Last time sensor LED blinked.
volatile bool ledState = false;        ///< Current state of the sensor LED.

 /**
  * Interrupt Service Routine (ISR) for button 0 press detection.
  * This function triggers when button 0 is pressed (Falling edge).
  */
void buttonISR0() {
  if (millis() - lastDebounceTime0 > debounceDelay) {
    lastDebounceTime0 = millis();
    isButtonPressed0 = true;
  }
}

/**
 * Interrupt Service Routine (ISR) for button 1 press detection.
 * This function triggers when button 1 is pressed (Falling edge).
 */
void buttonISR1() {
  if (millis() - lastDebounceTime1 > debounceDelay) {
    lastDebounceTime1 = millis();
    isButtonPressed1 = true;
  }
}

/**
 * Timer interrupt handler for periodic tasks.
 * This function is triggered when Timer1 reaches the preset count.
 */
ISR(TIMER1_COMPA_vect) {
  isTimerTriggered = true;
}

/**
 * The setup function is called once at the beginning.
 * It initializes pins, sets up interrupts, and configures the timer.
 */
void setup() {
  Serial.begin(9600);  ///< Initialize serial communication at 9600 baud rate.
  
  pinMode(buttonPin0, INPUT_PULLUP); ///< Set button pin 0 as input with internal pull-up resistor.
  pinMode(buttonPin1, INPUT_PULLUP); ///< Set button pin 1 as input with internal pull-up resistor.
  pinMode(buttonPin2, INPUT_PULLUP); ///< Set button pin 2 as input with internal pull-up resistor.
  
  pinMode(buttonLedPin0, OUTPUT);  ///< Set LED pin for button 0 as output.
  pinMode(buttonLedPin1, OUTPUT);  ///< Set LED pin for button 1 as output.
  pinMode(buttonLedPin2, OUTPUT);  ///< Set LED pin for button 2 as output.
  pinMode(timerLedPin, OUTPUT);    ///< Set timer LED pin as output.
  pinMode(sensorLedPin, OUTPUT);   ///< Set sensor LED pin as output.

  pinMode(echoPin, INPUT);  ///< Set Echo pin of distance sensor as input.
  pinMode(trigPin, OUTPUT); ///< Set Trig pin of distance sensor as output.
  
  attachInterrupt(digitalPinToInterrupt(buttonPin0), buttonISR0, FALLING); ///< Attach interrupt to button 0.
  attachInterrupt(digitalPinToInterrupt(buttonPin1), buttonISR1, FALLING); ///< Attach interrupt to button 1.

  // Timer setup: configure Timer1 for interrupt every 1 second.
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A = 15624; ///< Timer1 compare value for 1 second interval.
  TCCR1B |= (1 << WGM12);  ///< Configure Timer1 in CTC mode.
  TCCR1B |= (1 << CS12) | (1 << CS10);  ///< Set prescaler to 1024.
  TIMSK1 |= (1 << OCIE1A);  ///< Enable Timer1 compare interrupt.
  interrupts();
}

/**
 * The loop function is called repeatedly after setup.
 * It checks button presses, controls LEDs, and manages the distance sensor.
 */
void loop() {

  // Timer triggered LED blink.
  if (isTimerTriggered) {
    isTimerTriggered = false;
    digitalWrite(timerLedPin, HIGH);
    delay(75);
    digitalWrite(timerLedPin, LOW);
  }

  // Button 2 press detection with debounce.
  if (digitalRead(buttonPin2) == LOW && millis() - lastDebounceTime2 > debounceDelay) {
    isButtonPressed2 = true;
    lastDebounceTime2 = millis();
  }

  // Handle button 0 LED control.
  if (isButtonPressed0) {
    isButtonPressed0 = false;
    digitalWrite(buttonLedPin0, HIGH);
    delay(500);
    digitalWrite(buttonLedPin0, LOW);
  } 
  // Handle button 1 LED control.
  else if (isButtonPressed1) {
    isButtonPressed1 = false;
    digitalWrite(buttonLedPin1, HIGH);
    delay(500);
    digitalWrite(buttonLedPin1, LOW);
  } 
  // Handle button 2 LED control.
  else if (isButtonPressed2) {
    isButtonPressed2 = false;
    digitalWrite(buttonLedPin2, HIGH);
    delay(500);
    digitalWrite(buttonLedPin2, LOW);
  }

  // Distance sensor reading and LED control.
  long distance = readDistanceCM();

  // If the distance is below the threshold, blink the sensor LED.
  if (distance < distanceThreshold) {
      if (millis() - lastSensorBlinkTime >= sensorLedBlinkTimeInterval) {
          ledState = !ledState;
          digitalWrite(sensorLedPin, ledState ? HIGH : LOW); 
          lastSensorBlinkTime = millis();
      }
  } else {
      digitalWrite(sensorLedPin, LOW);  ///< Turn off sensor LED if distance is greater than threshold.
  }
}

/**
 * Reads the distance from the ultrasonic sensor.
 *
 * @return Distance in centimeters.
 */
float readDistanceCM() {
 digitalWrite(trigPin, LOW); ///< Set Trig pin low.
 delayMicroseconds(2); ///< Wait for 2 microseconds.
 digitalWrite(trigPin, HIGH); ///< Send trigger pulse.
 delayMicroseconds(10); ///< Wait for 10 microseconds.
 digitalWrite(trigPin, LOW); ///< Set Trig pin low again.
 int duration = pulseIn(echoPin, HIGH); ///< Measure the duration of the pulse.
 return duration * 0.034 / 2; ///< Calculate and return distance in centimeters.
}
