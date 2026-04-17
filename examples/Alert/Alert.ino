/**
  * MCP9808 Alert Example
  *
  * Sensor
  * * Microchip MCP9808
  * * High Accuracy Temperature Sensor with Alert Output
  *
  * Features Demonstrated
  * * Set upper / lower / critical temperature limits
  * * Enable alert output
  * * Read alert status flags
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
  * ALERT  ->     GPIO (interrupt input)
  *
  * Default I2C Address
  * * 0x18
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

  MCP9808.setUpperTemperature(30.0);  // Upper limit

  float upperTemperature = 0;
  if (MCP9808.getUpperTemperature(upperTemperature)) {
    Serial.print("Upper Temperature: ");
    Serial.println(upperTemperature);
  }

  MCP9808.setLoweTemperature(20.0);  // Lower limit
  float lowerTemperature = 0;
  if (MCP9808.getLoweTemperature(lowerTemperature)) {
    Serial.print("Lower Temperature: ");
    Serial.println(lowerTemperature);
  }

  MCP9808.setCriticalTemperature(35.0);  // Critical limit
  float criticalTemperature = 0;
  if (MCP9808.getCriticalTemperature(criticalTemperature)) {
    Serial.print("Critical Temperature: ");
    Serial.println(criticalTemperature);
  }

  MCP9808.setAlertMode(COMPARATOR_OUTPUT);  // Continuous compare
  MCP9808.setAlertPolarity(ACTIVE_HIGH);              // Active HIGH output
  MCP9808.enableAlert(true);                // Enable alert

  Serial.println("MCP9808 Alert Example Started");
}

void loop() {
  float temperature;

  if (MCP9808.getTemperature(temperature)) {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");
  }

  bool upper, lower, critical;

  if (MCP9808.getAlert(upper, lower, critical)) {
    if (upper)
      Serial.println("Upper Temperature Exceeded");


    if (lower)
      Serial.println("Lower Temperature Exceeded");

    if (critical)
      Serial.println("Critical Temperature Exceeded");
  }

  delay(1000);
}
