/*
 * parallel.h
 *
 *  Created on: Nov 3, 2018
 *      Author: Zekrom_64
 */

#ifndef LTOS_API_PORT_PARALLEL_H_
#define LTOS_API_PORT_PARALLEL_H_


#include "ltos/lttypes.h"

namespace ltos {

class ParallelPort {
protected:
	uintptr_t handle;
public:
	virtual ~ParallelPort(){}

	enum Status {
		STATUS_BUSY =  0b10000000,
		STATUS_ACK =   0b01000000,
		STATUS_PE =    0b00100000,
		STATUS_SEL =   0b00010000,
		STATUS_ERROR = 0b00001000
	};

	enum Control {
		CONTROL_SELIN =  0b00001000,
		CONTROL_INIT =   0b00000100,
		CONTROL_AUTOF =  0b00000010,
	};

	virtual void write(uint8_t data)=0;
	virtual uint8_t read()=0;

	virtual uint32_t getStatus()=0;
	virtual void setControl(uint32_t control)=0;
};

}


#endif /* LTOS_API_PORT_PARALLEL_H_ */
