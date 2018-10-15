#include <Servo.h>
const int rightEngine; //Den analoga pin nummret som den sitter på
const int leftEngine; //Den analoga pin nummret som den sitter på
const int rightBackEngine;
const int leftBackEngine;
const int microServo; //Den analoga pin nummret som den sitter på
const int left; //Vinkeln för servon för att svänga vänster
const int right; //Vinkeln för servon för att svänga höger
const int straight; //Vinkeln för servon för att vara rak
int fileNumber;
int fileValue;
int import=0;

Servo servo;
int velocity(int a, int b); //Declarar funktioner som kommer användas sen
int turning(int a, int l, int r);

void setup() {
 
  servo.attach (microServo) ;
  pinMode (rightEngine,OUTPUT);
  pinMode (leftEngine, OUTPUT);
  pinMode (rightBackEngine, OUTPUT);
  pinMode (leftBackEngine, OUTPUT);
  Serial.begin(9600);

}

void loop() {
if (Serial.available()>0){
  import = Serial.read();
}
if(import==0){ //"Stanna"
analogWrite(rightEngine,0);
analogWrite(leftEngine,0);
servo.write(straight);
}
if(import==1){        //"Växel 1"
  int velocity(1,30);
  }
if(import==2){        //"Växel 2"
  int velocity(2,60);
  }
if(import==3){        //"Växel 3"
  int velocity(3,120);
  }
if(import==4){        //"Växel 4"
  int velocity(4,255);
  }
if(import==5){        //"Sväng vänster"
  int turning(left,30,0);
  }
if(import==6){        //"Sväng höger"
  int turning(right,0,30);
  }
if(import==7){}       //Nästa kommando

}  //Loopen är slut





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

  int turning(int a, int l, int r)
  {
  fileValue=0;
  analogWrite(leftEngine, l);
  analogWrite(rightEngine, r);
  servo.write(a);
    while(Serial.available()==0){
    delay(1);
    fileValue++;
    }
    Serial.write(fileValue);
  }  
