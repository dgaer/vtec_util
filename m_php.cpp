// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- // 
// C++ Source Code File Name: m_php.cpp
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

PHP helper code
*/
// ----------------------------------------------------------- // 
#include "gxdlcode.h"

int WritePHPTail(gxString &webpage)
{
  if(!generate_page_headers) return 0;

  webpage << "<?php" << "\n";
  webpage << "// ----------------------------------------------------------- //" << "\n";
  webpage << "// ------------------------------- //" << "\n";
  webpage << "// --------- End of File --------- //" << "\n";
  webpage << "// ------------------------------- //" << "\n";
  webpage << "?>" << "\n";
  return 1;
}

int WritePHPIncludeHeader(const gxString &pagefilename,  
			  const gxString &description, gxString &webpage)
{
  if(!generate_page_headers) return 0;

  webpage.Clear();
  webpage << "<?php" << "\n";
  webpage << "// ------------------------------- //" << "\n";
  webpage << "// -------- Start of File -------- //" << "\n";
  webpage << "// ------------------------------- //" << "\n";
  webpage << "// ----------------------------------------------------------- // " << "\n";
  webpage << "// PHP Include File Name: " << pagefilename << "\n";
  webpage << "// PHP Interpreter Version: 4.x or higher" << "\n";
  webpage << "// Produced By: " << ProgramDescription << " version " << VersionString << "\n";
  SysTime systime;
  gxString year = systime.GetStrTime(SysTime::Year);
  gxString month = systime.GetStrTime(SysTime::Month);
  gxString day = systime.GetStrTime(SysTime::DayOfTheMonth);
  webpage << "// File Creation " << "Date: " << month << "/" << day << "/" << year << "\n";
  webpage << "// Date Last " << "Modified: " << month << "/" << day << "/" << year << "\n";
  webpage << "// ----------------------------------------------------------- // " << "\n";
  webpage << "// ----------- Include File Description and Details ---------- // " << "\n";
  webpage << "// ----------------------------------------------------------- // " << "\n";
  webpage << "//" << "\n";
  webpage << "// " << description << "\n";
  webpage << "//" << "\n";
  webpage << "// ----------------------------------------------------------- // " << "\n";
  webpage << "?>" << "\n";

  return 1;
}
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
