#include <TwoWire.hpp>

constexpr uint8_t twoWireAddress = 0xA;
constexpr uint32_t twoWireFrequency = 100000;

uint8_t receiveVariable;
uint8_t receiveBuffer[2]; // can be of any size
uint8_t secondReceiveBuffer[3]; // can be of any size

TwoWire::SlaveReceiver r{};
// or
TwoWire::SlaveReceiver r2{&receiveVariable};
// or
TwoWire::SlaveReceiver r3{receiveBuffer, sizeof(receiveBuffer)};

ISR(TWI_vect)
{
    r.interruptVectorRoutine(); // most important part
}

void setup()
{
    // Initialize TWI hardware
    TwoWire::init(twoWireAddress, twoWireFrequency);

    // Enable interrupt for ISR
    TwoWire::enableInterrupt();

    // Setup slave receiver (if you havent done it with constructor or you want to change it)
    r.receiveNextData(receiveBuffer, sizeof(receiveBuffer));
}

void loop()
{
    // Poll for data received
    if (r.isDataAvailable())
    {
        // Use data
        receiveBuffer;

        // Receive more
        r.receiveNextData(); // same location
        // or
        r.receiveNextData(secondReceiveBuffer, sizeof(secondReceiveBuffer)); // different location
    }
}
