/*
 * mmu.h
 *
 *  Created on: Jan 9, 2019
 *      Author: Zekrom_64
 */

#ifndef LTOS_MEM_MMU_H_
#define LTOS_MEM_MMU_H_


#include "ltos/mem/kmem.h"

class MMU {
public:
	class PageTable {
	public:
		virtual ~PageTable();
		virtual void bind();
	};
	static size_t getGranularity();
	static PageTable* buildTable(ltos::Mallocator* alloc);
};


#endif /* LTOS_MEM_MMU_H_ */
