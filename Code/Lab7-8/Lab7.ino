#include <Encoder.h>
Encoder myEnc(2, 3);

int i = 0;
float pos1 = 0, pos2 = 0;
float speed_m=0, rpm = 0;
float e1=0,e2=0,output=0;
int set_point = 1500;
int duty = 0;
float kp = 0.05, ki = 0.005, kd = 0;

ISR(TIMER1_COMPA_vect)          // timer1 Interrupt ทุกๆ 0.01 วินาที
{
  i++;
  if(i == 1){
    pos1 = myEnc.read();
    pos1 = pos1/4000;
    speed_m = (float)(pos1-pos2)/0.01;
    rpm  = speed_m * 60;
    if(rpm < 0){ rpm = -rpm; }
    e1 = (float)set_point - rpm;
  }
  if(i == 2){
    pos2 = myEnc.read();
    pos2 = pos2/4000;
    speed_m = (float)(pos2-pos1)/0.01;
    rpm  = speed_m * 60;
    if(rpm < 0){ rpm = -rpm; }
    e2 = (float)set_point - rpm;
    output = output + ((kp*e1)+(ki*(e1+e2)*0.01)+(kd*(e2-e1)/0.01));
    if(output <= -255) { output = -255; }
    if(output < 0) { output = -output; }
    if(output >= 255) { output = 255; }
    duty = (int)output;
    
    cw();
    analogWrite(11, duty);
    i = 0;
  }
}

void setup()
{
  Serial.begin(9600);
  pinMode(13, OUTPUT);      //IN1
  pinMode(12, OUTPUT);      //IN2
  pinMode(11, OUTPUT);      //EN
  noInterrupts();         
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A = 625;             // compare match register = [ 16,000,000Hz/ (prescaler * desired interrupt frequency) ] - 1
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12);    // 256 prescaler
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
  interrupts();             // enable all interrupts
}

void loop()
{
  Serial.print(set_point); Serial.print(" ");
  Serial.print(rpm); Serial.print(" ");
  Serial.println();
}

void cw(){
  digitalWrite(13, HIGH);
  digitalWrite(12, LOW);
}
void ccw(){
  digitalWrite(13, LOW);
  digitalWrite(12, HIGH);
}
void stop_m(){
  digitalWrite(13, LOW);
  digitalWrite(12, LOW);
}

