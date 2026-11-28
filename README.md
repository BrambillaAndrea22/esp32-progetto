# esp-32 stazioneAmbiente

STAZIONE AMBIENTE CON ESP32, DHT11 E DISPLAY OLED

Un ESP32 legge temperatura e umidità da un sensore DHT11 e mostra i valori su un display OLED. Se il sensore non risponde, lo schermo segnala l'errore.
Le foto del circuito montato si trovano nella cartella img.

COSA FA

- Legge temperatura e umidità ogni 2 secondi con una cifra decimale.
- Mostra i valori sul display OLED.
- Segnala l'errore se il sensore non risponde.

COMPONENTI

- ESP32 Dev Module (ESP-32S, CP2102), scheda a 30 pin con micro-USB
- Display OLED 0,96 pollici SSD1306, 128x64, I2C a 4 pin
- Sensore DHT11 
- Breadboard 830 punti e cavetti jumper

COLLEGAMENTI

Display OLED:
VCC -> 3V3
GND -> GND
SDA -> D21
SCL -> D22

Sensore DHT11:
VCC -> 3V3
GND -> GND
OUT -> D4

I pin si possono cambiare nel file include/config.h.

STRUTTURA DEL PROGETTO

- platformio.ini: scheda, librerie e velocità del monitor seriale
- include/config.h: pin, indirizzo I2C e intervallo di lettura
- src/main.cpp: programma principale
- img: foto del circuito

COME COMPILARE E CARICARE

Servono Visual Studio Code e l'estensione PlatformIO IDE.
1. Clona il repository e apri la cartella esp32-stazioneAmbiente in VS Code.
2. Aspetta che PlatformIO scarichi gli strumenti e le librerie (la prima volta può richiedere qualche minuto).
3. Collega l'ESP32 al PC con un cavo USB per dati. Su Windows può servire il driver CP210x di Silicon Labs.
4. Premi Build per compilare e Upload per caricare il programma. Se resta bloccato su "Connecting...", tieni premuto il pulsante BOOT sulla scheda.

Librerie usate:
- adafruit/DHT sensor library
- adafruit/Adafruit Unified Sensor
- olikraus/U8g2

LIMITI DEL DHT11

- Temperatura da 0 a 50 gradi con errore di circa 2 gradi.
- Umidità da circa 20% a circa 80% con errore di circa 5%.
- Può essere letto al massimo una volta al secondo.