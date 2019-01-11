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
#include "ltos/ltcpp.h"

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

			"out " X86_ASM_OP2(X86_ASM_REG("al"), X86_ASM_REG("dx"))
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
			"in " X86_ASM_OP2(X86_ASM_REG("dx"), X86_ASM_REG("al"))
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
			"or " X86_ASM_OP2(X86_ASM_DEC(0), "%1") "\n"
			"xor %0, %0\n"
			"jpo 1f\n"
			"mov " X86_ASM_OP2(X86_ASM_DEC(1), "%0") "\n"
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
			"mov " X86_ASM_OP2(X86_ASM_REG("eax"), X86_ASM_REG("ebx")) "\n"
			"shr " X86_ASM_OP2(X86_ASM_DEC(32), X86_ASM_REG("rax")) "\n"
			"bswap " X86_ASM_REG("eax") "\n"
			"bswap " X86_ASM_REG("ebx") "\n"
			"shl " X86_ASM_OP2(X86_ASM_DEC(32), X86_ASM_REG("rbx")) "\n"
			"or " X86_ASM_OP2(X86_ASM_REG("rax"), X86_ASM_REG("rbx"))
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

	enum MSR {
		// Timestamp counter
		MSR__TSC = 0x00000010,
		MSR__TSC_ADJUST = 0x0000003B,
		MSR__TSC_AUX = 0xC0000103,
		MSR__TSC_DEADLINE = 0x000006E0,
		MSR__TSC_MPERF = 0x000000E7,
		MSR__TSC_APERF = 0x000000E8,

		// Feature control
		MSR__TR12 = 0x0000000E,
		MSR__MISC_CTL = 0x00000119,
		MSR__MISC_ENABLE = 0x000001A0,
		MSR__MISC_FEAT_CTL = 0x000001A4,
		MSR__EFER = 0xC0000080,
		MSR__PLATFORM_INFO = 0x000000CE,
		MSR__MISC_FEATURES = 0x00000140,
		MSR__SPEC_CTRL = 0x00000048,
		MSR__PRED_CMD = 0x00000049,
		MSR__ARCH_CAPABILITIES = 0x0000010A,

		// SYSENTER and SYSEXIT
		MSR__SEP_SEL = 0x00000174,
		MSR__SEP_ESP = 0x00000175,
		MSR__SEP_RSP = 0x00000175,
		MSR__SEP_EIP = 0x00000176,
		MSR__SEP_RIP = 0x00000176,

		// SYSCALL and SYSRET
		MSR__STAR = 0xC0000081,
		MSR__LSTAR = 0xC0000082,
		MSR__CSTAR = 0xC0000083,
		MSR__FMASK = 0xC0000084,

		// FS and GS base
		MSR__FS_BAS = 0xC0000100,
		MSR__GS_BAS = 0xC0000101,
		MSR__KERNEL_GS_BAS = 0xC0000102,

		// Page attribute table
		MSR__PAT = 0x00000277,

		// Memory type range registers
		MSR__MTRR_CAP = 0x000000FE,
		MSR__MTRR_DEF_TYPE = 0x000002FF,

		// Fixed range MTRRs
		MSR__MTRR_FIX_64K_00000 = 0x00000250,
		MSR__MTRR_FIX_16K_80000 = 0x00000258,
		MSR__MTRR_FIX_16K_A0000 = 0x00000259,
		MSR__MTRR_FIX_4K_C0000 = 0x00000268,
		MSR__MTRR_FIX_4K_C8000 = 0x00000269,
		MSR__MTRR_FIX_4K_D0000 = 0x0000026A,
		MSR__MTRR_FIX_4K_D8000 = 0x0000026B,
		MSR__MTRR_FIX_4K_E0000 = 0x0000026C,
		MSR__MTRR_FIX_4K_E8000 = 0x0000026D,
		MSR__MTRR_FIX_4K_F0000 = 0x0000026E,
		MSR__MTRR_FIX_4K_F8000 = 0x0000026F,

		// SMRRs
		MSR__SMRR_PHYS_BASE = 0x000001F2,
		MSR__SMRR_PHYS_MASK = 0x000001F3,

		// Machine check exception
		MSR__MCAR = 0x00000000,
		MSR__MCTR = 0x00000001,

		// Machine check architecture
		MSR__MCG_CONTAIN = 0x00000178,
		MSR__MCG_CAP = 0x00000179,
		MSR__MCG_STATUS = 0x0000017A,
		MSR__MCG_CTL = 0x0000017B,
		MSR__MCG_EXT_CTL = 0x000004D0,

		// MCA Extended State Registers
		MSR__MCG_RAX = 0x00000180,
		MSR__MCG_RBX = 0x00000181,
		MSR__MCG_RCX = 0x00000182,
		MSR__MCG_RDX = 0x00000183,
		MSR__MCG_RSI = 0x00000184,
		MSR__MCG_RDI = 0x00000185,
		MSR__MCG_RBP = 0x00000186,
		MSR__MCG_RSP = 0x00000187,
		MSR__MCG_RFLAGS = 0x00000188,
		MSR__MCG_RIP = 0x00000189,
		MSR__MCG_MISC = 0x0000018A,
		MSR__MCG_R8 = 0x00000190,
		MSR__MCG_R9 = 0x00000191,
		MSR__MCG_R10 = 0x00000192,
		MSR__MCG_R11 = 0x00000193,
		MSR__MCG_R12 = 0x00000194,
		MSR__MCG_R13 = 0x00000195,
		MSR__MCG_R14 = 0x00000196,
		MSR__MCG_R15 = 0x00000197,

		// Local APIC
		MSR__APIC_BASE = 0x0000001B,

		// BNDCFGS
		MSR__BNDCFGS = 0x00000D90,

		// XSS
		MSR__XSS = 0x00000DA0,
	};

	// Variable range MTRRs
	inline static uint32_t MSR__MTRR_PHYS_BASE(uint32_t n) {
		return 0x00000200 + (n << 1);
	}

	inline static uint32_t MSR__MTRR_PHYS_MASK(uint32_t n) {
		return 0x00000201 + (n << 1);
	}

	// MCA Error reporting registers
	inline static uint32_t MSR__MC_CTL2(uint32_t n) {
		return 0x00000280 + n;
	}

	inline static uint32_t MSR__MC_CTL(uint32_t n) {
		return 0x00000400 + (n << 2);
	}

	inline static uint32_t MSR__MC_STATUS(uint32_t n) {
		return 0x00000401 + (n << 2);
	}

	inline static uint32_t MSR__MC_ADDR(uint32_t n) {
		return 0x00000402 + (n << 2);
	}

	inline static uint32_t MSR__MC_MISC(uint32_t n) {
		return 0x00000403 + (n << 2);
	}
	// MCA Extended State Registers

	inline static uint32_t MSR__MCG_RES(uint32_t n) {
		return 0x0000018B + n;
	}

	inline static uint64_t rdmsr(uint32_t msr) {
		union {
			struct {
				uint32_t vallo;
				uint32_t valhi;
			};
			uint64_t val;
		} valout;
		asm(
			"rdmsr"
			: "=a" (valout.vallo), "=d" (valout.valhi)
			: "c" (msr)
		);
		return valout.val;
	}

	inline static void wrmsr(uint32_t msr, uint64_t val) {
		asm(
			"wrmsr"
			:
			: "a" (val & 0xFFFFFFFF), "d" (val >> 32), "c"(msr)
		);
	}

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

	// Less common (and less creative) CPUIDs
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
