/*
 * ltoptimize.h
 *
 *  Created on: Nov 3, 2018
 *      Author: Zekrom_64
 */

#ifndef LTOS_API_LTOPTIMIZE_H_
#define LTOS_API_LTOPTIMIZE_H_


#include "ltos/ltarch.h"
#include "ltos/ltmacros.h"

#ifdef LT_ARCH_FAMILY_X86
#include "ltos/api/arch/x86/asm.h"
#define LT_ENDIAN_SWAP16(V) ltos::x86::X86::xchg(V)
#define LT_ENDIAN_SWAP32(V) ltos::x86::X86::bswap(V)
#define LT_ENDIAN_SWAP64(V) ltos::x86::X86::bswap64(V)
#endif


#ifdef LT_ENDIAN_LITTLE
#define LT_MAKE_LITTLE_ENDIAN16(X) (X)
#define LT_MAKE_LITTLE_ENDIAN32(X) (X)
#define LT_MAKE_LITTLE_ENDIAN64(X) (X)
#define LT_MAKE_BIG_ENDIAN16(X) LT_ENDIAN_SWAP16(X)
#define LT_MAKE_BIG_ENDIAN32(X) LT_ENDIAN_SWAP32(X)
#define LT_MAKE_BIG_ENDIAN64(X) LT_ENDIAN_SWAP64(X)
#else
#define LT_MAKE_LITTLE_ENDIAN16(X) LT_ENDIAN_SWAP16(X)
#define LT_MAKE_LITTLE_ENDIAN32(X) LT_ENDIAN_SWAP32(X)
#define LT_MAKE_LITTLE_ENDIAN64(X) LT_ENDIAN_SWAP64(X)
#define LT_MAKE_BIG_ENDIAN16(X) (X)
#define LT_MAKE_BIG_ENDIAN32(X) (X)
#define LT_MAKE_BIG_ENDIAN64(X) (X)
#endif

namespace ltos {

// Explicit endianness data types

	/** An unsigned 16-bit integer stored in big-endian format. */
	LT_PACK_PREFIX
	union uint16be_t {
		uint8_t bytes[2];
		uint16_t value;
#ifdef LT_ENDIAN_BIG
		uint16be_t(uint16_t val) : value(val) {}
		inline operator uint16_t() { return value; }
#else
		uint16be_t(uint16_t val) : value(LT_ENDIAN_SWAP16(val)) {}
		inline operator uint16_t() { return LT_ENDIAN_SWAP16(value); }
#endif
	} LT_PACK_ATTRIB;
	LT_PACK_SUFFIX
	LT_ASSERT_SIZEOF(uint16be_t, sizeof(uint16_t))

	/** An unsigned 32-bit integer stored in big-endian format. */
	LT_PACK_PREFIX
	union uint32be_t {
		uint8_t bytes[4];
		uint32_t value;
#ifdef LT_ENDIAN_BIG
		uint32be_t(uint32_t val) : value(val) {}
		inline operator uint32_t() { return value; }
#else
		uint32be_t(uint32_t val) : value(LT_ENDIAN_SWAP32(val)) {}
		inline operator uint32_t() { return LT_ENDIAN_SWAP32(value); }
#endif
	} LT_PACK_ATTRIB;
	LT_PACK_SUFFIX
	LT_ASSERT_SIZEOF(uint32be_t, sizeof(uint32_t))

	/** An unsigned 64-bit integer stored in big-endian format. */
	LT_PACK_PREFIX
	union uint64be_t {
		uint8_t bytes[8];
		uint64_t value;
#ifdef LT_ENDIAN_BIG
		uint16be_t(uint64_t val) : value(val) {}
		inline operator uint64_t() { return value; }
#else
		uint64be_t(uint64_t val) : value(LT_ENDIAN_SWAP64(val)) {}
		inline operator uint64_t() { return LT_ENDIAN_SWAP64(value); }
#endif
	} LT_PACK_ATTRIB;
	LT_PACK_SUFFIX
	LT_ASSERT_SIZEOF(uint64be_t, sizeof(uint16_t))

	/** An unsigned 16-bit integer stored in little-endian format. */
	LT_PACK_PREFIX
	union uint16le_t {
		uint8_t bytes[2];
		uint16_t value;
#ifdef LT_ENDIAN_LITTLE
		uint16le_t(uint16_t val) : value(val) {}
		inline operator uint16_t() { return value; }
#else
		uint16le_t(uint16_t val) : value(LT_ENDIAN_SWAP16(val)) {}
		inline operator uint16_t() { return LT_ENDIAN_SWAP16(value); }
#endif
	} LT_PACK_ATTRIB;
	LT_PACK_SUFFIX
	LT_ASSERT_SIZEOF(uint16le_t, sizeof(uint16_t))

	/** An unsigned 32-bit integer stored in little-endian format. */
	LT_PACK_PREFIX
	union uint32le_t {
		uint8_t bytes[4];
		uint32_t value;
#ifdef LT_ENDIAN_LITTLE
		uint32le_t(uint32_t val) : value(val) {}
		inline operator uint32_t() { return value; }
#else
		uint32le_t(uint32_t val) : value(LT_ENDIAN_SWAP32(val)) {}
		inline operator uint32_t() { return LT_ENDIAN_SWAP32(value); }
#endif
	} LT_PACK_ATTRIB;
	LT_PACK_SUFFIX
	LT_ASSERT_SIZEOF(uint32le_t, sizeof(uint32_t))

	/** An unsigned 64-bit integer stored in little-endian format. */
	LT_PACK_PREFIX
	union uint64le_t {
		uint8_t bytes[8];
		uint64_t value;
#ifdef LT_ENDIAN_LITTLE
		uint64le_t(uint64_t val) : value(val) {}
		inline operator uint64_t() { return value; }
#else
		uint64le_t(uint64_t val) : value(LT_ENDIAN_SWAP64(val)) {}
		inline operator uint64_t() { return LT_ENDIAN_SWAP64(value); }
#endif
	} LT_PACK_ATTRIB;
	LT_PACK_SUFFIX
	LT_ASSERT_SIZEOF(uint64le_t, sizeof(uint64_t))

}

#endif /* LTOS_API_LTOPTIMIZE_H_ */
