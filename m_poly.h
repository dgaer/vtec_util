// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ---------------------------------------------------------------- // 
// C++ Header File Name: m_poly.h
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

Polygon warning class
*/
// ----------------------------------------------------------- // 
#ifndef __M_POLY_HPP__
#define __M_POLY_HPP__

#include "gxdlcode.h"

class PolyPoint_header
{
public:
  PolyPoint_header() { Clear(); }
  PolyPoint_header(double x, double y) { Set(x, y); }
  PolyPoint_header(const char *x, const char *y) { Set(x, y); }
  ~PolyPoint_header() { }
  PolyPoint_header(const PolyPoint_header& ob) { Copy(ob); }
  PolyPoint_header &operator=(const PolyPoint_header &ob);

public:
  void Clear();
  void Copy(const PolyPoint_header &ob);
  void Set(double x, double y);
  void Set(const char *x, const char *y);
  void PrintLat(gxString &str, int precision = 2);
  void PrintLon(gxString &str, int precision = 2);
  void PrintPoint(double point, gxString &str, int precision = 2);
  void PrintGroup(gxString &str, int precision = 2);
  void Print(int precision = 2);

public:
  double lat;
  double lon;
};


class PolygonWarning
{
public:
  PolygonWarning() { Clear(); }
  ~PolygonWarning() { }
  PolygonWarning(const PolygonWarning& ob) { Copy(ob); }
  PolygonWarning &operator=(const PolygonWarning &ob);

public:
  void Clear();
  void Copy(const PolygonWarning &ob);
  int parse_lat_lon();
  int parse_time_mot_loc();
  void Print();

public:
  // Products formatted with LAT...LON
  gxString lat_lon_string; // Un-edited coords string
  gxList<PolyPoint_header> polygon_coords; // The polygon box
  PolyPoint_header polygon_center;  // Average of poly coords

  // Products formatted with TIME...MOT...LOC
  gxString time_mot_loc_string; // Un-edited time, location, motion

  gxString centroid_time;   // GMT time
  gxString centroid_motion; // Motion in DEG
  gxString centroid_speed;  // Speed in KT
  gxList<PolyPoint_header> centroid_coords; // List of centorid coordinated
  PolyPoint_header centroid_center;         // Average of centroid coords
  PolyPoint_header centroid_arrow_top_point;    //        - 
  PolyPoint_header centroid_arrow_line;         // --------|> [Storm center]
  PolyPoint_header centroid_arrow_bottom_point; //        -
};


#endif // __M_POLY_HPP__
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
 
