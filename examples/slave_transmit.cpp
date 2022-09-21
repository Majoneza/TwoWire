#include <TwoWire.hpp>

constexpr uint8_t twoWireAddress = 0xA;
constexpr uint32_t twoWireFrequency = 100000;

uint8_t trasmitBuffer[2] = {0xB, 0xC};

TwoWire::SlaveTransmitter t{trasmitBuffer, sizeof(trasmitBuffer)};

ISR(TWI_vect)
{
    t.interruptVectorRoutine();
    
    // This is better than placing it in loop (in this use case)
    // Because void loop() would be checking for data trasmitted when there is no indication, unlike here
    if (t.isDataTransmitted())
    {
        // Transmit same data again
        t.transmitDataAgain();
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
