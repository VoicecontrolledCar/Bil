#include <Servo.h> 
Servo servo;

//Nummret på den analoga pin för de olika komponenterna
const int rightEngine=5; 
const int leftEngine=3; 
const int rightReverse=6; 
const int leftReverse=11; 
const int microServo=9; 

//Vinkeln för servon för de olika riktningarna
const int left=60; 
const int right=120; 
const int straight=90; 

//Variablar deklareras
int executor=0;             //Variabeln vars värde styr bilen i loopen     
int durationWrite;          //Skriver in hur länge ett kommando använts i minnet, tiden ett kommando används
int memory[2][200];         //En array som sparar bilens rörelser
int commandWriteNumber=0;   //Kommer ihåg vilken kolumn som används för att skriva in i minnet
int commandReadNumber=-1;   //Kommer ihåg vilken kolumn som används för att läsa ur minnet
int durationRead=0;         //Läser ur minnet hur länge ett kommando använts i minnet, används för here, tiden ett kommando används

//De olika funktioner och deras datatyper deklararas här
int velocity(int executor, int power);
int turning(int angle, int leftEngineSpeed, int rightEngineSpeed, int executor);
void Here();
void STOP ();
void turnAround ();
void clearMemory ();
void driveTo (bool test);




void setup() {             //Visar för arduinon hur pinsen ska användas och startar bluetoothen
 
  servo.attach (microServo) ;
  pinMode (rightEngine,OUTPUT);
  pinMode (leftEngine, OUTPUT);
  pinMode (rightReverse, OUTPUT);
  pinMode (leftReverse, OUTPUT);
  Serial.begin (9600);
}




void loop() {             //Koden som går runt runt på repeat medan arduinon är igång

//Ett test så att herefunktionen inte håller på innan executorn får bluetoothsignalens värde
if(commandReadNumber==-1){
if (Serial.available()>0){
  executor = Serial.read();
  executor = executor-48;
}}

if(executor==0){        //"Stanna"
  STOP();
}
if(executor==1){        //"Växel 1"
  velocity(1,30);
  }
if(executor==2){        //"Växel 2"
  velocity(2,60);
  }
if(executor==3){        //"Växel 3"
  velocity(3,120);
  }
if(executor==4){        //"Växel 4"
  velocity(4,255);
  }
if(executor==5){        //"Sväng vänster"
  turning(left,30,0,6);
  }
if(executor==6){        //"Sväng höger"
  turning(right,0,30,5);
  }
if(executor==7){       //"Hit"
  Here();
}  
if (executor==8){      //"Vänd"
  turnAround();
}
if (executor==9) {     //"Nollställ minnet"
  clearMemory ();
}
if (executor==-1) {    //"Kör till x"
  driveTo (true);
}
if (executor==-2){     //"åk till y"
  driveTo (false);
}
//Loopen är slut
}




int velocity(int executor, int power)
{
  durationWrite=0;
  servo.write(straight);
  analogWrite(rightEngine, power);
  analogWrite(leftEngine, power);
      if(commandReadNumber==-1)
        {
            while(Serial.available()<=0){  
            delay(1);
            durationWrite++;
}
  memory[0][commandWriteNumber]=executor;  
  memory[1][commandWriteNumber]= durationWrite;
  commandWriteNumber++;
}
else
{
  delay(durationRead);
  Here();
}
  }




  int turning(int angle, int leftEngineSpeed, int rightEngineSpeed, int executor)
  {
  durationWrite=0;
  analogWrite(leftEngine, leftEngineSpeed);
  analogWrite(rightEngine, rightEngineSpeed);
  servo.write(angle);
     if(commandReadNumber==-1)
        {
         while(Serial.available()<=0){    
         delay(1);
          durationWrite++;
         }
    memory[0][commandWriteNumber]=executor;
    memory[1][commandWriteNumber]=durationWrite;
    commandWriteNumber++;
   }
   else{
    delay(durationRead);
    Here();
    }
    }  




    void STOP ()
    {
      analogWrite(rightEngine,0);
      analogWrite(rightReverse,0);
      analogWrite(leftEngine,0);
      analogWrite(leftReverse,0);
      servo.write(straight); 
    }




  void Here()
  {
    commandReadNumber--;       //Gör värdet till -2 ifall det är första gången - annars vänder bilen igen
    if (commandReadNumber==-2) turnAround();
    commandReadNumber=commandReadNumber+2;
    executor=memory[0][commandReadNumber];
    durationRead=memory[1][commandReadNumber];
    
    if(executor==0){                   //När det inte längre finns data att läsa in så stannar bilen
      STOP ();
      clearMemory ();
      Serial.read();  
      }
    }




   void turnAround ()
   {
      STOP();
      servo.write(left);
      analogWrite(rightEngine,70);
      delay (1600);
      analogWrite(rightEngine,0);
      servo.write(right);
      analogWrite(leftReverse,70);
      delay (1600);
      analogWrite(leftReverse,0);
      servo.write(straight); 
      analogWrite(rightEngine,70);
      analogWrite(leftEngine, 70);
      delay (600);
      STOP();
      if (commandReadNumber==-1) {
        memory [0][commandWriteNumber]=executor;
        commandWriteNumber++;
      }
   }



   void clearMemory () {
    for(int w=0;w++;w<200){   
              memory[0][w]=0;
              memory[1][w]=0;
              }
              commandWriteNumber=0;
              commandReadNumber=-1;
   }



   void driveTo (bool test) {
       analogWrite(rightEngine,120);
       analogWrite(leftEngine,120);
       delay (2000);
       if (test==true) {
          servo.write (right);
          analogWrite(rightEngine,80);
          analogWrite(leftEngine,80);
          delay (1500);
          STOP ();
          if (commandReadNumber==-1){
             memory[0][commandWriteNumber]=-2;  
             commandWriteNumber++;
       }}
       if (test==false){
           servo.write (left);
           analogWrite(rightEngine,80);
           analogWrite(leftEngine,80);
           delay (1500);
           STOP ();
           if (commandReadNumber==-1){
              memory[0][commandWriteNumber]=-1;  
              commandWriteNumber++;
           }}
      Serial.read ();
   }


   
