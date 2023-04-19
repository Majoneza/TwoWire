#include <TwoWire.hpp>

constexpr uint8_t twoWireAddress = 0xA;
constexpr uint32_t twoWireFrequency = 100000;

constexpr uint32_t twoWireTimeout = 35000;

void setup()
{
    // Initialize TWI hardware
    TwoWire::init(twoWireAddress, twoWireFrequency);

    // Pullup has to be manually activated for compatibility with 3.3V devices
    TwoWire::activatePullup();

    // Initialize class
    auto m = TwoWire::MasterConfiguration(twoWireTimeout);
    // or
    auto m2 = TwoWire::MasterConfiguration();
    // set timeout later
    m.setTimeout(twoWireTimeout);
    // set default timeout(25000 us)
    m.setTimeout();

    // Peripheral settings
    constexpr uint8_t peripheralAddress = 0xB;
    constexpr uint8_t peripheralSendRegister = 0xC;
    constexpr uint8_t peripheralReceiveRegister = 0xD;
    // Whether the peripheral supports I2C repeated start (set to true if you dont know)
    constexpr bool peripheralSupportsRepeatedStart = false;

    // Storage buffer (can be of any size)
    uint8_t data[2];

    // -- Send --

    if (m.signalStart() != TwoWire::MStatus::Success)
    {
        // something is wrong (necessary to handle)
        // m.signalStop() or m.signalStopStart() or m.acceptBusLost() or TwoWire::clearError() or nothing
    }
    if (m.addressForWriting(peripheralAddress) != TwoWire::MStatus::Success)
    {
        // something is wrong (necessary to handle)
        // m.signalStop() or m.signalStopStart() or m.acceptBusLost() or TwoWire::clearError() or nothing
    }
    // <--!
    TwoWire::MStatus s;
    if ((s = m.sendData(peripheralSendRegister)) != TwoWire::MStatus::Success)
    {
        // something is wrong (necessary to handle)
        // m.signalStop() or m.signalStopStart() or m.acceptBusLost() or TwoWire::clearError() or nothing
    }
    if ((s = m.sendData(0x2)) != TwoWire::MStatus::Success)
    {
        // something is wrong (necessary to handle)
        // m.signalStop() or m.signalStopStart() or m.acceptBusLost() or TwoWire::clearError() or nothing
    }
    // or
    data[0] = peripheralSendRegister;
    data[1] = 0x2;
    if ((s = m.sendData(data, sizeof(data))) != TwoWire::MStatus::Success)
    {
        // something is wrong (necessary to handle)
        // m.signalStop() or m.signalStopStart() or m.acceptBusLost() or TwoWire::clearError() or nothing

        // ex. error handling
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
    // !--> (sendData methods are mostly equivalent but 2nd requires less error handling)
    TwoWire::signalStop();

    //

    // -- Receive --

    if (m.signalStart() != TwoWire::MStatus::Success)
    {
        // something is wrong (necessary to handle)
        // m.signalStop() or m.signalStopStart() or m.acceptBusLost() or TwoWire::clearError() or nothing
    }
    if (m.addressForReading(peripheralAddress) != TwoWire::MStatus::Success)
    {
        // something is wrong (necessary to handle)
        // m.signalStop() or m.signalStopStart() or m.acceptBusLost() or TwoWire::clearError() or nothing
    }
    // <--!
    // Receive single byte
    uint8_t byte_to_receive;
    if (m.receiveData(&byte_to_receive) != TwoWire::MStatus::Success)
    {
        // something is wrong (necessary to handle)
        // m.signalStop() or m.signalStopStart() or m.acceptBusLost() or TwoWire::clearError() or nothing
    }
    // or
    // Receive multiple bytes
    if (m.receiveData(data, sizeof(data)) != TwoWire::MStatus::Success)
    {
        // something is wrong (necessary to handle)
        // m.signalStop() or m.signalStopStart() or m.acceptBusLost() or TwoWire::clearError() or nothing
    }
    // !-->
    TwoWire::signalStop();

    //

    // -- Read from register --

    if (m.signalStart() != TwoWire::MStatus::Success)
    {
        // something is wrong (necessary to handle)
        // m.signalStop() or m.signalStopStart() or m.acceptBusLost() or TwoWire::clearError() or nothing
    }
    if (m.addressForWriting(peripheralAddress) != TwoWire::MStatus::Success)
    {
        // something is wrong (necessary to handle)
        // m.signalStop() or m.signalStopStart() or m.acceptBusLost() or TwoWire::clearError() or nothing
    }
    if (m.sendData(peripheralReceiveRegister) != TwoWire::MStatus::Success)
    {
        // something is wrong (necessary to handle)
        // m.signalStop() or m.signalStopStart() or m.acceptBusLost() or TwoWire::clearError() or nothing
    }
    if ((peripheralSupportsRepeatedStart ? m.signalStart() : m.signalStopStart())
        != TwoWire::MStatus::Success)
    {
        // something is wrong (necessary to handle)
        // m.signalStop() or m.signalStopStart() or m.acceptBusLost() or TwoWire::clearError() or nothing
    }
    if (m.addressForReading(peripheralAddress) != TwoWire::MStatus::Success)
    {
        // something is wrong (necessary to handle)
        // m.signalStop() or m.signalStopStart() or m.acceptBusLost() or TwoWire::clearError() or nothing
    }
    if (m.receiveData(data, sizeof(data)) != TwoWire::MStatus::Success)
    {
        // something is wrong (necessary to handle)
        // m.signalStop() or m.signalStopStart() or m.acceptBusLost() or TwoWire::clearError() or nothing
    }
    TwoWire::signalStop();

    //

    // Make it look like TWI wasnt here
    TwoWire::deactivatePullup();
    TwoWire::disable();
}

void loop()
{
}
