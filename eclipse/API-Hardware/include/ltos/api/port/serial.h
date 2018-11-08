/*
 * serial.h
 *
 *  Created on: Oct 18, 2018
 *      Author: Zekrom_64
 */

#ifndef LTOS_API_PORT_SERIAL_H_
#define LTOS_API_PORT_SERIAL_H_


#include "ltos/lttypes.h"

namespace ltos {

class SerialPort {
protected:
	uintptr_t handle;
public:
	virtual ~SerialPort(){}

	virtual void write(uint8_t data)=0;
	virtual uint8_t read()=0;

	virtual bool readyWrite()=0;
	virtual bool readyRead()=0;

	virtual void setNumBits(uint8_t nbits)=0;
	enum StopBits {
		STOP_1 = 0,
		STOP_2
	};
	virtual void setStopBits(StopBits stop)=0;
	enum Parity {
		PARITY_NONE = 0,
		PARITY_ODD,
		PARITY_EVEN,
		PARITY_MARK,
		PARITY_SPACE
	};
	virtual void setParity(Parity p)=0;
	virtual void setBaudRate(uint32_t baud)=0;
};

}


#endif /* LTOS_API_PORT_SERIAL_H_ */
