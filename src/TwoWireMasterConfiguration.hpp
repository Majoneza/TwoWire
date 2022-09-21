#pragma once

#include "TwoWireCore.hpp"
#include <Arduino.h>

namespace TwoWire
{
    class MasterConfiguration
    {
    protected:
        static constexpr auto DEFAULT_TIMEOUT = 25000;

    public:
        enum class Status : int8_t
        {
            // Successful operation
            Success,
            // Unable to locate slave device of provided address
            AddressNACK,
            // Slave is unable to accept given data (only possible when writing)
            DataNACK,
            // Lost the I2C bus to another master device
            BusLost,
            // Lost the I2C bus to another master device who addressed us (check slave status for more information)
            AddressedAsSlave,
            // Provided timeout expired during operation
            Timeout,
            // Error raised when calling start or stop unexpectedly
            Error,
            // Error was unexpected and its cause is unknown
            Unknown
        };

    protected:
        uint32_t timeout;

        template<typename Member, typename... Args>
        using TimeoutFunction = Status (Member::*)(uint32_t, Args...);

        bool _awaitTWINT(uint32_t t);

        Status _signalStart(uint32_t t);

        Status _signalStopStart(uint32_t t);

        Status _addressSlaveW(uint32_t t, uint8_t address);

        Status _sendData(uint32_t t, uint8_t data);

        Status _sendData(uint32_t t, const uint8_t *data, size_t size);

        Status _addressSlaveR(uint32_t t, uint8_t address);

        Status _receiveData(uint32_t t, uint8_t *data);

        Status _receiveData(uint32_t t, uint8_t *data, size_t size);

        template <typename... Args>
        Status _executeTimedInstruction(TimeoutFunction<MasterConfiguration, Args...> f, Args... args)
        {
            uint32_t t = micros();
            // Execute instruction
            return (this->*f)(t, args...);
        }

    public:
        /**
         * @brief Create Master Configuration
         *
         * @param timeout Timeout in microseconds
         */
        MasterConfiguration(uint32_t timeout);

        /**
         * @brief Create Master Configuration
         *
         */
        MasterConfiguration();

        /**
         * @brief Set the timeout for I2C operations
         *
         * @param timeout Timeout in microseconds
         */
        void setTimeout(uint32_t timeout = DEFAULT_TIMEOUT);

        /**
         * @brief Disables timeout for I2C operations
         *  (in other words it sets timeout to be infinity)
         * 
         */
        void disableTimeout();

        /**
         * @brief Signal start to slave devices
         *
         * @return Status Command status
         */
        Status signalStart();

        /**
         * @brief Signal stop then start (in a single operation)
         *
         * @return Status Command status
         */
        Status signalStopStart();

        /**
         * @brief Address slave for writing
         *
         * @param address Address of the slave device
         * @return Status Command status
         */
        Status addressForWriting(uint8_t address);

        /**
         * @brief Address slave for reading
         *
         * @param address Address of the slave device
         * @return Status Command status
         */
        Status addressForReading(uint8_t address);

        /**
         * @brief Write data to the bus
         *
         * @param data Data to write
         * @return Status Command status
         */
        Status sendData(uint8_t data);
        Status sendData(const uint8_t *data, size_t size);

        /**
         * @brief Read data from the bus
         *
         * @param data Where to store data
         * @return Status Command status
         */
        Status receiveData(uint8_t *data);
        Status receiveData(uint8_t *data, size_t size);
    };
}
