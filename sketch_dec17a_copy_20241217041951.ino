// Pin Definitions
#define TRIG_PIN 7          // Ultrasonic Sensor TRIG pin
#define ECHO_PIN 6          // Ultrasonic Sensor ECHO pin
#define SOUND_SENSOR_PIN A0 // Sound sensor analog pin
#define BUZZER_PIN 10       // Buzzer pin
#define RED_LED_PIN 9       // Red LED for alarm indication
#define GREEN_LED_PIN 5     // Green LED for system OK indication
#define RESET_BUTTON 4      // Reset button pin

// Thresholds
const int SOUND_THRESHOLD = 400;   // Adjust sound threshold as needed
const int DISTANCE_THRESHOLD = 50; // Distance threshold in cm

bool alarmTriggered = false; // Alarm state

void setup() {
  // Pin Modes
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(SOUND_SENSOR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RESET_BUTTON, INPUT_PULLUP);

  // Initialize LEDs and Buzzer
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, HIGH); // Green LED ON initially
  noTone(BUZZER_PIN);

  Serial.begin(9600); // For debugging
}

void loop() {
  int distance = measureDistance();          // Get distance from ultrasonic sensor
  int soundLevel = analogRead(SOUND_SENSOR_PIN); // Get sound sensor value

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm | Sound Level: ");
  Serial.println(soundLevel);

  // Check for motion or sound
  if ((distance > 0 && distance < DISTANCE_THRESHOLD) || (soundLevel > SOUND_THRESHOLD)) {
    alarmTriggered = true; // Trigger alarm if thresholds are exceeded
  }

  // Handle Alarm State
  if (alarmTriggered) {
    triggerAlarm();
  }

  // Reset Alarm when Reset Button is Pressed
  if (digitalRead(RESET_BUTTON) == LOW) {
    resetAlarm();
  }

  delay(100); // Small delay for stability
}

// Function to Measure Distance with Ultrasonic Sensor
int measureDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  int distance = duration * 0.034 / 2; // Convert duration to distance in cm
  return distance;
}

// Function to Trigger Alarm
void triggerAlarm() {
  Serial.println("ALARM TRIGGERED!");

  // Flash Red LED and Sound the Buzzer
  for (int i = 0; i <= 255; i += 5) {  // Gradually increase brightness
    analogWrite(RED_LED_PIN, i);
    tone(BUZZER_PIN, 1000 + i * 2); // Increasing buzzer frequency
    delay(10);
  }
  for (int i = 255; i >= 0; i -= 5) { // Gradually decrease brightness
    analogWrite(RED_LED_PIN, i);
    tone(BUZZER_PIN, 1000 + i * 2);
    delay(10);
  }

  noTone(BUZZER_PIN); // Stop the buzzer
  digitalWrite(GREEN_LED_PIN, LOW); // Turn off Green LED during alarm
}

// Function to Reset the Alarm
void resetAlarm() {
  Serial.println("ALARM RESET!");
  alarmTriggered = false;
  digitalWrite(RED_LED_PIN, LOW);   // Turn off Red LED
  analogWrite(GREEN_LED_PIN, 255);  // Turn on Green LED
  noTone(BUZZER_PIN);               // Stop the buzzer
}
