/* @file CustomKeypad.pde
|| @version 1.0
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Demonstrates changing the keypad size and key values.
|| #
*/
#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char password[5];
char On_equip[]= "1234";
char Off_equip[]="4321";
int i=0;
int on=0;
int Buzzer=10;
int RedLed = 11;
int YellowLed= 12;

//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'0','1','2','3'},
  {'4','5','6','7'},
  {'8','9','A','B'},
  {'C','D','E','F'}
};
byte rowPins[ROWS] = {5, 4, 3, 2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {6, 7, 8, 9}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup(){
  Serial.begin(9600);
  pinMode(RedLed,OUTPUT);
  pinMode(YellowLed, OUTPUT);
  pinMode(Buzzer, OUTPUT);
}
  
void loop(){
  char customKey = customKeypad.getKey();
  
  if (customKey){
    password[i]=customKey;
    tone(Buzzer,20000,1000);
    i++;
    on++;
    Serial.println(password);
  }
  //Serial.println(password);
  if(on==4){
    if(!strcmp(password, On_equip)){
      digitalWrite(RedLed,HIGH);
      digitalWrite(YellowLed, LOW);
      i=0;
      Serial.println("den do da bat, mat khau dung");
      delay(100);
      tone(Buzzer, 5000);
      delay(1000);
      tone(Buzzer,2000,200);
    }

    if(!strcmp(password, Off_equip)){
      digitalWrite(RedLed,LOW);
      digitalWrite(YellowLed, LOW);
      Serial.println("den da tat");
      i=0;
      delay(100);
      tone(Buzzer, 2000,200);
      delay(100);
      tone(Buzzer,2000,200);
    }

    if(strcmp(password, On_equip)){
      if(strcmp(password, Off_equip)){
        i=0;
        Serial.println("SAI MAT KHAU NHAP LAI.....");
        digitalWrite(YellowLed,HIGH);
        delay(300);
        tone(Buzzer,2300,2000);
      }
    }
    on=0;
  }
}
