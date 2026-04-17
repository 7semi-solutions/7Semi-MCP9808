#include "7Semi_MCP9808.h"

MCP9808_7Semi::MCP9808_7Semi()
{
}

bool MCP9808_7Semi::beginI2C(uint8_t i2cAddress,
                             TwoWire &i2cPort,
                             uint32_t i2cClock)
{
    if (bus)
    {
        delete bus;
        bus = nullptr;
    }

    iface = &i2c;

    if (!i2c.beginI2C(i2cAddress, i2cPort, i2cClock))
        return false;

    bus = new BusIO_7Semi<Interface_7Semi>(*iface);

    if (!bus)
        return false;

    uint16_t id;
    if (!getManufacturerID(id))
        return false;

    if (id != MCP9808_EXPECTED_ID)
        return false;

    delay(100);

    return true;
}

bool MCP9808_7Semi::getManufacturerID(uint16_t &id)
{
    // Read chip ID register
    return bus->read(MCP9808_REG_MANUF_ID, id);
}

bool MCP9808_7Semi::getTemperatureRaw(uint16_t &rawTemperature)
{

    if (!bus->read(MCP9808_REG_TAMBIENT, rawTemperature))
        return false;

    return true;
}

bool MCP9808_7Semi::getTemperature(float &temperature)
{
    uint16_t raw_temperature;

    if (!getTemperatureRaw(raw_temperature))
        return false;

    // Extract temperature (lower 12 bits)
    float temp = raw_temperature & 0x0FFF;

    temp *= resolution;

    // Handle negative temperature
    if (raw_temperature & 0x1000)
        temp -= 256.0f;

    temperature = temp;
    return true;
}

bool MCP9808_7Semi::getAlert(bool &isUpperTemp,
                             bool &isLowerTemp,
                             bool &isCriticalTemp)
{
    uint16_t raw;

    if (!getTemperatureRaw(raw))
        return false;

    isCriticalTemp = (raw & 0x8000) != 0;
    isUpperTemp = (raw & 0x4000) != 0;
    isLowerTemp = (raw & 0x2000) != 0;

    return true;
}

uint16_t MCP9808_7Semi::convertTempToRaw(float temp)
{
    if (temp < -40.0f) temp = -40.0f;
    if (temp > 125.0f) temp = 125.0f;

    int16_t value = (int16_t)(temp / 0.25f);

    uint16_t raw;

    if (value < 0)
    {
        raw = (1 << 12) | (((uint16_t)(-value) & 0x07FF) << 2);
    }
    else
    {
        raw = ((uint16_t)value & 0x07FF) << 2;
    }

    return raw;
}

float MCP9808_7Semi::convertRawToTemp(uint16_t raw)
{
    int16_t value = (raw >> 2) & 0x07FF;

    float temp = value * 0.25f;

    if (raw & 0x1000) // sign bit
        temp -= 256.0f;

    return temp;
}

bool MCP9808_7Semi::setUpperTemperature(float upperTemp)
{
    uint16_t raw = convertTempToRaw(upperTemp);
    return bus->write(MCP9808_REG_TUPPER, raw);
}

bool MCP9808_7Semi::getUpperTemperature(float &upperTemp)
{
    uint16_t raw;

    if (!bus->read(MCP9808_REG_TUPPER, raw))
        return false;

    upperTemp = convertRawToTemp(raw);
    return true;
}

bool MCP9808_7Semi::setLoweTemperature(float lowerTemp)
{
    uint16_t raw = convertTempToRaw(lowerTemp);
    return bus->write(MCP9808_REG_TLOWER, raw);
}

bool MCP9808_7Semi::getLoweTemperature(float &lowerTemp)
{
    uint16_t raw;

    if (!bus->read(MCP9808_REG_TLOWER, raw))
        return false;

    lowerTemp = convertRawToTemp(raw);
    return true;
}

bool MCP9808_7Semi::setCriticalTemperature(float criticalTemp)
{
    uint16_t raw = convertTempToRaw(criticalTemp);
    return bus->write(MCP9808_REG_TCRIT, raw);
}

bool MCP9808_7Semi::getCriticalTemperature(float &criticalTemp)
{
    uint16_t raw;

    if (!bus->read(MCP9808_REG_TCRIT, raw))
        return false;

    criticalTemp = convertRawToTemp(raw);
    return true;
}

bool MCP9808_7Semi::setAlertMode(MCP9808_AlertMode mode)
{
    if (isConfigLock())
        return false;

    uint16_t v = 0;
    v = mode;
    return bus->writeBit(MCP9808_REG_CONFIG, 0, (uint16_t)(mode ? 1 : 0));
}

bool MCP9808_7Semi::getAlertMode(bool &mode)
{
    uint16_t v = 0;
    if (!bus->readBit(MCP9808_REG_CONFIG, 0, v))
        return false;

    mode = (v == 1);
    return true;
}

bool MCP9808_7Semi::setAlertPolarity(MCP9808_AlertPlarity polarity)
{
    if (isConfigLock())
        return false;
    return bus->writeBit(MCP9808_REG_CONFIG, 1, (uint16_t)polarity);
}

bool MCP9808_7Semi::getAlertPolarity(bool &polarity)
{
    uint16_t v = 0;
    if (!bus->readBit(MCP9808_REG_CONFIG, 1, v))
        return false;

    polarity = (v == 1);
    return true;
}

bool MCP9808_7Semi::setAlertSelect(MCP9808_AlertSelect onlyCrit)
{
    bool limit_locked = false;
    if (!lockLimits(limit_locked))
        return false;

    if (limit_locked)
        return false;

    return bus->writeBit(MCP9808_REG_CONFIG, 2, (uint16_t)onlyCrit);
}

bool MCP9808_7Semi::getAlertSelect(bool &onlyCrit)
{
    uint16_t v = 0;
    if (!bus->readBit(MCP9808_REG_CONFIG, 2, v))
        return false;

    onlyCrit = (v == 1);
    return true;
}

bool MCP9808_7Semi::enableAlert(bool enable)
{
    if (isConfigLock())
        return false;

    return bus->writeBit(MCP9808_REG_CONFIG, 3, (uint16_t)enable);
}

bool MCP9808_7Semi::isEnableAlert(bool &enable)
{
    uint16_t v = 0;
    if (!bus->readBit(MCP9808_REG_CONFIG, 3, v))
        return false;

    enable = (v == 1);
}

bool MCP9808_7Semi::getAlertStatus(bool &alert)
{
    uint16_t v = 0;
    if (!bus->readBit(MCP9808_REG_CONFIG, 4, v))
        return false;

    alert = (v == 1);
}

bool MCP9808_7Semi::clearAlert()
{
    return bus->writeBit(MCP9808_REG_CONFIG, 5, (uint16_t)true);
}

bool MCP9808_7Semi::lockLimits(bool lock)
{
    return bus->writeBit(MCP9808_REG_CONFIG, 6, (uint16_t)lock);
}

bool MCP9808_7Semi::isLimitsLock(bool &lock)
{
    uint16_t v = 0;
    if (!bus->readBit(MCP9808_REG_CONFIG, 6, v))
        return false;

    lock = (v == 1);

    return true;
}

bool MCP9808_7Semi::lockCrit(bool lock)
{
    return bus->writeBit(MCP9808_REG_CONFIG, 7, (uint16_t)lock);
}

bool MCP9808_7Semi::isCritLock(bool &lock)
{
    uint16_t v = 0;
    if (!bus->readBit(MCP9808_REG_CONFIG, 7, v))
        return false;

    lock = (v == 1);

    return true;
}

bool MCP9808_7Semi::setMode(MCP9808_Mode mode)
{
    if (mode == SHUTDOWN_MODE)
    {
        if (isConfigLock())
            return false;
    }
    return bus->writeBit(MCP9808_REG_CONFIG, 8, (uint16_t)mode);
}

bool MCP9808_7Semi::getMode(bool &mode)
{
    uint16_t v = 0;
    if (!bus->readBit(MCP9808_REG_CONFIG, 8, v))
        return false;

    mode = (v == 1);
    return true;
}

bool MCP9808_7Semi::setTHyst(MCP9808_Thyst level)
{
    if (level > MCP9808_THYST_6C)
        return false;

    return bus->writeBits(MCP9808_REG_CONFIG, 9, 2, (uint16_t)level);
}

bool MCP9808_7Semi::getTHyst(uint8_t &level)
{
    return bus->readBits(MCP9808_REG_CONFIG, 9, 2, level);
}

bool MCP9808_7Semi::setResolution(MCP9808_Resolution res)
{
    if (res > MCP9808_RES_0_0625C)
        return false;

    if (!bus->write(MCP9808_REG_RESOLUTION, (uint16_t)res))
        return false;

    switch (res)
    {
    case 0:
        resolution = 0.5f;
        break;
    case 1:
        resolution = 0.25f;
        break;
    case 2:
        resolution = 0.125f;
        break;
    case 3:
        resolution = 0.0625f;
        break;

    default:
        resolution = 0.0625f;
        break;
    }

    return true;
}

bool MCP9808_7Semi::getResolution(uint8_t &resolution)
{
    uint8_t v = 0;
    if (!bus->read(MCP9808_REG_RESOLUTION, v))
        return false;

    resolution = v & 0x3;
    return true;
}

bool MCP9808_7Semi::isConfigLock()
{
    bool limit_locked = true;
    bool crit_locked = true;
    if (!isLimitsLock(limit_locked))
        return true;

    if (!isCritLock(crit_locked))
        return true;

    if (crit_locked | limit_locked)
        return true;

    return false;
}

bool MCP9808_7Semi::getDeviceID(uint8_t &id)
{
    uint16_t v = 0;

    if (!bus->read(MCP9808_REG_DEVICE_ID, v))
        return false;

    id = (v >> 8) & 0xFF;
    return true;
}

bool MCP9808_7Semi::getDeviceRevisionID(uint8_t &id)
{
    uint16_t v = 0;

    if (!bus->read(MCP9808_REG_DEVICE_ID, v))
        return false;

    id = v & 0xFF;
    return true;
}