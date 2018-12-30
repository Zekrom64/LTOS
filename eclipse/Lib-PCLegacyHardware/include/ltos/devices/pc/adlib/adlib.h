/*
 * adlib.h
 *
 *  Created on: Dec 25, 2018
 *      Author: Zekrom_64
 */

#ifndef LTOS_DEVICES_PC_ADLIB_ADLIB_H_
#define LTOS_DEVICES_PC_ADLIB_ADLIB_H_

#include <ltos/lttypes.h>
#include <ltos/api/arch/x86/asm.h>

namespace ltos::device::pc::adlib {

	class AdLib {
	public:
		static constexpr uint16_t PORT_ADDRESS = 0x388;
		static constexpr uint16_t PORT_DATA = 0x389;

		static constexpr int channelOpOffset(int channel, int op) {
			int base = 0;
			if (channel < 4) {
				base = 0;
			} else if (channel < 8) {
				base = 0x08;
			} else {
				base = 0x10;
			}
			return base + channel + ((op - 1) * 3);
		}

		const uint16_t address, data;

		void write(uint8_t reg, uint8_t value) {
			ltos::x86::X86::outb(address, reg);
			for(int i = 0; i < 6; i++) ltos::x86::X86::inb(address);

			ltos::x86::X86::outb(data, value);
			for(int i = 0; i < 35; i++) ltos::x86::X86::inb(address);
		}
		uint8_t read(uint8_t reg) {
			ltos::x86::X86::outb(address, reg);
			for(int i = 0; i < 6; i++) ltos::x86::X86::inb(address);

			return ltos::x86::X86::inb(data);
		}

		void enableWaveformCtrl(bool enable) {
			write(0x01, enable ? 0x20 : 0);
		}

		void setTimer1(uint8_t ticks) {
			write(0x02, 255 - ticks);
		}
		void setTimer2(uint8_t ticks) {
			write(0x03, 255 - ticks);
		}

		void setTimerCtrl(bool reset, bool t1Disable, bool t2Disable, bool t1Stop, bool t2Stop) {
			write(0x04,
				(reset ?     0x80 : 0) |
				(t1Disable ? 0x40 : 0) |
				(t2Disable ? 0x20 : 0) |
				(t1Stop ?    0x01 : 0) |
				(t2Stop ?    0x02 : 0)
			);
		}

		void setCSM_KBSplit(bool csm, bool kbsplit) {
			write(0x08, (csm ? 0x80 : 0) | (kbsplit ? 0x40 : 0));
		}

		void setSynthParams(bool amDepth, bool vibratoDepth, bool rhythmEnable, bool bassEnable, bool snareEnable,
				bool tomTomEnable, bool cymbalEnable, bool hiHatEnable) {
			write(0xBD,
				(amDepth ?      0x80 : 0) |
				(vibratoDepth ? 0x40 : 0) |
				(rhythmEnable ? 0x20 : 0) |
				(bassEnable ?   0x10 : 0) |
				(snareEnable ?  0x08 : 0) |
				(tomTomEnable ? 0x04 : 0) |
				(cymbalEnable ? 0x02 : 0) |
				(hiHatEnable ?  0x01 : 0)
			);
		}

		struct Channel {
			AdLib& adlib;
			int8_t channelNumber;

			struct Operator {
				Channel& ch;
				int8_t operatorNumber;

				enum FrequencyMultiple {
					FREQ_MINUS_ONE_OCTAVE = 0,
					FREQ_NORMAL = 1,
					FREQ_PLUS_ONE_OCTAVE = 2,
					FREQ_PLUS_ONE_AND_ONE_FIFTH_OCTAVE = 3,
					FREQ_PLUS_TWO_OCTAVES = 4,
					FREQ_PLUS_TWO_OCTAVES_AND_MAJOR_THIRD = 5,
					FREQ_PLUS_TWO_OCTAVES_AND_ONE_FIFTH = 6,
					FREQ_PLUS_TWO_OCTAVES_AND_MINOR_SEVENTH = 7,
					FREQ_PLUS_THREE_OCTAVES = 8,
					FREQ_PLUS_THREE_OCTAVES_AND_MAJOR_SECOND = 9,
					FREQ_PLUS_THREE_OCTAVES_AND_MAJOR_THIRD = 0xA,
					FREQ_PLUS_THREE_OCTAVES_AND_ONE_FIFTH = 0xC,
					FREQ_PLUS_THREE_OCTAVES_AND_MAJOR_SEVENTH = 0xE
				};

				void setModulation(bool ampMod, bool vibrato, bool disableDecay, bool kbScale, FrequencyMultiple mult) {
					ch.adlib.write(0x20 + channelOpOffset(ch.channelNumber, operatorNumber),
						(ampMod ?       0x80 : 0) |
						(vibrato ?      0x40 : 0) |
						(disableDecay ? 0x20 : 0) |
						(kbScale ?      0x10 : 0) | mult
					);
				}

				enum ScalingLevel {
					SCALE_NONE = 0,
					SCALE_PLUS_1_5DB = 2,
					SCALE_PLUS_3DB = 1,
					SCALE_PLUS_6DB = 3
				};

				void setLevel(ScalingLevel scaling, uint8_t level) {
					ch.adlib.write(0x40 + channelOpOffset(ch.channelNumber, operatorNumber),
						(scaling << 6) |
						(level & 0x3F)
					);
				}

				void setAttackDecay(uint8_t attack, uint8_t decay) {
					ch.adlib.write(0x60 + channelOpOffset(ch.channelNumber, operatorNumber),
						(attack << 4) |
						(decay & 0xF)
					);
				}

				void setSustainRelease(uint8_t sustain, uint8_t release) {
					ch.adlib.write(0x80 + channelOpOffset(ch.channelNumber, operatorNumber),
						(sustain << 4) |
						(release & 0xF)
					);
				}

				enum Waveform {
					WAVE_SINE = 0,
					WAVE_HALF_SINE = 1,
					WAVE_POSITIVE_SINE = 2,
					WAVE_SINE_SAWTOOTH = 3
				};

				void setWaveform(Waveform wv) {
					ch.adlib.write(0xE0 + channelOpOffset(ch.channelNumber, operatorNumber), wv);
				}

				Operator(Channel& ref, int8_t num) : ch(ref), operatorNumber(num) {}
			} op1, op2;

			void setFeedback(uint8_t strength, bool noModulation) {
				adlib.write(
					(uint8_t)(0xC0 + (channelNumber - 1)),
					(uint8_t)(((strength << 1) & 0xE) | (noModulation ? 1 : 0))
				);
			}

			void setFrequency(bool enable, uint8_t octave, uint16_t freqNum) {
				adlib.write(
					(uint8_t)(0xA0 + (channelNumber - 1)),
					(uint8_t)freqNum
				);
				adlib.write(
					(uint8_t)(0xB0 + (channelNumber - 1)),
					(uint8_t)((enable ? 0x20 : 0) | ((octave << 2) & 0x1C) | ((freqNum >> 8) & 0x3))
				);
			}

			Channel(AdLib& ref, int8_t num) : adlib(ref), channelNumber(num), op1(*this, 1), op2(*this, 2) {}
		} channels[9];

		AdLib(uint16_t a, uint16_t d) : address(a), data(d), channels{{*this,1},{*this,2},{*this,3},{*this,4},{*this,5},{*this,6},{*this,7},{*this,8},{*this,9}} {}
	};

}


#endif /* LTOS_DEVICES_PC_ADLIB_ADLIB_H_ */
