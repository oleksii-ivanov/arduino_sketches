// Laser on Pin 4
const int pinLaser = 2;
const int pinSensor = 4;
int pinStateCurrent = LOW;   // current state of pin
int pinStatePrevious = LOW;  // previous state of pin


void setup() {
  pinMode(pinLaser, OUTPUT);
  pinMode(pinSensor, INPUT);
  Serial.begin(9600);  // Setup serial connection for print out to console

}

// the loop function runs over and over again forever
void loop() {

  pinStatePrevious = pinStateCurrent;            // store old state
  pinStateCurrent = digitalRead(pinSensor);  // read new state

  if (pinStatePrevious == LOW && pinStateCurrent == HIGH) {  // pin state change: LOW -> HIGH
    Serial.println("Motion detected!");
    digitalWrite(pinLaser, HIGH);  // emit red laser

  } else if (pinStatePrevious == HIGH && pinStateCurrent == LOW) {  // pin state change: HIGH -> LOW
    Serial.println("Motion stopped!");
    digitalWrite(pinLaser, LOW);  // emit red laser

  }
}
