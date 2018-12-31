/*
 * mem.h
 *
 *  Created on: Dec 31, 2018
 *      Author: Zekrom_64
 */

#ifndef LTOS_MEM_MEM_H_
#define LTOS_MEM_MEM_H_


#include "../ltos.h"

namespace ltos {

	class Mallocator {
	public:
		virtual ~Mallocator() {}
		virtual void* malloc(size_t size)=0;
		virtual void* realloc(void* old, size_t newsize)=0;
		virtual void free(void* ptr)=0;
	};

}


#endif /* LTOS_MEM_MEM_H_ */
