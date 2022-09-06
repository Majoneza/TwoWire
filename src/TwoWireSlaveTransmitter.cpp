#include "TwoWireSlaveTransmitter.hpp"

#include <compat/twi.h>

using namespace TwoWire;

SlaveTransmitter::SlaveTransmitter(const uint8_t *data, size_t size)
    : data(data), size(size), count(0)
{
}

SlaveTransmitter::SlaveTransmitter()
    : data(nullptr), size(0), count(0)
{
}

void SlaveTransmitter::transmitDataAgain()
{
    this->count = 0;
}

void SlaveTransmitter::transmitData(const uint8_t *data, size_t size)
{
    this->data = data;
    this->size = size;
    this->count = 0;
}

bool SlaveTransmitter::isDataTransmitted()
{
    return size == count;
}

void SlaveTransmitter::interruptVectorRoutine()
{
    switch (TW_STATUS)
    {
    case TW_ST_SLA_ACK:
    case TW_ST_ARB_LOST_SLA_ACK:
        if (count < size)
        {
            count = 0;
        }
        [[fallthrough]];
    case TW_ST_DATA_ACK:
        if (count < size)
        {
            TWDR = data[count];
            TWCR |= _BV(TWINT) | _BV(TWEA);
            count++;
        }
        else
        {
            TWCR &= ~(_BV(TWEA));
            TWCR |= _BV(TWINT);
        }
        break;
    // Ignore NACK or LAST_DATA
    }
}
