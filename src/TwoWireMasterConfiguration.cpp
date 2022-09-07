#include "TwoWireMasterConfiguration.hpp"

#include <compat/twi.h>

using namespace TwoWire;

using Status = MasterConfiguration::Status;

MasterConfiguration::MasterConfiguration(uint32_t timeout)
    : timeout(timeout)
{
}

MasterConfiguration::MasterConfiguration()
    : MasterConfiguration((uint32_t)(-1))
{
}

void MasterConfiguration::setTimeout(uint32_t timeout)
{
    this->timeout = timeout;
}

void MasterConfiguration::disableTimeout()
{
    this->timeout = (uint32_t)(-1);
}

void MasterConfiguration::acceptBusLost()
{
    TWCR = TWCR_W(_BV(TWINT));
}

void MasterConfiguration::signalStop()
{
    TWCR = TWCR_W(_BV(TWINT) | _BV(TWSTO));
}

bool MasterConfiguration::_awaitTWINT(uint32_t t)
{
    while (!(TWCR & _BV(TWINT)) && (uint32_t)micros() - t <= timeout)
    {
    }
    return micros() - t > timeout;
}

Status MasterConfiguration::_signalStart(uint32_t t)
{
    // Send START condition
    TWCR = TWCR_W(_BV(TWINT) | _BV(TWSTA));
    // Wait for TWINT or timeout
    if (_awaitTWINT(t))
        return Status::Timeout;
    // Check status
    switch (TW_STATUS)
    {
    case TW_START:
    case TW_REP_START:
        return Status::Success;
    case TW_BUS_ERROR:
        return Status::Error;
    default:
        return Status::Unknown;
    }
}

Status MasterConfiguration::_signalStopStart(uint32_t t)
{
    // Send STOP|START condition
    TWCR = TWCR_W(_BV(TWINT) | _BV(TWSTO) | _BV(TWSTA));
    // Wait for TWINT or timeout
    if (_awaitTWINT(t))
        return Status::Timeout;
    // Check status
    switch (TW_STATUS)
    {
    case TW_START:
    case TW_REP_START:
        return Status::Success;
    case TW_BUS_ERROR:
        return Status::Error;
    default:
        return Status::Unknown;
    }
}

Status MasterConfiguration::_addressSlaveW(uint32_t t, uint8_t address)
{
    // Set address (SLA+W)
    TWDR = (address << 1) | TW_WRITE;
    // Send SLA+W
    TWCR = TWCR_W(_BV(TWINT));
    // Wait for TWINT or timeout
    if (_awaitTWINT(t))
        return Status::Timeout;
    // Check status
    switch (TW_STATUS)
    {
    case TW_MT_SLA_ACK:
        return Status::Success;
    case TW_MT_SLA_NACK:
        signalStop();
        return Status::AddressNACK;
    // No need to stop when arbitration lost
    case TW_MT_ARB_LOST:
        return Status::BusLost;
    case TW_SR_ARB_LOST_SLA_ACK:
    case TW_SR_ARB_LOST_GCALL_ACK:
    case TW_ST_ARB_LOST_SLA_ACK:
        return Status::AddressedAsSlave;
    case TW_BUS_ERROR:
        return Status::Error;
    default:
        return Status::Unknown;
    }
}

Status MasterConfiguration::_sendData(uint32_t t, uint8_t data)
{
    // Set data
    TWDR = data;
    // Send data
    TWCR = TWCR_W(_BV(TWINT));
    // Wait for TWINT or timeout
    if (_awaitTWINT(t))
        return Status::Timeout;
    // Check status
    switch (TW_STATUS)
    {
    case TW_MT_DATA_ACK:
        return Status::Success;
    case TW_MT_DATA_NACK:
        signalStop();
        return Status::DataNACK;
    case TW_MT_ARB_LOST:
        // No need to stop when arbitration lost
        return Status::BusLost;
    case TW_BUS_ERROR:
        return Status::Error;
    default:
        return Status::Unknown;
    }
}

Status MasterConfiguration::_sendData(uint32_t t, const uint8_t *data, size_t size)
{
    while (size > 0)
    {
        auto s = _sendData(*data, t);
        if (s != Status::Success)
            return s;
        data++;
        size--;
    }
    return Status::Success;
}

Status MasterConfiguration::_addressSlaveR(uint32_t t, uint8_t address)
{
    // Set address (SLA+R)
    TWDR = (address << 1) | TW_READ;
    // Send SLA+R
    TWCR = TWCR_W(_BV(TWINT));
    // Wait for TWINT or timeout
    if (_awaitTWINT(t))
        return Status::Timeout;
    // Check status
    switch (TW_STATUS)
    {
    case TW_MR_SLA_ACK:
        return Status::Success;
    case TW_MR_SLA_NACK:
        signalStop();
        return Status::AddressNACK;
    // No need to stop when arbitration lost
    case TW_MR_ARB_LOST:
        return Status::BusLost;
    case TW_SR_ARB_LOST_SLA_ACK:
    case TW_SR_ARB_LOST_GCALL_ACK:
    case TW_ST_ARB_LOST_SLA_ACK:
        return Status::AddressedAsSlave;
    case TW_BUS_ERROR:
        return Status::Error;
    default:
        return Status::Unknown;
    }
}

// TODO: Save TWEA before using it (so it isnt changed after the function)
Status MasterConfiguration::_receiveData(uint32_t t, uint8_t *data)
{
    // Set to read only 1 byte
    TWCR &= ~(_BV(TWEA));
    // Read data
    TWCR = TWCR_W(_BV(TWINT));
    // Wait for TWINT or timeout
    if (_awaitTWINT(t))
        return Status::Timeout;
    // Check status
    switch (TW_STATUS)
    {
    case TW_MR_DATA_NACK:
        *data = TWDR;
        return Status::Success;
    case TW_MR_ARB_LOST:
        // No need to stop when arbitration lost
        return Status::BusLost;
    case TW_BUS_ERROR:
        return Status::Error;
    default:
        return Status::Unknown;
    }
}

// TODO: Save TWEA before using it (so it isnt changed after the function)
Status MasterConfiguration::_receiveData(uint32_t t, uint8_t *data, size_t size)
{
    while (size > 1)
    {
        // Set to read more than 1 byte
        TWCR = TWCR_W(_BV(TWINT) | _BV(TWEA));
        // Wait for TWINT or timeout
        if (_awaitTWINT(t))
            return Status::Timeout;
        // Check status
        switch (TW_STATUS)
        {
        case TW_MR_DATA_ACK:
            break;
        case TW_MR_ARB_LOST:
            // No need to stop when arbitration lost
            return Status::BusLost;
        case TW_BUS_ERROR:
            return Status::Error;
        default:
            return Status::Unknown;
        }
        *data = TWDR;
        data++;
        size--;
    }
    return _receiveData(t, data);
}

Status MasterConfiguration::signalStart()
{
    return _executeTimedInstruction(&MasterConfiguration::_signalStart);
}

Status MasterConfiguration::signalStopStart()
{
    return _executeTimedInstruction(&MasterConfiguration::_signalStopStart);
}

Status MasterConfiguration::addressForWriting(uint8_t address)
{
    return _executeTimedInstruction(&MasterConfiguration::_addressSlaveW, address);
}

Status MasterConfiguration::addressForReading(uint8_t address)
{
    return _executeTimedInstruction(&MasterConfiguration::_addressSlaveR, address);
}

Status MasterConfiguration::sendData(uint8_t data)
{
    return _executeTimedInstruction(&MasterConfiguration::_sendData, data);
}

Status MasterConfiguration::sendData(const uint8_t *data, size_t size)
{
    return _executeTimedInstruction(&MasterConfiguration::_sendData, data, size);
}

Status MasterConfiguration::receiveData(uint8_t *data)
{
    return _executeTimedInstruction(&MasterConfiguration::_receiveData, data);
}

Status MasterConfiguration::receiveData(uint8_t *data, size_t size)
{
    return _executeTimedInstruction(&MasterConfiguration::_receiveData, data, size);
}
