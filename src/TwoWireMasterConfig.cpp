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
        return _handleUnsuccessfulStatus(s, &MasterConfig::_send, t, address, data, stop);
    // Send SLA+W and check status
    s = _addressSlaveW(t, address);
    if (s != Status::Success)
        return _handleUnsuccessfulStatus(s, &MasterConfig::_send, t, address, data, stop);
    // Send data and check status
    s = _sendData(t, data);
    if (s != Status::Success)
        return _handleUnsuccessfulStatus(s, &MasterConfig::_send, t, address, data, stop);
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
        return _handleUnsuccessfulStatus(s, &MasterConfig::_send, t, address, data, size, stop);
    // Send SLA+W and check status
    s = _addressSlaveW(t, address);
    if (s != Status::Success)
        return _handleUnsuccessfulStatus(s, &MasterConfig::_send, t, address, data, size, stop);
    // Send data and check status
    s = _sendData(t, data, size);
    if (s != Status::Success)
        return _handleUnsuccessfulStatus(s, &MasterConfig::_send, t, address, data, size, stop);
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
        return _handleUnsuccessfulStatus(s, &MasterConfig::_receive, t, address, data, stop);
    // Send SLA+R and check status
    s = _addressSlaveR(t, address);
    if (s != Status::Success)
        return _handleUnsuccessfulStatus(s, &MasterConfig::_receive, t, address, data, stop);
    // Read data and check status
    s = _receiveData(t, data);
    if (s != Status::Success)
        return _handleUnsuccessfulStatus(s, &MasterConfig::_receive, t, address, data, stop);
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
        return _handleUnsuccessfulStatus(s, &MasterConfig::_receive, t, address, data, size, stop);
    // Send SLA+R and check status
    s = _addressSlaveR(t, address);
    if (s != Status::Success)
        return _handleUnsuccessfulStatus(s, &MasterConfig::_receive, t, address, data, size, stop);
    // Read data and check status
    s = _receiveData(t, data, size);
    if (s != Status::Success)
        return _handleUnsuccessfulStatus(s, &MasterConfig::_receive, t, address, data, size, stop);
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
    if (s != Status::Success)
        return _handleUnsuccessfulStatus(s, &MasterConfig::_receiveRegister, t, address, registerAddress, data, repeatStart, stop);
    // Send SLA+W and check status
    s = _addressSlaveW(t, address);
    if (s != Status::Success)
        return _handleUnsuccessfulStatus(s, &MasterConfig::_receiveRegister, t, address, registerAddress, data, repeatStart, stop);
    // Send data and check status
    s = _sendData(t, registerAddress);
    if (s != Status::Success)
        return _handleUnsuccessfulStatus(s, &MasterConfig::_receiveRegister, t, address, registerAddress, data, repeatStart, stop);
    // Restart or StopStart
    s = repeatStart ? _signalStart(t) : _signalStopStart(t);
    if (s != Status::Success)
        return _handleUnsuccessfulStatus(s, &MasterConfig::_receiveRegister, t, address, registerAddress, data, repeatStart, stop);
    // Send SLA+R and check status
    s = _addressSlaveR(t, address);
    if (s != Status::Success)
        return _handleUnsuccessfulStatus(s, &MasterConfig::_receiveRegister, t, address, registerAddress, data, repeatStart, stop);
    // Read data and check status
    s = _receiveData(t, data);
    if (s != Status::Success)
        return _handleUnsuccessfulStatus(s, &MasterConfig::_receiveRegister, t, address, registerAddress, data, repeatStart, stop);
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
    if (s != Status::Success)
        return _handleUnsuccessfulStatus(s, &MasterConfig::_receiveRegister, t, address, registerAddress, data, size, repeatStart, stop);
    // Send SLA+W and check status
    s = _addressSlaveW(t, address);
    if (s != Status::Success)
        return _handleUnsuccessfulStatus(s, &MasterConfig::_receiveRegister, t, address, registerAddress, data, size, repeatStart, stop);
    // Send data and check status
    s = _sendData(t, registerAddress);
    if (s != Status::Success)
        return _handleUnsuccessfulStatus(s, &MasterConfig::_receiveRegister, t, address, registerAddress, data, size, repeatStart, stop);
    // Restart or StopStart
    s = repeatStart ? _signalStart(t) : _signalStopStart(t);
    if (s != Status::Success)
        return _handleUnsuccessfulStatus(s, &MasterConfig::_receiveRegister, t, address, registerAddress, data, size, repeatStart, stop);
    // Send SLA+R and check status
    s = _addressSlaveR(t, address);
    if (s != Status::Success)
        return _handleUnsuccessfulStatus(s, &MasterConfig::_receiveRegister, t, address, registerAddress, data, size, repeatStart, stop);
    // Read data and check status
    s = _receiveData(t, data, size);
    if (s != Status::Success)
        return _handleUnsuccessfulStatus(s, &MasterConfig::_receiveRegister, t, address, registerAddress, data, size, repeatStart, stop);
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

Status MasterConfig::receiveRegister(uint8_t address, uint8_t registerAddress, uint8_t *data, bool repeatStart, bool stop)
{
    return _executeTimedInstruction(&MasterConfig::_receiveRegister, address, registerAddress, data, repeatStart, stop);
}

Status MasterConfig::receiveRegister(uint8_t address, uint8_t registerAddress, uint8_t *data, bool repeatStart)
{
    return receiveRegister(address, registerAddress, data, repeatStart, true);
}

Status MasterConfig::receiveRegister(uint8_t address, uint8_t registerAddress, uint8_t *data, size_t size, bool repeatStart, bool stop)
{
    return _executeTimedInstruction(&MasterConfig::_receiveRegister, address, registerAddress, data, size, repeatStart, stop);
}

Status MasterConfig::receiveRegister(uint8_t address, uint8_t registerAddress, uint8_t *data, size_t size, bool repeatStart)
{
    return receiveRegister(address, registerAddress, data, size, repeatStart, true);
}
