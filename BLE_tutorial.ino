//This will run the tutorial laid out at
// http://www.raywenderlich.com/85900/arduino-tutorial-integrating-bluetooth-le-ios-swift
// Here are some test changes

int LED = 13; //LED pin

void setup() {
  pinMode(LED, OUTPUT);   //Set the LED pin to be an output
  digitalWrite(LED, HIGH); //Starts with the LED on

}

void loop() {
  delay(500);
  digitalWrite(LED, LOW);
  delay(500);
  digitalWrite(LED, HIGH);

}
