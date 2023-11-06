#include <Encoder.h>
#include <EEPROMex.h>
#include "Arduino.h"

const int maxAllowedWrites = 80;
const int memBase          = 350;

Encoder myEnc(2, 3);

int i = 0;
float pos1 = 0, pos2 = 0;
float speed_m=0, rpm = 0;
float e1=0,e2=0,output=0;
int set_point = 1500;
int duty = 0;
float kp = 0.05, ki = 0.005, kd = 0;
String data, skp, ski, skd, ssp;
char k1[10], k2[10], k3[10];

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
  EEPROM.setMemPool(memBase, EEPROMSizeUno);
  EEPROM.setMaxAllowedWrites(maxAllowedWrites);
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

  kp = EEPROM.readFloat(0);
  ki = EEPROM.readFloat(4);
  kd = EEPROM.readFloat(8);
  set_point = EEPROM.readInt(12);
}

void loop()
{
  if(Serial.available() > 0) {
     data = Serial.readStringUntil('\r');  
     skp = getValue(data, ' ', 0);
     ski = getValue(data, ' ', 1);
     skd = getValue(data, ' ', 2);
     ssp = getValue(data, ' ', 3);
     
     skp.toCharArray(k1, 10);
     ski.toCharArray(k2, 10);
     skd.toCharArray(k3, 10);
     kp = atof(k1);
     ki = atof(k2);
     kd = atof(k3);
     set_point = ssp.toInt();

     EEPROM.writeFloat(0,kp);
     EEPROM.writeFloat(4,ki);
     EEPROM.writeFloat(8,kd);
     EEPROM.writeInt(12,set_point);
  }
  
  Serial.print(kp,3); Serial.print(" ");
  Serial.print(ki,3); Serial.print(" ");
  Serial.print(kd,3); Serial.print(" ");
  Serial.print(set_point); Serial.print(" ");
  Serial.print(rpm); Serial.print(" ");
  Serial.println();
}
///////////////////ฟังก์ชั่นการแยกข้อมูุล String //////////////////
String getValue(String data, char separator, int index)
{
   int found = 0;
  int strIndex[] = {0, -1  };
    int maxIndex = data.length()-1;
    for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
    found++;
    strIndex[0] = strIndex[1]+1;
    strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }
    return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
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

