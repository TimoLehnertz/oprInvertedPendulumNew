/********************************************************************************************************************************************/
/*                                                                                                                                          */
/*  Header-Datei: KM_GlobalObjects.h                                                                                                        */
/*                                                                                                                                          */
/*  Prof. Dr. Lothar Zunker      09.06.2021                                                                                                 */
/*                                                                                                                                          */
/********************************************************************************************************************************************/
#pragma once

/************************************************************* System-Includes **************************************************************/
#include "..\..\01_Common\CommonTypes.h"              // notwendig für Deklarationen der Typen: Single, Double, Byte, Boolean, UInt16, Int16, Int32 , Int32, UInt64, Int64 (native C++-Variante zu using namespace System) 
#include "KM_Ads.h"
#include <KM_EtherCAT_Koppler.h>

/*********************************************************** Anwender-Includes **************************************************************/
#include "..\..\01_Common\CD_GlobalObjects.h"           // CommonDeclares des Moduls GlobalObjects 


namespace Zunker
  {
  class C_KM_GlobalObjects
    {
    /********************************************************* Konstruktoren/Destruktor *********************************************************/
    public:
                  C_KM_GlobalObjects                                (KM_App_MainInputs* m_Inputs, KM_App_MainOutputs* m_Outputs);
                  ~C_KM_GlobalObjects                               ();

    /******************************************** Nicht öffentliche private Anwender-Attribute **************************************************/
    private:


    /**************************************************** Öffentliche Anwender-Attribute ********************************************************/
    public:
    KM_App_MainInputs*                              m_Inputs;
    KM_App_MainOutputs*                             m_Outputs;
    Double                                          Taktzeit;

    E_ModulConnection_ID*                           ModulConnection_ID_Level_B;
    E_ModulConnection_ID*                           ModulConnection_ID_Level_C;

    C_KM_Ads*                                       KM_Ads;
    C_EK1100_EtherCAT_Koppler*                      EK1100_EtherCAT_Koppler;

    /*************************************************** Nicht öffentliche private Methoden *****************************************************/
    private:

    /******************************************************* Öffentliche Anwender-Methoden ******************************************************/
    public:
    };  // class C_KM_GlobalObjects
  } // namespace Zunker
