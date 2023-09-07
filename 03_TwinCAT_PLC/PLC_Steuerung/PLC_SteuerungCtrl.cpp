// PLC_SteuerungCtrl.cpp : Implementation of CTcPLC_SteuerungCtrl
#include "TcPch.h"
#pragma hdrstop

#include "PLC_SteuerungW32.h"
#include "PLC_SteuerungCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CPLC_SteuerungCtrl

CPLC_SteuerungCtrl::CPLC_SteuerungCtrl() 
  : ITcOCFCtrlImpl<CPLC_SteuerungCtrl, CPLC_SteuerungClassFactory>() 
{
}

CPLC_SteuerungCtrl::~CPLC_SteuerungCtrl()
{
}

