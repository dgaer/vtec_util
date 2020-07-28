// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ---------------------------------------------------------------- // 
// C++ Header File Name: m_config.h
// C++ Compiler Used: MSVC, GCC
// Produced By: Douglas.Gaer@noaa.gov
// File Creation Date: 01/25/2008
// Date Last Modified: 04/23/2009
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

Program configuration utility
*/
// ----------------------------------------------------------- // 
#ifndef __M_CONFIG_HPP__
#define __M_CONFIG_HPP__

#include "gxdlcode.h"

// Function prototypes
int ProcessArgs(int argc, char *argv[]);
int LoadOrBuildConfigFile();
int ProcessDashDashArg(gxString &arg);
void CfgFixTrueFalseString(gxString &out, char *in);

#endif // __M_CONFIG_HPP__
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
