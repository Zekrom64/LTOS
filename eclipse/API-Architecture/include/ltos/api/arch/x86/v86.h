/*
 * v86.h
 *
 *  Created on: Oct 18, 2018
 *      Author: Zekrom_64
 */

#ifndef LTOS_API_ARCH_X86_V86_H_
#define LTOS_API_ARCH_X86_V86_H_

#include "ltos/lttypes.h"
#include "ltos/ltarch.h"

#ifdef LT_ARCH_FAMILY_X86

namespace ltos::x86 {

	/** Virtual 8086 CPU emulator. This may seem out of place since this is running
	 * on an x86 machine, but this is primarily for using BIOS calls effectively.
	 * BIOS calls only work reliably in Real Mode, and x86 makes switching to a
	 * compatible mode so convoluted that the most efficient way is to simply
	 * emulate a 16-bit 8086 CPU to run the BIOS code.
	 *
	 * Note: This is still an emulator, it is slower than running on actual hardware.
	 * BIOS calls should be made as little as possible, although they are sometimes
	 * unavoidable.
	 */
	class Virtual8086 {
	public:
		/** The registers of the 8086 CPU. */
		struct {
			// General purpose registers
			union {
				uint16_t ax;
				struct {
					uint8_t al;
					uint8_t ah;
				};
			};
			union {
				uint16_t bx;
				struct {
					uint8_t bl;
					uint8_t bh;
				};
			};
			union {
				uint16_t cx;
				struct {
					uint8_t cl;
					uint8_t ch;
				};
			};
			union {
				uint16_t dx;
				struct {
					uint8_t dl;
					uint8_t dh;
				};
			};
			// Index registers
			uint16_t si;
			uint16_t di;
			uint16_t bp;
			uint16_t sp;
			// Instruction pointer
			uint16_t ip;
			// Segment registers
			uint16_t cs;
			uint16_t ds;
			uint16_t es;
			uint16_t ss;
		} registers;
		/** The flags of the 8086 CPU. */
		struct {
			bool carry : 1;
			bool parity : 1;
			bool auxCarry : 1;
			bool zero : 1;
			bool sign : 1;
			bool trap : 1;
			bool interrupt : 1;
			bool direction : 1;
			bool overflow : 1;
		} flags;
		/** Access functions for the CPU. */
		struct {
			uint8_t (*memRead)(uint32_t address);
			void (*memWrite)(uint32_t address, uint8_t data);

			uint8_t (*ioRead)(uint16_t port);
			void (*ioWrite)(uint16_t port, uint8_t data);
		} access;
		/** Miscellaneous CPU state for instruction decoding. */
		enum SegmentOverride {
			SEGOVR_NONE = 0,
			SEGOVR_CS,
			SEGOVR_DS,
			SEGOVR_ES,
			SEGOVR_SS,
		};
		enum RepInsn {
			REP_NONE = 0,
			REP_Z,
			REP_NZ
		};
		struct {
			bool halted;
			SegmentOverride segOverride;
			RepInsn rep;
			bool lock;
		} state;

		void step();
		void executeInterrupt(uint8_t intNum);
	};

}

#endif

#endif /* LTOS_API_ARCH_X86_V86_H_ */
