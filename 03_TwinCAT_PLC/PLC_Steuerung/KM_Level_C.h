/********************************************************************************************************************************************/
/*                                                                                                                                          */
/*  Header-Datei: KM_Level_C.h                                                                                                              */
/*                                                                                                                                          */
/*  Prof. Dr. Lothar Zunker      09.06.2021                                                                                                 */
/*                                                                                                                                          */
/********************************************************************************************************************************************/
#pragma once

/************************************************************* System-Includes **************************************************************/
#include "PLC_SteuerungInterfaces.h"
#include "AdsR0.h"
#include "TcMath.h"
#include "..\..\01_Common\CommonTypes.h"              // notwendig für Deklarationen der Typen: Single, Double, Byte, Boolean, UInt16, Int16, Int32 , Int32, UInt64, Int64 (native C++-Variante zu using namespace System) 

/*********************************************************** Anwender-Includes **************************************************************/
#include "KM_GlobalObjects.h"
#include "..\..\01_Common\CD_Level_C.h"               // CommonDeclares des Moduls Level_C 


namespace Zunker
  {
  class C_KM_Level_C
    {
    /**************************************************************** Konstanten ****************************************************************/
    public:

    /******************************************************** Konstruktoren / Destruktor ********************************************************/
    public:
               C_KM_Level_C                           (C_KM_GlobalObjects* KM_GlobalObjects, Int32 Level_C_Nr);
               ~C_KM_Level_C                          ();

    /******************************************** Nicht öffentliche private Anwender-Attribute **************************************************/
    private:
    C_KM_GlobalObjects*                             KM_GlobalObjects;

    // ADS-Kommunikations-Initalisierung
    E_ModulConnection_ID                            ModulConnection_ID;
    S_U2K_RequestMessage_Level_C_00*                U2K_RequestMessage_Level_C_00;     // U2K_RequestMessage_SetDigitalOut aus CD_Level_C.h
    S_K2U_ResponeMessage_Level_C_00*                K2U_ResponeMessage_Level_C_00;     // K2U_ResponeMessage_SetDigitalOut aus CD_Level_C.h
    S_U2K_RequestMessage_Level_C_01*                U2K_RequestMessage_Level_C_01;
    S_K2U_ResponeMessage_Level_C_01*                K2U_ResponeMessage_Level_C_01;
    S_U2K_RequestMessage_Level_C_02*                U2K_RequestMessage_Level_C_02;
    S_K2U_ResponeMessage_Level_C_02*                K2U_ResponeMessage_Level_C_02;
    S_U2K_RequestMessage_Level_C_03*                U2K_RequestMessage_Level_C_03;
    S_K2U_ResponeMessage_Level_C_03*                K2U_ResponeMessage_Level_C_03;

    // Quasi-Parallel arbeitende Threads
    Int32                                           SM_Thread1_Schritt;
    Int32                                           SM_Thread1_Zaehler;
    Boolean                                         SM_Thread1_finished;

    Int32                                           SM_Thread2_Schritt;
    Int32                                           SM_Thread2_Zaehler;
    Boolean                                         SM_Thread2_finished;
    Int32                                           SM_Thread2_Zaehler_Max;
    
    Int32                                           SM_Thread3_Schritt;
    Int32                                           SM_Thread3_Zaehler;
    Boolean                                         SM_Thread3_finished;
    
    Int32                                           SM_Thread4_Schritt;
    Int32                                           SM_Thread4_Zaehler;
    Boolean                                         SM_Thread4_finished;

    Int32                                           SM_Thread5_Schritt;
    Int32                                           SM_Thread5_Zaehler;
    Boolean                                         SM_Thread5_finished;

    /**************************************************** Öffentliche Anwender-Attribute ********************************************************/
    public:
    // Kind-Objekt-Instanziierung 
    Int32                                           Level_C_Nr;


    /*************************************************** Nicht öffentliche private Methoden *****************************************************/
    private:
    void       Command_Interpreter                    (Int32 KM_Command);

    // Quasi-Parallel arbeitende Threads
    Boolean    SM_Thread_1                            (void);
    Boolean    SM_Thread_2                            (void);
    Boolean    SM_Thread_3                            (void);
    Boolean    SM_Thread_4                            (void);
    Boolean    SM_Thread_5                            (void);

    /******************************************************* Öffentliche Anwender-Methoden ******************************************************/
    public:
    void       Cycle_Update                           (void);                                      // muss zwingend in der Cycle_Update-Methode des Mutter-Objekt aufgerufen werden !!!
    void       AdsReadWriteIndication                 (Int32 ModulAdress, Int32 KM_Command);       // muss zwingend in der AdsReadWriteIndication-Methode des Mutter-Objekt aufgerufen werden !!!
    };
  } // namespace Zunker


