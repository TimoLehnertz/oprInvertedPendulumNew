///////////////////////////////////////////////////////////////////////////////
// PLC_SteuerungCtrl.h

#ifndef __PLC_STEUERUNGCTRL_H__
#define __PLC_STEUERUNGCTRL_H__

#include <atlbase.h>
#include <atlcom.h>

#define PLC_STEUERUNGDRV_NAME "PLC_STEUERUNG"

#include "resource.h"       // main symbols
#include "PLC_SteuerungW32.h"
#include "TcBase.h"
#include "PLC_SteuerungClassFactory.h"
#include "TcOCFCtrlImpl.h"

class CPLC_SteuerungCtrl 
  : public CComObjectRootEx<CComMultiThreadModel>
  , public CComCoClass<CPLC_SteuerungCtrl, &CLSID_PLC_SteuerungCtrl>
  , public IPLC_SteuerungCtrl
  , public ITcOCFCtrlImpl<CPLC_SteuerungCtrl, CPLC_SteuerungClassFactory>
{
public:
  CPLC_SteuerungCtrl();
  virtual ~CPLC_SteuerungCtrl();

DECLARE_REGISTRY_RESOURCEID(IDR_PLC_STEUERUNGCTRL)
DECLARE_NOT_AGGREGATABLE(CPLC_SteuerungCtrl)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CPLC_SteuerungCtrl)
  COM_INTERFACE_ENTRY(IPLC_SteuerungCtrl)
  COM_INTERFACE_ENTRY(ITcCtrl)
  COM_INTERFACE_ENTRY(ITcCtrl2)
END_COM_MAP()

};

#endif // #ifndef __PLC_STEUERUNGCTRL_H__
