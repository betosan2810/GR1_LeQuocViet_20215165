
#include <RTClib.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#include <SPI.h>
#include <Wire.h>


#define i2c_Address 0x3c //initialize with the I2C addr 0x3C Typically eBay OLED's
//#define i2c_Address 0x3d //initialize with the I2C addr 0x3D Typically Adafruit OLED's

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

RTC_DS1307 rtc;


char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup() {
  Serial.begin(57600);

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

}

void loop() {
  DateTime now = rtc.now();

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);

  display.setCursor(0, 0);
  display.println("Current Date & Time:");
  display.print(now.year(), DEC);
  display.print('/');
  display.print(now.month(), DEC);
  display.print('/');
  display.print(now.day(), DEC);
  display.print(" (");
  display.print(daysOfTheWeek[now.dayOfTheWeek()]);
  display.println(")");
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
