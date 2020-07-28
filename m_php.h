// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ---------------------------------------------------------------- // 
// C++ Header File Name: m_php.h
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

PHP helper code
*/
// ----------------------------------------------------------- // 
#ifndef __M_PHP_HPP__
#define __M_PHP_HPP__

#include "gxdlcode.h"

// --------------------------------------------------------------
// Global configuration variables
// --------------------------------------------------------------
// Set the default file names
gxString php_file_extension = ".php";
// --------------------------------------------------------------

int WritePHPIncludeHeader(const gxString &pagefilename,  
			  const gxString &description, gxString &webpage);
int WritePHPTail(gxString &webpage);

#endif // __M_PHP_HPP__
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
