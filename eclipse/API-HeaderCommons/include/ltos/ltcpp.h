/*
 * ltcpp.h
 *
 *  Created on: Nov 7, 2018
 *      Author: Zekrom_64
 */

#ifndef LTOS_LTCPP_H_
#define LTOS_LTCPP_H_


#include "lttypes.h"
#include "ltmacros.h"

namespace ltos {

template<typename T>
struct __LT_IFACE__ {
	static lt_cstring name() { return nullptr; }
};

#define LT_DECLARE_IFACE(T) \
	template<> \
	struct __LT_IFACE__<T> { \
		static lt_cstring name() { return LT_STRINGIFY(T); } \
	};

class LTObject {
protected:
	virtual ~LTObject(){}
public:
	virtual void* queryInterface(lt_cstring name) { return nullptr; }
	template<typename T>
	inline T* queryInterface() {
		return (T*)queryInterface(__LT_IFACE__<T>::name());
	}
};

}


#endif /* LTOS_LTCPP_H_ */
