int LED = 11;
 int import = 0;
 
 void setup() {
pinMode (LED, OUTPUT);
digitalWrite (LED, LOW);
Serial.begin (9600);
}

void loop() {
if (Serial.available()>0){
  import = Serial.read();

if (import == '0') {
  digitalWrite (LED, LOW);

  }
}else if (import == '1') {
  digitalWrite (LED, HIGH);
}
}
