// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ---------------------------------------------------------------- // 
// C++ Header File Name: m_time.h
// C++ Compiler Used: MSVC, GCC
// Produced By: Douglas.Gaer@noaa.gov
// File Creation Date: 01/25/2008
// Date Last Modified: 05/13/2009
// ---------------------------------------------------------------- // 
// ------------- Include File Description and Details ------------- // 
// ---------------------------------------------------------------- // 
/*
This software and documentation is distributed for the purpose of
reading and decoding NOAA/NWS VTEC products. This software can be 
freely modified and distributed, but without a warranty of any kind. 
Use for any purpose is not guaranteed. All third party libraries used 
to build this application are subject to the licensing agreement 
stated within the source code and any documentation supplied with the 
third party library.

Time classes and functions
*/
// ----------------------------------------------------------- // 
#ifndef __M_TIME_HPP__
#define __M_TIME_HPP__

#include "gxdlcode.h"

// --------------------------------------------------------------
// Data Structures
// --------------------------------------------------------------
class TextProduct_time_header
{
public:
  TextProduct_time_header() { Clear(); }
  ~TextProduct_time_header() { }
  TextProduct_time_header(const TextProduct_time_header &ob) { Copy(ob); };
  TextProduct_time_header &operator=(const TextProduct_time_header &ob);

public:
  void Copy(const TextProduct_time_header &ob);
  void Clear();

public:
  gxString time_string; // Un-edited time string

  // Parsed time values
  gxString time_zone; // xxx Timezone
  gxString year;      // YYYY 
  gxString month;     // MM 
  gxString day;       // DD
  gxString hour;      // HH
  gxString minute;    // MM
  gxString AMPM;      // AM or PM
  gxString wkday;     // Weekday name

  // Number of elapsed seconds since Jan 1, 1970 00:00:00 GMT
  // This field is used for time calculations
  time_t elapsed_time;

  // True if there is more than 1 zone in the time_string
  int span_multiple_zones;

  // Timezone offset prased from Timezone_table
  time_t timezone_offset; // Hours to offset back to local time
  time_t gmt_offset; // Number of seconds used to offset GMT time to localtime
  time_t localtime;  // Elapsed seconds use to calculate the local time  
};
// --------------------------------------------------------------

// Standalone Time utiliy functions
int ParseTextProductTime(TextProduct_time_header &th);
int ParseTextProductTime(TextProduct_time_header &th, gxString &error_string);
int ParseTextProductTime(const char *time_string, TextProduct_time_header &th);
int ParseTextProductTime(const char *time_string, TextProduct_time_header &th, 
			 gxString &error_string);
int GetLocalTimeString(const char *TZ, time_t gmttime, time_t offset, 
		       gxString &time_string, int short_format = 0);
int HasExpiredTimeString(const char *TZ, time_t product_time, time_t gmt_offset, 
			 gxString &time_string, int has_expired);
int MakeWarningTableTimeString(gxString &time_string);
int GetIconLinkLocalTimeString(const char *TZ, time_t gmttime, time_t gmt_offset, 
			       gxString &time_string, int short_format);
int GetSystemTimeOffset(time_t &gmt_offset, time_t &systime_gmt, time_t &systime_local,
			int &num_offset_hours, gxString &TZ);
int GetSystemTime(time_t &systime_gmt, time_t &systime_local);
int GetSystemTimeOffset(time_t &gmt_offset);
int GetSystemTimeOffset(int &num_offset_hours, gxString &TZ);

// Test functions
char *EchoTimeString(time_t elapsed_seconds, gxString &time_string);

#endif // __M_TIME_HPP__
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
 
