#include <Servo.h>

int maxPeople = 5;
int sensitivity = 5;

int currentPeople = 0;

int sensor1[] = {4, 5};
int sensor2[] = {6, 7};
int sensor1Initial;
int sensor2Initial;

String sequence = "";

int timeoutCounter = 0;

Servo myServo; // Declare a Servo object

// Function declaration
int measureDistance(int a[]);

void setup() {
  Serial.begin(9600);

  delay(500);
  sensor1Initial = measureDistance(sensor1);
  sensor2Initial = measureDistance(sensor2);

  myServo.attach(9); // Attach the servo to pin 9
}

void loop() {
  int sensor1Val = measureDistance(sensor1);
  int sensor2Val = measureDistance(sensor2);

  if (sensor1Val < sensor1Initial - 30 && sequence.charAt(0) != '1') {
    sequence += "1";
  } else if (sensor2Val < sensor2Initial - 30 && sequence.charAt(0) != '2') {
    sequence += "2";
  }

  if (sequence.equals("12")) {
    currentPeople++;
    sequence = "";
    delay(550);
  } else if (sequence.equals("21") && currentPeople > 0) {
    currentPeople--;
    sequence = "";
    delay(550);
  }

  if (sequence.length() > 2 || sequence.equals("11") || sequence.equals("22") || timeoutCounter > 200) {
    sequence = "";
  }

  if (sequence.length() == 1) {
    timeoutCounter++;
  } else {
    timeoutCounter = 0;
  }

  // Control the servo based on the currentPeople count
  if (currentPeople == 0) {
    myServo.write(90); // Turn 90 degrees
  } else if (currentPeople == 1) {
    myServo.write(0); // Turn back 90 degrees
  }

  // Print values to serial
  Serial.print("Current People Count: ");
  Serial.println(currentPeople);
}

int measureDistance(int a[]) {
  pinMode(a[1], OUTPUT);
  digitalWrite(a[1], LOW);
  delayMicroseconds(2);
  digitalWrite(a[1], HIGH);
  delayMicroseconds(10);
  digitalWrite(a[1], LOW);
  pinMode(a[0], INPUT);
  long duration = pulseIn(a[0], HIGH, 100000);
  return duration / 29 / 2;
}