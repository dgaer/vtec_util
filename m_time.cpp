// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- // 
// C++ Source Code File Name: m_time.cpp
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

Time classes and functions
*/
// ----------------------------------------------------------- // 
#include "gxdlcode.h"

TextProduct_time_header &TextProduct_time_header::operator=(const TextProduct_time_header &ob)
{
  if(&ob != this) Copy(ob);
  return *this;
}

void TextProduct_time_header::Copy(const TextProduct_time_header &ob)
{
  time_string = ob.time_string;
  time_zone = ob.time_zone;
  year = ob.year;
  month = ob.month;
  day = ob.day;
  hour = ob.hour;
  minute = ob.minute;
  AMPM = ob.AMPM;
  wkday = ob.wkday;
  elapsed_time = ob.elapsed_time;
  span_multiple_zones = ob.span_multiple_zones;
  timezone_offset = ob.timezone_offset;
  gmt_offset = ob.gmt_offset;
  localtime = ob.localtime;
}

void TextProduct_time_header::Clear()
{
  time_string.Clear();
  time_zone.Clear();
  year.Clear();
  month.Clear();
  day.Clear();
  hour.Clear();
  minute.Clear();
  AMPM.Clear();
  wkday.Clear();
  elapsed_time = (time_t)0;
  span_multiple_zones = 0;
  timezone_offset = (time_t)0;
  gmt_offset = (time_t)0;
  localtime = (time_t)0;
}


int ParseTextProductTime(TextProduct_time_header &th)
{
  gxString error_string;
  return ParseTextProductTime(th, error_string);
}

int ParseTextProductTime(TextProduct_time_header &th, gxString &error_string)
{
  error_string.Clear();
  if(th.time_string.is_null()) {
    error_string << clear << "Text product time error: " 
		 << "Time string is null";
    return 0;
  }
  gxString sbuf = th.time_string;
  return ParseTextProductTime(sbuf.c_str(), th, error_string);
}

int ParseTextProductTime(const char *time_string, TextProduct_time_header &th)
{
  gxString error_string;
  return ParseTextProductTime(time_string, th, error_string);
}

int ParseTextProductTime(const char *time_string, TextProduct_time_header &th, 
			 gxString &error_string)
{
  error_string.Clear();

  if(!time_string) {
    error_string << clear << "Text product time error: " 
		 << "Null time string pointer";
    return 0;
  }

  // Product time string format
  // HMM AM EST SUN JAN D YYYY
  // HMM AM EST SUN JAN DD YYYY
  // HHMM AM EST SUN JAN D YYYY
  // HHMM AM EST SUN JAN DD YYYY
  //  0   1   2   3   4  5   6

  char words[MAXWORDS][MAXWORDLENGTH];
  int numwords;
  int i;

  if(parse(th.time_string.c_str(), words, &numwords, ' ') == 1) {
    error_string << clear << "Text product error: " << th.time_string
		 << " bad issue time";
    return 0;
  }
  if(numwords < 7) {
    error_string << clear << "Text product error: " << th.time_string
		 << " issue time missing fields";
    return 0;
  }

  if(*words[0] != 0) {
    gxString HHMM = words[0];
    gxString MM = HHMM.Right(2);
    th.minute = MM;
    if(HHMM.length() == 3) {
      gxString HH = HHMM.Left(1);
      th.hour << clear << "0" << HH;
    }
    else if(HHMM.length() == 4) {
      gxString HH = HHMM.Left(2);
      th.hour = HH;
    }
    else {
      error_string << clear << "Text product error: " << th.time_string 
		   << " bad issue hour";
      return 0;
    }
  }
  else {
    error_string << clear << "Text product error: " << th.time_string 
		 << " missing issue hour";
    return 0;
  }
  if(*words[1] != 0) {
    th.AMPM = words[1];
  }
  else {
    error_string << clear << "Text product error: " << th.time_string 
		 << " missing issue AMPM";
    return 0;
  }
  if(*words[2] != 0) {
    th.time_zone = words[2];
  }
  else {
    error_string << clear << "Text product error: " << th.time_string 
		 << " missing issue timezone";
    return 0;
  }
  if(*words[3] != 0) {
    gxString WKDAY = words[3];
    int has_day = 0;
    for(i = 0; i < 7; i++) {
      if(WKDAY.Find(System_weekdays[i]) != -1) {
	has_day = 1;
	break;
      }
    }
    if(!has_day) {
      error_string << clear << "Text product error: " << th.time_string 
		   << " bad issue weekday name";
      return 0;
    }
    th.wkday << clear << "0" << i;
  }
  else {
    error_string << clear << "Text product error: " << th.time_string 
		 << " missing issue Weekday name";
    return 0;
  }
  if(*words[4] != 0) {
    gxString MONTH = words[4];
    int has_month = 0;
    for(i = 0; i < 12; i++) {
      if(MONTH.Find(System_months[i]) != -1) {
	has_month = 1;
	break;
      }
    }
    if(!has_month) {
      error_string << clear << "Text product error: " << th.time_string 
		   << " bad issue month";
      return 0;
    }
    i++;
    th.month.Clear();
    if(i < 10) th.month << "0";
    th.month << i;
  }
  else {
    error_string << clear << "Text product error: " << th.time_string 
		 << " missing issue month";
    return 0;
  }
  if(*words[5] != 0) {
    th.day = words[5];
    if(th.day.Atoi() < 10) th.day.InsertAt(0, "0");
  }
  else {
    error_string << clear << "Text product error: " << th.time_string 
		 << " missing issue day";
    return 0;
  }
  if(*words[6] != 0) {
    th.year = words[6];
  }
  else {
    error_string << clear << "Text product error: " << th.time_string 
		 << " missing issue year";
    return 0;
  }

  struct tm my_tm;
  memset(&my_tm, 0, sizeof(my_tm));
  my_tm.tm_isdst = 0;
  my_tm.tm_wday = th.wkday.Atoi();
  my_tm.tm_mon = th.month.Atoi();
  my_tm.tm_mon--;
  my_tm.tm_mday = th.day.Atoi();
  my_tm.tm_year = th.year.Atoi();
  my_tm.tm_year -= 1900;
  if(th.AMPM == "PM") {
    my_tm.tm_hour = th.hour.Atoi() +12;
    if(my_tm.tm_hour == 24) my_tm.tm_hour = 12; 
  }
  else {
    my_tm.tm_hour = th.hour.Atoi();
    if(my_tm.tm_hour == 12) my_tm.tm_hour = 0; 
  }

  my_tm.tm_sec = 0;
  my_tm.tm_min = th.minute.Atoi();

  // NOTE: This time header is in local time
  th.elapsed_time = mktime(&my_tm);

  // We need to offset the mktime result based 
  // on the local time zone setting.
  time_t system_gmt_offset;
  if(GetSystemTimeOffset(system_gmt_offset)) {
    // NOTE: This time header is in local time
    th.elapsed_time -= system_gmt_offset;
  }
  else {
    if(debug) NT_print("Error reading your system timezone offset for product time");
  }

  int has_timezone = 0;
  time_t gmt_offset = 0;

  gxListNode<gxString> *tzptr = Timezone_table.GetHead();
  while(tzptr) {
    gxString TZ = tzptr->data;
    if(TZ.Find(th.time_zone) != -1) {
      if((TZ.Find("(") == -1) || (TZ.Find(")") == -1)) {
	error_string << clear << "Text product error: " << TZ
		     << " bad timezone table entry";
	return 0;
      } 
      TZ.DeleteBeforeIncluding("(");
      TZ.DeleteAfterIncluding(")");
      TZ.TrimLeading('('); TZ.TrimTrailing(')');
      gmt_offset = (TZ.Atoi() * (60 * 60));
      th.timezone_offset = TZ.Atoi();
      th.gmt_offset = gmt_offset;
      has_timezone = 1;
      break;
    }
    tzptr = tzptr->next;
  }

  if(!has_timezone) {
    error_string << clear << "Text product error: " << th.time_zone 
		 << " unknown timezone";
    return 0;
  }

  if(th.time_string.Find(" /") != -1) th.span_multiple_zones = 1;

  th.localtime = th.elapsed_time; // Record the local time
  th.elapsed_time += gmt_offset;

  return 1;
}

int GetLocalTimeString(const char *TZ, time_t gmttime, time_t gmt_offset, 
		       gxString &time_string, int short_format)
{
  time_string.Clear();

  if((gmttime <= 0) || (gmt_offset < 0)) return 0;
  if(!TZ) return 0;
  if(gmttime < gmt_offset) return 0;

  time_t elapsed_seconds = gmttime - gmt_offset;

  char s[255];
  char month[25]; char day[25]; char year[25]; char wday_name[25];
  char hour[25]; char minutes[25];
  struct tm tm_local;
  struct tm *tm_ptr;
  memset(&tm_local, 0, sizeof(tm_local));


  tm_ptr = gmtime(&elapsed_seconds);
  memmove(&tm_local, tm_ptr, sizeof(tm_local));

  strftime(month, 25, "%b", &tm_local);
  strftime(day, 25, "%d", &tm_local);
  strftime(year, 25, "%Y", &tm_local);
  strftime(hour, 25, "%H", &tm_local);

  gxString AMPM = "AM";
  gxString hour_str = hour;
  int hour_int = hour_str.Atoi();
  int has_set_ampm = 0;  

  if((hour_int == 0) || (hour_int == 24)) {
    AMPM = "AM";
    hour_int = 12;
    hour_str << clear << "12";
    has_set_ampm = 1;
  }
  if((hour_int > 12) && (!has_set_ampm)) {
    hour_int -= 12;
    AMPM = "PM";
    hour_str << clear << hour_int;
    has_set_ampm = 1;
  }
  if((hour_int == 12) && (!has_set_ampm)) {
    AMPM = "PM";
    hour_str << clear << hour_int;
    has_set_ampm = 1;
  }

  strftime(minutes, 25, "%M", &tm_local);
  strftime(wday_name, 25, "%a", &tm_local);

  if(!short_format) {
    // Weekday Name Month Day, Year HH:MM:SS XX ZZZ
    // Example: Tue, 7 Feb 2008 08:12 AM EST
    sprintf(s, "%s, %s %s %s %s:%s %s %s",
	    wday_name, day, month, year, hour_str.c_str(), minutes, AMPM.c_str(), TZ);
    time_string = s;
  }
  else {
    // Example: 08:12 AM EST
    sprintf(s, "%s:%s %s %s",
	    hour_str.c_str(), minutes, AMPM.c_str(), TZ);
    time_string = s;

  }

  return 1;
}

int HasExpiredTimeString(const char *TZ, time_t product_time, time_t gmt_offset, gxString &time_string, int has_expired)
{
  time_string.Clear();
  has_expired = 0;
  time_t gmttime;
  gmtime(&gmttime);

  if((gmttime <= 0) || (gmt_offset < 0)) return 0;
  if(!TZ) return 0;
  if(gmttime < gmt_offset) return 0;

  time_t elapsed_seconds = gmttime - gmt_offset;

  if(elapsed_seconds > product_time) {
    has_expired = 1;
    // The product has expired so return a string of when it expired
    elapsed_seconds = product_time;
  }
  // ELSE - the product has not expired so return the time string of when 
  // it will expire.

  char s[255];
  char month[25]; char day[25]; char year[25]; char wday_name[25];
  char hour[25]; char minutes[25];
  struct tm tm_local;
  struct tm *tm_ptr;
  memset(&tm_local, 0, sizeof(tm_local));


  tm_ptr = gmtime(&elapsed_seconds);
  memmove(&tm_local, tm_ptr, sizeof(tm_local));

  strftime(month, 25, "%b", &tm_local);
  strftime(day, 25, "%d", &tm_local);
  strftime(year, 25, "%Y", &tm_local);
  strftime(hour, 25, "%H", &tm_local);

  gxString AMPM = "AM";
  gxString hour_str = hour;
  int hour_int = hour_str.Atoi();
  int has_set_ampm = 0;  

  if((hour_int == 0) || (hour_int == 24)) {
    AMPM = "AM";
    hour_int = 12;
    hour_str << clear << "12";
    has_set_ampm = 1;
  }
  if((hour_int > 12) && (!has_set_ampm)) {
    hour_int -= 12;
    AMPM = "PM";
    hour_str << clear << hour_int;
    has_set_ampm = 1;
  }
  if((hour_int == 12) && (!has_set_ampm)) {
    AMPM = "PM";
    hour_str << clear << hour_int;
    has_set_ampm = 1;
  }

  strftime(minutes, 25, "%M", &tm_local);
  strftime(wday_name, 25, "%a", &tm_local);

  // Weekday Name Month Day, Year HH:MM:SS XX ZZZ
  // Example: Tue, 7 Feb 2008 08:12 AM EST
  sprintf(s, "%s, %s %s %s %s:%s %s %s",
	  wday_name, day, month, year, hour_str.c_str(), minutes, AMPM.c_str(), TZ);
  time_string = s;


  return 1;
}

int MakeWarningTableTimeString(gxString &time_string) 
{
  time_string.Clear();

  int has_timezone = 0;
  time_t gmt_offset = 0;
  time_t timezone_offset = 0;
  gxString TZ;

  gxListNode<gxString> *tzptr = Timezone_table.GetHead();
  while(tzptr) {
    TZ = tzptr->data;
    if(TZ.Find(WarningTableTimezone) != -1) {
      if((TZ.Find("(") == -1) || (TZ.Find(")") == -1)) {
	if(debug) NT_print("Error no GMT offset listed for timezone", 
			   TZ.c_str());
	return 0;
      } 
      TZ.DeleteBeforeIncluding("(");
      TZ.DeleteAfterIncluding(")");
      TZ.TrimLeading('('); TZ.TrimTrailing(')');
      gmt_offset = (TZ.Atoi() * (60 * 60));
      timezone_offset = TZ.Atoi();
      gmt_offset = gmt_offset;
      has_timezone = 1;
      break;
    }
    tzptr = tzptr->next;
  }

  if(!has_timezone) {
    if(debug) NT_print("Error unknown timezone", WarningTableTimezone.c_str());
    return 0;
  }

  time_t curr_gmt_time;
  time(&curr_gmt_time);

  if(curr_gmt_time <= 0) {
    if(debug) NT_print("Error system clock error reading GMT time");
    return 0;
  }
  if(gmt_offset < 0) {
    if(debug) NT_print("Error bad GMT timezone offset");
    return 0;
  }
  if(TZ.is_null()) {
    if(debug) NT_print("Error NULL timezone string");
    return 0;
  }
  if(curr_gmt_time < gmt_offset) {
    if(debug) NT_print("Error GMT offet is greater than the system time");
    return 0;
  }

  time_t elapsed_seconds = curr_gmt_time - gmt_offset;
  char s[255];
  char month[25]; char day[25]; char year[25]; char wday_name[25];
  char hour[25]; char minutes[25];
  struct tm tm_local;
  struct tm *tm_ptr;
  memset(&tm_local, 0, sizeof(tm_local));

  tm_ptr = gmtime(&elapsed_seconds);
  memmove(&tm_local, tm_ptr, sizeof(tm_local));

  strftime(month, 25, "%b", &tm_local);
  strftime(day, 25, "%d", &tm_local);
  strftime(year, 25, "%Y", &tm_local);
  strftime(hour, 25, "%H", &tm_local);

  gxString AMPM = "AM";
  gxString hour_str = hour;
  int hour_int = hour_str.Atoi();
  int has_set_ampm = 0;  

  if((hour_int == 0) || (hour_int == 24)) {
    AMPM = "AM";
    hour_int = 12;
    hour_str << clear << "12";
    has_set_ampm = 1;
  }
  if((hour_int > 12) && (!has_set_ampm)) {
    hour_int -= 12;
    AMPM = "PM";
    hour_str << clear << hour_int;
    has_set_ampm = 1;
  }
  if((hour_int == 12) && (!has_set_ampm)) {
    AMPM = "PM";
    hour_str << clear << hour_int;
    has_set_ampm = 1;
  }

  strftime(minutes, 25, "%M", &tm_local);
  strftime(wday_name, 25, "%a", &tm_local);

  // Example: Feb 21, 2008 08:12 AM EST
  sprintf(s, "%s %s, %s %s:%s %s %s",
	  month, day, year, hour_str.c_str(), minutes, AMPM.c_str(), 
	  WarningTableTimezone.c_str());
  time_string = s;

  return 1;
}

int GetIconLinkLocalTimeString(const char *TZ, time_t gmttime, time_t gmt_offset, 
			       gxString &time_string, int short_format)
{
  time_string.Clear();

  if((gmttime <= 0) || (gmt_offset < 0)) return 0;
  if(!TZ) return 0;
  if(gmttime < gmt_offset) return 0;

  time_t elapsed_seconds = gmttime - gmt_offset;

  char s[255];
  char month[25]; char day[25]; char year[25];
  char hour[25]; char minutes[25];
  struct tm tm_local;
  struct tm *tm_ptr;
  memset(&tm_local, 0, sizeof(tm_local));

  tm_ptr = gmtime(&elapsed_seconds);
  memmove(&tm_local, tm_ptr, sizeof(tm_local));

  strftime(month, 25, "%m", &tm_local);
  strftime(day, 25, "%d", &tm_local);
  strftime(year, 25, "%Y", &tm_local);
  strftime(hour, 25, "%H", &tm_local);

  gxString AMPM = "AM";
  gxString hour_str = hour;
  int hour_int = hour_str.Atoi();
  int has_set_ampm = 0;  

  if((hour_int == 0) || (hour_int == 24)) {
    AMPM = "AM";
    hour_int = 12;
    hour_str << clear << "12";
    has_set_ampm = 1;
  }
  if((hour_int > 12) && (!has_set_ampm)) {
    hour_int -= 12;
    AMPM = "PM";
    hour_str << clear << hour_int;
    has_set_ampm = 1;
  }
  if((hour_int == 12) && (!has_set_ampm)) {
    AMPM = "PM";
    hour_str << clear << hour_int;
    has_set_ampm = 1;
  }

  strftime(minutes, 25, "%M", &tm_local);

  if(!short_format) {
    // Weekday Name Month Day, Year HH:MM:SS XX ZZZ
    // Example: 02/25/2008 08:12 AM EST
    sprintf(s, "%s/%s/%s %s:%s %s %s",
	    month, day, year, hour_str.c_str(), minutes, AMPM.c_str(), TZ);
    time_string = s;
  }
  else {
    strftime(month, 25, "%b", &tm_local);
    // Example: Feb 25 08:12 AM EST
    sprintf(s, "%s %s %s:%s %s %s",
	    month, day, hour_str.c_str(), minutes, AMPM.c_str(), TZ);
    time_string = s;

  }

  return 1;
}

int GetSystemTime(time_t &systime_gmt, time_t &systime_local)
{
  time_t gmt_offset;
  int num_offset_hours;
  gxString TZ;
  return GetSystemTimeOffset(gmt_offset, systime_gmt, systime_local,
			     num_offset_hours, TZ);
}

int GetSystemTimeOffset(time_t &gmt_offset)
{
  time_t systime_gmt, systime_local;
  int num_offset_hours;
  gxString TZ;
  return GetSystemTimeOffset(gmt_offset, systime_gmt, systime_local,
			     num_offset_hours, TZ);
}

int GetSystemTimeOffset(int &num_offset_hours, gxString &TZ)
{
  time_t gmt_offset, systime_gmt, systime_local;
  return GetSystemTimeOffset(gmt_offset, systime_gmt, systime_local,
			     num_offset_hours, TZ);
}

int GetSystemTimeOffset(time_t &gmt_offset, time_t &systime_gmt, time_t &systime_local,
			int &num_offset_hours, gxString &TZ)
{
  gmt_offset = (time_t)0;
  systime_local = (time_t)0;
  systime_gmt = (time_t)0;
  TZ.Clear();
  num_offset_hours = 0;
  
  time_t my_local_time, my_gmt_time;
  struct tm lt;
  struct tm gt;
  gxString sbuf;

  time(&my_local_time);
  time(&my_gmt_time);

  lt = *localtime(&my_local_time);
  gt = *gmtime(&my_gmt_time);
  systime_gmt = mktime(&gt);
  systime_local = mktime(&lt);

  if(systime_local < 0) {
    return 0;
  } 
  if(systime_gmt < 0) {
    return 0;
  }

  gmt_offset = systime_gmt - systime_local;

  if(gmt_offset == 0) { // We are in Z-time
    num_offset_hours = 0; 
    gmt_offset = time_t(0);
  }
  else {
    num_offset_hours = gmt_offset/(60*60);
  }

  // We need to offset the mktime results based on 
  // the local timezone setting
  systime_gmt -= gmt_offset;
  systime_local -= gmt_offset;

  gxString ibuf;
  ibuf << clear << "(" << num_offset_hours << ")";

  gxListNode<gxString> *tzptr = Timezone_table.GetHead();
  while(tzptr) {
    sbuf = tzptr->data;
    if(sbuf.Find(ibuf) != -1) {
      TZ = sbuf;
      TZ.DeleteAfterIncluding("(");
      TZ.TrimLeadingSpaces();
      TZ.TrimTrailingSpaces();
      break;
    }
    tzptr = tzptr->next;
  }

  return 1;
}

char *EchoTimeString(time_t elapsed_seconds, gxString &time_string)
{
  time_string = "Error invalid elapsed time";

  if(elapsed_seconds < 0) return time_string.c_str();

  char s[255];
  char month[25]; char day[25]; char year[25];
  char hour[25]; char minutes[25];
  struct tm tm_local;
  struct tm *tm_ptr;
  memset(&tm_local, 0, sizeof(tm_local));

  tm_ptr = gmtime(&elapsed_seconds);
  memmove(&tm_local, tm_ptr, sizeof(tm_local));

  strftime(month, 25, "%m", &tm_local);
  strftime(day, 25, "%d", &tm_local);
  strftime(year, 25, "%Y", &tm_local);
  strftime(hour, 25, "%H", &tm_local);

  gxString AMPM = "AM";
  gxString hour_str = hour;
  int hour_int = hour_str.Atoi();
  int has_set_ampm = 0;  

  if((hour_int == 0) || (hour_int == 24)) {
    AMPM = "AM";
    hour_int = 12;
    hour_str << clear << "12";
    has_set_ampm = 1;
  }
  if((hour_int > 12) && (!has_set_ampm)) {
    hour_int -= 12;
    AMPM = "PM";
    hour_str << clear << hour_int;
    has_set_ampm = 1;
  }
  if((hour_int == 12) && (!has_set_ampm)) {
    AMPM = "PM";
    hour_str << clear << hour_int;
    has_set_ampm = 1;
  }

  strftime(minutes, 25, "%M", &tm_local);

  // Weekday Name Month Day, Year HH:MM:SS XX
  // Example: 02/25/2008 08:12 AM
  sprintf(s, "%s/%s/%s %s:%s %s",
	  month, day, year, hour_str.c_str(), minutes, AMPM.c_str());
  time_string = s;

  return time_string.c_str();
}
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //



