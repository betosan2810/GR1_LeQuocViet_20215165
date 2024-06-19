// C++ code
//
int led_red=8;
int led_green=9;
void setup()
{
  pinMode(led_red, OUTPUT);
  pinMode(led_green, OUTPUT);
}

void loop()
{
  digitalWrite(led_red, HIGH);
  digitalWrite(led_green, LOW);
  delay(1000); // Wait for 1000 millisecond(s)
  digitalWrite(led_red, LOW);
  digitalWrite(led_green, HIGH);
  delay(1000); // Wait for 1000 millisecond(s)
}