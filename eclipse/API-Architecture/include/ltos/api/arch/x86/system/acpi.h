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
#include "ltos/ltarch.h"

#ifdef LT_ARCH_FAMILY_X86

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

LT_PACK_PREFIX
struct SDTHeader {
	char signature[4];
	uint32_t length;
	uint8_t revision;
	uint8_t checksum;
	char oemID[6];
	char oemTableID[8];
	uint32_t oemRevision;
	uint32_t creatorID;
	uint32_t creatorRevision;
} LT_PACK_ATTRIB;
LT_PACK_SUFFIX

LT_PACK_PREFIX
struct ACPIAddress {
	enum AddressSpace {
		ADDRESS_SYSTEM_MEMORY = 0,
		ADDRESS_SYSTEM_IO = 1,
		ADDRESS_PCI_CONFIG = 2,
		ADDRESS_EMBEDDED_CONTROLLER = 3,
		ADDRESS_SMBUS = 4
	};
	uint8_t addressSpace;
	uint8_t bitWidth;
	uint8_t bitOffset;
	uint8_t accessSize;
	uint64_t address;
} LT_PACK_ATTRIB;
LT_PACK_SUFFIX

LT_PACK_PREFIX
struct RSDT {
	SDTHeader header;
	uint32_t tables[];
	void* findTable(const char* name) {
		size_t count = (header.length - sizeof(SDTHeader)) >> 3;
		for(size_t i = 0; i < count; i++) {
#ifdef LT_ARCH_X86_32
			void* tablePtr = (void*)tables[i];
#elif defined(LT_ARCH_X86_64)
			void* tablePtr = (void*)((uint64_t)tables[i]);
#endif
			if (*((uint32_t*)tablePtr) == *((uint32_t*)name)) return tablePtr;
		}
		return nullptr;
	}
} LT_PACK_ATTRIB;
LT_PACK_SUFFIX

LT_PACK_PREFIX
struct XSDT {
	SDTHeader header;
	uint64_t tables[];
	void* findTable(const char* name) {
		size_t count = (header.length - sizeof(SDTHeader)) >> 3;
		for(size_t i = 0; i < count; i++) {
#ifdef LT_ARCH_X86_32
			void* tablePtr = (void*)((uint32_t)tables[i]);
#elif defined(LT_ARCH_X86_64)
			void* tablePtr = (void*)tables[i];
#endif
			if (*((uint32_t*)tablePtr) == *((uint32_t*)name)) return tablePtr;
		}
		return nullptr;
	}
} LT_PACK_ATTRIB;
LT_PACK_SUFFIX

LT_PACK_PREFIX
struct FADT {
	SDTHeader header;
	uint32_t firmwareCtrl;
	uint32_t dsdt;
	uint8_t reserved0;
	uint8_t preferredPMProfile;
	enum PMProfile {
		POWER_PROFILE_UNKNOWN = 0,
		POWER_PROFILE_DESKTOP = 1,
		POWER_PROFILE_MOBLE = 2,
		POWER_PROFILE_WORKSTATION = 3,
		POWER_PROFILE_SERVER_ENTERPRISE = 4,
		POWER_PROFILE_SERVER_SOHO = 5,
		POWER_PROFILE_APPLIANCE = 6,
		POWER_PROFILE_SERVER_PERFORMANCE = 7
	};
	uint16_t sciInterrupt;
	uint32_t smiCommandPort;
	uint8_t acpiEnable;
	uint8_t acpiDisable;
	uint8_t s4biosReq;
	uint8_t pstateControl;
	uint32_t pm1AEventBlock;
	uint32_t pm1BEventBlock;
	uint32_t pm1AControlBlock;
	uint32_t pm1BControlBlock;
	uint32_t pm2ControlBlock;
	uint32_t pmTimerBlock;
	uint32_t gpe0Block;
	uint32_t gpe1Block;
	uint8_t pm1EventLength;
	uint8_t pm1ControlLength;
	uint8_t pm2ControlLength;
	uint8_t pmTimerLength;
	uint8_t gpe0Length;
	uint8_t gpe1Length;
	uint8_t gpe1Base;
	uint8_t cStateControl;
	uint16_t worstC2Latency;
	uint16_t worstC3Latency;
	uint16_t flushSize;
	uint16_t flushStride;
	uint8_t dutyOffset;
	uint8_t dutyAlarm;
	uint8_t monthAlarm;
	uint8_t century;
	uint16_t bootArchFlags;
	uint8_t reserved1;
	uint32_t flags;
	ACPIAddress resetRegister;
	uint8_t resetValue;
	uint8_t reserved2[3];
	uint64_t pFirmwareControl;
	uint64_t pDsdt;
	ACPIAddress pPM1AEventBlock;
	ACPIAddress pPM1BEventBlock;
	ACPIAddress pPM1AControlBlock;
	ACPIAddress pPM1BControlBlock;
	ACPIAddress pPM2ControlBlock;
	ACPIAddress pPMTimerBlock;
	ACPIAddress pGPE0Block;
	ACPIAddress pGPE1Block;
} LT_PACK_ATTRIB;
LT_PACK_SUFFIX

}

#endif

#endif /* LTOS_API_ARCH_X86_ACPI_H_ */
