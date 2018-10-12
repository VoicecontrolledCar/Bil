#include <Servo.h>
const int Lampa = 11;
const int Knapp = 4;
const int Axel = 6;
bool Riktning = true;
int x=90;
Servo servo;


void setup() {
pinMode (Lampa, OUTPUT);
pinMode (Knapp, INPUT);
servo.attach (Axel);
}

void loop() {
if (digitalRead (Knapp)==HIGH) {
  while (digitalRead (Knapp)==HIGH) {
    servo.write (x);
    if (x>179) Riktning=true;
    if (x<1) Riktning=false;
    if (Riktning==true) x--;
    if (Riktning==false) x++;
    delay (10);
    if (x<5||x>175) digitalWrite (Lampa, HIGH);
    else digitalWrite (Lampa, LOW);
 }
}else {
  servo.write (x);
}
}
