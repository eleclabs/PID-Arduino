#include <Encoder.h>
Encoder myEnc(2, 3);

int pos = 0;

void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);      //IN1
  pinMode(12, OUTPUT);      //IN2
  pinMode(11, OUTPUT);      //EN

}

void loop() {
  pos = myEnc.read();
  cw();
  Serial.println(pos);
}

void cw(){
  digitalWrite(13, HIGH);
  digitalWrite(12, LOW);
  analogWrite(11, 150);
}
void ccw(){
  digitalWrite(13, LOW);
  digitalWrite(12, HIGH);
  analogWrite(11, 150);
}
void stop_m(){
  digitalWrite(13, LOW);
  digitalWrite(12, LOW);
  analogWrite(11, 0);
}
