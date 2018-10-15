#include <Servo.h>
const int rightEngine; //Den analoga pin nummret som den sitter på
const int leftEngine; //Den analoga pin nummret som den sitter på
const int rightBackEngine;
const int leftBackEngine;
const int microServo; //Den analoga pin nummret som den sitter på
const int left; //Vinkeln för servon för att svänga vänster
const int right; //Vinkeln för servon för att svänga höger
const int straight; //Vinkeln för servon för att vara rak
int arrayNumber;
int arrayValue;
int import=0;
int here[2][2000];
int commandNumber=0;
int x=-1;
int y;

Servo servo;
int velocity(int a, int b); //Declarar funktioner som kommer användas sen
int turning(int a, int l, int r, int n);
int Here();

void setup() {
 
  servo.attach (microServo) ;
  pinMode (rightEngine,OUTPUT);
  pinMode (leftEngine, OUTPUT);
  pinMode (rightBackEngine, OUTPUT);
  pinMode (leftBackEngine, OUTPUT);
  Serial.begin(9600);

}

void loop() {
if(x==-1){
if (Serial.available()>0){
  import = Serial.read();
}}
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
  int turning(left,30,0,6);
  }
if(import==6){        //"Sväng höger"
  int turning(right,0,30,5);
  }
if(import==7){}       //Here
int Here;
}  //Loopen är slut





int velocity(int a, int b)
{
  arrayNumber=a;
arrayValue=0;
servo.write(straight);
analogWrite(rightEngine, b);
analogWrite(leftEngine, b);
if(x==-1)
{
while(Serial.available()==0){
delay(1);
arrayValue++;
}
here[0][commandNumber]=arrayNumber;  
here[1][commandNumber]= arrayValue;
commandNumber++;
}
else
{
  delay(y);
  Here();
}
  }

  int turning(int a, int l, int r, int n)
  {
  arrayNumber=n;
  arrayValue=0;
  analogWrite(leftEngine, l);
  analogWrite(rightEngine, r);
  servo.write(a);
   if(x==-1)
   {
    while(Serial.available()==0){
    delay(1);
    arrayValue++;
    }
    here[0][commandNumber]=arrayNumber;
    here[1][commandNumber]=arrayValue;
    commandNumber++;
   }
   else{
    delay(y);
    Here();
    }
    }  

  int Here()
  {
    //Vänd också
    x++;
    import=here[0][x];
    y=here[1][x];
    if(import=0){
      x++;
      analogWrite(rightEngine,0);
      analogWrite(rightBackEngine,0);
      analogWrite(leftEngine,0);
      analogWrite(leftBackEngine,0);
      servo.write(straight);
      
      for(int w=0;w++;w<x){
        here[0][w]=0;
        here[1][w]=0;
      }
      x=-1;
      commandNumber=0;
      Serial.read();
      
      
      }
    
    
    }
