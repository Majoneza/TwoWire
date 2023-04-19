#include "TwoWireMasterConfig.hpp"

#define CHECK_RETURN_STATUS(expression) \
    if (s != Status::Success) \
    { \
        if (_handleBadStatus(s, t)) \
            return expression; \
        return s; \
    }

using namespace TwoWire;

using Status = MasterConfig::Status;

MasterConfig::MasterConfig(uint32_t timeout, BusLostBehaviour behaviour)
    : MasterConfiguration(timeout), busLostBehaviour(behaviour)
{
}

MasterConfig::MasterConfig(uint32_t timeout)
    : MasterConfiguration(timeout), busLostBehaviour(BusLostBehaviour::Abort)
{
}

MasterConfig::MasterConfig()
    : MasterConfiguration(), busLostBehaviour(BusLostBehaviour::Abort)
{
}

void MasterConfig::setBusLostBehaviour(BusLostBehaviour behaviour)
{
    this->busLostBehaviour = behaviour;
}

bool MasterConfig::_handleBadStatus(Status s, uint32_t& t)
{
    switch (s)
    {
    case Status::BusLost:
        switch (busLostBehaviour)
        {
        case BusLostBehaviour::RetryExtendingTimeout:
            t = micros();
            [[fallthrough]];
        case BusLostBehaviour::RetryWithinTimeout:
            return true;
        case BusLostBehaviour::Abort:
            TWCR |= _BV(TWINT);
        }
    case Status::AddressNACK:
        break;
    case Status::DataNACK:
        signalStop();
        break;
    case Status::Error:
        clearError();
        break;
    default:
        break;
    }
    return false;
}

Status MasterConfig::_send(uint32_t t, uint8_t address, uint8_t data, bool stop)
{
    Status s;
    // Send START condition and check status
    s = _signalStart(t);
    CHECK_RETURN_STATUS(_send(t, address, data, stop));
    // Send SLA+W and check status
    s = _addressSlaveW(t, address);
    CHECK_RETURN_STATUS(_send(t, address, data, stop));
    // Send data and check status
    s = _sendData(t, data);
    CHECK_RETURN_STATUS(_send(t, address, data, stop));
    // If stop is set, release bus
    if (stop)
        signalStop();
    // Return success
    return Status::Success;
}

Status MasterConfig::_send(uint32_t t, uint8_t address, const uint8_t *data, size_t size, bool stop)
{
    Status s;
    // Send START condition and check status
    s = _signalStart(t);
    CHECK_RETURN_STATUS(_send(t, address, data, size, stop));
    // Send SLA+W and check status
    s = _addressSlaveW(t, address);
    CHECK_RETURN_STATUS(_send(t, address, data, size, stop));
    // Send data and check status
    s = _sendData(t, data, size);
    CHECK_RETURN_STATUS(_send(t, address, data, size, stop));
    // If stop is set, release bus
    if (stop)
        signalStop();
    // Return success
    return Status::Success;
}

Status MasterConfig::_receive(uint32_t t, uint8_t address, uint8_t *data, bool stop)
{
    Status s;
    // Send START condition and check status
    s = _signalStart(t);
    CHECK_RETURN_STATUS(_receive(t, address, data, stop));
    // Send SLA+R and check status
    s = _addressSlaveR(t, address);
    CHECK_RETURN_STATUS(_receive(t, address, data, stop));
    // Read data and check status
    s = _receiveData(t, data);
    CHECK_RETURN_STATUS(_receive(t, address, data, stop));
    // If stop is set, release bus
    if (stop)
        signalStop();
    // Return success
    return Status::Success;
}

Status MasterConfig::_receive(uint32_t t, uint8_t address, uint8_t *data, size_t size, bool stop)
{
    Status s;
    // Send START condition and check status
    s = _signalStart(t);
    CHECK_RETURN_STATUS(_receive(t, address, data, size, stop));
    // Send SLA+R and check status
    s = _addressSlaveR(t, address);
    CHECK_RETURN_STATUS(_receive(t, address, data, size, stop));
    // Read data and check status
    s = _receiveData(t, data, size);
    CHECK_RETURN_STATUS(_receive(t, address, data, size, stop));
    // If stop is set, release bus
    if (stop)
        signalStop();
    // Return success
    return Status::Success;
}

Status MasterConfig::_receiveRegister(uint32_t t, uint8_t address, uint8_t registerAddress, uint8_t *data, bool repeatStart, bool stop)
{
    Status s;
    // Send START condition and check status
    s = _signalStart(t);
    CHECK_RETURN_STATUS(_receiveRegister(t, address, registerAddress, data, repeatStart, stop));
    // Send SLA+W and check status
    s = _addressSlaveW(t, address);
    CHECK_RETURN_STATUS(_receiveRegister(t, address, registerAddress, data, repeatStart, stop));
    // Send data and check status
    s = _sendData(t, registerAddress);
    CHECK_RETURN_STATUS(_receiveRegister(t, address, registerAddress, data, repeatStart, stop));
    // Restart or StopStart
    s = repeatStart ? _signalStart(t) : _signalStopStart(t);
    CHECK_RETURN_STATUS(_receiveRegister(t, address, registerAddress, data, repeatStart, stop));
    // Send SLA+R and check status
    s = _addressSlaveR(t, address);
    CHECK_RETURN_STATUS(_receiveRegister(t, address, registerAddress, data, repeatStart, stop));
    // Read data and check status
    s = _receiveData(t, data);
    CHECK_RETURN_STATUS(_receiveRegister(t, address, registerAddress, data, repeatStart, stop));
    // If stop is set, release bus
    if (stop)
        signalStop();
    // Return success
    return Status::Success;
}

Status MasterConfig::_receiveRegister(uint32_t t, uint8_t address, uint8_t registerAddress, uint8_t *data, size_t size, bool repeatStart, bool stop)
{
    Status s;
    // Send START condition and check status
    s = _signalStart(t);
    CHECK_RETURN_STATUS(_receiveRegister(t, address, registerAddress, data, size, repeatStart, stop));
    // Send SLA+W and check status
    s = _addressSlaveW(t, address);
    CHECK_RETURN_STATUS(_receiveRegister(t, address, registerAddress, data, size, repeatStart, stop));
    // Send data and check status
    s = _sendData(t, registerAddress);
    CHECK_RETURN_STATUS(_receiveRegister(t, address, registerAddress, data, size, repeatStart, stop));
    // Restart or StopStart
    s = repeatStart ? _signalStart(t) : _signalStopStart(t);
    CHECK_RETURN_STATUS(_receiveRegister(t, address, registerAddress, data, size, repeatStart, stop));
    // Send SLA+R and check status
    s = _addressSlaveR(t, address);
    CHECK_RETURN_STATUS(_receiveRegister(t, address, registerAddress, data, size, repeatStart, stop));
    // Read data and check status
    s = _receiveData(t, data, size);
    CHECK_RETURN_STATUS(_receiveRegister(t, address, registerAddress, data, size, repeatStart, stop));
    // If stop is set, release bus
    if (stop)
        signalStop();
    // Return success
    return Status::Success;
}

Status MasterConfig::send(uint8_t address, uint8_t data, bool stop)
{
    RETURN_EXECUTE_TIMED_FUNCTION(_send, address, data, stop);
}

Status MasterConfig::send(uint8_t address, uint8_t data)
{
    return send(address, data, true);
}

Status MasterConfig::send(uint8_t address, const uint8_t *data, size_t size, bool stop)
{
    RETURN_EXECUTE_TIMED_FUNCTION(_send, address, data, size, stop);
}

Status MasterConfig::send(uint8_t address, const uint8_t *data, size_t size)
{
    return send(address, data, size, true);
}

Status MasterConfig::receive(uint8_t address, uint8_t *data, bool stop)
{
    RETURN_EXECUTE_TIMED_FUNCTION(_receive, address, data, stop);
}

Status MasterConfig::receive(uint8_t address, uint8_t *data)
{
    return receive(address, data, true);
}

Status MasterConfig::receive(uint8_t address, uint8_t *data, size_t size, bool stop)
{
    RETURN_EXECUTE_TIMED_FUNCTION(_receive, address, data, size, stop);
}

Status MasterConfig::receive(uint8_t address, uint8_t *data, size_t size)
{
    return receive(address, data, size, true);
}

Status MasterConfig::receiveRegister(uint8_t address, uint8_t registerAddress, uint8_t *data, bool repeatStart, bool stop)
{
    RETURN_EXECUTE_TIMED_FUNCTION(_receiveRegister, address, registerAddress, data, repeatStart, stop);
}

Status MasterConfig::receiveRegister(uint8_t address, uint8_t registerAddress, uint8_t *data, bool repeatStart)
{
    return receiveRegister(address, registerAddress, data, repeatStart, true);
}

Status MasterConfig::receiveRegister(uint8_t address, uint8_t registerAddress, uint8_t *data, size_t size, bool repeatStart, bool stop)
{
    RETURN_EXECUTE_TIMED_FUNCTION(_receiveRegister, address, registerAddress, data, size, repeatStart, stop);
}

Status MasterConfig::receiveRegister(uint8_t address, uint8_t registerAddress, uint8_t *data, size_t size, bool repeatStart)
{
    return receiveRegister(address, registerAddress, data, size, repeatStart, true);
}
