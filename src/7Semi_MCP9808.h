#ifndef _7SEMI_MCP9808_H_ // If NOT defined
#define _7SEMI_MCP9808_H_ // Define it

#include <Arduino.h>
#include <Wire.h>

#include "7Semi_I2C_Interface.h"
#include "BusIO_7Semi.h"

// Default I2C address of MCP9808
#define MCP9808_DEFAULT_ADDR 0x18

// MCP9808 Register Map
#define MCP9808_REG_CONFIG 0x01
#define MCP9808_REG_TUPPER 0x02
#define MCP9808_REG_TLOWER 0x03
#define MCP9808_REG_TCRIT 0x04
#define MCP9808_REG_TAMBIENT 0x05
#define MCP9808_REG_MANUF_ID 0x06
#define MCP9808_REG_DEVICE_ID 0x07
#define MCP9808_REG_RESOLUTION 0x08

#define MCP9808_EXPECTED_ID 0x54

/**
 * Alert Mode
 *
 * * Comparator → continuous comparison
 * * Interrupt  → latch until cleared
 */
enum MCP9808_AlertMode
{
    COMPARATOR_OUTPUT = 0,
    INTERRUPT_OUTPUT = 1
};

/**
 * Alert Polarity
 *
 * * ACTIVE_LOW  → default
 * * ACTIVE_HIGH → inverted output
 */
enum MCP9808_AlertPlarity
{
    ACTIVE_LOW = 0,
    ACTIVE_HIGH = 1
};

/**
 * Alert Select
 *
 * * ALL_ALERT       → all thresholds trigger
 * * ONLY_CRIT_ALERT → only critical triggers
 */
enum MCP9808_AlertSelect
{
    ALL_ALERT = 0,
    ONLY_CRIT_ALERT = 1
};

/**
 * Sensor Mode
 *
 * * CONTINUOUS → normal operation
 * * SHUTDOWN   → low power mode
 */
enum MCP9808_Mode
{
    CONTINUOUS_MODE = 0,
    SHUTDOWN_MODE = 1
};

/**
 * Hysteresis Levels
 *
 * * Defines temperature offset for alert reset
 */
enum MCP9808_Thyst
{
    MCP9808_THYST_0C = 0,
    MCP9808_THYST_1_5C = 1,
    MCP9808_THYST_3C = 2,
    MCP9808_THYST_6C = 3
};

/**
 * Resolution Settings
 * 
 * * Defines temperature resolution and conversion time
 */
enum MCP9808_Resolution
{
    MCP9808_RES_0_5C = 0,
    MCP9808_RES_0_25C = 1,
    MCP9808_RES_0_125C = 2,
    MCP9808_RES_0_0625C = 3
};

class MCP9808_7Semi
{
public:

    MCP9808_7Semi();


    ~MCP9808_7Semi()
    {
        if (bus)
            delete bus;
    }

    /**

    * beginI2C
    *
    * * Initializes sensor over I2C
    * * Creates communication interface
    * * Verifies device presence using Manufacturer ID
    *
    * Returns:
    * * true  → Sensor ready
    * * false → Initialization failed
    */
    bool beginI2C(uint8_t address = MCP9808_DEFAULT_ADDR,
                  TwoWire &port = Wire,
                  uint32_t clock = 400000);

    /**
    * getManufacturerID
    *
    * * Reads manufacturer ID register
    * * Used for device verification
    */
    bool getManufacturerID(uint16_t &id);

    /**
    * setUpperTemperature
    *
    * * Sets upper temperature threshold
    */
    bool setUpperTemperature(float upperTemp);

    /**
    * getUpperTemperature
    *
    * * Reads upper temperature threshold
    */
    bool getUpperTemperature(float &upperTemp);

    /**
    * setLowerTemperature
    *
    * * Sets lower temperature threshold
    */
    bool setLoweTemperature(float lowerTemp);

    /**
    * getLowerTemperature
    *
    * * Reads lower temperature threshold
    */
    bool getLoweTemperature(float &lowerTemp);

    /**
    * setCriticalTemperature
    *
    * * Sets critical temperature threshold
    */
    bool setCriticalTemperature(float criticalTemp);

    /**
    * getCriticalTemperature
    *
    * * Reads critical temperature threshold
    */
    bool getCriticalTemperature(float &criticalTemp);

    /**
    * getTemperatureRaw
    *
    * * Reads raw 16-bit temperature register
    */
    bool getTemperatureRaw(uint16_t &rawTemperature);

    /**
    * getTemperature
    *
    * * Converts raw value to Celsius
    */
    bool getTemperature(float &temperature);

    /**
    * getAlert
    *
    * * Reads alert flags from temperature register
    */
    bool getAlert(bool &isUpperTemp,
                  bool &isLowerTemp,
                  bool &isCriticalTemp);

    /**
    * setAlertMode
    *
    * * Configures alert mode (comparator/interrupt)
    */
    bool setAlertMode(MCP9808_AlertMode mode);

    /**
    * getAlertMode
    *
    * * Reads alert mode
    */
    bool getAlertMode(bool &mode);

    /**
    * setAlertPolarity
    *
    * * Sets alert output polarity
    */
    bool setAlertPolarity(MCP9808_AlertPlarity polarity);

    /**
    * getAlertPolarity
    *
    * * Reads alert polarity
    */
    bool getAlertPolarity(bool &polarity);

    /**
    * setAlertSelect
    *
    * * Selects which alert condition triggers output
    */
    bool setAlertSelect(MCP9808_AlertSelect onlyCrit);

    /**
    * getAlertSelect
    *
    * * Reads alert selection mode
    */
    bool getAlertSelect(bool &onlyCrit);

    /**
    * enableAlert
    *
    * * Enables or disables alert output
    */
    bool enableAlert(bool enable);

    /**
    * isEnableAlert
    *
    * * Reads alert enable state
    */
    bool isEnableAlert(bool &enable);

    /**
    * getAlertStatus
    *
    * * Reads current alert status flag
    */
    bool getAlertStatus(bool &alert);

    /**
    * clearAlert
    *
    * * Clears interrupt alert flag
    */
    bool clearAlert();

    /**
    * lockLimits
    *
    * * Locks temperature limit registers
    */
    bool lockLimits(bool lock);

    /**
    * isLimitsLock
    *
    * * Reads limit lock status
    */
    bool isLimitsLock(bool &lock);

    /**
    * lockCrit
    *
    * * Locks critical temperature register
    */
    bool lockCrit(bool lock);

    /**
    * isCritLock
    *
    * * Reads critical lock status
    */
    bool isCritLock(bool &lock);

    /**
    * setMode
    *
    * * Sets sensor operating mode
    */
    bool setMode(MCP9808_Mode mode);

    /**
    * getMode
    *
    * * Reads sensor mode
    */
    bool getMode(bool &mode);

    /**
    * setTHyst
    *
    * * Sets hysteresis level
    */
    bool setTHyst(MCP9808_Thyst level);

    /**
    * getTHyst
    *
    * * Reads hysteresis level
    */
    bool getTHyst(uint8_t &level);

    /**
    * setResolution
    *
    * * Sets measurement resolution
    */
    bool setResolution(MCP9808_Resolution res);

    /**
    * getResolution
    *
    * * Reads resolution setting
    */
    bool getResolution(uint8_t &resolution);

    /**
    * getDeviceID
    *
    * * Reads device ID register
    */
    bool getDeviceID(uint8_t &id);

    /**
    * getDeviceRevisionID
    *
    * * Reads device revision ID
    */
    bool getDeviceRevisionID(uint8_t &id);

private:
    /** Communication interface */
    I2C_Interface i2c;

    /** Abstract interface pointer */
    Interface_7Semi *iface = nullptr;

    /** BusIO wrapper */
    BusIO_7Semi<Interface_7Semi> *bus = nullptr;


    /** Temperature resolution factor */
    float resolution = 0.0625;

    /**
     * convertTempToRaw
     *
     * - Converts Celsius to register format
     */
    uint16_t convertTempToRaw(float temp);

    /**
     * convertRawToTemp
     *
     * - Converts register value to Celsius
     */
    float convertRawToTemp(uint16_t raw);

    /**
     * isConfigLock
     *
     * - Checks if configuration is locked
     */
    bool isConfigLock();
};

#endif
