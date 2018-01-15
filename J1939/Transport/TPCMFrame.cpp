/*
 * BAMHeaderFrame.cpp
 *
 *  Created on: Oct 2, 2017
 *      Author: famez
 */

#include <string.h>
#include "TPCMFrame.h"

#define TPCM_NAME       "Transport Connection Management"


namespace J1939 {

TPCMFrame::TPCMFrame() : J1939Frame(TP_CM_PGN), mCtrlType(0), mTotalMsgSize(0), mTotalPackets(0), mMaxPackets(0),
		 mPacketsToTx(0), mNextPacket(0), mAbortReason(0), mDataPgn(0){

    mName = TPCM_NAME;

}

TPCMFrame::~TPCMFrame() {

}

void TPCMFrame::decodeData(const u8* buffer, size_t) {


	mCtrlType = buffer[0];


	switch(mCtrlType) {

	case CTRL_TPCM_RTS:
		decodeRTS(buffer + 1);
		break;

	case CTRL_TPCM_CTS:
		decodeCTS(buffer + 1);
		break;
	case CTRL_TPCM_ACK:
		decodeEndOfMsgACK(buffer + 1);
		break;
	case CTRL_TPCM_ABORT:
		decodeConnAbort(buffer + 1);
		break;
	case CTRL_TPCM_BAM:
		decodeBAM(buffer + 1);
		break;
	default:
        throw J1939DecodeException("[TPCMFrame::decodeData] Unknown Ctrl type");
		break;
	}


	mDataPgn = buffer[5] | (buffer[6] << 8) | (buffer[7] << 16);


}

void TPCMFrame::encodeData(u8* buffer, size_t ) const {

	/*
	 * If reserved, set to 0xFF
	 */
	memset(buffer, 0xFF, TP_CM_SIZE);

	buffer[0] = mCtrlType;


	switch(mCtrlType) {
	case CTRL_TPCM_RTS:
		encodeRTS(buffer + 1);
		break;

	case CTRL_TPCM_CTS:
		encodeCTS(buffer + 1);
		break;
	case CTRL_TPCM_ACK:
		encodeEndOfMsgACK(buffer + 1);
		break;
	case CTRL_TPCM_ABORT:
		encodeConnAbort(buffer + 1);
		break;
	case CTRL_TPCM_BAM:
		encodeBAM(buffer + 1);
		break;
	default:
        throw J1939DecodeException("[TPCMFrame::encodeData] Unknown Ctrl Type");
		break;
	}


	buffer[5] = mDataPgn & 0xFF;
	buffer[6] = (mDataPgn >> 8) & 0xFF;
	buffer[7] = (mDataPgn >> 16) & 0xFF;

}

void TPCMFrame::clear() {
	mCtrlType = 0;
	mTotalMsgSize = 0;
	mTotalPackets = 0;
	mMaxPackets = 0;
	mPacketsToTx = 0;
	mNextPacket = 0;
	mAbortReason = 0;

	mDataPgn = 0;
}


void TPCMFrame::decodeRTS(const u8* buffer) {

	mTotalMsgSize = buffer[0] | (buffer[1] << 8);
	mTotalPackets = buffer[2];
	mMaxPackets = buffer[3];

}

void TPCMFrame::decodeCTS(const u8* buffer) {

	mPacketsToTx = buffer[0];
	mNextPacket = buffer[1];
}

void TPCMFrame::decodeEndOfMsgACK(const u8* buffer) {
	mTotalMsgSize = buffer[0] | (buffer[1] << 8);
	mTotalPackets = buffer[2];

}

void TPCMFrame::decodeConnAbort(const u8* buffer) {
	mAbortReason = buffer[0];
}


void TPCMFrame::decodeBAM(const u8* buffer) {
	mTotalMsgSize = buffer[0] | (buffer[1] << 8);
	mTotalPackets = buffer[2];
}


void TPCMFrame::encodeRTS(u8* buffer) const {

	buffer[0] = mTotalMsgSize & 0xFF;
	buffer[1] = (mTotalMsgSize >> 8) & 0xFF;
	buffer[2] = mTotalPackets;
	buffer[3] = mMaxPackets;

}
void TPCMFrame::encodeCTS(u8* buffer) const {

	buffer[0] = mPacketsToTx;
	buffer[1] = mNextPacket;

}
void TPCMFrame::encodeEndOfMsgACK(u8* buffer) const {

	buffer[0] = mTotalMsgSize & 0xFF;
	buffer[1] = (mTotalMsgSize >> 8) & 0xFF;
	buffer[2] = mMaxPackets;

}
void TPCMFrame::encodeConnAbort(u8* buffer) const {

	buffer[0] = mAbortReason;

}
void TPCMFrame::encodeBAM(u8* buffer) const {

	buffer[0] = mTotalMsgSize & 0xFF;
	buffer[1] = (mTotalMsgSize >> 8) & 0xFF;
	buffer[2] = mMaxPackets;

}


} /* namespace J1939 */
