/*
 * sse.h
 *
 *  Created on: Nov 9, 2018
 *      Author: Zekrom_64
 */

#ifndef LTOS_API_ARCH_X86_CPU_SSE_H_
#define LTOS_API_ARCH_X86_CPU_SSE_H_


#include <xmmintrin.h>
#include "ltos/ltvecmath.h"

namespace ltos::x86 {

	typedef __m128 XMM;

	namespace SSE {

		// (X,Y,Z,W)float
		namespace Vec4f {

			enum Field {
				X = 0,
				Y = 1,
				Z = 2,
				W = 3,

				R = 0,
				G = 1,
				B = 2,
				A = 3,

				U = 0,
				V = 1,
				S = 2,
				T = 3
			};

			// Load/Store
			static inline XMM load(float vector[4]) {
				return (vector & 0xF) == 0 ? _mm_load_ps(vector) : _mm_loadu_ps(vector);
			}
			static inline XMM load(lt_vec4f vec) {
				return load(vec.elems);
			}

			static inline XMM load(float scalar) {
				return _mm_load1_ps(&scalar);
			}

			static inline XMM load(float x, float y, float z, float w) {
				return _mm_setr_ps(x, y, z, w);
			}

			static inline void store(register XMM x, float vector[4]) {
				if ((vector & 0xF) == 0) _mm_store_ps(vector, x);
				else _mm_storeu_ps(vector, x);
			}

			// Register-Register Moves

			template<int SRC0, int SRC1, int SRC2, int SRC3>
			static inline XMM shuffle(register XMM x, register XMM y) {
				x = _mm_shuffle_ps(x, y, _MM_SHUFFLE(SRC3, SRC2, SRC1, SRC0));
			}

			template<int SRC0, int SRC1, int SRC2, int SRC3>
			static inline XMM swizzle(register XMM x) {
				return _mm_shuffle_ps(x, x, _MM_SHUFFLE(SRC3, SRC2, SRC1, SRC0));
			}

			// Arithmetic
			static inline XMM add(register XMM x, register XMM y) {
				return _mm_add_ps(x, y);
			}
			static inline XMM sub(register XMM x, register XMM y) {
				return _mm_sub_ps(x, y);
			}
			static inline XMM mul(register XMM x, register XMM y) {
				return _mm_mul_ps(x, y);
			}
			static inline XMM div(register XMM x, register XMM y) {
				return _mm_div_ps(x, y);
			}
			static inline XMM min(register XMM x, register XMM y) {
				return _mm_min_ps(x, y);
			}
			static inline XMM max(register XMM x, register XMM y) {
				return _mm_max_ps(x, y);
			}

			// Advanced Arithmetic
			static inline XMM sqrt(register XMM x) {
				return _mm_sqrt_ps(x);
			}
			static inline XMM inv(register XMM x) {
				return _mm_rcp_ps(x);
			}
			static inline XMM invSqrt(register XMM x) {
				return _mm_rsqrt_ps(x);
			}

			// Bitwise operations
			static inline XMM _and(register XMM x, register XMM y) {
				return _mm_and_ps(x, y);
			}
			static inline XMM _or(register XMM x, register XMM y) {
				return _mm_or_ps(x, y);
			}
			static inline XMM _xor(register XMM x, register XMM y) {
				return _mm_xor_ps(x, y);
			}
			static inline XMM mask(register XMM x, register XMM y) {
				return _mm_andnot_ps(x, y);
			}

		}

	}

}


#endif /* LTOS_API_ARCH_X86_CPU_SSE_H_ */
