// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ---------------------------------------------------------------- // 
// C++ Header File Name: m_javascript.h
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

JavaScript helper code
*/
// ----------------------------------------------------------- // 
#ifndef __M_JAVASCRIPT_HPP__
#define __M_JAVASCRIPT_HPP__

#include "gxdlcode.h"

// --------------------------------------------------------------
// Global configuration variables
// --------------------------------------------------------------
// Set the default file names
gxString js_file_extension = ".js";
// --------------------------------------------------------------

// Standalone JavaScript helper functions
int MakeJSPopupProduct(UGCSegmentListNode *ugc_node);
int MakeJSProductIconLink(UGCSegmentListNode *ugc_node);

int MakeJSGoogleMapPopupProduct(gxString &jscode, TextProduct &text_product, UGC &ugc_segment, 
				int current_ugc_segment, const gxString pil, const gxString &etn, 
				const gxString &product_name);

void JSdocumentWriteLine(gxString &jscode, const gxString &piletn, const char *s);

void JSdocumentWrite(gxString &jscode, const gxString &piletn, const char *s);

void JSdocumentWriteFileLines(gxString &jscode, const gxString &piletn, const gxString &filelines);

int MakeJSGoogleMapFile(TextProduct &text_product, UGC &ugc_segment, 
			int current_ugc_segment, gxString &pagefilename,
			gxString &fname);

int MakeJSFileIconLink(gxString &jscode, const gxString &pil, 
		       gxString &pagefilename, const gxString &title);

int MakeJSPopupKeyTable(gxString &jscode, gxString &ahreftab, const gxString &table);

int MakeJSPopupVerTable(gxString &jscode, gxString &ahreftab, const gxString &table);

int WriteJavaScriptTail(gxString &jscode);
int WriteJavaScriptHeader(const gxString &pagefilename,  
			  const gxString &description, gxString &jscode);

#endif // __M_JAVASCRIPT_HPP__
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
