#include <Servo.h>
Servo servo;

//Nummret på stiftet för de olika komponenterna
const int rightReverse = 12;
const int leftEngine = 3;
const int rightEngine = 6;
const int leftReverse = 13;
const int microServo = 11;      //Pin 9 och 10 funkar ej med servobibloteket

//Vinkeln för servon för de olika riktningarna, motorhastigheter och andra konstanter
const int left = 45;
const int right = 135;
const int straight = 88;
const int turningMotorP = 250;
const int notTurningMotorP = 0;
const int ST = 700;               //Tiden för bilen att stanna i millisekunder
const int t1 = 1670;              //Sväng-tiden i turnAroundfunktionen
const int t2 = 1318;              //Back-tiden i turnAroundfunktionen

//Globala variablar deklareras
int executor = 0;           //Variabeln vars värde styr bilen i loopen
int durationWrite;          //Skriver in hur länge ett kommando använts i minnet, tiden ett kommando används
int memory [2][200];         //En array som sparar bilens rörelser
int commandWriteNumber = 0; //Kommer ihåg vilken kolumn som används för att skriva in i minnet
int commandReadNumber = -1; //Kommer ihåg vilken kolumn som används för att läsa ur minnet
int durationRead = 0;       //Läser ur minnet hur länge ett kommando använts i minnet, tiden ett kommando används

//De olika funktioner och deras datatyper deklararas här
void velocity (int Executor, int power);
void turning (int angle, int leftForward, int rightForward, bool leftBackward, bool rightBackward, int Executor);
void Here ();
void STOP ();
void turnAround ();
void clearMemory ();
void reverse (int Executor);



void setup() {             //Visar för arduinon hur pinsen ska användas och startar bluetoothen

  servo.attach (microServo) ;
  pinMode (rightEngine, OUTPUT);
  pinMode (leftEngine, OUTPUT);
  pinMode (rightReverse, OUTPUT);
  pinMode (leftReverse, OUTPUT);
  Serial.begin (9600);
  STOP ();
  clearMemory ();
}




void loop() {             //Koden som går runt runt på repeat medan arduinon är igång

  //Ett test så att herefunktionen inte håller på innan executorn får bluetoothsignalens värde
  if (commandReadNumber == -1 ) {
    int recent = executor;
    if (Serial.available() > 0) {
      executor = Serial.read ();
      executor -= 48;       //Korrigerar ett bluetooth-importfel
      if (commandWriteNumber > 0 && recent != 0) { //Om man backar och vill svänga fixar dessa det, kollar också att minnet inte är tomt
        if (memory [0][commandWriteNumber - 1] == 1 && executor == 5) executor = -2;
        if (memory [0][commandWriteNumber - 1] == 1 && executor == 6) executor = -1;
        if (memory [0][commandWriteNumber - 1] == -1 && executor == 5) executor = -2;
        if (memory [0][commandWriteNumber - 1] == -1 && executor == 6) executor = -1;
        if (memory [0][commandWriteNumber - 1] == -2 && executor == 5) executor = -2;
        if (memory [0][commandWriteNumber - 1] == -2 && executor == 6) executor = -1;
      }
    }
  }

  if (executor == 0) {        //"Stanna"
    STOP ();
    delay (200);
  }
  if (executor == 1) {        //"Backa"
    reverse (1);
  }
  if (executor == 2) {        //"Växel 1"
    velocity (2, 200);
  }
  if (executor == 3) {        //"Växel 2"
    velocity (3, 230);
  }
  if (executor == 4) {        //"Växel 3"
    velocity (4, 255);
  }
  if (executor == 5) {        //"Sväng vänster"
    turning (left, notTurningMotorP, turningMotorP, LOW, LOW, 6);
  }
  if (executor == 6) {        //"Sväng höger"
    turning (right, turningMotorP, notTurningMotorP, LOW, LOW, 5);
  }
  if (executor == 7) {       //"Here"
    Here ();
  }
  if (executor == 8) {      //"Vänd"
    turnAround ();
  }
  if (executor == 9) {     //"Nollställ minnet"
    clearMemory ();
  }
  if (executor == -1) {     //Backa och sväng höger (ej i färdriktningen)
    turning (right, 0, 0, HIGH, LOW, -2);
  }
  if (executor == -2) {     //Backa och sväng vänster (ej i färdriktningen)
    turning (left, 0, 0, LOW, HIGH, -1);
  }
}//Loopen är slut



void velocity (int Executor, int power)
{
  STOP ();
  delay (ST);

  durationWrite = 0;
  servo.write (straight);
  analogWrite (rightEngine, power);
  analogWrite (leftEngine, power);
  if (commandReadNumber == -1) {
    while (Serial.available () <= 0) {
      delay (1);
      durationWrite++;
    }
    memory [0][commandWriteNumber] = Executor;
    memory [1][commandWriteNumber] = durationWrite;
    commandWriteNumber++;
  } else {
    delay (durationRead);
    Here ();
  }
}




void turning (int angle, int leftForward, int rightForward, bool leftBackward, bool rightBackward, int Executor)
{
  durationWrite = 0;
  STOP ();
  delay (ST);
  analogWrite (leftEngine, leftForward);
  analogWrite (rightEngine, rightForward);
  digitalWrite (leftReverse, leftBackward);
  digitalWrite (rightReverse, rightBackward);
  servo.write (angle);
  if (commandReadNumber == -1) {
    while (Serial.available () <= 0) {
      delay (1);
      durationWrite++;
    }
    memory [0][commandWriteNumber] = Executor;
    memory [1][commandWriteNumber] = durationWrite;
    commandWriteNumber++;
  } else {
    delay (durationRead);
    Here ();
  }
}




void STOP ()
{
  analogWrite (rightEngine, 0);
  digitalWrite (rightReverse, LOW);
  analogWrite (leftEngine, 0);
  digitalWrite (leftReverse, LOW);
  servo.write (straight);
}




void Here()
{
  commandReadNumber--;       //Gör värdet till -2 ifall det är första gången - annars vänder bilen igen
  if (commandReadNumber == -2) turnAround();
  commandReadNumber += 2;
  executor = memory [0][commandReadNumber];        //Sätter executorn till minnets värde istället för inputens och läser tidens längd
  durationRead = memory [1][commandReadNumber];

  if (Serial.available () >= 0) {                    //Kollar så ingen gjort ett nödstopp.
    int emergency = Serial.read ();
    emergency -= 48;
    if (emergency == 0) {
      executor = 0;
    }
  }

  if (executor == 0) {                   //När det inte längre finns data att läsa in så stannar bilen
    STOP ();
    clearMemory ();
    if (Serial.available () >= 0) Serial.read ();
  }
}




void turnAround ()
{
  STOP ();
  delay (ST);
  servo.write (left);
  delay (200);
  analogWrite (rightEngine, turningMotorP);       //En 90 graders vänstersväng.
  analogWrite (leftEngine, notTurningMotorP);
  delay (t1);
  STOP ();
  delay (ST);
  digitalWrite (rightReverse, HIGH);              //Backar bakåt
  digitalWrite (leftReverse, HIGH);
  delay (t2);
  STOP ();
  delay (ST);
  servo.write (left);
  delay (200);
  analogWrite (rightEngine, turningMotorP);       //En till 90 graders vänstersväng.
  analogWrite (leftEngine, notTurningMotorP);
  delay (t1);
  STOP ();
  delay (ST);

  if (commandReadNumber == -1) {
    memory [0][commandWriteNumber] = executor;
    commandWriteNumber++;
    executor = 0;
  } else {
    Here();  //Så att den hoppar tillbaka till Here efter att ha vänt ifall den var på Here från början(inte verifierad), kan vara därför den fucka upp och fastna i loop
  }
}



void reverse (int Executor) {
  STOP ();
  delay (ST);
  durationWrite = 0;


  digitalWrite (rightReverse, HIGH);
  digitalWrite (leftReverse, HIGH);

  if (commandReadNumber == -1) {
    while (Serial.available () <= 0) {
      digitalWrite (rightReverse, LOW);
      digitalWrite (leftReverse, LOW);
      delay (1);
      digitalWrite (rightReverse, HIGH);
      digitalWrite (leftReverse, HIGH);
      delay(2);
      durationWrite++;
    }
    memory [0][commandWriteNumber] = Executor;
    memory [1][commandWriteNumber] = durationWrite;
    commandWriteNumber++;
  } else {
    for (int y = 0; y >= durationRead; y++) {
      digitalWrite (rightReverse, LOW);
      digitalWrite (leftReverse, LOW);
      delay (1);
      digitalWrite (rightReverse, HIGH);
      digitalWrite (leftReverse, HIGH);
      delay (2);
    }
    Here ();
  }
  STOP ();
}



void clearMemory () {
  for (int x = 0; x++; x < 200) {
    memory [0][x] = 0;
    memory [1][x] = 0;
  }
  commandWriteNumber = 0;
  commandReadNumber = -1;
}
