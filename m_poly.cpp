// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- // 
// C++ Source Code File Name: m_poly.cpp
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

Polygon warning class
*/
// ----------------------------------------------------------- // 
#include "gxdlcode.h"

PolyPoint_header &PolyPoint_header::operator=(const PolyPoint_header &ob)
{
  if(&ob != this) Copy(ob);
  return *this;
}

void PolyPoint_header::Clear()
{
  lat = 0;
  lon = 0;
}

void PolyPoint_header::Copy(const PolyPoint_header &ob)
{
  lat = ob.lat;
  lon = ob.lon;
}

void PolyPoint_header::Set(double x, double y)
{
  Clear();
  lat = x;
  lon = y;
}

void PolyPoint_header::Set(const char *x, const char *y)
{
  Clear();

  // Check for null string pointers
  if(!x) return;
  if(!y) return;

  gxString lat_s = x;
  gxString lon_s = y;

  if(lat_s.Find(".") == -1) {
    if(lat_s[0] == '-') {
      if(lat_s.length() >= 4) lat_s.InsertAt(3, ".");
    }
    else {
      if(lat_s.length() >= 3) lat_s.InsertAt(2, ".");
    }
  }
  if(lon_s.Find(".") == -1) {
    if(lon_s[0] == '-') {
      if(lon_s.length() >= 4) lon_s.InsertAt(3, ".");
    }
    else {
      if(lon_s.length() >= 3) lon_s.InsertAt(2, ".");
    }
  }
  
  lat = lat_s.Atof();
  lon = lon_s.Atof();
}

void PolyPoint_header::PrintLat(gxString &str, int precision)
{
  PrintPoint(lat, str, precision);
}

void PolyPoint_header::PrintLon(gxString &str, int precision)
{
  PrintPoint(lon, str, precision);
}

void PolyPoint_header::PrintPoint(double point, gxString &str, int precision)
{
  str.Clear();
  str.Reset();
  str.Precision(precision);
  str.SetFill('0');
  str << point;
  if(precision > 2) {
    str.TrimTrailing('0');
  }
}


void PolyPoint_header::PrintGroup(gxString &str, int precision)
{
  str.Clear();
  gxString lat_s, lon_s;
  PrintPoint(lat, lat_s, precision);
  PrintPoint(lon, lon_s, precision);
  str << lat_s << ", " << lon_s;
}

void PolyPoint_header::Print(int precision)
{
  gxString sbuf;
  PrintGroup(sbuf, precision);
  GXSTD::cout << sbuf.c_str() << GXSTD::flush;
}

PolygonWarning &PolygonWarning::operator=(const PolygonWarning &ob)
{
  if(&ob != this) Copy(ob);
  return *this;
}

void PolygonWarning::Clear()
{
  lat_lon_string.Clear();
  polygon_coords.ClearList();
  time_mot_loc_string.Clear();
  centroid_time.Clear();
  centroid_motion.Clear();
  centroid_coords.ClearList();
  centroid_speed.Clear();
  polygon_center.Clear();
  centroid_center.Clear();
  centroid_arrow_line.Clear();
  centroid_arrow_top_point.Clear();
  centroid_arrow_bottom_point.Clear();
}

void PolygonWarning::Copy(const PolygonWarning &ob)
{
  Clear();
  
  lat_lon_string = ob.lat_lon_string;
  time_mot_loc_string = ob.time_mot_loc_string;
  centroid_time = ob.centroid_time;
  centroid_motion = ob.centroid_motion;
  centroid_speed = ob.centroid_speed;
  polygon_center = ob.polygon_center;
  centroid_center = ob.centroid_center;
  centroid_arrow_line = ob.centroid_arrow_line;
  centroid_arrow_top_point = ob.centroid_arrow_top_point;
  centroid_arrow_bottom_point = ob.centroid_arrow_bottom_point;

  gxListNode<PolyPoint_header> *ptr;
  ptr = ob.centroid_coords.GetHead();
  while(ptr) {
    centroid_coords.Add(ptr->data);
    ptr = ptr->next;
  }
  ptr = ob.polygon_coords.GetHead();
  while(ptr) {
    polygon_coords.Add(ptr->data);
    ptr = ptr->next;
  }
}

int PolygonWarning::parse_lat_lon()
{
  if(lat_lon_string.is_null()) {
    return 0;
  }

  // LAT...LON 2596 8039 2597 8068 2599 8069 2598 8094
  //       2634 8093 2665 8094 2663 8038 2631 8037

  gxString lbuf = lat_lon_string;

  lbuf.DeleteBeforeIncluding("LAT...LON");
  lbuf.ReplaceChar('\n', ' ');
  lbuf.ReplaceChar('\r', ' ');
  lbuf.TrimLeadingSpaces();
  lbuf.TrimTrailingSpaces();
  lbuf.ReplaceString("               ", " ");
  lbuf.ReplaceString("              ", " ");
  lbuf.ReplaceString("             ", " ");
  lbuf.ReplaceString("            ", " ");
  lbuf.ReplaceString("           ", " ");
  lbuf.ReplaceString("          ", " ");
  lbuf.ReplaceString("         ", " ");
  lbuf.ReplaceString("        ", " ");
  lbuf.ReplaceString("       ", " ");
  lbuf.ReplaceString("     ", " ");
  lbuf.ReplaceString("    ", " ");
  lbuf.ReplaceString("   ", " ");
  lbuf.ReplaceString("  ", " ");
  
  char words[MAXWORDS][MAXWORDLENGTH];
  int numwords;
  int i;
  gxString lat_s, lon_s;
  PolyPoint_header poly_point;


  // 2596 8039 2597 8068 2599 8069 2598 8094 2634 8093 2665 8094 2663 8038 2631 8037
  if(parse(lbuf.c_str(), words, &numwords, ' ') == 1) {  
    return 0;
  }

  polygon_coords.ClearList();

  for(i = 0; i < numwords; i ++) {
    if(*words[i] != 0) {
      lat_s = words[i];
      FilterStringValue(lat_s);
    }
    i++;
    if(i >= numwords) break;
    if(*words[i] != 0) {
      lon_s = words[i];
      FilterStringValue(lon_s);
      lon_s.InsertAt(0, "-");
    }
    poly_point.Set(lat_s.c_str(), lon_s.c_str());
    polygon_coords.Add(poly_point);
  }

  int num_lats = 0;
  int num_lons = 0;
  double sum_lats = 0;
  double sum_lons = 0;

  gxListNode<PolyPoint_header> *pptr = polygon_coords.GetHead();
  while(pptr) {
    num_lats++;
    sum_lats += pptr->data.lat;
    pptr = pptr->next;
  }
  pptr = polygon_coords.GetHead();
  while(pptr) {
    num_lons++;
    sum_lons += pptr->data.lon;
    pptr = pptr->next;
  }

  if((num_lats == 0) || (num_lons == 0)) {
    return 0;
  }

  double center_x = sum_lats / num_lats;
  double center_y = sum_lons / num_lats;
  polygon_center.Set(center_x, center_y);

  return 1;
}

int PolygonWarning::parse_time_mot_loc()
{
  if(time_mot_loc_string.is_null()) {
    return 0;
  }

  // TIME...MOT...LOC 2039Z 273DEG 17KT 2656 8084 2644 8083
  //        2633 8082 2620 8083 2607 8084

  gxString lbuf = time_mot_loc_string;
  
  lbuf.DeleteBeforeIncluding("TIME...MOT...LOC");
  lbuf.ReplaceChar('\n', ' ');
  lbuf.ReplaceChar('\r', ' ');
  lbuf.TrimLeadingSpaces();
  lbuf.TrimTrailingSpaces();
  lbuf.ReplaceString("               ", " ");
  lbuf.ReplaceString("              ", " ");
  lbuf.ReplaceString("             ", " ");
  lbuf.ReplaceString("            ", " ");
  lbuf.ReplaceString("           ", " ");
  lbuf.ReplaceString("          ", " ");
  lbuf.ReplaceString("         ", " ");
  lbuf.ReplaceString("        ", " ");
  lbuf.ReplaceString("       ", " ");
  lbuf.ReplaceString("     ", " ");
  lbuf.ReplaceString("    ", " ");
  lbuf.ReplaceString("   ", " ");
  lbuf.ReplaceString("  ", " ");
  
  char words[MAXWORDS][MAXWORDLENGTH];
  int numwords;
  int i;
  gxString lat_s, lon_s;
  PolyPoint_header poly_point;

  // 2039Z 273DEG 17KT 2656 8084 2644 8083 2633 8082 2620 8083 2607 8084
  if(parse(lbuf.c_str(), words, &numwords, ' ') == 1) {  
    return 0;
  }
  
  centroid_time.Clear();
  centroid_motion.Clear();
  centroid_speed.Clear();
  centroid_coords.ClearList();


  for(i = 0; i < numwords; i ++) {
    if(i == 0) {
      if(*words[0] != 0) {
	centroid_time = words[0];
	centroid_time.FilterChar('Z');
	FilterStringValue(centroid_time);
      }
    }
    else if(i == 1) {
      if(*words[1] != 0) {
	centroid_motion = words[1];
	centroid_motion.FilterString("DEG");
	FilterStringValue(centroid_motion);
      }
    }
    else if(i == 2) {
      if(*words[2] != 0) {
	centroid_speed = words[2];
	centroid_speed.FilterString("KT");
	FilterStringValue(centroid_speed);
      }
    }
    else {
      if(*words[i] != 0) {
	lat_s = words[i];
	FilterStringValue(lat_s);
      }
      i++;
      if(i >= numwords) break;
      if(*words[i] != 0) {
	lon_s = words[i];
	FilterStringValue(lon_s);
	lon_s.InsertAt(0, "-");
      }
      poly_point.Set(lat_s.c_str(), lon_s.c_str());
      centroid_coords.Add(poly_point);

    }
  }

  int num_lats = 0;
  int num_lons = 0;
  double sum_lats = 0;
  double sum_lons = 0;

  gxListNode<PolyPoint_header> *pptr = centroid_coords.GetHead();
  while(pptr) {
    num_lats++;
    sum_lats += pptr->data.lat;
    pptr = pptr->next;
  }
  pptr = centroid_coords.GetHead();
  while(pptr) {
    num_lons++;
    sum_lons += pptr->data.lon;
    pptr = pptr->next;
  }

  if((num_lats == 0) || (num_lons == 0)) {
    return 0;
  }

  double center_x = sum_lats / num_lats;
  double center_y = sum_lons / num_lats;
  centroid_center.Set(center_x, center_y);

  double polyline1_lat = center_x + ((centroid_speed.Atof() / 200) * cos((centroid_motion.Atof() + 180) * .01745));
  double polyline1_lon = center_y + ((centroid_speed.Atof() / 200) * sin((centroid_motion.Atof() + 180) * .01745));
  centroid_arrow_line.Set(polyline1_lat, polyline1_lon);

  double arrow1_lat = polyline1_lat + (.05 * cos((centroid_motion.Atof() + 45) * .01745));
  double arrow1_lon = polyline1_lon + (.05 * sin((centroid_motion.Atof() + 45) * .01745));
  centroid_arrow_top_point.Set(arrow1_lat, arrow1_lon);

  double arrow2_lat = polyline1_lat + (.05 * cos((centroid_motion.Atof() - 45) * .01745));
  double arrow2_lon = polyline1_lon + (.05 * sin((centroid_motion.Atof() - 45) * .01745));
  centroid_arrow_bottom_point.Set(arrow2_lat, arrow2_lon);

  return 1;
}

void PolygonWarning::Print()
{
  GXSTD::cout << "Polygon = ";
  gxListNode<PolyPoint_header> *pptr = polygon_coords.GetHead();
  while(pptr) {
    GXSTD::cout << "(";
    pptr->data.Print();
    GXSTD::cout << ")";
    pptr = pptr->next;
    if(pptr) GXSTD::cout << ",";
  }
  GXSTD::cout << "\n" << GXSTD::flush;
  GXSTD::cout << "Polygon Center point = ";
  polygon_center.Print(12);
  GXSTD::cout << "\n" << GXSTD::flush;
  GXSTD::cout << "Centroid time = " << centroid_time.c_str() << " GMT" << "\n";
  GXSTD::cout << "Motion = " << centroid_motion.c_str() << " DEG" << "\n";
  GXSTD::cout << "Speed = " << centroid_speed.c_str() << " KT" << "\n";
  GXSTD::cout << "Centroid = ";
  pptr = centroid_coords.GetHead();
  while(pptr) {
    GXSTD::cout << "(";
    pptr->data.Print();
    GXSTD::cout << ")";
    pptr = pptr->next;
    if(pptr) GXSTD::cout << ",";
  }
  GXSTD::cout << "\n" << GXSTD::flush;
  GXSTD::cout << "Centroid Center point = ";
  centroid_center.Print(12);
  GXSTD::cout << "\n" << GXSTD::flush;
  GXSTD::cout << "Centroid arrow line = ";
  centroid_arrow_line.Print(12);
  GXSTD::cout << "\n" << GXSTD::flush;
  GXSTD::cout << "Centroid arrow line top = ";
  centroid_arrow_top_point.Print(12);
  GXSTD::cout << "\n" << GXSTD::flush;
  GXSTD::cout << "Centroid arrow line bottom = ";
  centroid_arrow_bottom_point.Print(12);
  GXSTD::cout << "\n" << GXSTD::flush;
}
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
