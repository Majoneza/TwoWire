#include "TwoWireSlave.hpp"

#include <compat/twi.h>

using namespace TwoWire;

Slave::BasicStatus Slave::getBasicStatus()
{
    switch (TW_STATUS)
    {
    // General
    case TW_NO_INFO:
        return BasicStatus::NoStatus;
    case TW_BUS_ERROR:
        return BasicStatus::Error;
    case TW_SR_STOP:
        return BasicStatus::SignalReceived;
    // Receiver
    case TW_SR_SLA_ACK:
    case TW_SR_ARB_LOST_SLA_ACK:
    case TW_SR_GCALL_ACK:
    case TW_SR_ARB_LOST_GCALL_ACK:
        return BasicStatus::AddressedAsReceiver;
    case TW_SR_DATA_ACK:
    case TW_SR_GCALL_DATA_ACK:
        return BasicStatus::DataReceived;
    case TW_SR_DATA_NACK:
    case TW_SR_GCALL_DATA_NACK:
        return BasicStatus::LastDataReceived;
    // Transmitter
    case TW_ST_SLA_ACK:
    case TW_ST_ARB_LOST_SLA_ACK:
        return BasicStatus::AddressedAsTransmitter;
    case TW_ST_DATA_ACK:
        return BasicStatus::SentDataAccepted;
    case TW_ST_DATA_NACK:
        return BasicStatus::SentDataDeclined;
    case TW_ST_LAST_DATA:
        return BasicStatus::MoreDataRequest;
    //
    default:
        return BasicStatus::Unknown;
    }
}

Slave::Status Slave::getStatus()
{
    switch (TW_STATUS)
    {
    // General
    case TW_NO_INFO:
        return Status::NoStatus;
    case TW_BUS_ERROR:
        return Status::Error;
    case TW_SR_STOP:
        return Status::SignalReceived;
    // Receiver
    case TW_SR_SLA_ACK:
        return Status::DirectlyAddressedAsReceiver;
    case TW_SR_ARB_LOST_SLA_ACK:
        return Status::BusLostDirectlyAddressedAsReceiver;
    case TW_SR_GCALL_ACK:
        return Status::GeneralCallAddressedAsReceiver;
    case TW_SR_ARB_LOST_GCALL_ACK:
        return Status::BusLostGeneralCallAdressedAsReceiver;
    case TW_SR_DATA_ACK:
        return Status::DirectDataReceived;
    case TW_SR_GCALL_DATA_ACK:
        return Status::GeneralCallDataReceived;
    case TW_SR_DATA_NACK:
        return Status::DirectLastDataReceived;
    case TW_SR_GCALL_DATA_NACK:
        return Status::GeneralCallLastDataReceived;
    // Transmitter
    case TW_ST_SLA_ACK:
        return Status::DirectlyAddressedAsTransmitter;
    case TW_ST_ARB_LOST_SLA_ACK:
        return Status::BusLostDirectlyAddressedAsTransmitter;
    case TW_ST_DATA_ACK:
        return Status::SentDataAccepted;
    case TW_ST_DATA_NACK:
        return Status::SentDataDeclined;
    case TW_ST_LAST_DATA:
        return Status::MoreDataRequest;
    //
    default:
        return Status::Unknown;
    }
}

void Slave::receiveNextData()
{
    TWCR = TWCR_W(_BV(TWINT) | _BV(TWEA));
}

void Slave::receiveLastData()
{
    TWCR &= ~(_BV(TWEA));
    TWCR = TWCR_W(_BV(TWINT));
}

uint8_t Slave::getData()
{
    return TWDR;
}

void Slave::sendData(uint8_t data)
{
    TWDR = data;
    TWCR = TWCR_W(_BV(TWINT) | _BV(TWEA));
}

void Slave::sendLastData(uint8_t data)
{
    TWDR = data;
    TWCR &= ~(_BV(TWEA));
    TWCR = TWCR_W(_BV(TWINT));
}
