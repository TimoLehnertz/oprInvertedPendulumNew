/********************************************************************************************************************************************/
/*                                                                                                                                          */
/*  Source-Code-Datei: KM_Ads.cpp                                                                                                           */
/*                                                                                                                                          */
/*  Prof. Dr. Lothar Zunker      06.06.2021                                                                                                 */
/*                                                                                                                                          */
/********************************************************************************************************************************************/
#pragma once
#pragma hdrstop

/************************************************************ Anwender-Includes *************************************************************/
#include "KM_Ads.h"

using namespace Zunker;                        // aus KM_Ads.h

/********************************************************************************************************************************************/
/*  Methoden-Definitionen der Klasse: C_KM_TransferMessage                                                                                  */
/********************************************************************************************************************************************/
/*************************************************************** Konstruktoren **************************************************************/
                 C_KM_TransferMessage::C_KM_TransferMessage                      (E_TransferType TransferType, Int32  KM_Message_Kapazitaet, Int32  KM_Message_DatensatzAnzahl_max, Int32  KM_Ringspeicher_Kapazitaet)
  {
  this->TransferType                                           = TransferType;
  this->Message_Kapazitaet                                     = KM_Message_Kapazitaet;
  this->Message_DatensatzAnzahl_max                            = KM_Message_DatensatzAnzahl_max;
  
  // Instanziierung der TransferMessage als einfaches ausreichend gro�es Byte-Array
  this->Message                                                = new Byte[static_cast<size_t>(this->Message_Kapazitaet)];
  this->Message_Header                                         = reinterpret_cast<S_Message_Header*>(this->Message);
  this->Message_Feld                                           = reinterpret_cast<Byte*>            (this->Message + sizeof (S_Message_Header));
  this->Message_Header->Feld_ByteAnzahl                        = 0;
  this->Message_Header->DatensatzAnzahl                        = 0;
  this->Message_Header->U2K_RB_WritePlaces_free                = KM_Ringspeicher_Kapazitaet;
  this->Message_Header->K2U_RB_WritePlaces_free                = KM_Ringspeicher_Kapazitaet;
             }
/**************************************************************** Destruktor ****************************************************************/
                 C_KM_TransferMessage::~C_KM_TransferMessage                     ()
  {
  // De-Instanziierung der TransferMessage
  this->Message_Header->K2U_RB_WritePlaces_free                = 0;
  this->Message_Header->U2K_RB_WritePlaces_free                = 0;
  this->Message_Header->DatensatzAnzahl                        = 0;
  this->Message_Header->Feld_ByteAnzahl                        = 0;
  this->Message_Feld                                           = nullptr;
  this->Message_Header                                         = nullptr;
  delete[] (this->Message);

  this->Message_DatensatzAnzahl_max                            = 0;
  this->Message_Kapazitaet                                     = 0;
  this->TransferType                                           = E_TransferType::NoType;
  }
/******************************************************* �ffentliche Anwender-Methoden ******************************************************/

/********************************************************************************************************************************************/
/*  Methoden-Definitionen der Klasse: C_KM_Ringspeicher                                                                                     */
/********************************************************************************************************************************************/
/*************************************************************** Konstruktoren **************************************************************/
                 C_KM_Ringspeicher::C_KM_Ringspeicher                            (Int32  UM_Ringspeicher_Kapazitaet)
  {
  this->Ringspeicher_Kapazitaet                                 = UM_Ringspeicher_Kapazitaet;

  // Instanziierung des Ringspeichers als einfach vorw�rts verketteten Liste von Ringspeicher-Elementen
  this->Read_Pointer                                            = new S_KM_Ringspeicherelement ();
  this->Write_Pointer                                           = this->Read_Pointer;
  for (Int32  i = 0; i < this->Ringspeicher_Kapazitaet; i++)                                           // Der Ringspeicher wurde zur Sicherheit um 10 Pl�tz gr��er gew�hlt als offziell angegeben
    {
    this->Write_Pointer->Nachfolger                             = new S_KM_Ringspeicherelement ();
    this->Write_Pointer->Datensatz                              = nullptr;

    this->Write_Pointer                                         = this->Write_Pointer->Nachfolger;
    }
  this->Write_Pointer->Nachfolger                               = this->Read_Pointer;
  this->Write_Pointer->Datensatz                                = nullptr;

  this->Write_Pointer                                           = this->Read_Pointer;                     // Read_Pointer und Write_Pointer m�ssen �bereinstimmen !!!!!

  this->KM_RB_ReadPlaces_occupied                               = 0;
  this->KM_RB_WritePlaces_free                                  = Ringspeicher_Kapazitaet;
  this->UM_RB_WritePlaces_free                                  = Ringspeicher_Kapazitaet;
  }
/**************************************************************** Destruktor ****************************************************************/
                 C_KM_Ringspeicher::~C_KM_Ringspeicher                           ()
  {
  this->UM_RB_WritePlaces_free                                  = 0;
  this->KM_RB_WritePlaces_free                                  = 0;
  this->KM_RB_ReadPlaces_occupied                               = 0;

  this->Write_Pointer                                           = this->Read_Pointer;                     // Read_Pointer und Write_Pointer m�ssen �bereinstimmen !!!!!

  // De-Instanziierung des Ringspeichers
  for (Int32  i = 0; i < this->Ringspeicher_Kapazitaet; i++)                                           // Der Ringspeicher wurde zur Sicherheit um 10 Pl�tz gr��er gew�hlt als offziell angegeben
    {
    this->Read_Pointer                                          = this->Write_Pointer->Nachfolger;
    this->Write_Pointer->Datensatz                              = nullptr;
    this->Write_Pointer->Nachfolger                             = nullptr;
    delete (this->Write_Pointer);
    this->Write_Pointer                                         = this->Read_Pointer;
    }
  this->Write_Pointer                                           = nullptr;
  this->Read_Pointer                                            = nullptr;

  this->Ringspeicher_Kapazitaet                                 = 0;
  }
/*********************************************** Nicht �ffentliche private Anwender-Methoden ************************************************/
void             C_KM_Ringspeicher::std_memcpy                                   (Byte* Dst, Byte* Src, Int32  Size)
  {
  size_t                     Size2                                   = static_cast<size_t>(Size);
  std::memcpy (Dst, Src, Size2);        // effizienter als for-Schleife
  }
/******************************************************* �ffentliche Anwender-Methoden ******************************************************/
void             C_KM_Ringspeicher::move_to_RB                                   (Byte* Datensatz, Int32  Datensatz_ByteAnzahl)
  {
  this->Write_Pointer->Datensatz                                 = new Byte[static_cast<size_t>(Datensatz_ByteAnzahl)];                    // Instanziiere DatensatzArray f�r Ringspeicher
  std_memcpy (this->Write_Pointer->Datensatz, Datensatz, Datensatz_ByteAnzahl);            // effizienter als for-Schleife

  /* Ringbuffer-Write_Pointer-Increment */
  this->Write_Pointer                                            = this->Write_Pointer->Nachfolger;                                        // Setzen des Schreibzeigers auf n�chsten Ringspeicher-Platz
  
  this->KM_RB_ReadPlaces_occupied                               += 1;                                                                      // Inkrementieren der besetzten RB-Pl�tze
  this->KM_RB_WritePlaces_free                                  -= 1;                                                                      // Dekrementieren der freien RB-Pl�tze
  }
void             C_KM_Ringspeicher::move_from_RB                                 (Byte* Datensatz, Int32& Datensatz_ByteAnzahl)
  {
  S_Datensatz_Header*    RB_Datensatz_Header                     = reinterpret_cast<S_Datensatz_Header*>(this->Read_Pointer->Datensatz);
  Datensatz_ByteAnzahl                                           = RB_Datensatz_Header->Feld_ByteAnzahl + sizeof(S_Datensatz_Header);      // Datensatz_ByteAnzahl =  Datensatz_Feld_ByteAnzahl + Datensatz_Header_ByteAnzahl; (Datensatz_Header_ByteAnzahl = 2x Int32  = 8)

  std_memcpy (Datensatz, this->Read_Pointer->Datensatz, Datensatz_ByteAnzahl);                                                             // effizienter als for-Schleife
  delete[] (this->Read_Pointer->Datensatz);                                                                                                // De-Instanziiere DatensatzArray f�r Ringspeicher

  /* Ringbuffer-Read_Pointer-PostIncrement */
  this->Read_Pointer                                             = this->Read_Pointer->Nachfolger;                                         // Setzen des Lesezeigers auf n�chsten Ringspeicher-Platz

  this->KM_RB_ReadPlaces_occupied                               -= 1;                                                                      // Dekrementieren der besetzten RB-Pl�tze
  this->KM_RB_WritePlaces_free                                  += 1;                                                                      // Inkrementieren der freien RB-Pl�tz
  }

/********************************************************************************************************************************************/
/*  Methoden-Definitionen der Klasse: C_KM_Queue                                                                                           */
/********************************************************************************************************************************************/
/*************************************************************** Konstruktoren **************************************************************/
                 C_KM_Queue::C_KM_Queue                                          (Int32 KM_Queue_Kapazitaet)
  {
  this->Queue_Kapazitaet                                         = KM_Queue_Kapazitaet;

  // Instanziierung der Queue mit einem einzigen Queue-Element
  this->Write_Pointer                                            = new S_KM_QueueElement ();
  this->Write_Pointer->Nachfolger                                = nullptr;
  this->Write_Pointer->Datensatz                                 = nullptr;

  this->Read_Pointer                                             = this->Write_Pointer;

  this->KM_QU_ReadPlaces_occupied                                = 0;
  this->KM_QU_WritePlaces_free                                   = this->Queue_Kapazitaet;
  this->UM_QU_WritePlaces_free                                   = this->Queue_Kapazitaet;
  }
/**************************************************************** Destruktor ****************************************************************/
                 C_KM_Queue::~C_KM_Queue                                         ()
  {
  this->UM_QU_WritePlaces_free                                   = 0;
  this->KM_QU_WritePlaces_free                                   = 0;
  this->KM_QU_ReadPlaces_occupied                                = 0;

  // Sofern die Queque nicht leergelaufen ist, wird diese hier bis auf ein Queue-Element de-intanziiert
  while (this->Read_Pointer != this->Write_Pointer)  
    {
    delete (this->Read_Pointer->Datensatz);
    S_KM_QueueElement* Nachfolger_Pointer                        = this->Read_Pointer->Nachfolger;                                      // Retten des Nachfolger-Queue-Elementes
    this->Read_Pointer->Nachfolger                               = nullptr;
    delete (this->Read_Pointer);

    this->Read_Pointer                                           = Nachfolger_Pointer;
    }
  //De-Instanziierung der Queue mit einem einzigen Queue-Element 
  this->Read_Pointer->Nachfolger                                 = nullptr;
  this->Read_Pointer->Datensatz                                  = nullptr;
  delete (this->Read_Pointer);

  this->Queue_Kapazitaet                                         = 0;
  }
/*********************************************** Nicht �ffentliche private Anwender-Methoden ************************************************/
void             C_KM_Queue::std_memcpy                                          (Byte* Dst, Byte* Src, Int32  Size)
  {
  size_t                     Size2                                   = static_cast<size_t>(Size);
  std::memcpy (Dst, Src, Size2);        // effizienter als for-Schleife
  }
/******************************************************* �ffentliche Anwender-Methoden ******************************************************/
void             C_KM_Queue::move_to_QU                                          (Byte* Datensatz, Int32  Datensatz_ByteAnzahl)
  {
  this->Write_Pointer->Datensatz                                     = new Byte[static_cast<size_t>(Datensatz_ByteAnzahl)];                   // Instanziieren des  DatensatzArray f�r Ringspeicher
  std_memcpy (this->Write_Pointer->Datensatz, Datensatz, Datensatz_ByteAnzahl);                // effizienter als for-Schleife
  this->Write_Pointer->Nachfolger                                    = new S_KM_QueueElement ();                                              // Instanziieren des n�chstes neuen Queue-Elementes

  /* Queue-Write_Pointer Increment */
  this->Write_Pointer                                                = this->Write_Pointer->Nachfolger;                                       // Springen zum neuen Queue-Element
  this->Write_Pointer->Nachfolger                                    = nullptr;                                                               // Vorbesetzen mit Null-Pointer
  this->Write_Pointer->Datensatz                                     = nullptr;                                                               // Vorbesetzen mit Null-Pointer

  this->KM_QU_ReadPlaces_occupied                                   += 1;                                                                     // Inkrementieren der besetzten QU-Pl�tze
  this->KM_QU_WritePlaces_free                                      -= 1;                                                                     // Dekrementieren der freien QU-Pl�tze
  }
void             C_KM_Queue::move_from_QU                                        (Byte* Datensatz, Int32& Datensatz_ByteAnzahl)
  {
  S_Datensatz_Header*     QU_Datensatz_Header                         = reinterpret_cast<S_Datensatz_Header*>(this->Read_Pointer->Datensatz);
  Datensatz_ByteAnzahl                                                = QU_Datensatz_Header->Feld_ByteAnzahl + sizeof(S_Datensatz_Header);   // Datensatz_ByteAnzahl =  Datensatz_Feld_ByteAnzahl + Datensatz_Header_ByteAnzahl; (Datensatz_Header_ByteAnzahl = 2x Int32  = 8)

  std_memcpy (Datensatz, this->Read_Pointer->Datensatz, Datensatz_ByteAnzahl);                                                               // effizienter als for-Schleife
  delete[](this->Read_Pointer->Datensatz);                                                                                                   // De-Instanziieren des DatensatzArray f�r Queue
  S_KM_QueueElement*      Nachfolger_Pointer                          = this->Read_Pointer->Nachfolger;                                      // Retten des Nachfolger-Queue-Elementes
  this->Read_Pointer->Nachfolger                                      = nullptr;
  delete (this->Read_Pointer);                                                                                                               // De-Instanziieren des vorherigen alten Queue-Elementes

  /* Queue-Read_Pointer-PostIncrement */
  this->Read_Pointer                                                  = Nachfolger_Pointer;                                                  // Springen zum n�chsten Queue-Element
  
  this->KM_QU_ReadPlaces_occupied                                    -= 1;                                                                   // Dekrementieren der besetzten QU-Pl�tze
  this->KM_QU_WritePlaces_free                                       += 1;                                                                   // Inkrementieren der freien QU-Pl�tze
  }

/********************************************************************************************************************************************/
/*  Methoden-Definitionen der Klasse: C_UM_Ads                                                                                              */
/********************************************************************************************************************************************/
/*************************************************************** Konstruktoren **************************************************************/
                 C_KM_Ads::C_KM_Ads                                              ()    // Standard-Konstruktor
  {
  // Commanding-Mechanismen
  this->ListOfListOfCmdMessageBundles                           = new  std::map<Int32, std::map<Int32, S_CmdMessageBundle*>*>;
  
  //  Streaming-Mechanismen
  this->KM_U2K_Ringbuffers                                      = new  std::map<Int32, C_KM_Ringspeicher*   > ();
  this->KM_U2K_Queues                                           = new  std::map<Int32, C_KM_Queue*          > ();
  this->KM_U2K_TransferMessages                                 = new  std::map<Int32, C_KM_TransferMessage*> ();

  this->KM_K2U_Ringbuffers                                      = new  std::map<Int32, C_KM_Ringspeicher*   > ();
  this->KM_K2U_Queues                                           = new  std::map<Int32, C_KM_Queue*          > ();
  this->KM_K2U_TransferMessages                                 = new  std::map<Int32, C_KM_TransferMessage*> ();
  } //    C_KM_Ads                            ()
/**************************************************************** Destruktor ****************************************************************/
                 C_KM_Ads::~C_KM_Ads                                             ()
  {
  // Streaming-Mechanismen
  delete (this->KM_K2U_TransferMessages);
  delete (this->KM_K2U_Queues);
  delete (this->KM_K2U_Ringbuffers);

  delete (this->KM_U2K_TransferMessages);
  delete (this->KM_U2K_Queues);
  delete (this->KM_U2K_Ringbuffers);

  // Commanding-Mechanismen
  delete (this->ListOfListOfCmdMessageBundles);
  }  //   ~C_KM_Ads                           ()  
/*********************************************** Nicht �ffentliche private Anwender-Methoden ************************************************/
void             C_KM_Ads::std_memcpy                                            (Byte* Dst, Byte* Src, Int32  Size)
  {
  size_t                     Size2                                   = static_cast<size_t>(Size);
  std::memcpy (Dst, Src, Size2);        // effizienter als for-Schleife
  }

void             C_KM_Ads::Transfer_U2K_TM_Datensaetze_to_U2K_RB                 (C_KM_Ringspeicher* KM_U2K_Ringbuffer, C_KM_Ringspeicher* KM_K2U_Ringbuffer, C_KM_TransferMessage* KM_U2K_TransferMessage)
  {
  // 1. Entnehme dem U2K_TranferMessageHeader den Wert von UM_K2U_RB_WritePlaces_free vom UM_K2U_Ringspeicher f�r den KM_K2U_Ringspeicher
  KM_K2U_Ringbuffer->UM_RB_WritePlaces_free                          = KM_U2K_TransferMessage->Message_Header->K2U_RB_WritePlaces_free;        // (gr�ner Transfer-Pfad laut Visio-Zeichnung RingspeicherMethoden_und_Attribute.vsdx)
  KM_U2K_Ringbuffer->UM_RB_WritePlaces_free                          = KM_U2K_TransferMessage->Message_Header->U2K_RB_WritePlaces_free;        // (roter  Transfer-Pfad laut Visio-Zeichnung RingspeicherMethoden_und_Attribute.vsdx)

  // 2. W�hle als DatensatzAnzahl die Anzahl in der U2K_TransferMessage 
  Int32                      TM_DatensatzAnzahl                      = KM_U2K_TransferMessage->Message_Header->DatensatzAnzahl;

  // 3. Entnehme U2K_TransferMessage_DatensatzAnzahl von U2K-Datens�tzen aus der U2K_Transfermessage und f�lle diese in den KM_U2K_Ringbuffer  // es ist garantiert, dass der Ringspeicher genau so viele Pl�tze f�r Datens�tze frei hat, wie die TransferMessage enth�lt
  if         (TM_DatensatzAnzahl > 0)                                                                                                         // dann transferiere genau DatensatzAnzahl U2K-Datensatze aus TransferMessage zum Ringspeicher; hier muss (KM_U2K_Ringbuffer->KM_RB_WritePlaces_free > 0), da die auf UM-Seite bereits abgetestet wurde
    {
    KM_U2K_TransferMessage->Message_Datensatz                        = KM_U2K_TransferMessage->Message_Feld;

    for (Int32  TM_Datensatz_Nr = 0; TM_Datensatz_Nr < TM_DatensatzAnzahl; TM_Datensatz_Nr++)
      {
      S_Datensatz_Header*    TM_Datensatz_Header                     = reinterpret_cast<S_Datensatz_Header*>(KM_U2K_TransferMessage->Message_Datensatz);
      Int32                  TM_Datensatz_ByteAnzahl                 = TM_Datensatz_Header->Feld_ByteAnzahl + sizeof(S_Datensatz_Header);      // Dataset_ByteAnzahl =  Datensatz_Feld_ByteAnzahl + Datensatz_Header_ByteAnzahl; (Datensatz_Header_ByteAnzahl = 2x Int32  = 8)
      KM_U2K_Ringbuffer->move_to_RB (KM_U2K_TransferMessage->Message_Datensatz, TM_Datensatz_ByteAnzahl);

      /* TransferMessage Post-Inkrement */
      KM_U2K_TransferMessage->Message_Datensatz                     += TM_Datensatz_ByteAnzahl;                                                // Erh�hen des Datensatz-Pointer um die L�nge des aktuellen Datensatz
      KM_U2K_TransferMessage->Message_Header->Feld_ByteAnzahl       -= TM_Datensatz_ByteAnzahl;                                                // Erniedrigen der L�nge des Message-Feldes um die L�nge des aktuellen Datensatz
      KM_U2K_TransferMessage->Message_Header->DatensatzAnzahl       -= 1;                                                                      // Dekrementieren der Datensatz-Anzahl
      }
    }
  else // if (TM_DatensatzAnzahl == 0)                                                                                                         // dann transferiere keine U2K-Datensatze aus TransferMessage zum Ringspeicher 
    {
    }
  }
void             C_KM_Ads::Transfer_K2U_RB_Datensaetze_to_K2U_TM                 (C_KM_TransferMessage* KM_K2U_TransferMessage, C_KM_Ringspeicher* KM_K2U_Ringbuffer, C_KM_Ringspeicher* KM_U2K_Ringbuffer)
  {
  KM_K2U_TransferMessage->Message_Header->Feld_ByteAnzahl          = 0;
  KM_K2U_TransferMessage->Message_Header->DatensatzAnzahl          = 0;
  KM_K2U_TransferMessage->Message_Datensatz                        = KM_K2U_TransferMessage->Message_Feld;

  // 1. Bef�lle den K2U_TransferMessageHeader mit dem Wert von KM_U2K_RB_WritePlaces_free vom KM_U2K_Ringspeicher f�r den UM_U2K_Ringsspeicher
  KM_K2U_TransferMessage->Message_Header->U2K_RB_WritePlaces_free  = KM_U2K_Ringbuffer->KM_RB_WritePlaces_free;  // (blauer    Transfer-Pfad laut Visio-Zeichnung RingspeicherMethoden_und_Attribute.vsdx)
  KM_K2U_TransferMessage->Message_Header->K2U_RB_WritePlaces_free  = KM_K2U_Ringbuffer->KM_RB_WritePlaces_free;  // (violetter Transfer-Pfad laut Visio-Zeichnung RingspeicherMethoden_und_Attribute.vsdx)

  // 2. W�hle als DatensatzAnzahl das Minimum aus: TransferMessage_DatensatzAnzahl_max, KM_K2U_Ringspeicher_ReadPlaces_occupied, UM_K2U_Ringspeicher_WritePlaces_free  
  Int32                      TM_DatensatzAnzahl                    = KM_K2U_TransferMessage->Message_DatensatzAnzahl_max;
  if (KM_K2U_Ringbuffer->KM_RB_ReadPlaces_occupied  < TM_DatensatzAnzahl)
    {
    TM_DatensatzAnzahl                                             = KM_K2U_Ringbuffer->KM_RB_ReadPlaces_occupied;                             // nur wenn mindestens ein Platz im KM_K2U_Ringbuffer belegt ist
    }
  if (KM_K2U_Ringbuffer->UM_RB_WritePlaces_free     < TM_DatensatzAnzahl)
    {
    TM_DatensatzAnzahl                                              = KM_K2U_Ringbuffer->UM_RB_WritePlaces_free;                               // nur wenn mindestens ein Platz im UM_K2U_Ringbuffer frei ist
    }

  // 3. Entnehme DatensatzAnzahl von K2U-Datens�tzen aus KM_K2U_Ringbuffer und f�lle diese in die K2U_TransferMessage
  if         (TM_DatensatzAnzahl  > 0)                                                                                                         // dann transferiere genau DatensatzAnzahl K2U-Datensatze aus Ringspeicher zur TransferMessage 
    {
    for (Int32  TM_Datensatz_Nr = 0; TM_Datensatz_Nr < TM_DatensatzAnzahl; TM_Datensatz_Nr++)
      {
      Int32                  RB_Datensatz_ByteAnzahl                = 0;
      KM_K2U_Ringbuffer->move_from_RB (KM_K2U_TransferMessage->Message_Datensatz, RB_Datensatz_ByteAnzahl);

      /* TransferMessage Post-Inkrement */
      KM_K2U_TransferMessage->Message_Datensatz                     += RB_Datensatz_ByteAnzahl;                                                // Erh�hen des Datensatz-Pointer um die L�nge des aktuellen Datensatz
      KM_K2U_TransferMessage->Message_Header->Feld_ByteAnzahl       += RB_Datensatz_ByteAnzahl;                                                // Erh�hen der L�nge des Message-Feldes um die L�nge des aktuellen Datensatz
      KM_K2U_TransferMessage->Message_Header->DatensatzAnzahl       += 1;                                                                      // Inkrementieren der Datensatz-Anzahl
      }
    }
  else // if (TM_DatensatzAnzahl == 0)                                                                                                            // dann transferiere keine K2U-Datensatze aus Ringspeicher zur TransferMessage 
    {
    }
  }

// Streaming-Mechanismen
void             C_KM_Ads::Transfer_U2K_TM_Datensaetze_to_U2K_QU                 (C_KM_Queue* KM_U2K_Queue, C_KM_Queue* KM_K2U_Queue, C_KM_TransferMessage* KM_U2K_TransferMessage)
  {
  // 1. Entnehme dem U2K_TranferMessageHeader den Wert von UM_K2U_QU_WritePlaces_free vom UM_K2U_Queue f�r den KM_K2U_Queue
  KM_K2U_Queue->UM_QU_WritePlaces_free                          = KM_U2K_TransferMessage->Message_Header->K2U_RB_WritePlaces_free;        // (gr�ner Transfer-Pfad laut Visio-Zeichnung QueueMethoden_und_Attribute.vsdx)
  KM_U2K_Queue->UM_QU_WritePlaces_free                          = KM_U2K_TransferMessage->Message_Header->U2K_RB_WritePlaces_free;        // (roter  Transfer-Pfad laut Visio-Zeichnung QueueMethoden_und_Attribute.vsdx)

  // 2. W�hle als DatensatzAnzahl die Anzahl in der U2K_TransferMessage 
  Int32                      TM_DatensatzAnzahl                      = KM_U2K_TransferMessage->Message_Header->DatensatzAnzahl;

  // 3. Entnehme U2K_TransferMessage_DatensatzAnzahl von U2K-Datens�tzen aus der U2K_Transfermessage und f�lle diese in den KM_U2K_Queue  // es ist garantiert, dass der Queue genau so viele Pl�tze f�r Datens�tze frei hat, wie die TransferMessage enth�lt
  if (TM_DatensatzAnzahl > 0)                                                                                                         // dann transferiere genau DatensatzAnzahl U2K-Datensatze aus TransferMessage zum Queue; hier muss (KM_U2K_Queue->KM_QU_WritePlaces_free > 0), da die auf UM-Seite bereits abgetestet wurde
    {
    KM_U2K_TransferMessage->Message_Datensatz                        = KM_U2K_TransferMessage->Message_Feld;

    for (Int32 TM_Datensatz_Nr = 0; TM_Datensatz_Nr < TM_DatensatzAnzahl; TM_Datensatz_Nr++)
      {
      S_Datensatz_Header* TM_Datensatz_Header                     = reinterpret_cast<S_Datensatz_Header*>(KM_U2K_TransferMessage->Message_Datensatz);
      Int32                  TM_Datensatz_ByteAnzahl                 = TM_Datensatz_Header->Feld_ByteAnzahl + sizeof(S_Datensatz_Header);      // Dataset_ByteAnzahl =  Datensatz_Feld_ByteAnzahl + Datensatz_Header_ByteAnzahl; (Datensatz_Header_ByteAnzahl = 2x Int32  = 8)
      KM_U2K_Queue->move_to_QU (KM_U2K_TransferMessage->Message_Datensatz, TM_Datensatz_ByteAnzahl);

      /* TransferMessage Post-Inkrement */
      KM_U2K_TransferMessage->Message_Datensatz                     += TM_Datensatz_ByteAnzahl;                                                // Erh�hen des Datensatz-Pointer um die L�nge des aktuellen Datensatz
      KM_U2K_TransferMessage->Message_Header->Feld_ByteAnzahl       -= TM_Datensatz_ByteAnzahl;                                                // Erniedrigen der L�nge des Message-Feldes um die L�nge des aktuellen Datensatz
      KM_U2K_TransferMessage->Message_Header->DatensatzAnzahl       -= 1;                                                                      // Dekrementieren der Datensatz-Anzahl
      }
    }
  else // if (TM_DatensatzAnzahl == 0)                                                                                                         // dann transferiere keine U2K-Datensatze aus TransferMessage zum Queue 
    {
    }
  }
void             C_KM_Ads::Transfer_K2U_QU_Datensaetze_to_K2U_TM                 (C_KM_TransferMessage* KM_K2U_TransferMessage, C_KM_Queue* KM_K2U_Queue, C_KM_Queue* KM_U2K_Queue)
  {
  KM_K2U_TransferMessage->Message_Header->Feld_ByteAnzahl          = 0;
  KM_K2U_TransferMessage->Message_Header->DatensatzAnzahl          = 0;
  KM_K2U_TransferMessage->Message_Datensatz                        = KM_K2U_TransferMessage->Message_Feld;

  // 1. Bef�lle den K2U_TransferMessageHeader mit dem Wert von KM_U2K_QU_WritePlaces_free vom KM_U2K_Queue f�r den UM_U2K_Ringsspeicher
  KM_K2U_TransferMessage->Message_Header->U2K_RB_WritePlaces_free  = KM_U2K_Queue->KM_QU_WritePlaces_free;  // (blauer    Transfer-Pfad laut Visio-Zeichnung QueueMethoden_und_Attribute.vsdx)
  KM_K2U_TransferMessage->Message_Header->K2U_RB_WritePlaces_free  = KM_K2U_Queue->KM_QU_WritePlaces_free;  // (violetter Transfer-Pfad laut Visio-Zeichnung QueueMethoden_und_Attribute.vsdx)

  // 2. W�hle als DatensatzAnzahl das Minimum aus: TransferMessage_DatensatzAnzahl_max, KM_K2U_Queue_ReadPlaces_occupied, UM_K2U_Queue_WritePlaces_free  
  Int32                      TM_DatensatzAnzahl                    = KM_K2U_TransferMessage->Message_DatensatzAnzahl_max;
  if (KM_K2U_Queue->KM_QU_ReadPlaces_occupied  < TM_DatensatzAnzahl)
    {
    TM_DatensatzAnzahl                                             = KM_K2U_Queue->KM_QU_ReadPlaces_occupied;                             // nur wenn mindestens ein Platz im KM_K2U_Queue belegt ist
    }
  if (KM_K2U_Queue->UM_QU_WritePlaces_free     < TM_DatensatzAnzahl)
    {
    TM_DatensatzAnzahl                                              = KM_K2U_Queue->UM_QU_WritePlaces_free;                               // nur wenn mindestens ein Platz im UM_K2U_Queue frei ist
    }

  // 3. Entnehme DatensatzAnzahl von K2U-Datens�tzen aus KM_K2U_Queue und f�lle diese in die K2U_TransferMessage
  if (TM_DatensatzAnzahl  > 0)                                                                                                         // dann transferiere genau DatensatzAnzahl K2U-Datensatze aus Queue zur TransferMessage 
    {
    for (Int32 TM_Datensatz_Nr = 0; TM_Datensatz_Nr < TM_DatensatzAnzahl; TM_Datensatz_Nr++)
      {
      Int32                  QU_Datensatz_ByteAnzahl                = 0;
      KM_K2U_Queue->move_from_QU (KM_K2U_TransferMessage->Message_Datensatz, QU_Datensatz_ByteAnzahl);

      /* TransferMessage Post-Inkrement */
      KM_K2U_TransferMessage->Message_Datensatz                     += QU_Datensatz_ByteAnzahl;                                                // Erh�hen des Datensatz-Pointer um die L�nge des aktuellen Datensatz
      KM_K2U_TransferMessage->Message_Header->Feld_ByteAnzahl       += QU_Datensatz_ByteAnzahl;                                                // Erh�hen der L�nge des Message-Feldes um die L�nge des aktuellen Datensatz
      KM_K2U_TransferMessage->Message_Header->DatensatzAnzahl       += 1;                                                                      // Inkrementieren der Datensatz-Anzahl
      }
    }
  else // if (TM_DatensatzAnzahl == 0)                                                                                                            // dann transferiere keine K2U-Datensatze aus Queue zur TransferMessage 
    {
    }
  }

/******************************************************* �ffentliche Anwender-Methoden ******************************************************/
// Streaming-Mechanismen
void             C_KM_Ads::Open_Streaming                                        (Int32 ModulConnection_ID, Int32  Ringspeicher_Kapazitaet, Int32  Message_Kapazitaet, Int32  Message_DatensatzAnzahl_max)
  {
  C_KM_TransferMessage*    KM_U2K_TransferMessage                    = new C_KM_TransferMessage (E_TransferType::U2K, Message_Kapazitaet, Message_DatensatzAnzahl_max, Ringspeicher_Kapazitaet);
  this->KM_U2K_TransferMessages->insert (std::pair<Int32, C_KM_TransferMessage*>(ModulConnection_ID, KM_U2K_TransferMessage));

  C_KM_Ringspeicher*       KM_U2K_Ringbuffer                         = new C_KM_Ringspeicher (Ringspeicher_Kapazitaet);
  this->KM_U2K_Ringbuffers->insert      (std::pair<Int32, C_KM_Ringspeicher*   >(ModulConnection_ID, KM_U2K_Ringbuffer));

  C_KM_Queue*              KM_U2K_Queue                              = new C_KM_Queue (Ringspeicher_Kapazitaet);
  this->KM_U2K_Queues->insert           (std::pair<Int32, C_KM_Queue*          >(ModulConnection_ID, KM_U2K_Queue));

  C_KM_TransferMessage*    KM_K2U_TransferMessage                    = new C_KM_TransferMessage (E_TransferType::K2U, Message_Kapazitaet, Message_DatensatzAnzahl_max, Ringspeicher_Kapazitaet);
  this->KM_K2U_TransferMessages->insert (std::pair<Int32, C_KM_TransferMessage*>(ModulConnection_ID, KM_K2U_TransferMessage));

  C_KM_Ringspeicher*       KM_K2U_Ringbuffer                         = new C_KM_Ringspeicher (Ringspeicher_Kapazitaet);
  this->KM_K2U_Ringbuffers->insert      (std::pair<Int32, C_KM_Ringspeicher*   >(ModulConnection_ID, KM_K2U_Ringbuffer));
  
  C_KM_Queue*              KM_K2U_Queue                              = new C_KM_Queue (Ringspeicher_Kapazitaet);
  this->KM_K2U_Queues->insert           (std::pair<Int32, C_KM_Queue*          >(ModulConnection_ID, KM_K2U_Queue));
  //this->KM_U2K_TransferMessages->insert (std::pair<Int32, C_KM_TransferMessage*>(ModulConnection_ID, new C_KM_TransferMessage (E_TransferType::U2K, Message_Kapazitaet, Message_DatensatzAnzahl_max, Ringspeicher_Kapazitaet)));
  //this->KM_U2K_Ringbuffers->insert      (std::pair<Int32, C_KM_Ringspeicher*>   (ModulConnection_ID, new C_KM_Ringspeicher    (Ringspeicher_Kapazitaet)));
  //this->KM_K2U_TransferMessages->insert (std::pair<Int32, C_KM_TransferMessage*>(ModulConnection_ID, new C_KM_TransferMessage (E_TransferType::K2U, Message_Kapazitaet, Message_DatensatzAnzahl_max, Ringspeicher_Kapazitaet)));
  //this->KM_K2U_Ringbuffers->insert      (std::pair<Int32, C_KM_Ringspeicher*>   (ModulConnection_ID, new C_KM_Ringspeicher    (Ringspeicher_Kapazitaet)));
  }
void             C_KM_Ads::Close_Streaming                                       (Int32 ModulConnection_ID)
  {
  delete ((*this->KM_K2U_Queues)          [ModulConnection_ID]);
  this->KM_K2U_Queues->erase              (ModulConnection_ID);

  delete ((*this->KM_K2U_Ringbuffers)     [ModulConnection_ID]);
  this->KM_K2U_Ringbuffers->erase         (ModulConnection_ID);

  delete ((*this->KM_K2U_TransferMessages)[ModulConnection_ID]);
  this->KM_K2U_TransferMessages->erase    (ModulConnection_ID);

  delete ((*this->KM_U2K_Queues)          [ModulConnection_ID]);
  this->KM_U2K_Queues->erase              (ModulConnection_ID);

  delete ((*this->KM_U2K_Ringbuffers)     [ModulConnection_ID]);
  this->KM_U2K_Ringbuffers->erase         (ModulConnection_ID);

  delete ((*this->KM_U2K_TransferMessages)[ModulConnection_ID]);
  this->KM_U2K_TransferMessages->erase    (ModulConnection_ID);
  }

void             C_KM_Ads::Transfer_KM_U2K_RB_Datensatz_to_Kernel                (Int32 ModulConnection_ID, Byte* KM_U2K_Datensatz)
  {
  C_KM_Ringspeicher*       KM_U2K_Ringbuffer                         = (*this->KM_U2K_Ringbuffers)[ModulConnection_ID];
  Int32                    RB_DatensatzAnzahl                        =  KM_U2K_Ringbuffer->KM_RB_ReadPlaces_occupied;

  if          (RB_DatensatzAnzahl >  0)                                                                                        // nur wenn im Ringspeicher ein Datensatz vorhanden dann transferiere genau 1 U2K-Datensatz aus Ringspeicher zum Kernel
    {
    Int32                  Kernel_Datensatz_Byte_Anzahl              = 0;
    KM_U2K_Ringbuffer->move_from_RB (KM_U2K_Datensatz, Kernel_Datensatz_Byte_Anzahl);
    }
  else //  if (RB_DatensatzAnzahl == 0)                                                                                                         // dann tranferiere keinen U2K-Datensatz aus Ringspeicher zum Kernel, sondern setze KM_U2K_Datensatz->Feld_ByteAnzahl = 0
    {
    S_Datensatz_Header*    KM_Datensatz_Header                       = reinterpret_cast<S_Datensatz_Header*>(KM_U2K_Datensatz);
    KM_Datensatz_Header->Feld_ByteAnzahl                             = 0;                                                                       // nur durch Feld_ByteAnzahl = 0 wird kein Datensatz zum Kernel transferiert.
    }
  }
Int32            C_KM_Ads::Get_KM_U2K_RB_ReadPlaces_occupied                     (Int32 ModulConnection_ID)
  {
  return ((*this->KM_U2K_Ringbuffers)[ModulConnection_ID]->KM_RB_ReadPlaces_occupied);
  }
Int32            C_KM_Ads::Get_KM_U2K_RB_WritePlaces_free                        (Int32 ModulConnection_ID)
  {
  return ((*this->KM_U2K_Ringbuffers)[ModulConnection_ID]->KM_RB_WritePlaces_free);                                                                       // (gr�ner    Transfer-Pfad laut Visio-Zeichnung RingspeicherMethoden_und_Attribute.vsdx) 
  }
Int32            C_KM_Ads::Get_UM_U2K_RB_WritePlaces_free                        (Int32 ModulConnection_ID)
  {
  return ((*this->KM_U2K_Ringbuffers)[ModulConnection_ID]->UM_RB_WritePlaces_free);                                                                       // (violetter Transfer-Pfad laut Visio-Zeichnung RingspeicherMethoden_und_Attribute.vsdx)
  }

void             C_KM_Ads::Transfer_Kernel_Datensatz_to_KM_K2U_RB                (Int32 ModulConnection_ID, Byte* KM_K2U_Datensatz)
  {
  C_KM_Ringspeicher*       KM_K2U_Ringbuffer                         = (*this->KM_K2U_Ringbuffers)[ModulConnection_ID];
  S_Datensatz_Header*      Kernel_Datensatz_Header                   = reinterpret_cast <S_Datensatz_Header*>(KM_K2U_Datensatz);

  Int32                    RB_DatensatzAnzahl                        = KM_K2U_Ringbuffer->KM_RB_WritePlaces_free;
  if         ((RB_DatensatzAnzahl  > 0) && (Kernel_Datensatz_Header->Feld_ByteAnzahl  > 0))                                                     // dann transferiere genau 1 K2U-Datensatz vom Kernel zum Ringspeicher; hier muss auch auf (KM_K2U_Ringbuffer->KM_RB_WritePlaces_free  > 0) abgefragt werden!!
    {
    Int32                  Kernel_Datensatz_ByteAnzahl               = Kernel_Datensatz_Header->Feld_ByteAnzahl + sizeof(S_Datensatz_Header);   // Dataset_ByteAnzahl =  Datensatz_Feld_ByteAnzahl + Datensatz_Header_ByteAnzahl; (Datensatz_Header_ByteAnzahl = 2x Int32  = 8)
    KM_K2U_Ringbuffer->move_to_RB (KM_K2U_Datensatz, Kernel_Datensatz_ByteAnzahl);
    }
  else // if ((RB_DatensatzAnzahl ==  0) || (User_Datensatz_Header->Feld_ByteAnzahl == 0))                                                      // nur Feld_ByteAnzahl = 0 �bergeben wurde, dann tranferiere keinen K2U-Datensatz vom Kernel zum Ringspeicher 
    {
    }
  }
Int32            C_KM_Ads::Get_KM_K2U_RB_WritePlaces_free                        (Int32 ModulConnection_ID)
  {
  return ((*this->KM_K2U_Ringbuffers)[ModulConnection_ID]->KM_RB_WritePlaces_free);                                                                       // (roter  Transfer-Pfad laut Visio-Zeichnung RingspeicherMethoden_und_Attribute.vsdx)
  }
Int32            C_KM_Ads::Get_UM_K2U_RB_WritePlaces_free                        (Int32 ModulConnection_ID)
  {
  return ((*this->KM_K2U_Ringbuffers)[ModulConnection_ID]->UM_RB_WritePlaces_free);                                                                       // (blauer Transfer-Pfad laut Visio-Zeichnung RingspeicherMethoden_und_Attribute.vsdx)
  }

void             C_KM_Ads::CycleUpdate_RB_Streaming                              (Int32 ModulConnection_ID)
  {     
  if ((this->KM_U2K_Ringbuffers->count(ModulConnection_ID) == 1) && (this->KM_U2K_TransferMessages->count(ModulConnection_ID) == 1) && (this->KM_K2U_Ringbuffers->count(ModulConnection_ID) == 1) && (this->KM_K2U_TransferMessages->count(ModulConnection_ID) == 1))
  //if (true)
    {
    // I. Initialisierung von Hilfsvariablen aus den Maps
    C_KM_Ringspeicher*     KM_U2K_Ringbuffer                         = (*this->KM_U2K_Ringbuffers)     [ModulConnection_ID];
    C_KM_TransferMessage*  KM_U2K_TransferMessage                    = (*this->KM_U2K_TransferMessages)[ModulConnection_ID];

    C_KM_Ringspeicher*     KM_K2U_Ringbuffer                         = (*this->KM_K2U_Ringbuffers)     [ModulConnection_ID];
    C_KM_TransferMessage*  KM_K2U_TransferMessage                    = (*this->KM_K2U_TransferMessages)[ModulConnection_ID];

    // II. (Downward) Bef�lle den U2K-Ringspeicher mit Datens�tzen aus der U2K_TransferMessage
    this->Transfer_U2K_TM_Datensaetze_to_U2K_RB (KM_U2K_Ringbuffer, KM_K2U_Ringbuffer, KM_U2K_TransferMessage);

    // III. (Upward) Bef�lle die K2U_TransferMessage mit Datens�tzen aus dem K2U-Ringspeicher  
    this->Transfer_K2U_RB_Datensaetze_to_K2U_TM (KM_K2U_TransferMessage, KM_K2U_Ringbuffer, KM_U2K_Ringbuffer);
    }
  else
    {
    }
  }

void             C_KM_Ads::Transfer_KM_U2K_QU_Datensatz_to_Kernel                (Int32 ModulConnection_ID, Byte* KM_U2K_Datensatz)
  {
  C_KM_Queue* KM_U2K_Queue                              = (*this->KM_U2K_Queues)[ModulConnection_ID];
  Int32                    QU_DatensatzAnzahl                        =  KM_U2K_Queue->KM_QU_ReadPlaces_occupied;

  if (QU_DatensatzAnzahl >  0)                                                                                                         // nur wenn im Queue ein Datensatz vorhanden dann transferiere genau 1 U2K-Datensatz aus Queue zum Kernel
    {
    Int32                  Kernel_Datensatz_Byte_Anzahl              = 0;
    KM_U2K_Queue->move_from_QU (KM_U2K_Datensatz, Kernel_Datensatz_Byte_Anzahl);
    }
  else //  if (QU_DatensatzAnzahl == 0)                                                                                                         // dann tranferiere keinen U2K-Datensatz aus Queue zum Kernel, sondern setze KM_U2K_Datensatz->Feld_ByteAnzahl = 0
    {
    S_Datensatz_Header* KM_Datensatz_Header                          = reinterpret_cast<S_Datensatz_Header*>(KM_U2K_Datensatz);
    KM_Datensatz_Header->Feld_ByteAnzahl                             = 0;                                                                       // nur durch Feld_ByteAnzahl = 0 wird kein Datensatz zum Kernel transferiert.
    }
  }
Int32            C_KM_Ads::Get_KM_U2K_QU_ReadPlaces_occupied                     (Int32 ModulConnection_ID)
  {
  return ((*this->KM_U2K_Queues)[ModulConnection_ID]->KM_QU_ReadPlaces_occupied);
  }
Int32            C_KM_Ads::Get_KM_U2K_QU_WritePlaces_free                        (Int32 ModulConnection_ID)
  {
  return ((*this->KM_U2K_Queues)[ModulConnection_ID]->KM_QU_WritePlaces_free);                                                                       // (gr�ner    Transfer-Pfad laut Visio-Zeichnung QueueMethoden_und_Attribute.vsdx) 
  }
Int32            C_KM_Ads::Get_KM_U2K_QU_Kapazitaet                              (Int32 ModulConnection_ID)
  {
  return ((*this->KM_U2K_Queues)[ModulConnection_ID]->Queue_Kapazitaet);  
  }
void             C_KM_Ads::Set_KM_U2K_QU_Kapazitaet                              (Int32 ModulConnection_ID, Int32 KM_U2K_QU_Kapazitaet)
  {
  C_KM_Queue*             KM_U2K_Queue                              = (*this->KM_U2K_Queues)[ModulConnection_ID];
  KM_U2K_Queue->KM_QU_WritePlaces_free                             -= KM_U2K_Queue->Queue_Kapazitaet;   // Subtrahieren der alten Queue-Kapazit�t
  KM_U2K_Queue->Queue_Kapazitaet                                    = KM_U2K_QU_Kapazitaet;
  KM_U2K_Queue->KM_QU_WritePlaces_free                             += KM_U2K_Queue->Queue_Kapazitaet;   // Addieren der neuen Queue-Kapazit�t; KM_QU_WritePlaces_free kann auch negativ werden falls die Queue_Kapazitaet kleiner als KM_QU_ReadPlaces_occupied gesetzt wird
  }
Int32            C_KM_Ads::Get_UM_U2K_QU_WritePlaces_free                        (Int32 ModulConnection_ID)
  {
  return ((*this->KM_U2K_Queues)[ModulConnection_ID]->UM_QU_WritePlaces_free);                                                                       // (violetter Transfer-Pfad laut Visio-Zeichnung QueueMethoden_und_Attribute.vsdx)
  }

void             C_KM_Ads::Transfer_Kernel_Datensatz_to_KM_K2U_QU                (Int32 ModulConnection_ID, Byte* KM_K2U_Datensatz)
  {
  C_KM_Queue*              KM_K2U_Queue                              = (*this->KM_K2U_Queues)[ModulConnection_ID];
  S_Datensatz_Header*      Kernel_Datensatz_Header                   = reinterpret_cast <S_Datensatz_Header*>(KM_K2U_Datensatz);

  Int32                    QU_DatensatzAnzahl                        = KM_K2U_Queue->KM_QU_WritePlaces_free;
  if ((QU_DatensatzAnzahl  > 0) && (Kernel_Datensatz_Header->Feld_ByteAnzahl  > 0))                                                     // dann transferiere genau 1 K2U-Datensatz vom Kernel zum Queue; hier muss auch auf (KM_K2U_Queue->KM_QU_WritePlaces_free  > 0) abgefragt werden!!
    {
    Int32                  Kernel_Datensatz_ByteAnzahl               = Kernel_Datensatz_Header->Feld_ByteAnzahl + sizeof(S_Datensatz_Header);   // Dataset_ByteAnzahl =  Datensatz_Feld_ByteAnzahl + Datensatz_Header_ByteAnzahl; (Datensatz_Header_ByteAnzahl = 2x Int32  = 8)
    KM_K2U_Queue->move_to_QU (KM_K2U_Datensatz, Kernel_Datensatz_ByteAnzahl);
    }
  else // if ((QU_DatensatzAnzahl ==  0) || (User_Datensatz_Header->Feld_ByteAnzahl == 0))                                                      // nur Feld_ByteAnzahl = 0 �bergeben wurde, dann tranferiere keinen K2U-Datensatz vom Kernel zum Queue 
    {
    }
  }
Int32            C_KM_Ads::Get_KM_K2U_QU_WritePlaces_free                        (Int32 ModulConnection_ID)
  {
  return ((*this->KM_K2U_Queues)[ModulConnection_ID]->KM_QU_WritePlaces_free);                                                                       // (roter  Transfer-Pfad laut Visio-Zeichnung QueueMethoden_und_Attribute.vsdx)
  }
Int32            C_KM_Ads::Get_KM_K2U_QU_Kapazitaet                              (Int32 ModulConnection_ID)
  {
  return ((*this->KM_K2U_Queues)[ModulConnection_ID]->Queue_Kapazitaet);
  }
void             C_KM_Ads::Set_KM_K2U_QU_Kapazitaet                              (Int32 ModulConnection_ID, Int32 KM_K2U_QU_Kapazitaet)
  {
  C_KM_Queue*             KM_K2U_Queue                              = (*this->KM_K2U_Queues)[ModulConnection_ID];
  KM_K2U_Queue->KM_QU_WritePlaces_free                             -= KM_K2U_Queue->Queue_Kapazitaet;   // Subtrahieren der alten Queue-Kapazit�t
  KM_K2U_Queue->Queue_Kapazitaet                                    = KM_K2U_QU_Kapazitaet;
  KM_K2U_Queue->KM_QU_WritePlaces_free                             += KM_K2U_Queue->Queue_Kapazitaet;   // Addieren der neuen Queue-Kapazit�t; KM_QU_WritePlaces_free kann auch negativ werden falls die Queue_Kapazitaet kleiner als KM_QU_ReadPlaces_occupied gesetzt wird
  }
Int32            C_KM_Ads::Get_UM_K2U_QU_WritePlaces_free                        (Int32 ModulConnection_ID)
  {
  return ((*this->KM_K2U_Queues)[ModulConnection_ID]->UM_QU_WritePlaces_free);                                                                       // (blauer Transfer-Pfad laut Visio-Zeichnung QueueMethoden_und_Attribute.vsdx)
  }

void             C_KM_Ads::CycleUpdate_QU_Streaming                              (Int32 ModulConnection_ID)
  {
  if ((this->KM_U2K_Queues->count(ModulConnection_ID) == 1) && (this->KM_U2K_TransferMessages->count(ModulConnection_ID) == 1) && (this->KM_K2U_Queues->count(ModulConnection_ID) == 1) && (this->KM_K2U_TransferMessages->count(ModulConnection_ID) == 1))
    {
    // I. Initialisierung von Hilfsvariablen aus den Maps
    C_KM_Queue*            KM_U2K_Queue                              = (*this->KM_U2K_Queues)          [ModulConnection_ID];
    C_KM_TransferMessage*  KM_U2K_TransferMessage                    = (*this->KM_U2K_TransferMessages)[ModulConnection_ID];

    C_KM_Queue*            KM_K2U_Queue                              = (*this->KM_K2U_Queues)          [ModulConnection_ID];
    C_KM_TransferMessage*  KM_K2U_TransferMessage                    = (*this->KM_K2U_TransferMessages)[ModulConnection_ID];

    // II. (Downward) Bef�lle den U2K-Queue mit Datens�tzen aus der U2K_TransferMessage
    this->Transfer_U2K_TM_Datensaetze_to_U2K_QU (KM_U2K_Queue, KM_K2U_Queue, KM_U2K_TransferMessage);

    // III. (Upward) Bef�lle die K2U_TransferMessage mit Datens�tzen aus dem K2U-Queue  
    this->Transfer_K2U_QU_Datensaetze_to_K2U_TM (KM_K2U_TransferMessage, KM_K2U_Queue, KM_U2K_Queue);
    }
  else
    {
    }
  }

// Commanding- und Streaming-Mechanismen
Boolean          C_KM_Ads::AdsReadWriteIndication                                (Int32 ModulConnection_ID, Int32 Cmd_Nr, ULONG& cbReadLength, void*& pReadData, ULONG& cbWriteLength, void*& pWriteData)
  {// ModulConnection_ID = Adresse der UM- und KM-Objekte; Cmd_Nr = Kommando-Nr innerhalb eines UM- bzw. KM-Objektes  
 
  this->KM_ModulConnection_ID                                                 = ModulConnection_ID;
  this->KM_Cmd_Nr                                                    = Cmd_Nr;

  if         (Cmd_Nr == Cmd_Streaming)
    {
    if ((this->KM_U2K_TransferMessages->count(this->KM_ModulConnection_ID) == 1) && (this->KM_K2U_TransferMessages->count(this->KM_ModulConnection_ID) == 1))
      {
      C_KM_TransferMessage* KM_U2K_TransferMessage                   = (*this->KM_U2K_TransferMessages)[ModulConnection_ID];
      std::memcpy (KM_U2K_TransferMessage->Message, pWriteData, static_cast<size_t>(cbWriteLength));                       // Umkopie von cbWriteData nach Message; memcpy ist effizienter als for-Schleife
      KM_U2K_TransferMessage->Message_Header->Feld_ByteAnzahl        = cbWriteLength - sizeof (S_Message_Header);

      //this->CycleUpdate_RB_Streaming (this->KM_ModulConnection_ID);
      this->CycleUpdate_QU_Streaming (this->KM_ModulConnection_ID);

      C_KM_TransferMessage* KM_K2U_TransferMessage                   = (*this->KM_K2U_TransferMessages)[ModulConnection_ID];
      pReadData                                                      = KM_K2U_TransferMessage->Message;                    // direkte �bergabe des Message-Zeiger an cbReadData; von Byte* nach void* muss nicht explizit umgecastet werden !!
      cbReadLength                                                   = KM_K2U_TransferMessage->Message_Header->Feld_ByteAnzahl + sizeof (S_Message_Header);
      return (true);
      }
    else
      {
      return (false);
      }
    }
  else // if (Cmd_Nr != Cmd_Streaming)
    {
    if (this->ListOfListOfCmdMessageBundles->count(ModulConnection_ID) == 1)                                                                             // wenn ModulConnection_ID als (einmaliger) Schl�ssel in der Map ListOfListOfCmdMessageBundles vorhanden
      {
      std::map<Int32, S_CmdMessageBundle*>* ListOfCmdMessageBundles  = (*this->ListOfListOfCmdMessageBundles)[ModulConnection_ID];

      if         (ListOfCmdMessageBundles->count          (Cmd_Nr)   == 1)                                                                      // wenn Cmd_Nr    als (einmaliger) Schl�ssel in der Map ListOfCmdMessageBundles vorhanden
        {
        S_CmdMessageBundle*     CmdMessageBundle                     = (*ListOfCmdMessageBundles)[Cmd_Nr];

        std::memcpy (CmdMessageBundle->U2K_ByteArray, pWriteData, static_cast<size_t>(cbWriteLength));                                          // Umkopie von cbWriteData nach Message-ByteArray; memcpy ist effizienter als for-Schleife
        CmdMessageBundle->U2K_ByteArray_Byteanzahl                   = cbWriteLength;

        pReadData                                                    = CmdMessageBundle->K2U_ByteArray;                                         // direkte �bergabe des Message-Zeiger an cbReadData; von Byte* nach void* muss nicht explizit umgecastet werden !!
        cbReadLength                                                 = CmdMessageBundle->K2U_ByteArray_Byteanzahl;
        return (true);
        }  // if (ListOfCmdMessageBundles->count          (Cmd_Nr)   == 1)
      else //       if (ListOfCmdMessageBundles->count    (Cmd_Nr)   != 1)
        {
        return (false);
        }  // else  if (ListOfCmdMessageBundles->count    (Cmd_Nr)    != 1)
      } //  if (this->ListOfListOfCmdMessageBundles->count(ModulConnection_ID) == 1)
    else // if (this->ListOfListOfCmdMessageBundles->count(ModulConnection_ID) != 1)  
      {
      return   (false);
      } // else  if (this->ListOfListOfCmdMessageBundles->count(ModulConnection_ID) != 1)
    } // else  if (Cmd_Nr !=  Cmd_Streaming)
  } //     C_KM_Ads::AdsReadWriteIndication   (Int32 ModulConnection_ID, Int32  Cmd_Nr, ULONG& cbReadLength, void*& pReadData, ULONG& cbWriteLength, void*& pWriteData)
