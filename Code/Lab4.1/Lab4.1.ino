#include <Encoder.h>
Encoder myEnc(2, 3);

int i = 0;
float pos1 = 0, pos2 = 0;
float speed_m=0, rpm = 0;

ISR(TIMER1_COMPA_vect)          // timer1 Interrupt ทุกๆ 0.01 วินาที
{
  i++;
  if(i == 1){
    pos1 = myEnc.read();
    pos1 = pos1/4000;
  }
  if(i == 2){
    pos2 = myEnc.read();
    pos2 = pos2/4000;
    speed_m = (float)(pos2-pos1)/0.1;
    i = 0;
  }
}

void setup()
{
  Serial.begin(9600);
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
  Serial.print(pos1); Serial.print(" ");
  Serial.print(speed_m); Serial.print(" ");
  Serial.println();
}
