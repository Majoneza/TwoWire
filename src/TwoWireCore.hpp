#pragma once

#include <stdint.h>

#define TWCR_UNUSED (TWCR & (_BV(TWEA) | _BV(TWWC) | _BV(TWEN) | _BV(TWIE)))
#define TWCR_W(d) ((d) | TWCR_UNUSED)

namespace TwoWire
{
    enum class BitRatePrescaler : int8_t
    {
        x1 = 0,
        x4 = 1,
        x16 = 2,
        x64 = 3
    };

    /**
     * @brief Most frequently used TWI frequency
     *
     */
    static constexpr auto DEFAULT_FREQUENCY = 100000;

    /**
     * @brief Enable TWI interface and initialize required parameters
     *
     * @param address Address of the TWI
     * @param frequency Frequency of the TWI
     */
    void init(uint8_t address, uint32_t frequency = DEFAULT_FREQUENCY);

    /**
     * @brief Enable TWI interface
     *
     */
    void enable();

    /**
     * @brief Disable TWI interface
     *
     */
    void disable();

    /**
     * @brief Set address of the TWI
     *
     * @param address 7 bit address of the TWI
     */
    void setAddress(uint8_t address);

    /**
     * @brief Set address of the TWI
     *
     * @param address 7 bit address of the TWI
     * @param allowGeneralCallAddress Allow TWI to be addressed by 0000 000 (general call address)
     */
    void setAddress(uint8_t address, bool allowGeneralCallAddress);

    /**
     * @brief Get address of the TWI
     *
     * @return uint8_t 7 bit address of the TWI
     */
    uint8_t getAddress();

    /**
     * @brief Set which address bits to ignore on comparison
     *
     * @param mask Address mask
     */
    void setAddressMask(uint8_t mask);

    /**
     * @brief Set TwoWire base frequency (without the prescaler)
     *
     * @param frequency Base frequency
     */
    void setBaseFrequency(uint32_t frequency = DEFAULT_FREQUENCY);

    /**
     * @brief Set frequency prescaler
     *
     * @param prescaler Prescaler value
     */
    void setFrequencyPrescaler(BitRatePrescaler prescaler);

    /**
     * @brief Get current TwoWire frequency (excluding the prescaler)
     *
     * @return uint32_t Base frequency of the TwoWire
     */
    uint32_t getBaseFrequency();

    /**
     * @brief Get current TwoWire frequency (including the prescaler)
     *
     * @return uint32_t Frequency of the TwoWire
     */
    uint32_t getFrequency();

    /**
     * @brief Allow other devices to communicate with this TWI device as slave
     *
     */
    void allowSlaveMode();

    /**
     * @brief Disallow other devices to communicate with this TWI device as slave
     *
     */
    void disallowSlaveMode();

    /**
     * @brief Allow TWI to be addressed by 0000 000 (general call address)
     *
     */
    void allowGeneralCall();

    /**
     * @brief Disallow TWI to be addressed by 0000 000 (general call address)
     *
     */
    void disallowGeneralCall();

    /**
     * @brief Allow TWI to call interrupt function on TWINT flag set
     *
     */
    void enableInterrupt();

    /**
     * @brief Disallow TWI to call interrupt function on TWI flag set
     *
     */
    void disableInterrupt();

    /**
     * @brief Activate pull-up on SCL & SDA pins
     * 
     */
    void activatePullup();

    /**
     * @brief Deactivate pull-up on SCL & SDA pins
     * 
     */
    void deactivatePullup();

    /**
     * @brief Signal stop to other devices
     *
     */
    void signalStop();

    /**
     * @brief Acknowledge TWI status (check it before)
     * 
     */
    void acknowledgeStatus();

    /**
     * @brief Check if the TWI is in error
     *
     * @return true TWI is in error
     * @return false TWI is not in error
     */
    bool isErrored();

    /**
     * @brief Clear error
     *  (check if its set before calling this function)
     * 
     */
    void clearError();

    /**
     * @brief Clear error if it is set
     * 
     */
    void clearErrorIfSet();
}
