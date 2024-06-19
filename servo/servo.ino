#include<Servo.h>
int analog = A0;
int position;
Servo dongco;
void setup() {
  // put your setup code here, to run once:
  dongco.attach(3);
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  int giatrianalog= analogRead(analog);
  position = map(giatrianalog,0,1023,0,180);
  dongco.write(position);
  

}
