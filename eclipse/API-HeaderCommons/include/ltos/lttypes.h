/*
 * lttypes.h
 *
 *  Created on: Oct 17, 2018
 *      Author: Zekrom_64
 */

#ifndef LTOS_LTTYPES_H_
#define LTOS_LTTYPES_H_


#include <cstdint>

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


#endif /* LTOS_LTTYPES_H_ */
