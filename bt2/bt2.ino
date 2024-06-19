// C++ code
//
int led_red=8;
int led_green=9;
int led_yellow=10;
void setup()
{
  pinMode(led_red, OUTPUT);
  pinMode(led_green, OUTPUT);
  pinMode(led_yellow, OUTPUT);
}

void loop()
{
  digitalWrite(led_red, HIGH);
  digitalWrite(led_green, HIGH);
  digitalWrite(led_yellow, HIGH);
  delay(1000); // Wait for 1000 millisecond(s)
  digitalWrite(led_red, LOW);
  delay(1000);
  digitalWrite(led_green, LOW);
  delay(1000);
  digitalWrite(led_yellow, LOW);
  delay(1000); // Wait for 1000 millisecond(s)
}