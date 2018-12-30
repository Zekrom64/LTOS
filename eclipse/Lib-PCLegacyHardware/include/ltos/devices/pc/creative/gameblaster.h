/*
 * gameblaster.h
 *
 *  Created on: Dec 30, 2018
 *      Author: Zekrom_64
 */

#ifndef LTOS_DEVICES_PC_CREATIVE_GAMEBLASTER_H_
#define LTOS_DEVICES_PC_CREATIVE_GAMEBLASTER_H_


#include <ltos/lttypes.h>

namespace ltos::devices::pc::creative {

	class GameBlaster {
	public:
		const uint16_t base;
		GameBlaster(uint16_t b) : base(b) {}
	};

}


#endif /* LTOS_DEVICES_PC_CREATIVE_GAMEBLASTER_H_ */
