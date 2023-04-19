#pragma once

#include "TwoWireMasterConfiguration.hpp"

namespace TwoWire
{
    class MasterConfig : protected MasterConfiguration
    {
    public:
        enum class BusLostBehaviour : int8_t
        {
            // Terminate instruction
            Abort,
            // Wait until the bus is free
            RetryWithinTimeout,
            RetryExtendingTimeout
        };

    protected:
        BusLostBehaviour busLostBehaviour;

        bool _handleBadStatus(Status s, uint32_t& t);

        Status _send(uint32_t t, uint8_t address, uint8_t data, bool stop);
        Status _send(uint32_t t, uint8_t address, const uint8_t *data, size_t size, bool stop);

        Status _receive(uint32_t t, uint8_t address, uint8_t *data, bool stop);
        Status _receive(uint32_t t, uint8_t address, uint8_t *data, size_t size, bool stop);

        Status _receiveRegister(uint32_t t, uint8_t address, uint8_t registerAddress, uint8_t *data, bool repeatStart, bool stop);
        Status _receiveRegister(uint32_t t, uint8_t address, uint8_t registerAddress, uint8_t *data, size_t size, bool repeatStart, bool stop);
    public:
        using MasterConfiguration::Status;

        using MasterConfiguration::setTimeout;
        using MasterConfiguration::disableTimeout;

        /**
         * @brief Create Master Config
         *
         * @param timeout Timeout in microseconds
         * @param busLostBehaviour Behaviour when the bus is lost to another master
         */
        MasterConfig(uint32_t timeout, BusLostBehaviour behaviour);

        /**
         * @brief Create Master Config
         *
         * @param timeout Timeout in microseconds
         */
        MasterConfig(uint32_t timeout);

        /**
         * @brief Create Master Config
         *
         */
        MasterConfig();

        /**
         * @brief Set the behaviour of the TWI when bus is lost to another master
         *
         * @param behaviour Behaviour to set
         */
        void setBusLostBehaviour(BusLostBehaviour behaviour);

        /**
         * @brief Send data to slave device at address
         *
         * @param address Address of the slave device
         * @param data Data to send
         * @param stop Whether to release the bus on completion
         * @return Status Status of the function
         */
        Status send(uint8_t address, uint8_t data, bool stop);
        Status send(uint8_t address, uint8_t data);
        Status send(uint8_t address, const uint8_t *data, size_t size, bool stop);
        Status send(uint8_t address, const uint8_t *data, size_t size);

        /**
         * @brief Receive data from slave device at address
         *
         * @param address Address of the slave device
         * @param data Where to receive the data
         * @param stop Whether to release the bus on completion
         * @return Status Status of the function
         */
        Status receive(uint8_t address, uint8_t *data, bool stop);
        Status receive(uint8_t address, uint8_t *data);
        Status receive(uint8_t address, uint8_t *data, size_t size, bool stop);
        Status receive(uint8_t address, uint8_t *data, size_t size);

        /**
         * @brief Receive slave device register contents
         *
         * @param address Address of the slave device
         * @param registerAddress Address of the slave device register
         * @param data Where to receive the data
         * @param repeatStart Does the device support repeat start (or should stop start be used)
         * @param stop Whether to release the bus on completion
         * @return Status Status of the function
         */
        Status receiveRegister(uint8_t address, uint8_t registerAddress, uint8_t *data, bool repeatStart, bool stop);
        Status receiveRegister(uint8_t address, uint8_t registerAddress, uint8_t *data, bool repeatStart);
        Status receiveRegister(uint8_t address, uint8_t registerAddress, uint8_t *data, size_t size, bool repeatStart, bool stop);
        Status receiveRegister(uint8_t address, uint8_t registerAddress, uint8_t *data, size_t size, bool repeatStart);
    };
}
