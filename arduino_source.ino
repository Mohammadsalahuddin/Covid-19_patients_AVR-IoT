/*******************************************************************
* The Arduino uno frimware for IoT based Covid-19 Patient Monitoring System *  
*        @author: Mohammad Salah Uddin           *
*        @email: akash.bangla@gmail.com          *
*        Last update : 20 January 2021           *
*                  *
********************************************************************
*/

#include <MAX30100_PulseOximeter.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>




//VARIABLE TO HOLD THE SENSORS DATA
int bpm;
int spo2;
float temp;

//the sea level presure in your region (****)
#define SEALEVELPRESSURE_HPA (1023)
Adafruit_BME280 bme;    // BME280  Sensnor declaration 
unsigned long currentMillis;   //hold the current time



//pulse oximeter time period (measurment time period)
#define REPORTING_PERIOD_MS     1000
PulseOximeter pox;
uint32_t tsLastReport = 0;

// Callback (registered below) fired when a pulse is detected
void onBeatDetected()
{
    Serial.println("Beat!");
}

//pulse sensor callback function
void measured_pulse(){
    // Make sure to call update as fast as possible
    pox.update();
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
      bpm = pox.getHeartRate();
      spo2 = pox.getSpO2();
      // temp =  pox.getTemperature();
      tsLastReport = millis();
    }
    
}


void setup() {
  Serial.begin(115200);
  Serial.println(F("BME280 test"));
  Serial.println("Initializing MAX30100");
  // Initialize the PulseOximeter instance and register a beat-detected callback
  pox.begin();
  pox.setOnBeatDetectedCallback(onBeatDetected);
  
  bool status;
  // default settings
  status = bme.begin();
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
 
}

void loop() {
  float humidity;
  currentMillis = millis();
  measured_pulse();
  humidity =  bme.readHumidity() ;
  temp = bme.readTemperature();

  // Push the data via UART
  Serial.print("#Temperature = ");
  Serial.print(temp);
  Serial.print("+Humidity = ");
  Serial.print(humidity);
  Serial.print("+Heart Rate = ");
  Serial.print(bpm);
  Serial.print("+SPO2 = ");
  Serial.print(spo2);
  Serial.println("&");
  
  
  
}
