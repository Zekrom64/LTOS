/*
 * gdt.h
 *
 *  Created on: Oct 24, 2018
 *      Author: Zekrom_64
 */

#ifndef LTOS_API_ARCH_X86_CPU_GDT_H_
#define LTOS_API_ARCH_X86_CPU_GDT_H_


#include "ltos/lttypes.h"
#include "ltos/ltmacros.h"

namespace ltos::x86 {

	struct GDT {
		uint16_t sizeSub1;
		uint32_t offset;
		inline void load() {
			asm(
				"lgdt %0"
				: "g"(this)
			);
		}
	};

	LT_ASSERT_SIZEOF(GDT, 6)
	LT_ASSERT_OFFSETOF(GDT, sizeSub1, 0)
	LT_ASSERT_OFFSETOF(GDT, offset, 2)

	struct GDTEntry {
		uint16_t limitLow;
		uint16_t baseLow;
		uint8_t baseMid;
		uint8_t access;
		uint8_t flagsAndLimitHigh;
		uint8_t baseHigh;

		inline void setLimit(uint32_t limit) {
			limitLow = (uint16_t)limit;
			flagsAndLimitHigh = (flagsAndLimitHigh & 0xF0) | ((limit >> 16) & 0xF);
		}
		inline uint32_t getLimit() {
			uint32_t limit = limitLow;
			limit |= (uint32_t)(flagsAndLimitHigh & 0xF) << 16;
			return limit;
		}
		inline void setBase(uint32_t base) {
			baseLow = (uint16_t)base;
			baseMid = (uint8_t)(base >> 16);
			baseHigh = (uint8_t)(base >> 24);
		}
		inline uint32_t getBase() {
			uint32_t base = baseLow;
			base |= (uint32_t)baseMid << 16;
			base |= (uint32_t)baseHigh << 24;
			return base;
		}

		enum Access {
			ACC_ACCESSED =         0b00000001,
			ACC_EDITABLE =         0b00000010,
			ACC_DOWNWARD =         0b00000100,
			ACC_LOWER_EXECUTABLE = 0b00000100,
			ACC_EXECUTABLE =       0b00001000,
			ACC_MASK_RING_LEVEL =  0b01100000,
			ACC_PRESENT =          0b10000000
		};
		enum Flags {
			FLAG_GRANULARITY =   0b10000000,
			FLAG_MASK_SIZEMODE = 0b01100000
		};
		enum Granularity {
			GRANULARITY_BYTE = 0,
			GRANULARITY_4K_PAGE = 1
		};
		enum SizeMode {
			SIZEMODE_16BIT_PMODE = 0b00,
			SIZEMODE_32BIT_PMODE = 0b10
		};
	};

	LT_ASSERT_SIZEOF(GDTEntry, 8)
	LT_ASSERT_OFFSETOF(GDTEntry, limitLow, 0)
	LT_ASSERT_OFFSETOF(GDTEntry, baseLow, 2)
	LT_ASSERT_OFFSETOF(GDTEntry, baseMid, 4)
	LT_ASSERT_OFFSETOF(GDTEntry, access, 5)
	LT_ASSERT_OFFSETOF(GDTEntry, flagsAndLimitHigh, 6)
	LT_ASSERT_OFFSETOF(GDTEntry, baseHigh, 7)

}


#endif /* LTOS_API_ARCH_X86_CPU_GDT_H_ */
