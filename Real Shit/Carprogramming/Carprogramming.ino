#include <Servo.h>
int rightEngine;
int leftEngine;
int microServo;
int left;
int right;
int straight;
int fileNumber;
int fileValue;
int import=0;

Servo servo;
int velocity(int a, int b);
int turning(int a, int b);

void setup() {
 
  servo.attach (microServo) ;
  pinMode (rightEngine,OUTPUT);
  pinMode (leftEngine, OUTPUT);
  Serial.begin(9600);

}

void loop() {
if (Serial.available()>0){
  import = Serial.read();
}
if(import=0){
analogWrite(rightEngine,0);
analogWrite(leftEngine,0);
servo.write(straight);
}
if(import=1){
  int velocity(1,30);
  }
if(import=2){
  int velocity(2,60);
  }
if(import=3){
  int velocity(3,120);
  }
if(import=4){
  int velocity(4,255);
  }
if(import=5){
  int turning(left);
  }
if(import=6){
  int turning(right);
  }
if(import=7){}

}  //loopen är slut





int velocity(int a, int b)
{
  fileNumber=a;
fileValue=0;
servo.write(straight);
analogWrite(rightEngine, b);
analogWrite(leftEngine, b);
Serial.write(fileNumber);
while(Serial.available()==0){
delay(1);
fileValue++;
}
Serial.write(fileValue);  
  }

  int turning(int a){
  fileValue=0;
  analogWrite(leftEngine, 0);
  analogWrite(rightEngine, 30);
  servo.write(a);
    while(Serial.available()==0){
    delay(1);
    fileValue++;
    }
    Serial.write(fileValue);
  }  
