// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ---------------------------------------------------------------- // 
// C++ Header File Name: m_html.h
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

HTML helper code
*/
// ----------------------------------------------------------- // 
#ifndef __M_HTML_HPP__
#define __M_HTML_HPP__

#include "gxdlcode.h"


// --------------------------------------------------------------
// Global configuration variables
// --------------------------------------------------------------
// Set the default file names
gxString html_file_extension = ".html";
// --------------------------------------------------------------

int WriteHTMLHeader(const gxString &pagefilename, const gxString &title, 
		    const gxString &description, gxString &webpage);
int WriteHTMLTail(gxString &webpage);

#endif // __M_HTML_HPP__
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
