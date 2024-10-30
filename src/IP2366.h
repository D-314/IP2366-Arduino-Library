#ifndef IP2366_H
#define IP2366_H

// #include <Arduino.h>
#include <Wire.h>

#include <stdint.h>

class IP2366
{
public:
    IP2366(uint8_t address = 0x75) : IP2366_address(address) {};
    void begin();

    uint8_t IP2366_address;

    // Enumeration for defining the charge state
    enum class ChargeState
    {
        STANDBY = 0,
        TRICKLE_CHARGE = 1,
        CONSTANT_CURRENT = 2,
        CONSTANT_VOLTAGE = 3,
        CHARGE_WAIT = 4,
        CHARGE_FULL = 5,
        CHARGE_TIMEOUT = 6
    };

    // Enumeration for defining the USB Type-C operating mode
    enum class TypeCMode
    {
        UFP = 0, // Upstream Facing Port (device connected as a consumer)
        DFP = 1, // Downstream Facing Port (device connected as a power source)
        DRP = 3  // Dual Role Port (device can act as both a source and a consumer)
    };

    // Enumeration for selecting Vbus1 output power
    enum class Vbus1OutputPower
    {
        W30 = 0,
        W45 = 1,
        W60 = 2,
        W65 = 3,
        W100 = 4,
        W140 = 5
    };

    // Enumeration for selecting charging PDO mode
    enum class ChargingPDOmode
    {
        V5 = 0,
        V9 = 1,
        V12 = 2,
        V15 = 3,
        V20 = 4
    };

    // Enumeration for setting the charge timeout
    enum class ChargeTimeout
    {
        Disabled = 0,
        H24 = 1,
        H36 = 2,
        H48 = 3
    };


    ///////// IS? ////////

    // SYS_CTL0

    bool isChargerEnabled(uint8_t * errorCode = nullptr);
    bool isVbusSinkSCPEnabled(uint8_t * errorCode = nullptr);
    bool isVbusSinkPDEnabled(uint8_t * errorCode = nullptr);
    bool isVbusSinkDPdMEnabled(uint8_t * errorCode = nullptr);
    bool isINTLowEnabled(uint8_t * errorCode = nullptr);
    bool isLoadOTPEnabled(uint8_t * errorCode = nullptr);

    // SYS_CTL9
    bool isStandbyModeEnabled(uint8_t * errorCode = nullptr);
    bool isStandby(uint8_t * errorCode = nullptr);
    bool isBATLowEnabled(uint8_t * errorCode = nullptr);

    // SYS_CTL11

    bool isDcDcOutputEnabled(uint8_t * errorCode = nullptr);
    bool isVbusSrcDPdMEnabled(uint8_t * errorCode = nullptr);
    bool isVbusSrcPdEnabled(uint8_t * errorCode = nullptr);
    bool isVbusSrcSCPEnabled(uint8_t * errorCode = nullptr);

    // TypeC_CTL9

    bool is5VPdo3AEnabled(uint8_t * errorCode = nullptr);
    bool isPps2PdoIsetEnabled(uint8_t * errorCode = nullptr);
    bool isPps1PdoIsetEnabled(uint8_t * errorCode = nullptr);
    bool is20VPdoIsetEnabled(uint8_t * errorCode = nullptr);
    bool is15VPdoIsetEnabled(uint8_t * errorCode = nullptr);
    bool is12VPdoIsetEnabled(uint8_t * errorCode = nullptr);
    bool is9VPdoIsetEnabled(uint8_t * errorCode = nullptr);
    bool is5VPdoIsetEnabled(uint8_t * errorCode = nullptr);

    // TypeC_CTL17

    bool isSrcPdo9VEnabled(uint8_t * errorCode = nullptr);
    bool isSrcPdo12VEnabled(uint8_t * errorCode = nullptr);
    bool isSrcPdo15VEnabled(uint8_t * errorCode = nullptr);
    bool isSrcPdo20VEnabled(uint8_t * errorCode = nullptr);
    bool isSrcPps1PdoEnabled(uint8_t * errorCode = nullptr);
    bool isSrcPps2PdoEnabled(uint8_t * errorCode = nullptr);

    // TypeC_CTL18

    bool isSrcPdoAdd10mA5VEnabled(uint8_t * errorCode = nullptr);
    bool isSrcPdoAdd10mA9VEnabled(uint8_t * errorCode = nullptr);
    bool isSrcPdoAdd10mA12VEnabled(uint8_t * errorCode = nullptr);
    bool isSrcPdoAdd10mA15VEnabled(uint8_t * errorCode = nullptr);
    bool isSrcPdoAdd10mA20VEnabled(uint8_t * errorCode = nullptr);

    // STATE_CTL0

    bool isCharging(uint8_t * errorCode = nullptr);
    bool isChargeFull(uint8_t * errorCode = nullptr);
    bool isDischarging(uint8_t * errorCode = nullptr);

    // STATE_CTL1

    bool isFastCharge(uint8_t * errorCode = nullptr);

    // STATE_CTL2

    bool isVbusPresent(uint8_t * errorCode = nullptr);
    bool isVbusOvervoltage(uint8_t * errorCode = nullptr);

    // TypeC_STATE

    bool isTypeCSinkConnected(uint8_t * errorCode = nullptr);
    bool isTypeCSrcConnected(uint8_t * errorCode = nullptr);
    bool isTypeCSrcPdConnected(uint8_t * errorCode = nullptr);
    bool isTypeCSinkPdConnected(uint8_t * errorCode = nullptr);
    bool isVbusSinkQcActive(uint8_t * errorCode = nullptr);
    bool isVbusSrcQcActive(uint8_t * errorCode = nullptr);

    // RECEIVED_PDO

    bool isReceives5VPdo(uint8_t * errorCode);
    bool isReceives9VPdo(uint8_t * errorCode);
    bool isReceives12VPdo(uint8_t * errorCode);
    bool isReceives15VPdo(uint8_t * errorCode);
    bool isReceives20VPdo(uint8_t * errorCode);

    // STATE_CTL3

    bool isVsysOverCurrent(uint8_t * errorCode = nullptr);
    bool isVsysSdortCircuitDt(uint8_t * errorCode = nullptr);

    // ADC
    bool isOverHeat(uint8_t * errorCode = nullptr);

    ///////// SET ////////

    // SYS_CTL0

    void enableCharger(bool enable = true, uint8_t * errorCode = nullptr);
    void enableVbusSinkSCP(bool enable = true, uint8_t * errorCode = nullptr);
    void enableVbusSinkPD(bool enable = true, uint8_t * errorCode = nullptr);
    void enableVbusSinkDPdM(bool enable = true, uint8_t * errorCode = nullptr);
    void enableINTLow(bool enable = true, uint8_t * errorCode = nullptr);
    void ResetMCU(bool enable = true, uint8_t * errorCode = nullptr);
    void enableLoadOTP(bool enable = true, uint8_t * errorCode = nullptr);

    // SYS_CTL2

    void setFullChargeVoltage(uint16_t voltage, uint8_t * errorCode = nullptr);

    // SYS_CTL3

    void setMaxInputPowerOrBatteryCurrent(uint16_t current_mA, uint8_t * errorCode = nullptr);

    // SYS_CTL6

    void setTrickleChargeCurrent(uint16_t current, uint8_t * errorCode = nullptr);

    // SYS_CTL8

    void setChargeStopCurrent(uint16_t current, uint8_t * errorCode = nullptr);
    void setCellRechargeThreshold(uint16_t voltageDrop_mV, uint8_t * errorCode = nullptr);

    // SYS_CTL9

    void enableStandbyMode(bool enable = true, uint8_t * errorCode = nullptr);
    void Standby(bool enable = true, uint8_t * errorCode = nullptr);
    void enableBATLow(bool enable = true, uint8_t * errorCode = nullptr);

    // SYS_CTL10

    void setLowBatteryVoltage(uint16_t threshold_mV, uint8_t * errorCode = nullptr);

    // SYS_CTL11

    void setOutputFeatures(bool enable = trueDcDcOutput, bool enable = trueVbusSrcDPdM, bool enable = trueVbusSrcPd, bool enable = trueVbusSrcSCP, uint8_t * errorCode = nullptr);

    // SYS_CTL12

    void setMaxOutputPower(Vbus1OutputPower power, uint8_t * errorCode = nullptr);

    // SELECT_PDO

    void setChargingPDOmode(ChargingPDOmode mode, uint8_t * errorCode);

    // TypeC_CTL8
    void setTypeCMode(TypeCMode mode, uint8_t * errorCode = nullptr);

    // TypeC_CTL9

    void enablePdoCurrentOutputSet(bool en5VPdoIset, bool en5VPdo3A, bool en9VPdoIset,
                                   bool en12VPdoIset, bool en15VPdoIset, bool en20VPdoIset,
                                   bool enPps1PdoIset, bool enPps2PdoIset, uint8_t * errorCode = nullptr);

    // TypeC_CTL10 - TypeC_CTL14

    void setPDOCurrent5V(uint16_t current_mA, uint8_t * errorCode = nullptr);
    void setPDOCurrent9V(uint16_t current_mA, uint8_t * errorCode = nullptr);
    void setPDOCurrent12V(uint16_t current_mA, uint8_t * errorCode = nullptr);
    void setPDOCurrent15V(uint16_t current_mA, uint8_t * errorCode = nullptr);
    void setPDOCurrent20V(uint16_t current_mA, uint8_t * errorCode = nullptr);

    // TypeC_CTL23 - TypeC_CTL24

    void setPDOCurrentPPS1(uint16_t current_mA, uint8_t * errorCode = nullptr);
    void setPDOCurrentPPS2(uint16_t current_mA, uint8_t * errorCode = nullptr);

    // TypeC_CTL17

    void enableSrcPdo(bool en9VPdo, bool en12VPdo, bool en15VPdo, bool en20VPdo, bool enPps1Pdo, bool enPps2Pdo, uint8_t * errorCode = nullptr);

    // TypeC_CTL18

    void enableSrcPdoAdd10mA(bool en5VPdoAdd10mA, bool en9VPdoAdd10mA, bool en12VPdoAdd10mA, bool en15VPdoAdd10mA, bool en20VPdoAdd10mA, uint8_t * errorCode);

    ///////// GET ////////

    // SYS_CTL2

    uint16_t getFullChargeVoltage(uint8_t * errorCode = nullptr);

    // SYS_CTL3

    uint16_t getMaxInputPowerOrBatteryCurrent(uint8_t * errorCode = nullptr);

    // SYS_CTL6

    uint16_t getTrickleChargeCurrent(uint8_t * errorCode = nullptr);

    // SYS_CTL8

    uint16_t getChargeStopCurrent(uint8_t * errorCode = nullptr);
    uint16_t getCellRechargeThreshold(uint8_t * errorCode = nullptr);

    // SYS_CTL10

    uint16_t getLowBatteryVoltage(uint8_t * errorCode = nullptr);

    // SYS_CTL12

    Vbus1OutputPower getMaxOutputPower(uint8_t * errorCode = nullptr);

    // SELECT_PDO

    ChargingPDOmode getChargingPDOmode(uint8_t * errorCode);

    // TypeC_CTL8

    TypeCMode getTypeCMode(uint8_t * errorCode = nullptr);

    // TypeC_CTL10 - TypeC_CTL14

    uint16_t getPDOCurrent5V(uint8_t * errorCode = nullptr);
    uint16_t getPDOCurrent9V(uint8_t * errorCode = nullptr);
    uint16_t getPDOCurrent12V(uint8_t * errorCode = nullptr);
    uint16_t getPDOCurrent15V(uint8_t * errorCode = nullptr);
    uint16_t getPDOCurrent20V(uint8_t * errorCode = nullptr);

    // TypeC_CTL23 - TypeC_CTL24

    uint16_t getPDOCurrentPPS1(uint8_t * errorCode = nullptr);
    uint16_t getPDOCurrentPPS2(uint8_t * errorCode = nullptr);

    // STATE_CTL0

    ChargeState getChargeState(uint8_t * errorCode = nullptr);

    uint8_t getChargeVoltage(uint8_t * errorCode = nullptr);

    // TIMENODE

    void getTimenode(char timenode[5], uint8_t * errorCode);

    // ADC

    uint16_t getVBATVoltage(uint8_t * errorCode = nullptr);
    uint16_t getVsysVoltage(uint8_t * errorCode = nullptr);
    uint16_t getBATCurrent(uint8_t * errorCode = nullptr);
    uint16_t getVsysCurrent(uint8_t * errorCode = nullptr);
    uint32_t getVsysPower(uint8_t * errorCode = nullptr);

    // GPIO

    uint16_t getNTCVoltage(uint8_t * errorCode = nullptr);

private:
    uint8_t writeRegister(uint8_t regAddress, uint8_t value, uint8_t * errorCode = nullptr);
    uint8_t readRegister(uint8_t regAddress, uint8_t * errorCode = nullptr);
    inline uint8_t setBit(uint8_t value, uint8_t bit, bool enable = true);
    void writeTypeCCurrentSetting(uint8_t reg, uint16_t current_mA, uint16_t step, uint16_t maxCurrent, uint8_t * errorCode = nullptr);
};

#endif
