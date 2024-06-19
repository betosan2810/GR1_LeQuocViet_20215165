#include<SPI.h>
#include<MFRC522.h>


int PinRST = 9;
int PinSDA = 10;
int UID[4], i;
MFRC522 My_mfRC522(PinSDA, PinRST); //Khai bao doi tuong My_mfRC522


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  SPI.begin();
  My_mfRC522.PCD_Init();




}


void loop() {
  if ( ! My_mfRC522.PICC_IsNewCardPresent()) {
    return;
  }


  // Select one of the cards
  if ( ! My_mfRC522.PICC_ReadCardSerial()) {
    return;
  }


  Serial.println(F("**UID cua the: **"));
  for( byte i=0;i<My_mfRC522.uid.size;i++){
    Serial.print(My_mfRC522.uid.uidByte[i] < 0x10 ? "0" : " ");
    UID[i] = My_mfRC522.uid.uidByte[i];
    Serial.print(UID[i]);


  }
  Serial.println("  ");
    My_mfRC522.PICC_HaltA();//ngung doc the
    My_mfRC522.PCD_StopCrypto1();// ngung su dung doi tuong My_mfRC522


  //-------------------------------------------
  //My_mfRC522.PICC_DumpToSerial(&(My_mfRC522.uid));


  //My_mfRC522.PICC_DumpDetailsToSerial(&(My_mfRC522.uid)); //dump some details about the card


  //mfrc522.PICC_DumpToSerial(&(mfrc522.uid));      //uncomment this to see all blocks in hex
  // put your main code here, to run repeatedly:


}
