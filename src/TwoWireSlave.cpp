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
        return BasicStatus::AcceptedDataReceived;
    case TW_SR_DATA_NACK:
    case TW_SR_GCALL_DATA_NACK:
        return BasicStatus::DeclinedDataReceived;
    // Transmitter
    case TW_ST_SLA_ACK:
    case TW_ST_ARB_LOST_SLA_ACK:
        return BasicStatus::AddressedAsTransmitter;
    case TW_ST_DATA_ACK:
        return BasicStatus::NextDataAccepted;
    case TW_ST_DATA_NACK:
        return BasicStatus::NextDataDeclined;
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
        return Status::DirectAcceptedDataReceived;
    case TW_SR_GCALL_DATA_ACK:
        return Status::GeneralCallAcceptedDataReceived;
    case TW_SR_DATA_NACK:
        return Status::DirectDeclinedDataReceived;
    case TW_SR_GCALL_DATA_NACK:
        return Status::GeneralCallDeclinedDataReceived;
    // Transmitter
    case TW_ST_SLA_ACK:
        return Status::DirectlyAddressedAsTransmitter;
    case TW_ST_ARB_LOST_SLA_ACK:
        return Status::BusLostDirectlyAddressedAsTransmitter;
    case TW_ST_DATA_ACK:
        return Status::NextDataAccepted;
    case TW_ST_DATA_NACK:
        return Status::NextDataDeclined;
    case TW_ST_LAST_DATA:
        return Status::MoreDataRequest;
    //
    default:
        return Status::Unknown;
    }
}

void Slave::acceptNextData()
{
    TWCR |= _BV(TWINT) | _BV(TWEA);
}

void Slave::declineNextData()
{
    TWCR &= ~(_BV(TWEA));
    TWCR |= _BV(TWINT);
}

uint8_t Slave::getData()
{
    return TWDR;
}

void Slave::sendData(uint8_t data)
{
    TWDR = data;
    TWCR |= _BV(TWINT) | _BV(TWEA);
}

void Slave::sendLastData(uint8_t data)
{
    TWDR = data;
    TWCR &= ~(_BV(TWEA));
    TWCR |= _BV(TWINT);
}
