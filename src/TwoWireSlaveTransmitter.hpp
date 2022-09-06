#pragma once

#include <stdint.h>
#include <stddef.h>

namespace TwoWire
{
    class SlaveTransmitter
    {
    private:
        const uint8_t* data;
        size_t size;
        size_t count;

    public:
        /**
         * @brief Construct Slave trasmitter with data to trasmit
         *  (to trasmit a single uint8_t variable set size to be 1)
         * 
         * @param data Buffer of data to trasmit
         * @param size Size of the buffer
         */
        SlaveTransmitter(const uint8_t *data, size_t size);

        /**
         * @brief Construct Slave trasmitter
         *  (data location has to be specified before the class is able to trasmit any data)
         * 
         */
        SlaveTransmitter();

        /**
         * @brief Trasmit data from the same location
         * 
         */
        void transmitDataAgain();

        /**
         * @brief Trasmit data from a new location
         * 
         * @param data 
         * @param size 
         */
        void transmitData(const uint8_t *data, size_t size);

        /**
         * @brief Check whether the storage location has been trasmitted
         * 
         * @return true Storage location has been trasmitted
         * @return false Waiting for connections
         */
        bool isDataTransmitted();

        /**
         * @brief Function to be called in TWI Interrupt Service Routine
         * 
         */
        void interruptVectorRoutine();
    };
}
