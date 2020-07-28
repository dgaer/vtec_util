// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- // 
// C++ Source Code File Name: m_ugc.cpp
// Compiler Used: MSVC, GCC
// Produced By: Douglas.Gaer@noaa.gov
// File Creation Date: 01/25/2008
// Date Last Modified: 06/23/2009
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

UGC header utility 
UNIVERSAL GEOGRAPHIC CODE (UGC)
*/
// ----------------------------------------------------------- // 
#include "gxdlcode.h"

UGC_time_header &UGC_time_header::operator=(const UGC_time_header &ob)
{
  if(&ob != this) Copy(ob);
  return *this;
}

void UGC_time_header::Copy(const UGC_time_header &ob)
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

void UGC_time_header::Clear()
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

UGC_header &UGC_header::operator=(const UGC_header &ob)
{
  if(&ob != this) Copy(ob);
  return *this;
}

void UGC_header::Clear()
{
  is_expired = 0;
  zone_list.ClearList();
  ugc_time.Clear();
}

void UGC_header::Copy(const UGC_header &ob)
{
  Clear();
  is_expired = ob.is_expired;
  ugc_time = ob.ugc_time;

  gxListNode<gxString> *ptr;
  ptr = ob.zone_list.GetHead();
  while(ptr) {
    zone_list.Add(ptr->data);
    ptr = ptr->next;
  }
}

int UGC_header::InsertList(gxString &code) 
{
  if(code.length() != 6) return 0;
  if((code[2] == 'Z') || (code[2] == 'C')) {
    zone_list.Add(code);

    if(use_dynamic_zone_table) {
      // We do not have any zone names so build the dynamic list with numbers
      if(!FindZoneNumber(code)) {
	ZoneTableData zone_table_data;
	zone_table_data.zone_number = code; 
	zone_table_data.zone_name = code;
	zone_table_data.area_number = "0";
	zone_table_data.area_name << clear 
				  << WarningTableCWA 
				  << " Zones and Counties"; 
	Zone_table.Add(zone_table_data);
      }
    }

    return 1;
  }
  return 0;
}

UGC &UGC::operator=(const UGC &ob)
{
  if(&ob != this) Copy(ob);
  return *this;
}

void UGC::Copy(const UGC &ob)
{
  Clear();
  ugc_string = ob.ugc_string;
  ugc_hdr = ob.ugc_hdr;
  error_string = ob.error_string;
  has_next_line = has_next_line;
  ugc_segment_time = ob.ugc_segment_time;
  polygon_warning = ob.polygon_warning;

  gxListNode<gxString> *sptr;
  sptr = ob.zones_names.GetHead();
  while(sptr) {
    zones_names.Add(sptr->data);
    sptr = sptr->next;
  }
  sptr = ob.include_city_names.GetHead();
  while(sptr) {
    include_city_names.Add(sptr->data);
    sptr = sptr->next;
  }
  
  gxListNode<VTEC> *vptr = ob.vtec_list.GetHead();
  while(vptr) {
    vtec_list.Add(vptr->data);
    vptr = vptr->next;
  }

  ugc_segment_header = ob.ugc_segment_header; 

  sptr = ob.ugc_segment_text.GetHead();
  while(sptr) {
    ugc_segment_text.Add(sptr->data);
    sptr = sptr->next;
  }
}

void UGC::Clear()
{
  ugc_string.Clear();
  ugc_hdr.Clear();
  error_string.Clear();
  has_next_line = 0;
  vtec_list.ClearList();
  zones_names.ClearList();
  include_city_names.ClearList();
  ugc_segment_time.Clear();
  ugc_segment_header.Clear();
  ugc_segment_text.ClearList();
  polygon_warning.Clear();
}

int UGC::TestUGCHeader(const char *linebuf, int &continue_next_line)
{
  if(!linebuf) {
    continue_next_line = has_next_line = 0;
    return 0;
  }

  // Make a copy of the line buffer
  gxString lbuf = linebuf;
  lbuf.TrimLeadingSpaces(); lbuf.TrimTrailingSpaces();
  lbuf.FilterChar('\n');
  lbuf.FilterChar('\r');

  // Smallest possible UGC line
  gxString min_ugc_line = "NNNXXX-DDHHMM-";

  if(!has_next_line) {
    ugc_string.Clear();
  }
  else {
    ugc_string << lbuf;
    if(ugc_string[ugc_string.length()-1] != '-') {
      continue_next_line = has_next_line = 0;
      return 0;
    }
    gxString DDHHMM = ugc_string.Right(7);
    DDHHMM.TrimTrailing('-');
    gxString DD = DDHHMM.Left(2);
    gxString HH = DDHHMM.Mid(2, 2);
    gxString MM = DDHHMM.Right(2);
    if((DD.Atoi() < 1) || (DD.Atoi() > 31) || 
       (HH.Atoi() < 0) || (HH.Atoi() > 60) ||  
       (MM.Atoi() < 0) || (MM.Atoi() > 60)) {
      // Keep continuing until we find all the UGC lines
      continue_next_line = has_next_line = 1;
      return 1;
    }
    // Found the entire sting spanning multiple lines
    continue_next_line = has_next_line = 0;
    return parse_ugc_header();
  }

  continue_next_line = has_next_line = 0;
  if(lbuf.length() < min_ugc_line.length())  {
    // This line is to small to be a UGC line
    return 0;
  }
  // NNNXXX-DDHHMM- and NNNXXX>XXX-DDHHMM- header types
  if(((lbuf[6] == '-') || (lbuf[6] == '>')) && 
     (lbuf[lbuf.length()-1] == '-')) { 
    if((lbuf[2] == 'Z') || (lbuf[2] == 'C')) { 
      // We found a valid UGC header and must check for the end of UGC line
      gxString DDHHMM = lbuf.Right(7);

      // Appears to have a date string so validate the string
      if(DDHHMM[DDHHMM.length()-1] != '-') return 0;
      DDHHMM.TrimTrailing('-'); 

      // Check for a valid date string
      if((DDHHMM.Find("-") != -1) || (DDHHMM.Find(">") != -1)) {
	// The UGC header continues on the next line
	continue_next_line = has_next_line = 1;
	ugc_string << lbuf;
	return 1;
      }
      gxString DD = DDHHMM.Left(2);
      gxString HH = DDHHMM.Mid(2, 2);
      gxString MM = DDHHMM.Right(2);
      if((DD.Atoi() < 1) || (DD.Atoi() > 31) || 
	 (HH.Atoi() < 0) || (HH.Atoi() > 60) ||  
	 (MM.Atoi() < 0) || (MM.Atoi() > 60)) {
	// The UGC header continues on the next line
	continue_next_line = has_next_line = 1;
	ugc_string << lbuf;
	return 1;
      }

      // We have the entire UGC line so parse it and return
      continue_next_line = has_next_line = 0;
      ugc_string << lbuf;
      return parse_ugc_header();
    }
  }
  
  return 0;
}

int UGC::parse_ugc_header()
{
  error_string.Clear();

  // SSZNNN-DDHHMM- 
  // SS is the Two letter state or Great Lakes I.D.
  // C indicates that the number NNN that follows 
  // represents a county or independent city, or
  // Z The NNN that follows represents a unique NWS zone (land or water) 
  // or part or all of a state (or Great Lake).
  // NNN (After "C"), the number of the county/city(1), or
  // (After "Z"), the number of the zone or part or all of a state(2)
  // DD is the purge Date (UTC)
  // HH Is the purge hour (UTC)
  // MM is the purge Minutes (UTC) 

  // Example 1: FLZ063-066-012230-
  // Example 2: AMZ630-650-651-670-671-FLZ063-066>075-168-172>174-GMZ656-657-676
  //            -020245-
  // Example 3: AMZ630-FLZ063-066>075-168-172>174-042230-
  // Example 4: FLZ063>066-042230-
  // Example 5: ALZ065>069-FLZ007>019-026>029-034-GAZ120>131-142>148-155>161-
  //            GMZ730-750-755-765-770-775-071330-

  gxString state = ugc_string.Left(2);
  int offset = 0;
  int offset2 = 0;

  // Collect all the zones and county codes
  while(offset != -1) {
    offset = ugc_string.Find("-", offset);
    if(offset == -1) break;
    gxString segment = ugc_string.Mid(offset2, (offset - offset2));
    segment.TrimLeading('-'); segment.TrimTrailing('-');
    // Set the State and (C)ounty or (Z)one code
    if(((segment.length() == 6) || (segment.length() == 10)) && 
       ((segment[2] == 'C') || (segment[2] == 'Z'))) {
      state << clear <<  segment[0] << segment[1] << segment[2];
    }

    // Record the FIPS code or Zone code
    if((segment.length() == 6) && 
       ((segment[2] == 'C') || (segment[2] == 'Z'))) {
      ugc_hdr.InsertList(segment);
    }

    if(segment.length() == 3) {
      segment.InsertAt(0, state);
      ugc_hdr.InsertList(segment);
    }
    if(segment.Find(">") != -1) {
      segment.DeleteBeforeIncluding("C");
      segment.DeleteBeforeIncluding("Z");
      gxString r1 = segment.Left(3);
      gxString r2 = segment.Right(3);
      gxString rsegment;
      int range = r2.Atoi() - r1.Atoi(); 
      if(range > 0) {
	for(int ir = 0; ir < (range+1); ir++) {
	  rsegment << clear << (r1.Atoi()+ir);
	  if(rsegment.length() == 1) { // Single digit zones
	    segment << clear << state << "00" << rsegment;
	    ugc_hdr.InsertList(segment);
	  }
	  else if(rsegment.length() == 2) { // Double digit zones
	    segment << clear << state << "0" << rsegment;
	    ugc_hdr.InsertList(segment);
	  }
	  else { // Triple digit zones
	    segment << clear << state << rsegment;
	    ugc_hdr.InsertList(segment);
	  }
	}
      }
    }
    offset2 = offset;
    offset++;
  }

  // Parse the time and return
  return parse_ugc_time_header();
}

int UGC::parse_ugc_time_header()
{
  error_string.Clear();

  // SSZNNN-DDHHMM- 
  // DD is the purge Date (UTC)
  // HH Is the purge hour (UTC)
  // MM is the purge Minutes (UTC) 

  gxString DDHHMM = ugc_string.Right(7);
  DDHHMM.TrimTrailing('-');
  gxString DD = DDHHMM.Left(2);
  gxString HH = DDHHMM.Mid(2, 2);
  gxString MM = DDHHMM.Right(2);
  if((DD.Atoi() < 1) || (DD.Atoi() > 31) || 
     (HH.Atoi() < 0) || (HH.Atoi() > 60) ||  
     (MM.Atoi() < 0) || (MM.Atoi() > 60)) {
    error_string << clear << "UGC parsing error: " << DDHHMM << " bad purge time";
    return 0; 
  }

  ugc_hdr.ugc_time.time_string = DDHHMM;
  ugc_hdr.ugc_time.day = DD;
  ugc_hdr.ugc_time.hour = HH;
  ugc_hdr.ugc_time.minute = MM;

  ugc_hdr.ugc_time.time_zone = "Z";

  time_t gmt_offset, systime_gmt, systime_local;
  int num_offset_hours;
  gxString TZ;
  if(!GetSystemTimeOffset(gmt_offset, systime_gmt, systime_local,
			  num_offset_hours, TZ)) {
    error_string << clear << "UGC parsing error: " << "Error reading system time";
    return 0;
  }

  struct tm tbuf;
  struct tm *tbufp = gmtime(&systime_gmt);
  memmove(&tbuf, tbufp, sizeof(tm));

  char system_day[25];
  char system_month[25];
  char system_year[25];
  strftime(system_day, 80, "%d", &tbuf);
  int system_day_n = atoi(system_day);
  strftime(system_month, 80, "%m", &tbuf);
  int system_month_n = atoi(system_month);
  strftime(system_year, 80, "%Y", &tbuf);
  int system_year_n = atoi(system_year);

  // Set the default values
  ugc_hdr.is_expired = 0;
  ugc_hdr.ugc_time.year = system_year;
  ugc_hdr.ugc_time.month = system_month;

  return set_ugc_time(system_day_n, system_month_n, system_year_n, 
		      system_day, system_month, system_year, systime_gmt);
}

int UGC::ResetUGCTime(TextProduct_time_header &new_exp_time)
{
  error_string.Clear();

  ugc_segment_time = new_exp_time;

  // SSZNNN-DDHHMM- 
  // DD is the purge Date (UTC)
  // HH Is the purge hour (UTC)
  // MM is the purge Minutes (UTC) 

  gxString DDHHMM = ugc_string.Right(7);

  // 08/20/2008: Account for 3 digit time strings
  DDHHMM.DeleteAfterIncluding("-");
  DDHHMM.TrimTrailing('-');
  DDHHMM.TrimLeadingSpaces();
  DDHHMM.TrimTrailingSpaces();

  // 08/20/2008: Account for 3 digit time strings
  gxString DD = DDHHMM.Left(2);
  gxString tbuf = DDHHMM;
  tbuf.DeleteAt(0, 2);
  if(tbuf.length() < 4) {
    tbuf.InsertAt(0, "0");
    // Assume Forecaster set time to X00 instead of 0X00:
    // Example:
    // AMZ630-651-671-FLZ071>074-172>174-20700-
    // 20700 should be 200700
    DDHHMM << clear << DD << tbuf;
  }

  gxString HH = DDHHMM.Mid(2, 2);
  gxString MM = DDHHMM.Right(2);
  if((DD.Atoi() < 1) || (DD.Atoi() > 31) || 
     (HH.Atoi() < 0) || (HH.Atoi() > 60) ||  
     (MM.Atoi() < 0) || (MM.Atoi() > 60)) {
    error_string << clear << "UGC parsing error: " 
		 << DDHHMM << " bad purge time";
    return 0; 
  }

  ugc_hdr.ugc_time.time_string = DDHHMM;
  ugc_hdr.ugc_time.day = DD;
  ugc_hdr.ugc_time.hour = HH;
  ugc_hdr.ugc_time.minute = MM;

  ugc_hdr.ugc_time.time_zone = "Z";

  // Set the default values
  ugc_hdr.is_expired = 0;
  ugc_hdr.ugc_time.year = ugc_segment_time.year;
  ugc_hdr.ugc_time.month = ugc_segment_time.month;

  int system_day_n = ugc_segment_time.day.Atoi();
  int system_month_n = ugc_segment_time.month.Atoi();
  int system_year_n = ugc_segment_time.year.Atoi();

  return set_ugc_time(system_day_n, system_month_n, system_year_n, 
		      ugc_segment_time.day.c_str(), ugc_segment_time.month.c_str(), 
		      ugc_segment_time.year.c_str(), ugc_segment_time.elapsed_time);
}

void UGC::PrintUGCHeader()
{
  if(ugc_string.is_null()) return;

  GXSTD::cout << "\n" << flush;
  GXSTD::cout << "UGC string: " << ugc_string.c_str() << "\n" << flush;
  if(ugc_hdr.is_expired) {
    GXSTD::cout << "UGC product has expired" << "\n" << flush; 
  }
  else {
    GXSTD::cout << "UGC product is active" << "\n" << flush; 
  }

  gxListNode<gxString> *ptr;
  if(!ugc_hdr.zone_list.IsEmpty()) {
    GXSTD::cout << "Zone numbers and county codes: " << flush;
    ptr = ugc_hdr.zone_list.GetHead();
    while(ptr) {
      GXSTD::cout << ptr->data.c_str() << flush;
      ptr = ptr->next;
      if(ptr) GXSTD::cout << ", " << flush;
    }
    GXSTD::cout << "\n" << flush;
  }
  PrintTimeHeader(ugc_hdr.ugc_time);
  GXSTD::cout << "\n" << flush;

  gxListNode<gxString> *sptr;
  if(!zones_names.IsEmpty()) {
    GXSTD::cout << "Zones/Counties names: " << flush;
    sptr = zones_names.GetHead();
    while(sptr) {
      GXSTD::cout << sptr->data.c_str() << flush;
      sptr = sptr->next;
      if(sptr) GXSTD::cout << ", " << flush;
    }
    GXSTD::cout << "\n" << flush;
  }
  if(!include_city_names.IsEmpty()) {
    GXSTD::cout << "Including areas for CRS: " << flush;
    sptr = include_city_names.GetHead();
    while(sptr) {
      GXSTD::cout << sptr->data.c_str() << flush;
      sptr = sptr->next;
      if(sptr) GXSTD::cout << ", " << flush;
    }
    GXSTD::cout << "\n" << flush;
  }

  if(!vtec_list.IsEmpty()) {
    gxListNode<VTEC> *vptr = vtec_list.GetHead();
    while(vptr) {
      vptr->data.PrintVTECHeader();
      vptr = vptr->next;
      if(vptr) GXSTD::cout << "\n" << flush;
    }
  }
}

void UGC::PrintTimeHeader(UGC_time_header &th)
{
  if(th.time_string.is_null()) return;

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

int UGC::ResetIsExpired(VTEC &vtec)
{
  gxString vtec_action = vtec.pvtec_hdr.action.Left(3);
  
  if(vtec.pvtec_hdr.end_time.elapsed_time == (time_t)0) {
    // The product is in affect for the current time
    // 000000T0000Z 
     return 0; // Do not reset the UGC expire time
  }

  if(vtec_action == "CAN") { 
    ugc_hdr.is_expired = 1;
    return 1; // Mark product as expired so it does not appear WWA table
  }
  
  if(vtec.pvtec_hdr.end_time.elapsed_time < 0) {
    error_string << clear << "UGC error: " << "VTEC EXP time is not valid";
    return 0;
  }

  time_t gmt_offset = (time_t)0;
  time_t systime_gmt = (time_t)0;
  time_t systime_local  = (time_t)0;
  int num_offset_hours = 0;
  gxString TZ;

  if(!GetSystemTimeOffset(gmt_offset, systime_gmt, systime_local,
			  num_offset_hours, TZ)) {

    error_string << clear << "UGC error: " 
		 << "cannot read system time to calculate VTEC EXP time";
    return 0;
  }

  time_t product_local_time = systime_gmt;
  time_t vtec_local_time = vtec.pvtec_hdr.end_time.elapsed_time;

  if(vtec_action == "EXP") {
    // Check to see if the EXP time is now or in the future    
    if(vtec_local_time > product_local_time) {
      ugc_hdr.is_expired = 0;
      return 1; // Product expires in the future
    }
    else {
      ugc_hdr.is_expired = 1;
      return 1; // Product has expired
    }
  }

  if(vtec_local_time > product_local_time) {
    ugc_hdr.is_expired = 0;
    return 1; // Product expires in the future
  }

  if(product_local_time > vtec_local_time) {
    ugc_hdr.is_expired = 1;
    return 1; // Product has expired
  }

  return 0; // No valid condition, do not reset the EXP time
}

int UGC::CalcTimeOffsetPast(int system_day_n, int system_month_n, int system_year_n, char *system_year)
{
  int i;

  // We are in the next month so go back one month
  if(system_month_n == 1) { 
    // Check for an overlapping year
    ugc_hdr.ugc_time.year << clear << (system_year_n-1);
    ugc_hdr.ugc_time.month = 12;
  }
  else {
    // Assume the previous month with the same year
    ugc_hdr.ugc_time.year = system_year;
    ugc_hdr.ugc_time.month.Clear();
    i = system_month_n - 1;
    if(i < 10) ugc_hdr.ugc_time.month << "0";
    ugc_hdr.ugc_time.month << i;
  }

  return 1;
}

int UGC::CalcTimeOffsetFuture(int system_day_n, int system_month_n, 
			      int system_year_n, char *system_year)
{
  int i;

  // The UGC product expires in the next mont
  if(system_year_n == 12) {
    // Check for an overlapping year
    ugc_hdr.ugc_time.year << clear << (system_year_n+1);
    ugc_hdr.ugc_time.month << clear << "01";
  }
  else {
    // Assume the next month with the same year
    ugc_hdr.ugc_time.year = system_year;
    i = system_month_n + 1; // Move to the next month 
    ugc_hdr.ugc_time.month.Clear();
    if(i < 10) ugc_hdr.ugc_time.month << "0";
    ugc_hdr.ugc_time.month << i;
  }

  return 1;
}

int UGC::set_ugc_time(int system_day_n, int system_month_n, 
		      int system_year_n, char *system_day,
		      char *system_month,  char *system_year,
		      time_t base_time)
{
  SysTime systime;

  // Match the UGC time against the current system time.
  if(ugc_hdr.ugc_time.day == system_day) {
    // We on the same day so assume the same month and year
    ugc_hdr.ugc_time.year = system_year;
    ugc_hdr.ugc_time.month = system_month;
    ugc_hdr.is_expired = 0;
  }
  else { // The UGC time does not match the current day
    int num_month_days = 31;
    int day_break_point = 17;

    if(system_month_n == 2) {
      num_month_days = 28;
      if(systime.IsLeapYear(system_year_n)) {
	num_month_days = 29;
      }
    }
    if((system_month_n == 1) || (system_month_n == 3) || (system_month_n == 5) ||
       (system_month_n == 7) || (system_month_n == 8) || (system_month_n == 10) ||
       (system_month_n == 12)) {
      num_month_days = 31;
    }
    if((system_month_n == 4) || (system_month_n == 6) || (system_month_n == 9) || 
       (system_month_n == 11)) {
      num_month_days = 30;
    } 

    if(num_month_days == 28) day_break_point = 14;
    if(num_month_days == 29) day_break_point = 15;
    if(num_month_days == 30) day_break_point = 16;
    if(num_month_days == 31) day_break_point = 17;

    if(ugc_hdr.ugc_time.day.Atoi() < day_break_point) {
      if(system_day_n < day_break_point) {
	ugc_hdr.ugc_time.year = system_year;
	ugc_hdr.ugc_time.month = system_month;
      }
      else {
	CalcTimeOffsetFuture(system_day_n, system_month_n, system_year_n, system_year);
      }
    }
    else if(ugc_hdr.ugc_time.day.Atoi() == day_break_point) {
      ugc_hdr.ugc_time.year = system_year;
      ugc_hdr.ugc_time.month = system_month;
    }
    else if((system_day_n == day_break_point) && (ugc_hdr.ugc_time.day.Atoi() > system_day_n)) {
      ugc_hdr.ugc_time.year = system_year;
      ugc_hdr.ugc_time.month = system_month;
    }
    else {
      if(system_day_n > day_break_point) {
	ugc_hdr.ugc_time.year = system_year;
	ugc_hdr.ugc_time.month = system_month;
      } 
      else {
	CalcTimeOffsetPast(system_day_n, system_month_n, system_year_n, system_year);
      }
    }
  }

  struct tm my_tm;
  memset(&my_tm, 0, sizeof(my_tm));
  my_tm.tm_isdst = 0;
  my_tm.tm_wday = -1;

  my_tm.tm_mon = ugc_hdr.ugc_time.month.Atoi();
  my_tm.tm_mon--; // Month index starts at 00 to 11
  my_tm.tm_mday = ugc_hdr.ugc_time.day.Atoi();
  my_tm.tm_year = ugc_hdr.ugc_time.year.Atoi();
  my_tm.tm_year -= 1900;
  my_tm.tm_hour = ugc_hdr.ugc_time.hour.Atoi();
  my_tm.tm_min = ugc_hdr.ugc_time.minute.Atoi();
  my_tm.tm_sec = 0;

  ugc_hdr.ugc_time.elapsed_time = mktime(&my_tm);

  // We need to offset the mktime result based 
  // on the local time zone setting.
  time_t system_gmt_offset;
  time_t systime_gmt, systime_local;
  int num_offset_hours;
  gxString TZ;

  if(GetSystemTimeOffset(system_gmt_offset, systime_gmt, systime_local,
			 num_offset_hours, TZ)) {
    if(system_gmt_offset > 0) {
      ugc_hdr.ugc_time.elapsed_time -= system_gmt_offset;
    }
  }
  else {
    error_string << clear << "UGC error: " 
		 << "cannot read system time to calculate UGC time";
    return 0;
  }

  if(systime_local > ugc_hdr.ugc_time.elapsed_time) {
    // This is an old product so make it expired
    ugc_hdr.is_expired = 1;
    return 1;
  }

  if(ugc_hdr.ugc_time.elapsed_time < base_time) {
    ugc_hdr.is_expired = 1;
  }
  else {
    ugc_hdr.is_expired = 0;
  }

  return 1;
}

int operator==(const UGC &a, const UGC &b)
{
  if(a.ugc_segment_header != b.ugc_segment_header) return 0;
  return 1;
}

// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
