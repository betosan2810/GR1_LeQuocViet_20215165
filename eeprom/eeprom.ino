#include <EEPROM.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <RTClib.h>

#define ONE_WIRE_BUS 4 // Data wire is plugged into port 4 on the ESP32

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

#define i2c_Address 0x3c //initialize with the I2C addr 0x3C Typically eBay OLED's
//#define i2c_Address 0x3d //initialize with the I2C addr 0x3D Typically Adafruit OLED's

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

RTC_DS1307 rtc;

#define EEPROM_SIZE 512 // Define the size of EEPROM (in bytes)
#define RECORD_SIZE 14  // Define the size of each record (temperature + time)

bool operationStarted = false;

void setup(void) {
  Serial.begin(57600);

  delay(250); // Wait for the OLED display to initialize
  display.begin(i2c_Address, true); // Initialize the OLED display
  display.clearDisplay(); // Clear the display

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // Start up the Dallas Temperature library
  sensors.begin();
}

void loop(void) {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    if (command == "START") {
      startOperation();
    } else if (command == "STOP") {
      stopOperation();
    } else if (command == "GETMIN") {
      getMinTemperature();
    } else if (command == "GETMAX") {
      getMaxTemperature();
    }
  }

  if (operationStarted) {
    display.clearDisplay(); // Clear the display

    // Request temperature from Dallas Temperature sensor
    sensors.requestTemperatures();
    float tempC = sensors.getTempCByIndex(0); // Get temperature in Celsius

    if (tempC != DEVICE_DISCONNECTED_C) {
      // Get current date and time from RTC
      DateTime now = rtc.now();
      
      // Display temperature on OLED
      display.setTextSize(1);
      display.setTextColor(SH110X_WHITE);
      display.setCursor(0,0);
      display.println("Current Temperature:");
      display.setTextColor(SH110X_WHITE);
      display.setTextSize(1.5);
      display.print(tempC);
      display.write(247); // Display the degree Celsius symbol
      display.println("C");
      display.setTextSize(1);
      display.print(now.year(), DEC);
      display.print('/');
      display.print(now.month(), DEC);
      display.print('/');
      display.print(now.day(), DEC);
      display.print(' ');
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
      
      // Save temperature and time to EEPROM
      saveRecord(tempC, now);
      
      // Send temperature data to PC over serial
      Serial.print("Temp:");
      Serial.print(tempC);
      Serial.print("|Date:");
      Serial.print(now.year());
      Serial.print("/");
      Serial.print(now.month());
      Serial.print("/");
      Serial.print(now.day());
      Serial.print("|Time:");
      Serial.print(now.hour());
      Serial.print(":");
      if (now.minute() < 10) {
        Serial.print('0');
      }
      Serial.print(now.minute());
      Serial.print(":");
      if (now.second() < 10) {
        Serial.print('0');
      }
      Serial.println(now.second());
    } else {
      Serial.println("Error: Could not read temperature data");
    }

    delay(5000); // Wait 5 seconds
  }
}

void startOperation() {
  operationStarted = true;
}

void stopOperation() {
  operationStarted = false;
}

void saveRecord(float temperature, DateTime time) {
  // Calculate the address to save the record in EEPROM
  int address = findNextAvailableRecord();

  // Check if EEPROM is full
  // if (address == -1) {
  //   Serial.println("EEPROM is full!");
  //   return;
  // }

  // Save temperature to EEPROM
  EEPROM.put(address, temperature);
  // Save time to EEPROM
  EEPROM.put(address + sizeof(float), time);

  // Commit the changes to EEPROM
  EEPROM.commit();
}

int findNextAvailableRecord() {
  for (int i = 0; i < EEPROM_SIZE; i += RECORD_SIZE) {
    float temperature;
    EEPROM.get(i, temperature);
    if (temperature == 0.0) {
      return i; // Return the address of the next available record
    }
  }
  return -1; // Return -1 if EEPROM is full
}

void getMinTemperature() {
  float minTemp = 100.0; // Initialize minTemp with a high value
  DateTime minTime;

  for (int i = 0; i < EEPROM_SIZE; i += RECORD_SIZE) {
    float temperature;
    DateTime time;
    EEPROM.get(i, temperature);
    EEPROM.get(i + sizeof(float), time);

    // Ignore default temperature values and update minTemp if current temperature is smaller
    if (temperature != 0.0 && temperature < minTemp) {
      minTemp = temperature;
      minTime = time;
    }
  }

  // Send minTemp and minTime over serial
  Serial.print("MinTemp:");
  Serial.print(minTemp);
  Serial.print("|Date:");
  Serial.print(minTime.year());
  Serial.print("/");
  Serial.print(minTime.month());
  Serial.print("/");
  Serial.print(minTime.day());
  Serial.print("|Time:");
  Serial.print(minTime.hour());
  Serial.print(":");
  if (minTime.minute() < 10) {
    Serial.print('0');
  }
  Serial.print(minTime.minute());
  Serial.print(":");
  if (minTime.second() < 10) {
    Serial.print('0');
  }
  Serial.println(minTime.second());
}

void getMaxTemperature() {
  float maxTemp = -100.0; // Initialize maxTemp with a low value
  DateTime maxTime;

  for (int i = 0; i < EEPROM_SIZE; i += RECORD_SIZE) {
    float temperature;
    DateTime time;
    EEPROM.get(i, temperature);
    EEPROM.get(i + sizeof(float), time);

    // Ignore default temperature values and update maxTemp if current temperature is larger
    if (temperature != 0.0 && temperature > maxTemp) {
      maxTemp = temperature;
      maxTime = time;
    }
  }

  // Send maxTemp and maxTime over serial
  Serial.print("MaxTemp:");
  Serial.print(maxTemp);
  Serial.print("|Date:");
  Serial.print(maxTime.year());
  Serial.print("/");
  Serial.print(maxTime.month());
  Serial.print("/");
  Serial.print(maxTime.day());
  Serial.print("|Time:");
  Serial.print(maxTime.hour());
  Serial.print(":");
  if (maxTime.minute() < 10) {
    Serial.print('0');
  }
  Serial.print(maxTime.minute());
  Serial.print(":");
  if (maxTime.second() < 10) {
    Serial.print('0');
  }
  Serial.println(maxTime.second());
}
