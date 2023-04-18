
#define SENSOR_PIN 23 
#define LED_PIN 22

void setup() {
  // Set the LED pin as an output
  pinMode(LED_PIN, OUTPUT);

  // Set the sensor pin as an input
  pinMode(SENSOR_PIN, INPUT);
}

void loop() {
  // Read the sensor output
  int sensor_value = digitalRead(SENSOR_PIN);
  
  // If the sensor doesn't detect liquid, turn on the LED
  if (sensor_value == HIGH) {
    digitalWrite(LED_PIN, HIGH);
  }
  // Otherwise, turn off the LED
  else {
    digitalWrite(LED_PIN, LOW);
  }
  
  // Wait for a short period of time
  delay(100);
}
