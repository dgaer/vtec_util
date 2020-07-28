// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- // 
// C++ Source Code File Name: m_html.cpp
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

HTML helper code
*/
// ----------------------------------------------------------- // 
#include "gxdlcode.h"

int WriteHTMLHeader(const gxString &pagefilename, const gxString &title, 
		    const gxString &description, gxString &webpage)
{
  if(!generate_page_headers) return 0;

  webpage.Clear();
  // NOTE: XHTML links if needed
  // webpage <<  "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\"\"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">" << "\n";
  // webpage <<  "<html xmlns=\"http://www.w3.org/1999/xhtml\" xmlns:v=\"urn:schemas-microsoft-com:vml\">" << "\n";
  //
  // Google Maps API programming tips say you should use the follow on MAP pages
  // http://code.google.com/apis/maps/documentation/introduction.html
  // <!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
  // <html xmlns="http://www.w3.org/1999/xhtml" xmlns:v="urn:schemas-microsoft-com:vml">
  //
  webpage << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\">" << "\n";
  webpage << "<!-- ******************************* -->" << "\n";
  webpage << "<!-- ******** Start of File ******** -->" << "\n";
  webpage << "<!-- ******************************* -->" << "\n";
  webpage << "<!-- *********************************************************** -->" << "\n";
  webpage << "<!-- HTML File Name: " << pagefilename << " -->" << "\n";
  webpage << "<!-- Supported Browsers: Internet Explorer, Firefox, Mozilla, Netscape -->" << "\n";
  webpage << "<!-- Produced by: " << ProgramDescription << " version " << VersionString  << " -->" << "\n";
  webpage << "<!-- File Creation " << "Date: meta data.created  -->" << "\n";
  webpage << "<!-- Date Last " << "Modified: meta data.reviewed -->" << "\n";
  webpage << "<!-- *********************************************************** -->" << "\n";
  webpage << "<!-- ************* Page Description and Details **************** -->" << "\n";
  webpage << "<!-- *********************************************************** -->" << "\n";
  webpage << "<!-- " << description << " -->" << "\n";
  webpage << "<!-- *********************************************************** -->" << "\n";
  webpage << "<html>" << "\n";
  webpage << "<head>" << "\n";
  // NOTE: This code can be implemented following the NWS Web consolidation
  // webpage << "<link rel=\"shortcut icon\" type=\"image/x-icon\" href=\"/images/favicon.ico\">" << "\n";
  webpage << "<link rel=\"schema.DC\" href=\"http://purl.org/dc/elements/1.1/\">" << "\n";

  // NOTE: UFT-8 coding if needed
  // webpage << "<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\"/>" << "\n";
  webpage << "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\">" << "\n";
  webpage << "<title>" << title << "</title>" << "\n";
  webpage << "<meta name=\"DC.title\" content=\"" << title << "\">" << "\n";
  webpage << "<meta name=\"DC.description\" content=\"" << title << "\">" << "\n";
  webpage << "<meta name=\"DC.creator\" content=\"US Department of Commerce, NOAA, National Weather Service\">" << "\n";
  webpage << "<meta name=\"DC.publisher\" content=\"NOAA's National Weather Service\">" << "\n";
  webpage << "<meta name=\"DC.contributor\" content=\"National Weather Service\">" << "\n";

  SysTime systime;
  gxString year = systime.GetStrTime(SysTime::Year);
  gxString month = systime.GetStrTime(SysTime::Month);
  gxString day = systime.GetStrTime(SysTime::DayOfTheMonth);
  webpage << "<meta name=\"DC.date.created\" scheme=\"ISO8601\" content=\"" <<  year << "-" << month << "-" << day <<"\">" << "\n";
  webpage << "<meta name=\"DC.date.reviewed\" scheme=\"ISO8601\" content=\"" <<  year << "-" << month << "-" << day <<"\">" << "\n";

  webpage << "<meta name=\"DC.language\" scheme=\"DCTERMS.RFC1766\" content=\"EN-US\">" << "\n";
  webpage << "<meta name=\"keywords\" content=\"" << title << "\">" << "\n";
  webpage << "<meta name=\"DC.rights\" content=\"http://www.weather.gov/disclaimer.php\">" << "\n";
  webpage << "<meta name=\"rating\" content=\"General\">" << "\n";
  webpage << "<meta name=\"robots\" content=\"index,follow\">" << "\n";
  webpage << "<meta name=\"Distribution\" content=\"Global\">" << "\n";
  webpage << "<meta name=\"Revisit-after\" content=\"1 month\">" << "\n";
  webpage << "<meta name=\"robots\" content=\"all\">" << "\n";

  if(add_sas_refresh_tag) {
    webpage << "<meta http-equiv=\"refresh\" content=\"" << sas_refresh_seconds << "\">" << "\n";
  }

  webpage << "</head>" << "\n";
  webpage << "<body bgcolor=\"#ffffff\" link=\"#0000ff\" vlink=\"#800080\">" << "\n";
  // NOTE: Do not load from the BODY tag, load() is called from a windows.onload() call in the HTML page
  // NOTE: This will allow the Google map code to be ported to a PHP template
  // webpage <<  "<body onload=\"load()\" onunload=\"GUnload()\">" << "\n";

  return 1;
}

int WriteHTMLTail(gxString &webpage)
{
  if(!generate_page_headers) return 0;

  webpage << "</body>" << "\n";
  webpage << "</html>" << "\n";
  webpage << "<!-- *********************************************************** -->" << "\n";
  webpage << "<!-- ******************************* -->" << "\n";
  webpage << "<!-- ********* End of File ********* -->" << "\n";
  webpage << "<!-- ******************************* -->" << "\n";
  return 1;
}
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
