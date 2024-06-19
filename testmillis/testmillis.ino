int timecho = 1000;
unsigned long hientai=0;
unsigned long thoigian;
 int led=10;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(led, OUTPUT);



}

void loop() {
  // put your main code here, to run repeatedly:
  thoigian=millis();
  Serial.print("Time: "); Serial.println(thoigian);

  if(thoigian - hientai >= timecho){
    hientai=millis();
    Serial.print("    ");
    Serial.print("hientai: "); 
    Serial.print(hientai); Serial.print("  ");
    if(digitalRead(led)== LOW){
      digitalWrite(led,HIGH);
      Serial.println("DEN BAT");
    }
    else{
      digitalWrite(led, LOW);
      Serial.println("DEN OFF");

    }

  }

}
