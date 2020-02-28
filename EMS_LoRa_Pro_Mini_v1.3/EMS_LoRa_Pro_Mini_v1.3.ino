#include <loraid.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_SHT31.h"
#include <Wire.h>
#include <BH1750.h>

Adafruit_SHT31 sht31 = Adafruit_SHT31();
BH1750 lightMeter;
long interval = 900000;                                                 // 15mnt interval to send message
long previousMillis = 0;                                                // will store last time message sent
unsigned int counter = 0;                                               // message counter
int pinAnalog=A0;
unsigned int sample;
const int sampleWindow=50;
float temp,hum,lux,dB;                                                  // Temp, Hum, light, and decibel parameter   
int recvStatus = 1;                                                     // Message Status 
// Device ID From Jeager
String deviceid = "1580896631692479664";                                //Device ID LoRa-1
//String deviceid = "1581045862297725697";                                //Device ID LoRa-2
//String deviceid = "1582789735166861272";                                //Device ID LoRa-3
//String deviceid = "1582789743094999237";                                //Device ID LoRa-4
//String deviceid = "1582789752414068129";                                //Device ID LoRa-5
// Parameter Kalibrasi
float kalibrasi_temp = -(0.5);
float kalibrasi_hum = 0;
float kalibrasi_lux = 0;

// Get Sensor Value from decibel Sensor
float decibelSensor()
{
   unsigned long startMillis= millis();                                                                     // Start of sample window
   float peakToPeak = 0;                                                                                    // peak-to-peak level
   unsigned int signalMax = 0;                                                                              // minimum value
   unsigned int signalMin = 1024;                                                                           // maximum value                                                                               
   while (millis() - startMillis < sampleWindow)                                                            // collect data for 50 mS
   {
      sample = analogRead(pinAnalog);                                                                       // get reading from microphone
      if (sample < 1024)                                                                                    // toss out spurious readings
      {
         if (sample > signalMax)
         {
            signalMax = sample;                                                                             // save just the max levels
         }
         else if (sample < signalMin)
         {
            signalMin = sample;                                                                             // save just the min levels
         }
      }
   } 
   peakToPeak = signalMax - signalMin;                                                                      // max - min = peak-peak amplitude
   float db = map(peakToPeak,20,900,44.5,90);                                                               // calibrate for deciBels
   return db;
}
void sensorValue()
{
    temp = sht31.readTemperature() + kalibrasi_temp;
    hum = sht31.readHumidity() + kalibrasi_hum;
    lux = lightMeter.readLightLevel() + kalibrasi_lux;                                                       // Get data sensor cahaya
    dB = decibelSensor();                                                                                    // Get data sensor suara

}

void setup() {
  // Setup loraid access
  lora.init();
  // Set LoRaWAN Class. CLASS_A and CLASS_C are available
  lora.setDeviceClass(CLASS_A);
  // Set Data Rate
  // SF12 to SF7 available. Also SF7_250 for 250KHz variant 
  lora.setDataRate(2);
  // Put your Antares account key and Lora.id device address here
  lora.setAccessKey("a08c0faa1b988b11:9ae97d4dee93a723");
  
  lora.setDeviceId("b9a3bf46");                                 //device address LoRa-1
//  lora.setDeviceId("e2f1042d");                                 //device address LoRa-2
//  lora.setDeviceId("06e8a411");                                 //device address LoRa-3
//  lora.setDeviceId("49e545cd");                                 //device address LoRa-4
//  lora.setDeviceId("b4d4ce86");                                 //device address LoRa-5
  
  // Set I2C 
  lightMeter.begin();
  sht31.begin(0x44); 
}

void loop() {
  sensorValue(); 
  unsigned long currentMillis = millis();
  char dataSend[50];
  // Check if more than 10 seconds
  if(currentMillis - previousMillis > interval) {
    previousMillis = currentMillis; 

    String data = String(temp) + "," + String(hum) + "," + String(dB) + "," + String(lux) + "," + deviceid;
    data.toCharArray(dataSend,50);  
    lora.sendToAntares((unsigned char *)dataSend,strlen(dataSend),0);
  }
  // Check Lora RX
  lora.update();
}
