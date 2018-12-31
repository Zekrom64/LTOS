/*
 * v86.cpp
 *
 *  Created on: Oct 18, 2018
 *      Author: Zekrom_64
 */

#include "ltos/api/arch/x86/v86.h"
#include "ltos/api/arch/x86/asm.h"

using namespace ltos::x86;

#include "v86_funcs.inc"

void v86Grp1_8(Virtual8086& vm, uint8_t modrm, uint16_t displace, uint8_t b) {
	uint8_t a = v86ReadRM8(vm, modrm, displace);
	switch((modrm >> 3) & 0x7) {
	case 0: a = v86Add8(vm, a, b); break;
	case 1: a = v86Or8(vm, a, b); break;
	case 2: a = v86Adc8(vm, a, b); break;
	case 3: a = v86Sbb8(vm, a, b); break;
	case 4: a = v86And8(vm, a, b); break;
	case 5: a = v86Sub8(vm, a, b); break;
	case 6: a = v86Xor8(vm, a, b); break;
	case 7: v86Cmp8(vm, a, b); return;
	}
	v86WriteRM8(vm, modrm, displace, a);
}

void v86Grp1_16(Virtual8086& vm, uint8_t modrm, uint16_t displace, uint16_t b) {
	uint16_t a = v86ReadRM16(vm, modrm, displace);
	switch((modrm >> 3) & 0x7) {
	case 0: a = v86Add16(vm, a, b); break;
	case 1: a = v86Or16(vm, a, b); break;
	case 2: a = v86Adc16(vm, a, b); break;
	case 3: a = v86Sbb16(vm, a, b); break;
	case 4: a = v86And16(vm, a, b); break;
	case 5: a = v86Sub16(vm, a, b); break;
	case 6: a = v86Xor16(vm, a, b); break;
	case 7: v86Cmp16(vm, a, b); return;
	}
	v86WriteRM16(vm, modrm, displace, a);
}

void v86Grp2_8(Virtual8086& vm, uint8_t modrm, uint16_t displace, uint8_t b) {
	uint8_t op = (modrm >> 3) & 0x7;
	if (op == 6) return; // GRP2:6 not implemented
	uint8_t a = v86ReadRM8(vm, modrm, displace);
	switch(op) {
	case 0: a = v86Rol8(vm, a, b); break;
	case 1: a = v86Ror8(vm, a, b); break;
	case 2: a = v86Rcl8(vm, a, b); break;
	case 3: a = v86Rcr8(vm, a, b); break;
	case 4: a = v86Shl8(vm, a, b); break;
	case 5: a = v86Shr8(vm, a, b); break;
	case 7: v86Sar8(vm, a, b); return;
	}
	v86WriteRM8(vm, modrm, displace, a);
}

void Virtual8086::step() {
	if (state.halted) return;

	bool doJump = false;
	int16_t initIP = (int16_t)registers.ip;
	uint8_t opcode = access.memRead(v86Segment(registers.cs, registers.ip++));

	bool setRep = false;
	bool setSeg = false;

	switch(opcode) {
	case 0x00: { V86_MODRM V86_WRITE_RM8(V86_ADD8(V86_READ_RM8, V86_READ_R8)) } break;     // ADD r/m8, r8
	case 0x01: { V86_MODRM V86_WRITE_RM16(V86_ADD16(V86_READ_RM16, V86_READ_R16)) } break; // ADD r/m16, r16
	case 0x02: { V86_MODRM V86_WRITE_R8(V86_ADD8(V86_READ_R8, V86_READ_RM8)) } break;      // ADD r8, r/m8
	case 0x03: { V86_MODRM V86_WRITE_R16(V86_ADD16(V86_READ_R16, V86_READ_RM16)) } break;  // ADD r16, r/m16
	case 0x04: { V86_IMM8  registers.al = V86_ADD8(registers.al, V86_IMM); } break;        // ADD al, imm8
	case 0x05: { V86_IMM16 registers.ax = V86_ADD16(registers.ax, V86_IMM); } break;       // ADD ax, imm16
	case 0x06: { V86_PUSH(registers.es) } break;                                           // PUSH es
	case 0x07: { registers.es = V86_POP; } break;                                          // POP es
	case 0x08: { V86_MODRM V86_WRITE_RM8(V86_OR8(V86_READ_RM8, V86_READ_R8)) } break;      // OR r/m8, r8
	case 0x09: { V86_MODRM V86_WRITE_RM16(V86_OR16(V86_READ_RM16, V86_READ_R16)) } break;  // OR r/m16, r16
	case 0x0A: { V86_MODRM V86_WRITE_R8(V86_OR8(V86_READ_R8, V86_READ_RM8)) } break;       // OR r8, r/m8
	case 0x0B: { V86_MODRM V86_WRITE_R16(V86_OR16(V86_READ_R16, V86_READ_RM16)) } break;   // OR r16, r/m16
	case 0x0C: { V86_IMM8  registers.al = V86_OR8(registers.al, V86_IMM); } break;         // OR al, imm8
	case 0x0D: { V86_IMM16 registers.ax = V86_OR16(registers.ax, V86_IMM); } break;        // OR ax, imm16
	case 0x0E: { V86_PUSH(registers.cs) } break;                                           // PUSH cs
	// 0x0F is unused

	case 0x10: { V86_MODRM V86_WRITE_RM8(V86_ADC8(V86_READ_RM8, V86_READ_R8)) } break;     // ADC r/m8, r8
	case 0x11: { V86_MODRM V86_WRITE_RM16(V86_ADC16(V86_READ_RM16, V86_READ_R16)) } break; // ADC r/m16, r16
	case 0x12: { V86_MODRM V86_WRITE_R8(V86_ADC8(V86_READ_R8, V86_READ_RM8)) } break;      // ADC r8, r/m8
	case 0x13: { V86_MODRM V86_WRITE_R16(V86_ADC16(V86_READ_R16, V86_READ_RM16)) } break;  // ADC r16, r/m16
	case 0x14: { V86_IMM8  registers.al = V86_ADC8(registers.al, V86_IMM); } break;        // ADC al, imm8
	case 0x15: { V86_IMM16 registers.ax = V86_ADC16(registers.ax, V86_IMM); } break;       // ADC ax, imm16
	case 0x16: { V86_PUSH(registers.ss) } break;                                           // PUSH ss
	case 0x17: { registers.ss = V86_POP; } break;                                          // POP ss
	case 0x18: { V86_MODRM V86_WRITE_RM8(V86_SBB8(V86_READ_RM8, V86_READ_R8)) } break;     // SBB r/m8, r8
	case 0x19: { V86_MODRM V86_WRITE_RM16(V86_SBB16(V86_READ_RM16, V86_READ_R16)) } break; // SBB r/m16, r16
	case 0x1A: { V86_MODRM V86_WRITE_R8(V86_SBB8(V86_READ_R8, V86_READ_RM8)) } break;      // SBB r8, r/m8
	case 0x1B: { V86_MODRM V86_WRITE_R16(V86_SBB16(V86_READ_R16, V86_READ_RM16)) } break;  // SBB r16, r/m16
	case 0x1C: { V86_IMM8  registers.al = V86_SBB8(registers.al, V86_IMM); } break;        // SBB al, imm8
	case 0x1D: { V86_IMM16 registers.ax = V86_SBB16(registers.ax, V86_IMM); } break;       // SBB ax, imm16
	case 0x1E: { V86_PUSH(registers.ds) } break;                                           // PUSH ds
	case 0x1F: { registers.ds = V86_POP; } break;                                          // POP ds

	case 0x20: { V86_MODRM V86_WRITE_RM8(V86_AND8(V86_READ_RM8, V86_READ_R8)) } break;     // AND r/m8, r8
	case 0x21: { V86_MODRM V86_WRITE_RM16(V86_AND16(V86_READ_RM16, V86_READ_R16)) } break; // AND r/m16, r16
	case 0x22: { V86_MODRM V86_WRITE_R8(V86_AND8(V86_READ_R8, V86_READ_RM8)) } break;      // AND r8, r/m8
	case 0x23: { V86_MODRM V86_WRITE_R16(V86_AND16(V86_READ_R16, V86_READ_RM16)) } break;  // AND r16, r/m16
	case 0x24: { V86_IMM8  registers.al = V86_AND8(registers.al, V86_IMM); } break;        // AND al, imm8
	case 0x25: { V86_IMM16 registers.ax = V86_AND16(registers.ax, V86_IMM); } break;       // AND ax, imm16
	case 0x26: { state.segOverride = Virtual8086::SEGOVR_ES; setSeg = true; } break;       // ES:
	case 0x27: {                                                                           // DAA
		uint8_t oldAL = registers.al;
		bool oldCF = flags.carry;
		if ((registers.al & 0xF) > 9 || flags.auxCarry) {
			uint16_t tmp = (uint16_t)registers.al + 6;
			flags.carry = flags.carry | (tmp & 0x100);
			registers.al = (uint8_t)tmp;
			flags.auxCarry = true;
		} else flags.auxCarry = false;
		if (oldAL > 0x99 || oldCF) {
			registers.al += 0x60;
			flags.carry = true;
		} else flags.carry = false;
	} break;
	case 0x28: { V86_MODRM V86_WRITE_RM8(V86_SUB8(V86_READ_RM8, V86_READ_R8)) } break;     // SUB r/m8, r8
	case 0x29: { V86_MODRM V86_WRITE_RM16(V86_SUB16(V86_READ_RM16, V86_READ_R16)) } break; // SUB r/m16, r16
	case 0x2A: { V86_MODRM V86_WRITE_R8(V86_SUB8(V86_READ_R8, V86_READ_RM8)) } break;      // SUB r8, r/m8
	case 0x2B: { V86_MODRM V86_WRITE_R16(V86_SUB16(V86_READ_R16, V86_READ_RM16)) } break;  // SUB r16, r/m16
	case 0x2C: { V86_IMM8  registers.al = V86_SUB8(registers.al, V86_IMM); } break;        // SUB al, imm8
	case 0x2D: { V86_IMM16 registers.ax = V86_SUB16(registers.ax, V86_IMM); } break;       // SUB ax, imm16
	case 0x2E: { state.segOverride = Virtual8086::SEGOVR_CS; setSeg = true; } break;       // CS:
	case 0x2F: {                                                                           // DAS
		uint8_t oldAL = registers.al;
		bool oldCF = flags.carry;
		flags.carry = false;
		if ((registers.al & 0xF) > 9 || flags.auxCarry) {
			uint16_t tmp = (uint16_t)registers.al - 6;
			flags.carry = oldCF | !(tmp & 0x100);
			flags.auxCarry = true;
			registers.al = (uint8_t)tmp;
		} else flags.auxCarry = false;
		if (oldAL > 0x99 || oldCF) {
			registers.al -= 0x60;
			flags.carry = true;
		} else flags.carry = false;
	} break;

	case 0x30: { V86_MODRM V86_WRITE_RM8(V86_XOR8(V86_READ_RM8, V86_READ_R8)) } break;     // XOR r/m, r8
	case 0x31: { V86_MODRM V86_WRITE_RM16(V86_XOR16(V86_READ_RM16, V86_READ_R16)) } break; // XOR r/m16, r16
	case 0x32: { V86_MODRM V86_WRITE_R8(V86_XOR8(V86_READ_R8, V86_READ_RM8)) } break;      // XOR r8, r/m8
	case 0x33: { V86_MODRM V86_WRITE_R16(V86_XOR16(V86_READ_R16, V86_READ_RM16)) } break;  // XOR r16, r/m16
	case 0x34: { V86_IMM8  registers.al = V86_XOR8(registers.al, V86_IMM); } break;        // XOR al, imm8
	case 0x35: { V86_IMM16 registers.ax = V86_XOR16(registers.ax, V86_IMM); } break;       // XOR ax, imm16
	case 0x36: { state.segOverride = Virtual8086::SEGOVR_SS; setSeg = true; } break;       // SS:
	case 0x37: {                                                                           // AAA
		if ((registers.al & 0xF) > 9 || flags.auxCarry) {
			registers.al += 6;
			registers.ah += 1;
			flags.auxCarry = true;
			flags.carry = true;
		} else {
			flags.auxCarry = false;
			flags.carry = false;
		}
		registers.al &= 0xF;
	} break;
	case 0x38: { V86_MODRM V86_CMP8(V86_READ_RM8, V86_READ_R8); } break;                   // CMP r/m, r8
	case 0x39: { V86_MODRM V86_CMP16(V86_READ_RM16, V86_READ_R16); } break;                // CMP r/m16, r16
	case 0x3A: { V86_MODRM V86_CMP8(V86_READ_R8, V86_READ_RM8); } break;                   // CMP r8, r/m8
	case 0x3B: { V86_MODRM V86_CMP16(V86_READ_R16, V86_READ_RM16); } break;                // CMP r16, r/m16
	case 0x3C: { V86_IMM8  V86_CMP8(registers.al, V86_IMM); } break;                       // CMP al, imm8
	case 0x3D: { V86_IMM16 V86_CMP16(registers.ax, V86_IMM); } break;                      // CMP ax, imm16
	case 0x3E: { state.segOverride = Virtual8086::SEGOVR_DS; setSeg = true; } break;       // DS:
	case 0x3F: {                                                                           // AAS
		if ((registers.al & 0xF) > 9 || flags.auxCarry) {
			registers.al -= 6;
			registers.ah -= 1;
			flags.auxCarry = true;
			flags.carry = true;
		} else {
			flags.auxCarry = false;
			flags.carry = false;
		}
		registers.al &= 0xF;
	} break;

	case 0x40: { V86_INCR16(registers.ax) } break;                                         // INC ax
	case 0x41: { V86_INCR16(registers.cx) } break;                                         // INC cx
	case 0x42: { V86_INCR16(registers.dx) } break;                                         // INC dx
	case 0x43: { V86_INCR16(registers.bx) } break;                                         // INC bx
	case 0x44: { V86_INCR16(registers.sp) } break;                                         // INC sp
	case 0x45: { V86_INCR16(registers.bp) } break;                                         // INC bp
	case 0x46: { V86_INCR16(registers.si) } break;                                         // INC si
	case 0x47: { V86_INCR16(registers.di) } break;                                         // INC di
	case 0x48: { V86_DECR16(registers.ax) } break;                                         // DEC ax
	case 0x49: { V86_DECR16(registers.cx) } break;                                         // DEC cx
	case 0x4A: { V86_DECR16(registers.dx) } break;                                         // DEC dx
	case 0x4B: { V86_DECR16(registers.bx) } break;                                         // DEC bx
	case 0x4C: { V86_DECR16(registers.sp) } break;                                         // DEC sp
	case 0x4D: { V86_DECR16(registers.bp) } break;                                         // DEC bp
	case 0x4E: { V86_DECR16(registers.si) } break;                                         // DEC si
	case 0x4F: { V86_DECR16(registers.di) } break;                                         // DEC di

	case 0x50: { V86_PUSH(registers.ax) } break;                                           // PUSH ax
	case 0x51: { V86_PUSH(registers.cx) } break;                                           // PUSH cx
	case 0x52: { V86_PUSH(registers.dx) } break;                                           // PUSH dx
	case 0x53: { V86_PUSH(registers.bx) } break;                                           // PUSH bx
	case 0x54: { V86_PUSH(registers.sp) } break;                                           // PUSH sp
	case 0x55: { V86_PUSH(registers.bp) } break;                                           // PUSH bp
	case 0x56: { V86_PUSH(registers.si) } break;                                           // PUSH si
	case 0x57: { V86_PUSH(registers.di) } break;                                           // PUSH di
	case 0x58: { registers.ax = V86_POP; } break;                                          // POP ax
	case 0x59: { registers.cx = V86_POP; } break;                                          // POP cx
	case 0x5A: { registers.dx = V86_POP; } break;                                          // POP dx
	case 0x5B: { registers.bx = V86_POP; } break;                                          // POP bx
	case 0x5C: { registers.sp = V86_POP; } break;                                          // POP sp
	case 0x5D: { registers.bp = V86_POP; } break;                                          // POP bp
	case 0x5E: { registers.si = V86_POP; } break;                                          // POP si
	case 0x5F: { registers.di = V86_POP; } break;                                          // POP di

	// 0x6X Opcodes are not implemented

	case 0x70: { V86_IMM8 V86_JCC(flags.overflow) } break;                                 // JO
	case 0x71: { V86_IMM8 V86_JCC(!flags.overflow) } break;                                // JNO
	case 0x72: { V86_IMM8 V86_JCC(flags.carry) } break;                                    // JB
	case 0x73: { V86_IMM8 V86_JCC(!flags.carry) } break;                                   // JNB
	case 0x74: { V86_IMM8 V86_JCC(flags.zero) } break;                                     // JZ
	case 0x75: { V86_IMM8 V86_JCC(!flags.zero) } break;                                    // JNZ
	case 0x76: { V86_IMM8 V86_JCC(flags.carry || flags.zero) } break;                      // JBE
	case 0x77: { V86_IMM8 V86_JCC(!(flags.carry || flags.zero)) } break;                   // JA
	case 0x78: { V86_IMM8 V86_JCC(flags.sign) } break;                                     // JS
	case 0x79: { V86_IMM8 V86_JCC(!flags.sign) } break;                                    // JNS
	case 0x7A: { V86_IMM8 V86_JCC(flags.parity) } break;                                   // JPE
	case 0x7B: { V86_IMM8 V86_JCC(!flags.parity) } break;                                  // JPO
	case 0x7C: { V86_IMM8 V86_JCC(flags.sign != flags.overflow) } break;                   // JL
	case 0x7D: { V86_IMM8 V86_JCC(flags.sign == flags.overflow) } break;                   // JGE
	case 0x7E: { V86_IMM8 V86_JCC(flags.zero || (flags.sign != flags.overflow)) } break;   // JLE
	case 0x7F: { V86_IMM8 V86_JCC(!flags.zero && (flags.sign == flags.overflow)) } break;  // JG

	case 0x80: { V86_MODRM V86_IMM8 v86Grp1_8(*this, modrm, displace, V86_IMM); } break;   // GRP1 r/m8, imm8
	case 0x81: { V86_MODRM V86_IMM16 v86Grp1_16(*this, modrm, displace, V86_IMM); } break; // GRP1 r/m16, imm16
	case 0x82: { V86_MODRM V86_IMM8 v86Grp1_8(*this, modrm, displace, V86_IMM); } break;   // GRP1 r/m8, imm8
	case 0x83: {                                                                           // GRP1 r/m16, imm8
		V86_MODRM
		V86_IMM8
		uint16_t imm16 = imm;
		if (imm & 0x80) imm16 |= 0xFF00;
		v86Grp1_16(*this, modrm, displace, imm16);
	} break;
	case 0x84: { V86_MODRM V86_TEST8(V86_READ_RM8, V86_READ_R8) } break;                   // TEST r/m8, r8
	case 0x85: { V86_MODRM V86_TEST16(V86_READ_RM16, V86_READ_R16) } break;                // TEST r/m16, r16
	case 0x86: {                                                                           // XCHG r/m8, r8
		V86_MODRM
		uint8_t tmp = V86_READ_RM8;
		V86_WRITE_RM8(V86_READ_R8)
		V86_WRITE_R8(tmp)
	} break;
	case 0x87: {                                                                           // XCHG r/m16, r16
		V86_MODRM
		uint16_t tmp = V86_READ_RM16;
		V86_WRITE_RM16(V86_READ_R16)
		V86_WRITE_R16(tmp)
	} break;
	case 0x88: { V86_MODRM V86_WRITE_RM8(V86_READ_R8) } break;                             // MOV r/m8, r8
	case 0x89: { V86_MODRM V86_WRITE_RM16(V86_READ_R16) } break;                           // MOV r/m16, r16
	case 0x8A: { V86_MODRM V86_WRITE_R8(V86_READ_RM8) } break;                             // MOV r8, r/m8
	case 0x8B: { V86_MODRM V86_WRITE_R16(V86_READ_RM16) } break;                           // MOV r16, r/m16
	case 0x8C: { V86_MODRM V86_WRITE_RM16(V86_READ_SEG16) } break;                         // MOV r/m16, sr16
	case 0x8D: { V86_MODRM V86_WRITE_R16(V86_LEA) } break;                                 // LEA r16, m16
	case 0x8E: { V86_MODRM V86_WRITE_SEG16(V86_READ_RM16) } break;                         // MOV sr16, r/m16
	case 0x8F: { V86_MODRM V86_WRITE_RM16(V86_POP) } break;                                // POP r/m16

	case 0x90: break;                                                                      // NOP
	case 0x91: { V86_SWAPR(registers.cx, registers.ax) } break;                            // XCHG cx, ax
	case 0x92: { V86_SWAPR(registers.dx, registers.ax) } break;                            // XCHG dx, ax
	case 0x93: { V86_SWAPR(registers.bx, registers.ax) } break;                            // XCHG bx, ax
	case 0x94: { V86_SWAPR(registers.sp, registers.ax) } break;                            // XCHG sp, ax
	case 0x95: { V86_SWAPR(registers.bp, registers.ax) } break;                            // XCHG bp, ax
	case 0x96: { V86_SWAPR(registers.si, registers.ax) } break;                            // XCHG si, ax
	case 0x97: { V86_SWAPR(registers.di, registers.ax) } break;                            // XCHG di, ax
	case 0x98: { registers.ax = (uint16_t)((int16_t)((int8_t)registers.al)); } break;      // CBW
	case 0x99: { registers.dx = registers.ax & 0x8000 ? 0xFFFF : 0; } break;               // CWD
	case 0x9A: {                                                                           // CALL ptr16:16
		V86_PUSH(registers.cs)
		V86_PUSH(registers.ip)
		initIP = v86ReadImm16(*this);
		registers.cs = v86ReadImm16(*this);
	} break;
	case 0x9B: break;                                                                      // WAIT
	case 0x9C: { V86_PUSH(V86_GETF) } break;                                               // PUSHF
	case 0x9D: { V86_SETF(V86_POP) } break;                                                // POPF
	case 0x9E: {                                                                           // SAHF
		flags.carry =    registers.ah & 0b00000001;
		flags.parity =   registers.ah & 0b00000100;
		flags.auxCarry = registers.ah & 0b00010000;
		flags.zero =     registers.ah & 0b01000000;
		flags.sign =     registers.ah & 0b10000000;
	} break;
	case 0x9F: {                                                                           // LAHF
		uint8_t ah = 0b00000010;
		if (flags.carry)    ah |= 0b00000001;
		if (flags.parity)   ah |= 0b00000100;
		if (flags.auxCarry) ah |= 0b00010000;
		if (flags.zero)     ah |= 0b01000000;
		if (flags.sign)     ah |= 0b10000000;
		registers.ah = ah;
	} break;

	case 0xA0: {                                                                           // MOV al, [seg:imm16]
		V86_IMM16
		registers.al = access.memRead(v86Segment(v86GetSeg(*this),V86_IMM));
	} break;
	case 0xA1: {                                                                           // MOV ax, [seg:imm16]
		V86_IMM16
		uint16_t seg = v86GetSeg(*this);
		registers.al = access.memRead(v86Segment(seg,V86_IMM));
		registers.ah = access.memRead(v86Segment(seg,V86_IMM+1));
	} break;
	case 0xA2: {                                                                           // MOV [seg:imm16], al
		V86_IMM16
		access.memWrite(v86Segment(v86GetSeg(*this),V86_IMM), registers.al);
	} break;
	case 0xA3: {                                                                           // MOV [seg:imm16], ax
		V86_IMM16
		uint16_t seg = v86GetSeg(*this);
		access.memWrite(v86Segment(seg,V86_IMM), registers.al);
		access.memWrite(v86Segment(seg,V86_IMM+1), registers.ah);
	} break;
	case 0xA4: {                                                                           // MOVSB
		if (state.rep == Virtual8086::REP_NZ) state.rep = Virtual8086::REP_NONE;
		V86_REP(false, {
			uint8_t byte = access.memRead(v86Segment(registers.ds,registers.si));
			access.memWrite(v86Segment(registers.es, registers.di), byte);
			if (flags.direction) {
				registers.si--;
				registers.di--;
			} else {
				registers.si++;
				registers.di++;
			}
		})
	} break;
	case 0xA5: {                                                                           // MOVSW
		if (state.rep == Virtual8086::REP_NZ) state.rep = Virtual8086::REP_NONE;
		V86_REP(false, {
			uint8_t byte = access.memRead(v86Segment(registers.ds,registers.si));
			access.memWrite(v86Segment(registers.es,registers.di), byte);
			byte = access.memRead(v86Segment(registers.ds,registers.si+1));
			access.memWrite(v86Segment(registers.es,registers.di+1), byte);
			if (flags.direction) {
				registers.si -= 2;
				registers.di -= 2;
			} else {
				registers.si += 2;
				registers.di += 2;
			}
		})
	} break;
	case 0xA6: {                                                                           // CMPSB
		V86_REP(true, {
			uint8_t a = access.memRead(v86Segment(registers.ds,registers.si));
			uint8_t b = access.memRead(v86Segment(registers.es,registers.di));
			V86_CMP8(a,b);
			if (flags.direction) {
				registers.si--;
				registers.di--;
			} else {
				registers.si++;
				registers.di++;
			}
		})
	} break;
	case 0xA7: {                                                                           // CMPSW
		V86_REP(true, {
			uint16_t a = access.memRead(v86Segment(registers.ds,registers.si));
			a |= (uint16_t)access.memRead(v86Segment(registers.ds,registers.si+1)) << 8;
			uint16_t b = access.memRead(v86Segment(registers.es,registers.di));
			b |= (uint16_t)access.memRead(v86Segment(registers.es,registers.di)) << 8;
			V86_CMP16(a,b);
			if (flags.direction) {
				registers.si -= 2;
				registers.di -= 2;
			} else {
				registers.si += 2;
				registers.di += 2;
			}
		})
	} break;
	case 0xA8: { V86_IMM8 V86_TEST8(registers.al, V86_IMM) } break;                        // TEST al, imm8
	case 0xA9: { V86_IMM16 V86_TEST16(registers.ax, V86_IMM) } break;                      // TEST ax, imm16
	case 0xAA: {                                                                           // STOSB
		V86_REP(false, {
			access.memWrite(v86Segment(registers.es,registers.di), registers.al);
			if (flags.direction) registers.di--;
			else registers.di++;
		})
	} break;
	case 0xAB: {                                                                           // STOSW
		V86_REP(false, {
			access.memWrite(v86Segment(registers.es,registers.di), registers.al);
			access.memWrite(v86Segment(registers.es,registers.di+1), registers.ah);
			if (flags.direction) registers.di -= 2;
			else registers.di += 2;
		})
	} break;
	case 0xAC: {                                                                           // LODSB
		V86_REP(false, {
			registers.al = access.memRead(v86Segment(registers.ds,registers.si));
			if (flags.direction) registers.si--;
			else registers.si++;
		})
	} break;
	case 0xAD: {                                                                           // LODSW
		V86_REP(false, {
			registers.al = access.memRead(v86Segment(registers.ds,registers.si));
			registers.ah = access.memRead(v86Segment(registers.ds,registers.si+1));
			if (flags.direction) registers.si -= 2;
			else registers.si += 2;
		})
	} break;
	case 0xAE: {                                                                           // SCASB
		V86_REP(true, {
			uint8_t byte = access.memRead(v86Segment(registers.es,registers.di));
			V86_CMP8(registers.al, byte);
			if (flags.direction) registers.di--;
			else registers.di++;
		})
	} break;
	case 0xAF: {                                                                           // SCASW
		V86_REP(true, {
			uint16_t word = access.memRead(v86Segment(registers.es,registers.di));
			word |= (uint16_t)access.memRead(v86Segment(registers.es,registers.di+1)) << 8;
			V86_CMP16(registers.ax, word);
			if (flags.direction) registers.di -= 2;
			else registers.di += 2;
		})
	} break;

	case 0xB0: { V86_IMM8 registers.al = V86_IMM; } break;                                 // MOV al, imm8
	case 0xB1: { V86_IMM8 registers.cl = V86_IMM; } break;                                 // MOV cl, imm8
	case 0xB2: { V86_IMM8 registers.dl = V86_IMM; } break;                                 // MOV dl, imm8
	case 0xB3: { V86_IMM8 registers.bl = V86_IMM; } break;                                 // MOV bl, imm8
	case 0xB4: { V86_IMM8 registers.ah = V86_IMM; } break;                                 // MOV ah, imm8
	case 0xB5: { V86_IMM8 registers.ch = V86_IMM; } break;                                 // MOV ch, imm8
	case 0xB6: { V86_IMM8 registers.dh = V86_IMM; } break;                                 // MOV dh, imm8
	case 0xB7: { V86_IMM8 registers.bh = V86_IMM; } break;                                 // MOV bh, imm8
	case 0xB8: { V86_IMM16 registers.ax = V86_IMM; } break;                                // MOV ax, imm16
	case 0xB9: { V86_IMM16 registers.cx = V86_IMM; } break;                                // MOV cx, imm16
	case 0xBA: { V86_IMM16 registers.dx = V86_IMM; } break;                                // MOV dx, imm16
	case 0xBB: { V86_IMM16 registers.bx = V86_IMM; } break;                                // MOV bx, imm16
	case 0xBC: { V86_IMM16 registers.sp = V86_IMM; } break;                                // MOV sp, imm16
	case 0xBD: { V86_IMM16 registers.bp = V86_IMM; } break;                                // MOV bp, imm16
	case 0xBE: { V86_IMM16 registers.si = V86_IMM; } break;                                // MOV si, imm16
	case 0xBF: { V86_IMM16 registers.di = V86_IMM; } break;                                // MOV di, imm16

	// 0xC0 unused
	// 0xC1 unused
	case 0xC2: { V86_IMM16 V86_JMP(V86_POP) registers.sp += V86_IMM; } break;              // RET imm16
	case 0xC3: { V86_JMP(V86_POP) } break;                                                 // RET
	case 0xC4: {                                                                           // LES es:r16, m16:16
		V86_MODRM
		uint8_t mod = modrm >> 6;
		uint8_t reg = modrm & 0x7;
		registers.es = v86ComputeRMSeg(*this, reg, mod);
		V86_WRITE_R16(v86ComputeRMAddr(*this, reg, mod, displace));
	} break;
	case 0xC5: {                                                                           // LDS ds:r16, m16:16
		V86_MODRM
		uint8_t mod = modrm >> 6;
		uint8_t reg = modrm & 0x7;
		registers.ds = v86ComputeRMSeg(*this, reg, mod);
		V86_WRITE_R16(v86ComputeRMAddr(*this, reg, mod, displace));
	} break;
	case 0xC6: { V86_MODRM V86_IMM8 V86_WRITE_RM8(V86_IMM) } break;                        // MOV r/m8, imm8
	case 0xC7: { V86_MODRM V86_IMM16 V86_WRITE_RM16(V86_IMM) } break;                      // MOV r/m16, imm16
	// 0xC8 unused
	// 0xC9 unused
	case 0xCA: {                                                                           // RETF imm16
		V86_IMM16
		V86_JMP(V86_POP)
		registers.cs = V86_POP;
		registers.sp += V86_IMM;
	} break;
	case 0xCB: { V86_JMP(V86_POP) registers.cs = V86_POP; } break;                         // RETF
	case 0xCC: { V86_INT(3) } break;                                                       // INT 3
	case 0xCD: { V86_IMM8 V86_INT(V86_IMM) } break;                                        // INT imm8
	case 0xCE: { if (flags.overflow) V86_INT(4) } break;                                   // INTO
	case 0xCF: {                                                                           // IRET
		registers.ip = V86_POP;
		registers.cs = V86_POP;
		V86_SETF(V86_POP)
	} break;

	case 0xD0: // GRP2 r/m8, 1 TODO Finish 8086 emulation
	case 0xD1: // GRP2 r/m16, 1 TODO Finish 8086 emulation
	case 0xD2: // GRP2 r/m8, cl TODO Finish 8086 emulation
	case 0xD3: // GRP2 r/m16, cl TODO Finish 8086 emulation
	case 0xD4: {                                                                           // AAM imm8
		V86_IMM8;
		uint8_t al = registers.al;
		registers.ah = al / V86_IMM;
		registers.al = al % V86_IMM;
		flags.sign = registers.al & 0x80;
		flags.zero = registers.al == 0;
		flags.parity = X86::parity(registers.al);
	} break;
	case 0xD5: {                                                                           // AAD imm8
		V86_IMM8
		uint8_t al = registers.al, ah = registers.ah;
		registers.al = (al + (ah * V86_IMM));
		registers.ah = 0;
	} break;
	// 0xD6 unused
	case 0xD7: {                                                                           // XLAT
		uint16_t index = registers.bx;
		index += registers.al;
		registers.al = v86Segment(registers.ds, index);
	} break;
	// 0xD8-0xDF unused

	case 0xE0: {                                                                           // LOOPNZ
		V86_IMM8
		if (--registers.cx != 0 && !flags.zero) {
			initIP += V86_IMM;
			doJump = true;
		}
	} break;
	case 0xE1: {                                                                           // LOOPZ
		V86_IMM8
		if (--registers.cx != 0 && flags.zero) {
			initIP += V86_IMM;
			doJump = true;
		}
	} break;
	case 0xE2: {                                                                           // LOOP
		V86_IMM8
		if (--registers.cx != 0) {
			initIP += V86_IMM;
			doJump = true;
		}
	} break;
	case 0xE3: { V86_IMM8 if (registers.cx == 0) { initIP += V86_IMM; doJump = true; }} break; // JCXZ
	case 0xE4: { V86_IMM8 registers.al = access.ioRead(V86_IMM); } break;                  // IN al, imm8
	case 0xE5: {                                                                           // IN ax, imm8
		V86_IMM8
		registers.al = access.ioRead(V86_IMM);
		registers.ah = access.ioRead((V86_IMM + 1) & 0xFF);
	} break;
	case 0xE6: { V86_IMM8 access.ioWrite(V86_IMM, registers.al); } break;                  // OUT imm8, al
	case 0xE7: {                                                                           // OUT imm8, ax
		V86_IMM8
		access.ioWrite(V86_IMM, registers.al);
		access.ioWrite((V86_IMM + 1) & 0xFF, registers.ah);
	} break;
	case 0xE8: {                                                                           // CALL imm16 (relative)
		V86_IMM16
		V86_PUSH(registers.ip)
		initIP = registers.ip + V86_IMM;
		doJump = true;
	} break;
	case 0xE9: {                                                                           // JMP imm16 (relative)
		V86_IMM16
		initIP = registers.ip + V86_IMM;
		doJump = true;
	} break;
	case 0xEA: { V86_IMM16 V86_JMP(V86_IMM) } break;                                       // JMP imm16
	case 0xEB: {                                                                           // JMP imm8 (relative)
		V86_IMM8
		initIP = registers.ip + V86_IMM;
		doJump = true;
	} break;
	case 0xEC: { registers.al = access.ioRead(registers.dx); } break;                      // IN al, dx
	case 0xED: {                                                                           // IN ax, dx
		registers.al = access.ioRead(registers.dx);
		registers.ah = access.ioRead(registers.dx+1);
	} break;
	case 0xEE: { access.ioWrite(registers.dx, registers.al); } break;                      // OUT dx, al
	case 0xEF: {                                                                           // OUT dx, ax
		access.ioWrite(registers.dx, registers.al);
		access.ioWrite(registers.dx+1, registers.ah);
	} break;

	case 0xF0: break; // LOCK
	// 0xF1 unused
	case 0xF2: { state.rep = Virtual8086::REP_NZ; setRep = true; } break;                  // REPNZ
	case 0xF3: { state.rep = Virtual8086::REP_Z; setRep = true; } break;                   // REPZ
	case 0xF4: state.halted = true; break;                                                 // HLT
	case 0xF5: flags.carry ^= true; break;                                                 // CMC
	case 0xF6: // GRP3a r/m8 TODO Finish 8086 emulation
	case 0xF7: // GRP3b r/m16 TODO Finish 8086 emulation
	case 0xF8: flags.carry = false; break;                                                 // CLC
	case 0xF9: flags.carry = true; break;                                                  // STC
	case 0xFA: flags.interrupt = false; break;                                             // CLI
	case 0xFB: flags.interrupt = true; break;                                              // STI
	case 0xFC: flags.direction = false; break;                                             // CLD
	case 0xFD: flags.direction = true; break;                                              // STD
	case 0xFE: // GRP4 r/m8 TODO Finish 8086 emulation
	case 0xFF: // GRP5 r/m16 TODO Finish 8086 emulation

	default: break;
	}

	if (doJump) {
		registers.ip = (uint16_t)initIP;
	}

	if (!setSeg) state.segOverride = Virtual8086::SEGOVR_NONE;
	if (!setRep) state.rep = Virtual8086::REP_NONE;
	state.lock = false;
}
