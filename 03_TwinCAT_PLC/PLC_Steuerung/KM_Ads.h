/********************************************************************************************************************************************/
/*                                                                                                                                          */
/*  Header-Datei: KM_Ads.h                                                                                                                  */
/*                                                                                                                                          */
/*  Prof. Dr. Lothar Zunker      06.06.2021                                                                                                 */
/*                                                                                                                                          */
/********************************************************************************************************************************************/
#pragma once

/************************************************************* System-Includes **************************************************************/
#include "PLC_SteuerungInterfaces.h"
#include "AdsR0.h"
#include "TcMath.h"
#include "..\..\01_Common\CommonTypes.h"              // notwendig für Deklarationen der Typen: Single, Double, Byte, Boolean, UInt16, Int16, Int32 , Int32, UInt64, Int64 (native C++-Variante zu using namespace System) 
#include <map>

/************************************************************ Anwender-Includes *************************************************************/

namespace Zunker
  {
  const Int32       Cmd_Streaming               =    0xFFFF;

  enum class        E_TransferType
    {
    NoType = 0,
    U2K    = 1,
    K2U    = 2
    };

  struct            S_Datensatz_Header
    {
    Int32    Feld_ByteAnzahl;
    };
  struct            S_Message_Header
    {
    Int32    Feld_ByteAnzahl;
    Int32    DatensatzAnzahl;
    Int32    U2K_RB_WritePlaces_free;  
    Int32    K2U_RB_WritePlaces_free;
    };
  class             C_KM_TransferMessage
    {
      public:
      /****************************************** Nicht öffentliche private Anwender-Deklarationen ************************************************/

      /*************************************************************** Konstruktoren **************************************************************/
      public:
      C_KM_TransferMessage                                               (E_TransferType TransferType, Int32  KM_Message_Kapazitaet, Int32  KM_Message_DatensatzAnzahl_max, Int32  KM_Ringspeicher_Kapazitaet);
      /**************************************************************** Destruktor ****************************************************************/
      public:
      ~C_KM_TransferMessage                                              ();

      /******************************************************* Öffentliche Anwender-Methoden ******************************************************/
      public:

      /********************************************** Nicht öffentliche private Anwender-Attribute ************************************************/
      private:

      /**************************************************** Öffentliche Anwender-Attribute ********************************************************/
      public:
      E_TransferType                            TransferType;
      Int32                                     Message_Kapazitaet;
      Int32                                     Message_DatensatzAnzahl_max;
      Byte*                                     Message;
      S_Message_Header*                         Message_Header;
      Byte*                                     Message_Feld;
      Byte*                                     Message_Datensatz;
    };

  struct            S_KM_Ringspeicherelement
    {
    S_KM_Ringspeicherelement*                  Nachfolger;
    Byte*                                      Datensatz;
    };
  class             C_KM_Ringspeicher
    {
    public:
    /****************************************** Nicht öffentliche private Anwender-Deklarationen ************************************************/

    /*************************************************************** Konstruktoren **************************************************************/
    public:
    C_KM_Ringspeicher                                                  (Int32  KM_Ringspeicher_Kapazitaet);
    /**************************************************************** Destruktor ****************************************************************/
    public:
    ~C_KM_Ringspeicher                                                 ();

    /*************************************************** Nicht öffentliche private Methoden *****************************************************/
    private:
    void                                      std_memcpy               (Byte* Dst, Byte* Src, Int32  Size);

    /******************************************************* Öffentliche Anwender-Methoden ******************************************************/
    public:
    void                                      move_to_RB               (Byte* Datensatz, Int32  Datensatz_ByteAnzahl);
    void                                      move_from_RB             (Byte* Datensatz, Int32 & Datensatz_ByteAnzahl);
    /********************************************** Nicht öffentliche private Anwender-Attribute ************************************************/
    private:

    /**************************************************** Öffentliche Anwender-Attribute ********************************************************/
    public:
    Int32                                     Ringspeicher_Kapazitaet;

    S_KM_Ringspeicherelement*                 Read_Pointer;
    S_KM_Ringspeicherelement*                 Write_Pointer;

    Int32                                    KM_RB_ReadPlaces_occupied;
    Int32                                    KM_RB_WritePlaces_free;

    Int32                                    UM_RB_WritePlaces_free;
    };

  struct            S_KM_QueueElement
    {
    S_KM_QueueElement* Nachfolger;
    Byte* Datensatz;
    };
  class             C_KM_Queue
    {
    public:
    /****************************************** Nicht öffentliche private Anwender-Deklarationen ************************************************/

    /*************************************************************** Konstruktoren **************************************************************/
    public:
          C_KM_Queue                                  (Int32  KM_Queue_Kapazitaet);
    /**************************************************************** Destruktor ****************************************************************/
    public:
          ~C_KM_Queue                                 ();

    /*************************************************** Nicht öffentliche private Methoden *****************************************************/
    private:
    void                     std_memcpy                                  (Byte* Dst, Byte* Src, Int32  Size);

    /******************************************************* Öffentliche Anwender-Methoden ******************************************************/
    public:
    void                     move_to_QU                                  (Byte* Datensatz, Int32  Datensatz_ByteAnzahl);
    void                     move_from_QU                                (Byte* Datensatz, Int32& Datensatz_ByteAnzahl);

    /********************************************** Nicht öffentliche private Anwender-Attribute ************************************************/
    private:

    /**************************************************** Öffentliche Anwender-Attribute ********************************************************/
    public:
    Int32                                     Queue_Kapazitaet;

    S_KM_QueueElement*                        Read_Pointer;
    S_KM_QueueElement*                        Write_Pointer;

    Int32                                     KM_QU_ReadPlaces_occupied;
    Int32                                     KM_QU_WritePlaces_free;

    Int32                                     UM_QU_WritePlaces_free;
    };

  struct            S_CmdMessageBundle
    {
    Byte*                                               U2K_ByteArray;
    Int32                                               U2K_ByteArray_Byteanzahl;

    Byte*                                               K2U_ByteArray;
    Int32                                               K2U_ByteArray_Byteanzahl;
    };
  class             C_KM_Ads
    {
    /****************************************** Nicht öffentliche private Anwender-Deklarationen ************************************************/

    /*************************************************************** Konstruktoren **************************************************************/
    public:
                             C_KM_Ads                                    ();

    /**************************************************************** Destruktor ****************************************************************/
    public:
                             ~C_KM_Ads                                   ();

    /********************************************** Nicht öffentliche private Anwender-Attribute ************************************************/
    private:
    std::map<Int32,std::map<Int32, S_CmdMessageBundle*>*>*  ListOfListOfCmdMessageBundles;


    /**************************************************** Öffentliche Anwender-Attribute ********************************************************/
    public:                                                 
    Int32                                                   KM_ModulConnection_ID;
    Int32                                                   KM_Cmd_Nr;

    // Streaming-Mechanismen
    std::map<Int32, C_KM_Ringspeicher*>*                    KM_U2K_Ringbuffers;
    std::map<Int32, C_KM_Queue*>*                           KM_U2K_Queues;
    std::map<Int32, C_KM_TransferMessage*>*                 KM_U2K_TransferMessages;

    std::map<Int32, C_KM_Ringspeicher*>*                    KM_K2U_Ringbuffers;
    std::map<Int32, C_KM_Queue*>*                           KM_K2U_Queues;
    std::map<Int32, C_KM_TransferMessage*>*                 KM_K2U_TransferMessages;

    /*********************************************** Nicht öffentliche private Anwender-Methoden ************************************************/
    private:
  
    // Streaming-Mechanismen
    void                    std_memcpy                                  (Byte* Dst, Byte* Src, Int32  Size);
    void                    Transfer_U2K_TM_Datensaetze_to_U2K_RB       (C_KM_Ringspeicher* KM_U2K_Ringbuffer, C_KM_Ringspeicher* KM_K2U_Ringbuffer, C_KM_TransferMessage* KM_U2K_TransferMessage);    // downward
    void                    Transfer_K2U_RB_Datensaetze_to_K2U_TM       (C_KM_TransferMessage* KM_K2U_TransferMessage, C_KM_Ringspeicher* KM_K2U_Ringbuffer, C_KM_Ringspeicher* KM_U2K_Ringbuffer);    // upward
    
    void                    Transfer_U2K_TM_Datensaetze_to_U2K_QU       (C_KM_Queue* KM_U2K_Queue, C_KM_Queue* KM_K2U_Queue, C_KM_TransferMessage* KM_U2K_TransferMessage);                            // downward
    void                    Transfer_K2U_QU_Datensaetze_to_K2U_TM       (C_KM_TransferMessage* KM_K2U_TransferMessage, C_KM_Queue* KM_K2U_Queue, C_KM_Queue* KM_U2K_Queue);                            // upward

    /******************************************************* Öffentliche Anwender-Methoden ******************************************************/
    public:
    // Commanding-Mechanismen
    template <typename U2K_RequestProtokoll>
    U2K_RequestProtokoll*   Add_RequestMessage                          (Int32 ModulConnection_ID, Int32 Cmd_Nr);
    template <typename K2U_ResponseProtokoll>
    K2U_ResponseProtokoll*  Add_ResponseMessage                         (Int32 ModulConnection_ID, Int32 Cmd_Nr);
    template <typename U2K_RequestProtokoll, typename K2U_ResponseProtokoll>
    void                    Release_Messages                            (Int32 ModulConnection_ID, Int32 Cmd_Nr);

    // Streaming-Mechanismen
    void                    Open_Streaming                              (Int32 ModulConnection_ID, Int32  Ringspeicher_Kapazitaet, Int32  Message_Kapazitaet, Int32  Message_DatensatzAnzahl_max);
    void                    Close_Streaming                             (Int32 ModulConnection_ID);

    void                    Transfer_KM_U2K_RB_Datensatz_to_Kernel      (Int32 ModulConnection_ID, Byte*  KM_U2K_Datensatz);                                                                                    // downward
    Int32                   Get_KM_U2K_RB_ReadPlaces_occupied           (Int32 ModulConnection_ID);
    Int32                   Get_KM_U2K_RB_WritePlaces_free              (Int32 ModulConnection_ID);
    Int32                   Get_UM_U2K_RB_WritePlaces_free              (Int32 ModulConnection_ID);
    
    void                    Transfer_Kernel_Datensatz_to_KM_K2U_RB      (Int32 ModulConnection_ID, Byte*  KM_K2U_Datensatz);                                                                                    // upward
    Int32                   Get_KM_K2U_RB_WritePlaces_free              (Int32 ModulConnection_ID);
    Int32                   Get_UM_K2U_RB_WritePlaces_free              (Int32 ModulConnection_ID);

    void                    CycleUpdate_RB_Streaming                    (Int32 ModulConnection_ID);

    void                    Transfer_KM_U2K_QU_Datensatz_to_Kernel      (Int32 ModulConnection_ID, Byte*  KM_U2K_Datensatz);                                                                                    // downward
    Int32                   Get_KM_U2K_QU_ReadPlaces_occupied           (Int32 ModulConnection_ID);
    Int32                   Get_KM_U2K_QU_WritePlaces_free              (Int32 ModulConnection_ID);
    Int32                   Get_KM_U2K_QU_Kapazitaet                    (Int32 ModulConnection_ID);
    void                    Set_KM_U2K_QU_Kapazitaet                    (Int32 ModulConnection_ID, Int32 KM_U2K_QU_Kapazitaet);                                                                                // upward
    Int32                   Get_UM_U2K_QU_WritePlaces_free              (Int32 ModulConnection_ID);

    void                    Transfer_Kernel_Datensatz_to_KM_K2U_QU      (Int32 ModulConnection_ID, Byte*  KM_K2U_Datensatz);
    Int32                   Get_KM_K2U_QU_WritePlaces_free              (Int32 ModulConnection_ID);
    Int32                   Get_KM_K2U_QU_Kapazitaet                    (Int32 ModulConnection_ID);
    void                    Set_KM_K2U_QU_Kapazitaet                    (Int32 ModulConnection_ID, Int32 KM_K2U_QU_Kapazitaet);
    Int32                   Get_UM_K2U_QU_WritePlaces_free              (Int32 ModulConnection_ID);

    void                    CycleUpdate_QU_Streaming                    (Int32 ModulConnection_ID);
   
    // Commanding-Mechanismen & Streaming-Mechanismus
    Boolean                 AdsReadWriteIndication                      (Int32 ModulConnection_ID, Int32 Cmd_Nr, ULONG& cbReadLength, void*& pReadData, ULONG& cbWriteLength, void*& pWriteData);
    };
  }   // namespace Zunker

#include "KM_Ads.hpp"                                   // Achtung: hier Einbindung der hpp-Datei der Templates !!!!
