#pragma once

#include "TwoWireCore.hpp"
#include "TwoWireMasterConfiguration.hpp"
#include "TwoWireMasterConfig.hpp"
#include "TwoWireSlave.hpp"
#include "TwoWireSlaveReceiver.hpp"
#include "TwoWireSlaveTransmitter.hpp"

namespace TwoWire
{
    // Master enums
    using MStatus = TwoWire::MasterConfiguration::Status;
    using MBusBehaviour = TwoWire::MasterConfig::BusLostBehaviour;
    // Slave enums
    using SBasicStatus = TwoWire::Slave::BasicStatus;
    using SStatus = TwoWire::Slave::Status;
}
