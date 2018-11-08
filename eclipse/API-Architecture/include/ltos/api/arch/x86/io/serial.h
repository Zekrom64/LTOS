/*
 * serial.h
 *
 *  Created on: Oct 18, 2018
 *      Author: Zekrom_64
 */

#ifndef LTOS_API_ARCH_X86_SERIAL_H_
#define LTOS_API_ARCH_X86_SERIAL_H_


#include "ltos/api/port/serial.h"

namespace ltos::x86 {

class StandardSerialPort : public ltos::SerialPort {
public:
	enum Base {
		COM1 = 0x3F8,
		COM2 = 0x2F8,
		COM3 = 0x3E8,
		COM4 = 0x2E8
	};
	static bool detect(Base b);

	const uint16_t ioBase;
	StandardSerialPort(uint16_t base) : ioBase(base) { }

	virtual void write(uint8_t data);
	virtual uint8_t read();

	virtual bool readyWrite();
	virtual bool readyRead();

	virtual void setNumBits(uint8_t nbits);
	virtual void setStopBits(StopBits stop);
	virtual void setParity(Parity p);
	virtual void setBaudRate(uint32_t baud);
};

}


#endif /* LTOS_API_ARCH_X86_SERIAL_H_ */
