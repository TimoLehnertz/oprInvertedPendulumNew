/********************************************************************************************************************************************/
/*                                                                                                                                          */
/*  Source-Datei: KM_Level_B.cpp                                                                                                            */
/*                                                                                                                                          */
/*  Prof. Dr. Lothar Zunker      09.06.2021                                                                                                 */
/*                                                                                                                                          */
/********************************************************************************************************************************************/

/************************************************************* System-Includes **************************************************************/
#include "TcPch.h"
#include "TcMath.h"
#pragma  hdrstop

/*********************************************************** Anwender-Includes **************************************************************/
#include "KM_Level_B.h"

using namespace Zunker;                        // aus KM_Level_B.h

/******************************************************** Konstruktoren / Destruktor ********************************************************/
            C_KM_Level_B::C_KM_Level_B                                       (C_KM_GlobalObjects* KM_GlobalObjects, Int32 Level_B_Nr)
  {
  this->KM_GlobalObjects                        = KM_GlobalObjects;
  this->Level_B_Nr                              = Level_B_Nr;

  // ADS-Kommunikations-Initalisierung
  this->ModulConnection_ID                      = this->KM_GlobalObjects->ModulConnection_ID_Level_B[this->Level_B_Nr];
  this->U2K_RequestMessage_Level_B_00           = this->KM_GlobalObjects->KM_Ads->Add_RequestMessage   <S_U2K_RequestMessage_Level_B_00>   ((Int32)this->ModulConnection_ID, (Int32)E_Level_B_CMD::KM_Kommando_00);
  this->K2U_ResponeMessage_Level_B_00           = this->KM_GlobalObjects->KM_Ads->Add_ResponseMessage  <S_K2U_ResponeMessage_Level_B_00>   ((Int32)this->ModulConnection_ID, (Int32)E_Level_B_CMD::KM_Kommando_00);
  this->U2K_RequestMessage_Level_B_01           = this->KM_GlobalObjects->KM_Ads->Add_RequestMessage   <S_U2K_RequestMessage_Level_B_01>   ((Int32)this->ModulConnection_ID, (Int32)E_Level_B_CMD::KM_Kommando_01);
  this->K2U_ResponeMessage_Level_B_01           = this->KM_GlobalObjects->KM_Ads->Add_ResponseMessage  <S_K2U_ResponeMessage_Level_B_01>   ((Int32)this->ModulConnection_ID, (Int32)E_Level_B_CMD::KM_Kommando_01);
  this->U2K_RequestMessage_Level_B_02           = this->KM_GlobalObjects->KM_Ads->Add_RequestMessage   <S_U2K_RequestMessage_Level_B_02>   ((Int32)this->ModulConnection_ID, (Int32)E_Level_B_CMD::KM_Kommando_02);
  this->K2U_ResponeMessage_Level_B_02           = this->KM_GlobalObjects->KM_Ads->Add_ResponseMessage  <S_K2U_ResponeMessage_Level_B_02>   ((Int32)this->ModulConnection_ID, (Int32)E_Level_B_CMD::KM_Kommando_02);
  this->U2K_RequestMessage_Level_B_03           = this->KM_GlobalObjects->KM_Ads->Add_RequestMessage   <S_U2K_RequestMessage_Level_B_03>   ((Int32)this->ModulConnection_ID, (Int32)E_Level_B_CMD::KM_Kommando_03);
  this->K2U_ResponeMessage_Level_B_03           = this->KM_GlobalObjects->KM_Ads->Add_ResponseMessage  <S_K2U_ResponeMessage_Level_B_03>   ((Int32)this->ModulConnection_ID, (Int32)E_Level_B_CMD::KM_Kommando_03);

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

  // Kind-Objekt-Instanziierung
  this->KM_Level_C                              = new C_KM_Level_C        (this->KM_GlobalObjects, this->Level_B_Nr);
  }
            C_KM_Level_B::~C_KM_Level_B                                      ()
  {
  //  Kind-Objekt-DeInstanziierung
  delete (this->KM_Level_C);

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
  this->KM_GlobalObjects->KM_Ads->Release_Messages <S_U2K_RequestMessage_Level_B_03, S_K2U_ResponeMessage_Level_B_03> ((Int32)this->ModulConnection_ID, (Int32)E_Level_B_CMD::KM_Kommando_03);
  this->KM_GlobalObjects->KM_Ads->Release_Messages <S_U2K_RequestMessage_Level_B_02, S_K2U_ResponeMessage_Level_B_02> ((Int32)this->ModulConnection_ID, (Int32)E_Level_B_CMD::KM_Kommando_02);
  this->KM_GlobalObjects->KM_Ads->Release_Messages <S_U2K_RequestMessage_Level_B_01, S_K2U_ResponeMessage_Level_B_01> ((Int32)this->ModulConnection_ID, (Int32)E_Level_B_CMD::KM_Kommando_01);
  this->KM_GlobalObjects->KM_Ads->Release_Messages <S_U2K_RequestMessage_Level_B_00, S_K2U_ResponeMessage_Level_B_00> ((Int32)this->ModulConnection_ID, (Int32)E_Level_B_CMD::KM_Kommando_00);
  this->ModulConnection_ID                      = E_ModulConnection_ID::NoConnection;

  this->Level_B_Nr                              = -1;
  this->KM_GlobalObjects                        = nullptr;
  }

/*************************************************** Nicht öffentliche private Methoden *****************************************************/
void        C_KM_Level_B::Command_Interpreter                                (Int32 KM_Command)
  {
  E_Level_B_CMD MyKM_Command               = static_cast <E_Level_B_CMD> (KM_Command);

  // Interpetieren der Kommandos
  switch (MyKM_Command)
    {
    case E_Level_B_CMD::KM_Kommando_00:
      {
      }
    break;
    case E_Level_B_CMD::KM_Kommando_01:
      {
      }
    break;
    case E_Level_B_CMD::KM_Kommando_02:
      {
      }
    break;
    case E_Level_B_CMD::KM_Kommando_03:
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
Boolean     C_KM_Level_B::SM_Thread_1                                        (void)
  {
  return (true);
  }
Boolean     C_KM_Level_B::SM_Thread_2                                        (void)
  {
  return (true);
  }
Boolean     C_KM_Level_B::SM_Thread_3                                        (void)
  {
  return (true);
  } 
Boolean     C_KM_Level_B::SM_Thread_4                                        (void)
  {
  return (true);
  }
Boolean     C_KM_Level_B::SM_Thread_5                                        (void)
  {
  return (true);
  }

/******************************************************* Öffentliche Anwender-Methoden ******************************************************/
void        C_KM_Level_B::Cycle_Update                                       (void)
  {
  this->SM_Thread_1                 ();
  this->SM_Thread_2                 ();
  this->SM_Thread_3                 ();
  if (!this->SM_Thread4_finished) this->SM_Thread4_finished = this->SM_Thread_4 ();
  if (!this->SM_Thread5_finished) this->SM_Thread5_finished = this->SM_Thread_5 ();

  this->KM_Level_C->Cycle_Update              ();                                       // muss zwingend in der Cycle_Update-Methode des Mutter-Objekt aufgerufen werden !!!
  }
void        C_KM_Level_B::AdsReadWriteIndication                             (Int32 ModulConnection_ID, Int32 KM_Command)
  {
  E_ModulConnection_ID MyModulConnection_ID = static_cast<E_ModulConnection_ID> (ModulConnection_ID);

   if (MyModulConnection_ID == this->ModulConnection_ID)            // wenn, KM-Kommando für das hiesige Modul bestimmt, dann Auswertung durch den hiesige Command-Interpreter 
    {
    this->Command_Interpreter                 (KM_Command);
    }
  else                                                        // sonst Weitergabe an die Kind-Objekte; vgl Cycle-Update
    {
    this->KM_Level_C->AdsReadWriteIndication  (ModulConnection_ID, KM_Command);         // muss zwingend in der Cycle_Update-Methode des Mutter-Objekt aufgerufen werden !!!
    }
  }  // void AdsReadWriteIndication (Int32 ModulConnection_ID, Int32 KM_Command)


