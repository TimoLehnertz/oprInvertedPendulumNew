///////////////////////////////////////////////////////////////////////////////
// PLC_SteuerungDriver.h

#ifndef __PLC_STEUERUNGDRIVER_H__
#define __PLC_STEUERUNGDRIVER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TcBase.h"

#define PLC_STEUERUNGDRV_NAME        "PLC_STEUERUNG"
#define PLC_STEUERUNGDRV_Major       1
#define PLC_STEUERUNGDRV_Minor       0

#define DEVICE_CLASS CPLC_SteuerungDriver

#include "ObjDriver.h"

class CPLC_SteuerungDriver : public CObjDriver
{
public:
  virtual IOSTATUS  OnLoad();
  virtual VOID    OnUnLoad();

  //////////////////////////////////////////////////////
  // VxD-Services exported by this driver
  static unsigned long  _cdecl PLC_STEUERUNGDRV_GetVersion();
  //////////////////////////////////////////////////////
  
};

Begin_VxD_Service_Table(PLC_STEUERUNGDRV)
  VxD_Service( PLC_STEUERUNGDRV_GetVersion )
End_VxD_Service_Table


#endif // ifndef __PLC_STEUERUNGDRIVER_H__