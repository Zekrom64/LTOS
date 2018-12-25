/*
 * scsi_cmds.inc
 *
 *  Created on: Nov 4, 2018
 *      Author: Zekrom_64
 */

#ifndef LTOS_API_BUS_SCSI_CMDS_INC_
#define LTOS_API_BUS_SCSI_CMDS_INC_

#include "scsi.h"
#include "ltos/ltmacros.h"

// Specification via Seagate: https://www.seagate.com/staticfiles/support/disc/manuals/Interface%20manuals/100293068c.pdf

namespace ltos::SCSI {

	typedef uint32be_t SCSIShortBlockAddr;
	typedef uint64be_t SCSILongBlockAddr;
	LT_PACK_PREFIX
	struct SCSICHSByteAddr {
		uint8_t cylinder[3];
		uint8_t head;
		uint32be_t byteOffset;
	} LT_PACK_ATTRIB;
	LT_PACK_SUFFIX
	LT_ASSERT_SIZEOF(SCSICHSByteAddr, 8)
	LT_ASSERT_OFFSETOF(SCSICHSByteAddr, cylinder, 0)
	LT_ASSERT_OFFSETOF(SCSICHSByteAddr, head, 3)
	LT_ASSERT_OFFSETOF(SCSICHSByteAddr, byteOffset, 4)
	LT_PACK_PREFIX
	struct SCSICHSSectorAddr {
		uint8_t cylinder[3];
		uint8_t head;
		uint32be_t sectorNumber;
	} LT_PACK_ATTRIB;
	LT_PACK_SUFFIX
	LT_ASSERT_SIZEOF(SCSICHSSectorAddr, 8)
	LT_ASSERT_OFFSETOF(SCSICHSSectorAddr, cylinder, 0)
	LT_ASSERT_OFFSETOF(SCSICHSSectorAddr, head, 3)
	LT_ASSERT_OFFSETOF(SCSICHSSectorAddr, sectorNumber, 4)

#define SCSI_MAKE_FIELD(B,O,S) ((O<<6)|(B<<3)|(S))
#define SCSI_FIELD_GETTER \
	inline int getField(int fieldID) { \
		uint8_t fieldByte = ((uint8_t*)this)[fieldID>>6]; \
		fieldByte >>= ((fieldID>>3)&0b111); \
		uint8_t mask = 0, bitcount = (fieldID&0x7); \
		while(bitcount-->0) { mask = (mask << 1) | 1; } \
		return mask & fieldByte; \
	}

	/* ---------------------
	 * | CHANGE DEFINITION |
	 * --------------------- */

	LT_DEPRECATE("SCSI command \"CHANGE_DEFINITION\" declared obsolete by T10 committee")
	enum SCSI__CHANGE_DEFINITION__DEFINITION_PARAM {
		SCSI_DEFINITION_PARAM__USE_CURRENT = 0x00,
		SCSI_DEFINITION_PARAM__SCSI_2 = 0x03,
		SCSI_DEFINITION_PARAM__SCSI_3 = 0x04,
		SCSI_DEFINITION_PARAM__MFG_DEFAULT = 0x3F,
	};

	LT_DEPRECATE("SCSI command \"CHANGE_DEFINITION\" declared obsolete by T10 committee")
	inline SCSICommand12 CHANGE_DEFINITION(bool save, SCSI__CHANGE_DEFINITION__DEFINITION_PARAM defParam,
			uint8_t paramLength) {
		SCSICommand12 cmd = {};
		cmd.op = 0x40;
		cmd.data_bytes[3] = save ? 1 : 0;
		cmd.data_bytes[4] = defParam & 0x7F;
		cmd.data_bytes[9] = paramLength;
		return cmd;
	}

	/* ---------------
	 * | FORMAT UNIT |
	 * --------------- */

	enum SCSI__FORMAT_UNIT__DEFECT_LIST_FORMAT {
		SCSI_DEFECT_LIST_FORMAT__NONE = 0,
		SCSI_DEFECT_LIST_FORMAT__SHORT_BLOCK = 0,
		SCSI_DEFECT_LIST_FORMAT__LONG_BLOCK = 3,
		SCSI_DEFECT_LIST_FORMAT__CHS_BYTE = 4,
		SCSI_DEFECT_LIST_FORMAT__CHS_SECTOR = 5
	};
	enum SCSI__FORMAT_UNIT__PARAMLIST_FLAGS {
		SCSI_FORMAT_UNIT_FLAG__FOV  = 0b10000000,
		SCSI_FORMAT_UNIT_FLAG__DPRY = 0b01000000,
		SCSI_FORMAT_UNIT_FLAG__DCRT = 0b00100000,
		SCSI_FORMAT_UNIT_FLAG__STPF = 0b00010000,
		SCSI_FORMAT_UNIT_FLAG__IP =   0b00001000,
		SCSI_FORMAT_UNIT_FLAG__IMMED = 0b00000010
	};
	enum SCSI__FORMAT_UNIT__IP_MODIFIER {
		SCSI_FORMAT_UNIT_IPMOD__NONE = 0,
		SCSI_FORMAT_UNIT_IPMOD__LOGICAL = 0x40,
		SCSI_FORMAT_UNIT_IPMOD__PHYSICAL = 0x80
	};
	enum SCSI__FORMAT_UNIT__IP_TYPE {
		SCSI_FORMAT_UNIT_IPTYPE__DEFAULT = 0,
		SCSI_FORMAT_UNIT_IPTYPE__PATTERN = 1
	};
	constexpr int SCSI_FORMAT_UNIT__SECURITY_INIT = 0x20;

	template<bool LONG>
	struct SCSI__FORMAT_UNIT__PARAMLIST {};

	template<>
	struct SCSI__FORMAT_UNIT__PARAMLIST<false> {
		uint8_t protFieldsUsage;
		uint8_t flags;
		uint16be_t defectListLength;
	};
	LT_ASSERT_SIZEOF(SCSI__FORMAT_UNIT__PARAMLIST<false>, 4)
	LT_ASSERT_OFFSETOF(SCSI__FORMAT_UNIT__PARAMLIST<false>, protFieldsUsage, 0)
	LT_ASSERT_OFFSETOF(SCSI__FORMAT_UNIT__PARAMLIST<false>, flags, 1)
	LT_ASSERT_OFFSETOF(SCSI__FORMAT_UNIT__PARAMLIST<false>, defectListLength, 2)
	template<>
	struct SCSI__FORMAT_UNIT__PARAMLIST<true> {
		uint8_t protFieldUsage;
		uint8_t flags;
		uint8_t reserved0;
		uint8_t protIntervalInfo;
		uint32be_t defectListLength;
	};
	LT_ASSERT_SIZEOF(SCSI__FORMAT_UNIT__PARAMLIST<true>, 8)
	LT_ASSERT_OFFSETOF(SCSI__FORMAT_UNIT__PARAMLIST<true>, protFieldUsage, 0)
	LT_ASSERT_OFFSETOF(SCSI__FORMAT_UNIT__PARAMLIST<true>, flags, 1)
	LT_ASSERT_OFFSETOF(SCSI__FORMAT_UNIT__PARAMLIST<true>, protIntervalInfo, 3)
	LT_ASSERT_OFFSETOF(SCSI__FORMAT_UNIT__PARAMLIST<true>, defectListLength, 4)
	template<int PATTERN_LENGTH>
	struct SCSI__FORMAT_UNIT__PATTERN_DESCRIPTOR {
		uint8_t flags;
		uint8_t patternType;
		uint16be_t patternLength;
		uint8_t pattern[PATTERN_LENGTH];
	};
	LT_ASSERT_SIZEOF(SCSI__FORMAT_UNIT__PATTERN_DESCRIPTOR<0>, 4)
	LT_ASSERT_OFFSETOF(SCSI__FORMAT_UNIT__PATTERN_DESCRIPTOR<0>, flags, 0)
	LT_ASSERT_OFFSETOF(SCSI__FORMAT_UNIT__PATTERN_DESCRIPTOR<0>, patternType, 1)
	LT_ASSERT_OFFSETOF(SCSI__FORMAT_UNIT__PATTERN_DESCRIPTOR<0>, patternLength, 2)
	LT_ASSERT_OFFSETOF(SCSI__FORMAT_UNIT__PATTERN_DESCRIPTOR<0>, pattern, 4)

	inline SCSICommand6 FORMAT_UNIT(uint8_t fmtpInfo, bool longList, bool fmtData, bool cmpList,
			SCSI__FORMAT_UNIT__DEFECT_LIST_FORMAT defectLstFmt) {
		SCSICommand6 cmd = {};
		cmd.op = 0x04;
		cmd.data_bytes[0] = (fmtpInfo << 6) | (longList ? 0x20 : 0) | (fmtData ? 0x10 : 0) | (cmpList ? 0x08 : 0) | defectLstFmt;
		return cmd;
	}

	/* -----------
	 * | INQUIRY |
	 * ----------- */

	enum SCSI__INQUIRY__FIELD {
		SCSI_INQUIRY__PERIPHERAL_QUALIFIER = SCSI_MAKE_FIELD(0,5,3),
		SCSI_INQUIRY__PERIPHERAL_DEVICE_TYPE = SCSI_MAKE_FIELD(0,0,5),
		SCSI_INQUIRY__REMOVEABLE_MEDIA = SCSI_MAKE_FIELD(1,7,1),
		SCSI_INQUIRY__NORMAL_ACA = SCSI_MAKE_FIELD(3,5,1),
		SCSI_INQUIRY__HIERARCH_SUPPORT = SCSI_MAKE_FIELD(3,4,1),
		SCSI_INQUIRY__RESPONSE_FORMAT = SCSI_MAKE_FIELD(3,0,4),
		SCSI_INQUIRY__SCC_SUPPORTED = SCSI_MAKE_FIELD(5,7,1),
		SCSI_INQUIRY__ACCESS_CONTROL_COORD = SCSI_MAKE_FIELD(5,6,1),
		SCSI_INQUIRY__TARGET_PORT_SUPPORT = SCSI_MAKE_FIELD(5,4,2),
		SCSI_INQUIRY__THIRD_PARTY_COPY = SCSI_MAKE_FIELD(5,3,1),
		SCSI_INQUIRY__PROTECTION_INFO = SCSI_MAKE_FIELD(5,0,1),
		SCSI_INQUIRY__BASIC_QUEUEING = SCSI_MAKE_FIELD(6,7,1),
		SCSI_INQUIRY__ENCLOSURE_SERVICES = SCSI_MAKE_FIELD(6,6,1),
		SCSI_INQUIRY__MULTI_PORT = SCSI_MAKE_FIELD(6,4,1),
		SCSI_INQUIRY__MEDIUM_CHANGER = SCSI_MAKE_FIELD(6,3,1),
		SCSI_INQUIRY__LINKED = SCSI_MAKE_FIELD(7,3,1),
		SCSI_INQUIRY__COMMAND_QUEUEING = SCSI_MAKE_FIELD(7,1,1),
		SCSI_INQUIRY__WIDE_SCSI_ADDRESS = SCSI_MAKE_FIELD(6,0,1),
		SCSI_INQUIRY__16BIT_TRANSFERS = SCSI_MAKE_FIELD(7,5,1),
		SCSI_INQUIRY__SYNCHRONOUS_TRANSFER = SCSI_MAKE_FIELD(7,4,1),
		SCSI_INQUIRY__CLOCKING = SCSI_MAKE_FIELD(56,2,2),
		SCSI_INQUIRY__QUICK_ARBIT_AND_SEL = SCSI_MAKE_FIELD(56,1,1),
		SCSI_INQUIRY__INFO_UNITS_SUPPORTED = SCSI_MAKE_FIELD(56,0,1)
	};

	enum SCSI__INQUIRY__PERIPHERAL_QUALIFIER {
		SCSI_INQUIRY_PERIPHERAL_QUALIFIER__CONNECTED = 0,
		SCSI_INQUIRY_PERIPHERAL_QUALIFIER__UNCONNECTED = 1,
		SCSI_INQUIRY_PERIPHERAL_QUALIFIER__UNDEFINED = 2
	};
	enum SCSI__INQUIRY__PERIPHERAL_DEVICE_TYPE {
		SCSI_INQUIRY_PERIPHERAL_DEV_TYPE__DIRECT_ACCESS = 0,
		SCSI_INQUIRY_PERIPHERAL_DEV_TYPE__SEQUENTIAL_ACCESS = 1,
		SCSI_INQUIRY_PERIPHERAL_DEV_TYPE__PRINTER = 2,
		SCSI_INQUIRY_PERIPHERAL_DEV_TYPE__PROCESSOR = 3,
		SCSI_INQUIRY_PERIPHERAL_DEV_TYPE__WRITE_ONCE = 4,
		SCSI_INQUIRY_PERIPHERAL_DEV_TYPE__CD_DVD = 5,
		SCSI_INQUIRY_PERIPHERAL_DEV_TYPE__SCANNER = 6,
		SCSI_INQUIRY_PERIPHERAL_DEV_TYPE__OPTICAL_MEMORY = 7,
		SCSI_INQUIRY_PERIPHERAL_DEV_TYPE__MEDIUM_CHANGER = 8,
		SCSI_INQUIRY_PERIPHERAL_DEV_TYPE__COMMUNICATIONS = 9,
		SCSI_INQUIRY_PERIPHERAL_DEV_TYPE__STORAGE_ARRAY = 0xC,
		SCSI_INQUIRY_PERIPHERAL_DEV_TYPE__ENCLOSURE_SERVICES = 0xD,
		SCSI_INQUIRY_PERIPHERAL_DEV_TYPE__SIMPLIFIED_DIRECT_ACCESS = 0xE,
		SCSI_INQUIRY_PERIPHERAL_DEV_TYPE__OPTICAL_CARD = 0xF,
		SCSI_INQUIRY_PERIPHERAL_DEV_TYPE__BRIDGE_CONTROLLER = 0x10,
		SCSI_INQUIRY_PERIPHERAL_DEV_TYPE__OBJECT_BASED_STORAGE = 0x11,
		SCSI_INQUIRY_PERIPHERAL_DEV_TYPE__AUTOMATION_IFACE = 0x12,
		SCSI_INQUIRY_PERIPHERAL_DEV_TYPE__UNDEFINED = 0x1F
	};
	enum SCSI__INQUIRY__VERSION {
		SCSI_INQUIRY_VERSION__UNSTANDARDIZED = 0,
		SCSI_INQUIRY_VERSION__SPC = 3,
		SCSI_INQUIRY_VERSION__SPC2 = 4,
		SCSI_INQUIRY_VERSION__SPC4 = 5,
		SCSI_INQUIRY_VERSION__T10_SPC4 = 6
	};
	enum SCSI__INQUIRY__TARGET_PORT_GROUP {
		SCSI_INQUIRY_TARGET_PORT_GROUP__ASYMMETRIC_UNSUPPORTED = 0,
		SCSI_INQUIRY_TARGET_PORT_GROUP__ASYMMETRIC_IMPLICIT = 1,
		SCSI_INQUIRY_TARGET_PORT_GROUP__ASYMMETRIC_EXPLICIT = 2,
		SCSI_INQUIRY_TARGET_PORT_GROUP__ASYMMETRIC_BOTH = 3
	};
	enum SCSI__INQUIRY__CLOCKING {
		SCSI_INQUIRY_CLOCKING__ST = 0,
		SCSI_INQUIRY_CLOCKING__DT = 1,
		SCSI_INQUIRY_CLOCKING__BOTH = 3
	};

	struct SCSI__INQUIRY__DATA {
		uint8_t flags0[2];
		uint8_t version;
		uint8_t flags1;
		uint8_t additionalLength;
		uint8_t flags2[3];
		uint64be_t t10VendorID;
		uint8_t productID[16];
		uint32be_t productRev;
		uint8_t serialNumber[8];
		uint8_t vendorUnique[12];
		uint8_t flags3;
		uint8_t reserved0;
		uint16be_t versionDescriptor[8];
		uint8_t reserved1[22];
		uint8_t vendorSpecific[164];
		SCSI_FIELD_GETTER
	};
	LT_ASSERT_SIZEOF(SCSI__INQUIRY__DATA, 260)
	LT_ASSERT_OFFSETOF(SCSI__INQUIRY__DATA, flags0, 0)
	LT_ASSERT_OFFSETOF(SCSI__INQUIRY__DATA, version, 2)
	LT_ASSERT_OFFSETOF(SCSI__INQUIRY__DATA, flags1, 3)
	LT_ASSERT_OFFSETOF(SCSI__INQUIRY__DATA, additionalLength, 4)
	LT_ASSERT_OFFSETOF(SCSI__INQUIRY__DATA, flags2, 5)
	LT_ASSERT_OFFSETOF(SCSI__INQUIRY__DATA, t10VendorID, 8)
	LT_ASSERT_OFFSETOF(SCSI__INQUIRY__DATA, productID, 16)
	LT_ASSERT_OFFSETOF(SCSI__INQUIRY__DATA, productRev, 32)
	LT_ASSERT_OFFSETOF(SCSI__INQUIRY__DATA, serialNumber, 36)
	LT_ASSERT_OFFSETOF(SCSI__INQUIRY__DATA, vendorUnique, 44)
	LT_ASSERT_OFFSETOF(SCSI__INQUIRY__DATA, flags3, 56)
	LT_ASSERT_OFFSETOF(SCSI__INQUIRY__DATA, versionDescriptor, 58)
	LT_ASSERT_OFFSETOF(SCSI__INQUIRY__DATA, vendorSpecific, 96)

	inline SCSICommand6 INQUIRY(bool evpd, uint8_t pageCode, uint16_t allocLength) {
		SCSICommand6 cmd = {};
		cmd.op = 0x12;
		cmd.data_bytes[0] = evpd ? 1 : 0;
		cmd.data_bytes[1] = pageCode;
		cmd.data_bytes[2] = allocLength >> 8;
		cmd.data_bytes[3] = allocLength;
		return cmd;
	}

	/* --------------
	 * | LOG SELECT |
	 * -------------- */

	enum SCSI__LOG_SELECT__PAGE_CONTROL {
		SCSI_LOG_SELECT_PAGE_CONTROL__THRESHOLD = 0,
		SCSI_LOG_SELECT_PAGE_CONTROL__CUMULATIVE = 1,
		SCSI_LOG_SELECT_PAGE_CONTROL__DEFAULT_THRESHOLD = 2,
		SCSI_LOG_SELECT_PAGE_CONTROL__DEFAULT_CUMULATIVE = 3
	};

	inline SCSICommand10 LOG_SELECT(bool pcr, bool sp, uint8_t pc, uint8_t pageCode, uint16_t paramListLength) {
		SCSICommand10 cmd = {};
		cmd.op = 0x4C;
		cmd.data_bytes[0] = (pcr ? 0x2 : 0) | (sp ? 1 : 0);
		cmd.data_bytes[1] = (pc << 6) | (pageCode & 0x3F);
		cmd.data_bytes[6] = paramListLength >> 8;
		cmd.data_bytes[7] = paramListLength;
		return cmd;
	}

	/* -------------
	 * | LOG SENSE |
	 * ------------- */

	inline SCSICommand10 LOG_SENSE(bool sp, uint8_t pc, uint8_t pageCode, uint16_t paramPtr, uint16_t allocLength) {
		SCSICommand10 cmd = {};
		cmd.op = 0x4D;
		cmd.data_bytes[0] = sp ? 1 : 0;
		cmd.data_bytes[1] = (pc << 6) | (pageCode & 0x3F);
		cmd.data_bytes[4] = paramPtr >> 8;
		cmd.data_bytes[5] = paramPtr;
		cmd.data_bytes[6] = allocLength >> 8;
		cmd.data_bytes[7] = allocLength;
		return cmd;
	}

	/* ---------------
	 * | MODE SELECT |
	 * --------------- */

	inline SCSICommand6 MODE_SELECT(bool pf, bool sp, uint8_t paramListLength) {
		SCSICommand6 cmd = {};
		cmd.op = 0x15;
		cmd.data_bytes[0] = (pf ? 0x10 : 0) | (sp ? 1 : 0);
		cmd.data_bytes[3] = paramListLength;
		return cmd;
	}

	inline SCSICommand10 MODE_SELECT_10(bool pf, bool sp, uint16_t paramListLength) {
		SCSICommand10 cmd = {};
		cmd.op = 0x55;
		cmd.data_bytes[0] = (pf ? 0x10 : 0) | (sp ? 1 : 0);
		cmd.data_bytes[6] = paramListLength >> 8;
		cmd.data_bytes[7] = paramListLength;
		return cmd;
	}

	/* --------------
	 * | MODE SENSE |
	 * -------------- */

	enum SCSI__MODE_SENSE__PAGE_CONTROL {
		SCSI_MODE_SENSE_PAGE_CONTROL__CURRENT = 0,
		SCSI_MODE_SENSE_PAGE_CONTROL__CHANGEABLE = 1,
		SCSI_MODE_SENSE_PAGE_CONTROL__DEFAULT = 2,
		SCSI_MODE_SENSE_PAGE_CONTROL__SAVED = 3
	};

	inline SCSICommand6 MODE_SENSE(bool dbd, uint8_t pc, uint8_t pageCode, uint8_t subpageCode, uint8_t allocLength) {
		SCSICommand6 cmd = {};
		cmd.op = 0x1A;
		cmd.data_bytes[0] = dbd ? 0x40 : 0;
		cmd.data_bytes[1] = (pc << 6) | (pageCode & 0x3F);
		cmd.data_bytes[2] = subpageCode;
		cmd.data_bytes[3] = allocLength;
		return cmd;
	}

	inline SCSICommand10 MODE_SENSE_10(bool llbaa, bool dbd, uint8_t pc, uint8_t pageCode, uint8_t subpageCode, uint16_t allocLength) {
		SCSICommand10 cmd = {};
		cmd.op = 0x5A;
		cmd.data_bytes[0] = (llbaa ? 0x10 : 0) | (dbd ? 0x08 : 0);
		cmd.data_bytes[1] = (pc << 6) | (pageCode & 0x3F);
		cmd.data_bytes[2] = subpageCode;
		cmd.data_bytes[6] = allocLength >> 16;
		cmd.data_bytes[7] = allocLength;
		return cmd;
	}

	/* -------------------------
	 * | PERSISTENT RESERVE IN |
	 * ------------------------- */

	struct SCSI__PERSISTENT_RESERVE_IN__DATA__READ_KEYS {
		uint32be_t prGeneration;
		uint32be_t additionalLength;
		uint32be_t reservationKeys[];
	};
	struct SCSI__PERSISTENT_RESERVE_IN__DATA__READ_RESERVATION {
		uint32be_t prGeneration;
		uint32be_t additionalLength;
		uint32be_t reservationKey;
		uint8_t reserved0[5];
		uint8_t scopeAndType;
		uint8_t reserved1[2];
	};
	struct SCSI__PERSISTENT_RESERVE_IN__DATA__REPORT_CAPABILITIES {
		uint64be_t length;
		uint8_t flags[2];
		uint16be_t prTypeMask;
		uint8_t reserved0[2];
		SCSI_FIELD_GETTER
	};

	enum SCSI__PERSISTENT_RESERVE_IN__REPORT_CAPABILITIES_FIELDS {
		SCSI_PERSISTENT_RESERVE_IN_REP_CAPS__COMPATIBLE_RESERV_HANDLING = SCSI_MAKE_FIELD(8,4,1),
		SCSI_PERSISTENT_RESERVE_IN_REP_CAPS__SPECIFY_INIT_PORTS_CAPABLE = SCSI_MAKE_FIELD(8,3,1),
		SCSI_PERSISTENT_RESERVE_IN_REP_CAPS__TARGET_PORTS_CAPABLE = SCSI_MAKE_FIELD(8,2,1),
		SCSI_PERSISTENT_RESERVE_IN_REP_CAPS__PERSISTENT_POWER_LOSS = SCSI_MAKE_FIELD(8,0,1),
		SCSI_PERSISTENT_RESERVE_IN_REP_CAPS__TYPE_MASK_VALID = SCSI_MAKE_FIELD(9,7,1),
		SCSI_PERSISTENT_RESERVE_IN_REP_CAPS__ALLOW_COMMANDS = SCSI_MAKE_FIELD(9,4,3),
		SCSI_PERSISTENT_RESERVE_IN_REP_CAPS__PERSIST_THROUGH_POWER_LOSS = SCSI_MAKE_FIELD(9,0,1)
	};
	enum SCSI__PERSISTENT_RESERVE_IN__ACTION {
		SCSI_PERSISTENT_RESERVE_IN_ACTION__READ_KEYS = 0,
		SCSI_PERSISTENT_RESERVE_IN_ACTION__READ_RESERVATION = 1,
		SCSI_PERSISTENT_RESERVE_IN_ACTION__REPORT_CAPABILITIES = 2,
		SCSI_PERSISTENT_RESERVE_IN_ACTION__READ_FULL_STATUS = 3
	};
	enum SCSI__PERSISTENT_RESERVE_IN__READ_RESERVATION__TYPE {
		SCSI_PERSISTENT_RESERVE_IN_RR_TYPE__WRITE_EXCLUSIVE = 1,
		SCSI_PERSISTENT_RESERVE_IN_RR_TYPE__EXCLUSIVE_ACCESS = 3,
		SCSI_PERSISTENT_RESERVE_IN_RR_TYPE__REGISTRANT_WRITE_EXCLUSIVE = 5,
		SCSI_PERSISTENT_RESERVE_IN_RR_TYPE__REGISTRANT_EXCLUSIVE_ACCESS = 6,
		SCSI_PERSISTENT_RESERVE_IN_RR_TYPE__ALL_WRITE_EXCLUSIVE = 7,
		SCSI_PERSISTENT_RESERVE_IN_RR_TYPE__ALL_EXCLUSIVE_ACCESS = 8,
	};
	enum SCSI__PERSISTENT_RESERVE_IN__ALLOW_COMMANDS {
		SCSI_PERSISTENT_RESERVE_IN_ALLOW_COMMANDS__UNDEFINED = 0,
		SCSI_PERSISTENT_RESERVE_IN_ALLOW_COMMANDS__UNKNOWN = 1,
		SCSI_PERSISTENT_RESERVE_IN_ALLOW_COMMANDS__DENY = 2,
		SCSI_PERSISTENT_RESERVE_IN_ALLOW_COMMANDS__ALLOW = 3
	};
	enum SCSI__PERSISTENT_RESERVE_IN__TYPE_MASK {
		SCSI_PERSISTENT_RESERVE_IN_TYPE_MASK__WR_EX_AR = 0x8000,
		SCSI_PERSISTENT_RESERVE_IN_TYPE_MASK__EX_AC_RO = 0x4000,
		SCSI_PERSISTENT_RESERVE_IN_TYPE_MASK__WR_EX_RO = 0x2000,
		SCSI_PERSISTENT_RESERVE_IN_TYPE_MASK__EX_AC    = 0x0800,
		SCSI_PERSISTENT_RESERVE_IN_TYPE_MASK__WR_EX    = 0x0200,
		SCSI_PERSISTENT_RESERVE_IN_TYPE_MASK__EX_AC_AR = 0x0001
	};

	inline SCSICommand10 PERSISTENT_RESERVE_IN(uint8_t serviceAction, uint16_t allocLength) {
		SCSICommand10 cmd = {};
		cmd.op = 0x5E;
		cmd.data_bytes[0] = serviceAction & 0x1F;
		cmd.data_bytes[6] = allocLength >> 8;
		cmd.data_bytes[7] = allocLength;
		return cmd;
	}

	inline SCSICommand10 PERSISTENT_RESERVE_OUT(uint8_t serviceAction, uint8_t scope, uint8_t type, uint16_t paramListLength) {
		SCSICommand10 cmd = {};
		cmd.op = 0x5F;
		cmd.data_bytes[0] = serviceAction & 0x1F;
		cmd.data_bytes[1] = (scope << 4) | (type & 0xF);
		cmd.data_bytes[4] = paramListLength >> 8;
		cmd.data_bytes[5] = paramListLength;
		return cmd;
	}

	inline SCSICommand6 READ(uint32_t lba, uint8_t length) {
		SCSICommand6 cmd = {};
		cmd.op = 0x08;
		cmd.data_bytes[0] = (lba >> 16) & 0x1F;
		cmd.data_bytes[1] = lba >> 8;
		cmd.data_bytes[2] = lba;
		cmd.data_bytes[3] = length;
		return cmd;
	}

	inline SCSICommand10 READ_10(uint8_t rdProt, bool dpo, bool fua, bool fuaNV, uint32_t lba, uint8_t group, uint16_t length) {
		SCSICommand10 cmd = {};
		cmd.op = 0x28;
		cmd.data_bytes[0] = (rdProt << 5) | (dpo ? 0x10 : 0) | (fua ? 0x08 : 0) | (fuaNV ? 0x02 : 0);
		cmd.data_bytes[1] = lba >> 24;
		cmd.data_bytes[2] = lba >> 16;
		cmd.data_bytes[3] = lba >> 8;
		cmd.data_bytes[4] = lba;
		cmd.data_bytes[5] = group & 0x1F;
		cmd.data_bytes[6] = length >> 8;
		cmd.data_bytes[7] = length;
		return cmd;
	}

	inline SCSICommand12 READ_12(uint8_t rdProt, bool dpo, bool fua, bool fuaNV, uint32_t lba, uint32_t length, uint8_t group) {
		SCSICommand12 cmd = {};
		cmd.data_bytes[0] = (rdProt << 5) | (dpo ? 0x10 : 0) | (fua ? 0x08 : 0) | (fuaNV ? 0x02 : 0);
		cmd.data_bytes[1] = lba >> 24;
		cmd.data_bytes[2] = lba >> 16;
		cmd.data_bytes[3] = lba >> 8;
		cmd.data_bytes[4] = lba;
		cmd.data_bytes[5] = length >> 24;
		cmd.data_bytes[6] = length >> 16;
		cmd.data_bytes[7] = length >> 8;
		cmd.data_bytes[8] = length;
		cmd.data_bytes[9] = group;
		return cmd;
	}

	inline SCSICommand16 READ_16(uint8_t rdProt, bool dpo, bool fua, bool fuaNV, uint64_t lba, uint32_t length, uint8_t group) {
		SCSICommand16 cmd = {};
		cmd.data_bytes[0] = (rdProt << 5) | (dpo ? 0x10 : 0) | (fua ? 0x08 : 0) | (fuaNV ? 0x02 : 0);
		cmd.data_bytes[1] = lba >> 56;
		cmd.data_bytes[2] = lba >> 48;
		cmd.data_bytes[3] = lba >> 40;
		cmd.data_bytes[4] = lba >> 32;
		cmd.data_bytes[5] = lba >> 24;
		cmd.data_bytes[6] = lba >> 16;
		cmd.data_bytes[7] = lba >> 8;
		cmd.data_bytes[8] = lba;
		cmd.data_bytes[9] = length >> 24;
		cmd.data_bytes[10] = length >> 16;
		cmd.data_bytes[11] = length >> 8;
		cmd.data_bytes[12] = length;
		cmd.data_bytes[13] = group;
		return cmd;
	}

	inline SCSICommand32 READ_32(uint8_t groupNum, uint8_t rdProt, bool dpo, bool fua, bool fuaNV,
			uint64_t lba, uint32_t initialLbaTag, uint16_t lbaAppTag, uint16_t lbaAppTagMask, uint32_t length) {
		SCSICommand32 cmd = {};
		cmd.op = 0x7F;
		cmd.groupNumber = groupNum & 0x1F;
		cmd.additionalLength = 0x18;
		cmd.serviceAction = 0x0009;
		cmd.flags = (rdProt << 5) | (dpo ? 0x10 : 0) | (fua ? 0x08 : 0) | (fuaNV ? 0x20 : 0);
		cmd.lba = lba;
		cmd.initialLbaTag = initialLbaTag;
		cmd.lbaAppTag = lbaAppTag;
		cmd.lbaAppTagMask = lbaAppTagMask;
		cmd.length = length;
		return cmd;
	}

	inline SCSICommand10 READ_BUFFER(uint8_t mode, uint8_t bufID, uint32_t bufOffset, uint32_t allocLength) {
		SCSICommand10 cmd = {};
		cmd.op = 0x3C;
		cmd.data_bytes[0] = mode & 0x1F;
		cmd.data_bytes[1] = bufID;
		cmd.data_bytes[2] = bufOffset >> 16;
		cmd.data_bytes[3] = bufOffset >> 8;
		cmd.data_bytes[4] = bufOffset;
		cmd.data_bytes[6] = allocLength >> 16;
		cmd.data_bytes[7] = allocLength >> 8;
		cmd.data_bytes[8] = allocLength;
		return cmd;
	}

	inline SCSICommand10 READ_CAPACITY(uint32_t lba, bool pmi) {
		SCSICommand10 cmd = {};
		cmd.op = 0x25;
		cmd.data_bytes[1] = lba >> 24;
		cmd.data_bytes[2] = lba >> 16;
		cmd.data_bytes[3] = lba >> 8;
		cmd.data_bytes[4] = lba;
		cmd.data_bytes[7] = pmi ? 1 : 0;
		return cmd;
	}

	inline SCSICommand16 READ_CAPACITY_16(uint64_t lba, uint32_t allocLength, bool pmi) {
		SCSICommand16 cmd = {};
		cmd.op = 0x9E;
		cmd.data_bytes[0] = 0x10;
		cmd.data_bytes[1] = lba >> 56;
		cmd.data_bytes[2] = lba >> 48;
		cmd.data_bytes[3] = lba >> 40;
		cmd.data_bytes[4] = lba >> 32;
		cmd.data_bytes[5] = lba >> 24;
		cmd.data_bytes[6] = lba >> 16;
		cmd.data_bytes[7] = lba >> 8;
		cmd.data_bytes[8] = lba;
		return cmd;
	}

	inline SCSICommand10 READ_DEFECT_DATA(bool plist, bool glist, uint8_t listFmt, uint16_t allocLength) {
		SCSICommand10 cmd = {};
		cmd.op = 0x37;
		cmd.data_bytes[1] = (plist ? 0x10 : 0) | (glist ? 0x08 : 0) | (listFmt & 0x7);
		cmd.data_bytes[6] = allocLength >> 8;
		cmd.data_bytes[7] = allocLength;
		return cmd;
	}

	inline SCSICommand12 READ_DEFECT_DATA_12(bool plist, bool glist, uint8_t listFmt, uint32_t allocLength) {
		SCSICommand12 cmd = {};
		cmd.op = 0xB7;
		cmd.data_bytes[1] = (plist ? 0x10 : 0) | (glist ? 0x08 : 0) | (listFmt & 0x7);
		cmd.data_bytes[5] = allocLength >> 24;
		cmd.data_bytes[6] = allocLength >> 16;
		cmd.data_bytes[7] = allocLength >> 8;
		cmd.data_bytes[8] = allocLength;
		return cmd;
	}

	inline SCSICommand10 READ_LONG(bool pblock, bool correct, uint32_t lba, uint16_t transferLen) {
		SCSICommand10 cmd = {};
		cmd.op = 0x3E;
		cmd.data_bytes[0] = (pblock ? 0x04 : 0) | (correct ? 0x02 : 0);
		cmd.data_bytes[1] = lba >> 24;
		cmd.data_bytes[2] = lba >> 16;
		cmd.data_bytes[3] = lba >> 8;
		cmd.data_bytes[4] = lba;
		cmd.data_bytes[6] = transferLen >> 8;
		cmd.data_bytes[7] = transferLen;
		return cmd;
	}

	inline SCSICommand6 REASSIGN_BLOCKS(bool longLBA, bool longList) {
		SCSICommand6 cmd = {};
		cmd.op = 0x07;
		cmd.data_bytes[0] = (longLBA ? 0x02 : 0) | (longList ? 0x01 : 0);
		return cmd;
	}

	inline SCSICommand6 RECEIVE_DIAGNOSTIC_RESULTS(bool pcv, uint8_t pageCode, uint16_t allocLength) {
		SCSICommand6 cmd = {};
		cmd.op = 0x1C;
		cmd.data_bytes[0] = pcv ? 1 : 0;
		cmd.data_bytes[1] = pageCode;
		cmd.data_bytes[2] = allocLength >> 8;
		cmd.data_bytes[3] = allocLength;
		return cmd;
	}

	inline SCSICommand12 REPORT_DEVICE_IDENTIFIER(uint32_t allocLength, uint8_t infoType) {
		SCSICommand12 cmd = {};
		cmd.op = 0xA3;
		cmd.data_bytes[0] = 0x05;
		cmd.data_bytes[5] = allocLength >> 24;
		cmd.data_bytes[6] = allocLength >> 16;
		cmd.data_bytes[7] = allocLength >> 8;
		cmd.data_bytes[8] = allocLength;
		cmd.data_bytes[9] = infoType << 1;
		return cmd;
	}

	inline SCSICommand12 REPORT_LUNS(uint8_t selReport, uint32_t allocLength) {
		SCSICommand12 cmd = {};
		cmd.op = 0xA0;
		cmd.data_bytes[1] = selReport;
		cmd.data_bytes[5] = allocLength >> 24;
		cmd.data_bytes[6] = allocLength >> 16;
		cmd.data_bytes[7] = allocLength >> 8;
		cmd.data_bytes[8] = allocLength;
		return cmd;
	}

	inline SCSICommand12 REPORT_SUPPORTED_OPERATION_CODES(bool rctd, uint8_t reportingOpts, uint8_t opcode, uint16_t serviceAction,
			uint32_t allocLength) {
		SCSICommand12 cmd = {};
		cmd.op = 0xA3;
		cmd.data_bytes[0] = 0x0C;
		cmd.data_bytes[1] = (rctd ? 0x80 : 0) | (reportingOpts & 0x7);
		cmd.data_bytes[2] = opcode;
		cmd.data_bytes[3] = serviceAction >> 8;
		cmd.data_bytes[4] = serviceAction;
		cmd.data_bytes[5] = allocLength >> 24;
		cmd.data_bytes[6] = allocLength >> 16;
		cmd.data_bytes[7] = allocLength >> 8;
		cmd.data_bytes[8] = allocLength;
		return cmd;
	}

	inline SCSICommand12 REPORT_SUPPORTED_TASK_MANAGEMENT_FUNCTIONS(uint32_t allocLength) {
		SCSICommand12 cmd = {};
		cmd.op = 0xA3;
		cmd.data_bytes[0] = 0x0D;
		cmd.data_bytes[5] = allocLength >> 24;
		cmd.data_bytes[6] = allocLength >> 16;
		cmd.data_bytes[7] = allocLength >> 8;
		cmd.data_bytes[8] = allocLength;
		return cmd;
	}

	inline SCSICommand6 REQUEST_SENSE(bool desc, uint8_t allocLength) {
		SCSICommand6 cmd = {};
		cmd.op = 0x03;
		cmd.data_bytes[0] = desc ? 1 : 0;
		cmd.data_bytes[3] = allocLength;
		return cmd;
	}

	inline SCSICommand12 SECURITY_PROTOCOL_IN(uint8_t protocol, uint16_t protSpecific, bool inc512, uint32_t allocLength) {
		SCSICommand12 cmd = {};
		cmd.op = 0xA2;
		cmd.data_bytes[0] = protocol;
		cmd.data_bytes[1] = protSpecific >> 8;
		cmd.data_bytes[2] = protSpecific;
		cmd.data_bytes[3] = inc512 ? 0x80 : 0;
		cmd.data_bytes[5] = allocLength >> 24;
		cmd.data_bytes[6] = allocLength >> 16;
		cmd.data_bytes[7] = allocLength >> 8;
		cmd.data_bytes[8] = allocLength;
		return cmd;
	}

	inline SCSICommand12 SECURITY_PROTOCOL_OUT(uint8_t protocol, uint16_t protSpecific, bool inc512, uint32_t transferLength) {
		SCSICommand12 cmd = {};
		cmd.op = 0xA2;
		cmd.data_bytes[0] = protocol;
		cmd.data_bytes[1] = protSpecific >> 8;
		cmd.data_bytes[2] = protSpecific;
		cmd.data_bytes[3] = inc512 ? 0x80 : 0;
		cmd.data_bytes[5] = transferLength >> 24;
		cmd.data_bytes[6] = transferLength >> 16;
		cmd.data_bytes[7] = transferLength >> 8;
		cmd.data_bytes[8] = transferLength;
		return cmd;
	}

	inline SCSICommand6 SEND_DIAGNOSTIC(uint8_t selfTestCode, bool pf, bool selfTest, bool devOffline, bool unitOffline,
			uint16_t paramListLength) {
		SCSICommand6 cmd = {};
		cmd.op = 0x1D;
		cmd.data_bytes[0] = (selfTestCode << 5) | (pf ? 0x10 : 0) | (selfTest ? 0x04 : 0) | (devOffline ? 0x02 : 0) | (unitOffline ? 1 : 0);
		cmd.data_bytes[2] = paramListLength >> 8;
		cmd.data_bytes[3] = paramListLength;
		return cmd;
	}

	inline SCSICommand12 SET_IDENTIFYING_INFORMATION(uint32_t paramListLength, uint8_t infoType) {
		SCSICommand12 cmd = {};
		cmd.op = 0xA4;
		cmd.data_bytes[0] = 0x06;
		cmd.data_bytes[5] = paramListLength >> 24;
		cmd.data_bytes[6] = paramListLength >> 16;
		cmd.data_bytes[7] = paramListLength >> 8;
		cmd.data_bytes[8] = paramListLength;
		cmd.data_bytes[9] = infoType << 1;
		return cmd;
	}

	inline SCSICommand12 SET_TIMESTAMP(uint32_t paramListLength) {
		SCSICommand12 cmd = {};
		cmd.op = 0x0F;
		cmd.data_bytes[0] = 0x0F;
		cmd.data_bytes[5] = paramListLength >> 24;
		cmd.data_bytes[6] = paramListLength >> 16;
		cmd.data_bytes[7] = paramListLength >> 8;
		cmd.data_bytes[8] = paramListLength;
		return cmd;
	}

	inline SCSICommand6 START_STOP_UNIT(bool immed, uint8_t powerCondMod, uint8_t powerCond, bool noflush, bool loej, bool start) {
		SCSICommand6 cmd = {};
		cmd.op = 0x1B;
		cmd.data_bytes[0] = immed ? 1 : 0;
		cmd.data_bytes[2] = powerCondMod & 0xF;
		cmd.data_bytes[3] = (powerCond << 4) | (noflush ? 0x04 : 0) | (loej ? 0x02 : 0) | (start ? 1 : 0);
		return cmd;
	}

	inline SCSICommand10 SYNCHRONIZE_CACHE(bool syncNV, bool immed, uint32_t lba, uint8_t groupNum, uint16_t numBlocks) {
		SCSICommand10 cmd = {};
		cmd.op = 0x35;
		cmd.data_bytes[0] = (syncNV ? 0x04 : 0) | (immed ? 0x02 : 0);
		cmd.data_bytes[1] = lba >> 24;
		cmd.data_bytes[2] = lba >> 16;
		cmd.data_bytes[3] = lba >> 8;
		cmd.data_bytes[4] = lba;
		cmd.data_bytes[5] = groupNum & 0x1F;
		cmd.data_bytes[6] = numBlocks >> 8;
		cmd.data_bytes[7] = numBlocks;
		return cmd;
	}

	inline SCSICommand6 TEST_UNIT_READY() {
		SCSICommand6 cmd = {};
		cmd.op = 0x00;
		return cmd;
	}

	inline SCSICommand10 UNMAP(uint8_t groupNum, uint16_t paramListLength) {
		SCSICommand10 cmd = {};
		cmd.op = 0x42;
		cmd.data_bytes[5] = groupNum & 0x1F;
		cmd.data_bytes[6] = paramListLength >> 8;
		cmd.data_bytes[7] = paramListLength;
		return cmd;
	}

	inline SCSICommand10 VERIFY(uint8_t vprot, bool dpo, bool byteCheck, uint32_t lba, uint8_t groupNum, uint16_t verifyLength) {
		SCSICommand10 cmd = {};
		cmd.op = 0x2F;
		cmd.data_bytes[0] = (vprot << 5) | (dpo ? 0x10 : 0) | (byteCheck ? 0x02 : 0);
		cmd.data_bytes[1] = lba >> 24;
		cmd.data_bytes[2] = lba >> 16;
		cmd.data_bytes[3] = lba >> 8;
		cmd.data_bytes[4] = lba;
		cmd.data_bytes[5] = groupNum & 0x1F;
		cmd.data_bytes[6] = verifyLength >> 8;
		cmd.data_bytes[7] = verifyLength;
		return cmd;
	}

	inline SCSICommand12 VERIFY_12(uint8_t vprot, bool dpo, bool byteCheck, uint32_t lba, uint32_t verifyLength, uint8_t groupNum) {
		SCSICommand12 cmd = {};
		cmd.op = 0xAF;
		cmd.data_bytes[0] = (vprot << 5) | (dpo ? 0x10 : 0) | (byteCheck ? 0x02 : 0);
		cmd.data_bytes[1] = lba >> 24;
		cmd.data_bytes[2] = lba >> 16;
		cmd.data_bytes[3] = lba >> 8;
		cmd.data_bytes[4] = lba;
		cmd.data_bytes[5] = verifyLength >> 24;
		cmd.data_bytes[6] = verifyLength >> 16;
		cmd.data_bytes[7] = verifyLength >> 8;
		cmd.data_bytes[8] = verifyLength;
		cmd.data_bytes[9] = groupNum & 0x1F;
		return cmd;
	}

	inline SCSICommand16 VERIFY_16(uint8_t vprot, bool dpo, bool byteCheck, uint64_t lba, uint32_t verifyLength, uint8_t groupNum) {
		SCSICommand12 cmd = {};
		cmd.op = 0xAF;
		cmd.data_bytes[0] = (vprot << 5) | (dpo ? 0x10 : 0) | (byteCheck ? 0x02 : 0);
		cmd.data_bytes[1] = lba >> 56;
		cmd.data_bytes[2] = lba >> 48;
		cmd.data_bytes[3] = lba >> 40;
		cmd.data_bytes[4] = lba >> 32;
		cmd.data_bytes[5] = lba >> 24;
		cmd.data_bytes[6] = lba >> 16;
		cmd.data_bytes[7] = lba >> 8;
		cmd.data_bytes[8] = lba;
		cmd.data_bytes[9] = verifyLength >> 24;
		cmd.data_bytes[10] = verifyLength >> 16;
		cmd.data_bytes[11] = verifyLength >> 8;
		cmd.data_bytes[12] = verifyLength;
		cmd.data_bytes[13] = groupNum & 0x1F;
		return cmd;
	}

	inline SCSICommand32 VERIFY_32(uint8_t groupNum, uint8_t vprot, bool dpo, bool byteCheck, uint64_t lba, uint32_t initialLbaTag,
			uint16_t lbaAppTag, uint16_t lbaAppTagMask, uint32_t length) {
		SCSICommand32 cmd = {};
		cmd.op = 0x7F;
		cmd.groupNumber = groupNum & 0x1F;
		cmd.additionalLength = 0x18;
		cmd.serviceAction = 0x000A;
		cmd.flags = (vprot << 5) | (dpo ? 0x10 : 0) | (byteCheck ? 0x02 : 0);
		cmd.lba = lba;
		cmd.initialLbaTag = initialLbaTag;
		cmd.lbaAppTag = lbaAppTag;
		cmd.lbaAppTagMask = lbaAppTagMask;
		cmd.length = length;
		return cmd;
	}

	inline SCSICommand6 WRITE(uint32_t lba, uint8_t length) {
		SCSICommand6 cmd = {};
		cmd.op = 0x0A;
		cmd.data_bytes[0] = (lba >> 16) & 0x1F;
		cmd.data_bytes[1] = lba >> 8;
		cmd.data_bytes[2] = lba;
		cmd.data_bytes[3] = length;
		return cmd;
	}

	inline SCSICommand10 WRITE_10(uint8_t wrProt, bool dpo, bool fua, bool fuaNV, uint32_t lba, uint8_t groupNum, uint16_t length) {
		SCSICommand10 cmd = {};
		cmd.op = 0x2A;
		cmd.data_bytes[0] = (wrProt << 5) | (dpo ? 0x10 : 0) | (fua ? 0x08 : 0) | (fuaNV ? 0x02 : 0);
		cmd.data_bytes[1] = lba >> 24;
		cmd.data_bytes[2] = lba >> 16;
		cmd.data_bytes[3] = lba >> 8;
		cmd.data_bytes[4] = lba;
		cmd.data_bytes[5] = groupNum & 0x1F;
		cmd.data_bytes[6] = length >> 8;
		cmd.data_bytes[7] = length;
		return cmd;
	}

	inline SCSICommand12 WRITE_12(uint8_t wrProt, bool dpo, bool fua, bool fuaNV, uint32_t lba, uint32_t length, uint8_t groupNum) {
		SCSICommand12 cmd = {};
		cmd.op = 0x2A;
		cmd.data_bytes[0] = (wrProt << 5) | (dpo ? 0x10 : 0) | (fua ? 0x08 : 0) | (fuaNV ? 0x02 : 0);
		cmd.data_bytes[1] = lba >> 24;
		cmd.data_bytes[2] = lba >> 16;
		cmd.data_bytes[3] = lba >> 8;
		cmd.data_bytes[4] = lba;
		cmd.data_bytes[5] = length >> 24;
		cmd.data_bytes[6] = length >> 16;
		cmd.data_bytes[7] = length >> 8;
		cmd.data_bytes[8] = length;
		cmd.data_bytes[9] = groupNum & 0x1F;
		return cmd;
	}

	inline SCSICommand16 WRITE_16(uint8_t wrProt, bool dpo, bool fua, bool fuaNV, uint64_t lba, uint32_t length, uint8_t groupNum) {
		SCSICommand16 cmd = {};
		cmd.op = 0x2A;
		cmd.data_bytes[0] = (wrProt << 5) | (dpo ? 0x10 : 0) | (fua ? 0x08 : 0) | (fuaNV ? 0x02 : 0);
		cmd.data_bytes[1] = lba >> 56;
		cmd.data_bytes[2] = lba >> 48;
		cmd.data_bytes[3] = lba >> 40;
		cmd.data_bytes[4] = lba >> 32;
		cmd.data_bytes[5] = lba >> 24;
		cmd.data_bytes[6] = lba >> 16;
		cmd.data_bytes[7] = lba >> 8;
		cmd.data_bytes[8] = lba;
		cmd.data_bytes[9] = length >> 24;
		cmd.data_bytes[10] = length >> 16;
		cmd.data_bytes[11] = length >> 8;
		cmd.data_bytes[12] = length;
		cmd.data_bytes[13] = groupNum & 0x1F;
		return cmd;
	}

	inline SCSICommand32 WRITE_32(uint8_t groupNum, uint8_t wrProt, bool dpo, bool fua, bool fuaNV, uint64_t lba, uint32_t initialLbaTag,
			uint16_t lbaAppTag, uint16_t lbaAppTagMask, uint32_t length) {
		SCSICommand32 cmd = {};
		cmd.op = 0x7F;
		cmd.groupNumber = groupNum & 0x1F;
		cmd.additionalLength = 0x18;
		cmd.serviceAction = 0x000B;
		cmd.flags = (wrProt << 5) | (dpo ? 0x10 : 0) | (fua ? 0x08 : 0) | (fuaNV ? 0x02 : 0);
		cmd.lba = lba;
		cmd.initialLbaTag = initialLbaTag;
		cmd.lbaAppTag = lbaAppTag;
		cmd.lbaAppTagMask = lbaAppTagMask;
		cmd.length = length;
		return cmd;
	}

	inline SCSICommand10 WRITE_AND_VERIFY(uint8_t wrProt, bool dpo, bool byteCheck, uint32_t lba, uint8_t groupNum, uint16_t length) {
		SCSICommand10 cmd = {};
		cmd.op = 0x2E;
		cmd.data_bytes[0] = (wrProt << 5) | (dpo ? 0x10 : 0) | (byteCheck ? 0x02 : 0);
		cmd.data_bytes[1] = lba >> 24;
		cmd.data_bytes[2] = lba >> 16;
		cmd.data_bytes[3] = lba >> 8;
		cmd.data_bytes[4] = lba;
		cmd.data_bytes[5] = groupNum & 0x1F;
		cmd.data_bytes[6] = length >> 8;
		cmd.data_bytes[7] = length;
		return cmd;
	}

	inline SCSICommand12 WRITE_AND_VERIFY_12(uint8_t wrProt, bool dpo, bool byteCheck, uint32_t lba, uint32_t length, uint8_t groupNum) {
		SCSICommand12 cmd = {};
		cmd.op = 0xAE;
		cmd.data_bytes[0] = (wrProt << 5) | (dpo ? 0x10 : 0) | (byteCheck ? 0x02 : 0);
		cmd.data_bytes[1] = lba >> 24;
		cmd.data_bytes[2] = lba >> 16;
		cmd.data_bytes[3] = lba >> 8;
		cmd.data_bytes[4] = lba;
		cmd.data_bytes[5] = length >> 24;
		cmd.data_bytes[6] = length >> 16;
		cmd.data_bytes[7] = length >> 8;
		cmd.data_bytes[8] = length;
		cmd.data_bytes[9] = groupNum & 0x1F;
		return cmd;
	}

	inline SCSICommand16 WRITE_AND_VERIFY_16(uint8_t wrProt, bool dpo, bool byteCheck, uint64_t lba, uint32_t length, uint8_t groupNum) {
		SCSICommand12 cmd = {};
		cmd.op = 0x8E;
		cmd.data_bytes[0] = (wrProt << 5) | (dpo ? 0x10 : 0) | (byteCheck ? 0x02 : 0);
		cmd.data_bytes[1] = lba >> 56;
		cmd.data_bytes[2] = lba >> 48;
		cmd.data_bytes[3] = lba >> 40;
		cmd.data_bytes[4] = lba >> 32;
		cmd.data_bytes[5] = lba >> 24;
		cmd.data_bytes[6] = lba >> 16;
		cmd.data_bytes[7] = lba >> 8;
		cmd.data_bytes[8] = lba;
		cmd.data_bytes[9] = length >> 24;
		cmd.data_bytes[10] = length >> 16;
		cmd.data_bytes[11] = length >> 8;
		cmd.data_bytes[12] = length;
		cmd.data_bytes[13] = groupNum & 0x1F;
		return cmd;
	}

	inline SCSICommand32 WRITE_AND_VERIFY_32(uint8_t groupNum, uint8_t wrProt, bool dpo, bool byteCheck, uint64_t lba, uint32_t initialLbaTag,
			uint16_t lbaAppTag, uint16_t lbaAppTagMask, uint32_t length) {
		SCSICommand32 cmd = {};
		cmd.op = 0x7F;
		cmd.groupNumber = groupNum & 0x1F;
		cmd.additionalLength = 0x18;
		cmd.serviceAction = 0x000C;
		cmd.flags = (wrProt << 5) | (dpo ? 0x10 : 0) | (byteCheck ? 0x02 : 0);
		cmd.lba = lba;
		cmd.initialLbaTag = initialLbaTag;
		cmd.lbaAppTag = lbaAppTag;
		cmd.lbaAppTagMask = lbaAppTagMask;
		cmd.length = length;
		return cmd;
	}

	inline SCSICommand10 WRITE_BUFFER(uint8_t mode, uint8_t bufID, uint32_t bufOffset, uint32_t paramListLength) {
		SCSICommand10 cmd = {};
		cmd.op = 0x3B;
		cmd.data_bytes[0] = mode & 0x1F;
		cmd.data_bytes[1] = bufID;
		cmd.data_bytes[2] = bufOffset >> 16;
		cmd.data_bytes[3] = bufOffset >> 8;
		cmd.data_bytes[4] = bufOffset;
		cmd.data_bytes[5] = paramListLength >> 16;
		cmd.data_bytes[6] = paramListLength >> 8;
		cmd.data_bytes[7] = paramListLength;
		return cmd;
	}

	inline SCSICommand10 WRITE_LONG(bool correctDisable, bool wrUncorrectable, bool pBlock, uint32_t lba, uint16_t length) {
		SCSICommand10 cmd = {};
		cmd.op = 0x3F;
		cmd.data_bytes[0] = (correctDisable ? 0x80 : 0) | (wrUncorrectable ? 0x40 : 0) | (pBlock ? 0x20 : 0);
		cmd.data_bytes[1] = lba >> 24;
		cmd.data_bytes[2] = lba >> 16;
		cmd.data_bytes[3] = lba >> 8;
		cmd.data_bytes[4] = lba;
		cmd.data_bytes[6] = length >> 8;
		cmd.data_bytes[7] = length;
		return cmd;
	}

	inline SCSICommand16 WRITE_LONG_16(bool correctDisable, bool wrUncorrectable, bool pBlock, uint64_t lba, uint16_t length) {
		SCSICommand16 cmd = {};
		cmd.op = 0x9F;
		cmd.data_bytes[0] = (correctDisable ? 0x80 : 0) | (wrUncorrectable ? 0x40 : 0) | (pBlock ? 0x20 : 0);
		cmd.data_bytes[1] = lba >> 56;
		cmd.data_bytes[2] = lba >> 48;
		cmd.data_bytes[3] = lba >> 40;
		cmd.data_bytes[4] = lba >> 32;
		cmd.data_bytes[5] = lba >> 24;
		cmd.data_bytes[6] = lba >> 16;
		cmd.data_bytes[7] = lba >> 8;
		cmd.data_bytes[8] = lba;
		cmd.data_bytes[11] = length >> 8;
		cmd.data_bytes[12] = length;
		return cmd;
	}

	inline SCSICommand10 WRITE_SAME(uint8_t wrProt, bool pbData, bool lbData, uint32_t lba, uint8_t groupNum, uint16_t numBlocks) {
		SCSICommand10 cmd = {};
		cmd.op = 0x41;
		cmd.data_bytes[0] = (wrProt << 5) | (pbData ? 0x04 : 0) | (lbData ? 0x02 : 0);
		cmd.data_bytes[1] = lba >> 24;
		cmd.data_bytes[2] = lba >> 16;
		cmd.data_bytes[3] = lba >> 8;
		cmd.data_bytes[4] = lba;
		cmd.data_bytes[5] = groupNum & 0x1F;
		cmd.data_bytes[6] = numBlocks >> 8;
		cmd.data_bytes[7] = numBlocks;
		return cmd;
	}

	inline SCSICommand16 WRITE_SAME_16(uint8_t wrProt, bool pbData, bool lbData, uint64_t lba, uint32_t numBlocks, uint8_t groupNum) {
		SCSICommand16 cmd = {};
		cmd.op = 0x93;
		cmd.data_bytes[0] = (wrProt << 5) | (pbData ? 0x04 : 0) | (lbData ? 0x02 : 0);
		cmd.data_bytes[1] = lba >> 56;
		cmd.data_bytes[2] = lba >> 48;
		cmd.data_bytes[3] = lba >> 40;
		cmd.data_bytes[4] = lba >> 32;
		cmd.data_bytes[5] = lba >> 24;
		cmd.data_bytes[6] = lba >> 16;
		cmd.data_bytes[7] = lba >> 8;
		cmd.data_bytes[8] = lba;
		cmd.data_bytes[9] = numBlocks >> 24;
		cmd.data_bytes[10] = numBlocks >> 16;
		cmd.data_bytes[11] = numBlocks >> 8;
		cmd.data_bytes[12] = numBlocks;
		cmd.data_bytes[13] = groupNum & 0x1F;
		return cmd;
	}

	inline SCSICommand32 WRITE_SAME_32(uint8_t groupNum, uint8_t wrProt, bool unmap, bool pbData, bool lbData, uint64_t lba,
			uint32_t initialLbaTag, uint16_t lbaAppTag, uint16_t lbaAppTagMask, uint32_t numBlocks) {
		SCSICommand32 cmd = {};
		cmd.op = 0x7F;
		cmd.groupNumber = groupNum & 0x1F;
		cmd.additionalLength = 0x18;
		cmd.serviceAction = 0x000D;
		cmd.flags = (wrProt << 5) | (unmap ? 0x08 : 0) | (pbData ? 0x04 : 0) | (lbData ? 0x02 : 0);
		cmd.lba = lba;
		cmd.initialLbaTag = initialLbaTag;
		cmd.lbaAppTag = lbaAppTag;
		cmd.lbaAppTagMask = lbaAppTagMask;
		cmd.length = numBlocks;
		return cmd;
	}

}


#endif /* LTOS_API_BUS_SCSI_CMDS_INC_ */
