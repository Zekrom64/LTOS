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
		/** Gets the mallocator for the kernel.
		 *
		 * @return Kernel mallocator
		 */
		virtual Mallocator* kernelAllocator();
		/** Enumeration of memory protection types. */
		enum Protections {
			PROT_READ_WRITE,//!< PROT_READ_WRITE The memory can be read and modified
			PROT_READ,      //!< PROT_READ The memory can only be read
			PROT_EXEC_READ, //!< PROT_EXEC_READ The memory can be read or executed as code
		};
		/** Gets the page size for the current system. The page size is the
		 * minimum granularity for applying memory protections.
		 *
		 * @return The page size for the current system
		 */
		virtual size_t getPageSize();
		/** Gets the minimum allocation size available. Allocations below this
		 *
		 * @return
		 */
		virtual size_t getMinAllocSize();
		/**
		 *
		 * @return
		 */
		virtual size_t getAllocGranularity();
		/** Sets memory protections for a block of memory, returning a pointer to the protected
		 * memory. If a null pointer is passed, the memory is allocated and protections are
		 * applied, the same as calling this function after calling malloc with the same size.
		 * Some memory protection changes may require copying of the memory to a new page, but
		 * this is not necessary if the data in memory can be discarded.
		 *
		 * @param ptr Pointer to memory to change protections
		 * @param size Size of memory to change protections
		 * @param user If the memory should be accessible as user-level
		 * @param prot Memory protections to apply
		 * @param copy If the memory should be copied if necessary
		 * @return Pointer to protected memory
		 */
		virtual void* setProtections(void* ptr, size_t size, bool user, Protections prot, bool copy);
	};

}


#endif /* LTOS_MEM_KMEM_H_ */
