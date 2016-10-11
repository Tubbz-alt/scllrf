/**
 *-----------------------------------------------------------------------------
 * Title      : superconducting low level RF EPICS interface
 * ----------------------------------------------------------------------------
 * File       : scllrfAsynPortDriver.cpp
 * Author     : Garth Brown, gwbrown@slac.stanford.edu
 * Created    : June 17, 2016
 * Last update: September 6, 2016
 * ----------------------------------------------------------------------------
 * Description:
 * Prototype for FPGA register access to FPGAs using protocol defined in papers
 * given to SLAC by LBNL (Larry Doolittle). Uses asynPortDriver interface. Some of this
 * should eventually be autogenerated from json files or some such, and templates.
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

#include "scllrfAsynPortDriver.h"
#include <asynOctetSyncIO.h>
#include <asynCommonSyncIO.h>
#include <limits>
#include <netinet/in.h>
#include <iostream>
using namespace std;
#include <math.h>

/** Constructor for the scllrfAsynPortDriver class.
 * Calls constructor for the asynPortDriver base class.
 * \param[in] portName The name of the asyn port driver to be created.
 * \param[in] path The path to the peripherial as built by the builder api
 * \param[in] nelms The number of elements of this device (max addr)
 * \paarm[in] nEntries The number of asyn params to be created for each device
 *
 * */
scllrfAsynPortDriver::scllrfAsynPortDriver(const char *drvPortName, const char *netPortName, int maxAddr, int paramTableSize)
: asynPortDriver(drvPortName,
		maxAddr, /* maxAddr */
		paramTableSize,
		asynInt32Mask | asynFloat64Mask | asynOctetMask | asynDrvUserMask | asynInt32ArrayMask|asynUInt32DigitalMask, /* Interface mask */
		asynInt32Mask | asynFloat64Mask | asynOctetMask | asynEnumMask | asynInt32ArrayMask,  /* Interrupt mask */
		ASYN_CANBLOCK | ASYN_MULTIDEVICE, /* asynFlags.  This driver does block and it is multi-device, so flag is 1 */
		1, /* Autoconnect */
		epicsThreadPriorityMedium,
		0), /* Default stack size*/
		isShuttingDown_(0), netSendCount_(0), lastResponseCount_ (0), netWaitingRequests_(0),
		newWaveAvailable_(0), newWaveRead_ (0), p_RunStop (stop)
{
	asynStatus status = asynSuccess;

    /* Create the epicsMutex for locking access to communications counters from other threads */
    comCountersMutexId_ = epicsMutexCreate();
    if (!comCountersMutexId_) {
        printf("%s ERROR: epicsMutexCreate failure\n", __PRETTY_FUNCTION__);
        return;
    }
    epicsThreadSleep(defaultPollPeriod);

	createParam(RunStopString, asynParamInt32, &p_RunStop);
	createParam(MaxParallelRequestsString, asynParamInt32,
			&p_MaxParallelRequests);
	createParam(CommErrorCountString, asynParamInt32, &p_CommErrorCount);
	createParam(PollPeriodString, asynParamFloat64, &p_PollPeriod);
    createParam(ReadOneRegString, asynParamInt32Array, &p_ReadOneReg);
    createParam(WriteOneRegString, asynParamInt32Array, &p_WriteOneReg);

	// Set these early, consider adding interlock for race condition with polling loop.
    setIntegerParam(p_RunStop, stop);
    setIntegerParam(p_MaxParallelRequests, defaultMaxParallelRequests);
    setIntegerParam(p_CommErrorCount, 0);
    setDoubleParam(p_PollPeriod, defaultPollPeriod);

    pPolledRegMsg_ = new FpgaReg[1] {{0,0}};
    PolledRegMsgSize_ = 1;

    epicsThreadSleep(defaultPollPeriod);

	printf("%s created %ld parameters.\n",__PRETTY_FUNCTION__,NUM_SCLLRF_PARAMS);

	status=pasynCommonSyncIO->connect(netPortName, 0, &pCommonAsynUser_, 0);
	if(status!=asynSuccess)
		printf( "%s: connect: failed to connect to port %s with status %d\n",
				__PRETTY_FUNCTION__,netPortName, status);
	else  printf( "%s: connect: %s asynUser \"Common\" connected to port %s\n",__PRETTY_FUNCTION__, drvPortName, netPortName);

	status=pasynOctetSyncIO->connect( netPortName,0,&pOctetAsynUser_,0);
	if(status!=asynSuccess)
		printf( "%s: connect: failed to connect to Read port %s with status %d\n",
				__PRETTY_FUNCTION__,netPortName, status);
	else  printf( "%s: connect: %s asynUser \"Octet\" connected to port %s\n",__PRETTY_FUNCTION__, drvPortName,netPortName);

    // The createParam calls are queued up rather than executed right away, so
    // give them a chance to be created before we start writing to them.
    // TODO: Is there a way to check that the params were finished being created?
    epicsThreadSleep(defaultPollPeriod);

	readEventId_ = epicsEventMustCreate(epicsEventEmpty);
	startResponseHandler();

    epicsThreadSleep(defaultPollPeriod);

	pollEventId_ = epicsEventMustCreate(epicsEventEmpty);
	startPoller(defaultPollPeriod);
	
    printf("%s %s initialized and threads started.\n",__PRETTY_FUNCTION__, drvPortName);
}

void scllrfAsynPortDriver::init()
{
	return;
}
scllrfAsynPortDriver::~scllrfAsynPortDriver()
{
	isShuttingDown_ = true;
	epicsThreadSleep(0.1); // Allow threads to run and exit
	wakeupPoller();
	wakeupReader();
	epicsThreadSleep(0.1); // Allow threads to run and exit
	wakeupPoller(); // call this twice in case the poller was never set to run
	wakeupReader();
	epicsThreadSleep(0.1); // Allow threads to run and exit
	wakeupReader();
	pasynOctetSyncIO->disconnect(pOctetAsynUser_);
	pasynCommonSyncIO->disconnectDevice(pCommonAsynUser_);
	pasynCommonSyncIO->disconnect(pCommonAsynUser_);
	delete pPolledRegMsg_;
}

/** Called when you have the asyn parameter name and want the corresponding
 * register address.
  * \param[in] function From pAsynUser->reason, corresponding to a registered parameter.
  * \param[in] pToFpga Pointer to the {Address, Data} structure where the register address will be written
  * \param[in] nElements Number of elements to read. */
asynStatus scllrfAsynPortDriver::functionToRegister(const int function,
		FpgaReg *pToFpga)
{
	// base class has no registers defined
	return asynError;
}

/** Called when asyn clients call pasynInt32->read().
 * \param[in] pasynUser pasynUser structure that encodes the reason and address.
 * \param[in] value Pointer to the value to read. */
asynStatus scllrfAsynPortDriver::writeInt32(asynUser *pasynUser, epicsInt32 value)
{
	int function = pasynUser->reason;
//	int addr = 0;
	asynStatus status = asynSuccess;
    const char *paramName;
    FpgaReg regSendBuf[2];

	epicsTimeStamp timeStamp; getTimeStamp(&timeStamp);

    /* Set the parameter in the parameter library. */
    status = (asynStatus) setIntegerParam(function, value);

    /* Fetch the parameter string name for possible use in debugging */
    getParamName(function, &paramName);
    asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, "%s: function=%d, %s\n",
			__PRETTY_FUNCTION__, function, paramName);

    if (function == p_RunStop) {
        if (value == run)
        	epicsEventSignal(pollEventId_);
    }
    else {
    	// Convert function to address & FpgaReg.
    	status = functionToRegister(function, &regSendBuf[1]);
    	if (status == asynSuccess) // Yes, this function is a register write
    	{
    		asynPrint(pasynUser, ASYN_TRACEIO_DRIVER,
                "%s: found function=%d, name=%s, at address %d\n",
  			  __PRETTY_FUNCTION__, function, paramName, regSendBuf[1].addr);
    		regSendBuf[1].data = (uint32_t) value;
        	htonFpgaRegArray(regSendBuf, 2);
        	sendRegRequest(regSendBuf, 2);
    	}
    	else
    	{
    		asynPrint(pasynUser, ASYN_TRACEIO_DRIVER,
                "%s: function=%d, name=%s not a register, skip network request\n",
  			  __PRETTY_FUNCTION__, function, paramName);
    	}
    }

	/* Do callbacks so higher layers see any changes */
	status = (asynStatus) callParamCallbacks();

    if (status)
        epicsSnprintf(pasynUser->errorMessage, pasynUser->errorMessageSize,
                  "%s: status=%d, function=%d, name=%s, value=%d",
				  __PRETTY_FUNCTION__, status, function, paramName, value);
    else
        asynPrint(pasynUser, ASYN_TRACEIO_DRIVER,
              "<-- %s: function=%d, name=%s, value=%d\n",
			  __PRETTY_FUNCTION__, function, paramName, value);
    return status;
}

/** Called when asyn clients call pasynInt32Array->write().
  * The base class implementation simply prints an error message.
  * Derived classes may reimplement this function if required.
  * \param[in] pasynUser pasynUser structure that encodes the reason and address.
  * \param[in] value Pointer to the array to write.
  * \param[in] nElements Number of elements to write. */
asynStatus scllrfAsynPortDriver::writeInt32Array(asynUser *pasynUser, epicsInt32 *value,
                                size_t nElements)
{
	int function = pasynUser->reason;
	asynStatus status = asynSuccess;
//	int nCopy;
	FpgaReg regSendBuf[maxMsgSize/sizeof(FpgaReg)];
	unsigned int i, uOutBuffIndex;
	uint32_t uRegAddr;
    const char *paramName;

	//getIntegerParam(P_ArrayLength, &nCopy);
	//if ((int) nElements < nCopy)
	//	nCopy = (int) nElements;

    /* Fetch the parameter string name for possible use in debugging */
    getParamName(function, &paramName);
    printf("%s: array with %d elements, function=%d, %s\n",
				__PRETTY_FUNCTION__, (int) nElements, function, paramName);

	if(nElements < 2)
	{
		asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, "%s: array with %d elements, function=%d, %s\n",
				__PRETTY_FUNCTION__, (int) nElements, function, paramName);
		return status;
	}
	else
		asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, "--> %s: array with %d elements, function=%d, %s\n",
				__PRETTY_FUNCTION__, (int) nElements, function, paramName);

    if ( function == p_ReadOneReg )
    {
    	uOutBuffIndex = 1; // index of first register past the nonce
    	regSendBuf[1].addr = uReadOneRegAddr = value[0] | flagReadMask;
    	regSendBuf[1].data = value[1];
    }
    else if ( function == p_WriteOneReg  )
    {
    	uOutBuffIndex = 1; // index of first register past the nonce
    	regSendBuf[1].addr = uWriteOneRegAddr = value[0];
    	regSendBuf[1].data = value[1];
    }
    else
    {
    	status = functionToRegister(function, &regSendBuf[1]);
    	if (status != asynSuccess)
    		return status;

    	uRegAddr = regSendBuf[1].addr;
    	uOutBuffIndex = 1; // index of first register past the nonce

    	for(i=0; i<nElements; ++i, ++uOutBuffIndex)
    	{
    		regSendBuf[uOutBuffIndex].data = (int32_t) value[i];
    		regSendBuf[uOutBuffIndex].addr = (uint32_t) uRegAddr + i;

    		// If there's more to send than will fit in the max message size, break
    		// it up into chunks and send each chunk individually
    		if(uOutBuffIndex == maxMsgSize/sizeof(FpgaReg)-1)
    		{
    			htonFpgaRegArray(regSendBuf, uOutBuffIndex+1);
    			sendRegRequest(regSendBuf, uOutBuffIndex+1);

    			if (status)
    				epicsSnprintf(pasynUser->errorMessage, pasynUser->errorMessageSize,
    						"%s: status=%d, function=%d, sending array segment %u of %s", __PRETTY_FUNCTION__,
							status, function, i/uOutBuffIndex +1, paramName);
    			else
    				asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, "%s: function=%d, sending array segment %u of %s\n",
    						__PRETTY_FUNCTION__, function, i/uOutBuffIndex +1, paramName);
    			uOutBuffIndex = 0; // loop will increment to index of first register past the nonce
    		}
    	}
    }

	htonFpgaRegArray(regSendBuf, uOutBuffIndex+1);
	sendRegRequest(regSendBuf, uOutBuffIndex+1);

	if (status)
		epicsSnprintf(pasynUser->errorMessage, pasynUser->errorMessageSize,
				"%s: status=%d, function=%d, sent %s", __PRETTY_FUNCTION__,
				status, function, paramName);
	else
		asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, "<-- %s: function=%d, sent %s\n",
				__PRETTY_FUNCTION__, function, paramName);

	return status; //(writeArray < epicsInt32 > (pasynUser, value, nElements));
}

typedef struct
{
	FpgaReg* sendBuff;
	size_t sendBuffSize;
	scllrfAsynPortDriver* pDriver;
} regPollerThreadArgs;

static void regPollerC(void *drvPvt)
{
	scllrfAsynPortDriver *pscllrfDriver = (scllrfAsynPortDriver*)drvPvt;
	pscllrfDriver->regPoller();
	printf("%s: exiting\n", __PRETTY_FUNCTION__);
}

/** Starts the poller thread.
 ** Derived classes will typically call this at near the end of their constructor.
 ** Derived classes can typically use the base class implementation of the poller thread,
 ** but are free to re-implement it if necessary.
 ** \param[in] pollPeriod The time between polls. */
asynStatus scllrfAsynPortDriver::startPoller(double pollPeriod)
{
	pollPeriod_ = pollPeriod;
	epicsThreadCreate("asynPoller",
			epicsThreadPriorityMedium,
			epicsThreadGetStackSize(epicsThreadStackMedium),
			(EPICSTHREADFUNC)regPollerC, (void *)this);
	return asynSuccess;
}

void scllrfAsynPortDriver::regPoller()
{
	epicsEventWaitStatus status;
    int runStop;

	epicsEventWait(pollEventId_); // Block when first created, to give subclass constructors a chance to finish
	while(1) {
		getDoubleParam(p_PollPeriod, &pollPeriod_);
        getIntegerParam(p_RunStop, &runStop);
		if (runStop == run && pollPeriod_ != 0.0) status = epicsEventWaitWithTimeout(pollEventId_, pollPeriod_);
		else               status = epicsEventWait(pollEventId_);
		if (status == epicsEventWaitOK) {
			/* We got an event, rather than a timeout.  This is because other software
			 ** knows that we should do a poll.
			 **/
		}
		if (isShuttingDown_) {
			break;
		}
		sendRegRequest(pPolledRegMsg_, PolledRegMsgSize_);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: woke up and sent a poll\n", __PRETTY_FUNCTION__);
	}
	printf("%s: exiting\n", __PRETTY_FUNCTION__);
}

/** Wakes up the poller thread to make it start polling. */
asynStatus scllrfAsynPortDriver::wakeupPoller()
{
	epicsEventSignal(pollEventId_);
	return asynSuccess;
}

/** Polls the device
  * \param[in] regBuffer array of FpgaReg messages in network byte order.
  * Note that the first "register" regBuffer[0] is really the nonce, not data.
  * This function will set the nonce before sending.
  * Other than the nonce, data in the array is not changed. This reduces the
  * processing required, since most messages are canned and repeated at regular intervals.
  * \param[in] regBuffCount is the number of FpgaReg type elements, including the nonce
 **  */
asynStatus scllrfAsynPortDriver::sendRegRequest(FpgaReg *regBuffer, unsigned int regBuffCount)
{
	assert(regBuffer != NULL);
	assert(regBuffCount >= 1);
	size_t writtenCount;
	epicsInt32 errorCount;
	asynStatus status = asynSuccess;
	epicsMutexLockStatus mutexStatus;
	char * pWriteBuffer;
	pWriteBuffer = reinterpret_cast<char*>(regBuffer);
	int maxParallelRequests;
	getIntegerParam(p_MaxParallelRequests, &maxParallelRequests);

	asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER, "--> %s( regBuffer=%p, regBuffCount=%u\n",
					__PRETTY_FUNCTION__, regBuffer, regBuffCount);
	// Throttle so that we don't overflow buffers if response handling falls behind
	if( netWaitingRequests_ > (unsigned) maxParallelRequests )
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: too many requests waiting for responses (%u), throttling requests.\n",__PRETTY_FUNCTION__, maxParallelRequests);
	while( netWaitingRequests_ > (unsigned) maxParallelRequests )
	{
		if (isShuttingDown_)
		{
			return asynDisconnected;
		}
		epicsThreadSleep(throttleLoopDelay);
	}

	mutexStatus = epicsMutexLock(comCountersMutexId_); // protect netSendCount and netWaitingRequests
	++netSendCount_; // increment and roll over if needed
	// use the nonce at the start of the buffer for register count, and
	// send counter. Can be used for error checking.
	regBuffer[0] = (FpgaReg)
							{ (uint32_t) htonl(netSendCount_), (int32_t) htonl(regBuffCount*sizeof(FpgaReg)) };

	status = pasynOctetSyncIO->write(pOctetAsynUser_, pWriteBuffer,
			regBuffCount*sizeof(FpgaReg), writeTimeout, &writtenCount);
	if (status != asynSuccess)
	{
		asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,"%s: failed to write. %s\n", __PRETTY_FUNCTION__,
				pOctetAsynUser_->errorMessage);
		getIntegerParam(p_CommErrorCount, &errorCount);
		setIntegerParam(p_CommErrorCount, errorCount + 1);
		callParamCallbacks();
	}
	else
	{
		++netWaitingRequests_;
	}
	epicsMutexUnlock(comCountersMutexId_);
	epicsThreadSleep(0); // to allow other threads to take the CPU
	wakeupReader();

	asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER, "<-- %s( regBuffer=%p, regBuffCount=%u\n",
					__PRETTY_FUNCTION__, regBuffer, regBuffCount);
	return asynSuccess;
}


static void responseHandlerC(void *drvPvt)
{
	scllrfAsynPortDriver *pscllrfDriver = (scllrfAsynPortDriver*)drvPvt;
	pscllrfDriver->responseHandler();
	printf("%s: exiting\n", __PRETTY_FUNCTION__);
}

/** Starts the poller thread.
 ** Derived classes will typically call this at near the end of their constructor.
 ** Derived classes can typically use the base class implementation of the poller thread,
 ** but are free to re-implement it if necessary.
 ** \param[in] pollPeriod The time between polls. */
asynStatus scllrfAsynPortDriver::startResponseHandler()
{
	readTimeout_ = readTimeout;
	epicsThreadCreate("responseHandler",
			epicsThreadPriorityMedium, epicsThreadGetStackSize(epicsThreadStackMedium),
			(EPICSTHREADFUNC) responseHandlerC, (void *) this);
	return asynSuccess;
}

/** Wakes up the reader thread to check for data. */
asynStatus scllrfAsynPortDriver::wakeupReader()
{
	epicsEventSignal(readEventId_);
	return asynSuccess;
}

void scllrfAsynPortDriver::responseHandler()
{
	asynStatus status;
	epicsEventWaitStatus waitStatus;
	epicsMutexLockStatus mutexStatus;
	static char pReadBuffer[maxMsgSize];
	FpgaReg *pRegReadback;
	size_t readCount = 0; // Number of bytes to process from the network read
	epicsInt32 errorCount;
	int eomReason;
	int noDataCounter = 0; // If we try to read this many times and get no data, give up and decrement netWaitingRequests_

	while (1)
	{

		// The asyn framework doesn't allow writes while a read is blocking,
		// so the same behavior is approximated here with events.
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: Caught up with message processing, blocking for data\n", __PRETTY_FUNCTION__);
		waitStatus = epicsEventWaitWithTimeout(readEventId_, readTimeout_);

		if (isShuttingDown_)
		{
			break;
		}
		if (waitStatus == epicsEventWaitOK) // We got an event, signaling to expect data back
		{
			while (netWaitingRequests_>0) // Writers increment atomic netWaitingRequests when they send a request
			{
				// Note: pRegReadback/pReadBuffer would have reentrancy problems, except only this one
				// thread reads it. More than one read thread and it could break.
				pRegReadback = reinterpret_cast<FpgaReg*>(pReadBuffer);
				pRegReadback[0] = (FpgaReg) {0, 0}; // Reset count, so we won't try to process data that isn't there.

				mutexStatus = epicsMutexLock(comCountersMutexId_); // protect netWaitingRequests from being modified by the write thread

				// First read just the nonce for a sanity check.
				status = pasynOctetSyncIO->read(pOctetAsynUser_, pReadBuffer,
						nonceSize*sizeof(FpgaReg), 0.01, &readCount, &eomReason);

				// Prevent an endless loop here while senders are throttled
				if(readCount == 0)
				{
					++noDataCounter;
					if(noDataCounter > (int)(defaultPollPeriod/throttleLoopDelay))
						netWaitingRequests_--;
					epicsMutexUnlock(comCountersMutexId_); // protect netWaitingRequests from being modified by the write thread
					continue;
				}
				else // Get the amount of data to read from the nonce presumably read in, and read that many more bytes
				{
					noDataCounter = 0;
					if(ntohl(pRegReadback[0].data) < (int) maxMsgSize)
					{ // read in the number of bytes the nonce says to expect, starting with the second register location
						asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,"%s: read %u byte nonce, says sequence # %u with %d bytes.\n",
								__PRETTY_FUNCTION__, (unsigned) readCount, ntohl(pRegReadback[0].addr), ntohl(pRegReadback[0].data));
						status = pasynOctetSyncIO->read(pOctetAsynUser_, &pReadBuffer[nonceSize*sizeof(FpgaReg)],
								ntohl(pRegReadback[0].data)-nonceSize*sizeof(FpgaReg), 0.01, &readCount, &eomReason);
						readCount += nonceSize*sizeof(FpgaReg);
						asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,"%s: got %u bytes total for response #%u\n",
								__PRETTY_FUNCTION__, (unsigned) readCount, ntohl(pRegReadback[0].addr));
					}
					else
					{
						netWaitingRequests_--;
						asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
							"%s: presumed nonce says sequence # %u with %d bytes.\n",
							__PRETTY_FUNCTION__, ntohl(pRegReadback[0].addr), ntohl(pRegReadback[0].data));
						epicsMutexUnlock(comCountersMutexId_); // protect netWaitingRequests from being modified by the write thread
						getIntegerParam(p_CommErrorCount, &errorCount);
						setIntegerParam(p_CommErrorCount, errorCount + 1);
						callParamCallbacks();
						continue;
					}

				}
				epicsMutexUnlock(comCountersMutexId_); // protect netWaitingRequests from being modified by the write thread

				ntohFpgaRegArray(pRegReadback, (readCount)/sizeof(FpgaReg));

				processReadbackBuffer(pRegReadback, readCount);
				/* Do callbacks so higher layers see any changes */
				status = (asynStatus) callParamCallbacks();

				epicsThreadSleep(0); // sleep to be nice to other threads
			} // while (netWaitingRequests>0)
		} // if (status == epicsEventWaitOK)
		else // Timed out waiting for sender to signal. Could be a network glitch, so count request as lost.
		{
			if(netWaitingRequests_>0)
			{
				asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
						"%s: receive thread timed out after %fs waiting for %d responses. Network losses?\n", __PRETTY_FUNCTION__, readTimeout_, netWaitingRequests_);
				netWaitingRequests_--;
			}
		}
		// Clear out any cruft left over that we didn't process.
		pasynOctetSyncIO->flush(pOctetAsynUser_);
	}
}

// Iterate over each register in the buffer returned from the FPGA
// Array passed to this function contains the nonce, with the buffer
// size at pRegReadback[0].data
asynStatus scllrfAsynPortDriver::processReadbackBuffer(FpgaReg *pRegReadback, unsigned int readCount)
{
	unsigned i;
	bool waveIsReady = false; // Made this local rather than member because of concurrency concerns
	asynStatus status = asynSuccess;
	int bytesLeft = readCount; // signed to make error detection easier
	epicsInt32 errorCount;

	// Check for an invalid nonce
	if(pRegReadback[0].addr < 1 || pRegReadback[0].data < 1)
		return asynError;

	// We put the message size in the data of the first buffer element for error checking
	// and to handle multiple messages received
	while(bytesLeft > 0)
	{

		if (isShuttingDown_)
		{
			break;
		}

		// We can read partial messages, so watch array bounds if received message size is smaller than expected
		if (bytesLeft < pRegReadback[0].data)
		{
			//pRegReadback[0].data = bytesLeft; // Testing made clear this isn't enough, so just bail.
			pasynOctetSyncIO->flush(pOctetAsynUser_); // Should we clear out waiting partial messages?
			if(netWaitingRequests_ > 0)
				netWaitingRequests_--;
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
					"%s: Read %u bytes from network, %d bytes left to process, but expected %u bytes\n",
					__PRETTY_FUNCTION__, readCount, bytesLeft, (unsigned) pRegReadback[0].data);
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
					"%s: Response from request #%u, expected is request #%u\n",
					__PRETTY_FUNCTION__, (unsigned) pRegReadback[0].addr,
					lastResponseCount_ + 1);
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
					"%s: Discarding this message\n",
					__PRETTY_FUNCTION__);
			getIntegerParam(p_CommErrorCount, &errorCount);
			setIntegerParam(p_CommErrorCount, errorCount + 1);
			return asynError;
		}

		if (pRegReadback[0].addr > lastResponseCount_ + 1)
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
					"%s: Missing response message, got response from request #%u, expected is request #%u\n",
					__PRETTY_FUNCTION__, (unsigned) pRegReadback[0].addr,
					lastResponseCount_ + 1);

			getIntegerParam(p_CommErrorCount, &errorCount);
			setIntegerParam(p_CommErrorCount, errorCount + 1);
		}
		if (pRegReadback[0].addr < lastResponseCount_ + 1)
		{// NOTE: In testing so far, it has only reached here from data problems other than true out-of-order data
			asynPrint(pOctetAsynUser_, ASYN_TRACE_ERROR,
					"%s: Out-of-order response message, got response from request #%u, expected is request #%u\n",
					__PRETTY_FUNCTION__, (unsigned) pRegReadback[0].addr,
					lastResponseCount_ + 1);

			getIntegerParam(p_CommErrorCount, &errorCount);
			setIntegerParam(p_CommErrorCount, errorCount + 1);
		}
		lastResponseCount_ = pRegReadback[0].addr;

		// Data read from the network may contain multiple responses. Take them one at a time.
		// Use the register count from the nonce in pRegReadback[0].data to determine how much
		// to process before the next response with its own nonce.
		for (i = 1; i < pRegReadback[0].data / sizeof(FpgaReg); ++i)
		{
			if(pRegReadback[i].addr & flagReadMask)
			{
				status = processRegReadback(&pRegReadback[i], waveIsReady);
			}
			else
			{
				processRegWriteResponse(&pRegReadback[i]);
			}
			if (status)
				epicsSnprintf(pOctetAsynUser_->errorMessage,
						pOctetAsynUser_->errorMessageSize,
						"%s: status=%d, couldn't process address=0x%x, value=%u",
						__PRETTY_FUNCTION__, status,
						(unsigned) pRegReadback[i].addr,
						(unsigned) pRegReadback[i].data);
			//		else
			//			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
			//					"%s: address=0x%x, value=%u\n", __PRETTY_FUNCTION__,
			//					(unsigned ) pRegReadback[i].addr,
			//					(unsigned ) pRegReadback[i].data);
		}

		// check if this was a response to the most recent request
		if (pRegReadback[0].addr < netSendCount_ )
		{
			asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
					"%s: processed response from request #%u, most recent is request #%u\n",
					__PRETTY_FUNCTION__, (unsigned) pRegReadback[0].addr,
					netSendCount_);
			netWaitingRequests_ = netSendCount_ - pRegReadback[0].addr;
		}
		else
			netWaitingRequests_ = 0; // We have the most recent, no outstanding requests

		// decrement number of bytes left to process by the numbr of bytes we just processed
		if(bytesLeft > pRegReadback[0].data)
			bytesLeft -= pRegReadback[0].data;
		else // guard against mangled/partial messages
			bytesLeft = 0;

		// The nonce in pRegReadback[0] contains the message size. Move pointer to the next message.
		pRegReadback = & pRegReadback[pRegReadback[0].data/sizeof(FpgaReg)];
	} // while(bytesLeft > 0)

	// If the waveIsReady flag is set,
	if (waveIsReady &&
			// and there isn't a pending waveform read
			(newWaveAvailable_ == newWaveRead_))
	{
		// Set the message counter with a "new waveform" notification
		// to the message counter value for the message we just received
		newWaveAvailable_ = pRegReadback[0].addr;
		epicsEventSignal(reqWaveEventId_);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,"%s: new waveform data available, signaling the waveform requester\n",
				__PRETTY_FUNCTION__);
	}

	return asynSuccess;
}

/**  Extract register address and data from the received message and set the appropriate
 * asyn parameter.
 * Some registers have a "new waveform data ready" flag. If they have this and it is set,
 * set weveIsReady to true.
 * Note: This function should not set waveIsReady to false. That is done by a loop in the
 * calling function.
* \param[in] pFromFpga Data returned from the FPGA for a single register
* \param[in] waveIsReady A flag that gets set to true if the appropriate bit was set by the FPGA
*/
asynStatus scllrfAsynPortDriver::processRegReadback(const FpgaReg *pFromFpga, bool &waveIsReady)
{
	epicsInt32 iReg[2];
	// Base class only has a generic register. Decode if appropriate
	if(pFromFpga->addr == (uReadOneRegAddr|flagReadMask))
	{
		iReg[0] = pFromFpga->addr;
		iReg[1] = pFromFpga->data;
		doCallbacksInt32Array(iReg, 2, p_ReadOneReg, 0);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: got one reg read response.\n", __PRETTY_FUNCTION__);
	}
	return asynSuccess;
}

/**  Extract register address and data from the received message and set the appropriate
 * asyn parameter.
 * Some registers have a "new waveform data ready" flag. If they have this and it is set,
 * set weveIsReady to true.
 * Note: This function should not set waveIsReady to false. That is done by a loop in the
 * calling function.
* \param[in] pFromFpga Data returned from the FPGA for a single register
* \param[in] waveIsReady A flag that gets set to true if the appropriate bit was set by the FPGA
*/
asynStatus scllrfAsynPortDriver::processRegWriteResponse(const FpgaReg *pFromFpga)
{
	epicsInt32 iReg[2];

	// Base class only has a generic register. Decode if appropriate
	if(pFromFpga->addr == (uWriteOneRegAddr))
	{
		iReg[0] = pFromFpga->addr;
		iReg[1] = pFromFpga->data;
		doCallbacksInt32Array(iReg, 2, p_WriteOneReg, 0);
		asynPrint(pOctetAsynUser_, ASYN_TRACEIO_DRIVER,
				"%s: got one reg write response.\n", __PRETTY_FUNCTION__);
	}
	return asynSuccess;
	return asynSuccess;
}

void htonFpgaRegArray(FpgaReg *buffer, unsigned int regCount)
{
	unsigned int i;

	for (i=0; i<regCount; ++i)
	{
		buffer[i].addr = htonl(buffer[i].addr);
		buffer[i].data = htonl(buffer[i].data);
	}
}


void ntohFpgaRegArray(FpgaReg *buffer, unsigned int regCount)
{
	unsigned int i;

	for (i=0; i<regCount; ++i)
	{
		buffer[i].addr = ntohl(buffer[i].addr);
		buffer[i].data = ntohl(buffer[i].data);
	}
}


extern "C" {

/* Configuration routine.  Called directly, or from the iocsh function below */

/** EPICS iocsh callable function to call constructor for the scllrfAsynPortDriver class.
 * \param[in] portName The name of the asyn port driver to be created.
 * \param[in] netPortName The name of the asynIPport this will use to communicate */
int scllrfAsynPortDriverConfigure(const char *drvPortName, const char *netPortName)
{
	new scllrfAsynPortDriver(drvPortName, netPortName, 0, 7);
	return asynSuccess;
}


/* EPICS iocsh shell commands */

static const iocshArg initArg0 = { "drvPortName",iocshArgString};
static const iocshArg initArg1 = { "IP port name",iocshArgString};
static const iocshArg * const initArgs[] = {&initArg0,
		&initArg1};
static const iocshFuncDef initFuncDef = {"scllrfAsynPortDriverConfigure",2,initArgs};
static void initCallFunc(const iocshArgBuf *args)
{
	scllrfAsynPortDriverConfigure(args[0].sval, args[1].sval);
}

void scllrfAsynPortDriverRegister(void)
{
	iocshRegister(&initFuncDef,initCallFunc);
}

epicsExportRegistrar(scllrfAsynPortDriverRegister);

}

