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
		/** Destroys the allocator, often this is a no-op. */
		virtual ~Mallocator() {}
		/** Allocates a memory block with of the given size.
		 *
		 * @param size Size of memory to allocate
		 */
		virtual void* malloc(size_t size)=0;
		/** Reallocates a block of memory to a new size. This will copy
		 * the contents of the old memory to the new memory.
		 *
		 * @param old Old memory
		 * @param newsize New memory size
		 */
		virtual void* realloc(void* old, size_t newsize)=0;
		/** Frees a block of allocated memory.
		 *
		 * @param ptr Memory to free
		 */
		virtual void free(void* ptr)=0;

		/** Gets the minimum allocation size available. Allocations below this
		 * size will be rounded up to this value.
		 *
		 * @return Minimum allocation size
		 */
		virtual size_t getMinAllocSize()=0;
		/** Gets the minimum allocation byte alignment.
		 *
		 * @return Minimum allocation alignment
		 */
		virtual size_t getMinAllocAlign()=0;

		/** Idential to a regular malloc, but the allocated memory will always be
		 * aligned to the given alignment, even if reallocated. This will be
		 * rounded up to the minimum alignment allowed for this allocator. There
		 * is a reasonable upper limit to the alignment (around 4-8KiB).
		 *
		 * @param size Size of memory to allocate
		 * @param align Alignment of allocated memory
		 */
		virtual void* malloc_aligned(size_t size, size_t align)=0;
	};

}


#endif /* LTOS_MEM_MEM_H_ */
