#include <Arduino.h>
#include <Wire.h>
#include <DHT.h>
#include <U8g2lib.h>
#include "config.h"

DHT airSensor(AIR_SENSOR_PIN, DHT11);
U8G2_SSD1306_128X64_NONAME_F_HW_I2C display(U8G2_R0, U8X8_PIN_NONE, DISPLAY_SCL_PIN, DISPLAY_SDA_PIN);

bool displayReady = false;
bool haveValidReadings = false;
bool sensorFailed = false;
float lastTemperature = NAN;
float lastHumidity = NAN;
unsigned long lastAttemptMs = 0;
unsigned long lastValidMs = 0;

void renderDisplay() {
    if (!displayReady) {
        return;
    }
    char line[32];
    display.clearBuffer();
    display.setFont(u8g2_font_6x12_tf);
    display.drawUTF8(0, 10, "Stazione ambiente");

    if (!haveValidReadings && !sensorFailed) {
        display.drawUTF8(0, 27, "Lettura...");
    } else {
        if (haveValidReadings) {
            snprintf(line, sizeof(line), "Temp: %.1f °C", lastTemperature);
            display.drawUTF8(0, 26, line);

            snprintf(line, sizeof(line), "Umidità: %.1f %%", lastHumidity);
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
    const float temperature = airSensor.readTemperature();
    const float humidity = airSensor.readHumidity();

    if (isnan(temperature) || isnan(humidity)) 
        sensorFailed = true;
    else {
        lastTemperature = temperature;
        lastHumidity = humidity;
        lastValidMs = millis();
        haveValidReadings = true;
        sensorFailed = false;
    }
    renderDisplay();
}

void setup() {
    airSensor.begin();

    Wire.begin(DISPLAY_SDA_PIN, DISPLAY_SCL_PIN);
    Wire.setClock(100000);
    Wire.beginTransmission(DISPLAY_I2C_ADDR);
    displayReady = (Wire.endTransmission() == 0);

    if (displayReady) {
        display.setI2CAddress(DISPLAY_I2C_ADDR << 1);
        display.begin();
        renderDisplay();
    } 

    lastAttemptMs = millis();
}

void loop() {
    const unsigned long time = millis();

    if (time - lastAttemptMs >= SENSOR_INTERVAL_MS) {
        lastAttemptMs = time;
        readAirSensor();
    }
}