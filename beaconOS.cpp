#include <sstream>
#include <string>
#include <WiFi.h>
#include <HTTPClient.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

#ifdef __cplusplus
extern "C" {
#endif
  uint8_t temprature_sens_read();
#ifdef __cplusplus
}
#endif

// Wi-Fi настройки (заменяем settings.h)
const char* ssid = "AuroraDebugServer";
const char* password = "12345678"; // Замените на ваш пароль, если он отличается

int beaconScanTime = 5;

typedef struct {
  char address[17];
  int rssi;
} BeaconData;

uint8_t bufferIndex = 0;
BeaconData buffer[50];

const char* serverUrl = "http://192.168.50.1:5000/";
const int beaconId = 1; // Установите ID для каждого маяка: 1, 2 или 3

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
public:
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    extern uint8_t bufferIndex;
    extern BeaconData buffer[];
    if (bufferIndex >= 50) {
      return;
    }
    if (advertisedDevice.haveRSSI()) {
      buffer[bufferIndex].rssi = advertisedDevice.getRSSI();
    } else {
      buffer[bufferIndex].rssi = 0;
    }
    strcpy(buffer[bufferIndex].address, advertisedDevice.getAddress().toString().c_str());
    bufferIndex++;
    Serial.printf("MAC: %s \n", advertisedDevice.getAddress().toString().c_str());
    Serial.printf("name: %s \n", advertisedDevice.getName().c_str());
    Serial.printf("RSSI: %d \n", advertisedDevice.getRSSI());
  }
};

void setup() {
  Serial.begin(115200);
  BLEDevice::init("");
  connectWiFi();
}

void connectWiFi() {
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi..");
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) { // Ограничим количество попыток
    delay(500);
    Serial.print(".");
    attempts++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected to the WiFi network");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nFailed to connect to WiFi. Restarting...");
    ESP.restart(); // Перезагрузка в случае неудачи
  }
}

void ScanBeacons() {
  delay(1000);
  BLEScan* pBLEScan = BLEDevice::getScan();
  MyAdvertisedDeviceCallbacks cb;
  pBLEScan->setAdvertisedDeviceCallbacks(&cb);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(beaconScanTime);
  Serial.print("Devices found: ");
  for (uint8_t i = 0; i < bufferIndex; i++) {
    Serial.print(buffer[i].address);
    Serial.print(" : ");
    Serial.println(buffer[i].rssi);
  }
  pBLEScan->stop();
  delay(1000);
  Serial.println("Scan done!");
}

float readBatteryVoltage() {
  int adcValue = analogRead(34);
  float vOut = (adcValue * 1.1) / 4095.0;
  float vBattery = vOut * (33.0 + 10.0) / 10.0;
  return vBattery;
}

int calculateBatteryPercentage(float voltage) {
  const float vMin = 3.0;
  const float vMax = 4.2;
  if (voltage <= vMin) return 0;
  if (voltage >= vMax) return 100;
  return (int)(((voltage - vMin) / (vMax - vMin)) * 100);
}

void sendDataToServer() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected, attempting to reconnect...");
    connectWiFi();
    return;
  }

  HTTPClient http;
  http.begin(serverUrl);
  http.addHeader("Content-Type", "application/json");

  int wifiRssi = WiFi.RSSI();
  float voltage = readBatteryVoltage();
  int batteryPercentage = calculateBatteryPercentage(voltage);

  String jsonData = "{";
  jsonData += "\"id\":" + String(beaconId) + ",";
  jsonData += "\"station_mac\":\"" + String(WiFi.macAddress()) + "\",";
  jsonData += "\"wifi_rssi\":" + String(wifiRssi) + ",";
  jsonData += "\"battery\":" + String(batteryPercentage) + ",";
  jsonData += "\"voltage\":" + String(voltage, 2) + ",";
  jsonData += "\"devices\":[";
  for (uint8_t i = 0; i < bufferIndex; i++) {
    jsonData += "{\"mac\":\"" + String(buffer[i].address) + "\",\"rssi\":" + String(buffer[i].rssi) + "}";
    if (i < bufferIndex - 1) {
      jsonData += ",";
    }
  }
  jsonData += "]}";

  int httpResponseCode = http.POST(jsonData);
  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println("HTTP Response code: " + String(httpResponseCode));
    Serial.println("Response: " + response);
  } else {
    Serial.print("Error on sending POST: ");
    Serial.println(httpResponseCode);
  }
  http.end();
}

void loop() {
  ScanBeacons();
  sendDataToServer();
  bufferIndex = 0;
  delay(5000);
}