///////////////////////////////////////////////////////////////////////////////
// PLC_SteuerungDriver.cpp
#include "TcPch.h"
#pragma hdrstop

#include "PLC_SteuerungDriver.h"
#include "PLC_SteuerungClassFactory.h"

DECLARE_GENERIC_DEVICE(PLC_STEUERUNGDRV)

IOSTATUS CPLC_SteuerungDriver::OnLoad( )
{
  TRACE(_T("CObjClassFactory::OnLoad()\n") );
  m_pObjClassFactory = new CPLC_SteuerungClassFactory();

  return IOSTATUS_SUCCESS;
}

VOID CPLC_SteuerungDriver::OnUnLoad( )
{
  delete m_pObjClassFactory;
}

unsigned long _cdecl CPLC_SteuerungDriver::PLC_STEUERUNGDRV_GetVersion( )
{
  return( (PLC_STEUERUNGDRV_Major << 8) | PLC_STEUERUNGDRV_Minor );
}

