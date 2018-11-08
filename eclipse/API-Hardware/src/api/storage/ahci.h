#ifndef __AHCI_H__
#define __AHCI_H__

#include "ltos/lttypes.h"
#include "ltos/ltmacros.h"


namespace ltos {

	struct volatile AHCIHostControl {
		uint32_t hostCapabilities;
		uint32_t globalHostControl;
		uint32_t interruptStatus;
		uint32_t portsImplemented;
		uint32_t version;
		uint32_t cccControl;
		uint32_t cccPorts;
		uint32_t emLocation;
		uint32_t emControl;
		uint32_t hostCapabilities2;
		uint32_t biosHandoffCtrlStat;
	};
	
	struct volatile AHCIPort {
		uint64_t commandListBase;
		uint64_t fisReceiveBase;
		uint32_t interruptStatus;
		uint32_t interruptEnable;
		uint32_t commandStatus;
		
	};
	
	struct volatile AHCIHostAdapter {
		AHCIHostControl control;
		uint8_t reserved0[116];
		uint8_t vendorSpecific[96];
		
	};

}


#endif

