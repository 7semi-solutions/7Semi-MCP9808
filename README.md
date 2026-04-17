# 7Semi MCP9808 Arduino Library

Arduino driver for the Microchip MCP9808 high-accuracy digital temperature sensor.

The MCP9808 provides precise temperature measurement with up to ±0.25°C accuracy, low power consumption, and programmable alert functionality for temperature monitoring applications.

---

## Features

- High-accuracy temperature measurement (±0.25°C typical)

- Configurable resolution (0.5°C to 0.0625°C)

- Upper, Lower, and Critical temperature thresholds

- Alert output support:
  - Comparator mode
  - Interrupt mode

- Hysteresis configuration

- Shutdown (low power) mode

- Raw temperature access

- Device ID and Manufacturer ID reading

---

## Connections / Wiring

The MCP9808 uses **I²C communication**.

---

## I²C Connection

| MCP9808 Pin | MCU Pin         | Notes            |
|-------------|-----------------|------------------|
| VCC         | 3.3V            | **3.3V only**    |
| GND         | GND             | Common ground    |
| SDA         | SDA             | I²C data         |
| SCL         | SCL             | I²C clock        |
| ALERT       | GPIO (optional) | Interrupt output |

---

## I²C Notes

- Default I²C address: `0x18`  
- Address configurable via A0, A1, A2 pins  
- Supported bus speeds:
  - 100 kHz
  - 400 kHz (recommended)

---

## Installation

### Arduino Library Manager

1. Open Arduino IDE  
2. Go to Library Manager  
3. Search for **7Semi MCP9808**  
4. Click Install  

---

### Manual Installation

1. Download repository as ZIP  
2. Arduino IDE → Sketch → Include Library → Add .ZIP Library  

---

## Library Overview

### Reading Temperature

```cpp
float temperature;

sensor.getTemperature(temperature);
```

- Returns temperature in °C

### Reading Raw Temperature

```cpp

uint16_t raw;

sensor.getTemperatureRaw(raw);
```

### Setting Resolution

```cpp
sensor.setResolution(MCP9808_RES_0_125C);
```

### Setting Temperature Limits

```cpp
sensor.setUpperTemperature(30.0);
sensor.setLoweTemperature(20.0);
sensor.setCriticalTemperature(35.0);
```

### Alert Configuration

```cpp
sensor.setAlertMode(COMPARATOR_OUTPUT);
sensor.setAlertPolarity(ACTIVE_HIGH);
sensor.enableAlert(true);
```

### Reading Alert Status

```cpp
bool upper, lower, critical;

sensor.getAlert(upper, lower, critical);
```

### Operating Modes

```cpp
sensor.setMode(SHUTDOWN_MODE);
sensor.setMode(CONTINUOUS_MODE);
```
