const int ntcTemperaturePin = A0;

// button konfiguracija
const int buttonPin = 2;
volatile bool isButtonPressed = false;
const int buttonLedPin = 4;

// motion konfiguracija
volatile bool isMotionDetected = false;
unsigned long lastMotionTime = 0;
const unsigned long motionDebounceTime = 500;
const int motionPirPin = 3;
const int motionLedPin = 13;

// timer konfiguracija
unsigned long previousTimerMillis = 0;
const long timerInterval = 10000;
volatile bool isTimerTriggered = false;
const int timerLedPin = 12;

void setup() {
  Serial.begin(9600);

  pinMode(buttonPin, INPUT);
  pinMode(motionPirPin, INPUT);
  pinMode(motionLedPin, OUTPUT);
  pinMode(timerLedPin, OUTPUT);
  pinMode(buttonLedPin, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonISR, FALLING);

  attachInterrupt(digitalPinToInterrupt(motionPirPin), pirISR, RISING);
}

void loop() {
  unsigned long currentMillis = millis();

  // provjera PIR senzora
  if (isMotionDetected) {
    Serial.println("Motion detected!");
    digitalWrite(motionLedPin, HIGH);
    delay(3000);
    digitalWrite(motionLedPin, LOW);
    isMotionDetected = false;
  }

  // postavljanje zastavice za timer
  if (currentMillis - previousTimerMillis >= timerInterval) {
    isTimerTriggered = true;
    previousTimerMillis = currentMillis;
  }

  // handlanje prekidima timera i gumba
  if (isTimerTriggered && isButtonPressed) {
      Serial.println("Timer and button triggered at the same time - handling timer first!");
      handleTimer();
      handleButton();
  } else if(isTimerTriggered) {
    handleTimer();
  } else if (isButtonPressed) {
    handleButton();
  }

  // izračun temperature
  const float BETA = 3950;
  int analogValue = analogRead(ntcTemperaturePin);
  float celsius = 1 / (log(1 / (1023. / analogValue - 1)) / BETA + 1.0 / 298.15) - 273.15;

  Serial.print("Temperature: ");
  Serial.print(celsius);
  Serial.println(" °C");

  delay(1000);
}

void handleTimer() {
    Serial.println("Timer triggered!");
    digitalWrite(timerLedPin, HIGH);
    delay(3000);
    digitalWrite(timerLedPin, LOW);
    isTimerTriggered = false;
}

void handleButton() {
    Serial.println("Button pressed!");
    digitalWrite(buttonLedPin, HIGH);
    delay(3000);
    digitalWrite(buttonLedPin, LOW);
    isButtonPressed = false;
}

void buttonISR() {
  isButtonPressed = true;
}

void pirISR() {
  unsigned long currentTime = millis();
  if (currentTime - lastMotionTime > motionDebounceTime) {
    isMotionDetected = true;
    lastMotionTime = currentTime;
  }
}
