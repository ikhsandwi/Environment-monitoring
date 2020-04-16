#include <DNSServer.h>                                                 // DNS Server Library (https://github.com/zhouhan0126/DNSServer---esp32)
#include <WebServer.h>                                                 // Web Server Library (https://github.com/zhouhan0126/WebServer-esp32)
#include <WiFiManager.h>                                               // WiFi Manager Library (https://github.com/Brunez3BD/WIFIMANAGER-ESP32)
#include <ESP32Ping.h>
#include <WiFi.h>
#include <AntaresESP32HTTP.h>


#define ACCESSKEY "a08c0faa1b988b11:9ae97d4dee93a723"
//#define WIFISSID "zero"
//#define PASSWORD "anakrantau"

#define projectName "EMS-JGR"
#define deviceName "test"

AntaresESP32HTTP antares(ACCESSKEY);
// DeepSleep Parameter
#define uS_TO_S_FACTOR 1000000                                         // Conversion factor for micro seconds to seconds
#define TIME_TO_SLEEP  10                                             // Time ESP32 will go to sleep (in seconds)

// Wifi Manager
const char* AP = "Jeager-Node";                                        // Wifi SSID name
const char* pass = "12345678";                                         // Wifi SSID password
bool wifistatus;
int input=15;
int counter = 0;
WiFiManager wifiManager;

void wakeupSetAP(){
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  if ( wakeup_reason == ESP_SLEEP_WAKEUP_EXT0) {
     Serial.println("Reset");
     if(!wifiManager.startConfigPortal(AP, pass) ){
        Serial.println("Failed to Connect");
        delay(2000);
        ESP.restart();
        delay(1000);
        }
     }
}

// Callback Function for Wifi Manager Setup
void configModeCallback (WiFiManager *myWiFiManager) {  
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  Serial.println(myWiFiManager->getConfigPortalSSID());
}

// Callback Function for Save Wifi Manager Setup
void saveConfigCallback () {
  Serial.println("Should save config");
  Serial.println(WiFi.softAPIP()); 
}

// Function WiFi Manager Access Point Mode
void APmode(){
  if ( digitalRead(input) == HIGH ) {
       Serial.println("Reset");
       if(!wifiManager.startConfigPortal(AP, pass) ){
          Serial.println("Failed to Connect");
          delay(2000);
          ESP.restart();
          delay(1000);
      }
      Serial.println("connected !!");
    }
}
// Check Wifi Connection
void wifiConnection(){
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network"); 
}

void setup() {
  Serial.begin(115200);                                                                                   // Set Baudrate for Serial Communication
  pinMode(input,INPUT);
  wifiManager.autoConnect(AP,pass);
  wifiManager.setAPCallback(configModeCallback);
  wifiManager.setSaveConfigCallback(saveConfigCallback);
  wakeupSetAP;
//  WiFi.begin(ssid, password);                                                                             // Set WiFi username and Password
//  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);                                          // Set timer to deep sleep
  antares.setDebug(true);
}

void loop() {

//   bool success = Ping.ping("www.google.com", 3);
 
//   if(!success){
//     Serial.println("Ping failed");
//     delay(1000);
//     return;
//   }
 
//   Serial.println("Ping succesful.");
 //  esp_deep_sleep_start();                                                                                 //ESP deep sleep start

  String text = "Hello! ";
  text += String(counter);
  Serial.println(text); // Print data to serial monitor
  antares.sendRaw(text, projectName, deviceName); // Store data from your Antares device
  counter++;
  delay(5000);

}
