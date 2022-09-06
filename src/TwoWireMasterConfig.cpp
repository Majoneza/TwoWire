#include "TwoWireMasterConfig.hpp"

using namespace TwoWire;

using Status = MasterConfig::Status;

void MasterConfig::setBusLostBehaviour(BusLostBehaviour behaviour)
{
    this->busLostBehaviour = behaviour;
}

Status MasterConfig::_send(uint32_t t, uint8_t address, uint8_t data, bool stop)
{
    Status s;
    // Send START condition and check status
    s = _signalStart(t);
    if (s != Status::Success)
        return s;
    // Send SLA+W and check status
    s = _addressSlaveW(t, address);
    if (s == Status::BusLost)
        return _handleBusLost(&MasterConfig::_send, t, address, data, stop);
    else if (s != Status::Success)
        return s;
    // Send data and check status
    s = _sendData(t, data);
    if (s == Status::BusLost)
        return _handleBusLost(&MasterConfig::_send, t, address, data, stop);
    else if (s != Status::Success)
        return s;
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
    if (s != Status::Success)
        return s;
    // Send SLA+W and check status
    s = _addressSlaveW(t, address);
    if (s == Status::BusLost)
        return _handleBusLost(&MasterConfig::_send, t, address, data, size, stop);
    else if (s != Status::Success)
        return s;
    // Send data and check status
    s = _sendData(t, data, size);
    if (s == Status::BusLost)
        return _handleBusLost(&MasterConfig::_send, t, address, data, size, stop);
    else if (s != Status::Success)
        return s;
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
    if (s != Status::Success)
        return s;
    // Send SLA+R and check status
    s = _addressSlaveR(t, address);
    if (s == Status::BusLost)
        return _handleBusLost(&MasterConfig::_receive, t, address, data, stop);
    else if (s != Status::Success)
        return s;
    // Read data and check status
    s = _receiveData(t, data);
    if (s == Status::BusLost)
        return _handleBusLost(&MasterConfig::_receive, t, address, data, stop);
    else if (s != Status::Success)
        return s;
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
    if (s != Status::Success)
        return s;
    // Send SLA+R and check status
    s = _addressSlaveR(t, address);
    if (s == Status::BusLost)
        return _handleBusLost(&MasterConfig::_receive, t, address, data, size, stop);
    else if (s != Status::Success)
        return s;
    // Read data and check status
    s = _receiveData(t, data, size);
    if (s == Status::BusLost)
        return _handleBusLost(&MasterConfig::_receive, t, address, data, size, stop);
    else if (s != Status::Success)
        return s;
    // If stop is set, release bus
    if (stop)
        signalStop();
    // Return success
    return Status::Success;
}

Status MasterConfig::send(uint8_t address, uint8_t data, bool stop)
{
    return _executeTimedInstruction(&MasterConfig::_send, address, data, stop);
}

Status MasterConfig::send(uint8_t address, uint8_t data)
{
    return send(address, data, true);
}

Status MasterConfig::send(uint8_t address, const uint8_t *data, size_t size, bool stop)
{
    return _executeTimedInstruction(&MasterConfig::_send, address, data, size, stop);
}

Status MasterConfig::send(uint8_t address, const uint8_t *data, size_t size)
{
    return send(address, data, size, true);
}

Status MasterConfig::receive(uint8_t address, uint8_t *data, bool stop)
{
    return _executeTimedInstruction(&MasterConfig::_receive, address, data, stop);
}

Status MasterConfig::receive(uint8_t address, uint8_t *data)
{
    return receive(address, data, true);
}

Status MasterConfig::receive(uint8_t address, uint8_t *data, size_t size, bool stop)
{
    return _executeTimedInstruction(&MasterConfig::_receive, address, data, size, stop);
}

Status MasterConfig::receive(uint8_t address, uint8_t *data, size_t size)
{
    return receive(address, data, size, true);
}
