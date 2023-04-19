#include <TwoWire.hpp>

constexpr uint8_t twoWireAddress = 0xA;
constexpr uint32_t twoWireFrequency = 100000;

constexpr uint32_t twoWireTimeout = 35000;

void handleStatus(TwoWire::MasterConfiguration& m, TwoWire::MStatus& s)
{
    switch (s)
    {
    case TwoWire::MStatus::AddressNACK:
    case TwoWire::MStatus::DataNACK:
        TwoWire::signalStop();
        break;
    case TwoWire::MStatus::BusLost:
        TwoWire::acknowledgeStatus(); // accept bus lost (should then check slave status TwoWire::Slave::getStatus())
        // or
        m.signalStart(); // wait for other master to finish communication
        break;
    case TwoWire::MStatus::AddressedAsSlave:
        // You can disallow this device to be addressed as slave by TwoWire::disallowSlaveMode();
        switch (TwoWire::Slave::getBasicStatus())
        {
        case TwoWire::SBasicStatus::AddressedAsReceiver:
            TwoWire::Slave::receiveLastData();
            // or
            TwoWire::Slave::receiveNextData();
            break;
        case TwoWire::SBasicStatus::AddressedAsTransmitter:
            uint8_t junkData = 0x0;
            TwoWire::Slave::sendLastData(junkData);
            // or
            uint8_t valuableData = 0x1;
            TwoWire::Slave::sendData(valuableData);
            break;
        default:
            break;
        }
        break;
    case TwoWire::MStatus::Timeout:
        // ignore
        break;
    case TwoWire::MStatus::Error:
        TwoWire::clearError();
        break;
    case TwoWire::MStatus::Unknown:
        // improbable
        break;
    default:
        // impossible
        break;
    }
}

void setup()
{
    // Initialize TWI hardware
    TwoWire::init(twoWireAddress, twoWireFrequency);

    // Initialize class
    auto m = TwoWire::MasterConfiguration(twoWireTimeout);

    // Peripheral settings
    constexpr uint8_t peripheralAddress = 0xB;
    constexpr uint8_t peripheralRegister = 0xC;

    // Write to register
    TwoWire::MStatus s;
    if ((s = m.signalStart()) != TwoWire::MStatus::Success)
    {
        // something is wrong (necessary to handle)
        // m.signalStop() or m.signalStopStart() or m.acceptBusLost() or TwoWire::clearError() or nothing
        handleStatus(m, s);
    }
    if ((s = m.addressForWriting(peripheralAddress)) != TwoWire::MStatus::Success)
    {
        // something is wrong (necessary to handle)
        // m.signalStop() or m.signalStopStart() or m.acceptBusLost() or TwoWire::clearError() or nothing
        handleStatus(m, s);
    }
    if ((s = m.sendData(peripheralRegister)) != TwoWire::MStatus::Success)
    {
        // something is wrong (necessary to handle)
        // m.signalStop() or m.signalStopStart() or m.acceptBusLost() or TwoWire::clearError() or nothing
        handleStatus(m, s);
    }
    if ((s = m.sendData(0x2)) != TwoWire::MStatus::Success)
    {
        // something is wrong (necessary to handle)
        // m.signalStop() or m.signalStopStart() or m.acceptBusLost() or TwoWire::clearError() or nothing
        handleStatus(m, s);
    }
    TwoWire::signalStop();

    // Make it look like TWI wasnt here
    TwoWire::disable();
}

void loop()
{
}
