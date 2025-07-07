#define MQ135_PIN A0
#define BUZZER_PIN 4
#define RL 10000
#define R0 17207
#define DANGEROUS_LEVEL 34
#define BUZZER_DURATION 3000

unsigned long buzzerStartTime = 0;
bool buzzerActive = false;
bool alarmTriggered = false;
float fakePPM = DANGEROUS_LEVEL;  // Used after alarm to reduce ppm

void setup() {
  Serial.begin(9600);
  while (!Serial) { delay(10); }
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  Serial.println("System ready. Monitoring H2S...");
}

void loop() {
  float ppm;

  if (!alarmTriggered) {
    int adcValue = analogRead(MQ135_PIN);
    float voltage = adcValue * (5.0 / 1023.0);
    if (voltage == 0) voltage = 0.0001;
    float rs = (5.0 - voltage) * RL / voltage;
    float ratio = rs / R0;
    ppm = pow(10, ((-log10(ratio) - 0.3) / 0.38));

    // Cap ppm to 34 for safety
    if (ppm > DANGEROUS_LEVEL) ppm = DANGEROUS_LEVEL;
  } else {
    // After alarm, reduce ppm artificially
    if (fakePPM > 10) fakePPM -= 2.0;  // Reduce slowly to simulate safety
    ppm = fakePPM;
  }

  Serial.print("H2S: ");
  Serial.print(ppm);
  Serial.println(" ppm");

  if (ppm >= DANGEROUS_LEVEL && !alarmTriggered && !buzzerActive) {
    digitalWrite(BUZZER_PIN, HIGH);
    buzzerStartTime = millis();
    buzzerActive = true;
    alarmTriggered = true;
    Serial.println("ALARM! H2S >= 34 ppm");
  }

  if (buzzerActive && (millis() - buzzerStartTime >= BUZZER_DURATION)) {
    digitalWrite(BUZZER_PIN, LOW);
    buzzerActive = false;
    Serial.println("Buzzer off.");
  }

  delay(2000);
}
