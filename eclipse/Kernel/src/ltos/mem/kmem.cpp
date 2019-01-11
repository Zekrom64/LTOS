/*
 * kmem.cpp
 *
 *  Created on: Jan 9, 2019
 *      Author: Zekrom_64
 */

#include "ltos/mem/kmem.h"
#include "mmu/mmu.h"
#include "../init.h"
#include "../global.h"

MMU::PageTable* kernelPageTable;

void kinit_kmem() {
	kernelPageTable = MMU::buildTable(KernelGlobals::malloc_startup);
}

ltos::Mallocator* ltos::KernelMemory::kernelAllocator() {

}

ltos::Mallocator* ltos::KernelMemory::constAllocator() {

}

size_t ltos::KernelMemory::getPageSize() {
	return MMU::getGranularity();
}

void* ltos::KernelMemory::setProtections(void* ptr, size_t size, bool user, ltos::KernelMemory::Protections prot, bool copy) {

}
