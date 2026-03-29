
#include <Servo.h>

// -------- PIN DEFINITIONS --------
#define TRIG_TOP 2
#define ECHO_TOP 3

#define TRIG_SIDE1 4   // Entry sensor
#define ECHO_SIDE1 5

#define TRIG_SIDE2 6   // Exit sensor
#define ECHO_SIDE2 7

#define BUZZER 8

Servo conveyorServo;   // MG996R
Servo rejectServo;     // SG90

// -------- CONSTANTS --------
float H_ref = 50.0;          // cm (reference height)
float conveyor_speed = 10.0; // cm/sec

// Thresholds
float H_min = 4, H_max = 10;
float L_min = 10, L_max = 25;

// Variables
unsigned long t_entry = 0, t_exit = 0;
float height = 0, length = 0;

// -------- FUNCTION: FILTERED DISTANCE --------
float getDistance(int trig, int echo) {
  float sum = 0;

  for (int i = 0; i < 5; i++) {
    digitalWrite(trig, LOW);
    delayMicroseconds(2);

    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);

    long duration = pulseIn(echo, HIGH, 30000); // timeout
    float distance = duration / 58.0;

    if (distance > 0) {
      sum += distance;
    }

    delay(10);
  }

  return sum / 5;
}

void setup() {
  Serial.begin(9600);

  pinMode(TRIG_TOP, OUTPUT);
  pinMode(ECHO_TOP, INPUT);

  pinMode(TRIG_SIDE1, OUTPUT);
  pinMode(ECHO_SIDE1, INPUT);

  pinMode(TRIG_SIDE2, OUTPUT);
  pinMode(ECHO_SIDE2, INPUT);

  pinMode(BUZZER, OUTPUT);

  conveyorServo.attach(9);
  rejectServo.attach(10);

  conveyorServo.write(180); // Conveyor ON
  rejectServo.write(0);     // Initial position
}

void loop() {

  // -------- HEIGHT MEASUREMENT --------
  float d_top = getDistance(TRIG_TOP, ECHO_TOP);
  height = H_ref - d_top;

  if (height < 0) height = 0;

  // -------- ENTRY DETECTION --------
  if (getDistance(TRIG_SIDE1, ECHO_SIDE1) < 20) {

    t_entry = millis();

    // -------- WAIT FOR EXIT SENSOR WITH TIMEOUT --------
    unsigned long timeout = millis();

    while (getDistance(TRIG_SIDE2, ECHO_SIDE2) > 20) {
      if (millis() - timeout > 2000) {
        Serial.println("Timeout: No object at exit sensor");
        return;
      }
    }

    t_exit = millis();

    // -------- LENGTH CALCULATION --------
    float time_diff = (t_exit - t_entry) / 1000.0;
    length = conveyor_speed * time_diff;

    Serial.print("Height: "); Serial.println(height);
    Serial.print("Length: "); Serial.println(length);

    // -------- FAULT CHECK --------
    if (height < H_min || height > H_max || length < L_min || length > L_max) {

      Serial.println("FAULT DETECTED");

      // Stop conveyor
      conveyorServo.write(90);

      // Buzzer alert
      digitalWrite(BUZZER, HIGH);
      delay(1000);
      digitalWrite(BUZZER, LOW);

      // Reject object
      rejectServo.write(90);
      delay(1000);
      rejectServo.write(0);

      // Restart conveyor
      conveyorServo.write(180);
    }
    else {
      Serial.println("PRODUCT OK");
    }
  }

  delay(100);
}
