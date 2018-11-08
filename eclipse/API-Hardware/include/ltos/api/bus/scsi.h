/*
 * scsi.h
 *
 *  Created on: Nov 3, 2018
 *      Author: Zekrom_64
 */

#ifndef LTOS_API_BUS_SCSI_H_
#define LTOS_API_BUS_SCSI_H_


#include "ltos/lttypes.h"
#include "ltos/ltmacros.h"
#include "ltos/api/ltoptimize.h"

namespace ltos::SCSI {

	LT_PACK_PREFIX
	struct SCSICommand6 {
		uint8_t op;
		union {
			uint8_t data_bytes[5];
			struct {
				uint8_t cdbInfoAndLBAHigh;
				uint8_t lbaMiddle;
				uint8_t lbaLow;
				uint8_t length;
				uint8_t control;
			};
		};
		inline void setLBA(uint32_t lba) {
			cdbInfoAndLBAHigh = (cdbInfoAndLBAHigh & 0xE0) | ((lba >> 16) & 0x1F);
			lbaMiddle = lba >> 8;
			lbaLow = lba;
		}
		inline uint32_t getLBA() {
			uint32_t lba = lbaLow;
			lba |= (uint32_t)lbaMiddle << 8;
			lba |= ((uint32_t)cdbInfoAndLBAHigh) & 0x1F << 16;
			return lba;
		}
	} LT_PACK_ATTRIBUTE;
	LT_PACK_SUFFIX

	LT_ASSERT_SIZEOF(SCSICommand6, 6)
	LT_ASSERT_OFFSETOF(SCSICommand6, op, 0)
	LT_ASSERT_OFFSETOF(SCSICommand6, data_bytes, 1)
	LT_ASSERT_OFFSETOF(SCSICommand6, cdbInfoAndLBAHigh, 1)
	LT_ASSERT_OFFSETOF(SCSICommand6, lbaMiddle, 2)
	LT_ASSERT_OFFSETOF(SCSICommand6, lbaLow, 3)
	LT_ASSERT_OFFSETOF(SCSICommand6, length, 4)
	LT_ASSERT_OFFSETOF(SCSICommand6, control, 5)

	LT_PACK_PREFIX
	struct SCSICommand10 {
		uint8_t op;
		union {
			uint8_t data_bytes[9];
			struct {
				uint8_t cdbInfoAndServiceAction;
				uint8_t lbaHigh;
				uint8_t lbaMidHigh;
				uint8_t lbaMidLow;
				uint8_t lbaLow;
				uint8_t cdbInfo;
				uint8_t lengthHigh;
				uint8_t lengthLow;
				uint8_t control;
			};
		};
	} LT_PACK_ATTRIB;
	LT_PACK_SUFFIX

	LT_ASSERT_OFFSETOF(SCSICommand10, op, 0)
	LT_ASSERT_OFFSETOF(SCSICommand10, data_bytes, 1)
	LT_ASSERT_OFFSETOF(SCSICommand10, cdbInfoAndServiceAction, 1)
	LT_ASSERT_OFFSETOF(SCSICommand10, lbaHigh, 2)
	LT_ASSERT_OFFSETOF(SCSICommand10, lbaMidHigh, 3)
	LT_ASSERT_OFFSETOF(SCSICommand10, lbaMidLow, 4)
	LT_ASSERT_OFFSETOF(SCSICommand10, lbaLow, 5)
	LT_ASSERT_OFFSETOF(SCSICommand10, cdbInfo, 6)
	LT_ASSERT_OFFSETOF(SCSICommand10, lengthHigh, 7)
	LT_ASSERT_OFFSETOF(SCSICommand10, lengthLow, 8)
	LT_ASSERT_OFFSETOF(SCSICommand10, control, 9)

	LT_PACK_PREFIX
	struct SCSICommand12 {
		uint8_t op;
		union {
			uint8_t data_bytes[11];
			struct {
				uint8_t cdbInfoAndServiceAction;
				uint8_t lbaHigh;
				uint8_t lbaMidHigh;
				uint8_t lbaMidLow;
				uint8_t lbaLow;
				uint8_t lengthHigh;
				uint8_t lengthMidHigh;
				uint8_t lengthMidLow;
				uint8_t lengthLow;
				uint8_t cdbInfo;
				uint8_t control;
			};
		};
	} LT_PACK_ATTRIB;
	LT_PACK_SUFFIX

	LT_ASSERT_OFFSETOF(SCSICommand12, op, 0)
	LT_ASSERT_OFFSETOF(SCSICommand12, data_bytes, 1)
	LT_ASSERT_OFFSETOF(SCSICommand12, cdbInfoAndServiceAction, 1)
	LT_ASSERT_OFFSETOF(SCSICommand12, lbaHigh, 2)
	LT_ASSERT_OFFSETOF(SCSICommand12, lbaMidHigh, 3)
	LT_ASSERT_OFFSETOF(SCSICommand12, lbaMidLow, 4)
	LT_ASSERT_OFFSETOF(SCSICommand12, lbaLow, 5)
	LT_ASSERT_OFFSETOF(SCSICommand12, lengthHigh, 6)
	LT_ASSERT_OFFSETOF(SCSICommand12, lengthMidHigh, 7)
	LT_ASSERT_OFFSETOF(SCSICommand12, lengthMidLow, 8)
	LT_ASSERT_OFFSETOF(SCSICommand12, lengthLow, 9)
	LT_ASSERT_OFFSETOF(SCSICommand12, cdbInfo, 10)
	LT_ASSERT_OFFSETOF(SCSICommand12, control, 11)

	LT_PACK_PREFIX
	struct SCSICommand16 {
		uint8_t op;
		union {
			uint8_t data_bytes[15];
			struct {
				uint8_t cdbInfoAndServiceAction;
				uint8_t lbaHigh;
				uint8_t lbaMidHigh;
				uint8_t lbaMidLow;
				uint8_t lbaLow;
				uint8_t cdbHigh;
				uint8_t cdbMidHigh;
				uint8_t cdbMidLow;
				uint8_t cdbLow;
				uint8_t lengthHigh;
				uint8_t lengthMidHigh;
				uint8_t lengthMidLow;
				uint8_t lengthLow;
				uint8_t cdbInfo;
				uint8_t control;
			};
		};
	} LT_PACK_ATTRIB;
	LT_PACK_SUFFIX

	LT_ASSERT_OFFSETOF(SCSICommand16, op, 0)
	LT_ASSERT_OFFSETOF(SCSICommand16, data_bytes, 1)
	LT_ASSERT_OFFSETOF(SCSICommand16, cdbInfoAndServiceAction, 1)
	LT_ASSERT_OFFSETOF(SCSICommand16, lbaHigh, 2)
	LT_ASSERT_OFFSETOF(SCSICommand16, lbaMidHigh, 3)
	LT_ASSERT_OFFSETOF(SCSICommand16, lbaMidLow, 4)
	LT_ASSERT_OFFSETOF(SCSICommand16, lbaLow, 5)
	LT_ASSERT_OFFSETOF(SCSICommand16, cdbHigh, 6)
	LT_ASSERT_OFFSETOF(SCSICommand16, cdbMidHigh, 7)
	LT_ASSERT_OFFSETOF(SCSICommand16, cdbMidLow, 8)
	LT_ASSERT_OFFSETOF(SCSICommand16, cdbLow, 9)
	LT_ASSERT_OFFSETOF(SCSICommand16, lengthHigh, 10)
	LT_ASSERT_OFFSETOF(SCSICommand16, lengthMidHigh, 11)
	LT_ASSERT_OFFSETOF(SCSICommand16, lengthMidLow, 12)
	LT_ASSERT_OFFSETOF(SCSICommand16, lengthLow, 13)
	LT_ASSERT_OFFSETOF(SCSICommand16, cdbInfo, 14)
	LT_ASSERT_OFFSETOF(SCSICommand16, control, 15)

	LT_PACK_PREFIX
	struct SCSICommand16Long {
		uint8_t op;
		union {
			uint8_t data_bytes[15];
			struct {
				uint8_t cdbInfo0;
				uint8_t lba[8];
				uint8_t lengthHigh;
				uint8_t lengthMidHigh;
				uint8_t lengthMidLow;
				uint8_t lengthLow;
				uint8_t cdbInfo1;
				uint8_t control;
			};
		};
	} LT_PACK_ATTRIB;
	LT_PACK_SUFFIX

	LT_ASSERT_OFFSETOF(SCSICommand16Long, op, 0)
	LT_ASSERT_OFFSETOF(SCSICommand16Long, data_bytes, 1)
	LT_ASSERT_OFFSETOF(SCSICommand16Long, cdbInfo0, 1)
	LT_ASSERT_OFFSETOF(SCSICommand16Long, lba, 2)
	LT_ASSERT_OFFSETOF(SCSICommand16Long, lengthHigh, 10)
	LT_ASSERT_OFFSETOF(SCSICommand16Long, lengthMidHigh, 11)
	LT_ASSERT_OFFSETOF(SCSICommand16Long, lengthMidLow, 12)
	LT_ASSERT_OFFSETOF(SCSICommand16Long, lengthLow, 13)
	LT_ASSERT_OFFSETOF(SCSICommand16Long, cdbInfo1, 14)
	LT_ASSERT_OFFSETOF(SCSICommand16Long, control, 15)

	LT_PACK_PREFIX
	struct SCSICommandVar {
		uint8_t op;
		uint8_t control;
		uint8_t cdbInfo[5];
		uint8_t lengthSub7;
		uint8_t serviceActionHigh;
		uint8_t serviceActionLow;
		uint8_t serviceActionFields[];
	} LT_PACK_ATTRIB;
	LT_PACK_SUFFIX

	LT_ASSERT_OFFSETOF(SCSICommandVar, op, 0)
	LT_ASSERT_OFFSETOF(SCSICommandVar, control, 1)
	LT_ASSERT_OFFSETOF(SCSICommandVar, cdbInfo, 2)
	LT_ASSERT_OFFSETOF(SCSICommandVar, lengthSub7, 7)
	LT_ASSERT_OFFSETOF(SCSICommandVar, serviceActionHigh, 8)
	LT_ASSERT_OFFSETOF(SCSICommandVar, serviceActionLow, 9)
	LT_ASSERT_OFFSETOF(SCSICommandVar, serviceActionFields, 10)

	LT_PACK_PREFIX
	struct SCSICommand32 {
		uint8_t op; // 0x7F
		union {
			uint8_t data_bytes[31];
			struct {
				uint8_t control;
				uint8_t reserved0[4];
				uint8_t groupNumber;
				uint8_t additionalLength;
				uint16be_t serviceAction;
				uint8_t flags;
				uint8_t reserved1;
				uint64be_t lba;
				uint32be_t initialLbaTag;
				uint16be_t lbaAppTag;
				uint16be_t lbaAppTagMask;
				uint32be_t length;
			};
		};
	} LT_PACK_ATTRIB;
	LT_PACK_SUFFIX

	LT_ASSERT_SIZEOF(SCSICommand32, 32)
	LT_ASSERT_OFFSETOF(SCSICommand32, op, 0)
	LT_ASSERT_OFFSETOF(SCSICommand32, control, 1)
	LT_ASSERT_OFFSETOF(SCSICommand32, groupNumber, 6)
	LT_ASSERT_OFFSETOF(SCSICommand32, additionalLength, 7)
	LT_ASSERT_OFFSETOF(SCSICommand32, serviceAction, 8)
	LT_ASSERT_OFFSETOF(SCSICommand32, flags, 10)
	LT_ASSERT_OFFSETOF(SCSICommand32, lba, 12)
	LT_ASSERT_OFFSETOF(SCSICommand32, initialLbaTag, 20)
	LT_ASSERT_OFFSETOF(SCSICommand32, lbaAppTag, 24)
	LT_ASSERT_OFFSETOF(SCSICommand32, lbaAppTagMask, 26)
	LT_ASSERT_OFFSETOF(SCSICommand32, length, 28)

	inline uint8_t MAKE_CONTROL(bool normalACA) {
		return normalACA ? 0x04 : 0;
	}

#include "scsi_cmds.inc"

	class SCSIChannel {
	protected:
		virtual ~SCSIChannel(){}
	public:
		virtual void send(void* cmd, void* response, ...)=0;
	};

}


#endif /* LTOS_API_BUS_SCSI_H_ */
