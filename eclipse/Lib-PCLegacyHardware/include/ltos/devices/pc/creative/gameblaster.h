/*
 * gameblaster.h
 *
 *  Created on: Dec 30, 2018
 *      Author: Zekrom_64
 */

#ifndef LTOS_DEVICES_PC_CREATIVE_GAMEBLASTER_H_
#define LTOS_DEVICES_PC_CREATIVE_GAMEBLASTER_H_


#include <ltos/lttypes.h>
#include <ltos/api/arch/x86/asm.h>

namespace ltos::devices::pc::creative {

	class GameBlaster {
	public:
		const uint16_t base;


		void write(uint8_t off, uint8_t value) {
			ltos::x86::X86::outb(base + off, value);
		}

		void reset(bool enableAll, bool resetAll) {
			write(0x01, 0x1C0);
			write(0x00, (enableAll ? 0x02 : 0) | (resetAll ? 0x01 : 0));
		}

		struct Voice {
			GameBlaster& gb;
			const uint8_t voiceNumber;

			void write(uint8_t reg, uint8_t data) {
				if (voiceNumber < 6) {
					gb.write(0x01, reg);
					gb.write(0x00, data);
				} else {
					gb.write(0x03, reg);
					gb.write(0x02, data);
				}
			}

			void setVolume(uint8_t right, uint8_t left) {
				write(0x00 + (voiceNumber % 6), (right << 4) | (left & 0xF));
			}

			enum Notes {
				NOTE_A = 3,
				NOTE_A_SHARP = 31,
				NOTE_B = 58,
				NOTE_C = 83,
				NOTE_C_SHARP = 107,
				NOTE_D = 130,
				NOTE_D_SHARP = 151,
				NOTE_E = 172,
				NOTE_F = 191,
				NOTE_F_SHARP = 209,
				NOTE_G = 226,
				NOTE_G_SHARP = 242
			};

			void setFrequency(uint8_t freq) {
				write(0x08 + (voiceNumber % 6), freq);
			}

			Voice(GameBlaster& ref, uint8_t num) : gb(ref), voiceNumber(num) {}
		};
		Voice voices[12];

		enum OctavePair {
			OCTAVE_1_2 = 0,
			OCTAVE_3_4 = 1,
			OCTAVE_5_6 = 2,
			OCTAVE_7_8 = 3,
			OCTAVE_9_10 = 4,
			OCTAVE_11_12 = 5
		};

		void setOctave(OctavePair pair, uint8_t pair2, uint8_t pair1) {
			if (pair < 3) {
				write(0x01, 0x10 + (pair % 3));
				write(0x00, (pair2 << 4) | (pair1 & 0xF));
			} else {
				write(0x03, 0x10 + (pair % 3));
				write(0x02, (pair2 << 4) | (pair1 & 0xF));
			}
		}

		enum NoiseFrequency {
			NOISE_28KHZ = 0,
			NOISE_14KHZ = 1,
			NOISE_6_8KHZ = 2
		};

		void setNoiseGen12(NoiseFrequency gen2, NoiseFrequency gen1) {
			write(0x01, 0x16);
			write(0x00, (gen2 << 4) | gen1);
		}

		void setNoiseGen34(NoiseFrequency gen4, NoiseFrequency gen3) {
			write(0x03, 0x16);
			write(0x02, (gen4 << 4) | gen3);
		}

		enum NoiseEnableMask {
			NOISEEN_1_THRU_6 = 0,
			NOISEEN_7_THRU_12
		};

		void enableNoise(NoiseEnableMask mask, uint8_t voices) {
			if (mask == NOISEEN_7_THRU_12)  {
				write(0x03, 0x15);
				write(0x02, voices);
			} else {
				write(0x01, 0x15);
				write(0x00, voices);
			}
		}

		GameBlaster(uint16_t b) : base(b), voices{{*this,0},{*this,1},{*this,2},{*this,3},{*this,4},{*this,5}} {}
	};

}


#endif /* LTOS_DEVICES_PC_CREATIVE_GAMEBLASTER_H_ */
