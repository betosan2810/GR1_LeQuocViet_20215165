// Include the libraries we need
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <RTClib.h>
// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 04

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

/*
 * The setup function. We only start the sensors here
 */

#define i2c_Address 0x3c //initialize with the I2C addr 0x3C Typically eBay OLED's
//#define i2c_Address 0x3d //initialize with the I2C addr 0x3D Typically Adafruit OLED's

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
RTC_DS1307 rtc;
void setup(void)
{
  // start serial port
  Serial.begin(57600);

  delay(250); // Đợi cho màn hình OLED khởi động
  display.begin(i2c_Address, true); // Khởi động màn hình OLED

  display.clearDisplay(); // Xóa màn hình
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  Serial.println("Dallas Temperature IC Control Library Demo");

  // Start up the library
  sensors.begin();
  
}

/*
 * Main function, get and show the temperature
 */
void loop(void)
{ 
  display.clearDisplay(); // Xóa màn hình

  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  // After we got the temperatures, we can print them here.
  // We use the function ByIndex, and as an example get the temperature from the first sensor only.
  float tempC = sensors.getTempCByIndex(0);

  // Check if reading was successful
  if(tempC != DEVICE_DISCONNECTED_C) 
  {
    Serial.print("Temperature for the device 1 (index 0) is: ");
    Serial.println(tempC);
  } 
  else
  {
    Serial.println("Error: Could not read temperature data");
  }

  DateTime now = rtc.now();

  //display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);

  display.setCursor(0,0);
  display.println("Current Temperature:  ");
  display.setTextColor(SH110X_WHITE);
  display.setTextSize(1.5);

  
  // display.setCursor(1,0);
  display.print(tempC);
  display.write(247);
  display.println("C");
  display.setTextSize(1);

  // display.display();
  // display.setCursor(2,0);
  delay(1000);
  display.print(now.year(), DEC);
  display.print('/');
  display.print(now.month(), DEC);
  display.print('/');
  display.print(now.day(), DEC);
  display.print('  ');

  display.print(now.hour(), DEC);
  display.print(':');
  if (now.minute() < 10) {
    display.print('0');
  }
  display.print(now.minute(), DEC);
  display.print(':');
  if (now.second() < 10) {
    display.print('0');
  }
  display.println(now.second(), DEC);

  display.display();


  delay(1000);

}
