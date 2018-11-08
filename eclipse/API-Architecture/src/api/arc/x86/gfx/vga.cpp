/*
 * vga.cpp
 *
 *  Created on: Oct 18, 2018
 *      Author: Zekrom_64
 */

#include "ltos/api/arch/x86/gfx/vga.h"
#include "ltos/api/arch/x86/asm.h"

using namespace ltos::x86;

void VGA::DAC::upload(uint8_t start, uint8_t count, PaletteColor* colors) {
	X86::outb(VGA::VGA_PORT_DAC_ADDRESS_WRITE_MODE, start);
	uint8_t* colorData = (uint8_t*)colors;
	count *= 3;
	while(count-- > 0) X86::outb(VGA::VGA_PORT_DAC_DATA, *colorData++);
}

void VGA::DAC::download(uint8_t start, uint8_t count, PaletteColor* colors) {
	X86::outb(VGA::VGA_PORT_DAC_ADDRESS_READ_MODE, start);
	uint8_t* colorData = (uint8_t*)colors;
	count *= 3;
	while(count-- > 0) *colorData++ = X86::inb(VGA::VGA_PORT_DAC_DATA);
}
