
int i = 0;
int val0 = 0,val1 = 0;
float V_0 = 0,V_1 = 0;
float Y0 = 0,Y1 = 0;

ISR(TIMER1_COMPA_vect)          // timer1 Interrupt ทุกๆ 0.01 วินาที
{
  i++;
  if(i == 1){
      val0 = analogRead(A0);
      V_0 = ((float)val0*5)/1023;
      Y0 = V_0 - V_1 + (Y1*0.99);
   }
  if(i == 2){
      val1 = analogRead(A0);
      V_1 = ((float)val1*5)/1023;
      Y1 = V_1 - V_0 + (Y0*0.99);
    if(Y1 <= 0){
         Y1 = 0;
      }
      i = 0;
   }
}

void setup() {
  Serial.begin(9600);
  noInterrupts();         
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  OCR1A = 625;            
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12);    // 256 prescaler
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
  interrupts();             // enable all interrupts

}

void loop() {
  Serial.print(V_0); Serial.print(" ");
  Serial.print(Y1); Serial.print(" ");
  Serial.println();

}
