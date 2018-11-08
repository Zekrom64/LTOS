/*
 * serial.c
 *
 *  Created on: Oct 18, 2018
 *      Author: Zekrom_64
 */

#include "ltos/api/arch/x86/io/serial.h"
#include "ltos/api/arch/x86/asm.h"

void ltos::x86::StandardSerialPort::write(uint8_t data) {
	X86::outb(ioBase, data);
}

uint8_t ltos::x86::StandardSerialPort::read() {
	return X86::inb(ioBase);
}

void ltos::x86::StandardSerialPort::setNumBits(uint8_t nbits) {
	if (nbits < 5 || nbits > 8) return;
	nbits -= 5;
	// Set number of bits in Line Control Register
	uint8_t lcr = X86::inb(ioBase + 3);
	lcr = (lcr & 0xFC) | nbits;
	X86::outb(ioBase + 3, lcr);
}

void ltos::x86::StandardSerialPort::setBaudRate(uint32_t baud) {
	uint16_t divisor = 115200 / baud;
	// Enable DLAB in Line Control Register
	uint8_t lcr = X86::inb(ioBase + 3);
	X86::outb(ioBase + 3, 0x80);
	// Write divisor
	X86::outb(ioBase, (uint8_t)(divisor & 0xFF));
	X86::outb(ioBase + 1, (uint8_t)(divisor >> 8));
	// Disable DLAB
	lcr &= ~0x80;
	X86::outb(ioBase + 3, lcr);
}

void ltos::x86::StandardSerialPort::setParity(Parity p) {
	uint8_t lcr = X86::inb(ioBase + 3);

	if (p == PARITY_NONE) lcr &= 0b111000;
	else {
		lcr |= 0b1000;
		switch(p) {
		case PARITY_ODD:  lcr &= ~0b110000;break;
		case PARITY_EVEN:  lcr |= 0b010000; break;
		case PARITY_MARK:  lcr |= 0b100000; break;
		case PARITY_SPACE: lcr |= 0b110000; break;
		default: break;
		}
	}

	X86::outb(ioBase + 3, lcr);
}

void ltos::x86::StandardSerialPort::setStopBits(StopBits stop) {
	uint8_t lcr = X86::inb(ioBase + 3);
	lcr = (lcr & 0b11111011) | (stop << 2);
	X86::outb(ioBase + 3, lcr);
}

bool ltos::x86::StandardSerialPort::readyWrite() {
	return X86::inb(ioBase + 5) & 0x20;
}

bool ltos::x86::StandardSerialPort::readyRead() {
	return X86::inb(ioBase + 5) & 1;
}

bool ltos::x86::StandardSerialPort::detect(Base b) {
	X86::outb(b + 7, 0xFF);
	if (X86::inb(b + 7) != 0xFF) return false;
	X86::outb(b + 7, 0x00);
	if (X86::inb(b + 7) != 0x00) return false;
	return true;
}
