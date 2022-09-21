#include <TwoWire.hpp>

constexpr uint8_t twoWireAddress = 0xA;
constexpr uint32_t twoWireFrequency = 100000;

ISR(TWI_vect)
{
    switch (TwoWire::Slave::getBasicStatus())
    {
    case TwoWire::SBasicStatus::AddressedAsReceiver:
        TwoWire::Slave::receiveNextData();
        // or
        TwoWire::Slave::receiveLastData();
        break;
    case TwoWire::SBasicStatus::AddressedAsTransmitter:
        TwoWire::Slave::sendData(0x1);
        // or
        TwoWire::Slave::sendLastData(0x1);
        break;
    case TwoWire::SBasicStatus::DataReceived:
        auto data = TwoWire::Slave::getData();
        // do whatever with data

        TwoWire::Slave::receiveNextData();
        // or
        TwoWire::Slave::receiveLastData();
        break;
    case TwoWire::SBasicStatus::LastDataReceived:
        auto data = TwoWire::Slave::getData();
        // do whatever with data

        // ignore
        break;
    case TwoWire::SBasicStatus::SentDataAccepted:
        TwoWire::Slave::sendData(0x1);
        // or
        TwoWire::Slave::sendLastData(0x1);
        break;
    case TwoWire::SBasicStatus::SentDataDeclined:
        // ignore
        break;
    case TwoWire::SBasicStatus::MoreDataRequest:
        // ignore
        break;
    default:
        break;
    }
}

void setup()
{
    // Initialize TWI hardware
    TwoWire::init(twoWireAddress, twoWireFrequency);

    // Enable interrupt for ISR
    TwoWire::enableInterrupt();
}

void loop()
{
}
