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
#include<Servo.h>
#include<Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
int vcc=11;
int relay_pin=11;
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char password[5];
char On_equip[]= "1234";
char Off_equip[]="4321";
int i=0;
int on=0;
int Buzzer=13;
int RedLed = 12;
Servo dongco;
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
  //pinMode(Buzzer, OUTPUT);
  dongco.attach(10);
  lcd.init();					
  lcd.backlight();
  dongco.write(90);
  delay(1000);
  pinMode(relay_pin, OUTPUT);
  pinMode(vcc, OUTPUT);
}
  
void loop(){
  digitalWrite(relay_pin,HIGH);
  //hiển thị mật khẩu đã nhập
  lcd.setCursor(0,0);
  lcd.print("Nhap Mat Khau: ");
  // for (int j = 0; j < i; j++) {
  //   lcd.setCursor(j,1);
  //   lcd.print(password[j]);
  // }

  //nhập mật khẩu từ keypad
  char customKey = customKeypad.getKey();
  
  if (customKey){
    password[i]=customKey;
    i++;
    on++;
    Serial.println(password);
  }

  //kiểm tra mật khẩu khi đã nhập đủ
  if(on==4){
    if(!strcmp(password, On_equip)){
      digitalWrite(RedLed,LOW);
      i=0;
      Serial.println(" mat khau dung, mo khoa");
      delay(100);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Khoa da duoc mo");
      //tone(Buzzer, 2000,200);
      
      dongco.write(0);
      delay(1000);
      digitalWrite(relay_pin,LOW);
      delay(2000);

    }
    else if(!strcmp(password, Off_equip)){
      digitalWrite(RedLed,LOW);
      Serial.println("khoa cua");
      i=0;
      delay(100);
      lcd.setCursor(1,0);
      lcd.clear();
      lcd.print("Khoa cua");
      delay(1000);
      //tone(Buzzer, 2000,200);
      dongco.write(90);
      delay(1000);
      digitalWrite(relay_pin,HIGH);
      delay(2000);
    }

    else {
      i=0;
      Serial.println("SAI MAT KHAU NHAP LAI.....");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Mat khau sai!");
      digitalWrite(RedLed,HIGH);
      delay(1000);
    }

    // Reset mật khẩu về trạng thái không chứa ký tự nào
    // for (int j = 0; j < 4; j++) {
    //   password[j] = '\0';
    // }

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Nhap Mat Khau: ");
    lcd.setCursor(0,1);
    lcd.print("************");
    on=0;
  }
}
