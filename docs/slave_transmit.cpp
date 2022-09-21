#include <TwoWire.hpp>

constexpr uint8_t twoWireAddress = 0xA;
constexpr uint32_t twoWireFrequency = 100000;

uint8_t trasmitVariable;
uint8_t trasmitBuffer[2]; // can be of any size
uint8_t secondTrasmitBuffer[3]; // can be of any size

TwoWire::SlaveTransmitter t{};
// or
TwoWire::SlaveTransmitter t2{&trasmitVariable, 1};
// or
TwoWire::SlaveTransmitter t3{trasmitBuffer, sizeof(trasmitBuffer)};

ISR(TWI_vect)
{
    t.interruptVectorRoutine(); // most important part

    // Can also add polling here if you just want to continuously trasmit data
    if (t.isDataTransmitted())
    {
        // Transmit more
        t.transmitDataAgain(); // same data
        // or
        t.transmitData(secondTrasmitBuffer, sizeof(secondTrasmitBuffer)); // new data
    }
}

void setup()
{
    // Initialize TWI hardware
    TwoWire::init(twoWireAddress, twoWireFrequency);

    // Enable interrupt for ISR
    TwoWire::enableInterrupt();

    // Setup slave receiver (if you havent done it with constructor or you want to change it)
    t.transmitData(trasmitBuffer, sizeof(trasmitBuffer));
}

void loop()
{
    // Poll for data trasmitted
    if (t.isDataTransmitted())
    {
        // Transmit more
        t.transmitDataAgain(); // same data
        // or
        t.transmitData(secondTrasmitBuffer, sizeof(secondTrasmitBuffer)); // new data
    }
}
