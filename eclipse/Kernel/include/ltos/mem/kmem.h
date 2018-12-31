/*
 * kmem.h
 *
 *  Created on: Dec 31, 2018
 *      Author: Zekrom_64
 */

#ifndef LTOS_MEM_KMEM_H_
#define LTOS_MEM_KMEM_H_

#include "mem.h"

namespace ltos {

	class KernelMemory : public LTObject {
	public:
		virtual Mallocator* kernelAllocator();
		virtual Mallocator* constAllocator();
		enum Protections {
			PROT_READ_WRITE,
			PROT_READ,
			PROT_EXEC_READ,
			PROT_EXEC
		};
		void
	};

}


#endif /* LTOS_MEM_KMEM_H_ */
