/*
 * pci.h
 *
 *  Created on: Oct 17, 2018
 *      Author: Zekrom_64
 */

#ifndef LTOS_API_BUS_PCI_H_
#define LTOS_API_BUS_PCI_H_


#include "ltos/lttypes.h"

namespace ltos {

/** A PCI ID identifies a specific PCI device by its bus, device, and function
 * number.
 *
 */
typedef uint32_t PCI_ID;

#define PCI_ID_BUS(ID) ((ID>>16)&0xFF)
#define PCI_ID_DEVICE(ID) ((ID>>11)&0x1F)
#define PCI_ID_FUNCTION(ID) ((ID>>8)&0x7)

#define PCI_MAKE_ID(BUS,DEV,FUNC) ((BUS<<16)|(DEV<<11)|(FUNC<<8))
#define PCI_SET_FUNCTION(ID,FUNC) ((ID))

union PCIDevice;

/** A PCI configuration access mechanism provides functions to
 * read or write words in the PCI configuration space.
 */
struct PCIConfigAccess {
	/**
	 *
	 * @param id
	 * @param offset
	 * @return
	 */
	uint32_t (*read)(PCI_ID id, uint32_t offset);

	/**
	 *
	 * @param id
	 * @param offset
	 * @param data
	 */
	void (*write)(PCI_ID id, uint32_t offset, uint32_t data);

	/** Enumerates the devices on the PCI busses using this access system.
	 *
	 * @param devList Pointer to array of PCI devices to receive enumeration, may be NULL
	 * @param devListSize Size of the array of devices
	 * @return The number of devices in the enumeration
	 */
	uint32_t enumerateDevices(PCIDevice* devList, uint32_t devListSize);
};

struct PCIDeviceHeader {
	PCIConfigAccess access;
	PCI_ID id;

	uint16_t deviceID;
	uint16_t vendorID;

	// Status & Command are special
	/** Reads the status of this PCI device. */
	inline uint16_t status() {
		return (uint16_t)(access.read(id, 1) >> 16);
	}

	/** Writes a command to this PCI device. */
	inline void command(uint16_t cmd) {
		uint32_t word = access.read(id, 1);
		word = (word & 0xFFFF0000) | cmd;
		access.write(id, 1, word);
	}

	uint8_t classCode;
	uint8_t subclass;
	uint8_t programIface;
	uint8_t revisionID;

	// BIST is special
	/** Reads the self-test status of this PCI device. */
	inline uint8_t getSelfTest() {
		return (uint8_t)(access.read(id, 3) >> 24);
	}

	/** Writes the self-test status of this PCI device. */
	inline void setSelfTest(uint8_t data) {
		uint32_t word = access.read(id, 3);
		word = (word & 0xFFFFFF) | (data << 24);
		access.write(id, 3, word);
	}

	uint8_t headerType;
	uint8_t latencyTimer;
	uint8_t cacheLineSize;

	void initHeader() {
		uint32_t word;

		word = access.read(id, 0);
		deviceID = (uint16_t)(word >> 16);
		vendorID = (uint16_t)word;

		word = access.read(id, 2);
		classCode = (uint8_t)(word >> 24);
		subclass = (uint8_t)(word >> 16);
		programIface = (uint8_t)(word >> 8);
		revisionID = (uint8_t)word;

		word = access.read(id, 3);
		headerType = (uint8_t)(word >> 16);
		latencyTimer = (uint8_t)(word >> 8);
		cacheLineSize = (uint8_t)word;
	}
};

struct PCIDeviceNormal : PCIDeviceHeader {
	uint32_t baseAddress[6];
	uint32_t cardbusCIS;
	uint16_t subsystemID;
	uint16_t subsystemVendorID;
	uint32_t expansionROMAddr;
	uint8_t capabilitiesOffset;
	uint8_t maxLatency;
	uint8_t minGrant;
	uint8_t interruptPin;
	uint8_t interruptLine;
};

struct PCIDeviceBridge : PCIDeviceHeader {
	uint32_t baseAddress[2];
	uint8_t secondaryLatencyTimer;
	uint8_t subordinateBusNum;
	uint8_t secondaryBusNum;
	uint8_t primaryBusNum;

	// Secondary status is special
	/** Reads the secondary status of the PCI bridge. */
	inline uint16_t secondaryStatus() {
		return (uint16_t)(access.read(id, 7) >> 16);
	}

	uint32_t ioLimit;
	uint32_t ioBase;
	uint16_t memoryLimit;
	uint16_t memoryBase;
	uint64_t prefetchLimit;
	uint64_t prefetchBase;
	uint8_t capabilitiesOffset;
	uint32_t expansionROMAddr;

	// Control is special
	/** Writes the bridge control register of the PCI bridge. */
	inline void control(uint16_t ctrl) {
		uint32_t word = access.read(id, 15);
		word = (word & 0xFFFF) | (ctrl << 16);
		access.write(id, 15, word);
	}

	uint8_t interruptPin;
	uint8_t interruptLine;
};

struct PCIDeviceCardbus : PCIDeviceHeader {
	uint32_t cardbusBaseAddr;

	// Secondary status is special
	/** Reads the secondary status of the PCI bridge. */
	inline uint16_t secondaryStatus() {
		return (uint16_t)(access.read(id, 5) >> 16);
	}

	uint8_t capabilitiesOffset;
	uint8_t cardbusLatencyTimer;
	uint8_t subordinateBusNum;
	uint8_t cardbusBusNum;
	uint8_t pciBusNum;
	uint32_t memoryBase[2];
	uint32_t memoryLimit[2];
	uint32_t ioBase[2];
	uint32_t ioLimit[2];

	// Control is special
	/** Writes the bridge control register of the PCI bridge. */
	inline void control(uint16_t ctrl) {
		uint32_t word = access.read(id, 15);
		word = (word & 0xFFFF) | (ctrl << 16);
		access.write(id, 15, word);
	}

	uint8_t interruptPin;
	uint8_t interruptLine;
	uint16_t subsystemID;
	uint16_t subsystemVendorID;
	uint32_t pcCardLegacyAddress;
};

union PCIDevice {
	PCIDeviceNormal device;
	PCIDeviceBridge bridge;
	PCIDeviceCardbus cardbus;
	void init(PCIConfigAccess access, PCI_ID id);
};

enum PCIDeviceClass {
	PCI_CLASS_UNKNOWN = 0,
	PCI_CLASS_MASS_STORAGE_CONTROLLER = 0x01,
	PCI_CLASS_NETWORK_CONTROLLER = 0x02,
	PCI_CLASS_DISPLAY_CONTROLLER = 0x03,
	PCI_CLASS_MULTIMEDIA_CONTROLLER = 0x04,
	PCI_CLASS_MEMORY_CONTROLLER = 0x05,
	PCI_CLASS_BRIDGE_DEVICE = 0x06,
	PCI_CLASS_SIMPLE_COMM_CONTROLLER = 0x07,
	PCI_CLASS_SYSTEM_PERIPHERAL = 0x08,
	PCI_CLASS_INPUT_DEVICE_CONTROLLER = 0x09,
	PCI_CLASS_DOCKING_STATION = 0x0A,
	PCI_CLASS_PROCESSOR = 0x0B,
	PCI_CLASS_SERIAL_BUS_CONTROLLER = 0x0C,
	PCI_CLASS_WIRELESS_CONTROLLER = 0x0D,
	PCI_CLASS_INTELLIGENT_CONTROLLER = 0x0E,
	PCI_CLASS_SATELLITE_COMM_CONTROLLER = 0x0F,
	PCI_CLASS_ENCRYPTION_CONTROLLER = 0x10,
	PCI_CLASS_SIGNAL_PROCESSOR_CONTROLLER = 0x11,
	PCI_CLASS_PROCESSING_ACCELERATOR = 0x12,
	PCI_CLASS_NON_ESSENTIAL_INSTRUMENT = 0x13,
	PCI_CLASS_COPROCESSOR = 0x40
};

enum PCIDeviceSubclass {
	PCI_SUBCLASS_00_NON_VGA_COMPATIBLE = 0x00,
	PCI_SUBCLASS_00_VGA_COMPATIBLE = 0x01,

	PCI_SUBCLASS_01_SCSI_BUS_CONTROLLER = 0x00,
	PCI_SUBCLASS_01_IDE_CONTROLLER = 0x01,
	PCI_SUBCLASS_01_FLOPPY_DISK_CONTROLLER = 0x02,
	PCI_SUBCLASS_01_IPI_BUS_CONTROLLER = 0x03,
	PCI_SUBCLASS_01_RAID_CONTROLLER = 0x04,
	PCI_SUBCLASS_01_ATA_CONTROLLER = 0x05,
	PCI_SUBCLASS_01_SERIAL_ATA = 0x06,
	PCI_SUBCLASS_01_SERIAL_ATTACHED_SCSI = 0x07,
	PCI_SUBCLASS_01_NON_VOLATILE_MEMORY_CONTROLLER = 0x08,
	PCI_SUBCLASS_01_OTHER = 0x80,

	PCI_SUBCLASS_02_ETHERNET_CONTROLLER = 0x00,
	PCI_SUBCLASS_02_TOKEN_RING_CONTROLLER = 0x01,
	PCI_SUBCLASS_02_FDDI_CONTROLLER = 0x02,
	PCI_SUBCLASS_02_ATM_CONTROLLER = 0x03,
	PCI_SUBCLASS_02_ISDN_CONTROLLER = 0x04,
	PCI_SUBCLASS_02_WORLDFLIP_CONTROLLER = 0x05,
	PCI_SUBCLASS_02_PICMG_2_14_MULTI_COMPUTING = 0x06,
	PCI_SUBCLASS_02_INFINIBAND_CONTROLLER = 0x07,
	PCI_SUBCLASS_02_FABRIC_CONTROLLER = 0x08,
	PCI_SUBCLASS_02_OTHER = 0x80,

	PCI_SUBCLASS_03_VGA_COMPATIBLE = 0x00,
	PCI_SUBCLASS_03_XGA_CONTROLLER = 0x01,
	PCI_SUBCLASS_03_3D_CONTROLLER = 0x02,
	PCI_SUBCLASS_03_OTHER = 0x80,

	PCI_SUBCLASS_04_MULTIMEDIA_VIDEO_CONTROLLER = 0x00,
	PCI_SUBCLASS_04_MULTIMEDIA_AUDIO_CONTROLLER = 0x01,
	PCI_SUBCLASS_04_COMPUTER_TELEPHONY_DEVICE = 0x02,
	PCI_SUBCLASS_04_AUDIO_DEVICE = 0x03,
	PCI_SUBCLASS_04_OTHER = 0x80,

	PCI_SUBCLASS_05_RAM_CONTROLLER = 0x00,
	PCI_SUBCLASS_05_FLASH_CONTROLLER = 0x01,
	PCI_SUBCLASS_05_OTHER = 0x80,

	PCI_SUBCLASS_06_HOST_BRIDGE = 0x00,
	PCI_SUBCLASS_06_ISA_BRIDGE = 0x01,
	PCI_SUBCLASS_06_EISA_BRIDGE = 0x02,
	PCI_SUBCLASS_06_MCA_BRIDGE = 0x03,
	PCI_SUBCLASS_06_PCI_TO_PCI_BRIDGE = 0x04,
	PCI_SUBCLASS_06_PCMCIA_BRIDGE = 0x05,
	PCI_SUBCLASS_06_NUBUS_BRIDGE = 0x06,
	PCI_SUBCLASS_06_CARDBUS_BRIDGE = 0x07,
	PCI_SUBCLASS_06_RACEWAY_BRIDGE = 0x08,
	PCI_SUBCLASS_06_PCI_TO_PCI_BRIDGE_2 = 0x05,
	PCI_SUBCLASS_06_INFINIBAND_BRIDGE = 0x0A,
	PCI_SUBCLASS_06_OTHER = 0x80,

	PCI_SUBCLASS_07_SERIAL_CONTROLLER = 0x00,
	PCI_SUBCLASS_07_PARALLEL_CONTROLLER = 0x01,
	PCI_SUBCLASS_07_MULTIPORT_SERIAL_CONTROLLER = 0x02,
	PCI_SUBCLASS_07_MODEM = 0x03,
	PCI_SUBCLASS_07_IEEE_488_GPIB_CONTROLLER = 0x04,
	PCI_SUBCLASS_07_SMART_CARD = 0x05,
	PCI_SUBCLASS_07_OTHER = 0x80,

	PCI_SUBCLASS_08_PIC = 0x00,
	PCI_SUBCLASS_08_DMA_CONTROLLER = 0x01,
	PCI_SUBCLASS_08_TIMER = 0x02,
	PCI_SUBCLASS_08_RTC_CONTROLLER = 0x03,
	PCI_SUBCLASS_08_PCI_HOT_PLUG_CONTROLLER = 0x04,
	PCI_SUBCLASS_08_SD_HOST_CONTROLLER = 0x05,
	PCI_SUBCLASS_08_IOMMU = 0x06,
	PCI_SUBCLASS_08_OTHER = 0x80,

	PCI_SUBCLASS_09_KEYBOARD_CONTROLLER = 0x00,
	PCI_SUBCLASS_09_DIGITIZER_PEN = 0x01,
	PCI_SUBCLASS_09_MOUSE_CONTROLLER = 0x02,
	PCI_SUBCLASS_09_SCANNER_CONTROLLER = 0x03,
	PCI_SUBCLASS_09_GAMEPORT_CONTROLLER = 0x04,
	PCI_SUBCLASS_09_OTHER = 0x80,

	PCI_SUBCLASS_0A_GENERIC = 0x00,
	PCI_SUBCLASS_0A_OTHER = 0x80,

	PCI_SUBCLASS_0B_386 = 0x00,
	PCI_SUBCLASS_0B_486 = 0x01,
	PCI_SUBCLASS_0B_PENTIUM = 0x02,
	PCI_SUBCLASS_0B_ALPHA = 0x10,
	PCI_SUBCLASS_0B_POWERPC = 0x20,
	PCI_SUBCLASS_0B_MIPS = 0x30,
	PCI_SUBCLASS_0B_COPROCESSOR = 0x40,

	PCI_SUBCLASS_0C_FIREWIRE_CONTROLLER = 0x00,
	PCI_SUBCLASS_0C_ACCESS_BUS = 0x01,
	PCI_SUBCLASS_0C_SSA = 0x02,
	PCI_SUBCLASS_0C_USB_CONTROLLER = 0x03,
	PCI_SUBCLASS_0C_FIBRE_CHANNEL = 0x04,
	PCI_SUBCLASS_0C_SMBUS = 0x05,
	PCI_SUBCLASS_0C_INFINIBAND = 0x06,
	PCI_SUBCLASS_0C_IPMI_INTERFACE = 0x07,
	PCI_SUBCLASS_0C_SERCOS_INTERFACE = 0x08,
	PCI_SUBCLASS_0C_CANBUS = 0x09,

	PCI_SUBCLASS_0D_IRDA_COMPATIBLE_CONTROLLER = 0x00,
	PCI_SUBCLASS_0D_CONSUMER_IR_CONTROLLER = 0x01,
	PCI_SUBCLASS_0D_RF_CONTROLLER = 0x10,
	PCI_SUBCLASS_0D_BLUETOOTH_CONTROLLER = 0x11,
	PCI_SUBCLASS_0D_BROADBAND_CONTROLLER = 0x12,
	PCI_SUBCLASS_0D_ETHERNET_802_1A = 0x20,
	PCI_SUBCLASS_0D_ETHERNET_802_1B = 0x21,
	PCI_SUBCLASS_0D_OTHER = 0x80,

	PCI_SUBCLASS_0E_INTELLIGENT_CONTROLLER = 0x00,

	PCI_SUBCLASS_0F_SATELLITE_TV_CONTROLLER = 0x01,
	PCI_SUBCLASS_0F_SATELLITE_AUDIO_CONTROLLER = 0x02,
	PCI_SUBCLASS_0F_SATELLITE_VOICE_CONTROLLER = 0x03,
	PCI_SUBCLASS_0F_SATELLITE_DATA_CONTROLLER = 0x04,

	PCI_SUBCLASS_10_NETWORK_AND_COMPUTING_CRYPTO = 0x00,
	PCI_SUBCLASS_10_ENTERTAINMENT_CRYPTO = 0x10,
	PCI_SUBCLASS_10_OTHER_CRYPTO = 0x80,

	PCI_SUBCLASS_11_DPIO_MODULES = 0x00,
	PCI_SUBCLASS_11_PERFORMANCE_COUNTERS = 0x01,
	PCI_SUBCLASS_11_COMMUNICATION_SYNCHRONIZER = 0x10,
	PCI_SUBCLASS_11_SIGNAL_PROCESSING_MANAGEMENT = 0x20,
	PCI_SUBCLASS_11_OTHER = 0x80
};

enum PCIDeviceProgramIFace {
	PCI_IFACE_01_01_BITMASK__PCI_NATIVE = 0x05,
	PCI_IFACE_01_01_BITMASK__COMPATIBLE = 0x0A,
	PCI_IFACE_01_01_BITMASK__BUS_MASTERING = 0x80,

	PCI_IFACE_01_05_SINGLE_DMA = 0x20,
	PCI_IFACE_01_05_CHAINED_DMA = 0x30,

	PCI_IFACE_01_06_VENDOR_SPECIFIC = 0x00,
	PCI_IFACE_01_06_AHCI_1_0_ = 0x01,
	PCI_IFACE_01_06_SERIAL_STORAGE_BUS = 0x02,

	PCI_IFACE_01_07_SAS = 0x00,
	PCI_IFACE_01_07_SERIAL_STORAGE_BUS = 0x01,

	PCI_IFACE_01_08_NVMHCI = 0x01,
	PCI_IFACE_01_08_NVM_EXPRESS = 0x02,

	PCI_IFACE_03_00_VGA_CONTROLLER = 0x00,
	PCI_IFACE_03_00_8514_COMPATIBLE = 0x01,

	PCI_IFACE_06_04_NORMAL_DECODE = 0x00,
	PCI_IFACE_06_04_SUBTRACTIVE_DECODE = 0x01,

	PCI_IFACE_06_08_TRANSPARENT_MODE = 0x00,
	PCI_IFACE_06_08_ENDPOINT_MODE = 0x01,

	PCI_IFACE_06_09_PRIMARY_TOWARDS_CPU = 0x40,
	PCI_IFACE_06_09_SECONDARY_TOWARDS_CPU = 0x80,

	PCI_IFACE_07_00_8250_COMPATIBLE = 0x00,
	PCI_IFACE_07_00_16450_COMPATIBLE = 0x01,
	PCI_IFACE_07_00_16550_COMPATIBLE = 0x02,
	PCI_IFACE_07_00_16650_COMPATIBLE = 0x03,
	PCI_IFACE_07_00_16750_COMPATIBLE = 0x04,
	PCI_IFACE_07_00_16850_COMPATIBLE = 0x05,
	PCI_IFACE_07_00_16950_COMPATIBLE = 0x06,

	PCI_IFACE_07_01_STANDARD_PARALLEL_PORT = 0x00,
	PCI_IFACE_07_01_BIDIRECTIONAL_PARALLEL_PORT = 0x01,
	PCI_IFACE_07_01_ECP_1_X_PARALLEL_PORT = 0x02,
	PCI_IFACE_07_01_IEEE_1284_CONTROLLER = 0x03,
	PCI_IFACE_07_01_IEEE_1284_TARGET_DEVICE = 0x04,

	PCI_IFACE_07_03_GENERIC_MODEM = 0x00,
	PCI_IFACE_07_03_HAYES_16450_COMPATIBLE = 0x01,
	PCI_IFACE_07_03_HAYES_16550_COMPATIBLE = 0x02,
	PCI_IFACE_07_03_HAYES_16650_COMPATIBLE = 0x03,
	PCI_IFACE_07_03_HAYES_16750_COMPATIBLE = 0x04,

	PCI_IFACE_08_00_GENERIC_8259_COMPATIBLE = 0x00,
	PCI_IFACE_08_00_ISA_COMPATIBLE = 0x01,
	PCI_IFACE_08_00_EISA_COMPATIBLE = 0x02,
	PCI_IFACE_08_00_IO_APIC_CONTROLLER = 0x10,
	PCI_IFACE_08_00_IOX_APIC_CONTROLLER = 0x20,

	PCI_IFACE_08_01_GENERIC_8237_COMPATIBLE = 0x00,
	PCI_IFACE_08_01_ISA_COMPATIBLE = 0x01,
	PCI_IFACE_08_01_EISA_COMPATIBLE = 0x02,

	PCI_IFACE_08_02_GENERIC_8254_COMPATIBLE = 0x00,
	PCI_IFACE_08_02_ISA_COMPATIBLE = 0x01,
	PCI_IFACE_08_02_EISA_COMPATIBLE = 0x02,
	PCI_IFACE_08_02_HPET = 0x03,

	PCI_IFACE_08_03_GENERIC_RTC = 0x00,
	PCI_IFACE_08_03_ISA_COMPATIBLE = 0x01,

	PCI_IFACE_09_04_GENERIC = 0x00,
	PCI_IFACE_09_04_EXTENDED = 0x10,

	PCI_IFACE_0C_00_GENERIC = 0x00,
	PCI_IFACE_0C_00_OHCI = 0x10,

	PCI_IFACE_0C_03_UHCI = 0x00,
	PCI_IFACE_0C_03_OHCI = 0x10,
	PCI_IFACE_0C_03_EHCI = 0x20,
	PCI_IFACE_0C_03_XHCI = 0x30,
	PCI_IFACE_0C_03_UNSPECIFIED = 0x80,
	PCI_IFACE_0C_03_USB_DEVICE = 0xFE,

	PCI_IFACE_0C_07_SMIC = 0x00,
	PCI_IFACE_0C_07_KEYBOARD_CONTROLLER_STYLE = 0x01,
	PCI_IFACE_0C_07_BLOCK_TRANSFER = 0x02
};

};


#endif /* LTOS_API_BUS_PCI_H_ */
