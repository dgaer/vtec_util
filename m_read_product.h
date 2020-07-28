// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ---------------------------------------------------------------- // 
// C++ Header File Name: m_prod.h
// C++ Compiler Used: MSVC, GCC
// Produced By: Douglas.Gaer@noaa.gov
// File Creation Date: 01/25/2008
// Date Last Modified: 07/17/2009
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

Product class
*/
// ----------------------------------------------------------- // 
#ifndef __M_READ_PRODUCT_HPP__
#define __M_READ_PRODUCT_HPP__

#include "gxdlcode.h"

// Global configuration variables 
int read_product_formatting = 0;
gxString read_product_config_file;
gxString default_read_product_config_file = "read_product.cfg";
gxString WriteJavaScriptCode = "FALSE";
gxString JavaScriptIncludeFile = "NONE";
gxString StartProductText;
gxString EndProductText;
gxString KeepStartProductText = "TRUE";
gxString KeepEndProductText = "FALSE";
gxString SkipEmptySegments = "FALSE";
gxString GetAllProductTextBlocks = "FALSE";
gxString MultiLineText = "TRUE";
gxString FilterStringFromText;
gxString OutputInHTMLFormat = "FALSE";
gxString CustomProductName = "NONE";
gxString CustomProductTitle = "NONE";
gxString ReadVerbose = "TRUE";
gxString ReadAllUpper = "TRUE";
gxString ReadIssueOffice = "TRUE";
gxString ReadVTECMessage = "TRUE";
gxString ReadZoneNumbers = "TRUE";
gxString ReadCountyCodes = "TRUE";
gxString ReadAreaNames = "TRUE";
gxString ReadIncludingAreaNames = "TRUE";
gxString ReadProductDelimter = ",";
gxString ReadPolygon = "TRUE";
gxString HasIssuedTitle = "HAS ISSUED A";
gxString IssuedFromTitle = "Issued at";
gxString IssuedUntilTitle = "until";
gxString ZoneListingTitle = "Issued for the following zones:";
gxString CountyListingTitle = "Issued for the following counties:";
gxString AreaListingTitle = "Issued for the following areas:";
gxString IncludingAreaListingTitle = "Including:";
gxString PolygonListingTitle = "Polygon warning strings:";
gxString CanceledTitle = "Canceled at";
gxString ExpiresTitle = "Issued";
gxString ExpiresAtTitle = "expires at";
gxString TextBodyColor = "black";

// 06/01/2009: Added support to parse text after the product header but before the first
// 06/01/2009: VTEC segment as global text.
gxString ReadGlobalText = "FALSE";
gxString ParseFromGlobalTextOnly = "FALSE";
gxString StartGlobalText;
gxString EndGlobalText;
gxString KeepStartGlobalText = "TRUE";
gxString KeepEndGlobalText = "FALSE";
gxString CustomGlobalName = "NONE";
gxString CustomGlobalTitle = "NONE";
gxString GlobalParseSkipsStartProductText = "FALSE";
gxString MultiLineGlobalText  = "TRUE";
gxString GetAllProductGlobalTextBlocks = "FALSE";
gxString FilterStringFromGlobalText;
gxString SkipGlobalTextIfEmptySegments = "FALSE";

// 06/17/2009: Added support for RSS feeds
gxString CustomProductURL = "NONE";
gxString CustomProductDescription = "NONE";
gxString RSSDefaultURL = "http://www.weather.gov";
gxString RSSWebmaster;
gxString RSSManagingEditor;
gxString RSSImageURL;
gxString RSSImageLink;
gxString RSSImageTitle;
gxString RSSImageDescription;
gxString RSSTextInputTitle;
gxString RSSTextInputDescription;
gxString RSSTextInputName;
gxString RSSTextInputLink;

// 07/16/2009: Added more regex support
gxString RegexOR = "||";

// Standalone functions
int LoadOrBuildReadProductConfigFile();
void RPCfgFixTrueFalseString(gxString &out, char *in);

#endif // __M_READ_PRODUCT_HPP__
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
 
