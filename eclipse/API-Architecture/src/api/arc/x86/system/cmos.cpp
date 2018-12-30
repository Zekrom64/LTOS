/*
 * cmos.cpp
 *
 *  Created on: Dec 25, 2018
 *      Author: Zekrom_64
 */

#include "ltos/api/arch/x86/system/cmos.h"
#include "ltos/api/arch/x86/system/acpi.h"

using namespace ltos::x86;

uint8_t readCMOS(uint8_t reg) {
	uint8_t port = X86::inb(0x70);
	port = (port & 0x80) | reg;
	X86::outb(0x70, port);
	for(int i = 0; i < 100; i++); // Delay, because old ISA stuff
	return X86::inb(0x71);
}

uint8_t CMOS::read(CMOS::CMOSRegister reg) {
	return readCMOS((uint8_t)reg);
}

inline void readRTCRaw(CMOS::RTC* rtc, int centuryRegOffset) {
	rtc->seconds = readCMOS(0x00);
	rtc->minutes = readCMOS(0x02);
	rtc->hours = readCMOS(0x04);
	rtc->day = readCMOS(0x07);
	rtc->month = readCMOS(0x08);
	rtc->year = readCMOS(0x09);
	if (centuryRegOffset != -1) rtc->century = readCMOS(centuryRegOffset);
	else rtc->century = 0;
}

inline bool rtcEqual(CMOS::RTC* rtc1, CMOS::RTC* rtc2) {
	if (rtc1->seconds != rtc2->seconds) return false;
	if (rtc1->minutes != rtc2->minutes) return false;
	if (rtc1->hours != rtc2->hours) return false;
	if (rtc1->day != rtc2->day) return false;
	if (rtc1->month != rtc2->month) return false;
	if (rtc1->year != rtc2->year) return false;
	if (rtc1->century != rtc2->century) return false;
	return true;
}

inline void bcdToBin(uint8_t& field) {
	field = (field & 0xF) + ((field >> 4) & 0xF);
}

void CMOS::readRTC(CMOS::RTC* rtc, int centuryRegOffset) {
	uint8_t statusB = CMOS::read(CMOS::REG_STATUS_B);
	uint8_t statusA;
	CMOS::RTC lastRTC;
	readRTCRaw(rtc, centuryRegOffset);
	do {
		lastRTC = *rtc;
		readRTCRaw(rtc, centuryRegOffset);
	} while(!rtcEqual(&lastRTC, rtc));

	bool hr24 = statusB & 2;
	bool binary = statusB & 4;

	bool pm = rtc->hours & 0x80;
	rtc->hours &= 0x7F;

	if (!binary) {
		bcdToBin(rtc->seconds);
		bcdToBin(rtc->minutes);
		bcdToBin(rtc->hours);
		bcdToBin(rtc->day);
		bcdToBin(rtc->month);
		bcdToBin(rtc->year);
		bcdToBin(rtc->century);
	}

	if (pm && !hr24) {
		rtc->hours += 12;
		if (rtc->hours >= 24) rtc->hours -= 24;
	}
}
