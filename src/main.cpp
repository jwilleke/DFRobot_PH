/*!
 * @file DFRobot_PH_Test.h
 * @brief This is the sample code for Gravity: Analog pH Sensor / Meter Kit V2, SKU:SEN0161-V2.
 * @n In order to guarantee precision, a temperature sensor such as DS18B20 is needed, to execute automatic temperature compensation.
 * @n You can send commands in the serial monitor to execute the calibration.
 * @n Serial Commands:
 * @n    enterph -> enter the calibration mode ENTERPH
 * @n    calph   -> calibrate with the standard buffer solution, two buffer solutions(4.0 and 7.0) will be automaticlly recognized CALPH
 * @n    exitph  -> save the calibrated parameters and exit from calibration mode EXITPH
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [Jiawei Zhang](jiawei.zhang@dfrobot.com)
 * @version  V1.0
 * @date  2018-11-06
 * @url https://github.com/DFRobot/DFRobot_PH
 */

#include <Arduino.h>
#include "arduino_secrets.h"
#include "DFRobot_PH.h"
#include <EEPROM.h>
#include <WiFiS3.h>    // works with the Ardiuno R4 WiFi
#include <WiFiUdp.h>   // works with the Ardiuno R4 WiFi
#include "RTC.h"       // works with the Ardiuno R4 WiFi
#include <NTPClient.h> //Include the NTP library

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID; // your network SSID (name)
char pass[] = SECRET_PASS; // your network password (use for WPA, or use as key for WEP)

int wifiStatus = WL_IDLE_STATUS;
WiFiUDP Udp; // A UDP instance to let us send and receive packets over UDP
NTPClient timeClient(Udp);

RTCTime currentTime;

void printWifiStatus()
{
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void connectToWiFi()
{
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE)
  {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true)
      ;
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION)
  {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to WiFi network:
  while (wifiStatus != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    wifiStatus = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
}
  void printDigits(int digits)
  {
    // utility function for digital clock display: prints preceding colon and leading 0
    Serial.print(":");
    if (digits < 10)
      Serial.print('0');
    Serial.print(digits);
  }

#define PH_PIN A1
#define TIME_HEADER "T" // Header tag for serial time sync message

  float voltage, phValue, temperature = 25; // assumes temperature is 25 C if not using temperature sensor
  DFRobot_PH ph;

  void consolePrintData()
  {
    RTC.getTime(currentTime);
    // Serial.print(currentTime.getYear());
    // Serial.print("-");
    // Serial.print(Month2int(currentTime.getMonth()));
    // Serial.print("-");
    // Serial.print(currentTime.getDayOfMonth());
    Serial.print("T");
    Serial.print(currentTime.getHour());
    Serial.print(":");
    Serial.print(currentTime.getMinutes());
    Serial.print(":");
    Serial.print(currentTime.getSeconds());
  }

  void setup()
  {
    Serial.begin(115200);
    RTC.begin();
    ph.begin();
  }

  void loop()
  {
    static unsigned long timepoint = millis();
    if (millis() - timepoint > 1000U)
    { // time interval: 1s
      timepoint = millis();
      consolePrintData();
      Serial.print(", ");
      // temperature = readTemperature();         // read your temperature sensor to execute temperature compensation
      voltage = analogRead(PH_PIN) / 1024.0 * 5000; // read the voltage
      phValue = ph.readPH(voltage, temperature);    // convert voltage to pH with temperature compensation
      Serial.print("temperature:");
      Serial.print(temperature, 1);
      Serial.print("^C  pH:");
      Serial.println(phValue, 2);
    }
    ph.calibration(voltage, temperature); // calibration process by Serail CMD
  }

  float readTemperature()
  {
    // add your code here to get the temperature from your temperature sensor
  }
