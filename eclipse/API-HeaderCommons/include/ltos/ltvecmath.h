/*
 * ltvecmath.h
 *
 *  Created on: Nov 8, 2018
 *      Author: Zekrom_64
 */

#ifndef LTOS_LTVECMATH_H_
#define LTOS_LTVECMATH_H_


#include "ltmacros.h"

namespace ltos {

LT_PACK_PREFIX
template<typename T, unsigned int N>
struct lt_vec {
	T elems[N];
} LT_PACK_ATTRIB;
LT_PACK_SUFFIX

LT_PACK_PREFIX
template<typename T>
struct lt_vec<T,1> {
	union {
		T elems[1];
		T value;
	};
} LT_PACK_ATTRIB;
LT_PACK_SUFFIX

LT_PACK_PREFIX
template<typename T>
struct lt_vec<T,2> {
	union {
		T elems[2];
		struct {
			T x;
			T y;
		};
	};
} LT_PACK_ATTRIB;
LT_PACK_SUFFIX

LT_PACK_PREFIX
template<typename T>
struct lt_vec<T,3> {
	union {
		T elems[3];
		struct {
			T x;
			T y;
			T z;
		};
		struct {
			T r;
			T g;
			T b;
		};
	};
} LT_PACK_ATTRIB;
LT_PACK_SUFFIX

LT_PACK_PREFIX
template<typename T>
struct lt_vec<T,4> {
	union {
		T elems[4];
		struct {
			T x;
			T y;
			T z;
			T w;
		};
		struct {
			T r;
			T g;
			T b;
			T a;
		};
	};
} LT_PACK_ATTRIB;
LT_PACK_SUFFIX

template<typename T>
using lt_scalar = lt_vec<T,1>;
template<typename T>
using lt_vec2 = lt_vec<T,2>;
template<typename T>
using lt_vec3 = lt_vec<T,3>;
template<typename T>
using lt_vec4 = lt_vec<T,4>;

typedef lt_vec2<int32_t> lt_vec2i;
typedef lt_vec2<uint32_t> lt_vec2u;
typedef lt_vec2<float> lt_vec2f;
typedef lt_vec2<double> lt_vec2d;

typedef lt_vec3<uint8_t> lt_vec3b;
typedef lt_vec3<int32_t> lt_vec3i;
typedef lt_vec3<uint32_t> lt_vec3u;
typedef lt_vec3<float> lt_vec3f;
typedef lt_vec3<double> lt_vec3d;

typedef lt_vec4<uint8_t> lt_vec4b;
typedef lt_vec4<int32_t> lt_vec4i;
typedef lt_vec4<uint32_t> lt_vec4u;
typedef lt_vec4<float> lt_vec4f;
typedef lt_vec4<double> lt_vec4d;

typedef lt_vec3b lt_color3b;
typedef lt_vec4b lt_color4b;
typedef lt_vec3f lt_color3f;
typedef lt_vec4f lt_color4f;

}


#endif /* LTOS_LTVECMATH_H_ */
