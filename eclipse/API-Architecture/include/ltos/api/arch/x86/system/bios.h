/*
 * ebda.h
 *
 *  Created on: Oct 18, 2018
 *      Author: Zekrom_64
 */

#ifndef LTOS_API_ARCH_X86_BIOS_H_
#define LTOS_API_ARCH_X86_BIOS_H_


#include "ltos/lttypes.h"
#include "ltos/ltmacros.h"

namespace ltos::x86 {

// BIOS Data Area: http://www.bioscentral.com/misc/bda.htm

LT_PACK_PREFIX
struct BDA {
	uint16_t serialPortIO[4];
	uint16_t parallelPortIO[3];
	uint16_t ebdaBaseDiv16;
	uint16_t hardwareFlags;
	uint8_t mfgTestIntFlag;
	struct {
		uint8_t low;
		uint8_t high;
	} memorySizeKB;
	struct {
		uint8_t low;
		uint8_t high;
	} errorCode;
	uint8_t keyboardFlags1;
	uint8_t keyboardFlags2;
	uint8_t numpadWorkArea;
	uint16_t keyboardNextCharPtr;
	uint16_t keyboardLastCharPtr;
	uint8_t keyboardBuffer[32];
	uint8_t floppyCalibrationStatus;
	uint8_t floppyMotorStatus;
	uint8_t floppyMotorTimeout;
	uint8_t floppyDriveStatus;
	uint8_t hddFloppyStatus[3];
	uint8_t floppyCylinder;
	uint8_t floppyHead;
	uint8_t floppySector;
	uint8_t floppyNumWritten;
	uint8_t activeVideoMode;
	uint16_t textColumnsPerRow;
	uint16_t activePageSize;
	uint16_t activePageOffset;
	uint16_t cursorPosition[8];
	uint16_t cursorShape;
	uint8_t activeVideoPage;
	struct {
		uint8_t low;
		uint8_t high;
	} videoAdapterIOPort;
	uint8_t videoInternalMode;
	uint8_t colorPalette;
	struct {
		uint8_t low;
		uint8_t high;
	} adapterROMOffset;
	struct {
		uint8_t low;
		uint8_t high;
	} adapterROMSegment;
	uint8_t lastInterrupt;
	uint32_t int1ACounter;
	uint8_t timer24HourFlag;
	uint8_t ctrlBreakFlag;
	uint16_t softResetFlag;
	uint8_t lastHDDOperation;
	uint8_t numHardDisks;
	uint8_t hardDiskControl;
	uint8_t hardDiskIOOffset;
	uint8_t parallelPortTimeout[3];
	uint8_t virtualDMAServices;
	uint8_t serialPortTimeout[4];
	uint16_t keyboardBufferStart;
	uint16_t keyboardBufferEnd;
	uint8_t numVideoRowsSub1;
	struct {
		uint8_t low;
		uint8_t high;
	} numScanLinesPerChar;
	uint8_t videoAdapterOptions;
	uint8_t videoAdapterSwitches;
	uint8_t vgaFlags[2];
	uint8_t floppyConfigData;
	uint8_t hardDiskStatus;
	uint8_t hardDiskError;
	uint8_t hardDiskTaskComplete;
	uint8_t floppyDriveInfo;
	uint8_t floppyDiskState[2];
	uint8_t floppyDiskStartingState[2];
	uint8_t floppyDiskCylinder[2];
	uint8_t keyboardFlags3;
	uint8_t keyboardFlags4;
	struct {
		uint16_t offset;
		uint16_t segment;
	} userWaitFlagPtr;
	uint32_t userWaitCount;
	uint8_t userWaitFlag;
	uint8_t localNetworkBytes[7];
	struct {
		uint16_t offset;
		uint16_t segment;
	} videoParamControl;
	uint8_t reserved[68];
	uint8_t intraAppComms[16];
} LT_PACK_ATTRIB;
LT_PACK_SUFFIX

#define X86_BDA ((BDA*)0x400)

LT_ASSERT_OFFSETOF(BDA, serialPortIO,            0x00)
LT_ASSERT_OFFSETOF(BDA, parallelPortIO,          0x08)
LT_ASSERT_OFFSETOF(BDA, ebdaBaseDiv16,           0x0E)
LT_ASSERT_OFFSETOF(BDA, hardwareFlags,           0x10)
LT_ASSERT_OFFSETOF(BDA, mfgTestIntFlag,          0x12)
LT_ASSERT_OFFSETOF(BDA, memorySizeKB,            0x13)
LT_ASSERT_OFFSETOF(BDA, errorCode,               0x15)
LT_ASSERT_OFFSETOF(BDA, keyboardFlags1,          0x17)
LT_ASSERT_OFFSETOF(BDA, keyboardFlags2,          0x18)
LT_ASSERT_OFFSETOF(BDA, numpadWorkArea,          0x19)
LT_ASSERT_OFFSETOF(BDA, keyboardNextCharPtr,     0x1A)
LT_ASSERT_OFFSETOF(BDA, keyboardLastCharPtr,     0x1C)
LT_ASSERT_OFFSETOF(BDA, keyboardBuffer,          0x1E)
LT_ASSERT_OFFSETOF(BDA, floppyCalibrationStatus, 0x3E)
LT_ASSERT_OFFSETOF(BDA, floppyMotorStatus,       0x3F)
LT_ASSERT_OFFSETOF(BDA, floppyMotorTimeout,      0x40)
LT_ASSERT_OFFSETOF(BDA, floppyDriveStatus,       0x41)
LT_ASSERT_OFFSETOF(BDA, hddFloppyStatus,         0x42)
LT_ASSERT_OFFSETOF(BDA, floppyCylinder,          0x45)
LT_ASSERT_OFFSETOF(BDA, floppyHead,              0x46)
LT_ASSERT_OFFSETOF(BDA, floppySector,            0x47)
LT_ASSERT_OFFSETOF(BDA, floppyNumWritten,        0x48)
LT_ASSERT_OFFSETOF(BDA, activeVideoMode,         0x49)
LT_ASSERT_OFFSETOF(BDA, textColumnsPerRow,       0x4A)
LT_ASSERT_OFFSETOF(BDA, activePageSize,          0x4C)
LT_ASSERT_OFFSETOF(BDA, activePageOffset,        0x4E)
LT_ASSERT_OFFSETOF(BDA, cursorPosition,          0x50)
LT_ASSERT_OFFSETOF(BDA, cursorShape,             0x60)
LT_ASSERT_OFFSETOF(BDA, activeVideoPage,         0x62)
LT_ASSERT_OFFSETOF(BDA, videoAdapterIOPort,      0x63)
LT_ASSERT_OFFSETOF(BDA, videoInternalMode,       0x65)
LT_ASSERT_OFFSETOF(BDA, colorPalette,            0x66)
LT_ASSERT_OFFSETOF(BDA, adapterROMOffset,        0x67)
LT_ASSERT_OFFSETOF(BDA, adapterROMSegment,       0x69)
LT_ASSERT_OFFSETOF(BDA, lastInterrupt,           0x6B)
LT_ASSERT_OFFSETOF(BDA, int1ACounter,            0x6C)
LT_ASSERT_OFFSETOF(BDA, timer24HourFlag,         0x70)
LT_ASSERT_OFFSETOF(BDA, ctrlBreakFlag,           0x71)
LT_ASSERT_OFFSETOF(BDA, softResetFlag,           0x72)
LT_ASSERT_OFFSETOF(BDA, lastHDDOperation,        0x74)
LT_ASSERT_OFFSETOF(BDA, numHardDisks,            0x75)
LT_ASSERT_OFFSETOF(BDA, hardDiskControl,         0x76)
LT_ASSERT_OFFSETOF(BDA, hardDiskIOOffset,        0x77)
LT_ASSERT_OFFSETOF(BDA, parallelPortTimeout,     0x78)
LT_ASSERT_OFFSETOF(BDA, virtualDMAServices,      0x7B)
LT_ASSERT_OFFSETOF(BDA, serialPortTimeout,       0x7C)
LT_ASSERT_OFFSETOF(BDA, keyboardBufferStart,     0x80)
LT_ASSERT_OFFSETOF(BDA, keyboardBufferEnd,       0x82)
LT_ASSERT_OFFSETOF(BDA, numVideoRowsSub1,        0x84)
LT_ASSERT_OFFSETOF(BDA, numScanLinesPerChar,     0x85)
LT_ASSERT_OFFSETOF(BDA, videoAdapterOptions,     0x87)
LT_ASSERT_OFFSETOF(BDA, videoAdapterSwitches,    0x88)
LT_ASSERT_OFFSETOF(BDA, vgaFlags,                0x89)
LT_ASSERT_OFFSETOF(BDA, floppyConfigData,        0x8B)
LT_ASSERT_OFFSETOF(BDA, hardDiskStatus,          0x8C)
LT_ASSERT_OFFSETOF(BDA, hardDiskError,           0x8D)
LT_ASSERT_OFFSETOF(BDA, hardDiskTaskComplete,    0x8E)
LT_ASSERT_OFFSETOF(BDA, floppyDriveInfo,         0x8F)
LT_ASSERT_OFFSETOF(BDA, floppyDiskState,         0x90)
LT_ASSERT_OFFSETOF(BDA, floppyDiskStartingState, 0x92)
LT_ASSERT_OFFSETOF(BDA, floppyDiskCylinder,      0x94)
LT_ASSERT_OFFSETOF(BDA, keyboardFlags3,          0x96)
LT_ASSERT_OFFSETOF(BDA, keyboardFlags4,          0x97)
LT_ASSERT_OFFSETOF(BDA, userWaitFlagPtr,         0x98)
LT_ASSERT_OFFSETOF(BDA, userWaitCount,           0x9C)
LT_ASSERT_OFFSETOF(BDA, userWaitFlag,            0xA0)
LT_ASSERT_OFFSETOF(BDA, localNetworkBytes,       0xA1)
LT_ASSERT_OFFSETOF(BDA, videoParamControl,       0xA8)
LT_ASSERT_OFFSETOF(BDA, intraAppComms,           0xF0)

// Extended BIOS Data Area: https://matrix.home.xs4all.nl/system/ebda.html

enum EBDASize {
	EBDA_SIZE_1KB = 1,
	EBDA_SIZE_2KB = 2
};

LT_PACK_PREFIX
struct EBDA {
	uint8_t size;
	uint8_t padding0[22];
	uint8_t numPostEntries;
	uint16_t postErrorLog[5];
	struct {
		uint16_t offset;
		uint16_t segment;
	} mouseDriverFarCall;
	uint8_t mouseFlags1;
	uint8_t mouseFlags2;
	uint8_t mouseData[8];
	uint8_t padding1[9];
	struct {
		uint8_t lower;
		uint8_t upper;
	} watchdogTimer;
	uint8_t padding2[2];
	uint8_t hardDisk0ParamTable[16];
	uint8_t hardDisk1ParamTable[16];
	uint8_t padding3[11];
	uint8_t cacheControl;
	uint8_t padding4[5];
	uint8_t keyboardRepeatRate;
	uint8_t keyRepeatDelay;
	uint8_t numHardDisksInstalled;
	uint8_t hardDiskDMAChannel;
	uint8_t hardDiskInterruptStatus;
	uint8_t hardDiskOperationFlags;
	uint32_t oldInt76VectorPtr;
	uint8_t hardDiskDMAType;
	uint8_t hardDiskOpStatus;
	uint8_t hardDiskTimeout;
	uint8_t padding5[3];
	uint16_t hardDiskStatusWords[8];
	uint8_t padding6[89];
	uint8_t disketteDriveType;
	uint8_t padding7[4];
	uint8_t hardDiskParamsLoaded;
	uint8_t padding8[1];
	uint8_t cpuFamilyID;
	uint8_t cpuStepping;
	uint8_t padding9[39];
	struct {
		uint8_t low;
		uint8_t high;
	} keyboardID;
	uint8_t padding10[1];
	uint16_t nonBiosInt18Flag;
	uint8_t padding;
	uint8_t userInt18FarPointer[4];

	static EBDA* find() {
		uint16_t addr = X86_BDA->ebdaBaseDiv16;
		if (addr < 0x9000) return (EBDA*)(0x9FC00);
		return (EBDA*)((uintptr_t)addr << 4);
	}
} LT_PACK_ATTRIB;
LT_PACK_SUFFIX

LT_ASSERT_OFFSETOF(EBDA, size,                    0x000)
LT_ASSERT_OFFSETOF(EBDA, numPostEntries,          0x017)
LT_ASSERT_OFFSETOF(EBDA, postErrorLog,            0x018)
LT_ASSERT_OFFSETOF(EBDA, mouseDriverFarCall,      0x022)
LT_ASSERT_OFFSETOF(EBDA, mouseFlags1,             0x026)
LT_ASSERT_OFFSETOF(EBDA, mouseFlags2,             0x027)
LT_ASSERT_OFFSETOF(EBDA, mouseData,               0x028)
LT_ASSERT_OFFSETOF(EBDA, watchdogTimer,           0x039)
LT_ASSERT_OFFSETOF(EBDA, hardDisk0ParamTable,     0x03D)
LT_ASSERT_OFFSETOF(EBDA, hardDisk1ParamTable,     0x04D)
LT_ASSERT_OFFSETOF(EBDA, cacheControl,            0x068)
LT_ASSERT_OFFSETOF(EBDA, keyboardRepeatRate,      0x06E)
LT_ASSERT_OFFSETOF(EBDA, keyRepeatDelay,          0x06F)
LT_ASSERT_OFFSETOF(EBDA, numHardDisksInstalled,   0x070)
LT_ASSERT_OFFSETOF(EBDA, hardDiskDMAChannel,      0x071)
LT_ASSERT_OFFSETOF(EBDA, hardDiskInterruptStatus, 0x072)
LT_ASSERT_OFFSETOF(EBDA, hardDiskOperationFlags,  0x073)
LT_ASSERT_OFFSETOF(EBDA, oldInt76VectorPtr,       0x074)
LT_ASSERT_OFFSETOF(EBDA, hardDiskDMAType,         0x078)
LT_ASSERT_OFFSETOF(EBDA, hardDiskOpStatus,        0x079)
LT_ASSERT_OFFSETOF(EBDA, hardDiskTimeout,         0x07A)
LT_ASSERT_OFFSETOF(EBDA, hardDiskStatusWords,     0x07E)
LT_ASSERT_OFFSETOF(EBDA, disketteDriveType,       0x0E7)
LT_ASSERT_OFFSETOF(EBDA, hardDiskParamsLoaded,    0x0EC)
LT_ASSERT_OFFSETOF(EBDA, cpuFamilyID,             0x0EE)
LT_ASSERT_OFFSETOF(EBDA, cpuStepping,             0x0EF)
LT_ASSERT_OFFSETOF(EBDA, keyboardID,              0x117)
LT_ASSERT_OFFSETOF(EBDA, nonBiosInt18Flag,        0x11A)
LT_ASSERT_OFFSETOF(EBDA, userInt18FarPointer,     0x11D)

#define LTOS_X86_BIOS_EBDA

}


#endif /* LTOS_API_ARCH_X86_BIOS_H_ */
