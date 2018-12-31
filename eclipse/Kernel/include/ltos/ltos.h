/*
 * ltos.h
 *
 *  Created on: Dec 30, 2018
 *      Author: Zekrom_64
 */

#ifndef LTOS_LTOS_H_
#define LTOS_LTOS_H_


#include <ltos/lttypes.h>
#include <ltos/ltmacros.h>
#include <ltos/ltarch.h>
#include <ltos/ltcpp.h>

struct KernelStartInfo {
	struct MemoryBlock {
		uint64_t base;
		uint64_t size;
	};
	MemoryBlock* memoryBlocks;
	uint32_t numMemoryBlocks;
	void (*consoleLogCallback)(const char* str);
	struct MiscInfo {
		uint32_t type;
		uint32_t flags;
		uint64_t values[7];
	};
	MiscInfo* miscInfo;
	uint32_t numMiscInfo;
};

extern "C" LT_API void kernel_main(KernelStartInfo* startInfo);

#include "mem/kmem.h"

namespace ltos {

	class Kernel : public LTObject {
	public:
		virtual KernelMemory* memory();
	};

}

#endif /* LTOS_LTOS_H_ */
