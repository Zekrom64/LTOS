/*
 * v86_funcs.inc
 *
 *  Created on: Oct 19, 2018
 *      Author: Zekrom_64
 */

#ifndef API_ARC_X86_V86_FUNCS_INC_
#define API_ARC_X86_V86_FUNCS_INC_

#include "ltos/api/arch/x86/v86.h"
#include "ltos/api/arch/x86/asm.h"

using namespace ltos::x86;

/* -----------------------
 * | Address Computation |
 * ----------------------- */

inline uint32_t v86Segment(uint16_t seg, uint16_t reg) {
	uint32_t address = seg;
	address <<= 4;
	address += reg;
	address &= 0xFFFFF;
	return address;
}

inline uint16_t v86GetSeg(Virtual8086& vm) {
	switch(vm.state.segOverride) {
	case Virtual8086::SEGOVR_CS: return vm.registers.cs;
	case Virtual8086::SEGOVR_ES: return vm.registers.es;
	case Virtual8086::SEGOVR_SS: return vm.registers.ss;
	default: return vm.registers.ds;
	}
}

/* --------------------
 * | Immediate Values |
 * --------------------
 */

#define V86_IMM8 uint8_t imm = access.memRead(v86Segment(registers.cs, registers.ip++));
#define V86_IMM16 uint16_t imm = v86ReadImm16(*this);
#define V86_IMM imm

inline uint16_t v86ReadImm16(Virtual8086& vm) {
	uint16_t imm = vm.access.memRead(v86Segment(vm.registers.cs, vm.registers.ip));
	imm |= vm.access.memRead(v86Segment(vm.registers.cs, vm.registers.ip)) << 8;
	vm.registers.ip += 2;
	return imm;
}

/* -----------------------
 * | Register Read/Write |
 * ----------------------- */

inline uint8_t v86ReadReg8(Virtual8086& vm, uint8_t modrm) {
	switch((modrm >> 3) & 0x7) {
	case 0: return vm.registers.al;
	case 1: return vm.registers.cl;
	case 2: return vm.registers.dl;
	case 3: return vm.registers.bl;
	case 4: return vm.registers.ah;
	case 5: return vm.registers.ch;
	case 6: return vm.registers.dh;
	case 7: return vm.registers.bh;
	}
	return 0;
}

#define V86_READ_R8 v86ReadReg8(*this, modrm)

inline void v86WriteReg8(Virtual8086& vm, uint8_t modrm, uint8_t val) {
	switch((modrm >> 3) & 0x7) {
	case 0: vm.registers.al = val; break;
	case 1: vm.registers.cl = val; break;
	case 2: vm.registers.dl = val; break;
	case 3: vm.registers.bl = val; break;
	case 4: vm.registers.ah = val; break;
	case 5: vm.registers.ch = val; break;
	case 6: vm.registers.dh = val; break;
	case 7: vm.registers.bh = val; break;
	}
}

#define V86_WRITE_R8(V) v86WriteReg8(*this, modrm, V);

inline uint16_t v86ReadReg16(Virtual8086& vm, uint8_t modrm) {
	switch((modrm >> 3) & 0x7) {
	case 0: return vm.registers.ax;
	case 1: return vm.registers.cx;
	case 2: return vm.registers.dx;
	case 3: return vm.registers.bx;
	case 4: return vm.registers.sp;
	case 5: return vm.registers.bp;
	case 6: return vm.registers.si;
	case 7: return vm.registers.di;
	}
	return 0;
}

#define V86_READ_R16 v86ReadReg16(*this, modrm)

inline void v86WriteReg16(Virtual8086& vm, uint8_t modrm, uint16_t val) {
	switch((modrm >> 3) & 0x7) {
	case 0: vm.registers.ax = val; break;
	case 1: vm.registers.cx = val; break;
	case 2: vm.registers.dx = val; break;
	case 3: vm.registers.bx = val; break;
	case 4: vm.registers.sp = val; break;
	case 5: vm.registers.bp = val; break;
	case 6: vm.registers.si = val; break;
	case 7: vm.registers.di = val; break;
	}
}

#define V86_WRITE_R16(V) v86WriteReg16(*this, modrm, V);

inline uint16_t v86ReadSeg16(Virtual8086& vm, uint8_t modrm) {
	switch((modrm >> 3) & 0x7) {
	case 0: return vm.registers.es;
	case 1: return vm.registers.cs;
	case 2: return vm.registers.ss;
	case 6: return vm.registers.ds;
	}
	return 0;
}
#define V86_READ_SEG16 v86ReadSeg16(*this, modrm)

inline void v86WriteSeg16(Virtual8086& vm, uint8_t modrm, uint16_t val) {
	switch((modrm >> 3) & 0x7) {
	case 0: vm.registers.es = val; break;
	case 1: vm.registers.cs = val; break;
	case 2: vm.registers.ss = val; break;
	case 6: vm.registers.ds = val; break;
	}
}
#define V86_WRITE_SEG16(V) v86WriteSeg16(*this, modrm, V);


/* ------------------------------
 * | ModR/M Operand Computation |
 * ------------------------------ */

// Macro to read ModR/M byte describing operands
#define V86_MODRM uint8_t modrm = access.memRead(v86Segment(registers.cs, registers.ip++)); \
	uint16_t displace = 0; \
	switch(modrm >> 6) { \
	case 1: displace = access.memRead(v86Segment(registers.cs,registers.ip++)); break; \
	case 2: displace = access.memRead(v86Segment(registers.cs,registers.ip++)); \
		displace |= access.memRead(v86Segment(registers.cs,registers.ip++)) << 8; break; \
	default: break; \
	}

uint16_t v86ComputeRMAddr(Virtual8086& vm, uint8_t reg, uint8_t mod, uint16_t disp) {
	uint16_t index = 0;
	switch(reg) {
	case 0: index = vm.registers.bx + vm.registers.si; break; // BX + SI
	case 1: index = vm.registers.bx + vm.registers.di; break; // BX + DI
	case 2: index = vm.registers.bp + vm.registers.si; break; // BP + SI
	case 3: index = vm.registers.bp + vm.registers.di; break; // BP + DI
	case 4: index = vm.registers.si; break; // SI
	case 5: index = vm.registers.di; break; // DI
	case 6: // BP + disp, or just disp
		if (mod == 0) index = disp;
		else index = vm.registers.bp + disp;
		break;
	case 7: index = vm.registers.bx; break;
	}
	if (mod == 1 || mod == 2) index += disp;
	return index;
}
#define V86_LEA v86ComputeRMAddr(*this, modrm & 0x7, modrm >> 6, displace)

bool v86AddressUsesBP[] = {
	false, false, false, false,
	false, false, false, false,
	true,  true,  false, false,
	true,  true,  false, false,
	false, false, false, false,
	false, false, false, false,
	false, true,  true,  false,
	false, false, false, false
};

inline uint16_t v86ComputeRMSeg(Virtual8086& vm, uint8_t reg, uint8_t mod) {
	uint16_t segment = 0;
	switch(vm.state.segOverride) {
	case Virtual8086::SEGOVR_NONE:
		if (v86AddressUsesBP[(reg << 2) | mod]) segment = vm.registers.ss; // Segment is SS when addressing with BP
		else segment = vm.registers.ds; // Default segment is normally DS
		break;
	case Virtual8086::SEGOVR_DS: segment = vm.registers.ds; break;
	case Virtual8086::SEGOVR_CS: segment = vm.registers.cs; break;
	case Virtual8086::SEGOVR_ES: segment = vm.registers.es; break;
	case Virtual8086::SEGOVR_SS: segment = vm.registers.ss; break;
	}
	return segment;
}

// Function to read the 8-bit r/m operand
uint8_t v86ReadRM8(Virtual8086& vm, uint8_t modrm, uint16_t disp) {
	uint8_t mod = modrm >> 6;
	uint8_t reg = modrm & 0x7;
	if (mod == 3) return v86ReadReg8(vm, reg << 3); // reg

	uint16_t index = v86ComputeRMAddr(vm, reg, mod, disp);
	uint16_t segment = v86ComputeRMSeg(vm, reg, mod);

	return vm.access.memRead(v86Segment(segment, index));
}
// Macro value of the read value of the first operand
#define V86_READ_RM8 v86ReadRM8(*this, modrm, displace)

// Function to write the 8-bit r/m operand
void v86WriteRM8(Virtual8086& vm, uint8_t modrm, uint16_t disp, uint8_t value) {
	uint8_t mod = modrm >> 6;
	uint8_t reg = modrm & 0x7;
	if (mod == 3) { // reg
		v86WriteReg8(vm, reg << 3, value);
		return;
	}

	uint16_t index = v86ComputeRMAddr(vm, reg, mod, disp);
	uint16_t segment = v86ComputeRMSeg(vm, reg, mod);

	vm.access.memWrite(v86Segment(segment, index), value);
}
#define V86_WRITE_RM8(V) v86WriteRM8(*this, modrm, displace, V);

uint16_t v86ReadRM16(Virtual8086& vm, uint8_t modrm, uint16_t disp) {
	uint8_t mod = modrm >> 6;
	uint8_t reg = modrm & 0x7;
	if (mod == 3) return v86ReadReg16(vm, modrm); // reg

	uint16_t index = v86ComputeRMAddr(vm, reg, mod, disp);
	uint16_t segment = v86ComputeRMSeg(vm, reg, mod);

	uint16_t value = vm.access.memRead(v86Segment(segment, index++));
	value |= vm.access.memRead(v86Segment(segment, index)) << 8;
	return value;
}
#define V86_READ_RM16 v86ReadRM16(*this, modrm, displace)

void v86WriteRM16(Virtual8086& vm, uint8_t modrm, uint16_t disp, uint16_t value) {
	uint8_t mod = modrm >> 6;
	uint8_t reg = modrm & 0x7;
	if (mod == 3) { // reg
		v86WriteReg16(vm, reg << 3, value);
		return;
	}

	uint16_t index = v86ComputeRMAddr(vm, reg, mod, disp);
	uint16_t segment = v86ComputeRMSeg(vm, reg, mod);

	vm.access.memWrite(v86Segment(segment, index++), (uint8_t)value);
	vm.access.memWrite(v86Segment(segment, index), (uint8_t)(value >> 8));
}
#define V86_WRITE_RM16(V) v86WriteRM16(*this, modrm, displace, V);

/* --------------------
 * | Stack Operations |
 * -------------------- */

inline void v86Push(Virtual8086& vm, uint16_t val) {
	vm.access.memWrite(v86Segment(vm.registers.ss, --vm.registers.sp), (uint8_t)(val >> 8));
	vm.access.memWrite(v86Segment(vm.registers.ss, --vm.registers.sp), (uint8_t)val);
}
#define V86_PUSH(V) v86Push(*this, V);

inline uint16_t v86Pop(Virtual8086& vm) {
	uint16_t val = vm.access.memRead(v86Segment(vm.registers.ss, vm.registers.sp++));
	val |= (uint16_t)vm.access.memRead(v86Segment(vm.registers.ss, vm.registers.sp++)) << 8;
	return val;
}
#define V86_POP v86Pop(*this)

/* -------------------
 * | Flag Read/Write |
 * ------------------- */

inline uint16_t v86GetFlags(Virtual8086& vm) {
	uint16_t f = 0;
	if (vm.flags.carry)     f |= 0b0000000000000001;
	if (vm.flags.parity)    f |= 0b0000000000000100;
	if (vm.flags.auxCarry)  f |= 0b0000000000010000;
	if (vm.flags.zero)      f |= 0b0000000001000000;
	if (vm.flags.sign)      f |= 0b0000000010000000;
	if (vm.flags.trap)      f |= 0b0000000100000000;
	if (vm.flags.interrupt) f |= 0b0000001000000000;
	if (vm.flags.direction) f |= 0b0000010000000000;
	if (vm.flags.overflow)  f |= 0b0000100000000000;
	return f;
}
#define V86_GETF v86GetFlags(*this)

inline void v86SetFlags(Virtual8086& vm, uint16_t v) {
	vm.flags.carry =     v & 0b0000000000000001;
	vm.flags.parity =    v & 0b0000000000000100;
	vm.flags.auxCarry =  v & 0b0000000000010000;
	vm.flags.zero =      v & 0b0000000001000000;
	vm.flags.sign =      v & 0b0000000010000000;
	vm.flags.trap =      v & 0b0000000100000000;
	vm.flags.interrupt = v & 0b0000001000000000;
	vm.flags.direction = v & 0b0000010000000000;
	vm.flags.overflow =  v & 0b0000100000000000;
}
#define V86_SETF(F) v86SetFlags(*this, F);

/* ------------------
 * | ALU Operations |
 * ------------------ */

// Add

inline uint8_t v86Add8(Virtual8086& vm, uint8_t a, uint8_t b) {
	uint16_t val = a;
	val += b;
	vm.flags.overflow = ((val >> 8) & 1) ^ ((val >> 7) & 1);
	vm.flags.sign = val & 0x80;
	vm.flags.zero = (val & 0xFF) == 0;
	vm.flags.auxCarry = ((a & 0xF) + (b & 0xF)) & 0x10;
	vm.flags.carry = val & 0x100;
	vm.flags.parity = X86::parity((uint8_t)val);
	return (uint8_t)val;
}
#define V86_ADD8(A,B) v86Add8(*this, A, B)

inline uint16_t v86Add16(Virtual8086& vm, uint16_t a, uint16_t b) {
	uint32_t val = a;
	val += b;
	vm.flags.overflow = ((val >> 16) & 1) ^ ((val >> 15) & 1);
	vm.flags.sign = val & 0x8000;
	vm.flags.zero = (val & 0xFFFF) == 0;
	vm.flags.auxCarry = ((a & 0xF) + (b & 0xF)) & 0x10;
	vm.flags.carry = val & 0x10000;
	vm.flags.parity = X86::parity((uint8_t)val);
	return (uint16_t)val;
}
#define V86_ADD16(A,B) v86Add16(*this, A, B)

// Add w/ carry

inline uint8_t v86Adc8(Virtual8086& vm, uint8_t a, uint8_t b) {
	uint16_t val = a;
	val += b;
	if (vm.flags.carry) val++;
	vm.flags.overflow = ((val >> 8) & 1) ^ ((val >> 7) & 1);
	vm.flags.sign = val & 0x80;
	vm.flags.zero = (val & 0xFF) == 0;
	vm.flags.auxCarry = ((a & 0xF) + (b & 0xF)) & 0x10;
	vm.flags.carry = val & 0x100;
	vm.flags.parity = X86::parity((uint8_t)val);
	return (uint8_t)val;
}
#define V86_ADC8(A,B) v86Adc8(*this, A, B)

inline uint16_t v86Adc16(Virtual8086& vm, uint16_t a, uint16_t b) {
	uint32_t val = a;
	val += b;
	if (vm.flags.carry) val++;
	vm.flags.overflow = ((val >> 16) & 1) ^ ((val >> 15) & 1);
	vm.flags.sign = val & 0x8000;
	vm.flags.zero = (val & 0xFFFF) == 0;
	vm.flags.auxCarry = ((a & 0xF) + (b & 0xF)) & 0x10;
	vm.flags.carry = val & 0x10000;
	vm.flags.parity = X86::parity((uint8_t)val);
	return (uint16_t)val;
}
#define V86_ADC16(A,B) v86Adc16(*this, A, B)

// Subtract

inline uint8_t v86Sub8(Virtual8086& vm, uint8_t a, uint8_t b) {
	uint8_t ret = v86Add8(vm, a, -b);
	vm.flags.carry ^= true;
	return ret;
}
#define V86_SUB8(A,B) v86Sub8(*this, A, -B)

inline uint16_t v86Sub16(Virtual8086& vm, uint16_t a, uint16_t b) {
	uint16_t ret = v86Add16(vm, a, -b);
	vm.flags.carry ^= true;
	return ret;
}
#define V86_SUB16(A,B) v86Sub16(*this, A, -B)

// Subtract w/ borrow

inline uint8_t v86Sbb8(Virtual8086& vm, uint8_t a, uint8_t b) {
	uint16_t val = a;
	val -= b;
	if (vm.flags.carry) val--;
	vm.flags.overflow = ((val >> 8) & 1) ^ ((val >> 7) & 1);
	vm.flags.sign = val & 0x80;
	vm.flags.zero = (val & 0xFF) == 0;
	vm.flags.auxCarry = ((a & 0xF) + (b & 0xF)) & 0x10;
	vm.flags.carry = !(val & 0x100);
	vm.flags.parity = X86::parity((uint8_t)val);
	return (uint8_t)val;
}
#define V86_SBB8(A,B) v86Sbb8(*this, A, B)

inline uint16_t v86Sbb16(Virtual8086& vm, uint16_t a, uint16_t b) {
	uint32_t val = a;
	val -= b;
	if (vm.flags.carry) val--;
	vm.flags.overflow = ((val >> 16) & 1) ^ ((val >> 15) & 1);
	vm.flags.sign = val & 0x8000;
	vm.flags.zero = (val & 0xFFFF) == 0;
	vm.flags.auxCarry = ((a & 0xF) + (b & 0xF)) & 0x10;
	vm.flags.carry = !(val & 0x10000);
	vm.flags.parity = X86::parity((uint8_t)val);
	return (uint16_t)val;
}
#define V86_SBB16(A,B) v86Sbb16(*this, A, B)

// Bitwise OR

inline uint8_t v86Or8(Virtual8086& vm, uint8_t a, uint8_t b) {
	vm.flags.overflow = false;
	vm.flags.carry = false;
	a |= b;
	vm.flags.sign = a & 0x80;
	vm.flags.zero = a == 0;
	vm.flags.parity = X86::parity(a);
	return a;
}
#define V86_OR8(A,B) v86Or8(*this, A, B)

inline uint16_t v86Or16(Virtual8086& vm, uint16_t a, uint16_t b) {
	vm.flags.overflow = false;
	vm.flags.carry = false;
	a |= b;
	vm.flags.sign = a & 0x8000;
	vm.flags.zero = a == 0;
	vm.flags.parity = X86::parity((uint8_t)a);
	return a;
}
#define V86_OR16(A,B) v86Or16(*this, A, B)

// Bitwise AND

inline uint8_t v86And8(Virtual8086& vm, uint8_t a, uint8_t b) {
	vm.flags.overflow = false;
	vm.flags.carry = false;
	a &= b;
	vm.flags.sign = a & 0x80;
	vm.flags.zero = a == 0;
	vm.flags.parity = X86::parity(a);
	return a;
}
#define V86_AND8(A,B) v86And8(*this, A, B)

inline uint16_t v86And16(Virtual8086& vm, uint16_t a, uint16_t b) {
	vm.flags.overflow = false;
	vm.flags.carry = false;
	a &= b;
	vm.flags.sign = a & 0x8000;
	vm.flags.zero = a == 0;
	vm.flags.parity = X86::parity((uint8_t)a);
	return a;
}
#define V86_AND16(A,B) v86And16(*this, A, B)

// Bitwise XOR

inline uint8_t v86Xor8(Virtual8086& vm, uint8_t a, uint8_t b) {
	vm.flags.overflow = false;
	vm.flags.carry = false;
	a ^= b;
	vm.flags.sign = a & 0x80;
	vm.flags.zero = a == 0;
	vm.flags.parity = X86::parity(a);
	return a;
}
#define V86_XOR8(A,B) v86Xor8(*this, A, B)

inline uint16_t v86Xor16(Virtual8086& vm, uint16_t a, uint16_t b) {
	vm.flags.overflow = false;
	vm.flags.carry = false;
	a ^= b;
	vm.flags.sign = a & 0x8000;
	vm.flags.zero = a == 0;
	vm.flags.parity = X86::parity((uint8_t)a);
	return a;
}
#define V86_XOR16(A,B) v86Xor16(*this, A, B)

// Compare

inline void v86Cmp8(Virtual8086& vm, uint8_t a, uint8_t b) {
	v86Sub8(vm, a, b);
}
#define V86_CMP8(A,B) v86Cmp8(*this, A, B)

inline void v86Cmp16(Virtual8086& vm, uint16_t a, uint16_t b) {
	v86Sub16(vm, a, b);
}
#define V86_CMP16(A,B) v86Cmp16(*this, A, B)

// Increment

inline uint8_t v86Inc8(Virtual8086& vm, uint8_t a) {
	bool oldCF = vm.flags.carry;
	a = v86Add8(vm, a, 1);
	vm.flags.carry = oldCF;
	return a;
}
#define V86_INC8(A) v86Inc8(*this, A)

inline uint16_t v86Inc16(Virtual8086& vm, uint16_t a) {
	bool oldCF = vm.flags.carry;
	a = v86Add16(vm, a, 1);
	vm.flags.carry = oldCF;
	return a;
}
#define V86_INC16(A) v86Inc16(*this, A)
#define V86_INCR16(R) R = v86Inc16(*this, R);

// Decrement

inline uint8_t v86Dec8(Virtual8086& vm, uint8_t a) {
	bool oldCF = vm.flags.carry;
	a = v86Sub8(vm, a, 1);
	vm.flags.carry = oldCF;
	return a;
}
#define V86_DEC8(A) v86Dec8(*this, A)

inline uint16_t v86Dec16(Virtual8086& vm, uint16_t a) {
	bool oldCF = vm.flags.carry;
	a = v86Sub16(vm, a, 1);
	vm.flags.carry = oldCF;
	return a;
}
#define V86_DEC16(A) v86Dec16(*this, A)
#define V86_DECR16(R) R = v86Dec16(*this, R);

// Exchange

inline void v86Xchg16(Virtual8086& vm, uint16_t& a, uint16_t& b) {
	uint16_t tmp = a;
	a = b;
	b = tmp;
}
#define V86_SWAPR(A,B) v86Xchg16(*this, A, B);

// Logical Compare (Test)

inline void v86Test8(Virtual8086& vm, uint8_t a, uint8_t b) {
	a &= b;
	vm.flags.sign = a & 0x80;
	vm.flags.zero = a == 0;
	vm.flags.parity = X86::parity(a);
}
#define V86_TEST8(A,B) v86Test8(*this, A, B);

inline void v86Test16(Virtual8086& vm, uint16_t a, uint16_t b) {
	a &= b;
	vm.flags.sign = a & 0x8000;
	vm.flags.zero = a == 0;
	vm.flags.parity = X86::parity((uint8_t)a);
}
#define V86_TEST16(A,B) v86Test16(*this, A, B);

// Rotate Left
inline uint8_t v86Rol8(Virtual8086& vm, uint8_t a, uint8_t b) {
	b &= 0x7;
	if (b != 0)
		a = (a << b) | (a >> (8-b));
	vm.flags.carry = a & 1;
	vm.flags.overflow = a & 0x80;
	return a;
}

inline uint16_t v86Rol16(Virtual8086& vm, uint16_t a, uint8_t b) {
	b &= 0xF;
	if (b != 0)
		a = (a << b) | (a >> (16-b));
	vm.flags.carry = a & 1;
	vm.flags.overflow = a & 0x8000;
	return a;
}

// Rotate right
inline uint8_t v86Ror8(Virtual8086& vm, uint8_t a, uint8_t b) {
	b &= 0x7;
	if (b != 0)
		a = (a >> b) | (a << (8-b));
	vm.flags.carry = a & 1;
	vm.flags.overflow = a & 0x80;
	return a;
}

inline uint16_t v86Ror16(Virtual8086& vm, uint16_t a, uint8_t b) {
	b &= 0xF;
	if (b != 0)
		a = (a >> b) | (a << (16-b));
	vm.flags.carry = a & 1;
	vm.flags.overflow = a & 0x8000;
	return a;
}

// Rotate left through carry
inline uint8_t v86Rcl8(Virtual8086& vm, uint8_t a, uint8_t b) {
	b %= 9;
	if (b == 0) return a;
	a = (a << b) | (a >> (9-b));
	if (vm.flags.carry) a |= 1 << b-1;
}

// Rotate right through carry
inline uint8_t v86Rcr8(Virtual8086& vm, uint8_t a, uint8_t b) {

}

// Shift left
inline uint8_t v86Shl8(Virtual8086& vm, uint8_t a, uint8_t b) {

}

// Shift right
inline uint8_t v86Shr8(Virtual8086& vm, uint8_t a, uint8_t b) {

}

// Arithmetic Shift Right
inline uint8_t v86Sar8(Virtual8086& vm, uint8_t a, uint8_t b) {

}


/* ---------
 * | Jumps |
 * --------- */

#define V86_JCC(C) if (C) initIP += (int8_t)imm; doJump = true;
#define V86_JMP(V) initIP = V; doJump = true;

inline void v86Int(Virtual8086& vm, uint8_t id, int16_t& initIP, bool& doJump) {
	if (!vm.flags.interrupt) return;

	v86Push(vm, v86GetFlags(vm)); // Push Flags
	v86Push(vm, vm.registers.cs); // Push CS
	v86Push(vm, vm.registers.ip); // Push IP

	// Read segment and offset from IVT
	uint32_t addr = id * 4;
	uint16_t seg, off;
	seg = vm.access.memRead(addr++);
	seg |= vm.access.memRead(addr++) << 8;
	off = vm.access.memRead(addr++);
	off |= vm.access.memRead(addr) << 8;

	// Clear interrupt and trap flags
	vm.flags.interrupt = false;
	vm.flags.trap = false;

	// Do far jump
	vm.registers.cs = seg;
	initIP = (int16_t)off;
	doJump = true;
}
#define V86_INT(V) v86Int(*this, V, initIP, doJump);

inline bool v86Rep(Virtual8086& vm, bool cmp) {
	switch(vm.state.rep) {
	case Virtual8086::REP_Z: return !(vm.registers.cx-- == 0 || (cmp && !vm.flags.zero));
	case Virtual8086::REP_NZ: return !(vm.registers.cx-- == 0 || vm.flags.zero);
	default: break;
	}
	return false;
}
#define V86_REP(C,X) if (state.rep == Virtual8086::REP_NONE) X else while(v86Rep(*this, C)) X

#endif /* API_ARC_X86_V86_FUNCS_INC_ */
