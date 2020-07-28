// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ---------------------------------------------------------------- // 
// C++ Header File Name: m_vtec.h
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

VTEC header utility

VTEC coding is something new to NWS products. 
It was designed for use by media and other means of communication 
to better track certain event times and expirations.

UCG Coding or Universal Geographic Code coding is used to identify 
each specific forecast zone or county the product was issued for.

MND Heading or Mass News Dissemination heading

Issuance Time: Time the Product was issued
Valid Time: Time period that the product is valid

VTEC coding:
P-VTEC will be used for most NWS watch, warning and advisory products.
P-VTEC Line: /k.aaa.cccc.pp.s.####.yymmddThhnnZB-yymmddThhnnZE/
k = Status
aaa = Action
cccc = Office ID
pp = Phenomena
s = Significance
#### = Event Tracking Number (ETN)
yymmddThhnnZB = Event Beginning Date/Time
yymmddThhnnZE = Event Ending Date/Time
yy - year hh - hour
mm - month nn - minute
dd - day Z - fixed UTC indicator
T - fixed time indicator

H-VTEC is used only for hydrology products and will follow the P-VTEC line.
H-VTEC Line /nwsli.s.ic.yymmddThhnnZB.yymmddThhnnZC.yymmddThhnnZE.fr/
nwsli = NWS is the station and LI is the State
s = Flood Severity
ic = Immediate Cause
yymmddThhnnZB = Flood Begin Date/Time
yymmddThhnnZC = Flood Crest Date/Time
yymmddThhnnZE = Flood End Date/Time
fr = Flood Record (NO, NR, UU)
*/
// ----------------------------------------------------------- // 
#ifndef __M_VTEC_HPP__
#define __M_VTEC_HPP__

#include "gxdlcode.h"

// --------------------------------------------------------------
// Data Structures
// --------------------------------------------------------------
class VTEC_time_header
{
public:
  VTEC_time_header() { Clear(); }
  ~VTEC_time_header() { }
  VTEC_time_header(const VTEC_time_header &ob) { Copy(ob); };
  VTEC_time_header &operator=(const VTEC_time_header &ob);

public:
  void Copy(const VTEC_time_header &ob);
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

class PVTEC_header
{
public:
  PVTEC_header() { Clear(); }
  ~PVTEC_header() { }
  PVTEC_header(const PVTEC_header &ob) { Copy(ob); };
  PVTEC_header &operator=(const PVTEC_header &ob);

public:
  void Copy(const PVTEC_header &ob);
  void Clear();

public:
  gxString status;
  gxString action;
  gxString office_id;
  gxString phenomena;
  gxString significance;
  gxString event_tracking_number;
  VTEC_time_header begin_time;
  VTEC_time_header end_time;
};

class HVTEC_header
{
public:
  HVTEC_header() { Clear(); }
  ~HVTEC_header() { }
  HVTEC_header(const HVTEC_header &ob) { Copy(ob); };
  HVTEC_header &operator=(const HVTEC_header &ob);

public:
  void Copy(const HVTEC_header &ob);
  void Clear();

public:
  gxString NWSLI;
  gxString flood_severity;
  gxString cause;
  VTEC_time_header begin_time;
  VTEC_time_header crest_time;
  VTEC_time_header end_time;
  gxString flood_record;
};
// --------------------------------------------------------------


// --------------------------------------------------------------
// Classes
// --------------------------------------------------------------
class VTEC 
{
public:
  VTEC() { }
  ~VTEC() { } 
  VTEC(const VTEC &ob) { Copy(ob); };
  VTEC &operator=(const VTEC &ob);

public:
  void Copy(const VTEC &ob);
  void Clear();

public: // User interface functions
  int TestVTECHeader(const char *linebuf);

  // After the TestVTECHeader we can analyze the fields
  char *Status();
  char *Action();
  char *OfficeID();
  char *Phenomena();
  char *Significance();
  char *EventTrackingNumber();
  time_t BeginTime();
  time_t EndTime();
  int IsHydro();
  char *NWSLI();
  char *FloodSeverity();
  time_t CrestTime();
  char *FloodRecord();

public: // Test and debugging fucntions
  void PrintVTECHeader();
  void PrintPVTECHeader();
  void PrintHVTECHeader();
  void PrintTimeHeader(VTEC_time_header &th);

private: // Internal processing functions
  int parse_pvtec_header();
  int parse_hvtec_header();
  int parse_vtec_header(const gxString &vs, char words[MAXWORDS][MAXWORDLENGTH], int &numwords);
  int parse_vtec_time_header(VTEC_time_header &th);

public:
  gxString pvtec_string;  // Un-edited PVTEC line found in product
  gxString hvtec_string;  // Un-edited HVTEC line found in hydro product
  PVTEC_header pvtec_hdr; // Parsed PVTEC header
  HVTEC_header hvtec_hdr; // Parsed HVTEC header from hydro product

public:
  gxString error_string; // Error string
};
// --------------------------------------------------------------

#endif // __M_VTEC_HPP__
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
 
