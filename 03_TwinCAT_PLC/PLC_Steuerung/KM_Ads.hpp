/********************************************************************************************************************************************/
/*                                                                                                                                          */
/*  Template-Definition-File KM_Ads.hpp                                                                                                     */
/*                                                                                                                                          */
/*  Prof. Dr. Lothar Zunker      06.06.2021                                                                                                 */
/*                                                                                                                                          */
/********************************************************************************************************************************************/
#pragma once
#pragma hdrstop

/************************************************************ Anwender-Includes *************************************************************/
using namespace Zunker;                        // aus KM_Ads.h

/********************************************************************************************************************************************/
template <typename U2K_RequestProtokoll>
U2K_RequestProtokoll*    C_KM_Ads::Add_RequestMessage           (Int32 ModulConnection_ID, Int32 Cmd_Nr)
  {// ModulConnection_ID = Adresse der UM- und KM-Objekte; Cmd_Nr = Kommando-Nr innerhalb eines UM- bzw. KM-Objektes
  std::map<Int32, S_CmdMessageBundle*>*   ListOfCmdMessageBundles;
  if (this->ListOfListOfCmdMessageBundles->count(ModulConnection_ID) == 0)                               // Wenn Modul-Adresse noch nicht vorhanden, dann neu Liste ListOfCmdMessageBundles erzeugen und und um erstes Kommando mit Cmd-Nr auffüllen
    {
    ListOfCmdMessageBundles                                       = new std::map<Int32, S_CmdMessageBundle*> ();                                // neue Liste von CmdMessageBundles erzeugen
    this->ListOfListOfCmdMessageBundles->insert (std::pair<Int32, std::map<Int32, S_CmdMessageBundle*>* >(ModulConnection_ID, ListOfCmdMessageBundles));  // in Liste der Liste von CmdMessageBundles mit ModulConnection_ID verknüpft eintragen
    }
  else
    {
    ListOfCmdMessageBundles                                       = (*this->ListOfListOfCmdMessageBundles)[ModulConnection_ID];
    }

  S_CmdMessageBundle*                     CmdMessageBundle;
  if (ListOfCmdMessageBundles->count(Cmd_Nr)                == 0)
    {
    CmdMessageBundle                                              = new S_CmdMessageBundle ();
    ListOfCmdMessageBundles->insert             (std::pair<Int32, S_CmdMessageBundle*                   >(Cmd_Nr, CmdMessageBundle));
    }
  else
    {
    CmdMessageBundle                                              = (*ListOfCmdMessageBundles)[Cmd_Nr];
    }
  U2K_RequestProtokoll* U2K_RequestMessage                        = new  U2K_RequestProtokoll ();
  UInt32                U2K_RequestMessage_Byteanzahl             = sizeof (U2K_RequestProtokoll);

  CmdMessageBundle->U2K_ByteArray                                 = reinterpret_cast<Byte *>(U2K_RequestMessage);
  CmdMessageBundle->U2K_ByteArray_Byteanzahl                      = U2K_RequestMessage_Byteanzahl;

  return (U2K_RequestMessage);
  } //    C_KM_Ads::Add_RequestMessage                               (Int32 ModulConnection_ID, Int32 Cmd_Nr, Int32* KM_Command_Request)

/********************************************************************************************************************************************/
template <typename K2U_ResponseProtokoll>
K2U_ResponseProtokoll*   C_KM_Ads::Add_ResponseMessage          (Int32 ModulConnection_ID, Int32 Cmd_Nr)
  {// ModulConnection_ID = Adresse der UM- und KM-Objekte; Cmd_Nr = Kommando-Nr innerhalb eines UM- bzw. KM-Objektes
  std::map<Int32, S_CmdMessageBundle*>*   ListOfCmdMessageBundles;
  if (this->ListOfListOfCmdMessageBundles->count(ModulConnection_ID) == 0)                               // Wenn Modul-Adresse noch nicht vorhanden, dann neu Liste ListOfCmdMessageBundles erzeugen und und um erstes Kommando mit Cmd-Nr auffüllen
    {
    ListOfCmdMessageBundles                                       = new std::map<Int32, S_CmdMessageBundle*> ();                                // neue Liste von CmdMessageBundles erzeugen
    this->ListOfListOfCmdMessageBundles->insert (std::pair<Int32, std::map<Int32, S_CmdMessageBundle*>* >(ModulConnection_ID, ListOfCmdMessageBundles));  // in Liste der Liste von CmdMessageBundles mit ModulConnection_ID verknüpft eintragen
    }
  else
    {
    ListOfCmdMessageBundles                                       = (*this->ListOfListOfCmdMessageBundles)[ModulConnection_ID];
    }

  S_CmdMessageBundle*                     CmdMessageBundle;
  if (ListOfCmdMessageBundles->count(Cmd_Nr)                == 0)
    {
    CmdMessageBundle                                              = new S_CmdMessageBundle ();
    ListOfCmdMessageBundles->insert             (std::pair<Int32, S_CmdMessageBundle*                   >(Cmd_Nr, CmdMessageBundle));
    }
  else
    {
    CmdMessageBundle                                              = (*ListOfCmdMessageBundles)[Cmd_Nr];
    }
 
  K2U_ResponseProtokoll* K2U_ResponseMessage                      = new  K2U_ResponseProtokoll ();
  UInt32                 K2U_ResponseMessage_Byteanzahl           = sizeof (K2U_ResponseProtokoll);

  CmdMessageBundle->K2U_ByteArray                                 = reinterpret_cast<Byte *>(K2U_ResponseMessage);
  CmdMessageBundle->K2U_ByteArray_Byteanzahl                      = K2U_ResponseMessage_Byteanzahl;

  return (K2U_ResponseMessage);
  } //    K2U_ResponseProtokoll*   C_KM_Ads::Add_ResponseMessage                              (Int32 ModulConnection_ID, Int32 Cmd_Nr, Int32* KM_Command_Request)

/********************************************************************************************************************************************/
template <typename U2K_RequestProtokoll, typename K2U_ResponseProtokoll>
void                     C_KM_Ads::Release_Messages             (Int32 ModulConnection_ID, Int32 Cmd_Nr)
  {// ModulConnection_ID = Adresse der UM- und KM-Objekte; Cmd_Nr = Kommando-Nr innerhalb eines UM- bzw. KM-Objektes
  if (this->ListOfListOfCmdMessageBundles->count(ModulConnection_ID) == 1)                               // Wenn Schlüssel ModulConnection_ID in map noch (einmalig) vorhanden, dann Datum ListOfCmdMessageBundles zum Schlüssel ModulConnection_ID der map entnehmen 
    {
    std::map<Int32, S_CmdMessageBundle*>* ListOfCmdMessageBundles = (*this->ListOfListOfCmdMessageBundles)[ModulConnection_ID];

    if (ListOfCmdMessageBundles->count(Cmd_Nr)              == 1)                               // Wenn Schlüssel Cmd_Nr in map noch (einmalig) vorhanden, dann Datum CmdMessageBundle zum Schlüssel Cmd_Nr der map entnehmen, deinitialisieren, 
      {
      S_CmdMessageBundle*                 CmdMessageBundle        = (*ListOfCmdMessageBundles)[Cmd_Nr];

      K2U_ResponseProtokoll* K2U_ResponseMessage                  = reinterpret_cast<K2U_ResponseProtokoll*>(CmdMessageBundle->K2U_ByteArray);
      delete (K2U_ResponseMessage);
      CmdMessageBundle->K2U_ByteArray_Byteanzahl                  = 0;
      CmdMessageBundle->K2U_ByteArray                             = nullptr;

      U2K_RequestProtokoll* U2K_RequestMessage                    = reinterpret_cast<U2K_RequestProtokoll*>(CmdMessageBundle->U2K_ByteArray);
      delete (U2K_RequestMessage);
      CmdMessageBundle->U2K_ByteArray_Byteanzahl                  = 0;
      CmdMessageBundle->U2K_ByteArray                             = nullptr;

      delete  (CmdMessageBundle);
      ListOfCmdMessageBundles->erase (Cmd_Nr);                                                  // CmdMessageBundle-Cmd_Nr-Kombination aus ListOfCmdMessageBundles enfernen 
      } // if (ListOfCmdMessageBundles->count(Cmd_Nr)       == 1)

    if (ListOfCmdMessageBundles->size() == 0)                                                   // wenn letztes CmdMessageBundle-Cmd_Nr-Kombi aus ListOfCmdMessageBundles zum Schlüsssel ModulConnection_ID gelöscht ist, dann 
      {
      this->ListOfListOfCmdMessageBundles->erase (ModulConnection_ID);                                   // dann lösche ListOfCmdMessageBundles-ModulConnection_ID-Kombi aus ListofListOfCmdMessageBundles
      }
    } // if (this->ListOfListOfCmdMessageBundles->count(ModulConnection_ID) == 1)) 
  } //    void        C_KM_Ads::Release_Messages                                 (Int32 ModulConnection_ID, Int32 Cmd_Nr)
