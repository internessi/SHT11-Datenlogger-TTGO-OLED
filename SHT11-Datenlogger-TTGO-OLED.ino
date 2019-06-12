#include <SHT1x.h>
#include <OneWire.h> 
#include <DallasTemperature.h>
#include <SSD1306.h>
#include <FS.h>
#include <SPIFFS.h>

#define dataPin  13
#define clockPin 14
#define ONE_WIRE_BUS 26 
#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  30        /* Time ESP32 will go to sleep (in seconds) */
#define FORMAT_SPIFFS_IF_FAILED true

RTC_DATA_ATTR int bootCount = 0;

int SHT_C, SHT_H, DS18_C, SHT_C2, SHT_H2, DS18_C2;
String output [6];


SSD1306 display(0x3c, 5, 4);
SHT1x sht1x(dataPin, clockPin);
OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);

void setup()
{
   delay(500);
  Serial.begin(115200);

  pinMode       (12, OUTPUT);  
  digitalWrite  (12, HIGH);
  pinMode       (27, OUTPUT);  
  digitalWrite  (27, HIGH);
  pinMode       (25, OUTPUT);  
  digitalWrite  (25, LOW);
  delay(500);
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
  display.displayOff(); 
  delay(500);


  output[1] = "ANEMOX LOG"; 
  output[2] = "SHT11: " + String(SHT_C) + "." + String(SHT_C2) + "c";
  output[3] = "SHT11: " + String(SHT_H) + "." + String(SHT_H2) + "%";
  output[4] = "DS18B: " + String(DS18_C) + "." + String(DS18_C2) + "c";


  display.displayOn();
  display.init();
  display.setFont(ArialMT_Plain_10);
  display.setColor(WHITE);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(1, (0), output[1]);
  display.drawString(1, (10), output[2]);
  display.drawString(1, (21), output[3]);
  display.drawString(1, (32), output[4]);
  display.display(); // display whatever is in the buffer

  delay(5000);

  display.clear();
  display.display();
  display.displayOff(); 





  //Increment boot number and print it every reboot
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));

  //Print the wakeup reason for ESP32
  print_wakeup_reason();

  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) +
  " Seconds");


  Serial.println("Going to sleep now");
  delay(1000);
  Serial.flush(); 
  esp_deep_sleep_start();
  Serial.println("This will never be printed");



}


void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
}
