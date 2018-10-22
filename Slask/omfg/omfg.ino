#include <Servo.h>
Servo servo;


void setup() {
  pinMode (9, OUTPUT);
  pinMode (4, INPUT);
  servo.attach (6);

}

void loop() {
  if (digitalRead (4) == HIGH) analogWrite (9, 100);
  if (digitalRead (4) == LOW) analogWrite (9, 0);
}
