#include <Arduino.h>

// INcludes so dependencies work
#include <SPIFFS.h>
#include <ETH.h>
#include <Wire.h>
#include <SPI.h>

#define VERSION 220822 01
#define MQTT_KEEPALIVE 60

// hostname / DNS name for web
#define HOST_NAME "PIUPS"

/* --------------- INFRA --------------------- */

// support for local credential file
#define MYWIFI_ENABLE_LOCAL_CREDS
#include <MyWifiNetwork.h>
MyWifiNetwork network(nullptr);

#include <BDInfra.h>
BDInfra infra;


/* MQTT connection */
#include <PubSubClient.h>
WiFiClient wifiClient;
PubSubClient pubSubClient(wifiClient);

#include "interfaces/MQTTIntf.h"
MQTTIntf mQTTIntf = MQTTIntf(&pubSubClient);

#include <Adafruit_INA219.h>
Adafruit_INA219 ina219;

/*
 * call back to get messages from network
 */
void handleNetworkMessage(MyNetwork::NetMessage netMessage, const char *msg)
{
  ILogger::log(l_info, "%s:%d", msg, (int)netMessage);
}

void setup() {
    Serial.begin(115200);
  infra.enableSerialLogging(&Serial);
  infra.enableTelnetLogging();

  if (!SPIFFS.begin())
  {
    SPIFFS.format();
    SPIFFS.begin();
  }




  ILogger::setLevel(l_diagnostics);

    if (! ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
    while (1) { delay(10); }
  }

  BDLanguage::addLanguage("en", "English", "/web/en/usertext.txt", nullptr); // no system file for english

  // ILogger::log(l_info, "Gateway starting, data on pin %d", RXPIN);

  bool safeModeActive = false; // hallSensor.sensing();

  network.setMessageCallback(handleNetworkMessage);

  infra.begin(network, HOST_NAME, safeModeActive);

  // GATEWAY_SERIAL_PORT.begin(115200, SERIAL_8N1, RXPIN, TXPIN);

  // decoder.begin();

  mQTTIntf.begin();
}

void saveAll()
{
}

void checkIfRestartNeeded()
{
  if (BDWebServer::restartNeeded())
  {
    ILogger::log(l_warning, "Rebooting...");

#ifdef ARDUINO_ESP32_GATEWAY
    digitalWrite(5, LOW);
    delay(500);
#endif
    saveAll();
    delay(100);
    ESP.restart();
  }
}

time_t nextMQTTcheck = 0;
void checkMQTT()
{
  if (TZManager::now() > nextMQTTcheck)
  {
    // mQTTIntf.loop();
    network.checkConnection();
    nextMQTTcheck = TZManager::now()+30;
  }
}

void readPower()
{
    float shuntvoltage = 0;
  float busvoltage = 0;
  float current_mA = 0;
  float loadvoltage = 0;
  float power_mW = 0;

  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  power_mW = ina219.getPower_mW();
  loadvoltage = busvoltage + (shuntvoltage / 1000);
  
  Serial.print("Bus Voltage:   "); Serial.print(busvoltage); Serial.println(" V");
  Serial.print("Shunt Voltage: "); Serial.print(shuntvoltage); Serial.println(" mV");
  Serial.print("Load Voltage:  "); Serial.print(loadvoltage); Serial.println(" V");
  Serial.print("Current:       "); Serial.print(current_mA); Serial.println(" mA");
  Serial.print("Power:         "); Serial.print(power_mW); Serial.println(" mW");
  Serial.println("");

  delay(2000);
}


void loop()
{
  // decoder.loop();
  delay(100);
  checkIfRestartNeeded();
  checkMQTT();
  // myFan.loop();
}