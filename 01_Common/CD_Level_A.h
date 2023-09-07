/********************************************************************************************************************************************/
/*                                                                                                                                          */
/*  Header-Datei: CD_Level_A.h                                                                                                              */
/*                                                                                                                                          */
/*  Prof. Dr. Lothar Zunker      09.06.2021                                                                                                 */
/*                                                                                                                                          */
/********************************************************************************************************************************************/
#pragma once

enum class E_Level_A_CMD : Int32
  {
  NO_COMMAND                          = 0x00000000,
  KM_Kommando_00                      = 0x00000001,
  KM_Kommando_01                      = 0x00000002,
  KM_Kommando_02                      = 0x00000003,
  KM_Kommando_03                      = 0x00000004,
  Kommando_Anzahl                     = 0x00000005
  };

struct S_U2K_RequestMessage_Level_A_00
  {
  };
struct S_K2U_ResponeMessage_Level_A_00
  {
  };
struct S_U2K_RequestMessage_Level_A_01
  {
  };
struct S_K2U_ResponeMessage_Level_A_01
  {
  };
struct S_U2K_RequestMessage_Level_A_02
  {
  };
struct S_K2U_ResponeMessage_Level_A_02
  {
  };
struct S_U2K_RequestMessage_Level_A_03
  {
  };
struct S_K2U_ResponeMessage_Level_A_03
  {
  };
