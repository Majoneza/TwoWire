#pragma once

#include <stdint.h>
#include <stddef.h>

namespace TwoWire
{
    class SlaveReceiver
    {
    private:
        uint8_t *data;
        size_t size;
        size_t count;

    public:
        /**
         * @brief Construct Slave receiver with save location
         * 
         * @param data Buffer to which data will be stored
         * @param size Size of the buffer
         */
        SlaveReceiver(uint8_t *data, size_t size);

        /**
         * @brief Construct Slave receiver with save location
         * 
         * @param data Variable to which data will be stored
         */
        SlaveReceiver(uint8_t *data);

        /**
         * @brief Construct Slave receiver
         *  (data location has to be specified before the class is able to receive any data)
         * 
         */
        SlaveReceiver();

        /**
         * @brief Instruct to receive data to the same storage location
         * 
         */
        void receiveNextData();

        /**
         * @brief Instruct to receive data to a new storage location
         * 
         * @param data Variable to which data will be stored
         * @param size Size of the storage variable
         */
        void receiveNextData(uint8_t *data, size_t size);
        void receiveNextData(uint8_t *data);

        /**
         * @brief Check whether the storage location has been filled
         * 
         * @return true Storage location is filled with data
         * @return false Still waiting for data
         */
        bool isDataAvailable();

        /**
         * @brief Function to be called in TWI Interrupt Service Routine
         * 
         */
        void interruptVectorRoutine();
    };
}
