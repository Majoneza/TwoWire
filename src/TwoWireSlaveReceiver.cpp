#include "TwoWireSlaveReceiver.hpp"

#include "TwoWireCore.hpp"

#include <compat/twi.h>

using namespace TwoWire;

SlaveReceiver::SlaveReceiver(uint8_t *data, size_t size)
    : data(data), size(size), count(0)
{
}

SlaveReceiver::SlaveReceiver(uint8_t *data)
    : data(data), size(1), count(0)
{
}

SlaveReceiver::SlaveReceiver()
    : data(nullptr), size(0), count(0)
{
}

void SlaveReceiver::receiveNextData()
{
    this->count = 0;
}

void SlaveReceiver::receiveNextData(uint8_t *data, size_t size)
{
    this->data = data;
    this->size = size;
    this->count = 0;
}

void SlaveReceiver::receiveNextData(uint8_t *data)
{
    this->data = data;
    this->size = 1;
    this->count = 0;
}

bool SlaveReceiver::isDataAvailable()
{
    return size == count;
}

void SlaveReceiver::interruptVectorRoutine()
{
    switch (TW_STATUS)
    {
    case TW_SR_SLA_ACK:
    case TW_SR_ARB_LOST_SLA_ACK:
    case TW_SR_GCALL_ACK:
    case TW_SR_ARB_LOST_GCALL_ACK:
        if (count < size)
        {
            TWCR = TWCR_W(_BV(TWINT) | _BV(TWEA));
            count = 0;
        }
        else
        {
            TWCR &= ~(_BV(TWEA));
            TWCR = TWCR_W(_BV(TWINT));
        }
        break;
    case TW_SR_DATA_ACK:
    case TW_SR_GCALL_DATA_ACK:
        data[count] = TWDR;
        count++;
        if (count < size)
        {
            TWCR = TWCR_W(_BV(TWINT) | _BV(TWEA));
        }
        else
        {
            TWCR &= ~(_BV(TWEA));
            TWCR = TWCR_W(_BV(TWINT));
        }
        break;
    // Ignore NACK
    }
}
