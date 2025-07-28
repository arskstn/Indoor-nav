#include <BLEDevice.h>
#include <BLEServer.h>
#include <esp_bt_main.h>

void setup() {
  Serial.begin(115200);

  BLEDevice::init("ESP32_PICO_V3_BLE_4");
  BLEServer *pServer = BLEDevice::createServer();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->start();

  BLEAddress bleAddr = BLEDevice::getAddress();
  Serial.print("BLE Address: ");
  Serial.println(bleAddr.toString().c_str());

  esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_DEFAULT, ESP_PWR_LVL_P9);
  Serial.println("BLE Tx Power set to +9 dBm");
}

void loop() {
  delay(1000);
}
/* 
 * Minimal beacon firmware, solely for BLE advertising. Battery voltage advertising removed, wifi connection to server removed.
*/
