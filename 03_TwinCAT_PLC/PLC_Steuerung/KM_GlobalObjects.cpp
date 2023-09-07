/********************************************************************************************************************************************/
/*                                                                                                                                          */
/*  Source-Datei: KM_GlobalObjects.cpp                                                                                                      */
/*                                                                                                                                          */
/*  Prof. Dr. Lothar Zunker      09.06.2021                                                                                                 */
/*                                                                                                                                          */
/********************************************************************************************************************************************/

/************************************************************* System-Includes **************************************************************/
//#include "TcPch.h"
#pragma  hdrstop
 
/*********************************************************** Anwender-Includes **************************************************************/
#include "KM_GlobalObjects.h"

using namespace Zunker;                        // aus KM_GlobalObjects.h

/******************************************************** Konstruktoren / Destruktor ********************************************************/
                  C_KM_GlobalObjects::C_KM_GlobalObjects            (KM_App_MainInputs* m_Inputs, KM_App_MainOutputs* m_Outputs)
  {
  this->m_Inputs                                = m_Inputs;
  this->m_Outputs                               = m_Outputs;
  this->Taktzeit                                = Takt_Zeit;                                  // aus CD_GlobalObjects.h

  this->ModulConnection_ID_Level_B              = new E_ModulConnection_ID[3];
  this->ModulConnection_ID_Level_B[0]           = E_ModulConnection_ID::Level_B0;
  this->ModulConnection_ID_Level_B[1]           = E_ModulConnection_ID::Level_B1;
  this->ModulConnection_ID_Level_B[2]           = E_ModulConnection_ID::Level_B2;

  this->ModulConnection_ID_Level_C              = new E_ModulConnection_ID[3];
  this->ModulConnection_ID_Level_C[0]           = E_ModulConnection_ID::Level_C0;
  this->ModulConnection_ID_Level_C[1]           = E_ModulConnection_ID::Level_C1;
  this->ModulConnection_ID_Level_C[2]           = E_ModulConnection_ID::Level_C2;

  this->KM_Ads                                  = new C_KM_Ads ();
  this->EK1100_EtherCAT_Koppler                 = new C_EK1100_EtherCAT_Koppler (&m_Inputs->EK1100_EtherCAT_Koppler,&m_Outputs->EK1100_EtherCAT_Koppler);
  }
                  C_KM_GlobalObjects::~C_KM_GlobalObjects           ()
  {
  delete (this->EK1100_EtherCAT_Koppler);
  delete (this->KM_Ads);

  this->ModulConnection_ID_Level_C[2]           = E_ModulConnection_ID::NoConnection;
  this->ModulConnection_ID_Level_C[1]           = E_ModulConnection_ID::NoConnection;
  this->ModulConnection_ID_Level_C[0]           = E_ModulConnection_ID::NoConnection;
  delete   (this->ModulConnection_ID_Level_C);

  this->ModulConnection_ID_Level_B[2]           = E_ModulConnection_ID::NoConnection;
  this->ModulConnection_ID_Level_B[1]           = E_ModulConnection_ID::NoConnection;
  this->ModulConnection_ID_Level_B[0]           = E_ModulConnection_ID::NoConnection;
  delete   (this->ModulConnection_ID_Level_B);

  this->Taktzeit                                = 0.0;
  this->m_Outputs                               = nullptr;
  this->m_Inputs                                = nullptr;
  }


/******************************************************* Öffentliche Anwender-Methoden ******************************************************/

/*************************************************** Nicht öffentliche private Methoden *****************************************************/

