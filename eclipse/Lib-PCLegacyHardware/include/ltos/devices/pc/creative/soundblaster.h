/*
 * soundblaster.h
 *
 *  Created on: Dec 25, 2018
 *      Author: Zekrom_64
 */

#ifndef LTOS_DEVICES_PC_CREATIVE_SOUNDBLASTER_H_
#define LTOS_DEVICES_PC_CREATIVE_SOUNDBLASTER_H_

#include <ltos/lttypes.h>
#include <ltos/api/arch/x86/asm.h>
#include "../adlib/adlib.h"

namespace ltos::device::pc::creative {

	class SoundBlaster {
	public:
		ltos::device::pc::adlib::AdLib fmLeft;
		ltos::device::pc::adlib::AdLib fmRight;
		ltos::device::pc::adlib::AdLib fmBoth;
		const uint16_t baseport;

		struct DSP {
			const SoundBlaster& sb;
			DSP(const SoundBlaster& ref) : sb(ref) {}

			enum DSPPort {
				PORT_RESET = 0x6,
				PORT_DATA = 0xA,
				PORT_COMMAND_STATUS = 0xC,
				PORT_AVAILABLE = 0xE
			};

			inline void write(DSPPort port, uint8_t value) {
				ltos::x86::X86::outb(sb.baseport + port, value);
			}
			inline uint8_t read(DSPPort port) {
				return ltos::x86::X86::inb(sb.baseport + port);
			}

			bool reset() {
				write(PORT_RESET, 1);
				for(int i = 0; i < 100; i++);
				write(PORT_RESET, 0);

				bool flag = true;
				for(int i = 0; i < 1000 && flag; i++) {
					if (read(PORT_AVAILABLE) & 0x80) flag = false;
				}
				if (flag) return false;

				flag = true;
				for(int i = 0; i < 1000 && flag; i++) {
					if (read(PORT_DATA) == 0xAA) flag = false;
				}
				if (flag) return false;

				return true;
			}

			inline void setDMAEnable(bool enable) {
				write(PORT_COMMAND_STATUS, enable ? 0xD4 : 0xD0);
			}

			inline void setSpeakerEnable(bool enable) {
				write(PORT_COMMAND_STATUS, enable ? 0xD1 : 0xD3);
			}

			inline void setDMAFrequency(int32_t freq) {
				write(PORT_COMMAND_STATUS, 0x40);
				write(PORT_COMMAND_STATUS, 256 - (1000000 / freq));
			}

			inline void setDACLevel(uint8_t level) {
				write(PORT_COMMAND_STATUS, 0x10);
				write(PORT_COMMAND_STATUS, level);
			}

			enum DMAFormat {
				FORMAT_8BIT = 0x14,
				FORMAT_4BIT_ADPCM = 0x74,
				FORMAT_4BIT_ADPCM_REF = 0x75,
				FORMAT_2_6BIT_ADPCM = 0x76,
				FORMAT_2_6BIT_ADPCM_REF = 0x77,
				FORMAT_2BIT_ADPCM = 0x16,
				FORMAT_2BIT_ADPCM_REF = 0x17
			};

			inline void prepareDMAtoDAC(DMAFormat format, uint16_t nBytes) {
				nBytes--;
				write(PORT_COMMAND_STATUS, format);
				write(PORT_COMMAND_STATUS, (uint8_t)(nBytes & 0xFF));
				write(PORT_COMMAND_STATUS, (uint8_t)(nBytes >> 8));
			}

			inline void prepareDMAfromADC(uint16_t nBytes) {
				nBytes--;
				write(PORT_COMMAND_STATUS, 0x24);
				write(PORT_COMMAND_STATUS, (uint8_t)(nBytes & 0xFF));
				write(PORT_COMMAND_STATUS, (uint8_t)(nBytes >> 8));
			}
		} dsp;

		struct Mixer {
			const SoundBlaster& sb;
			Mixer(const SoundBlaster& ref) : sb(ref) {}

			inline void write(uint8_t reg, uint8_t value) {
				ltos::x86::X86::outb(sb.baseport + 4, reg);
				ltos::x86::X86::outb(sb.baseport + 5, value);
			}

			inline void reset() {
				write(0x00, 0);
			}

			enum InputFilter {
				FILTER_LOW = 0,
				FILTER_HIGH = 1,
				FILTER_NONE = 2
			};
			enum ADCSource {
				ADC_MIC1 = 0,
				ADC_CD = 1,
				ADC_MIC2 = 2,
				ADC_LINE_IN = 3
			};

			inline void setInput(InputFilter filter, ADCSource src) {
				write(0x0C, (filter << 3) | (src << 1));
			}

			inline void setOutput(bool bypassFilter, bool stereo) {
				write(0x0E, (bypassFilter ? 0x20 : 0) | (stereo ? 0x02 : 0));
			}

			inline void setMasterVolume(uint8_t left, uint8_t right) {
				write(0x22, (left << 4) | (right & 0xF));
			}

			inline void setDSPVolume(uint8_t left, uint8_t right) {
				write(0x04, (left << 4) | (right & 0xF));
			}
			inline void setFMVolume(uint8_t left, uint8_t right) {
				write(0x26, (left << 4) | (right & 0xF));
			}
			inline void setCDVolume(uint8_t left, uint8_t right) {
				write(0x28, (left << 4) | (right & 0xF));
			}
			inline void setLineVolume(uint8_t left, uint8_t right) {
				write(0x2E, (left << 4) | (right & 0xF));
			}
			inline void setMicVolume(uint8_t vol) {
				write(0x0A, vol & 0x7);
			}
		} mixer;

		SoundBlaster(uint16_t base) : baseport(base) {
			fmLeft(base, base+1);
			fmRight(base+2, base+3);
			fmBoth(0x388, 0x389);
			dsp(*this);
			mixer(*this);
		}
	};

}


#endif /* LTOS_DEVICES_PC_CREATIVE_SOUNDBLASTER_H_ */
