#include "TwoWire.hpp"

#include <compat/twi.h>

void TwoWire::init(uint8_t address, uint32_t frequency)
{
    setAddress(address);

    setBaseFrequency(frequency);
    setFrequencyPrescaler(BitRatePrescaler::x1);

    enable();
}

void TwoWire::enable()
{
    TWCR = _BV(TWEA) | _BV(TWEN);
}

void TwoWire::disable()
{
    TWCR &= ~(_BV(TWEA) | _BV(TWEN) | _BV(TWIE));
}

void TwoWire::setAddress(uint8_t address)
{
    TWAR = (address << 1) | (TWAR & _BV(TWGCE));
}

void TwoWire::setAddress(uint8_t address, bool allowGeneralCallAddress)
{
    TWAR = (address << 1) | (allowGeneralCallAddress & _BV(TWGCE));
}

uint8_t TwoWire::getAddress()
{
    return TWAR >> 1;
}

void TwoWire::setAddressMask(uint8_t mask)
{
    TWAMR = mask << 1;
}

void TwoWire::setBaseFrequency(uint32_t frequency)
{
    TWBR = ((F_CPU / frequency) - 16) / 2;
}

void TwoWire::setFrequencyPrescaler(BitRatePrescaler prescaler)
{
    TWSR |= ((uint8_t)prescaler) & (_BV(TWPS1) | _BV(TWPS0));
}

uint32_t TwoWire::getBaseFrequency()
{
    return F_CPU / (16 + 2 * TWBR);
}

uint32_t TwoWire::getFrequency()
{
    return F_CPU / (16 + 2 * TWBR * (TWSR & (_BV(TWPS1) | _BV(TWPS0))));
}

void TwoWire::allowSlaveCommunication()
{
    TWCR |= _BV(TWEA);
}

void TwoWire::disallowSlaveCommunication()
{
    TWCR &= ~(_BV(TWEA));
}

void TwoWire::allowGeneralCall()
{
    TWAR |= _BV(TWGCE);
}

void TwoWire::disallowGeneralCall()
{
    TWAR &= ~(_BV(TWGCE));
}

void TwoWire::enableInterrupt()
{
    TWCR |= _BV(TWIE);
}

void TwoWire::disableInterrupt()
{
    TWCR &= ~(_BV(TWIE));
}

bool TwoWire::isErrored()
{
    return TW_STATUS == TW_BUS_ERROR;
}

void TwoWire::clearError()
{
    TWCR |= _BV(TWINT) | _BV(TWSTO);
}

void TwoWire::clearErrorIfSet()
{
    if (isErrored()) clearError();
}
