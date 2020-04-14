//////////////////////////////////////////
//File name : All_sensor_environment
//Version : 1.2
//Author : Ikhsan Dwi Murtiyanto
//Date Last Modify : 4-7-2019
//////////////////////////////////////////
char ID_sector[32]="Sector 1";
//----------------library---------------//
#include <Wire.h>                       //I2C
#include <BH1750.h>                     //sensor light intensity
#include <Arduino_JSON.h>               //JSON
#include <Adafruit_Sensor.h>            //Adafruit sensor
#include <Adafruit_BME280.h>            //sensor (temperature, humidity, pressure, altitude) BME280
#include <SimpleKalmanFilter.h>         //kalman filter
#include <math.h>
//------------initialization------------//
BH1750 lightMeter(0x23);                //light sensor BH1750 I2C Address
SimpleKalmanFilter kf_temp(2, 2, 0.01); //koefisien kalman filter temperature
SimpleKalmanFilter kf_hum(2, 2, 0.1);   //koefisien kalman filter humidity
SimpleKalmanFilter kf_press(2, 2, 0.1); //koefisien kalman filter pressure
SimpleKalmanFilter kf_light(2, 2, 0.1); //koefisien kalman filter light intensity
SimpleKalmanFilter kf_ppm(1, 1, 0.1);   //koefisien kalman filter ammonia
SimpleKalmanFilter kf_wind(1, 1, 0.1);  //koefisien kalman filter wind speed
Adafruit_BME280 bme;                    //sensor BME280
#define SEALEVELPRESSURE_HPA (1013.25)  //calibration value pressure hPa
#define ALTITUDE 8.0                    //Altitude in Jakarta 
JSONVar myJSON;                         // JSON
//-----------Global Variable------------//
int light;
float   temperature     ,
        humidity        ,
        pressure        ,
        altitude        ,
        real_temp       ,
        real_hum        ,
        real_press      ,
        real_light      ,
        real_ppm        ,
        real_wind       ,
        estimated_temp  ,
        estimated_hum   ,
        estimated_press ,
        estimated_light ,
        estimated_ppm   ,
        estimated_wind  ;
//wind velocity
const int m_time = 3            ;      //Meassuretime in Seconds
int       wind_ct = 0           ;
float     wind = 0.0            ,
          meterPerSecond = 0.0  ;
unsigned long time = 0          ;
//Amonia gas
int gas_sensor = A1 ;    //Sensor pin
float   m = -0.263          ,    //Slope
        b = 0.42            ,    //Y-Intercept
        R0=2.77             ,    //R0 value from calibration
        sensor_volt         ,    //Define variable for sensor voltage
        RS_gas              ,    //Define variable for sensor resistance
        ratio               ,    //Define variable for ratio
        sensorValue         ,    //get value from sensor
        RS_air              ,    //Define variable for sensor resistance
        batas_ppm=10        ;
double ppm_log      ,    //Get ppm value in linear scale according to the the ratio value
       ppm          ;    //Convert ppm value to log scale
bool   status       ;
const long SERIAL_REFRESH_TIME = 1000;
long refresh_time   ;

void setup() {
  Serial.begin(9600)        ;
  Wire.begin()              ;
  time = millis()           ;
  pinMode(gas_sensor, INPUT);               //Set gas sensor as input

  status = bme.begin(0x76)  ;               //The I2C address of the sensorBME280 I use is 0x76
  if (!status) {                            //checking i2c address
      Serial.print("Error. Check");
      Serial.println("I2C connections BME280");
      while (1);
  }
  
  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
//    Serial.println(F("--Start Monitoring-- \n"));
  }
  else {
    Serial.println(F("Error initialising I2C Address"));
  }
//  Serial.print("Reading sensor");
  Serial.println();

  delay(1000);
}

// Get data temperature
void getTemperature(){
  temperature = bme.readTemperature();
}
// Get data humidity
void getHumidity(){
  humidity = bme.readHumidity();
}
// Get data pressure
void getPressure(){
  pressure = bme.readPressure();
  pressure = bme.seaLevelForAltitude(ALTITUDE,pressure);
  pressure = pressure/100.0F;
}
// Get data Altitude
void getAltitude(){
  altitude=bme.readAltitude(SEALEVELPRESSURE_HPA);
}
// GEt data light intensity
void getLight(){
  light = lightMeter.readLightLevel();
}
// Wind speed unit increment
void countWind() {
  wind_ct ++;
}
// Calculate wind speed
void get_wind_speed() {
  //meassure wind speed with interupt digital pin 
  wind_ct       = 0                           ;
  time = millis()                             ;
  attachInterrupt(1, countWind, RISING)       ;  // RISING (mode)= the interrupt will be executed if there is a change in the reading value from LOW to HIGH (executed 1 time after a change occurs)
  delay(3000)                                 ;
  detachInterrupt(1)                          ;  //pin interrupt 1
  wind = (float)wind_ct / (float)m_time * 2.4 ;  //calculation in Km/h
  meterPerSecond = wind / 3.6                 ;  //convert Km/h to m/s
}
// Menghitung data gas
void get_gas_ppm() {  
  sensorValue = analogRead(gas_sensor)              ; // Read analog values of sensor
  sensor_volt = sensorValue * (5.0 / 1023.0)        ; // Convert analog values to voltage
  RS_gas      = ((5.0 * 10.0) / sensor_volt) - 10.0 ; // Get value of RS in a gas
  ratio       = RS_gas / R0                         ; // Get ratio RS_gas/RS_air
  
  //convert to ppm value//
  ppm_log   = (log10(ratio) - b) / m; 
  ppm       = pow(10, ppm_log); 
}

void filter(){      // filtering sensor value
  getTemperature();
  real_temp = temperature;
  estimated_temp = kf_temp.updateEstimate(real_temp);

  getHumidity();
  real_hum = humidity;
  estimated_hum = kf_hum.updateEstimate(real_hum);

  getPressure();
  real_press = pressure;
  estimated_press = kf_press.updateEstimate(real_press);

  getLight();
  real_light = light;
  estimated_light = kf_light.updateEstimate(real_light);

  get_gas_ppm();
  real_ppm = ppm;
  estimated_ppm = kf_ppm.updateEstimate(real_ppm);

  get_wind_speed();
  real_wind = meterPerSecond;
  estimated_wind = kf_wind.updateEstimate(real_wind);
}

void realValue(){       //display real sensor value in serial monitor
    get_wind_speed();
    get_gas_ppm();
    getLight();
    getPressure();
    getHumidity();
    getTemperature();
    Serial.print("real value | T:");
    Serial.print(temperature);
    Serial.print(" H:");
    Serial.print(humidity);
    Serial.print(" P:");
    Serial.print(pressure);
    Serial.print(" L:");
    Serial.print(light);
    Serial.print(" ppm:");
    Serial.print(ppm);
    Serial.print(" wind:");
    Serial.print(meterPerSecond);
    Serial.println();
}

void dataJSON(){            //convert data to JSON format

  filter();
  int kelembapan         = estimated_hum   ;
  int temperatur         = estimated_temp  ;
  int tekanan            = real_press      ;
  int kadar_amonia       = real_ppm        ;
  int intensitas_cahaya  = estimated_light ;
  int kecepatan_angin    = meterPerSecond  ;

  myJSON["id_perangkat"]        = ID_sector         ;
  myJSON["kadar_amonia"]        = kadar_amonia      ;
  myJSON["kelembapan"]          = kelembapan        ;
  myJSON["intensitas_cahaya"]   = intensitas_cahaya ;    
  myJSON["kecepatan_angin"]     = kecepatan_angin   ;
  myJSON["temperatur"]          = temperatur        ;
  myJSON["tekanan"]             = tekanan           ;
  
  if(millis() > refresh_time){
//    realValue();
    Serial.println(myJSON);                         //display data JSON in Serial monitor 
    refresh_time = millis() + SERIAL_REFRESH_TIME;
  }
}

void loop() {
  dataJSON();
}
