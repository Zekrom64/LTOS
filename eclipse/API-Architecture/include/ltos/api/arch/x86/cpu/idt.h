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
#include "ltos/ltarch.h"

#ifdef LT_ARCH_FAMILY_X86

namespace ltos::x86 {

	enum ExceptionCode {
		EXCEPTION__DIVIDE_BY_0 =          0x00,
		EXCEPTION__DEBUG =                0x01,
		EXCEPTION__NMI =                  0x02,
		EXCEPTION__BREAKPOINT =           0x03,
		EXCEPTION__OVERFLOW =             0x04,
		EXCEPTION__BOUND_RANGE_EXCEEDED = 0x05,
		EXCEPTION__INVALID_OPCODE =       0x06,
		EXCEPTION__DEVICE_NOT_AVAILABLE = 0x07,
		EXCEPTION__DOUBLE_FAULT =         0x08,
		EXCEPTION__INVALID_TSS =          0x0A,
		EXCEPTION__SEGMENT_NOT_PRESENT =  0x0B,
		EXCEPTION__STACK_SEGMENT_FAULT =  0x0C,
		EXCEPTION__GENERAL_PROT_FAULT =   0x0D,
		EXCEPTION__PAGE_FAULT =           0x0E,
		EXCEPTION__X87_FP_EXCEPTION =     0x10,
		EXCEPTION__ALIGNMENT_CHECK =      0x11,
		EXCEPTION__MACHINE_CHECK =        0x12,
		EXCEPTION__SIMD_FP_EXCEPTION =    0x13,
		EXCEPTION__VIRTUALIZE_EXCEPTION = 0x14,
		EXCEPTION__SECURITY_EXCEPTION =   0x30
	};

	template<unsigned int EX>
	struct __X86_EXCEPTION_HAS_CODE__ {
		static bool hasErrorCode() { return false; }
	};

#define __X86_DECLARE_EXCEPTION_HAS_CODE__(N) \
	template<> \
	struct __X86_EXCEPTION_HAS_CODE__##N { \
		static bool hasErrorCode() { return true; } \
	}; \

	__X86_DECLARE_EXCEPTION_HAS_CODE__(0x08)
	__X86_DECLARE_EXCEPTION_HAS_CODE__(0x0A)
	__X86_DECLARE_EXCEPTION_HAS_CODE__(0x0B)
	__X86_DECLARE_EXCEPTION_HAS_CODE__(0x0C)
	__X86_DECLARE_EXCEPTION_HAS_CODE__(0x0D)
	__X86_DECLARE_EXCEPTION_HAS_CODE__(0x0E)
	__X86_DECLARE_EXCEPTION_HAS_CODE__(0x11)
	__X86_DECLARE_EXCEPTION_HAS_CODE__(0x30)


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

#ifdef LT_ARCH_X86_64

	template<bool HAS_EXCEPT>
	struct ISRFrame{};

	template<>
	struct ISRFrame<false> {
		uint64_t rax;
		uint64_t rcx;
		uint64_t rdx;
		uint64_t r8;
		uint64_t r9;
		uint64_t r10;
		uint64_t r11;
		uint64_t rip;
		uint64_t cs;
		uint64_t rflags;
		uint64_t rsp;
		uint64_t ss;
	};

	template<>
	struct ISRFrame<true> {
		uint64_t rax;
		uint64_t rcx;
		uint64_t rdx;
		uint64_t r8;
		uint64_t r9;
		uint64_t r10;
		uint64_t r11;
		uint64_t error;
		uint64_t rip;
		uint64_t cs;
		uint64_t rflags;
		uint64_t rsp;
		uint64_t ss;
	};

#define X86_DECLARE_ISR(NAME,NUM,BODY) \
	void __X86__ISR_##NAME(ISRFrame<__X86_EXCEPTION_HAS_CODE__<NUM>::hasErrorCode()>* frame) BODY \
	LT_NAKED_ATTRIB void NAME() { \
		asm volatile( \
			"cli\n" \
			"push " X86_ASM_REG("r11") "\n" \
			"push " X86_ASM_REG("r10") "\n" \
			"push " X86_ASM_REG("r9") "\n" \
			"push " X86_ASM_REG("r8") "\n" \
			"push " X86_ASM_REG("rdx") "\n" \
			"push " X86_ASM_REG("rcx") "\n" \
			"push " X86_ASM_REG("rax") "\n" \
			"mov " X86_ASM_OP2(X86_ASM_REG("rsp"),X86_ASM_REG("rcx")) "\n" \
			"sub " X86_ASM_OP2(X86_ASM_DEC(32),X86_ASM_REG("rsp")) "\n" \
			"call __X86__ISR_" LT_STRINGIFY(NAME) "\n" \
			"add " X86_ASM_OP2(X86_ASM_DEC(32),X86_ASM_REG("rsp")) "\n" \
			"pop " X86_ASM_REG("rax") "\n" \
			"pop " X86_ASM_REG("rcx") "\n" \
			"pop " X86_ASM_REG("rdx") "\n" \
			"pop " X86_ASM_REG("r8") "\n" \
			"pop " X86_ASM_REG("r9") "\n" \
			"pop " X86_ASM_REG("r10") "\n" \
			"pop " X86_ASM_REG("r11") "\n" \
			"sti\n" \
			"iret" \
		); \
	}


#else

	struct ISRFrame {
		uint32_t eip;
		uint32_t cs;
		uint32_t eflags;
	};

#endif

}

#endif

#endif /* LTOS_API_ARCH_X86_CPU_IDT_H_ */
