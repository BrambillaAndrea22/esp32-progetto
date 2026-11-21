#include <Arduino.h>
#include <Wire.h>
#include <DHT.h>
#include <U8g2lib.h>
#include "config.h"

DHT airSensor(AIR_SENSOR_PIN, DHT11);
U8G2_SSD1306_128X64_NONAME_F_HW_I2C display(U8G2_R0, U8X8_PIN_NONE, DISPLAY_SCL_PIN, DISPLAY_SDA_PIN);

bool displayReady = false;
bool sensorFailed = false;
bool validReadings = false;
unsigned long lastAttempt_ms = 0;
unsigned long lastValid_ms = 0;
float lastTemp = NAN;
float lastHum = NAN;

void renderDisplay() {
    if (!displayReady)
        return;
    char line[32];
    display.clearBuffer();
    display.setFont(u8g2_font_6x12_tf);
    display.drawUTF8(0, 10, "Stazione ambiente");
    if (!validReadings && !sensorFailed) {
        display.drawUTF8(0, 27, "Lettura...");
    } else {
        if (validReadings) {
            snprintf(line, sizeof(line), "Temp: %.1f °C", lastTemp);
            display.drawUTF8(0, 26, line);

            snprintf(line, sizeof(line), "Umidità: %.1f %%", lastHum);
            display.drawUTF8(0, 40, line);
        } else {
            display.drawUTF8(0, 26, "Temp: -- °C");
            display.drawUTF8(0, 40, "Umidità: -- %");
        }
        if (sensorFailed) 
            display.drawUTF8(0, 52, "Errore sensore");
    }
    display.sendBuffer();
}

void readAirSensor() {
    float temp = airSensor.readTemperature();
    float hum = airSensor.readHumidity();

    if (isnan(temp) || isnan(hum))
        sensorFailed = true;
    else {
        lastTemp = temp;
        lastHum = hum;
        lastValid_ms = millis();
        validReadings = true;
        sensorFailed = false;
    }
    renderDisplay();
}
 
void setup() {
    airSensor.begin();
    Wire.begin();
    Wire.setClock(10000);
    Wire.beginTransmission(DISPLAY_I2C_ADDR);
    displayReady = (Wire.endTransmission() == 0);
    if (displayReady) {
        display.setI2CAddress(DISPLAY_I2C_ADDR << 1);
        display.begin();
        renderDisplay();
    } 
    lastAttempt_ms = millis();
}

void loop() {
    unsigned long time = millis();
    if (time - lastAttempt_ms >= SENSOR_INTERVAL_MS) {
        lastAttempt_ms = millis();
        readAirSensor();
    }
}