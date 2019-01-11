/*
 * ltmacros.h
 *
 *  Created on: Oct 18, 2018
 *      Author: Zekrom_64
 */

#ifndef LTOS_LTMACROS_H_
#define LTOS_LTMACROS_H_


#include <functional>


/* ----------------------
 * | Compiler Standards |
 * ---------------------- */

#if defined(__MINGW32__)
#define LT_COMPILER_MINGW
#define LT_COMPILER_FAMILY_GNUC

#elif defined(__INTEL_COMPILER)
#define LT_COMPILER_INTEL
#if defined(__GNUC__)
#define LT_COMPILER_FAMILY_GNUC
#elif defined(_MSC_VER)
#define LT_COMPILER_FAMILY_MSVC
#else
#define LT_COMPILER_FAMILY_OTHER
#endif

#elif defined(__SUNPRO_C)
#define LT_COMPILER_ORACLE
#define LT_COMPILER_FAMILY_GNUC

#elif defined(_MSC_VER)
#define LT_COMPILER_MSVC
#define LT_COMPILER_FAMILY_MSVC

#elif defined(__clang__)
#define LT_COMPILER_CLANG
#define LT_COMPILER_FAMILY_GNUC

#elif defined(__GNUC__)
#define LT_COMPILER_GCC
#define LT_COMPILER_FAMILY_GNUC

#endif

#define LT_CPLUSPLUS_97 199711L
#define LT_CPLUSPLUS_11 201103L
#define LT_CPLUSPLUS_14 201402L
#define LT_CPLUSPLUS_17 201703L

/* ---------------------------
 * | Compiler-Specific Tools |
 * --------------------------- */

#ifdef __GNUC__

#define LT_MSG(...) _Pragma(LT_STRINGIFY(message __VA_ARGS__))
#define LT_PACK_PREFIX
#define LT_PACK_ATTRIB __attribute__((__packed__))
#define LT_PACK_SUFFIX

#define LT_NAKED_PREFIX __attribute__((naked))

#elif defined(_MSC_VER)

#define LT_MSG(...) __pragma(message(LT_STRINGIFY(__VA_ARGS__)))
#define LT_PACK_PREFIX __pragma(pack(push,1))
#define LT_PACK_ATTRIB
#define LT_PACK_SUFFIX __pragma(pack(pop))

#endif

#if defined(LT_COMPILER_FAMILY_MSVC) || defined(LT_COMPILER_MINGW)

#define LT_EXPORT __declspec(dllexport)
#define LT_IMPORT __declspec(dllimport)

#elif defined(LT_COMPILER_FAMILY_GNUC)

#define LT_EXPORT extern
#define LT_IMPORT extern

#endif

#ifdef LT_BUILD
#define LT_API LT_EXPORT
#else
#define LT_API LT_IMPORT
#endif

/* ----------------------
 * | Standard Utilities |
 * ---------------------- */

#define LT_STRINGIFY2(...) #__VA_ARGS__
#define LT_STRINGIFY(...) LT_STRINGIFY2(__VA_ARGS__)

template<unsigned... digits>
struct __LT_TO_CHARS__ { static const char value[]; };
template<unsigned... digits>
constexpr char __LT_TO_CHARS__<digits...>::value[] = {('0'+digits)...,0};
template<unsigned rem, unsigned... digits>
struct __LT_EXPLODE__ : __LT_EXPLODE__<rem / 10, rem % 10, digits...> {};
template<unsigned... digits>
struct __LT_EXPLODE__<0, digits...> : __LT_TO_CHARS__<digits...> {};

template<unsigned num>
struct LT_CONSTEXPR_NUM2STR : __LT_EXPLODE__<num> {};

#if __cplusplus >= LT_CPLUSPLUS_14
#define LT_DEPRECATE(MSG) [[deprecated(MSG)]]
#else
#define LT_DEPRECATE(MSG)
//#define LT_DEPRECATE(MSG) LT_MSG("deprecated: " LT_STRINGIFY(MSG))
#endif


/* --------------------
 * | Inline Functions |
 * -------------------- */

template<typename T>
inline void LT_INLINE_ZERO(T X) {
	for(unsigned int i = 0; i < sizeof(X); i++) *(((char*)&X)+i) = 0;
}

template<typename T>
inline void LT_INLINE_ZERO_ARRAY(T X, unsigned int S) {
	for(unsigned int i = 0; i < sizeof(X[0]) * S; i++) *(((char*)X)+i) = 0;
}

inline bool LT_INLINE_STRNCMP(const char* s1, const char* s2, unsigned int n) {
	while(n-- > 0) if (s1[n] != s2[n]) return false;
	return true;
}

template<typename T>
inline void LT_STRING_SWITCH(register const T* str, register const T** matches, register size_t nMatches,
		std::function<void()>* cases, std::function<void()>* defCase) {
	register T key;
	bool valid[nMatches];
	for(size_t i = 0; i < nMatches; i++) valid[i] = true;
	do {
		key = *(str++);
		bool matched = false;
		for(register size_t i = 0; i < nMatches; i++) {
			if (!valid[i]) continue;
			T val = *(matches[i]++);
			if (key != val) valid[i] = false;
			matched = true;
		}
		if (!matched) {
			if (defCase != nullptr) (*defCase)();
			return;
		}
	} while(key != 0);
	for(register size_t i = 0; i < nMatches; i++) {
		if(valid[i]) {
			cases[i]();
			return;
		}
	}
}

template<typename X, typename Y>
inline Y LT_BITWISE_CAST(X x) {
	union {
		X x;
		Y y;
	} bitunion;
	bitunion.x = x;
	return bitunion.y;
}

/* ------------------------
 * | Compile-Time Asserts |
 * ------------------------ */

#define LT_ASSERT_SIZEOF(S,V) static_assert(sizeof(S) == V, "struct \"" LT_STRINGIFY(S) "\": Size is not " LT_STRINGIFY(V));

#define LT_ASSERT_OFFSETOF(S,F,O) static_assert(offsetof(S,F) == O, "struct \"" LT_STRINGIFY(S) "\": Offset of field \"" \
		LT_STRINGIFY(F) "\" is not " LT_STRINGIFY(O));


#endif /* LTOS_LTMACROS_H_ */
