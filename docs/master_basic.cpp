#include <TwoWire.hpp>

constexpr uint8_t twoWireAddress = 0xA;
constexpr uint32_t twoWireFrequency = 100000;

constexpr uint32_t twoWireTimeout = 35000;
constexpr TwoWire::MBusLostBehaviour twoWireBusLostBehaviour = TwoWire::MBusLostBehaviour::RetryWithinTimeout;

void setup()
{
    // Initialize TWI hardware
    TwoWire::init(twoWireAddress, twoWireFrequency);

    // Pullup has to be manually activated for compatibility with 3.3V devices
    TwoWire::activatePullup();

    // Initialize class
    auto m = TwoWire::MasterConfig(twoWireTimeout, twoWireBusLostBehaviour);
    // or
    auto m2 = TwoWire::MasterConfig(twoWireTimeout);
    // or
    auto m3 = TwoWire::MasterConfig();
    // set timeout later
    m.setTimeout(twoWireTimeout);
    // set default timeout(25000 us)
    m.setTimeout();
    // set bus lost behaviour
    m.setBusLostBehaviour(twoWireBusLostBehaviour);

    // Peripheral settings
    constexpr uint8_t peripheralAddress = 0xB;
    constexpr uint8_t peripheralSendRegister = 0xC;
    constexpr uint8_t peripheralReceiveRegister = 0xD;
    // Whether the peripheral supports I2C repeated start (set to true if you dont know)
    constexpr bool peripheralSupportsRepeatedStart = false;

    // Storage buffer (can be of any size)
    uint8_t data[2];

    // -- Send --

    // - Send single byte -
    constexpr uint8_t byte_to_send = 0x2;
    if (m.send(peripheralAddress, byte_to_send) != TwoWire::MStatus::Success)
    {
        // something is wrong (unnecessary to handle)
    }

    // - Send multiple bytes -
    data[0] = 0x3; // first byte
    data[1] = 0x5; // second byte
    if (m.send(peripheralAddress, data, sizeof(data)) != TwoWire::MStatus::Success)
    {
        // something is wrong (unnecessary to handle)
    }
    
    //

    // -- Receive --

    // - Receive single byte -
    uint8_t byte_to_receive;
    if (m.receive(peripheralAddress, &byte_to_receive) != TwoWire::MStatus::Success)
    {
        // something is wrong (unnecessary to handle)
    }

    // - Receive multiple bytes -
    // allocate array (we already have uint8_t data[])
    if (m.receive(peripheralAddress, data, (size_t)sizeof(data)) != TwoWire::MStatus::Success)
    {
        // something is wrong (unnecessary to handle)
    }

    //

    // -- Write to register --

    data[0] = peripheralSendRegister; // register address
    data[1] = 0x2; // register data
    if (m.send(peripheralAddress, data, sizeof(data)) != TwoWire::MStatus::Success)
    {
        // something is wrong (unnecessary to handle)
    }

    //

    // -- Read from register --
    
    // - Easy way -
    if (m.receiveRegister(peripheralAddress, peripheralReceiveRegister, data, (size_t)sizeof(data), peripheralSupportsRepeatedStart)
        != TwoWire::MStatus::Success)
    {
        // something is wrong (unnecessary to handle)
    }

    // - Difficult way -
    if (m.send(peripheralAddress, peripheralReceiveRegister, !peripheralSupportsRepeatedStart)
        != TwoWire::MStatus::Success)
    {
        // something is wrong (unnecessary to handle)
    }
    if (m.receive(peripheralAddress, data, (size_t)sizeof(data)) != TwoWire::MStatus::Success)
    {
        // something is wrong (unnecessary to handle)
    }

    //

    // Make it look like TWI wasnt here
    TwoWire::deactivatePullup();
    TwoWire::disable();
}

void loop()
{
}
