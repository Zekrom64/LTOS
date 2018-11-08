/*
 * acpi.c
 *
 *  Created on: Oct 18, 2018
 *      Author: Zekrom_64
 */

#include "ltos/api/arch/x86/system/acpi.h"
#include "ltos/api/arch/x86/system/bios.h"

ltos::x86::RSDPDescriptor* ltos::x86::RSDPDescriptor::locate(ltos::x86::EBDA* ebda) {
	uint8_t* base = (uint8_t*)ebda;
	uint8_t* head = base + (ebda->size * 1024) - 8;
	while(base < head) {
		if (LT_INLINE_STRNCMP((const char*)base, "RSD PTR ", 8)) return (ltos::x86::RSDPDescriptor*)base;
		base++;
	}
	return nullptr;
}
