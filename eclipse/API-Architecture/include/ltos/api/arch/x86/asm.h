/*
 * x86asm.h
 *
 *  Created on: Oct 18, 2018
 *      Author: Zekrom_64
 */

#ifndef LTOS_API_ARCH_X86_ASM_H_
#define LTOS_API_ARCH_X86_ASM_H_

#include "ltos/lttypes.h"
#include "ltos/ltarch.h"

#ifdef LT_ARCH_FAMILY_X86

namespace ltos::x86 {


	
#ifdef LT_COMPILER_FAMILY_GNUC

// AT&T Syntax by default
// Register names
#define X86_ASM_REG(R) "%%" R

#define X86_ASM_DEC(N) "$" LT_STRINGIFY(N)
#define X86_ASM_HEX(H) "$0x" LT_STRINGIFY(H)
#define X86_ASM_OP2(S,D) S ", " D
#define X86_ASM_MEM(X) "(" X ")"
#define X86_ASM_MEMX(BASE,INDEX,SCALE,OFFSET) LT_STRINGIFY(OFFSET) "(" LT_STRINGIFY(BASE) "," LT_STRINGIFY(INDEX) "," LT_STRINGIFY(SCALE) ")"

#else

// Intel Syntax by default
// Register names
#define X86_ASM_REG(R) R

#define X86_ASM_DEC(N) LT_STRINGIFY(N)
#define X86_ASM_HEX(H) LT_STRINGIFY(H) "h"
#define X86_ASM_OP2(S,D) D ", " S
#define X86_ASM_MEM(X) "[" X "]"
#define X86_ASM_MEMX(BASE,INDEX,SCALE,OFFSET) "[" LT_STRINGIFY(BASE) "+" LT_STRINGIFY(INDEX) "*" LT_STRINGIFY(SCALE) "+" LT_STRINGIFY(OFFSET) "]"

#endif


struct X86 {

	/** Writes a byte to an IO port.
	 *
	 * @param addr IO port address
	 * @param data Data to write
	 */
	inline static void outb(register uint32_t addr, register uint8_t data) {
		asm(
			"out %%al, %%dx\n"
			:
			: "d"(addr), "a"(data)
		);
	}

	/** Reads a byte from an IO port.
	 *
	 * @param addr IO port address
	 * @return Data read from port
	 */
	inline static uint8_t inb(register uint32_t addr) {
		register uint8_t data;
		asm(
			"in %%dx, %%al"
			: "=a" (data)
			: "d"(addr)
		);
		return data;
	}

	/** Computes the parity of a value. Equal to what
	 * the x86 parity flag is set to.
	 *
	 * @param val Value to compute parity of
	 * @return Value parity
	 */
	inline static bool parity(uint8_t val) {
		register bool flag;
		asm(
			"or $0, %1\n"
			"xor %0, %0\n"
			"jpo 1f\n"
			"mov $1, %0\n"
			"1:\n"
			: "=r"(flag)
			: "r"(val)
		);
		return flag;
	}

	/** Clears the interrupt flag.
	 *
	 */
	inline static void cli() {
		asm volatile(
			"cli"
		);
	}

	/** Sets the interrupt flag.
	 *
	 */
	inline static void sti() {
		asm volatile(
			"sti"
		);
	}

	/** Swaps the upper and lower bytes in a 16-bit value.
	 *
	 * @param val
	 * @return
	 */
	inline static uint16_t xchg(register uint16_t val) {
		asm(
			"xchg %0"
			: "=r" (val)
			: "r" (val)
		);
		return val;
	}

	inline static uint32_t bswap(register uint32_t val) {
		asm(
			"bswap %0"
			: "=r" (val)
			: "r" (val)
		);
		return val;
	}

#ifdef LT_ARCH_X86_64
	inline static uint64_t bswap64(uint64_t val) {
		asm(
			"mov %%eax, %%ebx\n"
			"shr $32, %%eax\n"
			"bswap %%eax\n"
			"bswap %%ebx\n"
			"shl $32, %%rbx\n"
			"or %%eax, %%ebx"
			: "=a" (val)
			: "a" (val)
			: "b"
		);
		return val;
	}
#else
	inline static uint64_t bswap64(uint64_t val) {
		uint64_t ret = bswap(val >> 32);
		ret |= (uint64_t)bswap(val) << 32;
	}
#endif

	// Common CPUID values
#define X86_CPUID_VENDOR_AMD_OLD       "AMDisbetter!"
#define X86_CPUID_VENDOR_AMD           "AuthenticAMD"
#define X86_CPUID_VENDOR_INTEL         "GenuineIntel"
#define X86_CPUID_VENDOR_CENTAUR       "CentaurHauls"
#define X86_CPUID_VENDOR_TRANSMETA_OLD "TransmetaCPU"
#define X86_CPUID_VENDOR_TRANSMETA     "GenuineTMx86"
#define X86_CPUID_VENDOR_CYRIX         "CyrixInstead"
#define X86_CPUID_VENDOR_NEXGEN        "NexGenDriven"
#define X86_CPUID_VENDOR_HYGON         "HygonGenuine"
#define X86_CPUID_VENDOR_VORTEX        "Vortex86 SoC"
#define X86_CPUID_VENDOR_NSC           "Geode by NSC"

	// Less common CPUIDs
#define X86_CPUID_VENDOR_UMC           "UMC UMC UMC "
#define X86_CPUID_VENDOR_SIS           "Sis Sis Sis "
#define X86_CPUID_VENDOR_RISE          "RiseRiseRise"
#define X86_CPUID_VENDOR_VIA           "VIA VIA VIA "

	// VM CPUID values
#define X86_CPUID_VENDOR_VMWARE        "VMwareVMware"
#define X86_CPUID_VENDOR_XENHVM        "XenVMMXenVMM"
#define X86_CPUID_VENDOR_MICROSOFT_HV  "Microsoft Hv"
// Someone forgot about byte ordering :P
#define X86_CPUID_VENDOR_PARALLELS     " lrpepyh vr"
#define X86_CPUID_VENDOR_BHYVE         "bhyve bhyve"
#define X86_CPUID_VENDOR_KVM           "KVMKVMKVM"

	enum CPUIDRegister {
		CPUID_EAX,
		CPUID_EBX,
		CPUID_ECX,
		CPUID_EDX
	};

#define X86_CPUID_MAKEFIELD(C,R,B,S) ((C&0x80000000)|((C&0x7FFFF)<<12)|(R<<10)|(B<<5)|(S-1))

	struct CPUID {
		uint32_t eax;
		uint32_t ebx;
		uint32_t ecx;
		uint32_t edx;
		void call(uint32_t value) {
			asm(
				"cpuid"
				: "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
				: "a"(value)
				: "a","b","c","d"
			);
		}
	};

};

}

#endif

#endif /* LTOS_API_ARCH_X86_ASM_H_ */
