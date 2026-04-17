/**
  * MCP9808 Basic Temperature Read Example
  *
  * Sensor
  * * Microchip MCP9808
  * * High Accuracy Digital Temperature Sensor
  *
  * Hardware Connection
  *
  * MCP9808        Microcontroller
  * ---
  * VCC    ->     3.3V
  * GND    ->     GND
  * SDA    ->     SDA
  * SCL    ->     SCL
  *
  * Optional
  * * ALERT → Interrupt pin (optional)
  *
  * Default I2C Address
  * * 0x18 (A0, A1, A2 = GND)
  *
  * Serial Output
  * * Converted Temperature (°C)
  */

#include <7Semi_MCP9808.h>

MCP9808_7Semi MCP9808;

void setup() {
  Serial.begin(115200);
  delay(100);  

  if (!MCP9808.beginI2C(0x18, Wire)) {
    Serial.println("MCP9808 not detected");
    while (1)
      ; 
  }

  /**
  * Set resolution
  * *  MCP9808_RES_0_125C = 0.0625°C 
  */
  MCP9808.setResolution(MCP9808_RES_0_0625C);

  Serial.println("MCP9808 initialized");
}

void loop() {

  float temperature;
  if (MCP9808.getTemperature(temperature)) {
    Serial.print("  Temperature: ");
    Serial.println(temperature, 2);
  }

  delay(500);
}
