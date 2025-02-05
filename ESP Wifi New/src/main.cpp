// #include <Arduino.h>
// #if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO_W)
// #include <WiFi.h>
// #elif defined(ESP8266)
// #include <ESP8266WiFi.h>
// #endif

// #include <FirebaseClient.h>
// #include <WiFiClientSecure.h>


// #define WIFI_SSID "UW MPSK"
// #define WIFI_PASSWORD "C5x=G>pLMy"

// #define DATABASE_SECRET "AIzaSyCshEgOsdVluhZ60lzpFH0G2B9QBZduGbo"
// #define DATABASE_URL "https://esp32-wifi-hwsf-lab-default-rtdb.firebaseio.com/"

// #define STAGE_INTERVAL 12000 // 12 seconds each stage
// #define MAX_WIFI_RETRIES 100 // Maximum number of WiFi connection retries

// WiFiClientSecure ssl;
// DefaultNetwork network;
// AsyncClientClass client(ssl, getNetwork(network));

// FirebaseApp app;
// RealtimeDatabase Database;
// AsyncResult result;
// LegacyToken dbSecret(DATABASE_SECRET);

// int uploadInterval = 4000; // 1 seconds each upload
// unsigned long sendDataPrevMillis = 0;
// int count = 0;
// // bool signupOK = false;

// // HC-SR04 Pins
// const int trigPin = 3;
// const int echoPin = 2;

// // Define sound speed in cm/usec
// const float soundSpeed = 0.034;

// //functions
// float measureDistance();
// void connectToWiFi();
// void initFirebase();
// void sendDataToFirebase(float distance);

// void printError(int code, const String &msg)
// {
//     Firebase.printf("Error, msg: %s, code: %d\n", msg.c_str(), code);
// }

// void setup(){
//   Serial.begin(115200);
//   delay(500);
//   // while(!Serial){
//   //   yield();
//   // }

//   pinMode(trigPin, OUTPUT);
//   pinMode(echoPin, INPUT);

//   // First, we let the device run for 12 seconds without doing anything
//   Serial.println("Running for 12 seconds without doing anything...");
//   unsigned long startTime = millis();
//   while (millis() - startTime < STAGE_INTERVAL)
//   {
//     delay(100); // Delay between measurements
//   }

//   // Second, we start with the ultrasonic sensor only
//   Serial.println("Measuring distance for 12 seconds...");
//   startTime = millis();
//   while (millis() - startTime < STAGE_INTERVAL)
//   {
//     measureDistance();
//     delay(100); // Delay between measurements
//   }

//   // Now, turn on WiFi and keep measuring
//   Serial.println("Turning on WiFi and measuring for 12 seconds...");
//   connectToWiFi();
//   startTime = millis();
//   while (millis() - startTime < STAGE_INTERVAL)
//   {
//     measureDistance();
//     delay(100); // Delay between measurements
//   }

//   // Now, turn on Firebase and send data every 1 second with distance measurements
//   Serial.println("Turning on Firebase and sending data every 1 second...");
//   initFirebase();
//   startTime = millis();
//   while (millis() - startTime < STAGE_INTERVAL)
//   {
//     float currentDistance = measureDistance();
//     sendDataToFirebase(currentDistance);
//     delay(100); // Delay between measurements
//   }

//   // Go to deep sleep for 12 seconds
//   Serial.println("Going to deep sleep for 12 seconds...");
//   WiFi.disconnect();
//   esp_sleep_enable_timer_wakeup(STAGE_INTERVAL * 1000); // in microseconds
//   esp_deep_sleep_start();
  
// }

// void loop(){
//   // This is not used
// }

// float measureDistance()
// {
//   digitalWrite(trigPin, LOW);
//   delayMicroseconds(2);
//   digitalWrite(trigPin, HIGH);
//   delayMicroseconds(10);
//   digitalWrite(trigPin, LOW);

//   long duration = pulseIn(echoPin, HIGH);
//   float distance = duration * soundSpeed / 2;

//   Serial.print("Distance: ");
//   Serial.print(distance);
//   Serial.println(" cm");
//   return distance;
// }

// void connectToWiFi()
// {
//   // Print the device's MAC address.
//   Serial.println(WiFi.macAddress());
//   WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
//   Serial.println("Connecting to WiFi");
//   int wifiCnt = 0;
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//     wifiCnt++;
//     if (wifiCnt > MAX_WIFI_RETRIES){
//       Serial.println("WiFi connection failed");
//       ESP.restart();
//     }
//   }
//   Serial.println("Connected to WiFi");
//   Serial.print("IP Address: ");
//   Serial.println(WiFi.localIP());
// }

// void initFirebase(){
//   Firebase.printf("Firebase Client v%s\n", FIREBASE_CLIENT_VERSION);

//     ssl.setInsecure();
// #if defined(ESP8266)
//     ssl.setBufferSizes(1024, 1024);
// #endif

//     // Initialize the authentication handler.
//     Serial.println("Initializing the app...");
//     initializeApp(client, app, getAuth(dbSecret));

//     // Binding the authentication handler with your Database class object.
//     app.getApp<RealtimeDatabase>(Database);

//     // Set your database URL (requires only for Realtime Database)
//     Database.url(DATABASE_URL);

//     // In sync functions, we have to set the operating result for the client that works with the function.
//     client.setAsyncResult(result);
// }

// void sendDataToFirebase(float distance){
//   if (millis() - sendDataPrevMillis > uploadInterval || sendDataPrevMillis == 0){
//     sendDataPrevMillis = millis();

//     Serial.print("Pushing the float value... ");
//     String name = Database.push<number_t>(client, "/test/distance", number_t(distance));
//     if (client.lastError().code() == 0){
//         Firebase.printf("ok, name: %s\n", name.c_str());
//         count ++;
//     }
//   }
// }

#include <Arduino.h>
#if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO_W)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

#include <FirebaseClient.h>
#include <WiFiClientSecure.h>

#define WIFI_SSID "UW MPSK"
#define WIFI_PASSWORD "C5x=G>pLMy"

#define DATABASE_SECRET "AIzaSyCshEgOsdVluhZ60lzpFH0G2B9QBZduGbo"
#define DATABASE_URL "https://esp32-wifi-hwsf-lab-default-rtdb.firebaseio.com/"

#define SENSOR_INTERVAL 4000    // Measure sensor data every 4 seconds (0.25Hz)
#define DEEP_SLEEP_INTERVAL 10000 // Deep sleep for 30s if no significant change
#define MAX_WIFI_RETRIES 100  

WiFiClientSecure ssl;
DefaultNetwork network;
AsyncClientClass client(ssl, getNetwork(network));

FirebaseApp app;
RealtimeDatabase Database;
AsyncResult result;
LegacyToken dbSecret(DATABASE_SECRET);

// HC-SR04 Pins
const int trigPin = 3;
const int echoPin = 2;
const float soundSpeed = 0.034; 

// Variables for sensor data storage
float lastDistance = -1;  // Store last measured distance
float threshold = 20.0;  // Only send data if change >20cm
bool wifiConnected = false; // Track WiFi status

// Function declarations
float measureDistance();
void connectToWiFi();
void initFirebase();
void sendDataToFirebase(float distance);
void enterDeepSleep();

void setup()
{
    Serial.begin(115200);
    delay(500);

    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    while (true) 
    {
        Serial.println("Measuring distance...");
        float currentDistance = measureDistance();

        if (lastDistance == -1 || abs(currentDistance - lastDistance) > threshold) 
        {
            Serial.println("Significant change detected, sending data...");

            // Connect to WiFi and Firebase only if needed
            connectToWiFi();
            initFirebase();
            sendDataToFirebase(currentDistance);
            
            // Update last recorded distance
            lastDistance = currentDistance;
        } 
        else 
        {
            Serial.println("No significant change, entering deep sleep...");
        }

        enterDeepSleep();
    }
}

void loop()
{
    // Not used
}

// Function to measure distance
float measureDistance()
{
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH);
    float distance = duration * soundSpeed / 2;

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    return distance;
}

// Function to connect to WiFi only when necessary
void connectToWiFi()
{
    if (!wifiConnected) 
    {
        Serial.println(WiFi.macAddress());
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
        Serial.println("Connecting to WiFi...");
        int wifiCnt = 0;
        while (WiFi.status() != WL_CONNECTED) 
        {
            delay(500);
            Serial.print(".");
            wifiCnt++;
            if (wifiCnt > MAX_WIFI_RETRIES)
            {
                Serial.println("WiFi connection failed, restarting...");
                ESP.restart();
            }
        }
        Serial.println("Connected to WiFi!");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
        wifiConnected = true; // Mark WiFi as connected
    }
}

// Function to initialize Firebase
void initFirebase()
{
    Firebase.printf("Firebase Client v%s\n", FIREBASE_CLIENT_VERSION);
    ssl.setInsecure();

    Serial.println("Initializing Firebase...");
    initializeApp(client, app, getAuth(dbSecret));

    app.getApp<RealtimeDatabase>(Database);
    Database.url(DATABASE_URL);
    client.setAsyncResult(result);
}

// Function to send data to Firebase
void sendDataToFirebase(float distance)
{
    Serial.print("Sending distance data to Firebase... ");
    String name = Database.push<number_t>(client, "/test/distance", number_t(distance));
    if (client.lastError().code() == 0)
    {
        Firebase.printf("Success! Data Name: %s\n", name.c_str());
    }
}

// Function to enter deep sleep mode if no major change
void enterDeepSleep()
{
    Serial.println("Entering deep sleep...");
    WiFi.disconnect();
    wifiConnected = false; // Reset WiFi status
    esp_sleep_enable_timer_wakeup(DEEP_SLEEP_INTERVAL * 1000); // 30s deep sleep
    esp_deep_sleep_start();
}
