void setup() {
  pinMode(13, OUTPUT);      //IN1
  pinMode(12, OUTPUT);      //IN2
  pinMode(11, OUTPUT);      //EN

}

void loop() {
  cw();
  delay(3000);
  ccw();
  delay(3000);
  stop_m();
  delay(3000);

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
