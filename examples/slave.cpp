#include <TwoWire.hpp>

constexpr uint8_t twoWireAddress = 0xA;
constexpr uint32_t twoWireFrequency = 100000;

volatile uint8_t trasmitBuffer[2] = {0xB, 0xC};
volatile uint8_t receiveBuffer[2];

volatile bool dataReceived = false;

ISR(TWI_vect)
{
    switch (TwoWire::Slave::getBasicStatus())
    {
    case TwoWire::SBasicStatus::AddressedAsReceiver:
        if (dataReceived)
            TwoWire::Slave::receiveLastData();
        else
            TwoWire::Slave::receiveNextData();
        break;
    case TwoWire::SBasicStatus::AddressedAsTransmitter:
        TwoWire::Slave::sendData(trasmitBuffer[0]);
        break;
    case TwoWire::SBasicStatus::DataReceived:
        receiveBuffer[0] = TwoWire::Slave::getData();
        TwoWire::Slave::receiveLastData();
        break;
    case TwoWire::SBasicStatus::LastDataReceived:
        if (!dataReceived)
            receiveBuffer[1] = TwoWire::Slave::getData();
        break;
    case TwoWire::SBasicStatus::SentDataAccepted:
        TwoWire::Slave::sendLastData(trasmitBuffer[1]);
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
    if (dataReceived)
    {
        Serial.write(receiveBuffer[0]);
        Serial.write(receiveBuffer[1]);
        Serial.println();

        dataReceived = false;
    }
}
