/*
 * lttypes.h
 *
 *  Created on: Oct 17, 2018
 *      Author: Zekrom_64
 */

#ifndef LTOS_LTTYPES_H_
#define LTOS_LTTYPES_H_


#include <cstdint>
#include "ltmacros.h"
#include "ltarch.h"


namespace ltos {

/* -----------
 * | Strings |
 * ----------- */
typedef char* lt_string8;
typedef const char* lt_cstring8;
typedef char16_t* lt_string16;
typedef const char16_t* lt_cstring16;
typedef char32_t* lt_string32;
typedef const char32_t* lt_cstring32;

#define LT_STRING8(S) u8##S
#define LT_STRING16(S) u##S
#define LT_STRING32(S) U##S

#define LT_STRING(S) LT_STRING8(S)
typedef lt_string8 lt_string;
typedef lt_cstring8 lt_cstring;


/* --------------
 * | Structures |
 * -------------- */
LT_PACK_PREFIX
template<typename K, typename V>
struct lt_pair {
	K key;
	V value;
} LT_PACK_ATTRIB;
LT_PACK_SUFFIX

LT_PACK_PREFIX
template<typename T>
struct lt_array {
	size_t size;
	T* elements;
} LT_PACK_ATTRIB;
LT_PACK_SUFFIX

}


#endif /* LTOS_LTTYPES_H_ */
