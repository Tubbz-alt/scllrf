#ifndef SCLLRFINT_DRIVER_H
#define SCLLRFINT_DRIVER_H

/**
 *-----------------------------------------------------------------------------
 * Title      : superconducting low level RF EPICS interface
 * ----------------------------------------------------------------------------
 * File       : templateScllrfDriver.h
 * Author     : Garth Brown, gwbrown@slac.stanford.edu
 * Created    : June 17, 2016
 * Last update: September 6, 2016
 * ----------------------------------------------------------------------------
 * Description:
 * Header defining register interface to scllrfINT, autogenerated from a python
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
#include "scllrfAsynPortDriver.h"

/* Registers */
static const char *C1Plfep1IRString = "C1PLFEP1I_R";
static const char *C1Plfep1IlhRString = "C1PLFEP1ILH_R";
static const char *C1Plfep1IlhWString = "C1PLFEP1ILH_W";
static const char *C1Plfep1IllRString = "C1PLFEP1ILL_R";
static const char *C1Plfep1IllWString = "C1PLFEP1ILL_W";
static const char *C1Plfep2IRString = "C1PLFEP2I_R";
static const char *C1Plfep2IlhRString = "C1PLFEP2ILH_R";
static const char *C1Plfep2IlhWString = "C1PLFEP2ILH_W";
static const char *C1Plfep2IllRString = "C1PLFEP2ILL_R";
static const char *C1Plfep2IllWString = "C1PLFEP2ILL_W";
static const char *C1Plfep3IRString = "C1PLFEP3I_R";
static const char *C1Plfep3IlhRString = "C1PLFEP3ILH_R";
static const char *C1Plfep3IlhWString = "C1PLFEP3ILH_W";
static const char *C1Plfep3IllRString = "C1PLFEP3ILL_R";
static const char *C1Plfep3IllWString = "C1PLFEP3ILL_W";
static const char *C1Pltmp1IRString = "C1PLTMP1I_R";
static const char *C1Pltmp1IWString = "C1PLTMP1I_W";
static const char *C1Pltmp1VRString = "C1PLTMP1V_R";
static const char *C1Pltmp1VlRString = "C1PLTMP1VL_R";
static const char *C1Pltmp1VlWString = "C1PLTMP1VL_W";
static const char *C1Pltmp2IRString = "C1PLTMP2I_R";
static const char *C1Pltmp2IWString = "C1PLTMP2I_W";
static const char *C1Pltmp2VRString = "C1PLTMP2V_R";
static const char *C1Pltmp2VlRString = "C1PLTMP2VL_R";
static const char *C1Pltmp2VlWString = "C1PLTMP2VL_W";
static const char *C1StmpiRString = "C1STMPI_R";
static const char *C1StmpiWString = "C1STMPI_W";
static const char *C1StmpvRString = "C1STMPV_R";
static const char *C1StmpvlRString = "C1STMPVL_R";
static const char *C1StmpvlWString = "C1STMPVL_W";
static const char *C1WalRString = "C1WAL_R";
static const char *C1WalWString = "C1WAL_W";
static const char *C1WapRString = "C1WAP_R";
static const char *C1WapWString = "C1WAP_W";
static const char *C1WapsRString = "C1WAPS_R";
static const char *C1WapsWString = "C1WAPS_W";
static const char *C1WavRString = "C1WAV_R";
static const char *C1WwlRString = "C1WWL_R";
static const char *C1WwlWString = "C1WWL_W";
static const char *C1WwtRString = "C1WWT_R";
static const char *C2Plfep1IRString = "C2PLFEP1I_R";
static const char *C2Plfep1IlhRString = "C2PLFEP1ILH_R";
static const char *C2Plfep1IlhWString = "C2PLFEP1ILH_W";
static const char *C2Plfep1IllRString = "C2PLFEP1ILL_R";
static const char *C2Plfep1IllWString = "C2PLFEP1ILL_W";
static const char *C2Plfep2IRString = "C2PLFEP2I_R";
static const char *C2Plfep2IlhRString = "C2PLFEP2ILH_R";
static const char *C2Plfep2IlhWString = "C2PLFEP2ILH_W";
static const char *C2Plfep2IllRString = "C2PLFEP2ILL_R";
static const char *C2Plfep2IllWString = "C2PLFEP2ILL_W";
static const char *C2Plfep3IRString = "C2PLFEP3I_R";
static const char *C2Plfep3IlhRString = "C2PLFEP3ILH_R";
static const char *C2Plfep3IlhWString = "C2PLFEP3ILH_W";
static const char *C2Plfep3IllRString = "C2PLFEP3ILL_R";
static const char *C2Plfep3IllWString = "C2PLFEP3ILL_W";
static const char *C2Pltmp1IRString = "C2PLTMP1I_R";
static const char *C2Pltmp1IWString = "C2PLTMP1I_W";
static const char *C2Pltmp1VRString = "C2PLTMP1V_R";
static const char *C2Pltmp1VlRString = "C2PLTMP1VL_R";
static const char *C2Pltmp1VlWString = "C2PLTMP1VL_W";
static const char *C2Pltmp2IRString = "C2PLTMP2I_R";
static const char *C2Pltmp2IWString = "C2PLTMP2I_W";
static const char *C2Pltmp2VRString = "C2PLTMP2V_R";
static const char *C2Pltmp2VlRString = "C2PLTMP2VL_R";
static const char *C2Pltmp2VlWString = "C2PLTMP2VL_W";
static const char *C2StmpiRString = "C2STMPI_R";
static const char *C2StmpiWString = "C2STMPI_W";
static const char *C2StmpvRString = "C2STMPV_R";
static const char *C2StmpvlRString = "C2STMPVL_R";
static const char *C2StmpvlWString = "C2STMPVL_W";
static const char *C2WalRString = "C2WAL_R";
static const char *C2WalWString = "C2WAL_W";
static const char *C2WapRString = "C2WAP_R";
static const char *C2WapWString = "C2WAP_W";
static const char *C2WapsRString = "C2WAPS_R";
static const char *C2WapsWString = "C2WAPS_W";
static const char *C2WavRString = "C2WAV_R";
static const char *C2WwlRString = "C2WWL_R";
static const char *C2WwlWString = "C2WWL_W";
static const char *C2WwtRString = "C2WWT_R";
static const char *C3Plfep1IRString = "C3PLFEP1I_R";
static const char *C3Plfep1IlhRString = "C3PLFEP1ILH_R";
static const char *C3Plfep1IlhWString = "C3PLFEP1ILH_W";
static const char *C3Plfep1IllRString = "C3PLFEP1ILL_R";
static const char *C3Plfep1IllWString = "C3PLFEP1ILL_W";
static const char *C3Plfep2IRString = "C3PLFEP2I_R";
static const char *C3Plfep2IlhRString = "C3PLFEP2ILH_R";
static const char *C3Plfep2IlhWString = "C3PLFEP2ILH_W";
static const char *C3Plfep2IllRString = "C3PLFEP2ILL_R";
static const char *C3Plfep2IllWString = "C3PLFEP2ILL_W";
static const char *C3Plfep3IRString = "C3PLFEP3I_R";
static const char *C3Plfep3IlhRString = "C3PLFEP3ILH_R";
static const char *C3Plfep3IlhWString = "C3PLFEP3ILH_W";
static const char *C3Plfep3IllRString = "C3PLFEP3ILL_R";
static const char *C3Plfep3IllWString = "C3PLFEP3ILL_W";
static const char *C3Pltmp1IRString = "C3PLTMP1I_R";
static const char *C3Pltmp1IWString = "C3PLTMP1I_W";
static const char *C3Pltmp1VRString = "C3PLTMP1V_R";
static const char *C3Pltmp1VlRString = "C3PLTMP1VL_R";
static const char *C3Pltmp1VlWString = "C3PLTMP1VL_W";
static const char *C3Pltmp2IRString = "C3PLTMP2I_R";
static const char *C3Pltmp2IWString = "C3PLTMP2I_W";
static const char *C3Pltmp2VRString = "C3PLTMP2V_R";
static const char *C3Pltmp2VlRString = "C3PLTMP2VL_R";
static const char *C3Pltmp2VlWString = "C3PLTMP2VL_W";
static const char *C3StmpiRString = "C3STMPI_R";
static const char *C3StmpiWString = "C3STMPI_W";
static const char *C3StmpvRString = "C3STMPV_R";
static const char *C3StmpvlRString = "C3STMPVL_R";
static const char *C3StmpvlWString = "C3STMPVL_W";
static const char *C3WalRString = "C3WAL_R";
static const char *C3WalWString = "C3WAL_W";
static const char *C3WapRString = "C3WAP_R";
static const char *C3WapWString = "C3WAP_W";
static const char *C3WapsRString = "C3WAPS_R";
static const char *C3WapsWString = "C3WAPS_W";
static const char *C3WavRString = "C3WAV_R";
static const char *C3WwlRString = "C3WWL_R";
static const char *C3WwlWString = "C3WWL_W";
static const char *C3WwtRString = "C3WWT_R";
static const char *C4Plfep1IRString = "C4PLFEP1I_R";
static const char *C4Plfep1IlhRString = "C4PLFEP1ILH_R";
static const char *C4Plfep1IlhWString = "C4PLFEP1ILH_W";
static const char *C4Plfep1IllRString = "C4PLFEP1ILL_R";
static const char *C4Plfep1IllWString = "C4PLFEP1ILL_W";
static const char *C4Plfep2IRString = "C4PLFEP2I_R";
static const char *C4Plfep2IlhRString = "C4PLFEP2ILH_R";
static const char *C4Plfep2IlhWString = "C4PLFEP2ILH_W";
static const char *C4Plfep2IllRString = "C4PLFEP2ILL_R";
static const char *C4Plfep2IllWString = "C4PLFEP2ILL_W";
static const char *C4Plfep3IRString = "C4PLFEP3I_R";
static const char *C4Plfep3IlhRString = "C4PLFEP3ILH_R";
static const char *C4Plfep3IlhWString = "C4PLFEP3ILH_W";
static const char *C4Plfep3IllRString = "C4PLFEP3ILL_R";
static const char *C4Plfep3IllWString = "C4PLFEP3ILL_W";
static const char *C4Pltmp1IRString = "C4PLTMP1I_R";
static const char *C4Pltmp1IWString = "C4PLTMP1I_W";
static const char *C4Pltmp1VRString = "C4PLTMP1V_R";
static const char *C4Pltmp1VlRString = "C4PLTMP1VL_R";
static const char *C4Pltmp1VlWString = "C4PLTMP1VL_W";
static const char *C4Pltmp2IRString = "C4PLTMP2I_R";
static const char *C4Pltmp2IWString = "C4PLTMP2I_W";
static const char *C4Pltmp2VRString = "C4PLTMP2V_R";
static const char *C4Pltmp2VlRString = "C4PLTMP2VL_R";
static const char *C4Pltmp2VlWString = "C4PLTMP2VL_W";
static const char *C4StmpiRString = "C4STMPI_R";
static const char *C4StmpiWString = "C4STMPI_W";
static const char *C4StmpvRString = "C4STMPV_R";
static const char *C4StmpvlRString = "C4STMPVL_R";
static const char *C4StmpvlWString = "C4STMPVL_W";
static const char *C4WalRString = "C4WAL_R";
static const char *C4WalWString = "C4WAL_W";
static const char *C4WapRString = "C4WAP_R";
static const char *C4WapWString = "C4WAP_W";
static const char *C4WapsRString = "C4WAPS_R";
static const char *C4WapsWString = "C4WAPS_W";
static const char *C4WavRString = "C4WAV_R";
static const char *C4WwlRString = "C4WWL_R";
static const char *C4WwlWString = "C4WWL_W";
static const char *C4WwtRString = "C4WWT_R";
static const char *XatfltRString = "XATFLT_R";
static const char *XatfltBit0RString = "XATFLT_BIT0_R";
static const char *XcienRString = "XCIEN_R";
static const char *XcienBit0RString = "XCIEN_BIT0_R";
static const char *XciencRString = "XCIENC_R";
static const char *XciencBit0RString = "XCIENC_BIT0_R";
static const char *XciencWString = "XCIENC_W";
static const char *XciencBit0WString = "XCIENC_BIT0_W";
static const char *XcienmRString = "XCIENM_R";
static const char *XcienmBit0RString = "XCIENM_BIT0_R";
static const char *XcienmWString = "XCIENM_W";
static const char *XcienmBit0WString = "XCIENM_BIT0_W";
static const char *XcplfepfcRString = "XCPLFEPFC_R";
static const char *XcplfepfcBit0RString = "XCPLFEPFC_BIT0_R";
static const char *XcplfeplsRString = "XCPLFEPLS_R";
static const char *XcplfeplsBit0RString = "XCPLFEPLS_BIT0_R";
static const char *XcpltmplsRString = "XCPLTMPLS_R";
static const char *XcpltmplsBit0RString = "XCPLTMPLS_BIT0_R";
static const char *XcvfRString = "XCVF_R";
static const char *XcvfBit0RString = "XCVF_BIT0_R";
static const char *XcvfcRString = "XCVFC_R";
static const char *XcvfcBit0RString = "XCVFC_BIT0_R";
static const char *XcvfcWString = "XCVFC_W";
static const char *XcvfcBit0WString = "XCVFC_BIT0_W";
static const char *XcvfmRString = "XCVFM_R";
static const char *XcvfmBit0RString = "XCVFM_BIT0_R";
static const char *XcvfmWString = "XCVFM_W";
static const char *XcvfmBit0WString = "XCVFM_BIT0_W";
static const char *XcwadRString = "XCWAD_R";
static const char *XcwadBit0RString = "XCWAD_BIT0_R";
static const char *XcwafcRString = "XCWAFC_R";
static const char *XcwafcBit0RString = "XCWAFC_BIT0_R";
static const char *XcwafcWString = "XCWAFC_W";
static const char *XcwafcBit0WString = "XCWAFC_BIT0_W";
static const char *XcwamRString = "XCWAM_R";
static const char *XcwamBit0RString = "XCWAM_BIT0_R";
static const char *XcwamWString = "XCWAM_W";
static const char *XcwamBit0WString = "XCWAM_BIT0_W";
static const char *XcwatRString = "XCWAT_R";
static const char *XcwatBit0RString = "XCWAT_BIT0_R";
static const char *XcwatWString = "XCWAT_W";
static const char *XcwatBit0WString = "XCWAT_BIT0_W";
static const char *XcwwdRString = "XCWWD_R";
static const char *XcwwdBit0RString = "XCWWD_BIT0_R";
static const char *XcwwfcRString = "XCWWFC_R";
static const char *XcwwfcBit0RString = "XCWWFC_BIT0_R";
static const char *XcwwmRString = "XCWWM_R";
static const char *XcwwmBit0RString = "XCWWM_BIT0_R";
static const char *XcwwtsRString = "XCWWTS_R";
static const char *XcwwtsBit0RString = "XCWWTS_BIT0_R";
static const char *XcwwtsWString = "XCWWTS_W";
static const char *XcwwtsBit0WString = "XCWWTS_BIT0_W";
static const char *XffsdRString = "XFFSD_R";
static const char *XffsdBit0RString = "XFFSD_BIT0_R";
static const char *XffsdcRString = "XFFSDC_R";
static const char *XffsdcBit0RString = "XFFSDC_BIT0_R";
static const char *XffsdcWString = "XFFSDC_W";
static const char *XffsdcBit0WString = "XFFSDC_BIT0_W";
static const char *XffsdmRString = "XFFSDM_R";
static const char *XffsdmBit0RString = "XFFSDM_BIT0_R";
static const char *XffsdmWString = "XFFSDM_W";
static const char *XffsdmBit0WString = "XFFSDM_BIT0_W";
static const char *XictlRString = "XICTL_R";
static const char *XictlBit0RString = "XICTL_BIT0_R";
static const char *XictlWString = "XICTL_W";
static const char *XictlBit0WString = "XICTL_BIT0_W";
static const char *XistatRString = "XISTAT_R";
static const char *XistatBit0RString = "XISTAT_BIT0_R";
static const char *XiverRString = "XIVER_R";
static const char *XstmplsRString = "XSTMPLS_R";
static const char *XstmplsBit0RString = "XSTMPLS_BIT0_R";

const unsigned int scllrfINTReadRegCount = 121;
const unsigned int scllrfINTWriteRegCount = 75;



 /* This class implements the AxiVersion driver. */
class scllrfINTDriver : public scllrfAsynPortDriver {
public:
    scllrfINTDriver(const char *drvPortName, const char *netPortName);
    virtual ~scllrfINTDriver();



protected:
    virtual asynStatus functionToRegister(const int function, FpgaReg *pToFpga); /**< Translate asyn function number/reason to a register address */
	virtual asynStatus processRegReadback(const FpgaReg *pFromFpga,
			bool &waveIsReady); // parse register data, write to PVs
	virtual asynStatus processRegWriteResponse(const FpgaReg *pFromFpga);

/* Registers */
    int p_C1Plfep1IR;
    #define FIRST_SCLLRFINT_PARAM p_C1Plfep1IR
    int p_C1Plfep1IlhR;
    int p_C1Plfep1IlhW;
    int p_C1Plfep1IllR;
    int p_C1Plfep1IllW;
    int p_C1Plfep2IR;
    int p_C1Plfep2IlhR;
    int p_C1Plfep2IlhW;
    int p_C1Plfep2IllR;
    int p_C1Plfep2IllW;
    int p_C1Plfep3IR;
    int p_C1Plfep3IlhR;
    int p_C1Plfep3IlhW;
    int p_C1Plfep3IllR;
    int p_C1Plfep3IllW;
    int p_C1Pltmp1IR;
    int p_C1Pltmp1IW;
    int p_C1Pltmp1VR;
    int p_C1Pltmp1VlR;
    int p_C1Pltmp1VlW;
    int p_C1Pltmp2IR;
    int p_C1Pltmp2IW;
    int p_C1Pltmp2VR;
    int p_C1Pltmp2VlR;
    int p_C1Pltmp2VlW;
    int p_C1StmpiR;
    int p_C1StmpiW;
    int p_C1StmpvR;
    int p_C1StmpvlR;
    int p_C1StmpvlW;
    int p_C1WalR;
    int p_C1WalW;
    int p_C1WapR;
    int p_C1WapW;
    int p_C1WapsR;
    int p_C1WapsW;
    int p_C1WavR;
    int p_C1WwlR;
    int p_C1WwlW;
    int p_C1WwtR;
    int p_C2Plfep1IR;
    int p_C2Plfep1IlhR;
    int p_C2Plfep1IlhW;
    int p_C2Plfep1IllR;
    int p_C2Plfep1IllW;
    int p_C2Plfep2IR;
    int p_C2Plfep2IlhR;
    int p_C2Plfep2IlhW;
    int p_C2Plfep2IllR;
    int p_C2Plfep2IllW;
    int p_C2Plfep3IR;
    int p_C2Plfep3IlhR;
    int p_C2Plfep3IlhW;
    int p_C2Plfep3IllR;
    int p_C2Plfep3IllW;
    int p_C2Pltmp1IR;
    int p_C2Pltmp1IW;
    int p_C2Pltmp1VR;
    int p_C2Pltmp1VlR;
    int p_C2Pltmp1VlW;
    int p_C2Pltmp2IR;
    int p_C2Pltmp2IW;
    int p_C2Pltmp2VR;
    int p_C2Pltmp2VlR;
    int p_C2Pltmp2VlW;
    int p_C2StmpiR;
    int p_C2StmpiW;
    int p_C2StmpvR;
    int p_C2StmpvlR;
    int p_C2StmpvlW;
    int p_C2WalR;
    int p_C2WalW;
    int p_C2WapR;
    int p_C2WapW;
    int p_C2WapsR;
    int p_C2WapsW;
    int p_C2WavR;
    int p_C2WwlR;
    int p_C2WwlW;
    int p_C2WwtR;
    int p_C3Plfep1IR;
    int p_C3Plfep1IlhR;
    int p_C3Plfep1IlhW;
    int p_C3Plfep1IllR;
    int p_C3Plfep1IllW;
    int p_C3Plfep2IR;
    int p_C3Plfep2IlhR;
    int p_C3Plfep2IlhW;
    int p_C3Plfep2IllR;
    int p_C3Plfep2IllW;
    int p_C3Plfep3IR;
    int p_C3Plfep3IlhR;
    int p_C3Plfep3IlhW;
    int p_C3Plfep3IllR;
    int p_C3Plfep3IllW;
    int p_C3Pltmp1IR;
    int p_C3Pltmp1IW;
    int p_C3Pltmp1VR;
    int p_C3Pltmp1VlR;
    int p_C3Pltmp1VlW;
    int p_C3Pltmp2IR;
    int p_C3Pltmp2IW;
    int p_C3Pltmp2VR;
    int p_C3Pltmp2VlR;
    int p_C3Pltmp2VlW;
    int p_C3StmpiR;
    int p_C3StmpiW;
    int p_C3StmpvR;
    int p_C3StmpvlR;
    int p_C3StmpvlW;
    int p_C3WalR;
    int p_C3WalW;
    int p_C3WapR;
    int p_C3WapW;
    int p_C3WapsR;
    int p_C3WapsW;
    int p_C3WavR;
    int p_C3WwlR;
    int p_C3WwlW;
    int p_C3WwtR;
    int p_C4Plfep1IR;
    int p_C4Plfep1IlhR;
    int p_C4Plfep1IlhW;
    int p_C4Plfep1IllR;
    int p_C4Plfep1IllW;
    int p_C4Plfep2IR;
    int p_C4Plfep2IlhR;
    int p_C4Plfep2IlhW;
    int p_C4Plfep2IllR;
    int p_C4Plfep2IllW;
    int p_C4Plfep3IR;
    int p_C4Plfep3IlhR;
    int p_C4Plfep3IlhW;
    int p_C4Plfep3IllR;
    int p_C4Plfep3IllW;
    int p_C4Pltmp1IR;
    int p_C4Pltmp1IW;
    int p_C4Pltmp1VR;
    int p_C4Pltmp1VlR;
    int p_C4Pltmp1VlW;
    int p_C4Pltmp2IR;
    int p_C4Pltmp2IW;
    int p_C4Pltmp2VR;
    int p_C4Pltmp2VlR;
    int p_C4Pltmp2VlW;
    int p_C4StmpiR;
    int p_C4StmpiW;
    int p_C4StmpvR;
    int p_C4StmpvlR;
    int p_C4StmpvlW;
    int p_C4WalR;
    int p_C4WalW;
    int p_C4WapR;
    int p_C4WapW;
    int p_C4WapsR;
    int p_C4WapsW;
    int p_C4WavR;
    int p_C4WwlR;
    int p_C4WwlW;
    int p_C4WwtR;
    int p_XatfltR;
    int p_XatfltBit0R;
    int p_XcienR;
    int p_XcienBit0R;
    int p_XciencR;
    int p_XciencBit0R;
    int p_XciencW;
    int p_XciencBit0W;
    int p_XcienmR;
    int p_XcienmBit0R;
    int p_XcienmW;
    int p_XcienmBit0W;
    int p_XcplfepfcR;
    int p_XcplfepfcBit0R;
    int p_XcplfeplsR;
    int p_XcplfeplsBit0R;
    int p_XcpltmplsR;
    int p_XcpltmplsBit0R;
    int p_XcvfR;
    int p_XcvfBit0R;
    int p_XcvfcR;
    int p_XcvfcBit0R;
    int p_XcvfcW;
    int p_XcvfcBit0W;
    int p_XcvfmR;
    int p_XcvfmBit0R;
    int p_XcvfmW;
    int p_XcvfmBit0W;
    int p_XcwadR;
    int p_XcwadBit0R;
    int p_XcwafcR;
    int p_XcwafcBit0R;
    int p_XcwafcW;
    int p_XcwafcBit0W;
    int p_XcwamR;
    int p_XcwamBit0R;
    int p_XcwamW;
    int p_XcwamBit0W;
    int p_XcwatR;
    int p_XcwatBit0R;
    int p_XcwatW;
    int p_XcwatBit0W;
    int p_XcwwdR;
    int p_XcwwdBit0R;
    int p_XcwwfcR;
    int p_XcwwfcBit0R;
    int p_XcwwmR;
    int p_XcwwmBit0R;
    int p_XcwwtsR;
    int p_XcwwtsBit0R;
    int p_XcwwtsW;
    int p_XcwwtsBit0W;
    int p_XffsdR;
    int p_XffsdBit0R;
    int p_XffsdcR;
    int p_XffsdcBit0R;
    int p_XffsdcW;
    int p_XffsdcBit0W;
    int p_XffsdmR;
    int p_XffsdmBit0R;
    int p_XffsdmW;
    int p_XffsdmBit0W;
    int p_XictlR;
    int p_XictlBit0R;
    int p_XictlW;
    int p_XictlBit0W;
    int p_XistatR;
    int p_XistatBit0R;
    int p_XiverR;
    int p_XstmplsR;
    int p_XstmplsBit0R;
    #define LAST_SCLLRFINT_PARAM p_XstmplsBit0R

#define NUM_SCLLRFINT_PARAMS (&LAST_SCLLRFINT_PARAM - &FIRST_SCLLRFINT_PARAM + NUM_SCLLRF_PARAMS + 1)

private:

    // mapping of register names to addresses
    enum ReadRegAddrs
    {
    	C1WavRAdr = 0x00000000,
    	C2WavRAdr = 0x00000001,
    	C3WavRAdr = 0x00000002,
    	C4WavRAdr = 0x00000003,
    	C1WapsRAdr = 0x00000004,
    	C2WapsRAdr = 0x00000005,
    	C3WapsRAdr = 0x00000006,
    	C4WapsRAdr = 0x00000007,
    	C1WalRAdr = 0x00000008,
    	C2WalRAdr = 0x00000009,
    	C3WalRAdr = 0x0000000A,
    	C4WalRAdr = 0x0000000B,
    	C1WapRAdr = 0x0000000C,
    	C2WapRAdr = 0x0000000D,
    	C3WapRAdr = 0x0000000E,
    	C4WapRAdr = 0x0000000F,
    	XcwadRAdr = 0x00000010,
    	XcwamRAdr = 0x00000011,
    	XcwatRAdr = 0x00000012,
    	XatfltRAdr = 0x00000013,
    	XcwafcRAdr = 0x00000014,
    	C1WwtRAdr = 0x00000015,
    	C2WwtRAdr = 0x00000016,
    	C3WwtRAdr = 0x00000017,
    	C4WwtRAdr = 0x00000018,
    	C1WwlRAdr = 0x00000019,
    	C2WwlRAdr = 0x0000001A,
    	C3WwlRAdr = 0x0000001B,
    	C4WwlRAdr = 0x0000001C,
    	XcwwdRAdr = 0x0000001D,
    	XcwwtsRAdr = 0x0000001E,
    	XcwwmRAdr = 0x0000001F,
    	XcwwfcRAdr = 0x00000020,
    	C1StmpiRAdr = 0x00000021,
    	C2StmpiRAdr = 0x00000022,
    	C3StmpiRAdr = 0x00000023,
    	C4StmpiRAdr = 0x00000024,
    	C1StmpvRAdr = 0x00000025,
    	C2StmpvRAdr = 0x00000026,
    	C3StmpvRAdr = 0x00000027,
    	C4StmpvRAdr = 0x00000028,
    	C1StmpvlRAdr = 0x00000029,
    	C2StmpvlRAdr = 0x0000002A,
    	C3StmpvlRAdr = 0x0000002B,
    	C4StmpvlRAdr = 0x0000002C,
    	XstmplsRAdr = 0x0000002D,
    	C1Pltmp1IRAdr = 0x0000002E,
    	C1Pltmp2IRAdr = 0x0000002F,
    	C2Pltmp1IRAdr = 0x00000030,
    	C2Pltmp2IRAdr = 0x00000031,
    	C3Pltmp1IRAdr = 0x00000032,
    	C3Pltmp2IRAdr = 0x00000033,
    	C4Pltmp1IRAdr = 0x00000034,
    	C4Pltmp2IRAdr = 0x00000035,
    	C1Pltmp1VRAdr = 0x00000036,
    	C1Pltmp2VRAdr = 0x00000037,
    	C2Pltmp1VRAdr = 0x00000038,
    	C2Pltmp2VRAdr = 0x00000039,
    	C3Pltmp1VRAdr = 0x0000003A,
    	C3Pltmp2VRAdr = 0x0000003B,
    	C4Pltmp1VRAdr = 0x0000003C,
    	C4Pltmp2VRAdr = 0x0000003D,
    	C1Pltmp1VlRAdr = 0x0000003E,
    	C1Pltmp2VlRAdr = 0x0000003F,
    	C2Pltmp1VlRAdr = 0x00000040,
    	C2Pltmp2VlRAdr = 0x00000041,
    	C3Pltmp1VlRAdr = 0x00000042,
    	C3Pltmp2VlRAdr = 0x00000043,
    	C4Pltmp1VlRAdr = 0x00000044,
    	C4Pltmp2VlRAdr = 0x00000045,
    	XcpltmplsRAdr = 0x00000046,
    	C1Plfep1IRAdr = 0x00000047,
    	C1Plfep2IRAdr = 0x00000048,
    	C1Plfep3IRAdr = 0x00000049,
    	C2Plfep1IRAdr = 0x0000004A,
    	C2Plfep2IRAdr = 0x0000004B,
    	C2Plfep3IRAdr = 0x0000004C,
    	C3Plfep1IRAdr = 0x0000004D,
    	C3Plfep2IRAdr = 0x0000004E,
    	C3Plfep3IRAdr = 0x0000004F,
    	C4Plfep1IRAdr = 0x00000050,
    	C4Plfep2IRAdr = 0x00000051,
    	C4Plfep3IRAdr = 0x00000052,
    	C1Plfep1IlhRAdr = 0x00000053,
    	C1Plfep2IlhRAdr = 0x00000054,
    	C1Plfep3IlhRAdr = 0x00000055,
    	C2Plfep1IlhRAdr = 0x00000056,
    	C2Plfep2IlhRAdr = 0x00000057,
    	C2Plfep3IlhRAdr = 0x00000058,
    	C3Plfep1IlhRAdr = 0x00000059,
    	C3Plfep2IlhRAdr = 0x0000005A,
    	C3Plfep3IlhRAdr = 0x0000005B,
    	C4Plfep1IlhRAdr = 0x0000005C,
    	C4Plfep2IlhRAdr = 0x0000005D,
    	C4Plfep3IlhRAdr = 0x0000005E,
    	C1Plfep1IllRAdr = 0x0000005F,
    	C1Plfep2IllRAdr = 0x00000060,
    	C1Plfep3IllRAdr = 0x00000061,
    	C2Plfep1IllRAdr = 0x00000062,
    	C2Plfep2IllRAdr = 0x00000063,
    	C2Plfep3IllRAdr = 0x00000064,
    	C3Plfep1IllRAdr = 0x00000065,
    	C3Plfep2IllRAdr = 0x00000066,
    	C3Plfep3IllRAdr = 0x00000067,
    	C4Plfep1IllRAdr = 0x00000068,
    	C4Plfep2IllRAdr = 0x00000069,
    	C4Plfep3IllRAdr = 0x0000006A,
    	XcplfeplsRAdr = 0x0000006B,
    	XcplfepfcRAdr = 0x0000006C,
    	XcvfRAdr = 0x0000006D,
    	XcvfmRAdr = 0x0000006E,
    	XcvfcRAdr = 0x0000006F,
    	XcienRAdr = 0x00000070,
    	XcienmRAdr = 0x00000071,
    	XciencRAdr = 0x00000072,
    	XffsdRAdr = 0x00000073,
    	XffsdmRAdr = 0x00000074,
    	XffsdcRAdr = 0x00000075,
    	XistatRAdr = 0x00000076,
    	XictlRAdr = 0x00000077,
    	XiverRAdr = 0x00000078,
    };

    // mapping of register names to addresses
    enum RegWriteAddrs
    {
    	C1WapsWAdr = 0x00000004,
    	C2WapsWAdr = 0x00000005,
    	C3WapsWAdr = 0x00000006,
    	C4WapsWAdr = 0x00000007,
    	C1WalWAdr = 0x00000008,
    	C2WalWAdr = 0x00000009,
    	C3WalWAdr = 0x0000000A,
    	C4WalWAdr = 0x0000000B,
    	C1WapWAdr = 0x0000000C,
    	C2WapWAdr = 0x0000000D,
    	C3WapWAdr = 0x0000000E,
    	C4WapWAdr = 0x0000000F,
    	XcwamWAdr = 0x00000011,
    	XcwatWAdr = 0x00000012,
    	XcwafcWAdr = 0x00000014,
    	C1WwlWAdr = 0x00000019,
    	C2WwlWAdr = 0x0000001A,
    	C3WwlWAdr = 0x0000001B,
    	C4WwlWAdr = 0x0000001C,
    	XcwwtsWAdr = 0x0000001E,
    	C1StmpiWAdr = 0x00000021,
    	C2StmpiWAdr = 0x00000022,
    	C3StmpiWAdr = 0x00000023,
    	C4StmpiWAdr = 0x00000024,
    	C1StmpvlWAdr = 0x00000029,
    	C2StmpvlWAdr = 0x0000002A,
    	C3StmpvlWAdr = 0x0000002B,
    	C4StmpvlWAdr = 0x0000002C,
    	C1Pltmp1IWAdr = 0x0000002E,
    	C1Pltmp2IWAdr = 0x0000002F,
    	C2Pltmp1IWAdr = 0x00000030,
    	C2Pltmp2IWAdr = 0x00000031,
    	C3Pltmp1IWAdr = 0x00000032,
    	C3Pltmp2IWAdr = 0x00000033,
    	C4Pltmp1IWAdr = 0x00000034,
    	C4Pltmp2IWAdr = 0x00000035,
    	C1Pltmp1VlWAdr = 0x0000003E,
    	C1Pltmp2VlWAdr = 0x0000003F,
    	C2Pltmp1VlWAdr = 0x00000040,
    	C2Pltmp2VlWAdr = 0x00000041,
    	C3Pltmp1VlWAdr = 0x00000042,
    	C3Pltmp2VlWAdr = 0x00000043,
    	C4Pltmp1VlWAdr = 0x00000044,
    	C4Pltmp2VlWAdr = 0x00000045,
    	C1Plfep1IlhWAdr = 0x00000053,
    	C1Plfep2IlhWAdr = 0x00000054,
    	C1Plfep3IlhWAdr = 0x00000055,
    	C2Plfep1IlhWAdr = 0x00000056,
    	C2Plfep2IlhWAdr = 0x00000057,
    	C2Plfep3IlhWAdr = 0x00000058,
    	C3Plfep1IlhWAdr = 0x00000059,
    	C3Plfep2IlhWAdr = 0x0000005A,
    	C3Plfep3IlhWAdr = 0x0000005B,
    	C4Plfep1IlhWAdr = 0x0000005C,
    	C4Plfep2IlhWAdr = 0x0000005D,
    	C4Plfep3IlhWAdr = 0x0000005E,
    	C1Plfep1IllWAdr = 0x0000005F,
    	C1Plfep2IllWAdr = 0x00000060,
    	C1Plfep3IllWAdr = 0x00000061,
    	C2Plfep1IllWAdr = 0x00000062,
    	C2Plfep2IllWAdr = 0x00000063,
    	C2Plfep3IllWAdr = 0x00000064,
    	C3Plfep1IllWAdr = 0x00000065,
    	C3Plfep2IllWAdr = 0x00000066,
    	C3Plfep3IllWAdr = 0x00000067,
    	C4Plfep1IllWAdr = 0x00000068,
    	C4Plfep2IllWAdr = 0x00000069,
    	C4Plfep3IllWAdr = 0x0000006A,
    	XcvfmWAdr = 0x0000006E,
    	XcvfcWAdr = 0x0000006F,
    	XcienmWAdr = 0x00000071,
    	XciencWAdr = 0x00000072,
    	XffsdmWAdr = 0x00000074,
    	XffsdcWAdr = 0x00000075,
    	XictlWAdr = 0x00000077,
    };

    // masks applied to returned register data
    enum RegMasks
    {
		XcwadBit0Mask = 0x00000001,
		XcwamBit0Mask = 0x00000001,
		XcwatBit0Mask = 0x00000001,
		XatfltBit0Mask = 0x00000001,
		XcwafcBit0Mask = 0x00000001,
		XcwwdBit0Mask = 0x00000001,
		XcwwtsBit0Mask = 0x00000001,
		XcwwmBit0Mask = 0x00000001,
		XcwwfcBit0Mask = 0x00000001,
		XstmplsBit0Mask = 0x00000001,
		XcpltmplsBit0Mask = 0x00000001,
		XcplfeplsBit0Mask = 0x00000001,
		XcplfepfcBit0Mask = 0x00000001,
		XcvfBit0Mask = 0x00000001,
		XcvfmBit0Mask = 0x00000001,
		XcvfcBit0Mask = 0x00000001,
		XcienBit0Mask = 0x00000001,
		XcienmBit0Mask = 0x00000001,
		XciencBit0Mask = 0x00000001,
		XffsdBit0Mask = 0x00000001,
		XffsdmBit0Mask = 0x00000001,
		XffsdcBit0Mask = 0x00000001,
		XistatBit0Mask = 0x00000001,
		XictlBit0Mask = 0x00000001,

    };
};

#endif

