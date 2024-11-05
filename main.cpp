// Landing Gear Servo Control Project
// Author: Devraj Katkoria

// Imports
#include <Servo.h> // Loading the Servo Library
#include <LiquidCrystal.h> // Loading the LCD Display Library
#include <dht.h>

// Constants
int redLED = 9; 
int blueLED = 10;
int servoPin = 11;
int position = 0;
int potentiometer_reading;
int potentiometer_pin = A5;
int echoPin = 7;
int trigPin = 5;
float distance, duration;
float deployed_distance = 20; // Will be determined experimentally (trigger Blue)
float retracted_distance = 4; // Will be determined experimentally (trigger Red)
// If the LG is in the process of being retracted, blue and red will flash together.

Servo LG_Servo; // Servo object
LiquidCrystal lcd(12, 6, 3, 4, 2, 8); // Filled in based on connections of the LCD to the Arduino UNO
dht DHT; 
#define DHT11_PIN 13 // Update with correct PIN FOR DHT 11 Sensor  

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Starting Landing Gear Tracking Simulation ...."); // Serial Monitor Setup 
  pinMode(redLED, OUTPUT); // Initialize Pin 13 (Blue LED to be OUTPUT)
  pinMode(blueLED, OUTPUT); // Initialize Pin 12 (Red LED to be OUTPUT)
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  LG_Servo.attach(servoPin);
  lcd.begin(16, 2);
}

void loop() {
  // put your main code here, to run repeatedly:
  // Potentiometer to control servo
  potentiometer_reading = analogRead(potentiometer_pin);
  position = map(potentiometer_reading, 0, 1023, 0, 90);
  LG_Servo.write(position);
  delayMicroseconds(10);

  // Proximity Sensor to control LEDs
  digitalWrite(trigPin, LOW); // Clear/Reset the trigger pin condition
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); // Command the sensor to send an ultrasonic wave
  delayMicroseconds(10); // Wait 10 microseconds
  digitalWrite(trigPin, LOW); // Command the sensor to stop sending an ultrasonic wave
  duration = pulseIn(echoPin, HIGH); // Measure the duration of the wave
  distance = duration * 0.0344 / 2; // Compute distance the object is away from the sensor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm");

  if (distance <= retracted_distance) {
    digitalWrite(blueLED, HIGH);
    digitalWrite(redLED, LOW);
  } else if (distance > retracted_distance && position > 10) {
    digitalWrite(blueLED, HIGH);
    digitalWrite(redLED, HIGH);
  } else {
    digitalWrite(redLED, HIGH);
    digitalWrite(blueLED, LOW);
  }

  // Humidity Sensor to keep track of conditions in the LG Bay
  int check = DHT.read11(DHT11_PIN);
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(DHT.temperature);
  lcd.print((char)223);
  lcd.print("C");
  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.print(DHT.humidity);
  lcd.print("%");
  delay(1);
}
