/**
 * ReadSHT1xValues
 *
 * Read temperature and humidity values from an SHT1x-series (SHT10,
 * SHT11, SHT15) sensor.
 *
 * Copyright 2009 Jonathan Oxer <jon@oxer.com.au>
 * www.practicalarduino.com
 */

#include <SHT1x.h>
#include <OneWire.h> 
#include <DallasTemperature.h>
#include <SSD1306.h>

// Specify data and clock connections and instantiate SHT1x object
#define dataPin  13
#define clockPin 14
#define ONE_WIRE_BUS 26 

int SHT_C, SHT_H, DS18_C, SHT_C2, SHT_H2, DS18_C2;
String output [6];


SSD1306 display(0x3c, 5, 4);
SHT1x sht1x(dataPin, clockPin);
OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);

void setup()
{
  Serial.begin(115200); // Open serial connection to report values to host

  pinMode       (12, OUTPUT);  
  digitalWrite  (12, HIGH);
  pinMode       (27, OUTPUT);  
  digitalWrite  (27, HIGH);
  pinMode       (25, OUTPUT);  
  digitalWrite  (25, LOW);
  delay(200);
  Serial.println("Starting up");
  
  sensors.begin(); 
  
  display.init();
  display.setFont(ArialMT_Plain_10);
  display.setColor(WHITE);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(20, 20, "ANEMOX");
  display.display();

}

void loop()
{


  
  float temp_c;
  float temp_DS18;
  float humidity;

  // Read values from the sensor
  temp_c = sht1x.readTemperatureC();
  temp_c = temp_c  + 0.05;  
  SHT_C = int(temp_c*10);  
  SHT_C2 = SHT_C-(int(SHT_C/10)*10);
  SHT_C = SHT_C/10; 
  
  humidity = sht1x.readHumidity();  
  humidity = humidity  + 0.05;  
  SHT_H = int(humidity*10);  
  SHT_H2 = SHT_H-(int(SHT_H/10)*10);
  SHT_H = SHT_H/10; 

  Serial.print("SHT: ");
  Serial.print(temp_c, DEC);
  Serial.print("/ ");
  Serial.print(humidity);
  sensors.requestTemperatures(); // Send the command to get temperature readings 
  temp_DS18 = sensors.getTempCByIndex(0);
  temp_DS18 = temp_DS18  + 0.05;  
  DS18_C = int(temp_DS18*10);  
  DS18_C2 = DS18_C-(int(DS18_C/10)*10);
  DS18_C = DS18_C/10; 
  Serial.print("% - DS18B20: "); 
  Serial.print(temp_DS18, DEC);
  Serial.println("C"); 

  display.clear();
  display.display();

  output[1] = "ANEMOX LOG"; 
  output[2] = "SHT11: " + String(SHT_C) + "." + String(SHT_C2) + "c";
  output[3] = "SHT11: " + String(SHT_H) + "." + String(SHT_H2) + "%";
  output[4] = "DS18B: " + String(DS18_C) + "." + String(DS18_C2) + "c";

  display.drawString(1, (0), output[1]);
  display.drawString(1, (10), output[2]);
  display.drawString(1, (21), output[3]);
  display.drawString(1, (32), output[4]);

  display.display(); // display whatever is in the buffer





  

  delay(3000);
}
