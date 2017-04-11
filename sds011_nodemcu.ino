#include <SoftwareSerial.h>
#include "Sds011.h"

#define SDS_RX D1
#define SDS_TX D2
#define SAMPLES 5

SoftwareSerial serialSDS(SDS_RX, SDS_TX);
sds011::Sds011 sds(serialSDS);

struct {
  bool valid;
  float pm10; 
  float pm25; 
} sds_data = {};

void initSDS(){
  sds.set_sleep(false);
  sds.set_mode(sds011::QUERY);
  sds.set_sleep(true);
}

void readSDS() {
  int pm25, pm10;
  sds.set_sleep(false);
  sds_data.valid = sds.query_data_auto(&pm25, &pm10, SAMPLES);
  sds_data.pm10 = float(pm10)/10;
  sds_data.pm25 = float(pm25)/10;
  sds.set_sleep(true);
}

void readAndPrint(){
  readSDS();
  
  if (sds_data.valid) {
   String result = "PM2.5: " + String(sds_data.pm25) + " | PM10: " + String(sds_data.pm10) + "\n";
   Serial.print(result);
  }
}

void setup() {
  Serial.begin(115200);
  serialSDS.begin(9600);
}

void loop() {
   readAndPrint();
}