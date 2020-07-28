// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- // 
// C++ Source Code File Name: m_prod.cpp
// Compiler Used: MSVC, GCC
// Produced By: Douglas.Gaer@noaa.gov
// File Creation Date: 01/25/2008
// Date Last Modified: 06/01/2009
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

Product class
*/
// ----------------------------------------------------------- // 
#include "gxdlcode.h"

TextProduct_header &TextProduct_header::operator=(const TextProduct_header &ob)
{
  if(&ob != this) Copy(ob);
  return *this;
}

void TextProduct_header::Copy(const TextProduct_header &ob)
{
  Clear();
  wmo_hdr_string = ob.wmo_hdr_string;
  pil = ob.pil;
  name = ob.name;
  issue_office = ob.issue_office;
  issue_time = ob.issue_time;
  num_ugc_segments = ob.num_ugc_segments;

  gxListNode<gxString> *gtext_sptr;
  gtext_sptr = ob.global_text.GetHead();
  while(gtext_sptr) {
    global_text.Add(gtext_sptr->data);
    gtext_sptr = gtext_sptr->next;
  }

  gxListNode<UGC> *uptr;
  uptr = ob.ugc_list.GetHead();
  while(uptr) {
    ugc_list.Add(uptr->data);
    uptr = uptr->next;
  }
}

void TextProduct_header::Clear()
{
  wmo_hdr_string.Clear();
  pil.Clear();
  name.Clear();
  issue_office.Clear();
  issue_time.Clear();
  ugc_list.ClearList();
  num_ugc_segments = 0;
  global_text.ClearList();
}

TextProduct &TextProduct::operator=(const TextProduct &ob)
{
  if(&ob != this) Copy(ob);
  return *this;
}

void TextProduct::Copy(const TextProduct &ob)
{
  product_hdr = ob.product_hdr;
  error_string = ob.error_string;

  gxListNode<gxString> *sptr;
  sptr = ob.product.GetHead();
  while(sptr) {
    product.Add(sptr->data);
    sptr = sptr->next;
  }
}

void TextProduct::Clear()
{
  product_hdr.Clear();
  product.ClearList();
  error_string.Clear();
}

int TextProduct::TestProductTime(const char *linebuf)
{
  // Parse the issue time
  gxString lbuf = linebuf;
  lbuf.TrimLeadingSpaces(); lbuf.TrimTrailingSpaces();
  lbuf.FilterChar('\n'); lbuf.FilterChar('\r');

  // Remove any double, triple, or quad spaces
  lbuf.ReplaceString("  ", " ");
  lbuf.ReplaceString("   ", " ");
  lbuf.ReplaceString("     ", " ");
  lbuf.ReplaceString("      ", " ");

  int i;
  gxString min_time_str = "HMM AM EST SUN JAN D YYYY";

  if(lbuf.length() < min_time_str.length()) return 0;

  gxString AMPM1 = lbuf.Mid(4, 2);
  gxString AMPM2 = lbuf.Mid(5, 2);
  
  if(((AMPM1 == "AM") || (AMPM1 == "PM")) || 
     ((AMPM2 == "AM") || (AMPM2 == "PM"))) {
    
    int has_day =0;
    for(i = 0; i < 7; i++) {
      if(lbuf.Find(System_weekdays[i]) != -1) {
	has_day = 1;
	break;
      }
    }
    int has_month = 0;
    for(i = 0; i < 12; i++) {
      if(lbuf.Find(System_months[i]) != -1) {
	has_month = 1;
	break;
      }
    }
    if(has_day && has_month) {
      product_hdr.issue_time.time_string = lbuf;
      return parse_product_time_header(product_hdr.issue_time);
    }
  }

  return 0;
}

int TextProduct::parse_product_time_header(TextProduct_time_header &th)
{
  return ParseTextProductTime(th, error_string);
}

void TextProduct::PrintTimeHeader(TextProduct_time_header &th)
{
  if(th.time_string.is_null()) return;

  GXSTD::cout << "Time string: "<< th.time_string.c_str() << "\n" << flush;
  GXSTD::cout << "Timezone: "<< th.time_zone.c_str() << "\n" << flush;
  GXSTD::cout << "Year: "<< th.year.c_str() << "\n" << flush;
  GXSTD::cout << "Month: "<< th.month.c_str() << "\n" << flush;
  GXSTD::cout << "Day: "<< th.day.c_str() << "\n" << flush;
  GXSTD::cout << "Hour: "<< th.hour.c_str() << "\n" << flush;
  GXSTD::cout << "Minute: "<< th.minute.c_str() << "\n" << flush;
  GXSTD::cout << "AMPM: " << th.AMPM.c_str() << "\n" << flush;
  GXSTD::cout << "Weekday: " << th.wkday.c_str() << "\n" << flush;
  GXSTD::cout << "Elasped time: " << th.elapsed_time  << "\n" << flush;

  if(th.elapsed_time > 1) {
    SysTime systime;
    gxString timetest = systime.MakeGMTDateTime(th.elapsed_time);
    GXSTD::cout << "Elasped time string: " << timetest.c_str()  << "\n" << flush;
  }
}

int TextProduct::LoadProduct(const char *fname)
{
  if(debug) NT_print("Loading product", fname);

  error_string.Clear();
  product.ClearList();

  if(!fname) {
    error_string << clear << "Text product error: " << "null file name passed to LoadProduct()";
    return 0;
  }

  char linebuf[1024];
  int num_lines = 0;
  DiskFileB dfile; // Current product file

  // Open the current product file
  dfile.df_Open(fname);
  if(!dfile) {
    error_string << clear << "Text product error: " << "Cannot open file " << fname;
    return 0;
  }

  while(!dfile.df_EOF()) {
    // Get each line of the file and trim the line feeds
    dfile.df_GetLine(linebuf, sizeof(linebuf), '\n');
    if(dfile.df_GetError() != DiskFileB::df_NO_ERROR) {
      if(num_lines == 0) num_lines++;
      error_string << clear << "Text product error: " << "Fatal read error on line " 
		   << num_lines << " " << fname;

      return 0;
    }

    num_lines++;
    gxString sbuf = linebuf;

    // START FILTERS - Add all line filters here
    sbuf.FilterChar('\r'); // Remove any DOS formatting from text  
    sbuf.FilterChar('\n');
    // END FILTERS

    // Convert in memory copy to a UNIX format
    sbuf << "\n";

    // Add line to the in memory copy
    product.Add(sbuf);
  }

  // Close all open product file
  dfile.df_Close();

  if(debug) NT_print("Product load complete", fname);
  return 1;
}

int TextProduct::ParseProduct(const char *fname)
{
  if(!LoadProduct(fname)) return 0;
  return ParseProduct();
}

int TextProduct::ParseWMOHeader()
{
  error_string.Clear();

  if(product.IsEmpty()) {
    error_string << clear << "Text product error: " << "No product is loaded";
    return 0;
  }

  gxListNode<gxString> *ptr = product.GetHead();

  char words[MAXWORDS][MAXWORDLENGTH];
  int numwords;
  gxString sbuf;
  
  product_hdr.pil.Clear();
  product_hdr.wmo_hdr_string.Clear();

  // Find the WMO header and product name
  while(ptr) {
    sbuf = ptr->data;
    sbuf.TrimLeading(' '); sbuf.TrimTrailing(' ');    
    sbuf.TrimTrailing('\n'); // Dump the end of line    

    // Look to AFOS teletype headers
    gxString ZCZC = sbuf.Left(4);
    if(ZCZC == "ZCZC") {
      // ZCZC MIASVRMFL DEF 
      gxString afos_pil = sbuf;
      // The last letter of the pil can be space padded
      afos_pil.ReplaceString("  ", " "); 
      if(parse(afos_pil.c_str(), words, &numwords, ' ') == 0) {
	if(numwords == 3) {
	  if(*words[1]) {
	    product_hdr.pil = words[1];
	  }
	}
      }
    }
    if(parse(sbuf.c_str(), words, &numwords, ' ') == 0) {
      if((numwords == 3) || (numwords == 4)) {
	// TTAAii CCCC YYGGgg (BBB)
	// YY = day
	// GG = hour
	// gg = minute
	gxString TTAAii = words[0];
	gxString CCCC = words[1];
	gxString DDHHMM = words[2];
	if((CCCC.length() == 4) && (DDHHMM.length() == 6)) {
	  gxString DD = DDHHMM.Left(2);
	  gxString HH = DDHHMM.Mid(2, 2);
	  gxString MM = DDHHMM.Right(2);
	  if((DD.Atoi() < 1) || (DD.Atoi() > 31) || 
	     (HH.Atoi() < 0) || (HH.Atoi() > 60) ||  
	     (MM.Atoi() < 0) || (MM.Atoi() > 60)) {
	    // Keep continuing until we find the WMO header
	    ptr = ptr->next;
	    continue;
	  }
	  // We found a valid WMO header
	  product_hdr.wmo_hdr_string << clear << TTAAii << " " << CCCC << " " <<  DDHHMM;

	  // We already have a PIL so return
	  if(!product_hdr.pil.is_null()) return 1; 

	  // Look for a valid 6 character product pil
	  ptr = ptr->next;
	  if(ptr) {
	    sbuf = ptr->data;
	    sbuf.TrimLeading(' '); sbuf.TrimTrailing(' ');    
	    sbuf.TrimTrailing('\n'); // Dump the end of line    
	    if(sbuf.length() != 6) {
	      error_string << clear << "Text product warning: " << "No PIL found after WMO header";
	    }
	    product_hdr.pil = sbuf; 
	    return 1;
	  }
	  else {
	    error_string << clear << "Text product warning: " << "No PIL found after WMO header";
	  }
	}
      }
    }
    ptr = ptr->next;
  }

  error_string << clear << "Text product error: " << "No WMO header found";
  return 0;
}

int TextProduct::ParseProductHeader()
{
  error_string.Clear();

  if(product.IsEmpty()) {
    error_string << clear << "Text product error: " << "No product is loaded";
    return 0;
  }

  gxString sbuf;
  gxListNode<gxString> *ptr = product.GetHead();
   
  while(ptr) {
    if(TestProductTime(ptr->data.c_str())) {

      product_hdr.name.Clear();
      product_hdr.issue_office.Clear();
      
      ptr = ptr->prev;
      if(ptr) {
	gxString NWS = ptr->data.Left(8);
	if(NWS == "NATIONAL") {
	  product_hdr.issue_office = ptr->data.c_str();
	  product_hdr.issue_office.TrimLeading(' '); 
	  product_hdr.issue_office.TrimTrailing(' ');    
	  product_hdr.issue_office.TrimTrailing('\n');
	  
	  ptr = ptr->prev;
	  if(ptr) {
	    product_hdr.name = ptr->data.c_str();
	    product_hdr.name.TrimLeading(' '); 
	    product_hdr.name.TrimTrailing(' ');    
	    product_hdr.name.TrimTrailing('\n');
	  }
	}
      }
      return 1;
    }

    // Advance to the next line
    ptr = ptr->next;
  }

  error_string << clear << "Text product error: " << "No product header found";
  return 0;
}

int TextProduct::ParseUGCSegments()
{
  error_string.Clear();

  if(product.IsEmpty()) {
    error_string << clear << "Text product error: " << "No product is loaded";
    return 0;
  }

  char words[MAXWORDS][MAXWORDLENGTH];
  int numwords;
  int i;
  gxString sbuf;
  product_hdr.ugc_list.ClearList();
  product_hdr.num_ugc_segments = 0;
  int ugc_incomplete = 0;
  UGC ugc;
  VTEC vtec;
  int ugc_line_number = 0;
  int line_number = 0;

  gxListNode<gxString> *ptr = product.GetHead();

  // 06/01/2009: Must account for text after product header and before the first VTEC segment.
  product_hdr.global_text.ClearList();
  int start_global = 0;
  int end_global = 0;

  // 06/04/2009: Move LAT/LON polygon parsing outside of loop for products without a TIME-MOT-LOC value
  int has_lat_lon = 0;
  int has_time_mot_loc = 0;

  while(ptr) {
    line_number++;

    if(TestProductTime(ptr->data.c_str())) {
      ugc.ugc_segment_time = product_hdr.issue_time;
      start_global = 1;
    }

    // Look for UGC line
    int continue_next_line = 0;

    if(ugc.TestUGCHeader(ptr->data.c_str(), continue_next_line)) {

      // 06/01/2009: We are in a VTEC segment so do not read any text between segments as 
      // 06/01/2009: affecting all ZONES.
      end_global = 1;

      ugc_incomplete = 1;
      ugc_line_number = line_number;
      ugc.ugc_segment_header << ptr->data;

      if(continue_next_line) {
	ptr = ptr->next;
	continue; // UGC header spans 2 or more lines
      }
      if(!ugc.error_string.is_null()) {
	error_string << clear << ugc.error_string.c_str();
	return -1;
      }
      // Found the UGC header look for a segmented time sequence
      gxString zone_table_str;
      gxString include_cities_str;
      int has_zone_info = 0;
      int has_include_cities = 0;
      ptr = ptr->next;

      // 06/04/2009: Move LAT/LON polygon parsing outside of loop for products without a TIME-MOT-LOC value
      has_lat_lon = 0;
      has_time_mot_loc = 0;

      // Record the location of end the header
      gxListNode<gxString> *ugc_end_of_header = ptr;

      while(ptr) {
	ugc.ugc_segment_header << ptr->data;

	if(TestProductTime(ptr->data.c_str())) {
	  // Found UGC segment ending with a timestame string
	  ugc.ugc_segment_time = product_hdr.issue_time;
	  has_zone_info = 1;
	  ugc_incomplete = 0;
	  break;
	}
	vtec.Clear(); // Reset the VTEC object before use
	if(vtec.TestVTECHeader(ptr->data.c_str())) {
	  // NOTE: Check errors when the list is processed
	  if(!vtec.error_string.is_null()) {
	    // Warn about the error only unless full debugging is enabled
	    error_string << clear << "Text product error: " 
			 << "Found bad VTEC header on line " 
			 << line_number;
	    if(debug) NT_printlines("Bad VTEC header", vtec.error_string.c_str());
	  }
	  ugc.vtec_list.Add(vtec);
	  // Update the end of the UGC header
	  ugc_end_of_header = ptr;
	}
	if(ptr->data[0] != '/') {
	  sbuf << clear << ptr->data;
	  sbuf.FilterChar('\n');
	  sbuf.FilterChar('\r');
	  sbuf.TrimLeadingSpaces(); sbuf.TrimTrailingSpaces();
	  int insert_zone = 1;
	  if(sbuf.is_null()) insert_zone = 0;
	  if(sbuf.Find(" - ") != -1) insert_zone = 0;
	  if(sbuf == product_hdr.name) insert_zone = 0;
	  if(sbuf == product_hdr.issue_office) insert_zone = 0;
	  if(insert_zone) {
	    zone_table_str << ptr->data;
	  }
	}

	ptr = ptr->next;

	if(ptr) {
	  sbuf = ptr->data;
	  sbuf.FilterChar('\n');
	  sbuf.FilterChar('\r');
	  sbuf.FilterChar(' ');
	  if(sbuf == "$$") {
	    // We went to the end of the segment so reset the pointer
	    ugc_incomplete = 0;
	    ptr = ugc_end_of_header; 
	    break; // Found the end of the segment
	  }
	}	  
      }
      
      if(!ugc.ugc_segment_time.time_string.is_null()) {
	ugc.ResetUGCTime(ugc.ugc_segment_time);
      }
      if((!zone_table_str.is_null()) && (has_zone_info)) {
	zone_table_str.FilterChar('\r');
	// 05/14/2008: Replace linefeeds with a space
	zone_table_str.ReplaceChar('\n', ' ');
	zone_table_str.TrimLeadingSpaces();
	zone_table_str.TrimTrailingSpaces();
	if(zone_table_str.Find("-") == -1)  has_zone_info = 0;

	if(has_zone_info) { // Look for included areas
	  if(zone_table_str[zone_table_str.length()-1] != '-') {
	    // We have text after the last zone
	    include_cities_str = zone_table_str;
	    zone_table_str.DeleteAfterLastIncluding("-");
	    include_cities_str.DeleteBeforeLastIncluding("-");
	    include_cities_str.DeleteBeforeIncluding("...");
	    if(include_cities_str.Find("...") != -1) has_include_cities = 1;
	  }
	}

	// Load the list of zone or county names found under UGC header
	if(has_zone_info) { 
	  zone_table_str.ReplaceString("-", "\t");
	  zone_table_str.ReplaceString("\t\t", "\t");
	  zone_table_str.TrimLeading('\t');
	  zone_table_str.TrimTrailing('\t');

	  gxListNode<gxString> *dzlptr = ugc.ugc_hdr.zone_list.GetHead();
	  int num_dynamic_zones = 0;
	  if(parse(zone_table_str.c_str(), words, &numwords, '\t') == 0) {
	    for(i = 0; i < numwords; i++) {
	      if(*words[i] != 0) {
		sbuf = words[i];
		sbuf.TrimLeadingSpaces();
		sbuf.TrimTrailingSpaces();
		ugc.zones_names.Add(sbuf);
		if((use_dynamic_zone_table) && (match_dynamic_zone_names)) {
		  if(dzlptr) {
		    if(num_dynamic_zones == i) {
		      // Ensure our zone numbers match the order of the zone names
		      gxString name_buf;
		      if(FindZoneName(dzlptr->data, name_buf)) {
			if(name_buf == dzlptr->data) { // This zone has no name
			  ZoneTableData zone_table_data;
			  zone_table_data.zone_number = dzlptr->data; 
			  if(debug) {
			    zone_table_data.zone_name << clear << sbuf << " - " << dzlptr->data;
			  }
			  else {
			    zone_table_data.zone_name << clear << sbuf;
			  }
			  zone_table_data.area_number = "0";
			  zone_table_data.area_name << clear 
						    << WarningTableCWA 
						    << " Zones and Counties"; 
			  // Assign the name
			  UpdateZoneName(dzlptr->data, zone_table_data);
			}
		      }
		      num_dynamic_zones++;
		      dzlptr = dzlptr->next;
		    }
		  }
		}
	      }
	    }
	  }
	}

	// Load the list of city names found under UGC header after zone names
	if(has_include_cities) {
	  include_cities_str.ReplaceString("...", "\t");
	  include_cities_str.ReplaceString("\t\t", "\t");
	  include_cities_str.TrimLeading('\t');
	  include_cities_str.TrimTrailing('\t');
	  if(parse(include_cities_str.c_str(), words, &numwords, '\t') == 0) {
	    for(i = 0; i < numwords; i++) {
	      if(*words[i] != 0) {
		sbuf = words[i];
		sbuf.TrimLeadingSpaces();
		sbuf.TrimTrailingSpaces();
		ugc.include_city_names.Add(sbuf);
	      }
	    }
	  }
	}
      }

      if(!ptr) {
	// We went to the end of the product so reset the product
	ptr = ugc_end_of_header;
	ugc_incomplete = 0;
      }

      // Skip to the next line
      ptr = ptr->next;
      if(!ptr) {
	error_string << clear << "Text product error: " 
		     << "Could not found end of UGC code in product";
	return -1;
      }

      while(ptr) {
	sbuf = ptr->data;
	sbuf.FilterChar('\n');
	sbuf.FilterChar('\r');
	sbuf.FilterChar(' ');
	if(sbuf == "$$") {
	  break; // Found the end of the segment
	}
	
	gxString lbuf1 = "LAT...LON ";
	gxString lbuf2 = "TIME...MOT...LOC ";

	if(ptr->data > lbuf1.length()) {
	  gxString lat_lon = ptr->data.Left(lbuf1.length()); 
	  if(lat_lon == lbuf1) {
	    has_lat_lon = 1;
	    ugc.polygon_warning.lat_lon_string << ptr->data;
	    ptr = ptr->next;
	    if(ptr) {
	      // Test the next line here
	      if(ptr->data > lbuf2.length()) {
		gxString time_mot_loc = ptr->data.Left(lbuf2.length());
		if(time_mot_loc != lbuf2) {
		  if((ptr->data[0] != ' ') && (ptr->data[2] != ' ')) {
		    ptr = ptr->prev;
		    break; // Line is not valid so reset pointer and continue
		  }
		}
	      }
	    }
	    while(ptr) {
	      // 08/20/2008: Account for flood products with no time_mot_loc string
	      sbuf = ptr->data;
	      sbuf.FilterChar('\n');
	      sbuf.FilterChar('\r');
	      sbuf.FilterChar(' ');
	      if(sbuf == "$$") {
		break; // Found the end of the segment
	      }
	      if(ptr->data > lbuf2.length()) {
		gxString time_mot_loc = ptr->data.Left(lbuf2.length());
		if(time_mot_loc == lbuf2) {
		  has_time_mot_loc = 1;
		  ugc.polygon_warning.time_mot_loc_string << ptr->data;
		  ptr = ptr->next;
		  while(ptr) {
		    if((ptr->data[0] == ' ') && (ptr->data[2] == ' ')) {
		      ugc.polygon_warning.time_mot_loc_string << ptr->data;
		    }
		    else {
		      break;
		    }
		    ptr = ptr->next;
		  }
		  break;
		}
	      }
	      if(!ptr) break; // Ensure the pointer is still valid
	      ugc.polygon_warning.lat_lon_string << ptr->data;
	      ptr = ptr->next;
	    }
	  }
	}
	if(!ptr) break; // Ensure the pointer is still valid

	// Add the UGC text 
	ugc.ugc_segment_text.Add(ptr->data);

	ptr = ptr->next; // Continue to the end of the product
      }

      // 06/04/2009: Move LAT/LON polygon parsing outside of loop for products without a TIME-MOT-LOC value
      if(has_lat_lon) {
	has_lat_lon = 0;
	if(!ugc.polygon_warning.parse_lat_lon()) {
	  error_string << clear << "Text product error: " 
		       << "Error parsing polygon warning"; 
	}
      }
      if(has_time_mot_loc) {
	has_time_mot_loc = 0; 
	if(!ugc.polygon_warning.parse_time_mot_loc()) {
	  error_string << clear << "Text product error: " 
		       << "Error parsing warning time motion and location"; 
	}
      }

      product_hdr.num_ugc_segments++;      

      product_hdr.ugc_list.Add(ugc); // Add this segment to the list
      ugc.Clear(); // Reset the buffer object
    }
    else {
      // 06/01/2009: We have text after the product header and before the first segment
      if((start_global) && (!end_global)) {
	if(!TestProductTime(ptr->data.c_str())) { // Skip the product header time stamp
	  product_hdr.global_text.Add(ptr->data);
	}
      }
    }

    if(!ptr) break; // Ensure the pointer is still valid

    // Continue to the the next line
    ptr = ptr->next;
  }

  if(ugc_incomplete == 1) {
    error_string << clear << "Text product error: " 
		 << "Found incomplete UGC header on line " 
		 << ugc_line_number;
    return -1;
  }

  return 1;
}

int TextProduct::ParseProduct()
{
  error_string.Clear();

  if(product.IsEmpty()) {
    error_string << clear << "Text product error: " << "No product is loaded";
    return 0;
  }

  if(debug) NT_print("Pasring product loaded in non-contiguous memory");

  int has_wmo_hdr = 1;
  int has_product_hdr = 1;
  int has_ugc_segments = 1;

  if(debug) NT_print("Pasring WMO and PIL headers");
  if(!ParseWMOHeader()) {
    if(error_string.is_null()) {
      error_string << clear << "Text product warning: " << "No WMO header found";
    }
    has_wmo_hdr = 0;
  }

  if(debug) NT_print("Pasring NWS text product header");
  if(!ParseProductHeader()) {
    if(error_string.is_null()) {
      error_string << clear << "Text product warning: " << "No NWS product header found";
    }
    has_product_hdr = 0;
  }

  if(debug) NT_print("Pasring UGC and VTEC elements");
  int rv = ParseUGCSegments();
  if(!rv) {
    if(error_string.is_null()) {
      error_string << clear << "Text product warning: " << "No UGC header found";
    }
    has_ugc_segments = 0;
    if(debug) NT_print("No UGC elements found in product");
  }
  if(rv < 0) {
    //  This is a UGC product with an error reading the UGC header and/or segment
    if(error_string.is_null()) {
      error_string << clear << "Text product error: " << "Bad UGC segment or product";
    }
    if(debug) NT_print("Bad or missing UGC elemnets found in product");
    return 0;
  }

  if((!has_wmo_hdr) && (!has_product_hdr) && (!has_ugc_segments)) {
    error_string << clear << "Text product error: " << "No NWS text product found in file";
    return 0;
  }

  if(!has_ugc_segments) {
    // TODO: Add code to parse and store non UGC/VTEC products
  }


  if(debug) NT_print("Pasring complete");
  return 1;
}

void TextProduct::PrintProductInfo() 
{
  GXSTD::cout << "\n" << flush;
  GXSTD::cout << "WMO header: " << product_hdr.wmo_hdr_string.c_str() << "\n" << flush;
  GXSTD::cout << "PIL: " << product_hdr.pil.c_str() << "\n" << flush;
  GXSTD::cout << "Name: " << product_hdr.name.c_str() << "\n" << flush;
  GXSTD::cout << "Issue Office: " << product_hdr.issue_office.c_str() << "\n" 
	    << flush;
  GXSTD::cout << "Issue time: " << product_hdr.issue_time.time_string.c_str() 
	    << "\n" << flush;

  if(!product_hdr.ugc_list.IsEmpty()) {
    gxListNode<UGC> *uptr = product_hdr.ugc_list.GetHead();
    while(uptr) {
      uptr->data.PrintUGCHeader();
      uptr = uptr->next;
    }
  }
}
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
