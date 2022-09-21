#include <TwoWire.hpp>

constexpr uint8_t twoWireAddress = 0xA;
constexpr uint32_t twoWireFrequency = 100000;

uint8_t receiveBuffer[2];

TwoWire::SlaveReceiver r{receiveBuffer, sizeof(receiveBuffer)};

ISR(TWI_vect)
{
    r.interruptVectorRoutine();
}

void setup()
{
    // Setup serial
    Serial.begin(9600);

    // Initialize TWI hardware
    TwoWire::init(twoWireAddress, twoWireFrequency);

    // Enable interrupt for ISR
    TwoWire::enableInterrupt();
}

void loop()
{
    // Poll for data received
    if (r.isDataAvailable())
    {
        // Use data
        Serial.write(receiveBuffer[0]);
        Serial.write(receiveBuffer[1]);
        Serial.println();

        // Receive more
        r.receiveNextData();
    }
}
