/*
  Truck-07 · I2C OLED dashboard (simple version)
  OLED: 0.96" SSD1306, 128x64, I2C address 0x3C
  Wiring: OLED VCC -> 3V3, GND -> GND, SDA -> GPIO 21, SCL -> GPIO 22
*/
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SDA_PIN   21
#define SCL_PIN   22
#define OLED_ADDR 0x3C

Adafruit_SSD1306 oled(128, 64, &Wire, -1);   // width, height, I2C bus, no reset pin

// Ask every address 1..126 "are you there?" and print who answers
void scanI2C() {
  Serial.println("Scanning I2C bus...");
  for (byte addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {
      Serial.printf("Found device at 0x%02X\n", addr);
    }
  }
  Serial.println("Scan done");
}

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);     // start I2C on our two pins
  scanI2C();

  // Is the OLED answering at its address? (ACK = 0)
  Wire.beginTransmission(OLED_ADDR);
  if (Wire.endTransmission() != 0) {
    Serial.println("OLED not found! Check wiring and address");
    while (true) delay(1000);       // stop here
  }

  oled.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  Serial.println("OLED ready");
}

void loop() {
  float temp = random(880, 960) / 10.0;   // fake engine temp 88.0 - 95.9
  int speed  = random(40, 81);            // fake speed 40 - 80 km/h

  oled.clearDisplay();                    // 1. clear the drawing (in memory)
  oled.setTextColor(SSD1306_WHITE);

  oled.setTextSize(1);                    // 2. title
  oled.setCursor(0, 0);
  oled.print("TRUCK-07  DAQ");
  oled.drawLine(0, 10, 127, 10, SSD1306_WHITE);

  oled.setTextSize(2);                    // 3. big temperature
  oled.setCursor(0, 16);
  oled.print(temp, 1);
  oled.print(" C");

  oled.setTextSize(1);                    // 4. speed text
  oled.setCursor(0, 38);
  oled.print("Speed: ");
  oled.print(speed);
  oled.print(" km/h");

  oled.drawRect(0, 52, 128, 10, SSD1306_WHITE);                 // 5. speed bar
  oled.fillRect(2, 54, map(speed, 0, 100, 0, 124), 6, SSD1306_WHITE);

  oled.display();                         // 6. send it to the screen over I2C

  Serial.printf("Temp %.1f C, Speed %d km/h\n", temp, speed);
  delay(1000);
}