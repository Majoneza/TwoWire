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

        Status _send(uint32_t t, uint8_t address, uint8_t data, bool stop);
        Status _send(uint32_t t, uint8_t address, const uint8_t *data, size_t size, bool stop);
        
        Status _receive(uint32_t t, uint8_t address, uint8_t *data, bool stop);
        Status _receive(uint32_t t, uint8_t address, uint8_t *data, size_t size, bool stop);

        template<typename... Args>
        Status _handleBusLost(Status (MasterConfig::*f)(uint32_t, Args...), uint32_t t, Args... args)
        {
            switch (busLostBehaviour)
            {
            case BusLostBehaviour::RetryExtendingTimeout:
                t = micros();
                [[fallthrough]];
            case BusLostBehaviour::RetryWithinTimeout:
                return (this->*f)(t, args...);
            case BusLostBehaviour::Abort:
                TWCR |= _BV(TWINT);
            default:
                return Status::BusLost;
            }
        }

        template <typename... Args>
        Status _executeTimedInstruction(Status (MasterConfig::*f)(uint32_t, Args...), Args... args)
        {
            uint32_t t = micros();
            // Execute instruction
            return (this->*f)(t, args...);
        }

    public:
        using MasterConfiguration::Status;

        using MasterConfiguration::MasterConfiguration;

        using MasterConfiguration::setTimeout;
        using MasterConfiguration::signalStop;

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
    };
}
