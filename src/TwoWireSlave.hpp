#pragma once

#include "TwoWireCore.hpp"

namespace TwoWire
{
    namespace Slave
    {
        enum class BasicStatus : int8_t
        {
            // General
            NoStatus,
            Error,
            SignalReceived,
            // Receiver
            AddressedAsReceiver,
            DataReceived,
            LastDataReceived,
            // Transmitter
            AddressedAsTransmitter,
            SentDataAccepted,
            SentDataDeclined,
            MoreDataRequest,
            //
            Unknown
        };

        enum class Status : int8_t
        {
            // General
            NoStatus,
            Error,
            SignalReceived,
            // Receiver
            DirectlyAddressedAsReceiver,
            BusLostDirectlyAddressedAsReceiver,
            GeneralCallAddressedAsReceiver,
            BusLostGeneralCallAdressedAsReceiver,
            DirectDataReceived,
            DirectLastDataReceived,
            GeneralCallDataReceived,
            GeneralCallLastDataReceived,
            // Transmitter
            DirectlyAddressedAsTransmitter,
            BusLostDirectlyAddressedAsTransmitter,
            SentDataAccepted,
            SentDataDeclined,
            MoreDataRequest,
            //
            Unknown
        };

        /**
         * @brief Get basic status of the TWI as slave
         *
         * @return BasicStatus Basic status of the TWI
         */
        BasicStatus getBasicStatus();

        /**
         * @brief Get full status of the TWI as slave
         *
         * @return Status Full status of the TWI
         */
        Status getStatus();

        /**
         * @brief Receive next data
         *
         */
        void receiveNextData();
        /**
         * @brief Receive next data declaring its the last
         *
         */
        void receiveLastData();
        /**
         * @brief Get current data
         *
         * @return uint8_t Currently stored data
         */
        uint8_t getData();

        /**
         * @brief Send data to master
         *
         * @param data Data to send
         */
        void sendData(uint8_t data);

        /**
         * @brief Send data to master expecting it to be last
         *
         * @param data Data to send
         */
        void sendLastData(uint8_t data);
    }
}
