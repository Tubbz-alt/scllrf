#ifndef SCLLRFRESEXTRA_DRIVER_H
#define SCLLRFRESEXTRA_DRIVER_H

/**
 *-----------------------------------------------------------------------------
 * Title      : superconducting low level RF EPICS interface
 * ----------------------------------------------------------------------------
 * File       : scllrfRESDriver.h
 * Author     : Garth Brown, gwbrown@slac.stanford.edu
 * Created    : June 17, 2016
 * Last update: September 6, 2016
 * ----------------------------------------------------------------------------
 * Description:
 * Header defining register interface to scllrfRES, autogenerated from a python
 * dictionary.
 *
 * ----------------------------------------------------------------------------
 * This file is part of LCLS II. It is subject to
 * the license terms in the LICENSE.txt file found in the top-level directory
 * of this distribution and at:
    * https://confluence.slac.stanford.edu/display/ppareg/LICENSE.html.
 * No part of LCLS II, including this file, may be
 * copied, modified, propagated, or distributed except according to the terms
 * contained in the LICENSE.txt file.
 * ----------------------------------------------------------------------------
**/
#include "scllrfRES.h"

class PiezoWave
{
// Waveforms are read by reading the same address 1024 times
// 8 waveforms per piezo card, 4 piezo cards
// Data is 18 bit, sign extended to 32 bits
public:
	static const unsigned piezoWaveRegCount = 1024;
	static const unsigned piezoWaveSegmentCount = (piezoWaveRegCount
			+ maxRegPerMsg - 1) / maxRegPerMsg; // # of UDP requests, divide and round up
	static const unsigned piezoWaveReqMsgSize = piezoWaveRegCount
			+ piezoWaveSegmentCount; // All register addresses plus nonce space
public:
	PiezoWave(unsigned int addr, int index, epicsInt32 readBuffer[]); // addr: The address to repeatedly request
	epicsInt32* waveReadback;
	int paramIndex;
// For these waveforms, you need to keep count of the number of reads to know where the end is
	unsigned int readCount;
	FpgaReg requestMsg[piezoWaveReqMsgSize];
	bool readInProgress;
	bool keep; // a flag to "turn off" this waveform. There is no equivalent in the FPGA
	asynStatus processPiezoBufReadback(const FpgaReg *pFromFpga, scllrfRESDriver *pDriver);
};

 /* This class implements the chassis type specific driver. */
class scllrfRESextra : public scllrfRESDriver {
public:
	scllrfRESextra(const char *drvPortName, const char *netPortName);
	virtual ~scllrfRESextra();
	void piezoBufRequester();

protected:
	static const unsigned int maxChannel;
	virtual asynStatus processRegReadback(const FpgaReg *pFromFpga,
			bool &waveIsReady); // parse register data, write to PVs
	asynStatus processPiezoBufReadback(const FpgaReg *pFromFpga, PiezoWave &waveToFill); // parse register data, write to array PV
	asynStatus startPiezoBufRequester(); // For system startup
	epicsEvent reqPiezoWave_; /**< Event ID to signal the waveform requester */


// Piezo 1 chan 1 current n
	PiezoWave Piezo1C1InWave_;
// Piezo 1 chan 1 current p
	PiezoWave Piezo1C1IpWave_;
// Piezo 1 chan 1 amp drive voltage
	PiezoWave Piezo1C1DriveVWave_;
// Piezo 1 chan 1 out voltage scaled
	PiezoWave Piezo1C1OutVWave_;
// Piezo 1 chan 2 current n
	PiezoWave Piezo1C2InWave_;
// Piezo 1 chan 2 current p
	PiezoWave Piezo1C2IpWave_;
// Piezo 1 chan 2 amp drive voltage
	PiezoWave Piezo1C2DriveVWave_;
// Piezo 1 chan 2 out voltage scaled
	PiezoWave Piezo1C2OutVWave_;
// Piezo 2 chan 1 current n
	PiezoWave Piezo2C1InWave_;
// Piezo 2 chan 1 current p
	PiezoWave Piezo2C1IpWave_;
// Piezo 2 chan 1 amp drive voltage
	PiezoWave Piezo2C1DriveVWave_;
// Piezo 2 chan 1 out voltage scaled
	PiezoWave Piezo2C1OutVWave_;
// Piezo 2 chan 2 current n
	PiezoWave Piezo2C2InWave_;
// Piezo 2 chan 2 current p
	PiezoWave Piezo2C2IpWave_;
// Piezo 2 chan 2 amp drive voltage
	PiezoWave Piezo2C2DriveVWave_;
// Piezo 2 chan 2 out voltage scaled
	PiezoWave Piezo2C2OutVWave_;
// Piezo 3 chan 1 current n
	PiezoWave Piezo3C1InWave_;
// Piezo 3 chan 1 current p
	PiezoWave Piezo3C1IpWave_;
// Piezo 3 chan 1 amp drive voltage
	PiezoWave Piezo3C1DriveVWave_;
// Piezo 3 chan 1 out voltage scaled
	PiezoWave Piezo3C1OutVWave_;
// Piezo 3 chan 2 current n
	PiezoWave Piezo3C2InWave_;
// Piezo 3 chan 2 current p
	PiezoWave Piezo3C2IpWave_;
// Piezo 3 chan 2 amp drive voltage
	PiezoWave Piezo3C2DriveVWave_;
// Piezo 3 chan 2 out voltage scaled
	PiezoWave Piezo3C2OutVWave_;
// Piezo 4 chan 1 current n
	PiezoWave Piezo4C1InWave_;
// Piezo 4 chan 1 current p
	PiezoWave Piezo4C1IpWave_;
// Piezo 4 chan 1 amp drive voltage
	PiezoWave Piezo4C1DriveVWave_;
// Piezo 4 chan 1 out voltage scaled
	PiezoWave Piezo4C1OutVWave_;
// Piezo 4 chan 2 current n
	PiezoWave Piezo4C2InWave_;
// Piezo 4 chan 2 current p
	PiezoWave Piezo4C2IpWave_;
// Piezo 4 chan 2 amp drive voltage
	PiezoWave Piezo4C2DriveVWave_;
// Piezo 4 chan 2 out voltage scaled
	PiezoWave Piezo4C2OutVWave_;

	static const char *PiezoWaveKeepString;
/* Register handles */
    int p_PiezoWaveKeep;
    #define FIRST_SCLLRFRESEXTRA_PARAM p_PiezoWaveKeep
    #define LAST_SCLLRFRESEXTRA_PARAM p_PiezoWaveKeep

#define NUM_SCLLRFRESEXTRA_PARAMS (&LAST_SCLLRFRESEXTRA_PARAM - &FIRST_SCLLRFRESEXTRA_PARAM + NUM_SCLLRF_PARAMS + 1)

//    	Piezo1C1InRAdr = 0x00000070,
//    	Piezo1C1IpRAdr = 0x00000071,
//    	Piezo1C1OutVRAdr = 0x00000072,
//    	Piezo1C1DriveVRAdr = 0x00000073,
//    	Piezo1C2InRAdr = 0x00000074,
//    	Piezo1C2IpRAdr = 0x00000075,
//    	Piezo1C2OutVRAdr = 0x00000076,
//    	Piezo1C2DriveVRAdr = 0x00000077,
//    	Piezo2C1InRAdr = 0x00000080,
//    	Piezo2C1IpRAdr = 0x00000081,
//    	Piezo2C1OutVRAdr = 0x00000082,
//    	Piezo2C1DriveVRAdr = 0x00000083,
//    	Piezo2C2InRAdr = 0x00000084,
//    	Piezo2C2IpRAdr = 0x00000085,
//    	Piezo2C2OutVRAdr = 0x00000086,
//    	Piezo2C2DriveVRAdr = 0x00000087,
//    	Piezo3C1InRAdr = 0x00000090,
//    	Piezo3C1IpRAdr = 0x00000091,
//    	Piezo3C1OutVRAdr = 0x00000092,
//    	Piezo3C1DriveVRAdr = 0x00000093,
//    	Piezo3C2InRAdr = 0x00000094,
//    	Piezo3C2IpRAdr = 0x00000095,
//    	Piezo3C2OutVRAdr = 0x00000096,
//    	Piezo3C2DriveVRAdr = 0x00000097,
//    	Piezo4C1InRAdr = 0x000000A0,
//    	Piezo4C1IpRAdr = 0x000000A1,
//    	Piezo4C1OutVRAdr = 0x000000A2,
//    	Piezo4C1DriveVRAdr = 0x000000A3,
//    	Piezo4C2InRAdr = 0x000000A4,
//    	Piezo4C2IpRAdr = 0x000000A5,
//    	Piezo4C2OutVRAdr = 0x000000A6,
//    	Piezo4C2DriveVRAdr = 0x000000A7,
};

#endif

