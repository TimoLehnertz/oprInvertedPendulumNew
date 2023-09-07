/********************************************************************************************************************************************/
/*                                                                                                                                          */
/*  Header-Datei: CommonTypes.h                                                                                                             */
/*                                                                                                                                          */
/*  Prof. Dr. Lothar Zunker      25.03.2021                                                                                                 */
/*                                                                                                                                          */
/********************************************************************************************************************************************/
#pragma once
// Hinweis: auf 64 Bit Maschinen ist; (int == long) und (unsigned int == unsigned long), was einem 32-Bit-Integer-Typ entspricht !!!

#ifndef  Single
  typedef  float                Single;
  typedef  double               Double;
  typedef  unsigned char        Byte;
  typedef  bool                 Boolean;
  typedef  short                Int16;
  typedef  unsigned short       UInt16;
  typedef  long                 Int32;   // identisch: int     
  typedef  unsigned long        UInt32;  // identisch: unsigned int
  typedef  long long            Int64;
  typedef  unsigned long long   UInt64;
#endif  

#define PROPERTY(t,n)           __declspec(property (put = property__set_##n, get = property__get_##n ) ) t n;  typedef t property__tmp_type_##n
#define READONLY_PROPERTY(t,n)  __declspec(property (get = property__get_##n) )                           t n;  typedef t property__tmp_type_##n
#define WRITEONLY_PROPERTY(t,n) __declspec(property (put = property__set_##n) )                           t n;  typedef t property__tmp_type_##n

#define GET(n) property__tmp_type_##n property__get_##n()
#define SET(n) void property__set_##n(const property__tmp_type_##n& value)


