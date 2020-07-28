// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ---------------------------------------------------------------- // 
// C++ Header File Name: m_ugc.h
// C++ Compiler Used: MSVC, GCC
// Produced By: Douglas.Gaer@noaa.gov
// File Creation Date: 01/25/2008
// Date Last Modified: 06/01/2009
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

UGC header utility
UNIVERSAL GEOGRAPHIC CODE (UGC)

UCG Coding or Universal Geographic Code coding is used to identify 
each specific forecast zone or county the product was issued for.
*/
// ----------------------------------------------------------- // 
#ifndef __M_UGC_HPP__
#define __M_UGC_HPP__

#include "gxdlcode.h"

// --------------------------------------------------------------
// Data Structures
// --------------------------------------------------------------
class UGC_time_header
{
public:
  UGC_time_header() { Clear(); }
  ~UGC_time_header() { }
  UGC_time_header(const UGC_time_header &ob) { Copy(ob); };
  UGC_time_header &operator=(const UGC_time_header &ob);

public:
  void Copy(const UGC_time_header &ob);
  void Clear();

public:
  gxString time_string; // Un-edited time string

  // Parsed time values
  gxString time_zone;
  gxString year;
  gxString month;
  gxString day;
  gxString hour;
  gxString minute;

  // Number of elapsed seconds since Jan 1, 1970 00:00:00 GMT
  // This field is used for time calculations
  time_t elapsed_time;
};

class UGC_header
{
public:
  UGC_header() { Clear(); }
  ~UGC_header() { }
  UGC_header(const UGC_header &ob) { Copy(ob); }
  UGC_header &operator=(const UGC_header &ob);

public:
  void Clear();
  void Copy(const UGC_header &ob);
  int InsertList(gxString &code);

public:
  int is_expired; // True if the product has expired
  gxList<gxString> zone_list;   // Zone numbers and FIPS county codes
  UGC_time_header ugc_time;     // UGC time to expire product
};
// --------------------------------------------------------------


// --------------------------------------------------------------
// Classes
// --------------------------------------------------------------
class UGC
{
public:
  UGC() { has_next_line = 0; }
  ~UGC() { }
  UGC(const UGC &ob) { Copy(ob); }
  UGC &operator=(const UGC &ob);

public: // User interface functions
  int TestUGCHeader(const char *linebuf, int &continue_next_line);
  int ResetUGCTime(TextProduct_time_header &new_exp_time);
  int ResetIsExpired(VTEC &vtec);
  void Copy(const UGC &ob);
  void Clear();
  friend int operator==(const UGC &a, const UGC &b);

public: // Test and debugging functions 
  void PrintUGCHeader();
  void PrintTimeHeader(UGC_time_header &th);

private: // Internal processing functions
  int parse_ugc_header();
  int parse_ugc_time_header();
  int CalcTimeOffsetPast(int system_day_n, int system_month_n, 
			 int system_year_n, char *system_year);
  int CalcTimeOffsetFuture(int system_day_n, int system_month_n, 
			   int system_year_n, char *system_year);
  int set_ugc_time(int system_day_n, int system_month_n, 
		   int system_year_n, char *system_day,
		   char *system_month, char *system_year,
		   time_t base_time);

public:
  gxString ugc_string; // Un-edited UGC line found in product
  UGC_header ugc_hdr;
  gxString error_string;

  // Support for multiple UGC blocks in a product
  gxList<VTEC> vtec_list; // VTEC headers associated with header
  gxList<gxString> zones_names; // Zone and county names found under UGC header
  gxList<gxString> include_city_names; // Area included after zone names
  TextProduct_time_header ugc_segment_time; // Issue or segment time if known
  
  gxString ugc_segment_header;
  gxList<gxString> ugc_segment_text;
  
  PolygonWarning polygon_warning;
  
private:
  int has_next_line;
};
// --------------------------------------------------------------

#endif // __M_UGC_HPP__
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
 
