#ifndef BQ76952_HPP
#define BQ76952_HPP
#include <stdint.h>
#include <string>
#include <string.h>
#include <fcntl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <vector> 
#include <iostream>



#define LOW_BYTE(data) (uint8_t)(data & 0x00FF)
#define HIGH_BYTE(data) (uint8_t)((data >> 8) & 0x00FF)

#define BQ76952_CMD_DIR_SUBCMD_LOW 0x3E
#define BQ76952_TRANSFER_BUFFER_START_ADDR 0x40
#define BQ76952_TRANSFER_BUFFER_CHECKSUM_ADDR 0x60
#define BQ76952_TRANSFER_BUFFER_LENGTH_ADDR 0x61
#define BQ76952_START_CONFIG_MODE_ADDR (uint16_t) 0x0090
#define BQ76952_EXIT_CONFIG_MODE_ADDR  (uint16_t) 0x0092
#define BQ76952_CELL_GAIN_START_ADDR (uint16_t)0x9180
#define BQ76952_CELL_VOLTAGE_START_ADDR 0x14
#define BQ76952_CELL_VOLTAE_END_ADDR 0x32
#define BQ76952_VOLTAGE_END_ADDR   0x38
#define BQ76952_VOLTAGE_COUNT  ( ( BQ76952_VOLTAGE_END_ADDR - BQ76952_CELL_VOLTAGE_START_ADDR ) / sizeof(uint16_t) ) + 1
#define BQ76952_CELL_COUNT ( ( BQ76952_CELL_VOLTAE_END_ADDR - BQ76952_CELL_VOLTAGE_START_ADDR ) / sizeof(uint16_t) ) + 1
#define BQ76952_STACK_VOLTAGE_ADDR 0x34
#define BQ76952_PACK_VOLTAGE_ADDR 0x36
#define BQ76952_LD_VOLTAGE_ADDR 0x38

#define BQ76952_CELL_BALANCE_CONFIG_ADDR (uint16_t) 0x9335
#define BQ76952_START_TEMP_ADDR 0x68
#define BQ76952_PACK_GAIN_ADDR (uint16_t) 0x91A0
#define BQ76952_TOS_GAIN_ADDR (uint16_t) 0x91A2
#define BQ76952_LD_GAIN_ADDR (uint16_t) 0x91A4
#define BQ76952_ADC_GAIN_ADDR (uint16_t) 0x91A6

#define BQ76952_CC_GAIN_ADDR (uint16_t) 0x91A8
#define BQ76952_CC_GAIN_MIN (float) 1.00E-02
#define BQ76952_CC_GAIN_MAX (float) 10.00E+02

#define BQ76952_CAP_GAIN_ADDR (uint16_t) 0x91AC
#define BQ76952_CAP_GAIN_MIN (float) 2.98262E+03
#define BQ76952_CAP_GAIN_MAX (float) 4.193046E+08


#define BQ76952_VOFFSET_ADDR (uint16_t) 0x91B0
#define BQ76952_VDIV_ADDR (uint16_t) 0x91B2
#define BQ76952_COLUMB_COUNTER_ADDR (uint16_t) 0x91C6
#define BQ76952_BOARD_OFFSET_ADDR (uint16_t) 0x91C8
#define BQ76952_INT_TEMP_GAIN_ADDR (uint16_t) 0x91E2

#define BQ76952_INT_TEMP_BASE_OFFSET_ADDR (uint16_t) 0x91E4
#define BQ76952_INT_TEMP_MAX_AD_ADDR (uint16_t) 0x91E6
#define BQ76952_INT_TEMP_MAX_ADDR (uint16_t) 0x91E8

#define BQ76952_COLOMB_COUNTER_DEADBAND_ADDR (uint16_t) 0x922D
#define BQ76952_CUV_THRESHOLD_ADDR (uint16_t)0x91D4
#define BQ76952_MIN_FUSE_ADDR (uint16_t)0x9231
#define BQ76952_FUSE_BLOWOUT_TIMEOUT_ADDR (uint16_t) 0x9233
#define BQ76952_POWER_CONFIG_ADDR (uint16_t) 0x9234
#define BQ76952_REG12_CONFIG_ADDR (uint16_t) 0x9236
#define BQ76952_REB0_CONFIG_ADDR (uint16_t) 0x9237
#define BQ76952_HWD_CONFIG_ADDR (uint16_t) 0x9238

#define BQ76952_COM_IDLETIME_ADDR (uint16_t) 0x923D
#define BQ76952_CFETOFF_CONFIG_ADDR (uint16_t) 0x92FA
#define BQ76952_DFETOFF_CONFIG_ADDR (uint16_t) 0x92FB
#define BQ76952_ALERT_CONFIG_ADDR (uint16_t) 0x92FC
#define BQ76952_TS1_CONFIG_ADDR (uint16_t) 0x92FD
#define BQ76952_TS2_CONFIG_ADDR (uint16_t) 0x92FE
#define BQ76952_TS3_CONFIG_ADDR (uint16_t) 0x92FF
#define BQ76952_HDQ_CONFIG_ADDR (uint16_t) 0x9300
#define BQ76952_DCHG_CONFIG_ADDR (uint16_t) 0x9301
#define BQ76952_DDSG_CONFIG_ADDR (uint16_t) 0x9302
#define BQ76952_DA_CONFIG_ADDR (uint16_t) 0x9303
#define BQ76952_VCELL_MODE_ADDR (uint16_t) 0x9304
#define BQ76952_CC3_SAMPLES_ADDR (uint16_t) 0x9307
#define BQ76952_DEVICE_NUMBER_ADDR (uint16_t) 0x0001

#define BQ76952_FW_VERSION_ADDR (uint16_t) 0x0002
#define BQ76952_HW_VERSION_ADDR (uint16_t) 0x0003





typedef union
{
    struct
    {
        uint8_t CB_CHG:1;      // This bit enables cell balancing while current is above Settings:Current Thresholds:Chg Current Threshold.  0 = Cell balancing is not allowed while charging. 1 = Cell balancing is allowed while charging
        uint8_t CB_RLX:1;      // This bit enable This bit enables cell balancing while current is under Settings:Current Thresholds:Chg Current Threshold nd above -Settings:Current Thresholds:Dsg Current Threshold.
        uint8_t CB_SLEEP:1;    // This bit configures whether or not the device is allowed to perform cell balancing while in SLEEP mode. 0 = Cell balancing is not performed in SLEEP mode. 1 = Cell balancing may be performed while in SLEEP mode.
        uint8_t CB_NOSLEEP:1;  // This bit configures the device to exit sleep mode to perform cell balancing. 0 = SLEEP mode is allowed while cell balancing is active. 1 = SLEEP is prevented while cell balancing is active.
        uint8_t CB_NO_CMD:1;   // This bit allows blocking of cell balancing commands if host-controlled balancing is not desired. 0 = Host-controlled balancing commands are accepted, 1 = Host-controlled balancing commands are ignored
        uint8_t reserved__1:1; // reserved
        uint8_t reserved__2:1; // reserved
        uint8_t reserved__3:1; // reserved
    };
    uint8_t m_flag;
}BQ76952CellBalanceConfig; 

typedef union
{
    struct
    {
        uint8_t WK_SPD_0:1;       // Selects coulomb counter conversion speed used in sleep more for current wake comparator function. slower == less noise
        uint8_t WK_SPD_1:1;       // OX0 = 48ms 0x1 = 24ms, 0x2 = 12ms, 0x3 = 6ms
        uint8_t LOOP_SLOW_0:1;    // Select normal ADC scan loop speed by inserting current only measurements after each voltage. Only used when balancing is not active
        uint8_t LOOP_SLOW_1:1;    // 0 = full speed, 1 = half speed, 2 = quarter speed, 3 eigth speed 
        uint8_t CB_LOOP_SLOW_0:1; // Selects ADC scan loop speed while cell balancing is active by inserting current- only measurements after each voltage and temperature scan loop
        uint8_t CB_LOOP_SLOW_1:1; // 0 = full speed, 1 = half speed, 2 = quarter speed, 3 eigth speed 
        uint8_t FAST_ADC:1;       // Selects ADC conversion speed for voltage and simultaneous current measurements. Higher speed results in lower accuracy. 0 = 3ms, 1 = 1.5ms
        uint8_t OTSD:1;           // Enables or disables on chip shutdown in case of severe over temperature. 0 Disable shutdown from on chip, 1 enable shtudown from on chip
        uint8_t SLEEP:1;          // Sets the default value of BatteryStatus()[SLEEP_EN] which enables or disables SLEEP mode. 0 = disable sleep mode, 1 = enable sleep mode
        uint8_t DPSLP_LFO:1;      // Determines whether or not to disable the Low Frequency Oscillator in DEEPSLEEP. 0 = disable Oscillator, 1 = enable oscillator
        uint8_t DPSLP_LDO:1;      // Determines whether or not REG1 and REG2 are disabled in DEEPSLEEP mode. 0 = disable reg1/reg2 in sleep, 1 = leave reg1/reg2 in current state
        uint8_t DPSLP_PD:1;       // Enables wake from DEEPSLEEP based on charger attach. When cleared must to exited by command. 0 = deep sleep not exited, 1 exited on rising ld
        uint8_t SHUT_TS2:1;       // If TS2 has a thermistor SHUTDOWN mode cannot be used If TS2 pulled down set this bit to prevent shutdown. 0 = standard shutdown mode behavior, 2 = shutdown mode replaced by rising ld edge
        uint8_t DPSLP_OT:1;       // Enables transition from DEEPSLEEP to SHUTDOWN based on on-chip over-temperature detection.  0 = In DEEPSLEEP, on-chip OT is disabled 1 = on chip OT is enable in deep sleep, allowing transition to shutdown
        uint8_t reserved__1:1;
        uint8_t reserved__2:1;
    }; 
    uint16_t m_flag;
}BQ76952PowerConfig; 


typedef union
{
    struct
    {
        uint8_t REG1_EN:1; // Configure default state for REG1 output. Settings is reapplied when initializing after reset or deepsleep mode. 0 = reg1 disabled, 1 = reg1 enabled
        uint8_t REG1V_0:1; // Selects voltage level for REG1 This setting should not be changed while REG1 is enabled.
        uint8_t REG1V_1:1; // 0-3 = 1.8V, 4 = 2.5V, 5=3V, 6=3.3V, 7=5V
        uint8_t REG1V_2:1;
        uint8_t REG2_EN:1; // Configure default state for REG2 output. This setting is reapplied when initializing after reset or DEEPSLEEP mode 0 = reg2 disabled, 1 = reg1 enabled
        uint8_t REG2V_0:1; // Selects voltage level for REG2 This setting should not be changed while REG2 is enabled.
        uint8_t REG2V_1:1; // 0-3 = 1.8V, 4 = 2.5V, 5=3V, 6=3.3V, 7=5V
        uint8_t REG2V_2:1;
    }; 
    uint8_t m_flag;
}BQ76952Reg12Config; 

typedef union
{
    struct
    {
        uint8_t REG0_EN:1;    // enable to disable preregulator for REG1 and REG2. 0 = pre-regulator is disabled 1 = pre-regulator is enabled
        uint8_t reserved__1:1;
        uint8_t reserved__2:1;
        uint8_t reserved__3:1;
        uint8_t reserved__4:1;
        uint8_t reserved__5:1;
        uint8_t reserved__6:1;
        uint8_t reserved__8:1;
    };
    uint8_t m_flag; 
}BQ76952Reg0Config;

typedef union
{ 
    struct
    {
        uint8_t TOGGLE_TIME_0:1; // How long to keep REG1 and REG2 regulators off when configured to toggle
        uint8_t TOGGLE_TIME_1:1; // 0  = Turn REG1 and REG2 regulators off and do not turn back on again
        uint8_t TOGGLE_TIME_2:1; // 1–15 = Turn REG1 and REG2 regulators back on again after this many seconds
        uint8_t TOGGLE_OPT_0:1;  // Action to take when HWD protection is triggered
        uint8_t TOGGLE_OPT_1:1;  // 0 = Take no action on REG1 and REG2 regulators, 1 = Turn REG1 and REG2 regulators off, 2 = Turn REG1 and REG2 regulators off for configured duration and then back on again.
        uint8_t reserved__1:1;
        uint8_t reserved__2:1;
        uint8_t reserved__3:1;
    }; 
    uint8_t m_flag;
}BQ76952HWDRegConfig; 

typedef union
{
    struct
    {
        uint8_t PIN_FXN0:1; // These bits configure which function this pin is used for. 
        uint8_t PIN_FXN1:1; // CHECK THE REFERENCE MANUAL FOR THESE BITS 
        uint8_t OPT0:1;     // CHECK THE REFERENCE MANUAL FOR THESE BITS 
        uint8_t OPT1:1;     // CHECK THE REFERENCE MANUAL FOR THESE BITS 
        uint8_t OPT2:1;     // CHECK THE REFERENCE MANUAL FOR THESE BITS 
        uint8_t OPT3:1;     // CHECK THE REFERENCE MANUAL FOR THESE BITS 
        uint8_t OPT4:1;     // CHECK THE REFERENCE MANUAL FOR THESE BITS 
        uint8_t OPT5:1;     // CHECK THE REFERENCE MANUAL FOR THESE BITS 
    };
    uint8_t m_flag;
}BQ76952PinConfig;  // The documentation here varies for each Pin. Check the reference manual for CFETOFF, DFET, ALERT, TS1, TS2, TS3, HDQ, DCHG, DDSG, and DA config

typedef union
{
    struct
    {
        uint8_t USER_AMPS_0:1;   // Changes the unit measured current
        uint8_t USER_AMPS_1:1;   // 0 = decimilliamp, 1 = milliamp, 2 = centiamp, 3 = deciamp
        uint8_t USER_VOLTS_CV:1; // Changes the unit measured voltage.  0 = millivolts 1 = centivolt
        uint8_t TINT_EN:1;       // nables the internal temperature source to be used as the “Cell temperature" for protections and logic that use min/max or average temp. 0 = internal temp is not used for cell temp, 1 internal temp is used for cell temp
        uint8_t TINT_FETT:1;     // TINT_FETT enables the internal temperature source to be used as the "FET temperature" TINT_EN_ must also be set. 0 = internal temperature is not used for FET temp, 1 = internal temp is used for FET temperature
        uint8_t reserved__1:1;
        uint8_t reserved__2:1;
        uint8_t reserved__3:1;
    };
    uint8_t m_flag; 
}BQ76952DAConfig;

typedef union
{
    struct
    {
        uint8_t CELL_MODE_0:1;  // 0 = A cell is not connected between VC1 and VC0(DISABLE PROTECTIONS). 1 = A cell is connected between VC1 and VC0(ENABLE).
        uint8_t CELL_MODE_1:1;  // 0 = A cell is not connected between VC2 and VC1(DISABLE PROTECTIONS). 1 = A cell is connected between VC2 and VC1(ENABLE).
        uint8_t CELL_MODE_2:1;  // 0 = A cell is not connected between VC3 and VC2(DISABLE PROTECTIONS). 1 = A cell is connected between VC3 and VC2(ENABLE).
        uint8_t CELL_MODE_3:1;  // 0 = A cell is not connected between VC4 and VC3(DISABLE PROTECTIONS). 1 = A cell is connected between VC4 and VC3(ENABLE).
        uint8_t CELL_MODE_4:1;  // 0 = A cell is not connected between VC5 and VC4(DISABLE PROTECTIONS). 1 = A cell is connected between VC5 and VC4(ENABLE).
        uint8_t CELL_MODE_5:1;  // 0 = A cell is not connected between VC6 and VC5(DISABLE PROTECTIONS). 1 = A cell is connected between VC6 and VC5(ENABLE).
        uint8_t CELL_MODE_6:1;  // 0 = A cell is not connected between VC7 and VC6(DISABLE PROTECTIONS). 1 = A cell is connected between VC7 and VC6(ENABLE).
        uint8_t CELL_MODE_7:1;  // 0 = A cell is not connected between VC8 and VC6(DISABLE PROTECTIONS). 1 = A cell is connected between VC8 and VC7(ENABLE).
        uint8_t CELL_MODE_8:1;  // 0 = A cell is not connected between VC9 and VC7(DISABLE PROTECTIONS). 1 = A cell is connected between VC9 and VC8(ENABLE).
        uint8_t CELL_MODE_9:1;  // 0 = A cell is not connected between VC10 and VC9(DISABLE PROTECTIONS). 1 = A cell is connected between VC10 and VC9(ENABLE).
        uint8_t CELL_MODE_10:1; // 0 = A cell is not connected between VC11 and VC10(DISABLE PROTECTIONS). 1 = A cell is connected between VC11 and VC10(ENABLE).
        uint8_t CELL_MODE_11:1; // 0 = A cell is not connected between VC12 and VC11(DISABLE PROTECTIONS). 1 = A cell is connected between VC12 and VC11(ENABLE).
        uint8_t CELL_MODE_12:1; // 0 = A cell is not connected between VC13 and VC12(DISABLE PROTECTIONS). 1 = A cell is connected between VC13 and VC12(ENABLE).
        uint8_t CELL_MODE_13:1; // 0 = A cell is not connected between VC14 and VC13(DISABLE PROTECTIONS). 1 = A cell is connected between VC14 and VC13(ENABLE).
        uint8_t CELL_MODE_14:1; // 0 = A cell is not connected between VC15 and VC14(DISABLE PROTECTIONS). 1 = A cell is connected between VC15 and VC14(ENABLE).
        uint8_t CELL_MODE_15:1; // 0 = A cell is not connected between VC16 and VC15(DISABLE PROTECTIONS). 1 = A cell is connected between VC16 and VC15(ENABLE).
    }; 
    uint16_t m_flag;
}BQ76952VCellMode; 

enum BQ76952Thermistor 
{
    INT_TEMP = 0,
    CFET_TEMP,
    DFET_TEMP,
    ALERT_TEMP,
    TS1_TEMP,
    TS2_TEMP,
    TS3_TEMP, 
    HDQ_TEMP,
    DCHG_TEMP,
    DDSG_TEMP
};

enum BQ76952TempOffset : uint16_t
{
    INTERNAL_TEMP_OFFSET = 0x91CA,
    CFETOFF_TEMP_OFFSET  = 0x91CB,
    DFETOFF_TEMP_OFFSET  = 0x91CC,
    ALERT_TEMP_OFFSET    = 0x91CD,
    TS1_TEMP_OFFSET      = 0x91CE,
    TS2_TEMP_OFFSET      = 0x91CF,
    TS3_TEMP_OFFSET      = 0x91D0,
    HDQ_TEMP_OFFSET      = 0x91D1,
    DCHG_TEMP_OFFSET     = 0x91D2,
    DDSG_TEMP_OFFSEt     = 0x91D3
};

enum BQ76952TempModel : uint16_t
{
    TEMP_MODEL_18K_COEFF_A1 = 0x91EA,
    TEMP_MODEL_18K_COEFF_A2 = 0x91EC,
    TEMP_MODEL_18K_COEFF_A3 = 0x91EE,
    TEMP_MODEL_18K_COEFF_A4 = 0x91F0,
    TEMP_MODEL_18K_COEFF_A5 = 0x91F2,
    TEMP_MODEL_18K_COEFF_B1 = 0x91F4,
    TEMP_MODEL_18K_COEFF_B2 = 0x91F6,
    TEMP_MODEL_18K_COEFF_B3 = 0x91F8,
    TEMP_MODEL_18K_COEFF_B4 = 0x91FA,
    TEMP_MODEL_18K_COEFF_ADC0 = 0x91FE,


    TEMP_MODEL_180K_COEFF_A1 = 0x9200,
    TEMP_MODEL_180K_COEFF_A2 = 0x9202,
    TEMP_MODEL_180K_COEFF_A3 = 0x9204,
    TEMP_MODEL_180K_COEFF_A4 = 0x9206,
    TEMP_MODEL_180K_COEFF_A5 = 0x9208,
    TEMP_MODEL_180K_COEFF_B1 = 0x920A,
    TEMP_MODEL_180K_COEFF_B2 = 0x920C,
    TEMP_MODEL_180K_COEFF_B3 = 0x920E,
    TEMP_MODEL_180K_COEFF_B4 = 0x9210,
    TEMP_MODEL_180K_COEFF_ADC0 = 0x9214,

    TEMP_MODEL_CUSTOM_COEFF_A1 = 0x9216,
    TEMP_MODEL_CUSTOM_COEFF_A2 = 0x9218,
    TEMP_MODEL_CUSTOM_COEFF_A3 = 0x921A,
    TEMP_MODEL_CUSTOM_COEFF_A4 = 0x921C,
    TEMP_MODEL_CUSTOM_COEFF_A5 = 0x921E,
    TEMP_MODEL_CUSTOM_COEFF_B1 = 0x9220,
    TEMP_MODEL_CUSTOM_COEFF_B2 = 0x9222,
    TEMP_MODEL_CUSTOM_COEFF_B3 = 0x9224,
    TEMP_MODEL_CUSTOM_COEFF_B4 = 0x9226,
    TEMP_MODEL_CUSTOM_COEFF_RC0 = 0x9228,
    TEMP_MODEL_CUSTOM_COEFF_ADC0 = 0x922A
};


class BQ76952
{
private:    
    const int32_t m_bus;
    const uint8_t m_device_addr;

    std::string getErrorMessage()
    {
        return strerror(errno);
    }
    uint8_t computeChecksum(uint8_t old_checksum, const uint8_t* data, const uint8_t len)
    {
        for(uint8_t i = 0; i < len; i++)
        {
            old_checksum = ~(old_checksum) + data[i];
            old_checksum = ~(old_checksum);
        }
        return old_checksum;
    }
    uint8_t computeChecksum(uint8_t old_checksum, const uint8_t data)
    {
        if(!old_checksum)
            old_checksum = data;
        else
            old_checksum = ~(old_checksum) + data;
        return ~(old_checksum);
    }

    template<typename T> 
    bool writeToDevice(uint8_t addr, T value)
    {   
        struct i2c_msg ioctl_msg[2];
        struct i2c_rdwr_ioctl_data ioctl_data;

        memset(&ioctl_msg, 0, sizeof(ioctl_msg));
        memset(&ioctl_data, 0, sizeof(ioctl_data));

        ioctl_data.nmsgs =	2;
        ioctl_data.msgs	 =	ioctl_msg;

        ioctl_msg[0].len   = sizeof(uint8_t); 
        ioctl_msg[0].addr  = m_device_addr;
        ioctl_msg[0].buf   = &addr;
        ioctl_msg[0].flags = 0;

        ioctl_msg[1].len   = sizeof(T); 
        ioctl_msg[1].addr  = m_device_addr;
        ioctl_msg[1].buf   = reinterpret_cast<uint8_t*>(&value);
        ioctl_msg[1].flags = 0;

        return ioctl(m_bus, I2C_RDWR, (uint64_t)&ioctl_data) != -1;
    }
    
    bool readDeviceMemory(uint8_t addr, uint8_t* value, const uint16_t len)
    {
        struct i2c_msg ioctl_msg[2];
        struct i2c_rdwr_ioctl_data ioctl_data;

        memset(&ioctl_msg, 0, sizeof(ioctl_msg));
        memset(&ioctl_data, 0, sizeof(ioctl_data));

        ioctl_data.nmsgs =	2;
        ioctl_data.msgs	 =	ioctl_msg;

        ioctl_msg[0].len   = sizeof(uint8_t); 
        ioctl_msg[0].addr  = m_device_addr;
        ioctl_msg[0].buf   = &addr;
        ioctl_msg[0].flags = 0;

        ioctl_msg[1].len   = len; 
        ioctl_msg[1].addr  = m_device_addr;
        ioctl_msg[1].buf   = value;
        ioctl_msg[1].flags = I2C_M_RD;

        return ioctl(m_bus, I2C_RDWR, (uint64_t)&ioctl_data) != -1;
    }

    bool readSubcommand(uint16_t addr, uint8_t* value, const uint16_t len)
    {
        uint8_t current_attempt = 0, num_attempts = 10, reply_length;
        uint16_t reply_command;

        if(!writeToDevice(BQ76952_CMD_DIR_SUBCMD_LOW, addr))
            return false;
        
        while(current_attempt < num_attempts)
        {
            readDeviceMemory(BQ76952_CMD_DIR_SUBCMD_LOW, reinterpret_cast<uint8_t*>(&reply_command), sizeof(uint16_t));

            if(reply_command != 0xFF) // data is ready
            {
                readDeviceMemory(BQ76952_TRANSFER_BUFFER_LENGTH_ADDR, &reply_length, sizeof(uint8_t));
                readDeviceMemory(BQ76952_TRANSFER_BUFFER_START_ADDR, value, len);
                return true;
            }
            current_attempt++;
        }
        return false;
    }
    template<typename T> 
    bool writeDeviceMemory(const uint16_t addr, const T data)
    {
        uint8_t checksum = 0, reply_length = 0, reply_checksum = 0, current_attempt = 0, data_length = 0x06, num_attempts = 10;
        T reply_data = 0;
        bool success = true;

        checksum = computeChecksum(checksum, LOW_BYTE(addr));
        checksum = computeChecksum(checksum, HIGH_BYTE(addr));
        checksum = computeChecksum(checksum, reinterpret_cast<const uint8_t*>(&data), sizeof(T));

        if(!writeToDevice(BQ76952_CMD_DIR_SUBCMD_LOW, BQ76952_START_CONFIG_MODE_ADDR)) // enter config mode
            return false;

        while(current_attempt < num_attempts)
        {   
            writeToDevice(BQ76952_CMD_DIR_SUBCMD_LOW, addr);                                                                 // write to command addr
            writeToDevice(BQ76952_TRANSFER_BUFFER_START_ADDR, data);                                                         // write to transfer buffer
            writeToDevice(BQ76952_TRANSFER_BUFFER_CHECKSUM_ADDR, checksum);                                                  // write to checksum
            writeToDevice(BQ76952_TRANSFER_BUFFER_LENGTH_ADDR, data_length);                                                 // write buffer length
            writeToDevice(BQ76952_CMD_DIR_SUBCMD_LOW, addr);                                                                 // write command addr to read data back
            readDeviceMemory(BQ76952_TRANSFER_BUFFER_LENGTH_ADDR, &reply_length, sizeof(uint8_t));                       // read the length buffer
            readDeviceMemory(BQ76952_TRANSFER_BUFFER_START_ADDR, reinterpret_cast<uint8_t*>(&reply_data), sizeof(T));    // read the data back
            readDeviceMemory(BQ76952_TRANSFER_BUFFER_CHECKSUM_ADDR, &reply_checksum, sizeof(uint8_t));                   // read the checksum

            current_attempt++;

            if(reply_checksum == checksum) // could also check data
                break;
        }

        if(writeToDevice(BQ76952_CMD_DIR_SUBCMD_LOW, BQ76952_EXIT_CONFIG_MODE_ADDR))
            return success;
        
        return false;
    }
    template<typename T>
    bool writeDeviceMemoryImpl(const uint16_t addr, const T value)
    {
        if(!writeDeviceMemory(addr, value))
        {
            std::cout << "Failed to write to device memory. Errno: " << getErrorMessage() << '\n';
            return false;
        }
        return true;
    }
public:
    BQ76952(const std::string& bus_path, const uint8_t device_addr)    
        : m_bus(open(bus_path.c_str(), O_RDWR)), m_device_addr((device_addr))
    {
        if(m_bus == -1)
            std::cout << "FAILED TO OPEN: " << bus_path << " ERRNO " << getErrorMessage() << '\n';
    }
    ~BQ76952()
    {
        if(m_bus != -1)
            close(m_bus);
    }

    BQ76952() = delete;
    BQ76952(const BQ76952&) = delete;
    BQ76952(BQ76952&&) = delete;


    bool exitSleepMode()
    {
       return true;
    }
    bool setCellBalanceConfig(const BQ76952CellBalanceConfig config)
    {
        return writeDeviceMemoryImpl(BQ76952_CELL_BALANCE_CONFIG_ADDR, config.m_flag);
    }
    bool setVoltageGain(const uint16_t cell_index, const int16_t gain) // fix me 
    {
        return writeDeviceMemoryImpl(BQ76952_CELL_GAIN_START_ADDR + (cell_index - 1) * 2, gain);
    }
    bool setPackGain(const uint16_t gain)
    {
        return writeDeviceMemoryImpl(BQ76952_PACK_GAIN_ADDR, gain);
    }
    bool setTosGain(const uint16_t gain)
    {
        return writeDeviceMemoryImpl(BQ76952_TOS_GAIN_ADDR, gain);
    }
    bool setLdGain(const uint16_t gain)
    {
        return writeDeviceMemoryImpl(BQ76952_LD_GAIN_ADDR, gain);
    }
    bool setAdcGain(const int16_t gain)
    {
        return writeDeviceMemoryImpl(BQ76952_ADC_GAIN_ADDR, gain);
    }
    bool setCCGain(const float gain)
    {
        if(gain < BQ76952_CC_GAIN_MIN || gain > BQ76952_CC_GAIN_MAX)
            return false;
        return writeDeviceMemoryImpl(BQ76952_CC_GAIN_ADDR, gain);
    }
    bool setCapacityGain(const float gain)
    {
        if(gain < BQ76952_CAP_GAIN_MIN || gain > BQ76952_CAP_GAIN_MAX)
            return false;
        return writeDeviceMemoryImpl(BQ76952_CAP_GAIN_ADDR, gain);
    }
    bool setVCellOffset(const int16_t offset)
    {
        return writeDeviceMemoryImpl(BQ76952_VOFFSET_ADDR, offset);
    }
    bool setVDivOffset(const int16_t offset)
    {
        return writeDeviceMemoryImpl(BQ76952_VDIV_ADDR, offset);
    }
    bool setColumbCounterOffset(const uint16_t offset)
    {
        return writeDeviceMemoryImpl(BQ76952_COLUMB_COUNTER_ADDR, offset);
    }
    bool setBoardOffset(const int16_t offset)
    {
        return writeDeviceMemoryImpl(BQ76952_BOARD_OFFSET_ADDR, offset);
    }
    bool setTempOffset(const BQ76952TempOffset offset, const int8_t offset_value)
    {
        return writeDeviceMemoryImpl(offset, offset_value);
    }
    bool setIntTempGain(const int16_t gain)
    {
        return writeDeviceMemoryImpl(BQ76952_INT_TEMP_GAIN_ADDR, gain);
    }
    bool setIntTempBaseOffset(const int16_t offset)
    {
        return writeDeviceMemoryImpl(BQ76952_INT_TEMP_BASE_OFFSET_ADDR, offset);
    }
    bool setIntTempMaxAD(const int16_t max)
    {
        return writeDeviceMemoryImpl(BQ76952_INT_TEMP_MAX_AD_ADDR, max);
    }
    bool setIntTempMax(const int16_t max)
    {
        return writeDeviceMemoryImpl(BQ76952_INT_TEMP_MAX_ADDR, max);
    }
    bool setTempCoeff(const BQ76952TempModel coeff, const int16_t value)
    {
        return writeDeviceMemoryImpl(coeff, value);
    }
    bool setColombCounterDeadband(const int8_t value)
    {
        if(value < 0)
            return false;
        return writeDeviceMemoryImpl(BQ76952_COLOMB_COUNTER_DEADBAND_ADDR, value);
    }
    bool setCuvThresholdOverride(const uint16_t value)
    {
        return writeDeviceMemoryImpl(BQ76952_CUV_THRESHOLD_ADDR, value);
    }
    bool setMinFuseVoltage(const int16_t voltage)
    {
        return writeDeviceMemoryImpl(BQ76952_MIN_FUSE_ADDR, voltage);
    }
    bool setFuseBlowTimeout(const uint8_t time_seconds)
    {
        return writeDeviceMemoryImpl(BQ76952_FUSE_BLOWOUT_TIMEOUT_ADDR, time_seconds);
    }
    bool setPowerConfig(const BQ76952PowerConfig config)
    {
        return writeDeviceMemoryImpl(BQ76952_POWER_CONFIG_ADDR, config.m_flag);
    }
    bool setReg12Config(const BQ76952Reg12Config config)
    {
        return writeDeviceMemoryImpl(BQ76952_REG12_CONFIG_ADDR, config.m_flag);
    }
    bool setReg0Config(const BQ76952Reg0Config config)
    {
        return writeDeviceMemoryImpl(BQ76952_REB0_CONFIG_ADDR, config.m_flag);
    }
    bool setHWDConfig(const BQ76952HWDRegConfig config)
    {
        return writeDeviceMemoryImpl(BQ76952_HWD_CONFIG_ADDR, config.m_flag);
    }
    bool setI2CAddress(const uint8_t new_address)
    {
        return false; // probably add later
    }
    bool setComIdleTime(const uint8_t idle_time_seconds)
    {
        return writeDeviceMemoryImpl(BQ76952_COM_IDLETIME_ADDR, idle_time_seconds);
    }
    bool setCfetoffConfig(const BQ76952PinConfig config)
    {
        return writeDeviceMemoryImpl(BQ76952_CFETOFF_CONFIG_ADDR, config.m_flag); 
    }
    bool setDfetoffConfig(const BQ76952PinConfig config)
    {
        return writeDeviceMemoryImpl(BQ76952_DFETOFF_CONFIG_ADDR, config.m_flag); 
    }
    bool setAlertConfig(const BQ76952PinConfig config)
    {
        return writeDeviceMemoryImpl(BQ76952_ALERT_CONFIG_ADDR, config.m_flag); 
    }
    bool setTS1Config(const BQ76952PinConfig config)
    {
        return writeDeviceMemoryImpl(BQ76952_TS1_CONFIG_ADDR, config.m_flag); 
    }
    bool setTS2Config(const BQ76952PinConfig config)
    {
        return writeDeviceMemoryImpl(BQ76952_TS2_CONFIG_ADDR, config.m_flag); 
    }
    bool setTS3Config(const BQ76952PinConfig config)
    {
        return writeDeviceMemoryImpl(BQ76952_TS3_CONFIG_ADDR, config.m_flag); 
    }
    bool setHDQConfig(const BQ76952PinConfig config)
    {
        return writeDeviceMemoryImpl(BQ76952_HDQ_CONFIG_ADDR, config.m_flag); 
    }
    bool setDCHGConfig(const BQ76952PinConfig config)
    {
        return writeDeviceMemoryImpl(BQ76952_DCHG_CONFIG_ADDR, config.m_flag); 
    }
    bool setDDSGConfig(const BQ76952PinConfig config)
    {
        return writeDeviceMemoryImpl(BQ76952_DDSG_CONFIG_ADDR, config.m_flag); 
    }
    bool setDAConfig(const BQ76952DAConfig config)
    {
        return writeDeviceMemoryImpl(BQ76952_DA_CONFIG_ADDR, config.m_flag);
    }
    bool setVCellMode(const BQ76952VCellMode config)
    {
        return writeDeviceMemoryImpl(BQ76952_VCELL_MODE_ADDR, config.m_flag);
    }
    bool setCC3Samples(const uint8_t samples)
    {
        if(samples < 2)
            return false;
        return writeDeviceMemoryImpl(BQ76952_CC3_SAMPLES_ADDR, samples);
    }
    std::vector<uint16_t> getAllVoltages()
    {
        std::vector<uint16_t> voltages;
        uint16_t voltage;
        for(int16_t i = 0; i < BQ76952_VOLTAGE_COUNT; i++)
        {
            if(!readDeviceMemory(BQ76952_CELL_VOLTAGE_START_ADDR + i * sizeof(uint16_t), reinterpret_cast<uint8_t*>(&voltage), sizeof(uint16_t)))
                voltages.push_back(0);
            else
                voltages.push_back(voltage); 
        }
        return voltages;
    }
    uint16_t getCellVoltage(const uint16_t index)
    {
        if(index > BQ76952_CELL_COUNT || index < 1)
            return 0;

        uint16_t voltage; 
        if(!readDeviceMemory(uint8_t(BQ76952_CELL_VOLTAGE_START_ADDR + (index - 1) * sizeof(uint16_t)), reinterpret_cast<uint8_t*>(&voltage), sizeof(uint16_t)))
            voltage = 0;
        return voltage; 
    }
    uint16_t getStackVoltage()
    {
        uint16_t voltage; 
        if(!readDeviceMemory(BQ76952_STACK_VOLTAGE_ADDR, reinterpret_cast<uint8_t*>(&voltage), sizeof(uint16_t)))
            voltage = 0;
        return voltage;
    }
    uint16_t getPackVoltage()
    {
        uint16_t voltage;
        if(!readDeviceMemory(BQ76952_PACK_VOLTAGE_ADDR, reinterpret_cast<uint8_t*>(&voltage), sizeof(uint16_t)))
            voltage = 0;
        return voltage;
    }
    uint16_t getLDVoltage()
    {
        uint16_t voltage; 
        if(!readDeviceMemory(BQ76952_LD_VOLTAGE_ADDR, reinterpret_cast<uint8_t*>(&voltage), sizeof(uint16_t)))
            voltage = 0;
        return voltage;
    }
    uint16_t getThermistorTemp(const BQ76952Thermistor thermistor)
    {
        if(thermistor > DDSG_TEMP)
            return 0;
        uint16_t thermistor_value;
        if(!readDeviceMemory(BQ76952_START_TEMP_ADDR + thermistor * sizeof(uint16_t), reinterpret_cast<uint8_t*>(&thermistor_value), sizeof(uint16_t)))
            thermistor_value = 0;
        return thermistor_value; 
    }
    uint16_t getDeviceNumber()
    {
        uint16_t device_number;
        if(!readSubcommand(BQ76952_DEVICE_NUMBER_ADDR, reinterpret_cast<uint8_t*>(&device_number), sizeof(uint8_t)))
            device_number = 0;
        return device_number;
    }
    uint16_t getFWVersion()
    {
        uint16_t fw_version;
        if(!readSubcommand(BQ76952_FW_VERSION_ADDR, reinterpret_cast<uint8_t*>(&fw_version), sizeof(uint8_t)))
            fw_version = 0;
        return fw_version;
    }
    uint16_t getHWVersion()
    {
        uint16_t hw_version;
        if(!readSubcommand(BQ76952_HW_VERSION_ADDR, reinterpret_cast<uint8_t*>(&hw_version), sizeof(uint8_t)))
            hw_version = 0;
        return hw_version;
    }
};



#endif // BQ76952_HPP