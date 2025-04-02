#include <Arduino.h>

// Gumb konfiguracija
const int buttonPin0 = 3;
const int buttonPin1 = 2;
const int buttonPin2 = 4;

const int buttonLedPin0 = 13;
const int buttonLedPin1 = 12;
const int buttonLedPin2 = 11;
const int timerLedPin = 10;
const int sensorLedPin = 9;

volatile bool isButtonPressed0 = false;
volatile bool isButtonPressed1 = false;
volatile bool isButtonPressed2 = false;

// Debounce konfiguracija
unsigned long lastDebounceTime0 = 0;
unsigned long lastDebounceTime1 = 0;
unsigned long lastDebounceTime2 = 0;
const unsigned long debounceDelay = 3000;

// Timer configuration
volatile bool isTimerTriggered = false;

// Distance sensor konfiguracija 
const int echoPin = 5;
const int trigPin = 6;
const int distanceThreshold = 100; // 100 cm
const int sensorLedBlinkTimeInterval = 200;
volatile long lastSensorBlinkTime = 0;
volatile bool ledState = false;

void buttonISR0() {
  if (millis() - lastDebounceTime0 > debounceDelay) {
    lastDebounceTime0 = millis();
    isButtonPressed0 = true;
  }
}

void buttonISR1() {
  if (millis() - lastDebounceTime1 > debounceDelay) {
    lastDebounceTime1 = millis();
    isButtonPressed1 = true;
  }
}

ISR(TIMER1_COMPA_vect) {
  isTimerTriggered = true;
}

void setup() {
  Serial.begin(9600);
  
  pinMode(buttonPin0, INPUT_PULLUP);
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  
  pinMode(buttonLedPin0, OUTPUT);
  pinMode(buttonLedPin1, OUTPUT);
  pinMode(buttonLedPin2, OUTPUT);
  pinMode(timerLedPin, OUTPUT);
  pinMode(sensorLedPin, OUTPUT);

  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  
  attachInterrupt(digitalPinToInterrupt(buttonPin0), buttonISR0, FALLING);
  attachInterrupt(digitalPinToInterrupt(buttonPin1), buttonISR1, FALLING);

  // setupanje timera
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A = 15624;
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  TIMSK1 |= (1 << OCIE1A);
  interrupts();
}

void loop() {

  if (isTimerTriggered) {
    isTimerTriggered = false;
    digitalWrite(timerLedPin, HIGH);
    delay(75);
    digitalWrite(timerLedPin, LOW);
  }

  if (digitalRead(buttonPin2) == LOW && millis() - lastDebounceTime2 > debounceDelay) {
    isButtonPressed2 = true;
    lastDebounceTime2 = millis();
  }

  if (isButtonPressed0) {
    isButtonPressed0 = false;
    digitalWrite(buttonLedPin0, HIGH);
    delay(500);
    digitalWrite(buttonLedPin0, LOW);
  } else if (isButtonPressed1) {
    isButtonPressed1 = false;
    digitalWrite(buttonLedPin1, HIGH);
    delay(500);
    digitalWrite(buttonLedPin1, LOW);
  } else if (isButtonPressed2) {
    isButtonPressed2 = false;
    digitalWrite(buttonLedPin2, HIGH);
    delay(500);
    digitalWrite(buttonLedPin2, LOW);
  }

  long distance = readDistanceCM();

  if (distance < distanceThreshold) {
      if (millis() - lastSensorBlinkTime >= sensorLedBlinkTimeInterval) {
          ledState = !ledState;
          digitalWrite(sensorLedPin, ledState ? HIGH : LOW); 
          lastSensorBlinkTime = millis();
      }
  } else {
      digitalWrite(sensorLedPin, LOW);  
  }
}

float readDistanceCM() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  int duration = pulseIn(echoPin, HIGH);
  if (duration == 0) {
    Serial.println("Nevaljan signal! Ignoriram očitanje.");
    return -1;
  }
  return duration * 0.034 / 2;
}
