// simple project using the Arduino UNO, Temperature sensor DHT22 and E-ink display from Waveshare to display temperature and humidity
// created by upir, 2022
// youtube channel: https://www.youtube.com/upir_upir
//
// FULL TUTORIAL: 
//
//
// Links from the video:
// Display (1.9inch Ink Screen Segment E-Paper E-Ink Display): https://s.click.aliexpress.com/e/_DlHYGnv
// WOKWI DHT22 sensor example project: https://wokwi.com/projects/344892337559700051
// Bitwise operators gifs: https://blog.wokwi.com/bitwise-operators-in-gifs/
// Display documentation: https://www.waveshare.com/wiki/1.9inch_Segment_e-Paper_Module_Manual#Arduino
// Arduino UNO: https://s.click.aliexpress.com/e/_AXDw1h
// Arduino breadboard prototyping shield: https://s.click.aliexpress.com/e/_ApbCwx
// Breadboard kit: https://s.click.aliexpress.com/e/_Dm6wuZN
//
// Related videos:
// E-ink Display Project Ideas 2022: https://youtu.be/oAJUBPxjCwk




#include <stdio.h>
#include <Wire.h>
#include "EPD_1in9.h"
#include <dht.h> // search for "DHTlib" when adding this library

dht DHT;

#define DHT22_PIN 5 // DHT22 sensor should be connected to pin 5

char digit_left[] = {0xbf, 0x00, 0xfd, 0xf5, 0x47, 0xf7, 0xff, 0x21, 0xff, 0xf7, 0x00};  // individual segments for the left part od the digit, index 10 is empty
char digit_right[] ={0x1f, 0x1f, 0x17, 0x1f, 0x1f, 0x1d, 0x1d, 0x1f, 0x1f, 0x1f, 0x00};  // individual segments for the right part od the digit, index 10 is empty

char temperature_digits[] = {1, 2, 3, 4}; // temperature digits > 1, 2, 3, 4 = 123.4°C
char humidity_digits[] = {5, 6, 7}; // humidity digits > 5, 6, 7 = 56.7%

float temperature_value = 1.2; // temperature value - will be used from the sensor
float humidity_value = 3.4; // humidity value - will be used from the sensor

unsigned char eink_segments[]  = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,       };  // all white, but will be updated later with proper digits

void setup()
{

  Serial.begin(115200); // start serial communication
	Wire.begin();  // start I2C commnunication with the e-ink display
	Serial.println("e-Paper init and clear");
	GPIOInit();
	EPD_1in9_init();

}

void loop()
{

  int chk = DHT.read22(DHT22_PIN); // read temperature and humidity from the DHT22 sensor
  temperature_value = DHT.temperature; // set the temperature
  humidity_value = DHT.humidity; // set the humidity

  // some updates for the e-ink display, I don´t understand most of it, so I´m keepin it here
  EPD_1in9_lut_5S();
  EPD_1in9_Write_Screen(DSPNUM_1in9_off);
  delay(500);
  EPD_1in9_lut_GC();
  EPD_1in9_lut_DU_WB();

  // set correct digits values based on the temperature
  temperature_digits[0] = int(temperature_value / 100) % 10;
  temperature_digits[1] = int(temperature_value / 10) % 10;
  temperature_digits[2] = int(temperature_value ) % 10;
  temperature_digits[3] = int(temperature_value * 10) % 10;

  // set correct digits values based on the humidity
  humidity_digits[0] = int(humidity_value / 10) % 10;
  humidity_digits[1] = int(humidity_value ) % 10;
  humidity_digits[2] = int(humidity_value * 10) % 10;

  // do not show leading zeros for values <100 and <10 both temperature and humidity
  if (temperature_value < 100) {temperature_digits[0] = 10;}
  if (temperature_value < 10) {temperature_digits[1] = 10;}  

  if (humidity_value < 10) {humidity_digits[0] = 10;}    

  // temperature digits
  eink_segments[0] = digit_right[temperature_digits[0]];
  eink_segments[1] = digit_left[temperature_digits[1]];
  eink_segments[2] = digit_right[temperature_digits[1]];  
  eink_segments[3] = digit_left[temperature_digits[2]];
  eink_segments[4] = digit_right[temperature_digits[2]] | B00100000 /* decimal point */;   
  eink_segments[11] = digit_left[temperature_digits[3]];
  eink_segments[12] = digit_right[temperature_digits[3]];    

  // humidity digits
  eink_segments[5] = digit_left[humidity_digits[0]];
  eink_segments[6] = digit_right[humidity_digits[0]];    
  eink_segments[7] = digit_left[humidity_digits[1]];
  eink_segments[8] = digit_right[humidity_digits[1]] | B00100000 /* decimal point */;        
  eink_segments[9] = digit_left[humidity_digits[2]];
  eink_segments[10] = digit_right[humidity_digits[2]] | B00100000 /* percentage sign */;   

  // special symbols - °C / °F, bluetooth, battery
  eink_segments[13] = 0x05 /* °C */ | B00001000 /* bluetooth */ | B00010000 /* battery icon */;

  // write segments to the e-ink screen
  EPD_1in9_Write_Screen(eink_segments);

  
  EPD_1in9_sleep();

  delay(2000); // wait 2 seconds, since the DHT22 sensor will only provide new values every 2 seconds


}
