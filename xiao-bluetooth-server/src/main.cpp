// #include <Arduino.h>
// #include <BLEDevice.h>
// #include <BLEServer.h>
// #include <BLEUtils.h>
// #include <BLE2902.h>
// #include <stdlib.h>

// BLEServer* pServer = NULL;
// BLECharacteristic* pCharacteristic = NULL;
// bool deviceConnected = false;
// bool oldDeviceConnected = false;
// unsigned long previousMillis = 0;
// const long interval = 1000;

// // TODO: add new global variables for your sensor readings and processed data

// // TODO: Change the UUID to your own (any specific one works, but make sure they're different from others'). You can generate one here: https://www.uuidgenerator.net/
// #define SERVICE_UUID        "2e9820c8-47ee-458a-b576-585f385049a3"
// #define CHARACTERISTIC_UUID "db1065a1-e33f-47db-9a9d-d6541f5e0178"

// class MyServerCallbacks : public BLEServerCallbacks {
//     void onConnect(BLEServer* pServer) {
//         deviceConnected = true;
//     };

//     void onDisconnect(BLEServer* pServer) {
//         deviceConnected = false;
//     }
// };

// // TODO: add DSP algorithm functions here

// void setup() {
//     Serial.begin(115200);
//     Serial.println("Starting BLE work!");

//     // TODO: add codes for handling your sensor setup (pinMode, etc.)

//     // TODO: name your device to avoid conflictions
//     BLEDevice::init("XIAO_ESP32S3");
//     pServer = BLEDevice::createServer();
//     pServer->setCallbacks(new MyServerCallbacks());
//     BLEService *pService = pServer->createService(SERVICE_UUID);
//     pCharacteristic = pService->createCharacteristic(
//         CHARACTERISTIC_UUID,
//         BLECharacteristic::PROPERTY_READ |
//         BLECharacteristic::PROPERTY_WRITE |
//         BLECharacteristic::PROPERTY_NOTIFY
//     );
//     pCharacteristic->addDescriptor(new BLE2902());
//     pCharacteristic->setValue("Hello World");
//     pService->start();
//     // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
//     BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
//     pAdvertising->addServiceUUID(SERVICE_UUID);
//     pAdvertising->setScanResponse(true);
//     pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
//     pAdvertising->setMinPreferred(0x12);
//     BLEDevice::startAdvertising();
//     Serial.println("Characteristic defined! Now you can read it in your phone!");
// }

// void loop() {
//     // TODO: add codes for handling your sensor readings (analogRead, etc.)

//     // TODO: use your defined DSP algorithm to process the readings

    
//     if (deviceConnected) {
//         // Send new readings to database
//         // TODO: change the following code to send your own readings and processed data
//         unsigned long currentMillis = millis();
//         if (currentMillis - previousMillis >= interval) {
//         pCharacteristic->setValue("Hello World");
//         pCharacteristic->notify();
//         Serial.println("Notify value: Hello World");
//         }
//     }
//     // disconnecting
//     if (!deviceConnected && oldDeviceConnected) {
//         delay(500);  // give the bluetooth stack the chance to get things ready
//         pServer->startAdvertising();  // advertise again
//         Serial.println("Start advertising");
//         oldDeviceConnected = deviceConnected;
//     }
//     // connecting
//     if (deviceConnected && !oldDeviceConnected) {
//         // do stuff here on connecting
//         oldDeviceConnected = deviceConnected;
//     }
//     delay(1000);
// }

//---------------------------------------

// #include <Arduino.h>
// #include <BLEDevice.h>
// #include <BLEUtils.h>
// #include <BLEScan.h>
// #include <BLEAdvertisedDevice.h>
// // Client Code
// #include "BLEDevice.h"
// //#include "BLEScan.h"

// // TODO: change the service UUID to the one you are using on the server side.
// // The remote service we wish to connect to.
// static BLEUUID serviceUUID("2e9820c8-47ee-458a-b576-585f385049a3");
// // The characteristic of the remote service we are interested in.
// static BLEUUID    charUUID("db1065a1-e33f-47db-9a9d-d6541f5e0178");

// static boolean doConnect = false;
// static boolean connected = false;
// static boolean doScan = false;
// static BLERemoteCharacteristic* pRemoteCharacteristic;
// static BLEAdvertisedDevice* myDevice;

// // TODO: define new global variables for data collection

// // TODO: define a new function for data aggregation

// static void notifyCallback(
//   BLERemoteCharacteristic* pBLERemoteCharacteristic,
//   uint8_t* pData,
//   size_t length,
//   bool isNotify) {
//     // TODO: add codes to handle the data received from the server, and call the data aggregation function to process the data

//     // TODO: change the following code to customize your own data format for printing
//     Serial.print("Notify callback for characteristic ");
//     Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
//     Serial.print(" of data length ");
//     Serial.println(length);
//     Serial.print("data: ");
//     Serial.write(pData, length);
//     Serial.println();
// }

// class MyClientCallback : public BLEClientCallbacks {
//   void onConnect(BLEClient* pclient) {
//   }

//   void onDisconnect(BLEClient* pclient) {
//     connected = false;
//     Serial.println("onDisconnect");
//   }
// };

// bool connectToServer() {
//     Serial.print("Forming a connection to ");
//     Serial.println(myDevice->getAddress().toString().c_str());

//     BLEClient*  pClient  = BLEDevice::createClient();
//     Serial.println(" - Created client");

//     pClient->setClientCallbacks(new MyClientCallback());

//     // Connect to the remove BLE Server.
//     pClient->connect(myDevice);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
//     Serial.println(" - Connected to server");
//     pClient->setMTU(517); //set client to request maximum MTU from server (default is 23 otherwise)

//     // Obtain a reference to the service we are after in the remote BLE server.
//     BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
//     if (pRemoteService == nullptr) {
//       Serial.print("Failed to find our service UUID: ");
//       Serial.println(serviceUUID.toString().c_str());
//       pClient->disconnect();
//       return false;
//     }
//     Serial.println(" - Found our service");

//     // Obtain a reference to the characteristic in the service of the remote BLE server.
//     pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
//     if (pRemoteCharacteristic == nullptr) {
//       Serial.print("Failed to find our characteristic UUID: ");
//       Serial.println(charUUID.toString().c_str());
//       pClient->disconnect();
//       return false;
//     }
//     Serial.println(" - Found our characteristic");

//     // Read the value of the characteristic.
//     if(pRemoteCharacteristic->canRead()) {
//       std::string value = pRemoteCharacteristic->readValue();
//       Serial.print("The characteristic value was: ");
//       Serial.println(value.c_str());
//     }

//     if(pRemoteCharacteristic->canNotify())
//       pRemoteCharacteristic->registerForNotify(notifyCallback);

//     connected = true;
//     return true;
// }
// /**
//  * Scan for BLE servers and find the first one that advertises the service we are looking for.
//  */
// class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
//   /**
//    * Called for each advertising BLE server.
//    */
//   void onResult(BLEAdvertisedDevice advertisedDevice) {
//     Serial.print("BLE Advertised Device found: ");
//     Serial.println(advertisedDevice.toString().c_str());

//     // We have found a device, let us now see if it contains the service we are looking for.
//     if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {

//       BLEDevice::getScan()->stop();
//       myDevice = new BLEAdvertisedDevice(advertisedDevice);
//       doConnect = true;
//       doScan = true;

//     } // Found our server
//   } // onResult
// }; // MyAdvertisedDeviceCallbacks

// void setup() {
//   Serial.begin(115200);
//   Serial.println("Starting Arduino BLE Client application...");
//   BLEDevice::init("");

//   // Retrieve a Scanner and set the callback we want to use to be informed when we
//   // have detected a new device.  Specify that we want active scanning and start the
//   // scan to run for 5 seconds.
//   BLEScan* pBLEScan = BLEDevice::getScan();
//   pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
//   pBLEScan->setInterval(1349);
//   pBLEScan->setWindow(449);
//   pBLEScan->setActiveScan(true);
//   pBLEScan->start(5, false);
// } // End of setup.

// // This is the Arduino main loop function.
// void loop() {
//   // If the flag "doConnect" is true then we have scanned for and found the desired
//   // BLE Server with which we wish to connect.  Now we connect to it.  Once we are
//   // connected we set the connected flag to be true.
//   if (doConnect == true) {
//     if (connectToServer()) {
//       Serial.println("We are now connected to the BLE Server.");
//     } else {
//       Serial.println("We have failed to connect to the server; there is nothin more we will do.");
//     }
//     doConnect = false;
//   }
 
//   // If we are connected to a peer BLE Server, update the characteristic each time we are reached
//   // with the current time since boot.
//   if (connected) {
//     String newValue = "Time since boot: " + String(millis()/1000);
//     Serial.println("Setting new characteristic value to \"" + newValue  + "\"");

//     // Set the characteristic's value to be the array of bytes that is actually a string.
//     pRemoteCharacteristic->writeValue(newValue.c_str(), newValue.length());
//   }else if(doScan){
//     BLEDevice::getScan()->start(0);  // this is just example to start scan after disconnect, most likely there is better way to do it in arduino
//   }

//   delay(1000); // Delay a second between loops.
// } // End of loop

// --------------------------------------------------------

// #include <Arduino.h>
// #include <BLEDevice.h>
// #include <BLEUtils.h>
// #include <BLEScan.h>
// #include <BLEAdvertisedDevice.h>
// static BLEUUID serviceUUID("2e9820c8-47ee-458a-b576-585f385049a3");
// static BLEUUID charUUID("db1065a1-e33f-47db-9a9d-d6541f5e0178");
// static boolean doConnect = false;
// static boolean connected = false;
// static boolean doScan = false;
// static BLERemoteCharacteristic* pRemoteCharacteristic;
// static BLEAdvertisedDevice* myDevice;
// float maxDistance = 0;
// float minDistance = 1000;
// // **更新最大/最小值**
// void updateMinMax(float value) {
// if (value > maxDistance) maxDistance = value;
// if (value < minDistance) minDistance = value;
// }
// static void notifyCallback(
//     BLERemoteCharacteristic* pBLERemoteCharacteristic,
//     uint8_t* pData,
//     size_t length,
//     bool isNotify) {

//     Serial.print("Received Data: ");
//     Serial.write(pData, length);
//     Serial.println();
//     float receivedDistance = atof((char*)pData);
//     updateMinMax(receivedDistance);
//     Serial.print("Current: ");
//     Serial.print(receivedDistance);
//     Serial.print(" cm | Max: ");
//     Serial.print(maxDistance);
//     Serial.print(" cm | Min: ");
//     Serial.print(minDistance);
//     Serial.println(" cm");
//     }
// class MyClientCallback : public BLEClientCallbacks {
// void onConnect(BLEClient* pclient) {}
// void onDisconnect(BLEClient* pclient) {
// connected = false;
// Serial.println("Disconnected!");
// }
// };
// bool connectToServer() {
// Serial.print("Connecting to ");
// Serial.println(myDevice->getAddress().toString().c_str());

// BLEClient* pClient = BLEDevice::createClient();
// pClient->setClientCallbacks(new MyClientCallback());
// pClient->connect(myDevice);
// Serial.println("Connected to server!");
// BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
// if (pRemoteService == nullptr) {
// Serial.println("Failed to find service");
// pClient->disconnect();
// return false;
// }
// pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
// if (pRemoteCharacteristic == nullptr) {
// Serial.println("Failed to find characteristic");
// pClient->disconnect();
// return false;
// }
// if (pRemoteCharacteristic->canNotify())
// pRemoteCharacteristic->registerForNotify(notifyCallback);
// connected = true;
// return true;
// }
// class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
// void onResult(BLEAdvertisedDevice advertisedDevice) {
// if (advertisedDevice.haveServiceUUID() &&
// advertisedDevice.isAdvertisingService(serviceUUID)) {
// BLEDevice::getScan()->stop();
// myDevice = new BLEAdvertisedDevice(advertisedDevice);
// doConnect = true;
// doScan = true;
// }
// }
// };
// void setup() {
// Serial.begin(115200);
// Serial.println("Starting BLE Client...");
// BLEDevice::init("");
// BLEScan* pBLEScan = BLEDevice::getScan();
// pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
// pBLEScan->setActiveScan(true);
// pBLEScan->start(5, false);
// }
// void loop() {
// if (doConnect) {
// if (connectToServer()) Serial.println("Connected to Server!");
// doConnect = false;
// }
// if (connected) BLEDevice::getScan()->start(0);
// delay(1000);
// }


//----------------------------------------------


// #include <Arduino.h>
// #include <BLEDevice.h>
// #include <BLEUtils.h>
// #include <BLEScan.h>
// #include <BLEAdvertisedDevice.h>
// static BLEUUID serviceUUID("2e9820c8-47ee-458a-b576-585f385049a3");
// static BLEUUID charUUID("db1065a1-e33f-47db-9a9d-d6541f5e0178");
// static boolean doConnect = false;
// static boolean connected = false;
// static boolean doScan = false;
// static BLERemoteCharacteristic* pRemoteCharacteristic;
// static BLEAdvertisedDevice* myDevice;
// float maxDistance = 0;
// float minDistance = 9999;
// void aggregateData(float currentDistance) {
// if (currentDistance > maxDistance) maxDistance = currentDistance;
// if (currentDistance < minDistance) minDistance = currentDistance;
// }
// static void notifyCallback(
// BLERemoteCharacteristic* pBLERemoteCharacteristic,
// uint8_t* pData,
// size_t length,
// bool isNotify) {
// char dataStr[20];
// size_t copyLength = (length < sizeof(dataStr) - 1) ? length : (sizeof(dataStr) - 1);
// memcpy(dataStr, pData, copyLength);
// dataStr[copyLength] = '\0';
// float currentDistance = atof(dataStr);
// aggregateData(currentDistance);
// Serial.print("Received Distance: ");
// Serial.print(currentDistance);
// Serial.print(" cm | Max: ");
// Serial.print(maxDistance);
// Serial.print(" cm | Min: ");
// Serial.println(minDistance);
// }
// class MyClientCallback : public BLEClientCallbacks {
// void onConnect(BLEClient* pclient) {}
// void onDisconnect(BLEClient* pclient) {
// connected = false;
// Serial.println("Disconnected");
// }
// };
// bool connectToServer() {
// Serial.print("Connecting to: ");
// Serial.println(myDevice->getAddress().toString().c_str());
// BLEClient* pClient = BLEDevice::createClient();
// pClient->setClientCallbacks(new MyClientCallback());
// pClient->connect(myDevice);
// pClient->setMTU(517);
// BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
// if (!pRemoteService) {
// Serial.println("Failed to find service!");
// pClient->disconnect();
// return false;
// }
// pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
// if (!pRemoteCharacteristic) {
// Serial.println("Failed to find characteristic!");
// pClient->disconnect();
// return false;
// }
// if (pRemoteCharacteristic->canNotify())
// pRemoteCharacteristic->registerForNotify(notifyCallback);
// connected = true;
// return true;
// }
// class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
// void onResult(BLEAdvertisedDevice advertisedDevice) {
// if (advertisedDevice.haveServiceUUID() &&
// advertisedDevice.isAdvertisingService(serviceUUID)) {
// BLEDevice::getScan()->stop();
// myDevice = new BLEAdvertisedDevice(advertisedDevice);
// doConnect = true;
// doScan = true;
// }
// }
// };
// void setup() {
// Serial.begin(115200);
// BLEDevice::init("");
// BLEScan* pBLEScan = BLEDevice::getScan();
// pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
// pBLEScan->setInterval(1349);
// pBLEScan->setWindow(449);
// pBLEScan->setActiveScan(true);
// pBLEScan->start(5, false);
// }
// void loop() {
// if (doConnect) {
// if (connectToServer()) Serial.println("Connected!");
// else Serial.println("Failed to connect!");
// doConnect = false;
// }
// if (connected) delay(1000);
// else if (doScan) BLEDevice::getScan()->start(0);
// }


//-----------------------------------------------------

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
static BLEUUID serviceUUID("2e9820c8-47ee-458a-b576-585f385049a3");
static BLEUUID charUUID("db1065a1-e33f-47db-9a9d-d6541f5e0178");
static boolean doConnect = false;
static boolean connected = false;
static boolean doScan = false;
static BLERemoteCharacteristic* pRemoteCharacteristic;
static BLEAdvertisedDevice* myDevice;
// **存储最大 & 最小值**
float maxDistance = 0;
float minDistance = 9999;
int dataCount = 0; // **记录接收到的数据次数**
void updateMinMax(float currentDistance) {
if (currentDistance > maxDistance) maxDistance = currentDistance;
if (currentDistance < minDistance) minDistance = currentDistance;
}
// **回调函数：接收 BLE 数据**
static void notifyCallback(
BLERemoteCharacteristic* pBLERemoteCharacteristic,
uint8_t* pData,
size_t length,
bool isNotify) {
char dataStr[20];
size_t copyLength = (length < sizeof(dataStr) - 1) ? length : (sizeof(dataStr) - 1);
memcpy(dataStr, pData, copyLength);
dataStr[copyLength] = '\0';
float currentDistance = atof(dataStr);
updateMinMax(currentDistance);
dataCount++; // **记录数据条数**
// **串口打印当前数据、最大 & 最小值**
Serial.print("[DATA ");
Serial.print(dataCount);
Serial.print("] Received Distance: ");
Serial.print(currentDistance);
Serial.print(" cm | Max: ");
Serial.print(maxDistance);
Serial.print(" cm | Min: ");
Serial.println(minDistance);
if (dataCount >= 10) {
Serial.println("\n Received at least 10 readings! Take screenshot now.\n");
}
}
class MyClientCallback : public BLEClientCallbacks {
void onConnect(BLEClient* pclient) {}
void onDisconnect(BLEClient* pclient) {
connected = false;
Serial.println("Disconnected");
}
};
bool connectToServer() {
Serial.print("Connecting to: ");
Serial.println(myDevice->getAddress().toString().c_str());
BLEClient* pClient = BLEDevice::createClient();
pClient->setClientCallbacks(new MyClientCallback());
pClient->connect(myDevice);
pClient->setMTU(517);
BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
if (!pRemoteService) {
Serial.println("Failed to find service!");
pClient->disconnect();
return false;
}
pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
if (!pRemoteCharacteristic) {
Serial.println("Failed to find characteristic!");
pClient->disconnect();
return false;
}
if (pRemoteCharacteristic->canNotify())
pRemoteCharacteristic->registerForNotify(notifyCallback);
connected = true;
return true;
}
class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
void onResult(BLEAdvertisedDevice advertisedDevice) {
if (advertisedDevice.haveServiceUUID() &&
advertisedDevice.isAdvertisingService(serviceUUID)) {
BLEDevice::getScan()->stop();
myDevice = new BLEAdvertisedDevice(advertisedDevice);
doConnect = true;
doScan = true;
}
}
};
void setup() {
Serial.begin(115200);
BLEDevice::init("");
BLEScan* pBLEScan = BLEDevice::getScan();
pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
pBLEScan->setInterval(1349);
pBLEScan->setWindow(449);
pBLEScan->setActiveScan(true);
pBLEScan->start(5, false);
}
void loop() {
if (doConnect) {
if (connectToServer()) Serial.println("Connected!");
else Serial.println("Failed to connect!");
doConnect = false;
}
if (connected) delay(1000);
else if (doScan) BLEDevice::getScan()->start(0);
}