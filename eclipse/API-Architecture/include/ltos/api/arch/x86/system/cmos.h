/*
 * cmos.h
 *
 *  Created on: Dec 24, 2018
 *      Author: Zekrom_64
 */

#ifndef LTOS_API_ARCH_X86_SYSTEM_CMOS_H_
#define LTOS_API_ARCH_X86_SYSTEM_CMOS_H_


#include <ltos/lttypes.h>
#include <ltos/ltarch.h>

#ifdef LT_ARCH_FAMILY_X86

#include "../asm.h"

namespace ltos::x86 {

	class CMOS {
	public:
		enum CMOSRegister {
			REG_STATUS_A = 0x0A,
			REG_STATUS_B = 0x0B,
			REG_STATUS_C = 0x0C,
			REG_FLOPPY_TYPES = 0x10
		};

		enum DriveType {
			DRIVETYPE_NONE = 0,
			DRIVETYPE_360K_5IN = 1,
			DRIVETYPE_1_2M_5IN = 2,
			DRIVETYPE_720K_3IN = 3,
			DRIVETYPE_1_44M_3IN = 4,
			DRIVETYPE_2_88M_2IN = 5
		};

		static uint8_t read(CMOSRegister reg);

		struct RTC {
			uint8_t seconds;
			uint8_t minutes;
			uint8_t hours;
			uint8_t day;
			uint8_t month;
			uint8_t year;
			uint8_t century;
		};

		/** Reads the RTC from the CMOS. An offset to the century register
		 * may be provided, or may be -1 in which case the century value
		 * will be set to 0.
		 *
		 * @param rtc RTC value
		 * @param centuryRegOffset Offset to century register, or -1
		 */
		static void readRTC(RTC* rtc, int centuryRegOffset);
	};

}

#endif

#endif /* LTOS_API_ARCH_X86_SYSTEM_CMOS_H_ */
