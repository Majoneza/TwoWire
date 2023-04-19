#include <TwoWire.hpp>

constexpr uint8_t twoWireAddress = 0xA;
constexpr uint32_t twoWireFrequency = 100000;

constexpr uint32_t twoWireTimeout = 35000;
constexpr TwoWire::MBusLostBehaviour twoWireBusLostBehaviour = TwoWire::MBusLostBehaviour::RetryWithinTimeout;

void setup()
{
    // Init serial
    Serial.begin(9600);

    // Initialize TWI hardware
    TwoWire::init(twoWireAddress, twoWireFrequency);

    // Initialize class
    auto m = TwoWire::MasterConfig(twoWireTimeout, twoWireBusLostBehaviour);

    // Peripheral settings
    constexpr uint8_t peripheralAddress = 0xB;
    constexpr uint8_t peripheralRegister = 0xC;

    // Write to register
    uint8_t data[2];
    data[0] = peripheralRegister; // register address
    data[1] = 0x2; // register data
    TwoWire::MStatus s;
    if ((s = m.send(peripheralAddress, data, sizeof(data))) != TwoWire::MStatus::Success)
    {
        // something is wrong (unnecessary to handle)
        Serial.print("TwoWire error: ");
        Serial.write((uint8_t)s);
        Serial.println();
    }

    // Make it look like TWI wasnt here
    TwoWire::disable();
}

void loop()
{
}
