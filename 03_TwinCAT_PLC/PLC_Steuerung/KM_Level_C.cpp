/********************************************************************************************************************************************/
/*                                                                                                                                          */
/*  Source-Datei: KM_Level_C.cpp                                                                                                            */
/*                                                                                                                                          */
/*  Prof. Dr. Lothar Zunker      09.06.2021                                                                                                 */
/*                                                                                                                                          */
/********************************************************************************************************************************************/

/************************************************************* System-Includes **************************************************************/
#include "TcPch.h"
#include "TcMath.h"
#pragma  hdrstop

/*********************************************************** Anwender-Includes **************************************************************/
#include "KM_Level_C.h"

using namespace Zunker;                        // aus KM_Level_C.h

/******************************************************** Konstruktoren / Destruktor ********************************************************/
            C_KM_Level_C::C_KM_Level_C                                       (C_KM_GlobalObjects* KM_GlobalObjects, Int32 Level_C_Nr)
  {
  this->KM_GlobalObjects                        = KM_GlobalObjects;
  this->Level_C_Nr                              = Level_C_Nr;

  // ADS-Kommunikations-Initalisierung
  this->ModulConnection_ID                      = this->KM_GlobalObjects->ModulConnection_ID_Level_C[this->Level_C_Nr];
  this->U2K_RequestMessage_Level_C_00           = this->KM_GlobalObjects->KM_Ads->Add_RequestMessage   <S_U2K_RequestMessage_Level_C_00>   ((Int32)this->ModulConnection_ID, (Int32)E_Level_C_CMD::KM_Kommando_00);
  this->K2U_ResponeMessage_Level_C_00           = this->KM_GlobalObjects->KM_Ads->Add_ResponseMessage  <S_K2U_ResponeMessage_Level_C_00>   ((Int32)this->ModulConnection_ID, (Int32)E_Level_C_CMD::KM_Kommando_00);
  this->U2K_RequestMessage_Level_C_01           = this->KM_GlobalObjects->KM_Ads->Add_RequestMessage   <S_U2K_RequestMessage_Level_C_01>   ((Int32)this->ModulConnection_ID, (Int32)E_Level_C_CMD::KM_Kommando_01);
  this->K2U_ResponeMessage_Level_C_01           = this->KM_GlobalObjects->KM_Ads->Add_ResponseMessage  <S_K2U_ResponeMessage_Level_C_01>   ((Int32)this->ModulConnection_ID, (Int32)E_Level_C_CMD::KM_Kommando_01);
  this->U2K_RequestMessage_Level_C_02           = this->KM_GlobalObjects->KM_Ads->Add_RequestMessage   <S_U2K_RequestMessage_Level_C_02>   ((Int32)this->ModulConnection_ID, (Int32)E_Level_C_CMD::KM_Kommando_02);
  this->K2U_ResponeMessage_Level_C_02           = this->KM_GlobalObjects->KM_Ads->Add_ResponseMessage  <S_K2U_ResponeMessage_Level_C_02>   ((Int32)this->ModulConnection_ID, (Int32)E_Level_C_CMD::KM_Kommando_02);
  this->U2K_RequestMessage_Level_C_03           = this->KM_GlobalObjects->KM_Ads->Add_RequestMessage   <S_U2K_RequestMessage_Level_C_03>   ((Int32)this->ModulConnection_ID, (Int32)E_Level_C_CMD::KM_Kommando_03);
  this->K2U_ResponeMessage_Level_C_03           = this->KM_GlobalObjects->KM_Ads->Add_ResponseMessage  <S_K2U_ResponeMessage_Level_C_03>   ((Int32)this->ModulConnection_ID, (Int32)E_Level_C_CMD::KM_Kommando_03);

  // Control-Attribute der Quasi-Parallel arbeitende Thread-Methoden
  this->SM_Thread1_Schritt                      = 0;
  this->SM_Thread1_Zaehler                      = 0;
  this->SM_Thread1_finished                     = true;

  this->SM_Thread2_Schritt                      = 0;
  this->SM_Thread2_Zaehler                      = 0;
  this->SM_Thread2_finished                     = true;
  
  this->SM_Thread3_Schritt                      = 0;
  this->SM_Thread3_Zaehler                      = 0;
  this->SM_Thread3_finished                     = true;
  
  this->SM_Thread4_Schritt                      = 0;
  this->SM_Thread4_Zaehler                      = 0;
  this->SM_Thread4_finished                     = true;
  
  this->SM_Thread5_Schritt                      = 0;
  this->SM_Thread5_Zaehler                      = 0;
  this->SM_Thread5_finished                     = true;
  }
            C_KM_Level_C::~C_KM_Level_C                                      ()
  {
  // Control-Attribute der Quasi-Parallel arbeitende Thread-Methoden
  this->SM_Thread5_finished                     = true;
  this->SM_Thread5_Zaehler                      = 0;
  this->SM_Thread5_Schritt                      = 0;
  
  this->SM_Thread4_finished                     = true;
  this->SM_Thread4_Zaehler                      = 0;
  this->SM_Thread4_Schritt                      = 0;
  
  this->SM_Thread3_finished                     = true;
  this->SM_Thread3_Zaehler                      = 0;
  this->SM_Thread3_Schritt                      = 0;
  
  this->SM_Thread2_finished                     = true;
  this->SM_Thread2_Zaehler                      = 0;
  this->SM_Thread2_Schritt                      = 0;
 
  this->SM_Thread1_finished                     = true;
  this->SM_Thread1_Schritt                      = 0;
  this->SM_Thread1_Zaehler                      = 0;

  // ADS-Kommunikations-DeInitalisierung
  this->KM_GlobalObjects->KM_Ads->Release_Messages <S_U2K_RequestMessage_Level_C_03, S_K2U_ResponeMessage_Level_C_03> ((Int32)this->ModulConnection_ID, (Int32)E_Level_C_CMD::KM_Kommando_03);
  this->KM_GlobalObjects->KM_Ads->Release_Messages <S_U2K_RequestMessage_Level_C_02, S_K2U_ResponeMessage_Level_C_02> ((Int32)this->ModulConnection_ID, (Int32)E_Level_C_CMD::KM_Kommando_02);
  this->KM_GlobalObjects->KM_Ads->Release_Messages <S_U2K_RequestMessage_Level_C_01, S_K2U_ResponeMessage_Level_C_01> ((Int32)this->ModulConnection_ID, (Int32)E_Level_C_CMD::KM_Kommando_01);
  this->KM_GlobalObjects->KM_Ads->Release_Messages <S_U2K_RequestMessage_Level_C_00, S_K2U_ResponeMessage_Level_C_00> ((Int32)this->ModulConnection_ID, (Int32)E_Level_C_CMD::KM_Kommando_00);
  this->ModulConnection_ID                      = E_ModulConnection_ID::NoConnection;

  this->Level_C_Nr                              = -1;
  this->KM_GlobalObjects                        = nullptr;
  }

/*************************************************** Nicht öffentliche private Methoden *****************************************************/
void        C_KM_Level_C::Command_Interpreter                                (Int32 KM_Command)
  {
  E_Level_C_CMD MyKM_Command               = static_cast <E_Level_C_CMD> (KM_Command);

  // Interpetieren der Kommandos
  switch (MyKM_Command)
    {
    case E_Level_C_CMD::KM_Kommando_00:
      {
      }
    break;
    case E_Level_C_CMD::KM_Kommando_01:
      {
      }
    break;
    case E_Level_C_CMD::KM_Kommando_02:
      {
      }
    break;
    case E_Level_C_CMD::KM_Kommando_03:
      {
      }
    break;
    default:
      {
      }
    break;
    }  // switch (KM_Kommado)
  }

// Quasi-Parallel arbeitende Threads
Boolean     C_KM_Level_C::SM_Thread_1                                        (void)
  {
  return (true);
  }
Boolean     C_KM_Level_C::SM_Thread_2                                        (void)
  {
  return (true);
  }
Boolean     C_KM_Level_C::SM_Thread_3                                        (void)
  {
  return (true);
  } 
Boolean     C_KM_Level_C::SM_Thread_4                                        (void)
  {
  return (true);
  }
Boolean     C_KM_Level_C::SM_Thread_5                                        (void)
  {
  return (true);
  }

/******************************************************* Öffentliche Anwender-Methoden ******************************************************/
void        C_KM_Level_C::Cycle_Update                                       (void)
  {
  this->SM_Thread_1                 ();
  this->SM_Thread_2                 ();
  this->SM_Thread_3                 ();
  if (!this->SM_Thread4_finished) this->SM_Thread4_finished = this->SM_Thread_4 ();
  if (!this->SM_Thread5_finished) this->SM_Thread5_finished = this->SM_Thread_5 ();
  }
void        C_KM_Level_C::AdsReadWriteIndication                             (Int32 ModulConnection_ID, Int32 KM_Command)
  {
  E_ModulConnection_ID MyModulConnection_ID = static_cast<E_ModulConnection_ID> (ModulConnection_ID);

  if (MyModulConnection_ID == this->ModulConnection_ID)          // wenn, KM-Kommando für das hiesige Modul bestimmt, dann Auswertung durch den hiesige Command-Interpreter 
    {
    this->Command_Interpreter                 (KM_Command);
    }
  else                                                        // sonst Weitergabe an die Kind-Objekte; vgl Cycle-Update
    {
    }
  }  // void AdsReadWriteIndication (Int32 ModulConnection_ID, Int32 KM_Command)
