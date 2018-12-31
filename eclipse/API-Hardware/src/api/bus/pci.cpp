/*
 * pci.c
 *
 *  Created on: Oct 17, 2018
 *      Author: Zekrom_64
 */

#include "ltos/api/bus/pci.h"
#include "ltos/ltmacros.h"

const int PCI_ENUM_CHECK_BUS = 0b00000001;
const int PCI_ENUM_USE_BUS =   0b00000010;

uint32_t ltos::PCIConfigAccess::enumerateDevices(ltos::PCIDevice* devList, uint32_t devListCount) {
	uint32_t count = 0;

	PCIDevice dev;
	// Brute force scan
	for(uint16_t bus = 0; bus < 256; bus++) {
		for(uint8_t device = 0; device < 32; device++) {
			dev.init(*this, PCI_MAKE_ID(bus, device, 0));
			// Vendor ID must be valid
			if (dev.device.vendorID != 0xFFFF) {
				count++;
				if (devListCount > 0) {
					*devList++ = dev;
					devListCount--;
				}
				// Multi-function device
				if (dev.device.headerType & 0x80) {
					for(uint8_t i = 1; i < 8; i++) {
						dev.init(*this, PCI_MAKE_ID(bus, device, i));
						if (dev.device.vendorID != 0xFFFF) {
							count++;
							if (devListCount > 0) {
								*devList++ = dev;
								devListCount--;
							}
						}
					}
				}
			}
		}
	}

	return count;
}

void ltos::PCIDevice::init(ltos::PCIConfigAccess access, ltos::PCI_ID id)  {
	device.access = access;
	device.id = id;
	device.initHeader();
	switch(device.headerType & 0x3) {
	case 0x00: { // Normal device
		for(unsigned int i = 0; i < 6; i++)
			device.baseAddress[i] = device.access.read(id, 4 + i);
		device.cardbusCIS = device.access.read(id, 10);

		uint32_t word = device.access.read(id, 11);
		device.subsystemID = (uint16_t)(word >> 16);
		device.subsystemVendorID = (uint16_t)word;

		device.expansionROMAddr = device.access.read(id, 12);
		device.capabilitiesOffset = (uint8_t)device.access.read(id, 13);

		word = device.access.read(id, 15);
		device.maxLatency = (uint8_t)(word >> 24);
		device.minGrant = (uint8_t)(word >> 16);
		device.interruptPin = (uint8_t)(word >> 8);
		device.interruptLine = (uint8_t)word;
	} break;
	case 0x01: { // PCI-PCI bridge
		bridge.baseAddress[0] = device.access.read(id, 4);
		bridge.baseAddress[1] = device.access.read(id, 5);

		uint32_t word = device.access.read(id, 6);
		bridge.secondaryLatencyTimer = (uint8_t)(word >> 24);
		bridge.subordinateBusNum = (uint8_t)(word >> 16);
		bridge.secondaryBusNum = (uint8_t)(word >> 8);
		bridge.primaryBusNum = (uint8_t)word;

		word = device.access.read(id, 7);
		bridge.ioLimit = (word >> 8) & 0xFF;
		bridge.ioBase = word & 0xFF;

		word = device.access.read(id, 8);
		bridge.memoryLimit = (uint16_t)(word >> 16);
		bridge.memoryBase = (uint16_t)word;

		word = device.access.read(id, 9);
		bridge.prefetchLimit = (word >> 16) & 0xFFFF;
		bridge.prefetchBase = word & 0xFFFF;

		bridge.prefetchBase |= device.access.read(id, 10) << 16;
		bridge.prefetchLimit |= device.access.read(id, 11) << 16;

		word = device.access.read(id, 12);
		bridge.ioLimit |= (word >> 16) << 8;
		bridge.ioBase |= (word & 0xFFFF) << 8;

		bridge.capabilitiesOffset = device.access.read(id, 13) & 0xFF;

		bridge.expansionROMAddr = device.access.read(id, 14);

		word = device.access.read(id, 15);
		bridge.interruptPin = (uint8_t)(word >> 8);
		bridge.interruptLine = (uint8_t)word;
	} break;
	case 0x02: { // PCI-Cardbus bridge
		cardbus.cardbusBaseAddr = device.access.read(id, 4);

		uint32_t word = device.access.read(id, 5);
		cardbus.capabilitiesOffset = (uint8_t)word;

		word = device.access.read(id, 6);
		cardbus.cardbusLatencyTimer = (uint8_t)(word >> 24);
		cardbus.subordinateBusNum = (uint8_t)(word >> 16);
		cardbus.cardbusBusNum = (uint8_t)(word >> 8);
		cardbus.pciBusNum = (uint8_t)word;

		cardbus.memoryBase[0] = device.access.read(id, 7);
		cardbus.memoryLimit[0] = device.access.read(id, 8);
		cardbus.memoryBase[1] = device.access.read(id, 9);
		cardbus.memoryLimit[1] = device.access.read(id, 10);

		cardbus.ioBase[0] = device.access.read(id, 11);
		cardbus.ioLimit[0] = device.access.read(id, 12);
		cardbus.ioBase[1] = device.access.read(id, 13);
		cardbus.ioLimit[1] = device.access.read(id, 14);

		word = device.access.read(id, 15);
		cardbus.interruptPin = (uint8_t)(word >> 8);
		cardbus.interruptLine = (uint8_t)word;

		word = device.access.read(id, 16);
		cardbus.subsystemVendorID = (uint16_t)(word >> 16);
		cardbus.subsystemID = (uint16_t)word;

		cardbus.pcCardLegacyAddress = device.access.read(id, 17);
	} break;
	}
}
