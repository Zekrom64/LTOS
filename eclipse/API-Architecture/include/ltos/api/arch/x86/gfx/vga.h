/*
 * vga.h
 *
 *  Created on: Oct 18, 2018
 *      Author: Zekrom_64
 */

#ifndef LTOS_API_ARCH_X86_GFX_VGA_H_
#define LTOS_API_ARCH_X86_GFX_VGA_H_


#include "ltos/lttypes.h"

namespace ltos::x86 {

	struct VGA {
		
		static constexpr uintptr_t BASE = 0xA0000;
	
		/** Enumeration of IO ports used by VGA.
		 *
		 */
		enum VGAPort {
			VGA_PORT_CRTC_ADDRESS = 0x3B4,            //!< VGA_PORT_CRTC_ADDRESS The register address port for the CRT Controller
			VGA_PORT_CRTC_DATA = 0x3B5,               //!< VGA_PORT_CRTC_DATA The register data port for the CRT Controller
			VGA_PORT_INPUT_STATUS1_READ = 0x3BA,      //!< VGA_PORT_INPUT_STATUS1_READ The read port for the #1 Input Status register
			VGA_PORT_FEATURE_CONTROL_WRITE = 0x3BA,   //!< VGA_PORT_FEATURE_CONTROL_WRITE The write port for the Feature Control register
			VGA_PORT_ATTRIBUTE_ADDRESS_DATA = 0x3C0,  //!< VGA_PORT_ATTRIBUTE_ADDRESS_DATA The register address and data write port for the Attribute Controller
			VGA_PORT_ATTRIBUTE_DATA_READ = 0x3C1,     //!< VGA_PORT_ATTRIBUTE_DATA_READ The register read port for the Attribute Controller
			VGA_PORT_INPUT_STATUS0 = 0x3C2,           //!< VGA_PORT_INPUT_STATUS0 The read port for the #0 Input Status register
			VGA_PORT_MISC_OUTPUT_WRITE = 0x3C2,       //!< VGA_PORT_MISC_OUTPUT_WRITE The write port for the Misc. Output register
			VGA_PORT_SEQUENCER_ADDRESS = 0x3C4,       //!< VGA_PORT_SEQUENCER_ADDRESS The register address port for the Sequencer
			VGA_PORT_SEQUENCER_DATA = 0x3C5,          //!< VGA_PORT_SEQUENCER_DATA The register data port for the Sequencer
			VGA_PORT_DAC_STATE = 0x3C7,               //!< VGA_PORT_DAC_STATE The port for reading the DAC State register
			VGA_PORT_DAC_ADDRESS_READ_MODE = 0x3C7,   //!< VGA_PORT_DAC_ADDRESS_READ_MODE The port to write the DAC Address in reading mode
			VGA_PORT_DAC_ADDRESS_WRITE_MODE = 0x3C8,  //!< VGA_PORT_DAC_ADDRESS_WRITE_MODE The port to write the DAC Address in writing mode
			VGA_PORT_DAC_DATA = 0x3C9,                //!< VGA_PORT_DAC_DATA The port to read or write DAC data
			VGA_PORT_FEATURE_CONTROL_READ = 0x3CA,    //!< VGA_PORT_FEATURE_CONTROL_READ The read port for the Feature Control register
			VGA_PORT_MISC_OUTPUT_READ = 0x3CC,        //!< VGA_PORT_MISC_OUTPUT_READ The read port for the Misc. Output register
			VGA_PORT_GRAPHICS_CONTROL_ADDRESS = 0x3CE,//!< VGA_PORT_GRAPHICS_CONTROL_ADDRESS The register address port for the Graphics Controller
			VGA_PORT_GRAPHICS_CONTROL_DATA = 0x3CF,   //!< VGA_PORT_GRAPHICS_CONTROL_DATA The register data port for the Graphics Controller
			VGA_PORT_CRTC_ADDRESS_ALT = 0x3D4,        //!< VGA_PORT_CRTC_ADDRESS_ALT Alternate register address port for the CRT Controller
			VGA_PORT_CRTC_DATA_ALT = 0x3D5,           //!< VGA_PORT_CRTC_DATA_ALT Alternate register data port for the CRT Controller
			VGA_PORT_INPUT_STATUS1_ALT = 0x3DA,       //!< VGA_PORT_INPUT_STATUS1_ALT Alternate read port for the #1 Input Status register
			VGA_PORT_FEATURE_CONTROL_WRITE_ALT = 0x3DA//!< VGA_PORT_FEATURE_CONTROL_WRITE_ALT Alternate write port for the Feature Control register
		};

		/** A VGA palette color is a triad of 6-bit red, green, and blue
		 * values. When accessed through IO ports the 6-bit values are
		 * treated as 8-bit values.
		 */
		struct PaletteColor {
			/** The maximum value a color channel may contain. */
			static const uint8_t COLOR_MAX = 63;
			/** Value of the red channel. */
			uint8_t red;
			/** Value of the green channel. */
			uint8_t green;
			/** Value of the blue channel. */
			uint8_t blue;
		};

		/** The DAC converts 8-bit color values read from video memory into the analog
		 * signals sent to the display. The DAC has an internal palette memory to
		 * do this conversion. The palette is made up of 6-bit triads of red, green
		 * and blue, which are converted between 8-bits when the palette is changed.
		 *
		 * Note: Accessing the palette outside of a blanking interval may cause
		 * flickering, although some adapters may not exhibit this behavior.
		 */
		struct DAC {
			/** Uploads palette entries to the DAC's palette memory.
			 *
			 * @param start The palette index to start at
			 * @param count The number of entries to upload
			 * @param colors Array of palette colors to upload
			 */
			static void upload(uint8_t start, uint8_t count, PaletteColor* colors);
			/** Downloads palette entries from the DAC's palette memory.
			 *
			 * @param start The palette index to start at
			 * @param count The number of entries to download
			 * @param colors Array of palette colors to download to
			 */
			static void download(uint8_t start, uint8_t count, PaletteColor* colors);
		};
		
		static void setCursorEnabled(bool enable);
		
		static void setCursorPosition(uint16_t offset);
	};

}


#endif /* LTOS_API_ARCH_X86_GFX_VGA_H_ */
