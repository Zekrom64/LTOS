/*
 * acpi.h
 *
 *  Created on: Oct 18, 2018
 *      Author: Zekrom_64
 */

#ifndef LTOS_API_ARCH_X86_ACPI_H_
#define LTOS_API_ARCH_X86_ACPI_H_


#include "ltos/lttypes.h"
#include "ltos/ltmacros.h"

#include "bios.h"

namespace ltos::x86 {

LT_PACK_PREFIX
struct RSDPDescriptor {
	char signature[8];
	uint8_t checksum;
	char oemID[6];
	uint8_t revision;
	uint32_t rsdtAddress;

	static RSDPDescriptor* locate(EBDA* ebda);
} LT_PACK_ATTRIB;
LT_PACK_SUFFIX

LT_ASSERT_OFFSETOF(RSDPDescriptor, signature,   0x00)
LT_ASSERT_OFFSETOF(RSDPDescriptor, checksum,    0x08)
LT_ASSERT_OFFSETOF(RSDPDescriptor, oemID,       0x09)
LT_ASSERT_OFFSETOF(RSDPDescriptor, revision,    0x0F)
LT_ASSERT_OFFSETOF(RSDPDescriptor, rsdtAddress, 0x10)

LT_PACK_PREFIX
struct RSDPDescriptor2 {
	RSDPDescriptor v1_0;
	uint32_t length;
	uint64_t xsdtAddress;
	uint8_t extendedChecksum;
} LT_PACK_ATTRIB;
LT_PACK_SUFFIX

LT_ASSERT_OFFSETOF(RSDPDescriptor2, length,           0x14)
LT_ASSERT_OFFSETOF(RSDPDescriptor2, xsdtAddress,      0x18)
LT_ASSERT_OFFSETOF(RSDPDescriptor2, extendedChecksum, 0x20)

}


#endif /* LTOS_API_ARCH_X86_ACPI_H_ */
