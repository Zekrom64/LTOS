/*
 * idt.h
 *
 *  Created on: Oct 24, 2018
 *      Author: Zekrom_64
 */

#ifndef LTOS_API_ARCH_X86_CPU_IDT_H_
#define LTOS_API_ARCH_X86_CPU_IDT_H_


#include "ltos/lttypes.h"
#include "ltos/ltmacros.h"

namespace ltos::x86 {

	LT_PACK_PREFIX
	struct IDT {
		uint16_t limit;
		uint32_t base;
	} LT_PACK_ATTRIB;
	LT_PACK_SUFFIX

	LT_ASSERT_SIZEOF(IDT, 12)
	LT_ASSERT_OFFSETOF(IDT, limit, 0)
	LT_ASSERT_OFFSETOF(IDT, base, 2)

	LT_PACK_PREFIX
	struct IDTEntry {
		uint16_t offsetLow;
		uint16_t selector;
		uint8_t reserved0;
		uint8_t flags;
		uint16_t offsetHigh;

		inline void setOffset(uint32_t offset) {
			offsetLow = (uint16_t)offset;
			offsetHigh = (uint16_t)(offset >> 16);
		}
		inline uint32_t getOffset() {
			uint32_t offset = offsetLow;
			offset = (uint32_t)offsetHigh << 16;
			return offset;
		}

		enum Flags {
			FLAGS_PRESENT =         0b10000000,
			FLAGS_PRIVILIGE_LEVEL = 0b01100000,
			FLAGS_STORAGE_SEG =     0b00010000,
			FLAGS_MASK_TYPE =       0b00001111
		};
		enum Type {
			TYPE_32BIT_TASK = 5,
			TYPE_16BIT_INTERRUPT = 6,
			TYPE_16BIT_TRAP = 7,
			TYPE_32BIT_INTERRUPT = 14,
			TYPE_32BIT_TRAP = 15
		};
	} LT_PACK_ATTRIB;
	LT_PACK_SUFFIX

	LT_ASSERT_SIZEOF(IDTEntry, 8)
	LT_ASSERT_OFFSETOF(IDTEntry, offsetLow, 0)
	LT_ASSERT_OFFSETOF(IDTEntry, selector, 2)
	LT_ASSERT_OFFSETOF(IDTEntry, reserved0, 4)
	LT_ASSERT_OFFSETOF(IDTEntry, flags, 5)
	LT_ASSERT_OFFSETOF(IDTEntry, offsetHigh, 6)

	LT_PACK_PREFIX
	struct IDTEntry64 {
		uint16_t offsetLow;
		uint16_t selector;
		uint8_t istOffset;
		uint8_t flags;
		uint16_t offsetMid;
		uint32_t offsetHigh;
		uint32_t reserved;
	} LT_PACK_ATTRIB;
	LT_PACK_SUFFIX

	LT_ASSERT_SIZEOF(IDTEntry64, 16)
	LT_ASSERT_OFFSETOF(IDTEntry64, offsetLow, 0)
	LT_ASSERT_OFFSETOF(IDTEntry64, selector, 2)
	LT_ASSERT_OFFSETOF(IDTEntry64, istOffset, 4)
	LT_ASSERT_OFFSETOF(IDTEntry64, flags, 5)
	LT_ASSERT_OFFSETOF(IDTEntry64, offsetMid, 6)
	LT_ASSERT_OFFSETOF(IDTEntry64, offsetHigh, 8)
	LT_ASSERT_OFFSETOF(IDTEntry64, reserved, 12)

#define X86_DECLARE_ISR(NAME,BODY) \
	void __X86__ISR_##NAME() BODY \
	LT_NAKED_ATTRIB void NAME() { \
		asm volatile( \
			"cli\n" \
			"call __X86__ISR_" LT_STRINGIFY(NAME) "\n" \
			"sti\n" \
			"iret" \
		); \
	}

}


#endif /* LTOS_API_ARCH_X86_CPU_IDT_H_ */
