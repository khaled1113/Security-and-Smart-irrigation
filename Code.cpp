#include <Keypad.h>
#include <Servo.h>

// Define keypad pins
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {6, 7, 8, 9};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Define servo pin
const int servoPin = 10;

// Define dc motor and transistor pins
const int motorPin = 13;
const int transistorPin = 12;

// Define soil moisture sensor pin
const int moistureSensorPin = A0;

// Define buzzer pin
const int buzzerPin = 11;

// Define variables to store keypad input and moisture sensor value
String inputString = "";
int moistureValue = 0;

// Create servo object
Servo myservo;

void setup() {
  // Setup servo
  myservo.attach(servoPin);

  // Setup dc motor
  pinMode(motorPin, OUTPUT);
  pinMode(transistorPin, OUTPUT);

  // Setup buzzer
  pinMode(buzzerPin, OUTPUT);

  // Setup serial communication
  Serial.begin(9600);
}

void loop() {
  // Read keypad input
  char key = keypad.getKey();

  // If a key was pressed, append it to inputString
  if (key != NO_KEY) {
    inputString += key;
  }

  // If inputString is "1234", open the servo
  if (inputString == "1234") {
    Serial.println("Opening servo...");
     myservo.write(180);
    delay(5000);  // Wait for 5 seconds
    myservo.write(0);  // Close the servo
    delay(500);
    inputString = "";
  }

  // If inputString is not "1234" and has length greater than or equal to 4, sound the buzzer for 5 seconds
  if (inputString.length() >= 4 && inputString != "1234") {
    Serial.println("Wrong password!");
    digitalWrite(buzzerPin, HIGH);
    delay(5000);
    digitalWrite(buzzerPin, LOW);
    inputString = "";
  }

  // Read moisture sensor value
  moistureValue = analogRead(moistureSensorPin);

  // If moisture sensor value is below a certain threshold, turn on dc motor
  if (moistureValue < 500) {
    digitalWrite(transistorPin, HIGH);
    digitalWrite(motorPin, HIGH);
  } else {
    digitalWrite(transistorPin, LOW);
    digitalWrite(motorPin, LOW);
  }

  // Print moisture sensor value to serial monitor
  Serial.print("Moisture sensor value: ");
  Serial.println(moistureValue);
}
