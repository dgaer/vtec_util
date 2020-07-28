// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ---------------------------------------------------------------- // 
// C++ Header File Name: m_google.h
// C++ Compiler Used: MSVC, GCC
// Produced By: Douglas.Gaer@noaa.gov
// File Creation Date: 01/25/2008
// Date Last Modified: 04/23/2013
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

Code used to generate Google map pages for polygon warnings
*/
// ----------------------------------------------------------- // 
#ifndef __M_GOOGLE_HPP__
#define __M_GOOGLE_HPP__

#include "gxdlcode.h"

// --------------------------------------------------------------
// Global configuration variables
// --------------------------------------------------------------
// 04/23/2013: Added Google maps 3 code
gxString GoogleMaps3URL = "http://maps.googleapis.com/maps/api/js?oe=utf-8&amp;file=api&amp;v=MYVERSION";
gxString GoogleMaps3Key = "&amp;client=MYCLIENTNAME&amp;sensor=false&amp;channel=MYCHANNEL";
gxString GoogleMapsVersion = "3";
gxString GoogleMapsChannel = "noaa.nws.srh.vtecutil";

gxString GoogleMapsClientName = "gme-noaa";
gxString GoogleMapsWMSTransparentLayersScript = "http://www.srh.noaa.gov/ridge/kml/gmap-wms_keith2.js"; 
gxString GoogleMapsWMSGAL = "http://gis.srh.noaa.gov/wmsconnector/com.esri.wms.Esrimap?ServiceName=radar_warnings";
gxString GoogleMapsGIcon = "http://maps.google.com/mapfiles/kml/pal2/icon15.png";
gxString GoogleMapsWebPageName = "DEFAULT";
gxString GoogleMapsDefaultTextColor = "red";
gxString GoogleMapsTextHighlightColor = "red";
gxString GoogleMapsTextExpiredHighlightColor = "black";
gxString GoogleMapsDefaultZoom = "8";

// Version 2 legacy code
// NOTE: As of May 2013 Google maps V2 will no longer work
gxString GoogleMapsURL = "http://maps.google.com/maps?oe=utf-8&amp;file=api&amp;v=2&amp;key=MYKEY&amp;client=MYCLIENT";
gxString GoogleMapsKey = "ABQIAAAAIoZKb9JrajP0IwKLFr335hTsk_juWuwt_LNtOqByAr93uT9HuRTzovVKWJSi3TQGBPsWMWPyWdIQdg";
gxString GoogleMapsWMSSpec = "WMS_URL_GAL, \"Radar\", \"Radar\", \"Radar\", \"default\", \"image/gif\"";
gxString GoogleMapsWMSOverlaySpec = "G_NORMAL_MAP, rad, \"Radar\", \"Radar\"";

// 06/02/2008: Changes to use the WWA map and RIDGE colors
gxString GoogleMapsTextColorByProduct = "TOR,#ff0000|SVR,#ffff00|FFW,#00ff00|SMW,#0000ff";

gxString GoogleMapsZoomByProduct = "TOR,9|SVR,9|FFW,9|SMW,8|SVS,9";
gxString GoogleMapsPHPHeaderIncludeCMS = "NONE";
gxString GoogleMapsPHPHeaderIncludeSite = "NONE";
gxString GoogleMapsPHPLeftIncludeCMS = "NONE";
gxString GoogleMapsPHPLeftIncludeSite = "NONE";
gxString GoogleMapsPHPFooterIncludeSite = "NONE";
gxString GoogleMapsPHPFooterIncludeCMS = "NONE";
int GooleMapsPrintExpiredMessage = 1;
int GoogleMapsTextOverlay = 1;
int GoogleMapsWidth = 525;
int GoogleMapsHeight = 350;

// 12/04/2008: Allow the Radar overlay to be optional
gxString GoogleMapsRadarOverlay = "TRUE";

// 06/10/2008: Make disclamier message optional
gxString GoogleMapsDisplayDisclaimer = "TRUE";
gxString GoogleMapsDisclaimer = "The map portion of the following product is experimental and may not always be available.";

// 07/16/2008: Add a path for Google map page on the web server
gxString GoogleMapsWebServerLocation;

// 01/14/2009: Add a PHP wrapper script for the CMS pages
gxString GoogleMapsWebServerRequestScript;
gxString GoogleMapsWebServerRequestVar = "n";
gxString GoogleMapsWebServerTitleVar = "title";
// --------------------------------------------------------------

// Command line args configuration overrides
int gen_js_files = 0;

// Google map functions
int generate_google_map_page(TextProduct &text_product);

// Helper function that generates the page text per UGC segment
int generate_google_map_page(TextProduct &text_product, UGC &ugc_segment, 
			     int current_ugc_segment, gxString &webpage, gxString &pagefilename, 
			     gxString &fname, gxString &js_file);
#endif // __M_GOOGLE_HPP__
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
 
