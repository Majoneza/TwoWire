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
            AcceptedDataReceived,
            DeclinedDataReceived,
            // Transmitter
            AddressedAsTransmitter,
            NextDataAccepted,
            NextDataDeclined,
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
            DirectAcceptedDataReceived,
            DirectDeclinedDataReceived,
            GeneralCallAcceptedDataReceived,
            GeneralCallDeclinedDataReceived,
            // Transmitter
            DirectlyAddressedAsTransmitter,
            BusLostDirectlyAddressedAsTransmitter,
            NextDataAccepted,
            NextDataDeclined,
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
         * @brief Accept next coming data
         *
         */
        void acceptNextData();
        /**
         * @brief Decline next coming data
         *
         */
        void declineNextData();
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
