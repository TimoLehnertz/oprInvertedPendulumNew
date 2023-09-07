/********************************************************************************************************************************************/
/*                                                                                                                                          */
/*  Header-Datei: CD_GlobalObjects.h                                                                                                        */
/*                                                                                                                                          */
/*  Prof. Dr. Lothar Zunker      09.06.2021                                                                                                 */
/*                                                                                                                                          */
/********************************************************************************************************************************************/
#pragma once
#pragma once

#define       MyNetID           "127.0.0.1.1.1"
//#define       MyNetID         "192.168.100.21.1.1"
//#define       MyNetID         "10.102.0.123.1.1"

const  Int32   MyAdsPort      = 0x3030;
const  Double  Takt_Zeit      = 0.001;    // = 1 ms
const  Int32   Level_B_Anzahl = 3;
const  Int32   Level_C_Anzahl = 3;



/********************* ModulConnection_ID = IndexGroup *************************/
enum class E_ModulConnection_ID : Int32
  {
  NoConnection                = 0x00000000,
  Level_A                     = 0x00000001,
  Level_B0                    = 0x00000002,
  Level_B1                    = 0x00000003,
  Level_B2                    = 0x00000004,
  Level_C0                    = 0x00000005,
  Level_C1                    = 0x00000006,
  Level_C2                    = 0x00000007,
  ID_Anzahl                   = 0x00000008
  };

/********************* Co-Slave-Adressen von IO-Klemmen mit CoE-Verzeichnis *************************/
enum class Enum_CoE_EtherCAT_Klemmen_ADR : UInt32
  {
  CoE_Adr_Klemme_EL5151       = 1006,
  CoE_Adr_Klemme_EL6002_0     = 1007,
  CoE_Adr_Klemme_EL6002_1     = 1007,
  CoE_Adr_Klemme_EL7201       = 1008,
  };

