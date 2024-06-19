// C++ code
//
int button=2;
int led=3;
void setup()
{
  pinMode(button, INPUT_PULLUP);
  pinMode(led,OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  int trangthai=digitalRead(button);
  if(trangthai==0){
    digitalWrite(led,HIGH);
  }
  else digitalWrite(led,LOW);	
}