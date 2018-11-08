/*
 * scsi_cmds.inc
 *
 *  Created on: Nov 4, 2018
 *      Author: Zekrom_64
 */

#ifndef LTOS_API_BUS_SCSI_CMDS_INC_
#define LTOS_API_BUS_SCSI_CMDS_INC_

#include "scsi.h"

namespace ltos::SCSI {

	enum SCSI__CHANGE_DEFINITION__DEFINITION_PARAM {
		SCSI_DEFINITION_PARAM__USE_CURRENT = 0x00,
		SCSI_DEFINITION_PARAM__SCSI_2 = 0x03,
		SCSI_DEFINITION_PARAM__SCSI_3 = 0x04,
		SCSI_DEFINITION_PARAM__MFG_DEFAULT = 0x3F,
	};
	inline SCSICommand12 CHANGE_DEFINITION(bool save, SCSI__CHANGE_DEFINITION__DEFINITION_PARAM defParam,
			uint8_t paramLength) {
		SCSICommand12 cmd = {};
		cmd.op = 0x40;
		cmd.data_bytes[3] = save ? 1 : 0;
		cmd.data_bytes[4] = defParam & 0x7F;
		cmd.data_bytes[9] = paramLength;
		return cmd;
	}

	enum SCSI__FORMAT_UNIT__DEFECT_LIST_FORMAT {
		SCSI_DEFECT_LIST_FORMAT__NONE = 0,
		SCSI_DEFECT_LIST_FORMAT__SHORT_BLOCK = 0,
		SCSI_DEFECT_LIST_FORMAT__LONG_BLOCK = 3,
	};
	inline SCSICommand6 FORMAT_UNIT(uint8_t fmtpInfo, bool longList, bool fmtData, bool cmpList,
			SCSI__FORMAT_UNIT__DEFECT_LIST_FORMAT defectLstFmt) {
		SCSICommand6 cmd = {};
		cmd.op = 0x04;
		cmd.data_bytes[0] = (fmtpInfo << 6) | (longList ? 0x20 : 0) | (fmtData ? 0x10 : 0) | (cmpList ? 0x08 : 0) | defectLstFmt;
		return cmd;
	}

	inline SCSICommand6 INQUIRY(bool evpd, uint8_t pageCode, uint16_t allocLength) {
		SCSICommand6 cmd = {};
		cmd.op = 0x12;
		cmd.data_bytes[0] = evpd ? 1 : 0;
		cmd.data_bytes[1] = pageCode;
		cmd.data_bytes[2] = allocLength >> 8;
		cmd.data_bytes[3] = allocLength;
		return cmd;
	}

	inline SCSICommand10 LOG_SELECT(bool pcr, bool sp, uint8_t pc, uint8_t pageCode, uint16_t paramListLength) {
		SCSICommand10 cmd = {};
		cmd.op = 0x4C;
		cmd.data_bytes[0] = (pcr ? 0x2 : 0) | (sp ? 1 : 0);
		cmd.data_bytes[1] = (pc << 6) | (pageCode & 0x3F);
		cmd.data_bytes[6] = paramListLength >> 8;
		cmd.data_bytes[7] = paramListLength;
		return cmd;
	}

	inline SCSICommand10 LOG_SENSE(bool sp, uint8_t pc, uint8_t pageCode, uint16_t paramPtr, uint16_t allocLength) {
		SCSICommand10 cmd = {};
		cmd.op = 0x4D;
		cmd.data_bytes[0] = sp ? 1 : 0;
		cmd.data_bytes[1] = (pc << 6) | (pageCode & 0x3F);
		cmd.data_bytes[4] = paramPtr >> 8;
		cmd.data_bytes[5] = paramPtr;
		cmd.data_bytes[6] = allocLength >> 8;
		cmd.data_bytes[7] = allocLength;
		return cmd;
	}

	inline SCSICommand6 MODE_SELECT(bool pf, bool sp, uint8_t paramListLength) {
		SCSICommand6 cmd = {};
		cmd.op = 0x15;
		cmd.data_bytes[0] = (pf ? 0x10 : 0) | (sp ? 1 : 0);
		cmd.data_bytes[3] = paramListLength;
		return cmd;
	}

	inline SCSICommand10 MODE_SELECT_10(bool pf, bool sp, uint16_t paramListLength) {
		SCSICommand10 cmd = {};
		cmd.op = 0x55;
		cmd.data_bytes[0] = (pf ? 0x10 : 0) | (sp ? 1 : 0);
		cmd.data_bytes[6] = paramListLength >> 8;
		cmd.data_bytes[7] = paramListLength;
		return cmd;
	}

	inline SCSICommand6 MODE_SENSE(bool dbd, uint8_t pc, uint8_t pageCode, uint8_t subpageCode, uint8_t allocLength) {
		SCSICommand6 cmd = {};
		cmd.op = 0x1A;
		cmd.data_bytes[0] = dbd ? 0x40 : 0;
		cmd.data_bytes[1] = (pc << 6) | (pageCode & 0x3F);
		cmd.data_bytes[2] = subpageCode;
		cmd.data_bytes[3] = allocLength;
		return cmd;
	}

	inline SCSICommand10 MODE_SENSE_10(bool llbaa, bool dbd, uint8_t pc, uint8_t pageCode, uint8_t subpageCode, uint16_t allocLength) {
		SCSICommand10 cmd = {};
		cmd.op = 0x5A;
		cmd.data_bytes[0] = (llbaa ? 0x10 : 0) | (dbd ? 0x08 : 0);
		cmd.data_bytes[1] = (pc << 6) | (pageCode & 0x3F);
		cmd.data_bytes[2] = subpageCode;
		cmd.data_bytes[6] = allocLength >> 16;
		cmd.data_bytes[7] = allocLength;
		return cmd;
	}

	inline SCSICommand10 PERSISTENT_RESERVE_IN(uint8_t serviceAction, uint16_t allocLength) {
		SCSICommand10 cmd = {};
		cmd.op = 0x5E;
		cmd.data_bytes[0] = serviceAction & 0x1F;
		cmd.data_bytes[6] = allocLength >> 8;
		cmd.data_bytes[7] = allocLength;
		return cmd;
	}

	inline SCSICommand10 PERSISTENT_RESERVE_OUT(uint8_t serviceAction, uint8_t scope, uint8_t type, uint16_t paramListLength) {
		SCSICommand10 cmd = {};
		cmd.op = 0x5F;
		cmd.data_bytes[0] = serviceAction & 0x1F;
		cmd.data_bytes[1] = (scope << 4) | (type & 0xF);
		cmd.data_bytes[4] = paramListLength >> 8;
		cmd.data_bytes[5] = paramListLength;
		return cmd;
	}

	inline SCSICommand6 READ(uint32_t lba, uint8_t length) {
		SCSICommand6 cmd = {};
		cmd.op = 0x08;
		cmd.data_bytes[0] = (lba >> 16) & 0x1F;
		cmd.data_bytes[1] = lba >> 8;
		cmd.data_bytes[2] = lba;
		cmd.data_bytes[3] = length;
		return cmd;
	}

	inline SCSICommand10 READ_10(uint8_t rdProt, bool dpo, bool fua, bool fuaNV, uint32_t lba, uint8_t group, uint16_t length) {
		SCSICommand10 cmd = {};
		cmd.op = 0x28;
		cmd.data_bytes[0] = (rdProt << 5) | (dpo ? 0x10 : 0) | (fua ? 0x08 : 0) | (fuaNV ? 0x02 : 0);
		cmd.data_bytes[1] = lba >> 24;
		cmd.data_bytes[2] = lba >> 16;
		cmd.data_bytes[3] = lba >> 8;
		cmd.data_bytes[4] = lba;
		cmd.data_bytes[5] = group & 0x1F;
		cmd.data_bytes[6] = length >> 8;
		cmd.data_bytes[7] = length;
		return cmd;
	}

	inline SCSICommand12 READ_12(uint8_t rdProt, bool dpo, bool fua, bool fuaNV, uint32_t lba, uint32_t length, uint8_t group) {
		SCSICommand12 cmd = {};
		cmd.data_bytes[0] = (rdProt << 5) | (dpo ? 0x10 : 0) | (fua ? 0x08 : 0) | (fuaNV ? 0x02 : 0);
		cmd.data_bytes[1] = lba >> 24;
		cmd.data_bytes[2] = lba >> 16;
		cmd.data_bytes[3] = lba >> 8;
		cmd.data_bytes[4] = lba;
		cmd.data_bytes[5] = length >> 24;
		cmd.data_bytes[6] = length >> 16;
		cmd.data_bytes[7] = length >> 8;
		cmd.data_bytes[8] = length;
		cmd.data_bytes[9] = group;
		return cmd;
	}

	inline SCSICommand16 READ_16(uint8_t rdProt, bool dpo, bool fua, bool fuaNV, uint64_t lba, uint32_t length, uint8_t group) {
		SCSICommand16 cmd = {};
		cmd.data_bytes[0] = (rdProt << 5) | (dpo ? 0x10 : 0) | (fua ? 0x08 : 0) | (fuaNV ? 0x02 : 0);
		cmd.data_bytes[1] = lba >> 56;
		cmd.data_bytes[2] = lba >> 48;
		cmd.data_bytes[3] = lba >> 40;
		cmd.data_bytes[4] = lba >> 32;
		cmd.data_bytes[5] = lba >> 24;
		cmd.data_bytes[6] = lba >> 16;
		cmd.data_bytes[7] = lba >> 8;
		cmd.data_bytes[8] = lba;
		cmd.data_bytes[9] = length >> 24;
		cmd.data_bytes[10] = length >> 16;
		cmd.data_bytes[11] = length >> 8;
		cmd.data_bytes[12] = length;
		cmd.data_bytes[13] = group;
		return cmd;
	}

	inline SCSICommand32 READ_32(uint8_t groupNum, uint8_t rdProt, bool dpo, bool fua, bool fuaNV,
			uint64_t lba, uint32_t initialLbaTag, uint16_t lbaAppTag, uint16_t lbaAppTagMask, uint32_t length) {
		SCSICommand32 cmd = {};
		cmd.op = 0x7F;
		cmd.groupNumber = groupNum & 0x1F;
		cmd.additionalLength = 0x18;
		cmd.serviceAction = 0x0009;
		cmd.flags = (rdProt << 5) | (dpo ? 0x10 : 0) | (fua ? 0x08 : 0) | (fuaNV ? 0x20 : 0);
		cmd.lba = lba;
		cmd.initialLbaTag = initialLbaTag;
		cmd.lbaAppTag = lbaAppTag;
		cmd.lbaAppTagMask = lbaAppTagMask;
		cmd.length = length;
		return cmd;
	}

	inline SCSICommand10 READ_BUFFER(uint8_t mode, uint8_t bufID, uint32_t bufOffset, uint32_t allocLength) {
		SCSICommand10 cmd = {};
		cmd.op = 0x3C;
		cmd.data_bytes[0] = mode & 0x1F;
		cmd.data_bytes[1] = bufID;
		cmd.data_bytes[2] = bufOffset >> 16;
		cmd.data_bytes[3] = bufOffset >> 8;
		cmd.data_bytes[4] = bufOffset;
		cmd.data_bytes[6] = allocLength >> 16;
		cmd.data_bytes[7] = allocLength >> 8;
		cmd.data_bytes[8] = allocLength;
		return cmd;
	}

	inline SCSICommand10 READ_CAPACITY(uint32_t lba, bool pmi) {
		SCSICommand10 cmd = {};
		cmd.op = 0x25;
		cmd.data_bytes[1] = lba >> 24;
		cmd.data_bytes[2] = lba >> 16;
		cmd.data_bytes[3] = lba >> 8;
		cmd.data_bytes[4] = lba;
		cmd.data_bytes[7] = pmi ? 1 : 0;
		return cmd;
	}

	inline SCSICommand16 READ_CAPACITY_16(uint64_t lba, uint32_t allocLength, bool pmi) {
		SCSICommand16 cmd = {};
		cmd.op = 0x9E;
		cmd.data_bytes[0] = 0x10;
		cmd.data_bytes[1] = lba >> 56;
		cmd.data_bytes[2] = lba >> 48;
		cmd.data_bytes[3] = lba >> 40;
		cmd.data_bytes[4] = lba >> 32;
		cmd.data_bytes[5] = lba >> 24;
		cmd.data_bytes[6] = lba >> 16;
		cmd.data_bytes[7] = lba >> 8;
		cmd.data_bytes[8] = lba;
		return cmd;
	}

	inline SCSICommand10 READ_DEFECT_DATA(bool plist, bool glist, uint8_t listFmt, uint16_t allocLength) {
		SCSICommand10 cmd = {};
		cmd.op = 0x37;
		cmd.data_bytes[1] = (plist ? 0x10 : 0) | (glist ? 0x08 : 0) | (listFmt & 0x7);
		cmd.data_bytes[6] = allocLength >> 8;
		cmd.data_bytes[7] = allocLength;
		return cmd;
	}

	inline SCSICommand12 READ_DEFECT_DATA_12(bool plist, bool glist, uint8_t listFmt, uint32_t allocLength) {
		SCSICommand12 cmd = {};
		cmd.op = 0xB7;
		cmd.data_bytes[1] = (plist ? 0x10 : 0) | (glist ? 0x08 : 0) | (listFmt & 0x7);
		cmd.data_bytes[5] = allocLength >> 24;
		cmd.data_bytes[6] = allocLength >> 16;
		cmd.data_bytes[7] = allocLength >> 8;
		cmd.data_bytes[8] = allocLength;
		return cmd;
	}

	inline SCSICommand10 READ_LONG(bool pblock, bool correct, uint32_t lba, uint16_t transferLen) {
		SCSICommand10 cmd = {};
		cmd.op = 0x3E;
		cmd.data_bytes[0] = (pblock ? 0x04 : 0) | (correct ? 0x02 : 0);
		cmd.data_bytes[1] = lba >> 24;
		cmd.data_bytes[2] = lba >> 16;
		cmd.data_bytes[3] = lba >> 8;
		cmd.data_bytes[4] = lba;
		cmd.data_bytes[6] = transferLen >> 8;
		cmd.data_bytes[7] = transferLen;
		return cmd;
	}

	inline SCSICommand6 REASSIGN_BLOCKS(bool longLBA, bool longList) {
		SCSICommand6 cmd = {};
		cmd.op = 0x07;
		cmd.data_bytes[0] = (longLBA ? 0x02 : 0) | (longList ? 0x01 : 0);
		return cmd;
	}

	inline SCSICommand6 RECEIVE_DIAGNOSTIC_RESULTS(bool pcv, uint8_t pageCode, uint16_t allocLength) {
		SCSICommand6 cmd = {};
		cmd.op = 0x1C;
		cmd.data_bytes[0] = pcv ? 1 : 0;
		cmd.data_bytes[1] = pageCode;
		cmd.data_bytes[2] = allocLength >> 8;
		cmd.data_bytes[3] = allocLength;
		return cmd;
	}

	inline SCSICommand12 REPORT_DEVICE_IDENTIFIER(uint32_t allocLength, uint8_t infoType) {
		SCSICommand12 cmd = {};
		cmd.op = 0xA3;
		cmd.data_bytes[0] = 0x05;
		cmd.data_bytes[5] = allocLength >> 24;
		cmd.data_bytes[6] = allocLength >> 16;
		cmd.data_bytes[7] = allocLength >> 8;
		cmd.data_bytes[8] = allocLength;
		cmd.data_bytes[9] = infoType << 1;
		return cmd;
	}

	inline SCSICommand12 REPORT_LUNS(uint8_t selReport, uint32_t allocLength) {
		SCSICommand12 cmd = {};
		cmd.op = 0xA0;
		cmd.data_bytes[1] = selReport;
		cmd.data_bytes[5] = allocLength >> 24;
		cmd.data_bytes[6] = allocLength >> 16;
		cmd.data_bytes[7] = allocLength >> 8;
		cmd.data_bytes[8] = allocLength;
		return cmd;
	}

	inline SCSICommand12 REPORT_SUPPORTED_OPERATION_CODES(bool rctd, uint8_t reportingOpts, uint8_t opcode, uint16_t serviceAction,
			uint32_t allocLength) {
		SCSICommand12 cmd = {};
		cmd.op = 0xA3;
		cmd.data_bytes[0] = 0x0C;
		cmd.data_bytes[1] = (rctd ? 0x80 : 0) | (reportingOpts & 0x7);
		cmd.data_bytes[2] = opcode;
		cmd.data_bytes[3] = serviceAction >> 8;
		cmd.data_bytes[4] = serviceAction;
		cmd.data_bytes[5] = allocLength >> 24;
		cmd.data_bytes[6] = allocLength >> 16;
		cmd.data_bytes[7] = allocLength >> 8;
		cmd.data_bytes[8] = allocLength;
		return cmd;
	}

	inline SCSICommand12 REPORT_SUPPORTED_TASK_MANAGEMENT_FUNCTIONS(uint32_t allocLength) {
		SCSICommand12 cmd = {};
		cmd.op = 0xA3;
		cmd.data_bytes[0] = 0x0D;
		cmd.data_bytes[5] = allocLength >> 24;
		cmd.data_bytes[6] = allocLength >> 16;
		cmd.data_bytes[7] = allocLength >> 8;
		cmd.data_bytes[8] = allocLength;
		return cmd;
	}

	inline SCSICommand6 REQUEST_SENSE(bool desc, uint8_t allocLength) {
		SCSICommand6 cmd = {};
		cmd.op = 0x03;
		cmd.data_bytes[0] = desc ? 1 : 0;
		cmd.data_bytes[3] = allocLength;
		return cmd;
	}

	inline SCSICommand12 SECURITY_PROTOCOL_IN(uint8_t protocol, uint16_t protSpecific, bool inc512, uint32_t allocLength) {
		SCSICommand12 cmd = {};
		cmd.op = 0xA2;
		cmd.data_bytes[0] = protocol;
		cmd.data_bytes[1] = protSpecific >> 8;
		cmd.data_bytes[2] = protSpecific;
		cmd.data_bytes[3] = inc512 ? 0x80 : 0;
		cmd.data_bytes[5] = allocLength >> 24;
		cmd.data_bytes[6] = allocLength >> 16;
		cmd.data_bytes[7] = allocLength >> 8;
		cmd.data_bytes[8] = allocLength;
		return cmd;
	}

	inline SCSICommand12 SECURITY_PROTOCOL_OUT(uint8_t protocol, uint16_t protSpecific, bool inc512, uint32_t transferLength) {
		SCSICommand12 cmd = {};
		cmd.op = 0xA2;
		cmd.data_bytes[0] = protocol;
		cmd.data_bytes[1] = protSpecific >> 8;
		cmd.data_bytes[2] = protSpecific;
		cmd.data_bytes[3] = inc512 ? 0x80 : 0;
		cmd.data_bytes[5] = transferLength >> 24;
		cmd.data_bytes[6] = transferLength >> 16;
		cmd.data_bytes[7] = transferLength >> 8;
		cmd.data_bytes[8] = transferLength;
		return cmd;
	}

	inline SCSICommand6 SEND_DIAGNOSTIC(uint8_t selfTestCode, bool pf, bool selfTest, bool devOffline, bool unitOffline,
			uint16_t paramListLength) {
		SCSICommand6 cmd = {};
		cmd.op = 0x1D;
		cmd.data_bytes[0] = (selfTestCode << 5) | (pf ? 0x10 : 0) | (selfTest ? 0x04 : 0) | (devOffline ? 0x02 : 0) | (unitOffline ? 1 : 0);
		cmd.data_bytes[2] = paramListLength >> 8;
		cmd.data_bytes[3] = paramListLength;
		return cmd;
	}

	inline SCSICommand12 SET_IDENTIFYING_INFORMATION(uint32_t paramListLength, uint8_t infoType) {
		SCSICommand12 cmd = {};
		cmd.op = 0xA4;
		cmd.data_bytes[0] = 0x06;
		cmd.data_bytes[5] = paramListLength >> 24;
		cmd.data_bytes[6] = paramListLength >> 16;
		cmd.data_bytes[7] = paramListLength >> 8;
		cmd.data_bytes[8] = paramListLength;
		cmd.data_bytes[9] = infoType << 1;
		return cmd;
	}

	inline SCSICommand12 SET_TIMESTAMP(uint32_t paramListLength) {
		SCSICommand12 cmd = {};
		cmd.op = 0x0F;
		cmd.data_bytes[0] = 0x0F;
		cmd.data_bytes[5] = paramListLength >> 24;
		cmd.data_bytes[6] = paramListLength >> 16;
		cmd.data_bytes[7] = paramListLength >> 8;
		cmd.data_bytes[8] = paramListLength;
		return cmd;
	}

	inline SCSICommand6 START_STOP_UNIT(bool immed, uint8_t powerCondMod, uint8_t powerCond, bool noflush, bool loej, bool start) {
		SCSICommand6 cmd = {};
		cmd.op = 0x1B;
		cmd.data_bytes[0] = immed ? 1 : 0;
		cmd.data_bytes[2] = powerCondMod & 0xF;
		cmd.data_bytes[3] = (powerCond << 4) | (noflush ? 0x04 : 0) | (loej ? 0x02 : 0) | (start ? 1 : 0);
		return cmd;
	}

	inline SCSICommand10 SYNCHRONIZE_CACHE(bool syncNV, bool immed, uint32_t lba, uint8_t groupNum, uint16_t numBlocks) {
		SCSICommand10 cmd = {};
		cmd.op = 0x35;
		cmd.data_bytes[0] = (syncNV ? 0x04 : 0) | (immed ? 0x02 : 0);
		cmd.data_bytes[1] = lba >> 24;
		cmd.data_bytes[2] = lba >> 16;
		cmd.data_bytes[3] = lba >> 8;
		cmd.data_bytes[4] = lba;
		cmd.data_bytes[5] = groupNum & 0x1F;
		cmd.data_bytes[6] = numBlocks >> 8;
		cmd.data_bytes[7] = numBlocks;
		return cmd;
	}

	inline SCSICommand6 TEST_UNIT_READY() {
		SCSICommand6 cmd = {};
		cmd.op = 0x00;
		return cmd;
	}

	inline SCSICommand10 UNMAP(uint8_t groupNum, uint16_t paramListLength) {
		SCSICommand10 cmd = {};
		cmd.op = 0x42;
		cmd.data_bytes[5] = groupNum & 0x1F;
		cmd.data_bytes[6] = paramListLength >> 8;
		cmd.data_bytes[7] = paramListLength;
		return cmd;
	}

	inline SCSICommand10 VERIFY(uint8_t vprot, bool dpo, bool byteCheck, uint32_t lba, uint8_t groupNum, uint16_t verifyLength) {
		SCSICommand10 cmd = {};
		cmd.op = 0x2F;
		cmd.data_bytes[0] = (vprot << 5) | (dpo ? 0x10 : 0) | (byteCheck ? 0x02 : 0);
		cmd.data_bytes[1] = lba >> 24;
		cmd.data_bytes[2] = lba >> 16;
		cmd.data_bytes[3] = lba >> 8;
		cmd.data_bytes[4] = lba;
		cmd.data_bytes[5] = groupNum & 0x1F;
		cmd.data_bytes[6] = verifyLength >> 8;
		cmd.data_bytes[7] = verifyLength;
		return cmd;
	}

	inline SCSICommand12 VERIFY_12(uint8_t vprot, bool dpo, bool byteCheck, uint32_t lba, uint32_t verifyLength, uint8_t groupNum) {
		SCSICommand12 cmd = {};
		cmd.op = 0xAF;
		cmd.data_bytes[0] = (vprot << 5) | (dpo ? 0x10 : 0) | (byteCheck ? 0x02 : 0);
		cmd.data_bytes[1] = lba >> 24;
		cmd.data_bytes[2] = lba >> 16;
		cmd.data_bytes[3] = lba >> 8;
		cmd.data_bytes[4] = lba;
		cmd.data_bytes[5] = verifyLength >> 24;
		cmd.data_bytes[6] = verifyLength >> 16;
		cmd.data_bytes[7] = verifyLength >> 8;
		cmd.data_bytes[8] = verifyLength;
		cmd.data_bytes[9] = groupNum & 0x1F;
		return cmd;
	}

	inline SCSICommand16 VERIFY_16(uint8_t vprot, bool dpo, bool byteCheck, uint64_t lba, uint32_t verifyLength, uint8_t groupNum) {
		SCSICommand12 cmd = {};
		cmd.op = 0xAF;
		cmd.data_bytes[0] = (vprot << 5) | (dpo ? 0x10 : 0) | (byteCheck ? 0x02 : 0);
		cmd.data_bytes[1] = lba >> 56;
		cmd.data_bytes[2] = lba >> 48;
		cmd.data_bytes[3] = lba >> 40;
		cmd.data_bytes[4] = lba >> 32;
		cmd.data_bytes[5] = lba >> 24;
		cmd.data_bytes[6] = lba >> 16;
		cmd.data_bytes[7] = lba >> 8;
		cmd.data_bytes[8] = lba;
		cmd.data_bytes[9] = verifyLength >> 24;
		cmd.data_bytes[10] = verifyLength >> 16;
		cmd.data_bytes[11] = verifyLength >> 8;
		cmd.data_bytes[12] = verifyLength;
		cmd.data_bytes[13] = groupNum & 0x1F;
		return cmd;
	}

	inline SCSICommand32 VERIFY_32(uint8_t groupNum, uint8_t vprot, bool dpo, bool byteCheck, uint64_t lba, uint32_t initialLbaTag,
			uint16_t lbaAppTag, uint16_t lbaAppTagMask, uint32_t length) {
		SCSICommand32 cmd = {};
		cmd.op = 0x7F;
		cmd.groupNumber = groupNum & 0x1F;
		cmd.additionalLength = 0x18;
		cmd.serviceAction = 0x000A;
		cmd.flags = (vprot << 5) | (dpo ? 0x10 : 0) | (byteCheck ? 0x02 : 0);
		cmd.lba = lba;
		cmd.initialLbaTag = initialLbaTag;
		cmd.lbaAppTag = lbaAppTag;
		cmd.lbaAppTagMask = lbaAppTagMask;
		cmd.length = length;
		return cmd;
	}

	inline SCSICommand6 WRITE(uint32_t lba, uint8_t length) {
		SCSICommand6 cmd = {};
		cmd.op = 0x0A;
		cmd.data_bytes[0] = (lba >> 16) & 0x1F;
		cmd.data_bytes[1] = lba >> 8;
		cmd.data_bytes[2] = lba;
		cmd.data_bytes[3] = length;
		return cmd;
	}

	inline SCSICommand10 WRITE_10(uint8_t wrProt, bool dpo, bool fua, bool fuaNV, uint32_t lba, uint8_t groupNum, uint16_t length) {
		SCSICommand10 cmd = {};
		cmd.op = 0x2A;
		cmd.data_bytes[0] = (wrProt << 5) | (dpo ? 0x10 : 0) | (fua ? 0x08 : 0) | (fuaNV ? 0x02 : 0);
		cmd.data_bytes[1] = lba >> 24;
		cmd.data_bytes[2] = lba >> 16;
		cmd.data_bytes[3] = lba >> 8;
		cmd.data_bytes[4] = lba;
		cmd.data_bytes[5] = groupNum & 0x1F;
		cmd.data_bytes[6] = length >> 8;
		cmd.data_bytes[7] = length;
		return cmd;
	}

	inline SCSICommand12 WRITE_12(uint8_t wrProt, bool dpo, bool fua, bool fuaNV, uint32_t lba, uint32_t length, uint8_t groupNum) {
		SCSICommand12 cmd = {};
		cmd.op = 0x2A;
		cmd.data_bytes[0] = (wrProt << 5) | (dpo ? 0x10 : 0) | (fua ? 0x08 : 0) | (fuaNV ? 0x02 : 0);
		cmd.data_bytes[1] = lba >> 24;
		cmd.data_bytes[2] = lba >> 16;
		cmd.data_bytes[3] = lba >> 8;
		cmd.data_bytes[4] = lba;
		cmd.data_bytes[5] = length >> 24;
		cmd.data_bytes[6] = length >> 16;
		cmd.data_bytes[7] = length >> 8;
		cmd.data_bytes[8] = length;
		cmd.data_bytes[9] = groupNum & 0x1F;
		return cmd;
	}

	inline SCSICommand16 WRITE_16(uint8_t wrProt, bool dpo, bool fua, bool fuaNV, uint64_t lba, uint32_t length, uint8_t groupNum) {
		SCSICommand16 cmd = {};
		cmd.op = 0x2A;
		cmd.data_bytes[0] = (wrProt << 5) | (dpo ? 0x10 : 0) | (fua ? 0x08 : 0) | (fuaNV ? 0x02 : 0);
		cmd.data_bytes[1] = lba >> 56;
		cmd.data_bytes[2] = lba >> 48;
		cmd.data_bytes[3] = lba >> 40;
		cmd.data_bytes[4] = lba >> 32;
		cmd.data_bytes[5] = lba >> 24;
		cmd.data_bytes[6] = lba >> 16;
		cmd.data_bytes[7] = lba >> 8;
		cmd.data_bytes[8] = lba;
		cmd.data_bytes[9] = length >> 24;
		cmd.data_bytes[10] = length >> 16;
		cmd.data_bytes[11] = length >> 8;
		cmd.data_bytes[12] = length;
		cmd.data_bytes[13] = groupNum & 0x1F;
		return cmd;
	}

	inline SCSICommand32 WRITE_32(uint8_t groupNum, uint8_t wrProt, bool dpo, bool fua, bool fuaNV, uint64_t lba, uint32_t initialLbaTag,
			uint16_t lbaAppTag, uint16_t lbaAppTagMask, uint32_t length) {
		SCSICommand32 cmd = {};
		cmd.op = 0x7F;
		cmd.groupNumber = groupNum & 0x1F;
		cmd.additionalLength = 0x18;
		cmd.serviceAction = 0x000B;
		cmd.flags = (wrProt << 5) | (dpo ? 0x10 : 0) | (fua ? 0x08 : 0) | (fuaNV ? 0x02 : 0);
		cmd.lba = lba;
		cmd.initialLbaTag = initialLbaTag;
		cmd.lbaAppTag = lbaAppTag;
		cmd.lbaAppTagMask = lbaAppTagMask;
		cmd.length = length;
		return cmd;
	}

	inline SCSICommand10 WRITE_AND_VERIFY(uint8_t wrProt, bool dpo, bool byteCheck, uint32_t lba, uint8_t groupNum, uint16_t length) {
		SCSICommand10 cmd = {};
		cmd.op = 0x2E;
		cmd.data_bytes[0] = (wrProt << 5) | (dpo ? 0x10 : 0) | (byteCheck ? 0x02 : 0);
		cmd.data_bytes[1] = lba >> 24;
		cmd.data_bytes[2] = lba >> 16;
		cmd.data_bytes[3] = lba >> 8;
		cmd.data_bytes[4] = lba;
		cmd.data_bytes[5] = groupNum & 0x1F;
		cmd.data_bytes[6] = length >> 8;
		cmd.data_bytes[7] = length;
		return cmd;
	}

	inline SCSICommand12 WRITE_AND_VERIFY_12(uint8_t wrProt, bool dpo, bool byteCheck, uint32_t lba, uint32_t length, uint8_t groupNum) {
		SCSICommand12 cmd = {};
		cmd.op = 0xAE;
		cmd.data_bytes[0] = (wrProt << 5) | (dpo ? 0x10 : 0) | (byteCheck ? 0x02 : 0);
		cmd.data_bytes[1] = lba >> 24;
		cmd.data_bytes[2] = lba >> 16;
		cmd.data_bytes[3] = lba >> 8;
		cmd.data_bytes[4] = lba;
		cmd.data_bytes[5] = length >> 24;
		cmd.data_bytes[6] = length >> 16;
		cmd.data_bytes[7] = length >> 8;
		cmd.data_bytes[8] = length;
		cmd.data_bytes[9] = groupNum & 0x1F;
		return cmd;
	}

	inline SCSICommand16 WRITE_AND_VERIFY_16(uint8_t wrProt, bool dpo, bool byteCheck, uint64_t lba, uint32_t length, uint8_t groupNum) {
		SCSICommand12 cmd = {};
		cmd.op = 0x8E;
		cmd.data_bytes[0] = (wrProt << 5) | (dpo ? 0x10 : 0) | (byteCheck ? 0x02 : 0);
		cmd.data_bytes[1] = lba >> 56;
		cmd.data_bytes[2] = lba >> 48;
		cmd.data_bytes[3] = lba >> 40;
		cmd.data_bytes[4] = lba >> 32;
		cmd.data_bytes[5] = lba >> 24;
		cmd.data_bytes[6] = lba >> 16;
		cmd.data_bytes[7] = lba >> 8;
		cmd.data_bytes[8] = lba;
		cmd.data_bytes[9] = length >> 24;
		cmd.data_bytes[10] = length >> 16;
		cmd.data_bytes[11] = length >> 8;
		cmd.data_bytes[12] = length;
		cmd.data_bytes[13] = groupNum & 0x1F;
		return cmd;
	}

	inline SCSICommand32 WRITE_AND_VERIFY_32(uint8_t groupNum, uint8_t wrProt, bool dpo, bool byteCheck, uint64_t lba, uint32_t initialLbaTag,
			uint16_t lbaAppTag, uint16_t lbaAppTagMask, uint32_t length) {
		SCSICommand32 cmd = {};
		cmd.op = 0x7F;
		cmd.groupNumber = groupNum & 0x1F;
		cmd.additionalLength = 0x18;
		cmd.serviceAction = 0x000C;
		cmd.flags = (wrProt << 5) | (dpo ? 0x10 : 0) | (byteCheck ? 0x02 : 0);
		cmd.lba = lba;
		cmd.initialLbaTag = initialLbaTag;
		cmd.lbaAppTag = lbaAppTag;
		cmd.lbaAppTagMask = lbaAppTagMask;
		cmd.length = length;
		return cmd;
	}

	inline SCSICommand10 WRITE_BUFFER(uint8_t mode, uint8_t bufID, uint32_t bufOffset, uint32_t paramListLength) {
		SCSICommand10 cmd = {};
		cmd.op = 0x3B;
		cmd.data_bytes[0] = mode & 0x1F;
		cmd.data_bytes[1] = bufID;
		cmd.data_bytes[2] = bufOffset >> 16;
		cmd.data_bytes[3] = bufOffset >> 8;
		cmd.data_bytes[4] = bufOffset;
		cmd.data_bytes[5] = paramListLength >> 16;
		cmd.data_bytes[6] = paramListLength >> 8;
		cmd.data_bytes[7] = paramListLength;
		return cmd;
	}

	inline SCSICommand10 WRITE_LONG(bool correctDisable, bool wrUncorrectable, bool pBlock, uint32_t lba, uint16_t length) {
		SCSICommand10 cmd = {};
		cmd.op = 0x3F;
		cmd.data_bytes[0] = (correctDisable ? 0x80 : 0) | (wrUncorrectable ? 0x40 : 0) | (pBlock ? 0x20 : 0);
		cmd.data_bytes[1] = lba >> 24;
		cmd.data_bytes[2] = lba >> 16;
		cmd.data_bytes[3] = lba >> 8;
		cmd.data_bytes[4] = lba;
		cmd.data_bytes[6] = length >> 8;
		cmd.data_bytes[7] = length;
		return cmd;
	}

	inline SCSICommand16 WRITE_LONG_16(bool correctDisable, bool wrUncorrectable, bool pBlock, uint64_t lba, uint16_t length) {
		SCSICommand16 cmd = {};
		cmd.op = 0x9F;
		cmd.data_bytes[0] = (correctDisable ? 0x80 : 0) | (wrUncorrectable ? 0x40 : 0) | (pBlock ? 0x20 : 0);
		cmd.data_bytes[1] = lba >> 56;
		cmd.data_bytes[2] = lba >> 48;
		cmd.data_bytes[3] = lba >> 40;
		cmd.data_bytes[4] = lba >> 32;
		cmd.data_bytes[5] = lba >> 24;
		cmd.data_bytes[6] = lba >> 16;
		cmd.data_bytes[7] = lba >> 8;
		cmd.data_bytes[8] = lba;
		cmd.data_bytes[11] = length >> 8;
		cmd.data_bytes[12] = length;
		return cmd;
	}

	inline SCSICommand10 WRITE_SAME(uint8_t wrProt, bool pbData, bool lbData, uint32_t lba, uint8_t groupNum, uint16_t numBlocks) {
		SCSICommand10 cmd = {};
		cmd.op = 0x41;
		cmd.data_bytes[0] = (wrProt << 5) | (pbData ? 0x04 : 0) | (lbData ? 0x02 : 0);
		cmd.data_bytes[1] = lba >> 24;
		cmd.data_bytes[2] = lba >> 16;
		cmd.data_bytes[3] = lba >> 8;
		cmd.data_bytes[4] = lba;
		cmd.data_bytes[5] = groupNum & 0x1F;
		cmd.data_bytes[6] = numBlocks >> 8;
		cmd.data_bytes[7] = numBlocks;
		return cmd;
	}

	inline SCSICommand16 WRITE_SAME_16(uint8_t wrProt, bool pbData, bool lbData, uint64_t lba, uint32_t numBlocks, uint8_t groupNum) {
		SCSICommand16 cmd = {};
		cmd.op = 0x93;
		cmd.data_bytes[0] = (wrProt << 5) | (pbData ? 0x04 : 0) | (lbData ? 0x02 : 0);
		cmd.data_bytes[1] = lba >> 56;
		cmd.data_bytes[2] = lba >> 48;
		cmd.data_bytes[3] = lba >> 40;
		cmd.data_bytes[4] = lba >> 32;
		cmd.data_bytes[5] = lba >> 24;
		cmd.data_bytes[6] = lba >> 16;
		cmd.data_bytes[7] = lba >> 8;
		cmd.data_bytes[8] = lba;
		cmd.data_bytes[9] = numBlocks >> 24;
		cmd.data_bytes[10] = numBlocks >> 16;
		cmd.data_bytes[11] = numBlocks >> 8;
		cmd.data_bytes[12] = numBlocks;
		cmd.data_bytes[13] = groupNum & 0x1F;
		return cmd;
	}

}


#endif /* LTOS_API_BUS_SCSI_CMDS_INC_ */
