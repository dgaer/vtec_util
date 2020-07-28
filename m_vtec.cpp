// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- // 
// C++ Source Code File Name: m_vtec.cpp
// Compiler Used: MSVC, GCC
// Produced By: Douglas.Gaer@noaa.gov
// File Creation Date: 01/25/2008
// Date Last Modified: 05/13/2009
// ----------------------------------------------------------- // 
// ------------- Program Description and Details ------------- // 
// ----------------------------------------------------------- // 
/*
This software and documentation is distributed for the purpose of
reading and decoding NOAA/NWS VTEC products. This software can be 
freely modified and distributed, but without a warranty of any kind. 
Use for any purpose is not guaranteed. All third party libraries used 
to build this application are subject to the licensing agreement 
stated within the source code and any documentation supplied with the 
third party library.

VTEC header utility 
*/
// ----------------------------------------------------------- // 
#include "gxdlcode.h"

VTEC_time_header &VTEC_time_header::operator=(const VTEC_time_header &ob)
{
  if(&ob != this) Copy(ob);
  return *this;
}

void VTEC_time_header::Copy(const VTEC_time_header &ob)
{
  time_string = ob.time_string;
  time_zone = ob.time_zone;
  year = ob.year;
  month = ob.month;
  day = ob.day;
  hour = ob.hour;
  minute = ob.minute;
  elapsed_time = ob.elapsed_time;
}

void VTEC_time_header::Clear()
{
  time_string.Clear();
  time_zone.Clear();
  year.Clear();
  month.Clear();
  day.Clear();
  hour.Clear();
  minute.Clear();
  elapsed_time = (time_t)0;
}

PVTEC_header &PVTEC_header::operator=(const PVTEC_header &ob)
{
  if(&ob != this) Copy(ob);
  return *this;
}

void PVTEC_header::Copy(const PVTEC_header &ob)
{
  status = ob.status;
  action = ob.action;
  office_id = ob.office_id;
  phenomena = ob.phenomena;
  significance = ob.significance;
  event_tracking_number = ob.event_tracking_number;
  begin_time = ob.begin_time;
  end_time = ob.end_time;
}

void PVTEC_header::Clear()
{
  status.Clear();
  action.Clear();
  office_id.Clear();
  phenomena.Clear();
  significance.Clear();
  event_tracking_number.Clear();
  begin_time.Clear();
  end_time.Clear();
}

HVTEC_header &HVTEC_header::operator=(const HVTEC_header &ob)
{
  if(&ob != this) Copy(ob);
  return *this;
}

void HVTEC_header::Copy(const HVTEC_header &ob)
{
  NWSLI = ob.NWSLI;
  flood_severity = ob.flood_severity;
  cause = ob.cause;
  begin_time = ob.begin_time;
  crest_time = ob.crest_time;
  end_time = ob.end_time;
  flood_record = ob.flood_record;
}

void HVTEC_header::Clear()
{
  NWSLI.Clear();
  flood_severity.Clear();
  cause.Clear();
  begin_time.Clear();
  crest_time.Clear();
  end_time.Clear();
  flood_record.Clear();
}


VTEC &VTEC::operator=(const VTEC &ob)
{
  if(&ob != this) Copy(ob);
  return *this;
}

void VTEC::Copy(const VTEC &ob)
{
  pvtec_string = ob.pvtec_string;
  hvtec_string = ob.hvtec_string;
  pvtec_hdr = ob.pvtec_hdr;
  hvtec_hdr = ob.hvtec_hdr;
  error_string = ob.error_string;
}

void VTEC::Clear()
{
  pvtec_string.Clear();
  hvtec_string.Clear();
  pvtec_hdr.Clear();
  hvtec_hdr.Clear();
  error_string.Clear();
}

time_t VTEC::BeginTime()
{
  if(IsHydro()) {
    return hvtec_hdr.begin_time.elapsed_time;
  }
  return pvtec_hdr.begin_time.elapsed_time;
}

time_t VTEC::EndTime()
{
  if(IsHydro()) {
    return hvtec_hdr.end_time.elapsed_time;
  }
  return pvtec_hdr.end_time.elapsed_time;
}

int VTEC::IsHydro() 
{ 
  if(hvtec_string.is_null()) {
    return 0;
  }
  else {
    return 1;
  }

  return 0; // Assume PVTEC type
}

char *VTEC::Status() 
{ 
  return pvtec_hdr.status.c_str(); 
}

char *VTEC::Action() 
{ 
  return pvtec_hdr.action.c_str(); 
}

char *VTEC::OfficeID() 
{ 
  return pvtec_hdr.office_id.c_str(); 
}

char *VTEC::Phenomena() 
{ 
  return pvtec_hdr.phenomena.c_str(); 
}

char *VTEC::Significance() 
{ 
  return pvtec_hdr.significance.c_str(); 
}

char *VTEC::EventTrackingNumber() 
{ 
  return pvtec_hdr.event_tracking_number.c_str(); 
}

char *VTEC::NWSLI() 
{ 
  return hvtec_hdr.NWSLI.c_str(); 
}

char *VTEC::FloodSeverity() 
{ 
  return hvtec_hdr.flood_severity.c_str(); 
}

time_t VTEC::CrestTime() 
{ 
  return hvtec_hdr.crest_time.elapsed_time; 
}

char *VTEC::FloodRecord() 
{ 
  return hvtec_hdr.flood_record.c_str(); 
}

int VTEC::TestVTECHeader(const char *linebuf)
{
  if(!linebuf) return 0;
  gxString sbuf = linebuf;
  sbuf.FilterChar('\r'); sbuf.FilterChar('\n');
  sbuf.TrimLeading(' '); sbuf.TrimTrailing(' ');
  if((sbuf[0] == '/') && (sbuf[sbuf.length()-1] == '/')) {
    gxString h1 = sbuf.Left(3);
    gxString h2 = sbuf.Left(7);
    if(h1[h1.length()-1] == '.') {
      pvtec_string = sbuf; // Copy the pvtec header string
      if(parse_pvtec_header()) return 1;

    }
    if(h2[h2.length()-1] == '.') {
      hvtec_string = sbuf; // Copy the hvtec header string
      if(parse_hvtec_header()) return 1;
    } 
  }

  return 0;
}

int VTEC::parse_pvtec_header()
{
  error_string.Clear();

  char words[MAXWORDS][MAXWORDLENGTH];
  int numwords;
  gxString time_fields;

  if(!parse_vtec_header(pvtec_string, words, numwords)) return 0;

  // P-VTEC string: /k.aaa.cccc.pp.s.####.yymmddThhnnZB-yymmddThhnnZE/
  // Field numbers:  0  1   2   3  4  5                6
  if(*words[0] != 0) {
    pvtec_hdr.status = words[0];
  }
  else {
    error_string << clear << "PVTEC parsing error: " << pvtec_string << " missing k status";
    return 0;
  }
  if(*words[1] != 0) {
    pvtec_hdr.action = words[1];
  }
  else {
    error_string << clear << "PVTEC parsing error: " << pvtec_string << " missing aaa action";
    return 0;
  }
  if(*words[2] != 0) {
    pvtec_hdr.office_id = words[2];
  }
  else {
    error_string << clear << "PVTEC parsing error: " << pvtec_string << " missing cccc office ID";
    return 0;
  }
  if(*words[3] != 0) {
    pvtec_hdr.phenomena = words[3];
  }
  else {
    error_string << clear << "PVTEC parsing error: " << pvtec_string << " missing pp phenomena";
    return 0;
  }
  if(*words[4] != 0) {
    pvtec_hdr.significance = words[4];
  }
  else {
    error_string << clear << "PVTEC parsing error: " << pvtec_string << " missing s significance";
    return 0;
  }
  if(*words[5] != 0) {
    pvtec_hdr.event_tracking_number = words[5];
  }
  else {
    error_string << clear << "PVTEC parsing error: " << pvtec_string << " missing #### event tracking number";
    return 0;
  }
  if(*words[6] != 0) {
    time_fields = words[6];
  }
  else {
    error_string << clear << "PVTEC parsing error: " << pvtec_string << " missing time fields";
    return 0;
  }

  pvtec_hdr.begin_time.time_string  = time_fields;
  pvtec_hdr.begin_time.time_string.DeleteAfterIncluding("-");
  if(!parse_vtec_time_header(pvtec_hdr.begin_time)) {
    error_string << clear << "PVTEC parsing error: " << pvtec_hdr.begin_time.time_string.c_str() << " bad begin time";
    return 0;
  }

  pvtec_hdr.end_time.time_string = time_fields;
  pvtec_hdr.end_time.time_string.DeleteBeforeIncluding("-");
  if(!parse_vtec_time_header(pvtec_hdr.end_time)) {
    error_string << clear << "PVTEC parsing error: " << pvtec_hdr.end_time.time_string.c_str() << " bad end time";
    return 0;
  }

  return 1;
}


int VTEC::parse_vtec_time_header(VTEC_time_header &th)
{
  if(th.time_string.is_null()) return 0;

  char timezone = th.time_string[th.time_string.length()-1]; 
  th.time_zone << clear << timezone;
  gxString yymmdd = th.time_string.Left(6);
  gxString hhnn = th.time_string.Right(5);
  hhnn.FilterChar(timezone); // Get rid of the timezone character

  if(yymmdd.length() != 6) return 0;
  if(hhnn.length() != 4) return 0;

  th.year = th.month = th.day = yymmdd;
  th.year.DeleteAt(2, 4);
  th.month.DeleteAt(0, 2);
  th.month.DeleteAt(2, 2);
  th.day.DeleteAt(0, 4);
  th.hour = th.minute = hhnn;
  th.hour.DeleteAt(2, 2);
  th.minute.DeleteAt(0, 2);

  // Check for products with 000000T0000Z times 
  // Continuation products will always be set to 000000T0000Z
  // NHC/TPC products may have 000000T0000Z start and/or expire times 
  if((th.year.Atoi() == 0) && (th.month.Atoi() == 0) &&
     (th.day.Atoi() == 0) && (th.hour.Atoi() == 0) &&
     (th.minute.Atoi() == 0)) {
    th.elapsed_time = 0;
    return 1;
  }

  struct tm my_tm;
  memset(&my_tm, 0, sizeof(my_tm));
  my_tm.tm_isdst = 0;
  my_tm.tm_wday = -1;

  my_tm.tm_mon = th.month.Atoi();
  my_tm.tm_mon--; // Month index starts at 00 to 11
  my_tm.tm_mday = th.day.Atoi();
  my_tm.tm_year = th.year.Atoi() + 2000;
  my_tm.tm_year -= 1900;
  my_tm.tm_hour = th.hour.Atoi();
  my_tm.tm_min = th.minute.Atoi();
  my_tm.tm_sec = 0;

  th.elapsed_time = mktime(&my_tm);

  // We need to offset the mktime result based 
  // on the local time zone setting.
  time_t system_gmt_offset;
  if(GetSystemTimeOffset(system_gmt_offset)) {
    th.elapsed_time -= system_gmt_offset;
  }
  else {
    if(debug) NT_print("Error reading your system timezone offset for VTEC time");
  }

  return 1;
}

int VTEC::parse_hvtec_header()
{
  error_string.Clear();

  char words[MAXWORDS][MAXWORDLENGTH];
  int numwords;
  
  if(!parse_vtec_header(hvtec_string, words, numwords)) return 0;

  // Process the header here
  // H-VTEC string: /nwsli.s.ic.yymmddThhnnZB.yymmddThhnnZC.yymmddThhnnZE.fr/
  // Field numbers:    0   1  2     3            4             5          6
    if(*words[0] != 0) {
    hvtec_hdr.NWSLI = words[0];
  }
  else {
    error_string << clear << "HVTEC parsing error: " << hvtec_string << " missing NWSLI";
    return 0;
  }
  if(*words[1] != 0) {
    hvtec_hdr.flood_severity = words[1];
  }
  else {
    error_string << clear << "HVTEC parsing error: " << hvtec_string << " missing flood severity";
    return 0;
  }
  if(*words[2] != 0) {
    hvtec_hdr.cause = words[2];
  }
  else {
    error_string << clear << "HVTEC parsing error: " << hvtec_string << " missing cause";
    return 0;
  }
  if(*words[3] != 0) {
    hvtec_hdr.begin_time.time_string = words[3];
  }
  else {
    error_string << clear << "HVTEC parsing error: " << hvtec_string << " missing begin time";
    return 0;
  }
  if(*words[4] != 0) {
    hvtec_hdr.crest_time.time_string = words[4];
  }
  else {
    error_string << clear << "HVTEC parsing error: " << hvtec_string << " missing crest time";
    return 0;
  }

  if(*words[5] != 0) {
    hvtec_hdr.end_time.time_string = words[5];
  }
  else {
    error_string << clear << "HVTEC parsing error: " << hvtec_string << " missing end time";
    return 0;
  }
  if(*words[6] != 0) {
    hvtec_hdr.flood_record = words[6];
  }
  else {
    error_string << clear << "HVTEC parsing error: " << hvtec_string << " missing flood record";
    return 0;
  }

  if(!parse_vtec_time_header(hvtec_hdr.begin_time)) {
    error_string << clear << "HVTEC parsing error: " << hvtec_hdr.begin_time.time_string.c_str() 
		 << " bad begin time";
    return 0;
  }
  if(!parse_vtec_time_header(hvtec_hdr.crest_time)) {
    error_string << clear << "HVTEC parsing error: " << hvtec_hdr.crest_time.time_string.c_str() 
		 << " bad crest time";
    return 0;
  }
  if(!parse_vtec_time_header(hvtec_hdr.end_time)) {
    error_string << clear << "HVTEC parsing error: " << hvtec_hdr.end_time.time_string.c_str() 
		 << " bad end time";
    return 0;
  }

  return 1;
}

int VTEC::parse_vtec_header(const gxString &vs, char words[MAXWORDS][MAXWORDLENGTH], int &numwords)
{
  error_string.Clear();

  if(vs.is_null())  {
    error_string << clear << "VTEC parsing error: VTEC string is null";
    return 0;
  }
  
  // P-VTEC string: /k.aaa.cccc.pp.s.####.yymmddThhnnZB-yymmddThhnnZE/
  // H-VTEC string: /nwsli.s.ic.yymmddThhnnZB.yymmddThhnnZC.yymmddThhnnZE.fr/
  gxString sbuf = vs;
  sbuf.TrimLeading('/'); sbuf.TrimTrailing('/');
  numwords = 0;
  if(parse(sbuf.c_str(), words, &numwords, '.') == 1) {
    error_string << clear << "VTEC parsing error: " << sbuf;
    return 0;
  }
  if(numwords < 5) {
    error_string << clear << "VTEC parsing error: " << sbuf << " missing fields";
    return 0;
  }

  return 1;
}

void VTEC::PrintPVTECHeader()
{
  GXSTD::cout << "P-VTEC string: " << pvtec_string.c_str() << "\n" << flush;
  if(debug) {
    GXSTD::cout << "P-VTEC parsed fields:" << "\n" << flush;
  }
  GXSTD::cout << "(k) status: " << pvtec_hdr.status.c_str() << "\n" << flush;
  GXSTD::cout << "(aaa) action: " << pvtec_hdr.action.c_str() << "\n" << flush;
  GXSTD::cout << "(cccc) office ID: " << pvtec_hdr.office_id.c_str() << "\n" << flush;
  GXSTD::cout << "(pp) phenomena: " << pvtec_hdr.phenomena.c_str() << "\n" << flush;
  GXSTD::cout << "(s) significance: " << pvtec_hdr.significance.c_str() << "\n" << flush;
  GXSTD::cout << "(####) event tracking number: " << pvtec_hdr.event_tracking_number.c_str() << "\n" << flush;

  if(pvtec_hdr.begin_time.elapsed_time == 0) {
    GXSTD::cout << "begin time: NONE" << "\n" << flush;
  }
  else {
    GXSTD::cout << "begin time:" << "\n" << flush;
    PrintTimeHeader(pvtec_hdr.begin_time);
  }

  if(pvtec_hdr.end_time.elapsed_time == 0) {
    GXSTD::cout << "end time: NONE" << "\n" << flush;
  }
  else {
    GXSTD::cout << "end time:" << "\n" << flush;
    PrintTimeHeader(pvtec_hdr.end_time);
  }
}

void VTEC::PrintTimeHeader(VTEC_time_header &th)
{
  if(th.elapsed_time == 0) return;

  if(debug) {
    GXSTD::cout << "Time string: "<< th.time_string.c_str() << "\n" << flush;
    GXSTD::cout << "Timezone: "<< th.time_zone.c_str() << "\n" << flush;
    GXSTD::cout << "Year: "<< th.year.c_str() << "\n" << flush;
    GXSTD::cout << "Month: "<< th.month.c_str() << "\n" << flush;
    GXSTD::cout << "Day: "<< th.day.c_str() << "\n" << flush;
    GXSTD::cout << "Hour: "<< th.hour.c_str() << "\n" << flush;
    GXSTD::cout << "Minute: "<< th.minute.c_str() << "\n" << flush;
    GXSTD::cout << "Elasped time: " << th.elapsed_time  << "\n" << flush;
  }

  if(th.elapsed_time > 1) {
    SysTime systime;
    gxString timetest = systime.MakeGMTDateTime(th.elapsed_time);
    GXSTD::cout << "Time string: " << timetest.c_str()  << "\n" << flush;
  }
}

void VTEC::PrintHVTECHeader()
{
  GXSTD::cout << "H-VTEC string: " << hvtec_string.c_str() << "\n" << flush;
  if(debug) {
    GXSTD::cout << "H-VTEC parsed fields:" << "\n" << flush;
  }
  GXSTD::cout << "NWSLI: " << hvtec_hdr.NWSLI.c_str() << "\n" << flush;
  GXSTD::cout << "(s) flood severity: " << hvtec_hdr.flood_severity.c_str() << "\n" << flush;
  GXSTD::cout << "(ic) cause: " << hvtec_hdr.cause.c_str() << "\n" << flush;
  GXSTD::cout << "(fr) flood record: " << hvtec_hdr.flood_record.c_str() << "\n" << flush;

  if(hvtec_hdr.begin_time.elapsed_time == 0) {
    GXSTD::cout << "begin time: NONE" << "\n" << flush;
  }
  else {
    GXSTD::cout << "begin time:" << "\n" << flush;
    PrintTimeHeader(hvtec_hdr.begin_time);
  }
  if(hvtec_hdr.crest_time.elapsed_time == 0) {
    GXSTD::cout << "crest time: NONE" << "\n" << flush;
  }
  else {
    GXSTD::cout << "crest time:" << "\n" << flush;
    PrintTimeHeader(hvtec_hdr.crest_time);
  }
  if(hvtec_hdr.end_time.elapsed_time == 0) {
    GXSTD::cout << "end time: NONE" << "\n" << flush;
  }
  else {
    GXSTD::cout << "end time:" << "\n" << flush;
    PrintTimeHeader(hvtec_hdr.end_time);
  }
}

void VTEC::PrintVTECHeader()
{
  if(!pvtec_string.is_null()) PrintPVTECHeader(); 
  if(!hvtec_string.is_null()) PrintHVTECHeader(); 
}
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
