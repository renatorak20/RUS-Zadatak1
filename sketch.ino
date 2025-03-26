const int ntcPin = A0;
const int pirPin = 3;
const int buttonPin = 2;
const int ledPin = 13;

volatile bool buttonPressed = false;
volatile bool motionDetected = false;
unsigned long lastMotionTime = 0;
const unsigned long motionDebounceTime = 500; // 500 ms debounce time

void setup() {
  Serial.begin(9600);

  pinMode(buttonPin, INPUT);
  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);

  // Postavljanje prekida za tipkalo
  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonISR, FALLING);

  // Postavljanje prekida za PIR senzor
  attachInterrupt(digitalPinToInterrupt(pirPin), pirISR, RISING);
}

void loop() {
  if (buttonPressed) {
    Serial.println("Button pressed!");
    buttonPressed = false;
    delay(3000);
  }

  if (motionDetected) {
    Serial.println("Motion detected!");
    digitalWrite(ledPin, HIGH);
    delay(3000); // LED svijetli 1 sekundu
    digitalWrite(ledPin, LOW);
    motionDetected = false;
  }

  // Čitanje temperature s NTC termistora
  int adcValue = analogRead(ntcPin);
  float resistance = 10000.0 / ((1023.0 / adcValue) - 1);
  float temperatureK = 1.0 / (log(resistance / 10000.0) / 3950.0 + 1.0 / (25.0 + 273.15));
  float temperatureC = temperatureK - 273.15;

  Serial.print("Temperature: ");
  Serial.print(temperatureC);
  Serial.println(" °C");

  delay(1000); // Čitanje svakih 1 sekundu
}

void buttonISR() {
  buttonPressed = true;
}

void pirISR() {
  unsigned long currentTime = millis();
  if (currentTime - lastMotionTime > motionDebounceTime) {
    motionDetected = true;
    lastMotionTime = currentTime;
  }
}