// code by Gagandeepkaur Saluja
// Youtube Channel: VoltiqueX - https://www.youtube.com/@VoltiqueX

#include <Servo.h>

const int SERVO_PIN       = 9;   // Servo signal
const int TRIG_PIN        = 5;   // HC-SR04 TRIG
const int ECHO_PIN        = 6;   // HC-SR04 ECHO

const int LID_OPEN_ANGLE  = 100; // Adjust per your mechanism (0–180)
const int LID_CLOSED_ANGLE= 0;

const int OPEN_AT_CM      = 25;  // Open when hand is closer than this
const int CLOSE_AT_CM     = 30;  // Close when distance is beyond this (hysteresis)

const unsigned long CLOSE_AFTER_MS = 1500; // Time with no hand before closing

// Distance sampling
const int SAMPLES = 5;                   // median of N samples
const unsigned long PULSE_TIMEOUT_US = 25000; // 25ms timeout for pulseIn

// Optional: slow, smooth lid motion
const int SERVO_STEP_DEG = 3;            // degrees per step when moving
const int SERVO_STEP_DELAY_MS = 8;       // ms between steps
//
// ======================

Servo lid;

enum LidState { LID_CLOSED, LID_OPEN };
LidState state = LID_CLOSED;

unsigned long lastSeenCloseMs = 0;  // last time a "hand is near" was detected

// Simple swap for median sort
void iswap(int &a, int &b) { int t = a; a = b; b = t; }

// Get one distance reading (cm). Returns 999 on timeout.
int readDistanceOnceCM() {
  // Ensure clean trigger
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  unsigned long duration = pulseIn(ECHO_PIN, HIGH, PULSE_TIMEOUT_US);
  if (duration == 0) return 999; // timeout/no echo
  // Speed of sound ~343 m/s => 0.0343 cm/us. Divide by 2 (go & return)
  float cm = (duration * 0.0343f) / 2.0f;
  // Constrain to reasonable range
  if (cm < 0) cm = 0;
  if (cm > 400) cm = 400;
  return (int)(cm + 0.5f); // round
}

// Median of N simple reads to stabilize
int readDistanceMedianCM() {
  int a[SAMPLES];
  for (int i = 0; i < SAMPLES; i++) {
    a[i] = readDistanceOnceCM();
    delay(5); // tiny spacing between pings
  }
  // sort small array (simple bubble sort)
  for (int i = 0; i < SAMPLES - 1; i++) {
    for (int j = 0; j < SAMPLES - i - 1; j++) {
      if (a[j] > a[j+1]) iswap(a[j], a[j+1]);
    }
  }
  return a[SAMPLES / 2];
}

void moveServoSmooth(int fromAngle, int toAngle) {
  if (fromAngle == toAngle) {
    lid.write(toAngle);
    return;
  }
  int step = (toAngle > fromAngle) ? SERVO_STEP_DEG : -SERVO_STEP_DEG;
  for (int pos = fromAngle; (step > 0 ? pos <= toAngle : pos >= toAngle); pos += step) {
    lid.write(pos);
    delay(SERVO_STEP_DELAY_MS);
  }
  lid.write(toAngle);
}

void openLid() {
  moveServoSmooth(LID_CLOSED_ANGLE, LID_OPEN_ANGLE);
  state = LID_OPEN;
}

void closeLid() {
  moveServoSmooth(LID_OPEN_ANGLE, LID_CLOSED_ANGLE);
  state = LID_CLOSED;
}

void setup() {
  Serial.begin(9600);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  lid.attach(SERVO_PIN);
  lid.write(LID_CLOSED_ANGLE);
  delay(300);

  // initialize "last seen near" so it doesn't instantly close
  lastSeenCloseMs = millis();
  Serial.println(F("Smart Dustbin Ready"));
}

void loop() {
  int dist = readDistanceMedianCM();
  unsigned long now = millis();

  // Consider "hand present" if within OPEN_AT_CM
  bool handNear = (dist <= OPEN_AT_CM);

  if (handNear) {
    lastSeenCloseMs = now;
  }

  Serial.print(F("Distance: "));
  Serial.print(dist);
  Serial.print(F(" cm | State: "));
  Serial.println(state == LID_OPEN ? F("OPEN") : F("CLOSED"));

  switch (state) {
    case LID_CLOSED:
      // Open if hand near
      if (handNear) {
        openLid();
      }
      break;

    case LID_OPEN:
      // Close only if:
      // 1) it's been a while since we last saw a hand, AND
      // 2) distance is clearly beyond the close threshold (hysteresis)
      if ((now - lastSeenCloseMs >= CLOSE_AFTER_MS) && (dist >= CLOSE_AT_CM)) {
        closeLid();
      }
      break;
  }

  // Loop pace (adjust as needed)
  delay(40);
}
