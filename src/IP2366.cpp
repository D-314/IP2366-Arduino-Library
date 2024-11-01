#include "IP2366.h"
#include <Arduino.h>

// Registers

// System Control Registers
#define IP2366_REG_SYS_CTL0 0x00  // Charge enable and other control settings
#define IP2366_REG_SYS_CTL2 0x02  // Vset full-charge voltage setting
#define IP2366_REG_SYS_CTL3 0x03  // Iset charge power or current setting
#define IP2366_REG_SYS_CTL4 0x04  // Battery capacity setting
#define IP2366_REG_SYS_CTL6 0x06  // Trickle charge current, threshold and charge timeout setting
#define IP2366_REG_SYS_CTL8 0x08  // Stop charge current and recharge threshold setting
#define IP2366_REG_SYS_CTL9 0x09  // Standby enable and low battery voltage settings
#define IP2366_REG_SYS_CTL10 0x0A // Low battery voltage setting
#define IP2366_REG_SYS_CTL11 0x0B // Output enable register
#define IP2366_REG_SYS_CTL12 0x0C // Output maximum power selection register

// TYPE-C Control Registers
#define IP2366_REG_SYS_CTL12 0x0C   // Output maximum power selection register
#define IP2366_REG_SELECT_PDO 0x0D  // select charging PDO gear
#define IP2366_REG_TypeC_CTL8 0x22  // TYPE-C mode control register
#define IP2366_REG_TypeC_CTL9 0x23  // Output Pdo current setting register
#define IP2366_REG_TypeC_CTL10 0x24 // 5VPdo current setting register
#define IP2366_REG_TypeC_CTL11 0x25 // 9VPdo current setting register
#define IP2366_REG_TypeC_CTL12 0x26 // 12VPdo current setting register
#define IP2366_REG_TypeC_CTL13 0x27 // 15VPdo current setting register
#define IP2366_REG_TypeC_CTL14 0x28 // 20VPdo current setting register
#define IP2366_REG_TypeC_CTL17 0x2B // Output Pdo setting register
#define IP2366_REG_TypeC_CTL23 0x29 // Pps1 Pdo current setting register
#define IP2366_REG_TypeC_CTL24 0x2A // Pps2 Pdo current setting register
#define IP2366_REG_TypeC_CTL18 0x2C // PDO plus 10mA current enable, needs to be configured together with the current setting register

// Read-only Status Indication Registers
#define IP2366_REG_STATE_CTL0 0x31   // Charge status control register
#define IP2366_REG_STATE_CTL1 0x32   // Charge status control register 2
#define IP2366_REG_STATE_CTL2 0x33   // Input Pd status control register
#define IP2366_REG_TypeC_STATE 0x34  // System status indication register
#define IP2366_REG_RECEIVED_PDO 0x35 // receive PDO gear
#define IP2366_REG_STATE_CTL3 0x38   // System over-current indication register

// ADC Data Registers
#define IP2366_REG_BATVADC_DAT0 0x50         // VBAT voltage low 8 bits
#define IP2366_REG_BATVADC_DAT1 0x51         // VBAT voltage high 8 bits
#define IP2366_REG_VsysVADC_DAT0 0x52        // Vsys voltage low 8 bits
#define IP2366_REG_VsysVADC_DAT1 0x53        // Vsys voltage high 8 bits
#define IP2366_REG_TIMENODE1 0x69            // 1st bit of the timestamp register (the timestamp symbol is an ASCII character)
#define IP2366_REG_TIMENODE2 0x6A            // 2nd bit of the timestamp register (the timestamp symbol is an ASCII character)
#define IP2366_REG_TIMENODE3 0x6B            // 3rd bit of the timestamp register (the timestamp symbol is an ASCII character)
#define IP2366_REG_TIMENODE4 0x6C            // 4th bit of the timestamp register (the timestamp symbol is an ASCII character)
#define IP2366_REG_TIMENODE5 0x6D            // 4th bit of the timestamp register (the timestamp symbol is an ASCII character)
#define IP2366_REG_IBATIADC_DAT0 0x6E        // BAT-end current low 8 bits
#define IP2366_REG_IBATIADC_DAT1 0x6F        // BAT-end current high 8 bits
#define IP2366_REG_ISYS_IADC_DAT0 0x70       // IVsys-end current low 8 bits
#define IP2366_REG_IVsys_IADC_DAT1 0x71      // IVsys-end current high 8 bits
#define IP2366_REG_Vsys_POW_DAT0 0x74        // Vsysterminal power low 8 bits
#define IP2366_REG_Vsys_POW_DAT1 0x75        // Vsysterminal power high 8 bits

// Additional ADC Data Registers for NTC, GPIOs
#define IP2366_REG_INTC_IADC_DAT0 0x77     // NTC output current setting
#define IP2366_REG_VGPIO0_NTC_DAT0 0x78    // VGPIO0_NTC ADC voltage low 8 bits
#define IP2366_REG_VGPIO0_NTC_DAT1 0x79    // VGPIO0_NTC ADC voltage high 8 bits

#define ADC_TO_MV(adc_val) ((uint16_t)((((uint32_t)(adc_val) * 3300) / 0xFFFF)))
#define TwoWire_h
void IP2366::begin()
{
#ifdef TwoWire_h
    Wire.begin();
#endif
}

uint8_t IP2366::writeRegister(uint8_t regAddress, uint8_t value, uint8_t * errorCode)
{
#ifdef TwoWire_h
    Wire.beginTransmission(IP2366_address);
    delay (1);
    Wire.write(regAddress);
    delay (1);
    Wire.write(value);
    delay (1);
    uint8_t _errorCode = Wire.endTransmission(); // Send a stop signal
    delay (1);

    if (_errorCode)
    {
        if (errorCode != nullptr)
        {
            *errorCode = _errorCode; // write error code only if it > 0
        }
        return -1;
    }
    return 0;
#else
    return -1;
#endif
}

uint8_t IP2366::readRegister(uint8_t regAddress, uint8_t * errorCode)
{
#ifdef TwoWire_h
    Wire.beginTransmission(IP2366_address);
    delay (1); // increase the delay by 1ms between each byte
    Wire.write(regAddress);
    delay (1); // increase the delay by 1ms between each byte
    uint8_t _errorCode = Wire.endTransmission(false); // Do not send a stop signal
    delay (1);
    
    if (_errorCode)
    {
        if (errorCode != nullptr)
        {
            *errorCode = _errorCode; // write error code only if it > 0
        }
        return -1;
    }

    uint8_t bytesRead = Wire.requestFrom(IP2366_address, (uint8_t)1, (bool)true); // Request 1 byte and send a stop signal

    if (bytesRead != 1)
    {
        if (errorCode != nullptr)
        {
            *errorCode = _errorCode; // write error code only if it > 0
        }
        return 0;
    }
    return Wire.read(); //read from I2C internal buffer
#else
    return -1;
#endif
}

uint8_t IP2366::setBit(uint8_t value, uint8_t bit, bool enable)
{
     return (enable) ? (value |  (1 << bit)) : (value & ~(1 << bit));
}

// SYS_CTL0

void IP2366::enableCharger(bool enable, uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    uint8_t value = readRegister(IP2366_REG_SYS_CTL0, errorCode);
    value = setBit(value, 0, enable);
    writeRegister(IP2366_REG_SYS_CTL0, value, errorCode);
}

bool IP2366::isChargerEnabled(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_SYS_CTL0, errorCode) & 0x01;
}

void IP2366::enableVbusSinkSCP(bool enable, uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    uint8_t value = readRegister(IP2366_REG_SYS_CTL0, errorCode);
    value = setBit(value, 2, enable);
    writeRegister(IP2366_REG_SYS_CTL0, value, errorCode);
}

bool IP2366::isVbusSinkSCPEnabled(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_SYS_CTL0, errorCode) & 0x04;
}

void IP2366::enableVbusSinkPD(bool enable, uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    uint8_t value = readRegister(IP2366_REG_SYS_CTL0, errorCode);
    value = setBit(value, 3, enable);
    writeRegister(IP2366_REG_SYS_CTL0, value, errorCode);
}

bool IP2366::isVbusSinkPDEnabled(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_SYS_CTL0, errorCode) & 0x08;
}

void IP2366::enableVbusSinkDPdM(bool enable, uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    uint8_t value = readRegister(IP2366_REG_SYS_CTL0, errorCode);
    value = setBit(value, 4, enable);
    writeRegister(IP2366_REG_SYS_CTL0, value, errorCode);
}

bool IP2366::isVbusSinkDPdMEnabled(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_SYS_CTL0, errorCode) & 0x10;
}

void IP2366::enableINTLow(bool enable, uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    uint8_t value = readRegister(IP2366_REG_SYS_CTL0, errorCode);
    value = setBit(value, 5, enable);
    writeRegister(IP2366_REG_SYS_CTL0, value, errorCode);
}

bool IP2366::isINTLowEnabled(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_SYS_CTL0, errorCode) & 0x20;
}

void IP2366::ResetMCU(bool enable, uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    uint8_t value = readRegister(IP2366_REG_SYS_CTL0, errorCode);
    value = setBit(value, 6, enable);
    writeRegister(IP2366_REG_SYS_CTL0, value, errorCode);
}

void IP2366::enableLoadOTP(bool enable, uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    uint8_t value = readRegister(IP2366_REG_SYS_CTL0, errorCode);
    value = setBit(value, 7, enable);
    writeRegister(IP2366_REG_SYS_CTL0, value, errorCode);
}

bool IP2366::isLoadOTPEnabled(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_SYS_CTL0, errorCode) & 0x80;
}

// SYS_CTL2

void IP2366::setFullChargeVoltage(uint16_t voltage, uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    uint16_t minVoltage, maxVoltage;
    minVoltage = 2500;
    maxVoltage = 4400;

    if (voltage < minVoltage)
        voltage = minVoltage;
    else if (voltage > maxVoltage)
        voltage = maxVoltage;

    uint8_t regValue = (voltage - minVoltage) / 10;
    writeRegister(IP2366_REG_SYS_CTL2, regValue, errorCode);
}

uint16_t IP2366::getFullChargeVoltage(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    uint8_t regValue = readRegister(IP2366_REG_SYS_CTL2, errorCode);

    uint16_t baseVoltage = 2500;

    return baseVoltage + regValue * 10;
}

// SYS_CTL3

void IP2366::setMaxInputPowerOrBatteryCurrent(uint16_t current_mA, uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    uint8_t regValue;
    uint16_t maxCurrent = 9700;
    if (current > maxCurrent)
        current = maxCurrent;

    regValue = current / 100;

    writeRegister(IP2366_REG_SYS_CTL3, regValue, errorCode);
}

uint16_t IP2366::getMaxInputPowerOrBatteryCurrent(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    uint8_t regValue = readRegister(IP2366_REG_SYS_CTL3, errorCode);

    return regValue * 100;
}

// SYS_CTL6

void IP2366::setTrickleChargeCurrent(uint16_t current, uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    uint8_t regValue = current / 50;
    writeRegister(IP2366_REG_SYS_CTL6, regValue, errorCode);
}

uint16_t IP2366::getTrickleChargeCurrent(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    uint8_t regValue = readRegister(IP2366_REG_SYS_CTL6, errorCode);
    return static_cast<uint16_t>(regValue * 50);
}

// SYS_CTL8

void IP2366::setChargeStopCurrent(uint16_t current, uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    if (current > 750)
        current = 750;
    uint8_t regValue = current / 50;

    uint8_t currentValue = readRegister(IP2366_REG_SYS_CTL8, errorCode) & 0x0F;
    writeRegister(IP2366_REG_SYS_CTL8, currentValue | (regValue << 4), errorCode);
}

uint16_t IP2366::getChargeStopCurrent(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    uint8_t value = (readRegister(IP2366_REG_SYS_CTL8, errorCode) >> 4) & 0x0F;
    return value * 50;
}

void IP2366::setCellRechargeThreshold(uint16_t voltageDrop_mV, uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    uint8_t regValue = 0;

    if (voltageDrop_mV >= 400)
    {
        regValue = 3;
    }
    else
    {
        regValue = voltageDrop_mV / 50;
    }

    uint8_t currentValue = readRegister(IP2366_REG_SYS_CTL8, errorCode) & 0xF3;
    writeRegister(IP2366_REG_SYS_CTL8, currentValue | (regValue << 2), errorCode);
}

uint16_t IP2366::getCellRechargeThreshold(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    uint8_t value = (readRegister(IP2366_REG_SYS_CTL8, errorCode) >> 2) & 0x03;
    return (value * 50);
}

// SYS_CTL9

void IP2366::enableStandbyMode(bool enable, uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    uint8_t value = readRegister(IP2366_REG_SYS_CTL9, errorCode);
    value = setBit(value, 7, enable);
    writeRegister(IP2366_REG_SYS_CTL9, value, errorCode);
}

bool IP2366::isStandbyModeEnabled(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_SYS_CTL9, errorCode) & (1 << 7);
}

void IP2366::Standby(bool enable, uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    uint8_t value = readRegister(IP2366_REG_SYS_CTL9, errorCode);
    value = setBit(value, 6, enable);
    writeRegister(IP2366_REG_SYS_CTL9, value, errorCode);
}

bool IP2366::isStandby(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_SYS_CTL9, errorCode) & (1 << 6);
}

void IP2366::enableBATLow(bool enable, uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    uint8_t value = readRegister(IP2366_REG_SYS_CTL9, errorCode);
    value = setBit(value, 5, enable);
    writeRegister(IP2366_REG_SYS_CTL9, value, errorCode);
}

bool IP2366::isBATLowEnabled(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_SYS_CTL9, errorCode) & (1 << 5);
}

// SYS_CTL10

void IP2366::setLowBatteryVoltage(uint16_t voltage_mV, uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    uint8_t voltageSetting;
    uint16_t stepVoltage = 100;

    uint16_t baseVoltage = 2500;

    if (voltage_mV < baseVoltage)
    {
        voltage_mV = baseVoltage;
    }
    else if (voltage_mV > (baseVoltage + stepVoltage * 7))
    {
        voltage_mV = (baseVoltage + stepVoltage * 7);
    }

    voltageSetting = (voltage_mV - baseVoltage) / stepVoltage;

    uint8_t currentValue = readRegister(IP2366_REG_SYS_CTL10, errorCode) & 0x1F;
    writeRegister(IP2366_REG_SYS_CTL10, (voltageSetting << 5 | currentValue), errorCode);
}

uint16_t IP2366::getLowBatteryVoltage(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    uint8_t voltageSetting = (readRegister(IP2366_REG_SYS_CTL10, errorCode) >> 5) & 0x03;

    uint16_t baseVoltage = 2500;
    return baseVoltage + voltageSetting * 100;
}

// SYS_CTL11

void IP2366::setOutputFeatures(bool enableDcDcOutput, bool enableVbusSrcDPdM, bool enableVbusSrcPd, bool enableVbusSrcSCP, uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    uint8_t currentValue = readRegister(IP2366_REG_SYS_CTL11, errorCode) & 0x0F;
    currentValue |= (enableDcDcOutput << 7) | (enableVbusSrcDPdM << 6) | (enableVbusSrcPd << 5) | (enableVbusSrcSCP << 4);
    writeRegister(IP2366_REG_SYS_CTL11, currentValue, errorCode);
}

bool IP2366::isDcDcOutputEnabled(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_SYS_CTL11, errorCode) & 0x80;
}

bool IP2366::isVbusSrcDPdMEnabled(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_SYS_CTL11, errorCode) & 0x40;
}

bool IP2366::isVbusSrcPdEnabled(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_SYS_CTL11, errorCode) & 0x20;
}

bool IP2366::isVbusSrcSCPEnabled(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_SYS_CTL11, errorCode) & 0x10;
}

// SYS_CTL12

void IP2366::setMaxOutputPower(Vbus1OutputPower power, uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    uint8_t value = readRegister(IP2366_REG_SYS_CTL12, errorCode) & 0x1F; 
    value |= static_cast<uint8_t>(power) << 5;                 
    writeRegister(IP2366_REG_SYS_CTL12, value, errorCode);
}

IP2366::Vbus1OutputPower IP2366::getMaxOutputPower(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return static_cast<Vbus1OutputPower>((readRegister(IP2366_REG_SYS_CTL12, errorCode) >> 5) & 0x03); 
}

// SELECT_PDO

void IP2366::setChargingPDOmode(ChargingPDOmode mode, uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    uint8_t currentValue = readRegister(IP2366_REG_SYS_CTL10, errorCode) & 0xF8;
    writeRegister(IP2366_REG_SELECT_PDO, ((static_cast<uint8_t>(mode) | currentValue), errorCode);
}

IP2366::ChargingPDOmode IP2366::getChargingPDOmode(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return static_cast<ChargingPDOmode>(readRegister(IP2366_REG_SELECT_PDO, errorCode) & 0x03);
}

// TypeC_CTL8

void IP2366::setTypeCMode(TypeCMode mode, uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    uint8_t value = readRegister(IP2366_REG_TypeC_CTL8, errorCode) & ~0xC0; 

    value |= (static_cast<uint8_t>(mode) << 6); 

    writeRegister(IP2366_REG_TypeC_CTL8, value, errorCode);
}

IP2366::TypeCMode IP2366::getTypeCMode(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return static_cast<TypeCMode>(readRegister(IP2366_REG_TypeC_CTL8, errorCode) >> 6); 
}

// TypeC_CTL9

void IP2366::enablePdoCurrentOutputSet(bool en5VPdoIset, bool en5VPdo3A, bool en9VPdoIset,
                                       bool en12VPdoIset, bool en15VPdoIset, bool en20VPdoIset,
                                       bool enPps1PdoIset, bool enPps2PdoIset, uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    uint8_t value = 0;
    value = setBit(value, 7, en5VPdo3A);
    value = setBit(value, 6, enPps2PdoIset);
    value = setBit(value, 5, enPps1PdoIset);
    value = setBit(value, 4, en20VPdoIset);
    value = setBit(value, 3, en15VPdoIset);
    value = setBit(value, 2, en12VPdoIset);
    value = setBit(value, 1, en9VPdoIset);
    value = setBit(value, 0, en5VPdoIset);
    writeRegister(IP2366_REG_TypeC_CTL9, value, errorCode);
}

bool IP2366::is5VPdo3AEnabled(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_TypeC_CTL9, errorCode) & (1 << 7);
}

bool IP2366::isPps2PdoIsetEnabled(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_TypeC_CTL9, errorCode) & (1 << 6);
}

bool IP2366::isPps1PdoIsetEnabled(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_TypeC_CTL9, errorCode) & (1 << 5);
}

bool IP2366::is20VPdoIsetEnabled(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_TypeC_CTL9, errorCode) & (1 << 4);
}

bool IP2366::is15VPdoIsetEnabled(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_TypeC_CTL9, errorCode) & (1 << 3);
}

bool IP2366::is12VPdoIsetEnabled(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_TypeC_CTL9, errorCode) & (1 << 2);
}

bool IP2366::is9VPdoIsetEnabled(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_TypeC_CTL9, errorCode) & (1 << 1);
}

bool IP2366::is5VPdoIsetEnabled(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_TypeC_CTL9, errorCode) & (1 << 0);
}

void IP2366::writeTypeCCurrentSetting(uint8_t reg, uint16_t current_mA, uint16_t step, uint16_t maxCurrent, uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    if (current_mA > maxCurrent)
        current_mA = maxCurrent;
    uint8_t value = current_mA / step;
    writeRegister(reg, value, errorCode);
}

// TypeC_CTL10 - TypeC_CTL14

void IP2366::setPDOCurrent5V(uint16_t current_mA, uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    writeTypeCCurrentSetting(IP2366_REG_TypeC_CTL10, current_mA, 20, 3000, errorCode);
}

uint16_t IP2366::getPDOCurrent5V(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_TypeC_CTL10, errorCode) * 20;
}

void IP2366::setPDOCurrent9V(uint16_t current_mA, uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    writeTypeCCurrentSetting(IP2366_REG_TypeC_CTL11, current_mA, 20, 3000, errorCode);
}

uint16_t IP2366::getPDOCurrent9V(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_TypeC_CTL11, errorCode) * 20;
}

void IP2366::setPDOCurrent12V(uint16_t current_mA, uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    writeTypeCCurrentSetting(IP2366_REG_TypeC_CTL12, current_mA, 20, 3000, errorCode);
}

uint16_t IP2366::getPDOCurrent12V(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_TypeC_CTL12, errorCode) * 20;
}

void IP2366::setPDOCurrent15V(uint16_t current_mA, uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    writeTypeCCurrentSetting(IP2366_REG_TypeC_CTL13, current_mA, 20, 3000, errorCode);
}

uint16_t IP2366::getPDOCurrent15V(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_TypeC_CTL13, errorCode) * 20;
}

void IP2366::setPDOCurrent20V(uint16_t current_mA, uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    writeTypeCCurrentSetting(IP2366_REG_TypeC_CTL14, current_mA, 20, 5000, errorCode);
}

uint16_t IP2366::getPDOCurrent20V(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_TypeC_CTL14, errorCode) * 20;
}

// TypeC_CTL23 - TypeC_CTL24

void IP2366::setPDOCurrentPPS1(uint16_t current_mA, uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    writeTypeCCurrentSetting(IP2366_REG_TypeC_CTL23, current_mA, 50, 5000, errorCode);
}

uint16_t IP2366::getPDOCurrentPPS1(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_TypeC_CTL23, errorCode) * 50;
}

void IP2366::setPDOCurrentPPS2(uint16_t current_mA, uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    writeTypeCCurrentSetting(IP2366_REG_TypeC_CTL24, current_mA, 50, 5000, errorCode);
}

uint16_t IP2366::getPDOCurrentPPS2(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_TypeC_CTL24, errorCode) * 50;
}

// TypeC_CTL17

void IP2366::enableSrcPdo(bool en9VPdo, bool en12VPdo, bool en15VPdo, bool en20VPdo, bool enPps1Pdo, bool enPps2Pdo, uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    uint8_t value = readRegister(IP2366_REG_TypeC_CTL17, errorCode);
    value = setBit(value, 6, enPps2Pdo);
    value = setBit(value, 5, enPps1Pdo);
    value = setBit(value, 4, en20VPdo);
    value = setBit(value, 3, en15VPdo);
    value = setBit(value, 2, en12VPdo);
    value = setBit(value, 1, en9VPdo);
    writeRegister(IP2366_REG_TypeC_CTL17, value, errorCode);
}

bool IP2366::isSrcPdo9VEnabled(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_TypeC_CTL17, errorCode) & (1 << 1);
}

bool IP2366::isSrcPdo12VEnabled(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_TypeC_CTL17, errorCode) & (1 << 2);
}

bool IP2366::isSrcPdo15VEnabled(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_TypeC_CTL17, errorCode) & (1 << 3);
}

bool IP2366::isSrcPdo20VEnabled(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_TypeC_CTL17, errorCode) & (1 << 4);
}

bool IP2366::isSrcPps1PdoEnabled(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_TypeC_CTL17, errorCode) & (1 << 5);
}

bool IP2366::isSrcPps2PdoEnabled(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_TypeC_CTL17, errorCode) & (1 << 6);
}

// TypeC_CTL18

void IP2366::enableSrcPdoAdd10mA(bool en5VPdoAdd10mA, bool en9VPdoAdd10mA, bool en12VPdoAdd10mA, bool en15VPdoAdd10mA, bool en20VPdoAdd10mA, uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    uint8_t value = readRegister(IP2366_REG_TypeC_CTL18, errorCode);
    value = setBit(value, 4, en20VPdoAdd10mA);
    value = setBit(value, 3, en15VPdoAdd10mA);
    value = setBit(value, 2, en12VPdoAdd10mA);
    value = setBit(value, 1, en9VPdoAdd10mA);
    value = setBit(value, 0, en5VPdoAdd10mA);
    writeRegister(IP2366_REG_TypeC_CTL18, value, errorCode);
}

bool IP2366::isSrcPdoAdd10mA5VEnabled(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_TypeC_CTL18, errorCode) & (1 << 0);
}

bool IP2366::isSrcPdoAdd10mA9VEnabled(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_TypeC_CTL18, errorCode) & (1 << 1);
}

bool IP2366::isSrcPdoAdd10mA12VEnabled(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_TypeC_CTL18, errorCode) & (1 << 2);
}

bool IP2366::isSrcPdoAdd10mA15VEnabled(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_TypeC_CTL18, errorCode) & (1 << 3);
}

bool IP2366::isSrcPdoAdd10mA20VEnabled(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_TypeC_CTL18, errorCode) & (1 << 4);
}


// STATE_CTL0

bool IP2366::isCharging(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_STATE_CTL0, errorCode) & (1 << 5);
}

bool IP2366::isChargeFull(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_STATE_CTL0, errorCode) & (1 << 4);
}

bool IP2366::isDischarging(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_STATE_CTL0, errorCode) & (1 << 3);
}

IP2366::ChargeState IP2366::getChargeState(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    uint8_t data = readRegister(IP2366_REG_STATE_CTL0, errorCode);
    uint8_t stateBits = data & 0x07;
    return static_cast<ChargeState>(stateBits);
}

// STATE_CTL1

bool IP2366::isFastCharge(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_STATE_CTL1, errorCode) & (1 << 6);
}

// STATE_CTL2

bool IP2366::isVbusPresent(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_STATE_CTL2, errorCode) & (1 << 7);
}

bool IP2366::isVbusOvervoltage(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_STATE_CTL2, errorCode) & (1 << 6);
}

uint8_t IP2366::getChargeVoltage(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    uint8_t value = readRegister(IP2366_REG_STATE_CTL2, errorCode) & 0x07;
    switch (value)
    {
    case 0x07:
        value = 20;
        break;

    case 0x06:
        value = 15;
        break;

    case 0x05:
        value = 12;
        break;

    case 0x04:
        value = 9;
        break;

    case 0x03:
        value = 7;
        break;

    case 0x02:
        value = 5;
        break;
    default:
        value = 0;
        break;
    }
    return value;
}

// TypeC_STATE

bool IP2366::isTypeCSinkConnected(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_TypeC_STATE, errorCode) & (1 << 7);
}

bool IP2366::isTypeCSrcConnected(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_TypeC_STATE, errorCode) & (1 << 6);
}

bool IP2366::isTypeCSrcPdConnected(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_TypeC_STATE, errorCode) & (1 << 5);
}

bool IP2366::isTypeCSinkPdConnected(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_TypeC_STATE, errorCode) & (1 << 4);
}

bool IP2366::isVbusSinkQcActive(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_TypeC_STATE, errorCode) & (1 << 3);
}

bool IP2366::isVbusSrcQcActive(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_TypeC_STATE, errorCode) & (1 << 2);
}

// RECEIVED_PDO

bool IP2366::isReceives5VPdo(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_RECEIVED_PDO, errorCode) & (1 << 0);
}

bool IP2366::isReceives9VPdo(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_RECEIVED_PDO, errorCode) & (1 << 1);
}

bool IP2366::isReceives12VPdo(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_RECEIVED_PDO, errorCode) & (1 << 2);
}

bool IP2366::isReceives15VPdo(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_RECEIVED_PDO, errorCode) & (1 << 3);
}

bool IP2366::isReceives20VPdo(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_RECEIVED_PDO, errorCode) & (1 << 4);
}

// STATE_CTL3

bool IP2366::isVsysOverCurrent(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_STATE_CTL3, errorCode) & (1 << 5);
}

bool IP2366::isVsysSdortCircuitDt(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return readRegister(IP2366_REG_STATE_CTL3, errorCode) & (1 << 4);
}

// TIMENODE

void IP2366::getTimenode(char timenode[5], uint8_t * errorCode) {
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    for (uint8_t i = 0; i < 5; i++)
    {
        timenode[i] = readRegister((IP2366_REG_TIMENODE1+i), errorCode);
    }
}

// ADC

uint16_t IP2366::getVBATVoltage(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return (((uint16_t)readRegister(IP2366_REG_BATVADC_DAT1, errorCode) << 8) | (uint16_t)readRegister(IP2366_REG_BATVADC_DAT0, errorCode));
}

uint16_t IP2366::getVsysVoltage(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return (((uint16_t)readRegister(IP2366_REG_VsysVADC_DAT1, errorCode) << 8) | (uint16_t)readRegister(IP2366_REG_VsysVADC_DAT0, errorCode));
}

uint16_t IP2366::getBATCurrent(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return (((uint16_t)readRegister(IP2366_REG_IBATIADC_DAT1, errorCode) << 8) | (uint16_t)readRegister(IP2366_REG_IBATIADC_DAT0, errorCode));
}

uint16_t IP2366::getVsysCurrent(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return (((uint16_t)readRegister(IP2366_REG_IVsys_IADC_DAT1, errorCode) << 8) | (uint16_t)readRegister(IP2366_REG_ISYS_IADC_DAT0, errorCode));
}

uint32_t IP2366::getVsysPower(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return (((uint32_t)readRegister(IP2366_REG_Vsys_POW_DAT1, errorCode) << 8) | (uint32_t)readRegister(IP2366_REG_Vsys_POW_DAT0, errorCode));
}

bool IP2366::isOverHeat(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return (readRegister(IP2366_REG_INTC_IADC_DAT0, errorCode) & (1 << 7));
}

// GPIO aka ADC raw readings

uint16_t IP2366::getNTCVoltage(uint8_t * errorCode)
{
    if (errorCode != nullptr) *errorCode = 0; // reset error code
    return ADC_TO_MV(((uint16_t)readRegister(IP2366_REG_VGPIO0_NTC_DAT1, errorCode) << 8) | (uint16_t)readRegister(IP2366_REG_VGPIO0_NTC_DAT0, errorCode));
}
