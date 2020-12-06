/*
 * mhz19b_sensor.ino
 */

#include <Arduino.h>
#include <arduino_homekit_server.h>
#include <SoftwareSerial.h>
#include "MHZ19.h"
#include "wifi_info.h"

#define LOG_D(fmt, ...)   printf_P(PSTR(fmt "\n") , ##__VA_ARGS__);
#define RX_PIN 5
#define TX_PIN 4
#define BAUDRATE 9600

MHZ19 myMHZ19;

SoftwareSerial mySerial(RX_PIN, TX_PIN); 

void setup() {
	Serial.begin(115200);
  mySerial.begin(BAUDRATE);
  myMHZ19.begin(mySerial);
  Serial.print("ABC Status: "); myMHZ19.getABC() ? Serial.println("ON") :  Serial.println("OFF"); 
	wifi_connect(); // in wifi_info.h
  //homekit_storage_reset();
	my_homekit_setup();
}

void loop() {
	my_homekit_loop();
	delay(10);
}

//==============================
// Homekit setup and loop
//==============================

// access your homekit characteristics defined in my_accessory.c
extern "C" homekit_server_config_t config;
extern "C" homekit_characteristic_t cha_current_temperature;
extern "C" homekit_characteristic_t cha_carbon_dioxide_detected;
extern "C" homekit_characteristic_t cha_carbon_dioxide_level;
extern "C" homekit_characteristic_t cha_carbon_dioxide_peak_level;

static uint32_t next_heap_millis = 0;
static uint32_t next_report_millis = 0;

void my_homekit_setup() {
	arduino_homekit_setup(&config);
}

void my_homekit_loop() {
	arduino_homekit_loop();
	const uint32_t t = millis();
	if (t > next_report_millis) {
		// report sensor values every 10 seconds
		next_report_millis = t + 10 * 1000;
		my_homekit_report();
	}
	if (t > next_heap_millis) {
		// show heap info every 5 seconds
		next_heap_millis = t + 5 * 1000;
		LOG_D("Free heap: %d, HomeKit clients: %d",
				ESP.getFreeHeap(), arduino_homekit_connected_clients_count());

	}
}

void my_homekit_report() {
	float co2_value = myMHZ19.getCO2();
  float temperature_value = myMHZ19.getTemperature();
	cha_carbon_dioxide_level.value.float_value = co2_value;
  cha_current_temperature.value.float_value = temperature_value;
	LOG_D("Current CO2 level: %.1f", co2_value);
  LOG_D("Current temperature: %.1f", temperature_value);
  if (cha_carbon_dioxide_level.value.float_value > cha_carbon_dioxide_peak_level.value.float_value) {
    cha_carbon_dioxide_peak_level.value.float_value = cha_carbon_dioxide_level.value.float_value;
  }
  if (cha_carbon_dioxide_level.value.float_value > 1000) {
    cha_carbon_dioxide_detected.value.uint8_value = 1;
  }
  else {
    cha_carbon_dioxide_detected.value.uint8_value = 0;
  }
	homekit_characteristic_notify(&cha_carbon_dioxide_detected, cha_carbon_dioxide_detected.value);
  homekit_characteristic_notify(&cha_carbon_dioxide_level, cha_carbon_dioxide_level.value);
  homekit_characteristic_notify(&cha_carbon_dioxide_peak_level, cha_carbon_dioxide_peak_level.value);
  homekit_characteristic_notify(&cha_current_temperature, cha_current_temperature.value);
}
