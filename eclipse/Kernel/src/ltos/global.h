/*
 * global.h
 *
 *  Created on: Jan 9, 2019
 *      Author: Zekrom_64
 */

#ifndef LTOS_MEM_GLOBAL_H_
#define LTOS_MEM_GLOBAL_H_


#include "ltos/ltos.h"
#include "ltos/mem/mem.h"

class KernelGlobals {
public:
	KernelStartInfo* start_info;
	ltos::Kernel* kernel;

	ltos::Mallocator* malloc_startup;
};


#endif /* LTOS_MEM_GLOBAL_H_ */
