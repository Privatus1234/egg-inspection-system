#define MQ135_PIN A0
#define BUZZER_PIN 4
#define LED_PIN 3

int threshold = 355;

void setup() {
  pinMode(MQ135_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
  Serial.println("MQ135 real-time H2S monitoring started...");
  Serial.print("Threshold set to: ");
  Serial.println(threshold);
}

void loop() {
  int sensorValue = analogRead(MQ135_PIN);
  float voltage = sensorValue * (5.0 / 1023.0);  // optional, for info

  Serial.print("Analog: ");
  Serial.print(sensorValue);
  Serial.print(" | Voltage: ");
  Serial.print(voltage, 2);
  Serial.println(" V");

  if (sensorValue >= threshold) {
    // 🔔 Sound buzzer continuously
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(LED_PIN, HIGH);
  } else {
    // 🔕 Stop buzzer
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
  }

  delay(500); // Read twice per second
}
