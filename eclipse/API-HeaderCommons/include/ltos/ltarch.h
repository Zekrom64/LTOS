/*
 * ltarch.h
 *
 *  Created on: Nov 3, 2018
 *      Author: Zekrom_64
 */

#ifndef LTOS_LTARCH_H_
#define LTOS_LTARCH_H_


// x86_64
#if defined(__amd64__) || defined(_M_X64)
#define LT_ARCH_FAMILY_X86
#define LT_ARCH_X86_64
#define LT_ENDIAN_LITTLE

// x86_32
#elif defined(__i386__) || defined(_M_IX86_)
#define LT_ARCH_FAMILY_X86
#define LT_ARCH_X86_32
#define LT_ENDIAN_LITTLE

#endif


#endif /* LTOS_LTARCH_H_ */
