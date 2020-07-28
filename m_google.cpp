// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- // 
// C++ Source Code File Name: m_google.cpp
// Compiler Used: MSVC, GCC
// Produced By: Douglas.Gaer@noaa.gov
// File Creation Date: 01/25/2008
// Date Last Modified: 04/23/2013
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

Code used to generate Google map pages for polygon warnings
*/
// ----------------------------------------------------------- // 
#include "gxdlcode.h"

int generate_google_map_page(TextProduct &text_product)
{
  NT_print("Generating Google map Web page");

  gxString sbuf;

  sbuf << clear << WebpageOutputDirectory;
#if defined (__WIN32__)
  sbuf.TrimTrailing('\\');
#else
  sbuf.TrimTrailing('/');
#endif
  if(!futils_exists(sbuf.c_str())) {
    if(futils_mkdir(sbuf.c_str()) != 0) {
      NT_print("Error making Web page output directory", sbuf.c_str()); 
      NT_print("No Web page will be generated");
      GXSTD::cout << "Error-mkdir" << "\n" << GXSTD::flush;
      return 0;
    }
  }
  
  if(text_product.product.IsEmpty()) {
    NT_print("Error no text product is loaded");
    NT_print("No Web page will be generated");
    GXSTD::cout << "Error-noproduct" << "\n" << GXSTD::flush;
    return 0;
  }
  if(text_product.product_hdr.wmo_hdr_string.is_null()) {
    NT_print("Error text product does not contain a valid WMO header");
    NT_print("No Web page will be generated");
    GXSTD::cout << "Error-WMOheader" << "\n" << GXSTD::flush;
    return 0;
  }
  if(text_product.product_hdr.pil.is_null()) {
    NT_print("Error text product does not contain a valid PIL header");
    NT_print("No Web page will be generated");
    GXSTD::cout << "Error-PIL" << "\n" << GXSTD::flush;
    return 0;
  }
  
  NT_print("Reading product", text_product.product_hdr.pil.c_str());
  gxString issued_by = text_product.product_hdr.issue_office;
  issued_by.ReplaceString("NATIONAL WEATHER SERVICE", "NWS");
  NT_print(issued_by.c_str(), text_product.product_hdr.name.c_str());
  gxString issue_time = text_product.product_hdr.issue_time.time_string;
  NT_print("Issue time", issue_time.c_str());
  
  if(text_product.product_hdr.ugc_list.IsEmpty()) {
    NT_print("Product does not contain any UGC segments");
    NT_print("No Web page will be generated");
    GXSTD::cout << "Error-UGCsegments" << "\n" << GXSTD::flush;
    return 0;
  }

  gxListNode<UGC> *uptr = text_product.product_hdr.ugc_list.GetHead();

  // Read all the UGC segments
  int num_pages = 0;
  int current_ugc_segment = 0;
  gxString webpage;

  while(uptr) {
    // Start processing this UGC segment
    current_ugc_segment++;

    gxString fname;
    gxString webpage;
    gxString pagefilename;
    gxString js_file;
    if(!generate_google_map_page(text_product, uptr->data, current_ugc_segment, webpage, pagefilename, fname, js_file)) {
      uptr = uptr->next;
      continue;
    }

    if(WriteWebPageFile(fname, pagefilename, webpage)) {
      num_pages++; 
    }

    if(gen_js_files) {
      gxString js_file_name = fname;
      js_file_name.DeleteAfterLastIncluding(".");
      gxString js_pagefilename = pagefilename;
      js_pagefilename.DeleteAfterLastIncluding(".");
      js_file_name << js_file_extension;
      js_pagefilename << js_file_extension;
      WriteWebPageFile(js_file_name, js_pagefilename, js_file);
    }

    uptr = uptr->next;
  }

  if(num_pages == 0) {
    NT_print("Error generating Web page map files");
    NT_print("No Web page were made");
    GXSTD::cout << "Error-nopagesmade" << "\n" << GXSTD::flush;
    return 0;
  }

  if(num_pages == 1) {
    sbuf << clear << "Made " << num_pages << " Wep page";
  }
  else {
    sbuf << clear << "Made " << num_pages << " Wep pages";
  }
  NT_print(sbuf.c_str());

  return 1;
}

int generate_google_map_page(TextProduct &text_product, UGC &ugc_segment, 
			     int current_ugc_segment, gxString &webpage, 
			     gxString &pagefilename, gxString &fname,
			     gxString &js_file)
{
  webpage.Clear();
  pagefilename.Clear();
  fname.Clear();
  js_file.Clear();

  char words[MAXWORDS][MAXWORDLENGTH];
  int numwords;
  int i;
  char words_nest[MAXWORDS][MAXWORDLENGTH];
  int numwords_nest;
  int is_expired = 0;
  int is_followup_statement = 0;
  gxString vtec_action;
  gxString sbuf, inputbuf;
  gxString followup_org_pil;

  gxString product = text_product.product_hdr.pil;
  if(product.length() == 9) product.DeleteAt(0, 3);
  if(product.length() == 6) product.DeleteAt(3, 3);

  gxListNode<gxString> *flptr = Followup_statement_pils_table.GetHead();
  while(flptr) {
    if(product == flptr->data) {
      is_followup_statement = 1;
      // 06/02/2008: Keep a record of the orginal follow-up PIL 
      followup_org_pil = product;
      break;
    }
    flptr = flptr->next;
  }
  
  // Always use the first VTEC header for short-fused warnings
  gxListNode<VTEC> *vptr = ugc_segment.vtec_list.GetHead();

  // 06/02/2008: Reset the 3 letter product and pil for follow statements 
  if((is_followup_statement) && (vptr)) {
    gxString new_pil;
    // Get the PIL from the VTEC ID
    gxListNode<gxString> *vtablep;
    // Parse pp table
    vtablep = PVTEC_phenomena_table.GetHead();
    while(vtablep) {
      sbuf = vtablep->data;
      gxString pp = sbuf.Left(2);
      if(pp == vptr->data.Phenomena()) {
	gxListNode<gxString> *flpilptr = Followup_phenomena_code_to_pil_table.GetHead();
	while(flpilptr) {
	  gxString pp_pil = flpilptr->data.Left(2);
	  if(pp == pp_pil) {
	    gxString pp_new_pil = flpilptr->data.Right(3);
	    new_pil = pp_new_pil;
	    new_pil.ToUpper();
	    break;
	  }
	  flpilptr = flpilptr->next;
	}
	break;
      }
      vtablep = vtablep->next;
    }

    if(!new_pil.is_null()) product = new_pil; 
  }

  webpage.Clear();
  is_expired = 0;

  if(ugc_segment.vtec_list.IsEmpty()) {
    NT_print("UGC segment does not contain any VTEC headers");
    // 12/11/2008: TODO: Add fix for site that have polygons in non-vtec products
    if(!ugc_segment.polygon_warning.polygon_coords.IsEmpty()) {
      NT_print("UGC segment has polygon coordinates but no VTEC header");
    }
    sbuf << clear << "Skipping Google map page for UGC segment " << current_ugc_segment;
    NT_print (sbuf.c_str());
    return 0;
  } 

  if(ugc_segment.polygon_warning.polygon_coords.IsEmpty()) {
    NT_print("UGC segment does not contain any polygon coordinates");
    sbuf << clear << "Skipping Google map page for UGC segment " << current_ugc_segment;
    NT_print (sbuf.c_str());
    return 0;
  }

  gxString issued_by = text_product.product_hdr.issue_office;
  issued_by.ReplaceString("NATIONAL WEATHER SERVICE", "NWS");

  gxString color = GoogleMapsDefaultTextColor;
  gxString zoom = GoogleMapsDefaultZoom;
  gxString type = text_product.product_hdr.name;
  gxString iss = text_product.product_hdr.issue_time.time_string;

  color = GoogleMapsDefaultTextColor;
  if(GoogleMapsTextColorByProduct.IFind(product) != -1) {
    if(parse(GoogleMapsTextColorByProduct.c_str(), words, &numwords, '|') == 1) {
      NT_print("Error parsing text color by product table");
      NT_print(GoogleMapsTextColorByProduct.c_str());
    }
    else {
      for(i = 0; i < numwords; i ++) {
	if(*words[i] != 0) {
	  sbuf = words[i];
	  if(sbuf.IFind(product) != -1) {
	    if(parse(sbuf.c_str(), words_nest, &numwords_nest, ',') == 0) {
	      if(*words_nest[1] != 0) {
		color = words_nest[1];
	      }
	    }
	    break;
	  }
	}
      }
    }
  }
  if(GoogleMapsZoomByProduct.IFind(product) != -1) {
    if(parse(GoogleMapsZoomByProduct.c_str(), words, &numwords, '|') == 1) {
      NT_print("Error parsing zoom by product table");
      NT_print(GoogleMapsZoomByProduct.c_str());
    }
    else {
      for(i = 0; i < numwords; i ++) {
	if(*words[i] != 0) {
	  sbuf = words[i];
	  if(sbuf.IFind(product) != -1) {
	    if(parse(sbuf.c_str(), words_nest, &numwords_nest, ',') == 0) {
	      if(*words_nest[1] != 0) {
		zoom = words_nest[1];
	      }
	    }
	    break;
	  }
	}
      }
    }
  }

  if(ugc_segment.ugc_hdr.is_expired) {
    is_expired = 1;
  }

  fname = GoogleMapsWebPageName;
  if((fname == "DEFAULT") || (fname == "default")) {
    // Use the default file name
    fname = text_product.product_hdr.pil;
    if(fname.length() == 9) fname.DeleteAt(0, 3);
    if(is_followup_statement) {
      // 06/02/2008: All follow-up statements are mapped orginal PIL 
      gxString new_pil = product;
      gxString old_pil = fname;
      old_pil.DeleteAt(3, 3); // Keep the old PIL for a default
      fname.DeleteAt(0, 3); // Keep the station ID
      if(new_pil.is_null()) {
	fname.InsertAt(0, old_pil);
      }
      else {
	fname.InsertAt(0, new_pil);
      }
    }
    fname << "-" << vptr->data.pvtec_hdr.event_tracking_number << "S" << current_ugc_segment;      

    if(generate_html_page) { // Static HTML page extention
      fname << html_file_extension;
    }
    else { // PHP include file extension
      fname << php_file_extension;
    }
  }

  if(!overwrite_output_webpage) insert_time_stamp(fname);

  gxString exp_time, long_exp_time;
  if(vptr->data.EndTime() > 1) {
    gxString time_string;
    time_t gmttime = vptr->data.EndTime();
    time_t offset = text_product.product_hdr.issue_time.gmt_offset;
    if(GetLocalTimeString(text_product.product_hdr.issue_time.time_zone.c_str(), 
			  gmttime, offset, time_string, 1)) {
      exp_time << clear << time_string.c_str();	  
    }
    if(GetLocalTimeString(text_product.product_hdr.issue_time.time_zone.c_str(), 
			  gmttime, offset, time_string, 0)) {
      long_exp_time << clear << time_string.c_str();	  
    }
  }
  NT_print("Valid until", long_exp_time.c_str());

  // Set the pagename and relative link here
  pagefilename = fname;
  pagefilename.DeleteBeforeLastIncluding("/");
  pagefilename.DeleteBeforeLastIncluding("\\");

#if defined (__WIN32__)
  fname.InsertAt(0, "\\");
#else
  fname.InsertAt(0, "/");
#endif
  fname.InsertAt(0, WebpageOutputDirectory);

  gxString vtec_message;

  // Process the VTEC headers
  vtec_message.Clear();
  VTEC curr_vtec;

  while(vptr) {
    if(vptr->data.IsHydro()) {
      if(debug) {
	NT_print("Skipping H-VTEC header", product.c_str());
      }
      vptr = vptr->next;    
      continue;
    }

    gxListNode<gxString> *vtablep;
    // Parse pp table
    vtablep = PVTEC_phenomena_table.GetHead();
    while(vtablep) {
      sbuf = vtablep->data;
      gxString pp = sbuf.Left(2);
      if(pp == vptr->data.Phenomena()) {
	sbuf.DeleteBeforeIncluding(pp);
	sbuf.TrimLeadingSpaces();
	sbuf.TrimTrailingSpaces();
	vtec_message << sbuf;
      }
      vtablep = vtablep->next;
    }
    // End parse
      
    vtablep = PVTEC_significance_table.GetHead();
    while(vtablep) {
      sbuf = vtablep->data;
      gxString s = sbuf.Left(1);
      if(s == vptr->data.Significance()) {
	sbuf.DeleteBeforeIncluding(s);
	sbuf.TrimLeadingSpaces();
	sbuf.TrimTrailingSpaces();
	vtec_message << " " << sbuf;
      }
      vtablep = vtablep->next;
    }
    
    vtablep = PVTEC_action_table.GetHead();
    vtec_action.Clear();
    while(vtablep) {
      sbuf = vtablep->data;
      gxString aaa = sbuf.Left(3);
      if(aaa == vptr->data.Action()) {
	vtec_action = aaa;
	sbuf.DeleteBeforeIncluding(aaa);
	sbuf.TrimLeadingSpaces();
	sbuf.TrimTrailingSpaces();
	vtec_message << " " << sbuf;
      }
      vtablep = vtablep->next;
    }
	
    if(vptr->data.BeginTime() > 1) {
      gxString time_string;
      time_t gmttime = vptr->data.BeginTime();
      time_t offset = text_product.product_hdr.issue_time.gmt_offset;
      if(GetLocalTimeString(text_product.product_hdr.issue_time.time_zone.c_str(), 
			    gmttime, offset, time_string)) {
	vtec_message << " from " << time_string.c_str();
      }
    }
    if(vptr->data.EndTime() > 1) {
      gxString time_string;
      time_t gmttime = vptr->data.EndTime();
      time_t offset = text_product.product_hdr.issue_time.gmt_offset;
      if(GetLocalTimeString(text_product.product_hdr.issue_time.time_zone.c_str(), 
			    gmttime, offset, time_string)) {
	if((vtec_action == "CAN") || (vtec_action == "EXP")) {
	  vtec_message << " at " << time_string.c_str();	  
	}
	else {
	  vtec_message << " until " << time_string.c_str();	  
	}
      }
    }
    
    vtec_message.ToUpper();

    curr_vtec = vptr->data;

    vptr = vptr->next;
    
    if(vptr) vtec_message << "\n";
  }

  // Polygon warnings only have one VTEC header so use the last action

  // Check to see if the product has EXP action and when and if it expires
  if(ugc_segment.ResetIsExpired(curr_vtec)) {
    is_expired = ugc_segment.ugc_hdr.is_expired;
  }
  
  // End of VTEC and UGC header reads
  
  // Generate the Web page or include table here
  gxString title, description;
  title << clear << issued_by << " - Warning Polygon Map";
  description << clear << title;
  webpage.Clear();

  if(generate_html_page) {
    if(debug) NT_print("Generating the HTML prolog");
    WriteHTMLHeader(pagefilename, title, description, webpage);
  }
  else { // Default to PHP include file
    if(debug) NT_print("Generating the PHP include file prolog");
    WritePHPIncludeHeader(pagefilename, description, webpage);
    
    inputbuf << clear << GoogleMapsPHPHeaderIncludeCMS;
    inputbuf.ToUpper();
    if(inputbuf != "NONE") {
      webpage << "\n";
      webpage << "<?php" << "\n";
      webpage << "include(\"" << GoogleMapsPHPHeaderIncludeCMS << "\");" << "\n";
      webpage << "?>" << "\n";
      webpage << "\n";
    }
    inputbuf << clear << GoogleMapsPHPHeaderIncludeSite;
    inputbuf.ToUpper();
    if(inputbuf != "NONE") {
      webpage << "\n";
      webpage << "<?php" << "\n";
      webpage << "include(\"" << GoogleMapsPHPHeaderIncludeSite << "\");" << "\n";
      webpage << "?>" << "\n";
      webpage << "\n";
    }
    inputbuf << clear << GoogleMapsPHPLeftIncludeCMS;
    inputbuf.ToUpper();
    if(inputbuf != "NONE") {
      webpage << "\n";
      webpage << "<?php" << "\n";
      webpage << "include(\"" << GoogleMapsPHPLeftIncludeCMS << "\");" << "\n";
      webpage << "?>" << "\n";
      webpage << "\n";
    }
    inputbuf << clear << GoogleMapsPHPLeftIncludeSite;
    inputbuf.ToUpper();
    if(inputbuf != "NONE") {
      webpage << "\n";
      webpage << "<?php" << "\n";
      webpage << "include(\"" << GoogleMapsPHPLeftIncludeSite << "\");" << "\n";
      webpage << "?>" << "\n";
      webpage << "\n";
    }

    // Set initial PHP variables here
    if((is_expired) || (vtec_action == "CAN")) {
      webpage << "\n";
      webpage << "<?php" << "\n";
      webpage << "$ProductHasExpired = 1;" << "\n";
      webpage << "?>" << "\n";
      webpage << "\n";
    }
    else {
      webpage << "\n";
      webpage << "<?php" << "\n";
      webpage << "$ProductHasExpired = 0;" << "\n";
      webpage << "$MustExpireThisPage = 0;" << "\n";
      webpage << "$UGCTimeToExpire = " << ugc_segment.ugc_hdr.ugc_time.elapsed_time << ";" << "\n";
      webpage << "$SystemTime = time();" << "\n";
      webpage << "\n";
      webpage << "if($SystemTime > $UGCTimeToExpire) {" << "\n";
      webpage << "  $MustExpireThisPage = 1;" << "\n";
      webpage << "}" << "\n";
      webpage << "?>" << "\n";
      webpage << "\n";
    }
  }

  webpage << "\n";
  if(!strip_html_comments) {
    webpage << "<!-- *********************************************************** -->" << "\n";
    webpage << "<!-- ********* Start of JavaScript code for Google maps ******** -->" << "\n";
    webpage << "<!-- *********************************************************** -->" << "\n";
  }

  if(GoogleMapsVersion == "2") { // As of May 2013, version 2 no longer works
    // Code below is legacy code for reference only

    sbuf << clear << GoogleMapsURL;
    sbuf.ReplaceString("MYKEY", GoogleMapsKey.c_str());
    sbuf.ReplaceString("MYCLIENT", GoogleMapsClientName.c_str());
    
    webpage << "<!-- BEGIN BLOCK1 -->" << "\n";
    webpage <<  "<script src=\"" << sbuf << "\" type=\"text/javascript\"></script>\n";
    if(GoogleMapsRadarOverlay == "TRUE") { 
      webpage <<  "<script src=\"" << GoogleMapsWMSTransparentLayersScript << "\" type=\"text/javascript\"></script>\n";
    }
    webpage <<  "<script type=\"text/javascript\">\n";
    webpage <<  "//<![CDATA[\n";
    
    // 06/10/2008: Changed the load() function name to fix an IE bug
    js_file <<  "function LoadGoogleMap() {\n";

    if(GoogleMapsRadarOverlay == "TRUE") { 
      js_file <<  "var WMS_URL_GAL=\"" << GoogleMapsWMSGAL << "\";\n";
      js_file <<  "var rad = createWMSSpec(" << GoogleMapsWMSSpec << ");\n";
      js_file <<  "var G_MAP_RIDGE_SPEC = createWMSOverlaySpec(" << GoogleMapsWMSOverlaySpec << ");\n";
    }
    
    js_file <<  "var mapOpts = {\n";
    js_file <<  " draggableCursor:\"default\",\n";
    js_file <<  " draggingCursor:\"default\"\n";
    js_file <<  "}\n";
    js_file <<  " if (GBrowserIsCompatible()) {\n";
    js_file <<  "  var map = new GMap2(document.getElementById(\"map\"), mapOpts);\n";
    js_file <<  "  var baseIcon = new GIcon();\n";
    js_file <<  "      baseIcon.iconSize=new GSize(32,32);\n";
    js_file <<  "      baseIcon.iconAnchor=new GPoint(16,32);\n";
    js_file <<  "      baseIcon.infoWindowAnchor=new GPoint(16,0);\n";
    js_file <<  "  var blank = new GIcon(baseIcon, \"" << GoogleMapsGIcon << "\", null);\n";
    
    
    js_file <<  "  map.addMapType(G_NORMAL_MAP);\n";
    js_file <<  "  map.addMapType(G_SATELLITE_MAP);\n";
    
    if(GoogleMapsRadarOverlay == "TRUE") { 
      js_file <<  "  map.addMapType(G_MAP_RIDGE_SPEC);\n";
    }
    
    js_file <<  "  map.enableContinuousZoom();\n";
    js_file <<  "  map.enableDoubleClickZoom();\n";
    js_file <<  "  map.addControl(new GLargeMapControl());\n";
    js_file <<  "  map.addControl(new GMapTypeControl());\n";
    js_file <<  "  map.addControl(new GScaleControl());\n";
    js_file <<  "  map.addControl(new GOverviewMapControl());\n";
    
    ugc_segment.polygon_warning.polygon_center.PrintLat(sbuf, 4);
    js_file <<  "  map.setCenter(new GLatLng(" << sbuf;
    ugc_segment.polygon_warning.polygon_center.PrintLon(sbuf, 4);
    js_file << ", " << sbuf << "), " << zoom << ", G_NORMAL_MAP);\n";
    js_file <<  " }\n";
    js_file <<  " var polygon = new GPolygon([";
    
    gxListNode<PolyPoint_header> *pptr = ugc_segment.polygon_warning.polygon_coords.GetHead();
    while(pptr) {
      js_file <<  "new GLatLng(";
      pptr->data.PrintGroup(sbuf, 4);
      js_file << sbuf;
      js_file << ")";
      pptr = pptr->next;
      if(pptr) js_file << ", ";
    }
    // Set the last coord equal to the first to complete the polygon box
    pptr = ugc_segment.polygon_warning.polygon_coords.GetHead();
    if(pptr) {
      js_file <<  ", new GLatLng(";
      pptr->data.PrintGroup(sbuf, 4);
      js_file << sbuf;
      js_file << ")";
    }
    
    js_file <<  "], \"" << color << "\", 5, .5, \"" << color << "\");\n";
    js_file <<  " map.addOverlay(polygon);\n";
    ugc_segment.polygon_warning.centroid_center.PrintLat(sbuf, 4);
    js_file <<  " var polyline = new GPolyline([new GLatLng(" << sbuf << ", ";
    ugc_segment.polygon_warning.centroid_center.PrintLon(sbuf, 4);
    js_file << sbuf << "), new GLatLng(";
    ugc_segment.polygon_warning.centroid_arrow_line.PrintLat(sbuf, 4);
    js_file << sbuf << ", ";
    ugc_segment.polygon_warning.centroid_arrow_line.PrintLon(sbuf, 4);
    js_file << sbuf << ")], \"blue\", 5, .5);\n";
    js_file <<  " map.addOverlay(polyline);\n";
    ugc_segment.polygon_warning.centroid_arrow_line.PrintLat(sbuf, 4);
    js_file <<  " var polyline2 = new GPolyline([new GLatLng(" << sbuf;
    ugc_segment.polygon_warning.centroid_arrow_line.PrintLon(sbuf, 4);
    js_file << ", " << sbuf << "), new GLatLng(";
    ugc_segment.polygon_warning.centroid_arrow_top_point.PrintLat(sbuf, 4);
    js_file << sbuf << ", ";
    ugc_segment.polygon_warning.centroid_arrow_top_point.PrintLon(sbuf, 4);
    js_file << sbuf << ")], \"blue\", 5, .5);\n";
    js_file <<  " map.addOverlay(polyline2);\n";
    ugc_segment.polygon_warning.centroid_arrow_line.PrintLat(sbuf, 4);
    js_file <<  " var polyline3 = new GPolyline([new GLatLng(" << sbuf;
    ugc_segment.polygon_warning.centroid_arrow_line.PrintLon(sbuf, 4);
    js_file << ", " << sbuf << "), new GLatLng(";
    ugc_segment.polygon_warning.centroid_arrow_bottom_point.PrintLat(sbuf, 4);
    js_file << sbuf << ", ";
    ugc_segment.polygon_warning.centroid_arrow_bottom_point.PrintLon(sbuf, 4);
    js_file << sbuf << ")], \"blue\", 5, .5);\n";
    js_file <<  " map.addOverlay(polyline3);\n";
    ugc_segment.polygon_warning.polygon_center.PrintLat(sbuf, 4);
    js_file <<  " var marker = new GMarker(new GLatLng(" << sbuf;
    ugc_segment.polygon_warning.polygon_center.PrintLon(sbuf, 4);
    js_file << ", " << sbuf << "), blank);\n";
    js_file <<  " map.addOverlay(marker);\n";
    
    if(GoogleMapsTextOverlay) {
      js_file <<  " marker.openInfoWindowHtml(\"<span style='font:8pt courier;color:" << color;
      js_file << ";'><b>" << type << "</b><br></span><span style='font:8pt courier'>Issued at " << iss;
      if (!exp_time.is_null()) {
	js_file <<  "<br>Valid until " << exp_time;
      }
      js_file <<  "<br></span><span style='font:8pt courier;color:green;'>Text appears below map</span>\");\n";
      
      js_file <<  " GEvent.addListener(marker, \"click\", function() {\n";
      js_file <<  "  marker.openInfoWindowHtml(\"<span style='font:8pt courier;color:" << color;
      js_file << ";'><b>" << type << "</b><br></span><span style='font:8pt courier'>Issued at " << iss;
      if (!exp_time.is_null()) {
	js_file <<  "<br>Valid until " << exp_time;
      }
      js_file <<  "<br></span><span style='font:8pt courier;color:green;'>Text appears below map</span>\");\n";
      js_file <<  " });\n";
    }
    js_file <<  "};\n"; 
  }
  else { // Default to the current Google maps version.

    webpage << "<!-- BEGIN BLOCK1 -->" << "\n";

    if((!GoogleMaps3Key.is_null()) && (GoogleMaps3Key != "NONE")) {
      sbuf << clear << GoogleMaps3URL << GoogleMaps3Key;
      sbuf.ReplaceString("MYVERSION", GoogleMapsVersion.c_str());
      sbuf.ReplaceString("MYCLIENTNAME", GoogleMapsClientName.c_str());
      sbuf.ReplaceString("MYCHANNEL", GoogleMapsChannel.c_str());
    }
    else {
      sbuf << clear << GoogleMaps3URL;
      sbuf.ReplaceString("MYVERSION", GoogleMapsVersion.c_str());
    } 

    webpage <<  "<script src=\"" << sbuf << "\" type=\"text/javascript\"></script>\n";
    webpage <<  "<script type=\"text/javascript\">\n";
    webpage <<  "//<![CDATA[\n";
    js_file <<  "function LoadGoogleMap() {\n";
    js_file << "   var infoindow;\n";
    ugc_segment.polygon_warning.polygon_center.PrintLat(sbuf, 4);
    js_file << "   var myLatLng = new google.maps.LatLng(" << sbuf;
    ugc_segment.polygon_warning.polygon_center.PrintLon(sbuf, 4);
    js_file << ", " << sbuf << ");\n";
    js_file << "   var mapOptions = {\n";
    js_file << "     zoom: " << zoom << ",\n";
    js_file << "     center: myLatLng,\n";
    js_file << "     mapTypeId: google.maps.MapTypeId.TERRAIN\n";
    js_file << "   };\n";
    js_file << "   var thePolygon;\n";
    js_file << "\n";
    js_file << "   var map = new google.maps.Map(document.getElementById('map'), mapOptions);\n";
    js_file << "\n";
    js_file <<  "   var polygonCoords = [ ";

        gxListNode<PolyPoint_header> *pptr = ugc_segment.polygon_warning.polygon_coords.GetHead();
    while(pptr) {
      js_file <<  "new google.maps.LatLng(";
      pptr->data.PrintGroup(sbuf, 4);
      js_file << sbuf;
      js_file << ")";
      pptr = pptr->next;
      if(pptr) js_file << ", ";
    }
    // Set the last coord equal to the first to complete the polygon box
    pptr = ugc_segment.polygon_warning.polygon_coords.GetHead();
    if(pptr) {
      js_file <<  ", new google.maps.LatLng(";
      pptr->data.PrintGroup(sbuf, 4);
      js_file << sbuf;
      js_file << ")";
    }
    js_file << "];\n";

    js_file << "\n";
    js_file <<  "  // Construct the polygon\n";
    js_file <<  "  thePolygon = new google.maps.Polygon({\n";
    js_file <<  "    paths: polygonCoords,\n";
    js_file <<  "    strokeColor: '" << color << "',\n";
    js_file <<  "    strokeOpacity: 0.8,\n";
    js_file <<  "    strokeWeight: 2,\n";
    js_file <<  "    fillColor: '" << color << "',\n";
    js_file <<  "    fillOpacity: 0.35\n";
    js_file <<  "  });\n";
    js_file << "\n";
    js_file <<  "  thePolygon.setMap(map);\n";

    if(GoogleMapsTextOverlay) {
      js_file <<  "  infowindow = new google.maps.InfoWindow();\n";
      js_file <<  "  google.maps.event.addListener(thePolygon, 'click', function(event){\n";
      js_file <<  "     infowindow.setContent(\"<span style='font:8pt courier;color:" << color << ";'><b>" << type << "</b><br></span><span style='font:8pt courier'>Issued at " << iss;
      if (!exp_time.is_null()) {
	js_file <<  "<br>Valid until " << exp_time;
      }
      js_file <<  "<br></span><span style='font:8pt courier;color:green;'>Text appears below map</span>\");\n";
      js_file <<  "       infowindow.setPosition(event.latLng);\n";
      js_file <<  "       infowindow.open(map);\n";
      js_file <<  "    });\n";
      js_file << "\n";
    }

    js_file <<  "  var polylinesCoordinates = [\n";
    ugc_segment.polygon_warning.centroid_center.PrintLat(sbuf, 4);
    js_file <<  "      new google.maps.LatLng(" << sbuf << ", ";
    ugc_segment.polygon_warning.centroid_center.PrintLon(sbuf, 4);
    js_file << sbuf << "), new google.maps.LatLng(";
    ugc_segment.polygon_warning.centroid_arrow_line.PrintLat(sbuf, 4);
    js_file << sbuf << ", ";
    ugc_segment.polygon_warning.centroid_arrow_line.PrintLon(sbuf, 4);
    js_file << sbuf << "),\n";
    ugc_segment.polygon_warning.centroid_arrow_line.PrintLat(sbuf, 4);
    js_file <<  "      new google.maps.LatLng(" << sbuf;
    ugc_segment.polygon_warning.centroid_arrow_line.PrintLon(sbuf, 4);
    js_file << ", " << sbuf << "), new google.maps.LatLng(";
    ugc_segment.polygon_warning.centroid_arrow_top_point.PrintLat(sbuf, 4);
    js_file << sbuf << ", ";
    ugc_segment.polygon_warning.centroid_arrow_top_point.PrintLon(sbuf, 4);
    js_file << sbuf << "),\n";
    ugc_segment.polygon_warning.centroid_arrow_line.PrintLat(sbuf, 4);
    js_file <<  "      new google.maps.LatLng(" << sbuf;
    ugc_segment.polygon_warning.centroid_arrow_line.PrintLon(sbuf, 4);
    js_file << ", " << sbuf << "), new google.maps.LatLng(";
    ugc_segment.polygon_warning.centroid_arrow_bottom_point.PrintLat(sbuf, 4);
    js_file << sbuf << ", ";
    ugc_segment.polygon_warning.centroid_arrow_bottom_point.PrintLon(sbuf, 4);
    js_file << sbuf << ")\n";
    js_file <<  "  ];\n";

    js_file << "\n";
    js_file <<  "  var polyline = new google.maps.Polyline({\n";
    js_file <<  "    path: polylinesCoordinates,\n";
    js_file <<  "    strokeColor: '#0000FF',\n";
    js_file <<  "    strokeOpacity: .5,\n";
    js_file <<  "    strokeWeight: 5\n";
    js_file <<  "   });\n";
    js_file << "\n";
    js_file <<  "   polyline.setMap(map);\n";
    js_file <<  "}\n";
    js_file << "\n";
    js_file <<  "google.maps.event.addDomListener(window, 'load', initialize);\n";
  }

  webpage << js_file;

  webpage <<  "//]]>\n";
  webpage <<  "</script>\n";
  webpage << "<!-- END BLOCK1 -->" << "\n";
  if(!strip_html_comments) {
    webpage << "\n";
    webpage << "<!-- *********************************************************** -->" << "\n";
    webpage << "<!-- ********** End of JavaScript code for Google maps ********* -->" << "\n";
    webpage << "<!-- *********************************************************** -->" << "\n";
    webpage << "\n";
  }
  webpage << "\n";
  if(!strip_html_comments) {
    webpage << "<!-- Page Content Starts Here -->" << "\n";
  }

  if((is_expired) && (GooleMapsPrintExpiredMessage)) {
    webpage << "\n" << "<font color=\"red\"><H1>NOTICE: Product has expired</H1></font>" << "\n"; 
    webpage << "\n" << "<font color=\"red\"><H2>Page posted for archiving and testing only</H2></font>" << "\n"; 
  }
  else {
    if((vtec_action == "CAN") && (GooleMapsPrintExpiredMessage)) {
      webpage << "\n" << "<font color=\"red\"><H1>NOTICE: Product has been cancelled</H1></font>" << "\n"; 
    }
  }

  // PHP code used to expire a product in case a follow statemet is never issued to cancel or expire
  if(!generate_html_page) {
    if((!is_expired) && (vtec_action != "CAN")) {
      if(GooleMapsPrintExpiredMessage) {
	webpage << "\n";
	webpage << "<?php" << "\n";
	webpage << "if($ProductHasExpired == 0) {" << "\n";
	webpage << "  if($MustExpireThisPage == 1) {" << "\n";
	webpage << "    echo \"\\n<font color=\\\"red\\\"><H1>NOTICE: Product has expired</H1></font>\\n\";" << "\n";
	webpage << "    echo \"\\n<font color=\\\"red\\\"><H2>Page posted for archiving and testing only</H2></font>\\n\";" << "\n";
	webpage << "  }" << "\n";
	webpage << "}" << "\n";
	webpage << "?>" << "\n";
	webpage << "\n";
      }
    }
  }
  

  if(GoogleMapsDisplayDisclaimer == "TRUE") {
    if(GoogleMapsDisclaimer.is_null()) {
      GoogleMapsDisclaimer = "The map portion of the following product is experimental and may not always be available.";
    }
    webpage <<  "<span style=\"font:8pt arial;color:red;\"><b>DISCLAIMER:</b>&nbsp; " << GoogleMapsDisclaimer << "</span><br>\n";
  }

  webpage <<  "<div id=\"map\" style=\"width:" << GoogleMapsWidth << "px;height:" << GoogleMapsHeight << "px;border:solid 1px black;\"></div>\n";

  webpage << "<!-- BEGIN BLOCK2 -->" << "\n";
  webpage << "\n";
  if(!strip_html_comments) {
    webpage << "<!-- Load the Google map -->" << "\n";
  }

  webpage << "<script language=\"javascript\">" << "\n";
  webpage << "<!--" << "\n";
  // 06/10/2008: Changed the load() function name to LoadGoogleMap() to fix an IE bug
  // webpage << "window.onload = load();" << "\n";

  // 07/23/2008: Add an onload event handler to fix IE problems with the Google map API.
  // 07/23/2008: If not called from a <body "onload=LoadGoogleMap()"> tag IE will not 
  // 07/23/2008: wait for the Google map variable to load and will not load the Web page. 
  // webpage << "LoadGoogleMap();" << "\n";
  webpage << "function addLoadEvent(func) {" << "\n";
  webpage << "  var oldonload = window.onload;" << "\n";
  webpage << "  if (typeof window.onload != \'function\') {" << "\n";
  webpage << "    window.onload = func;" << "\n";
  webpage << "  }" << "\n"; 
  webpage << "  else {" << "\n"; 
  webpage << "    window.onload = function() {" << "\n";
  webpage << "     if (oldonload) {" << "\n";
  webpage << "	     oldonload();" << "\n";
  webpage << "	   }" << "\n";
  webpage << "	   func();" << "\n";
  webpage << "   }" << "\n";
  webpage << "  }" << "\n";
  webpage << "}" << "\n";
  webpage << "\n";
  webpage << "addLoadEvent(LoadGoogleMap);" << "\n";

  webpage << "// -->" << "\n";
  webpage << "</script>" << "\n";
  webpage << "\n";
  webpage << "<!-- END BLOCK2 -->" << "\n";

  if((is_expired) && (GooleMapsPrintExpiredMessage)) {
    webpage << "\n" << "<font color=\"red\"><H1>PRODUCT EXPIRED!!!</H1></font>" << "\n"; 
  }
  else {
    if((vtec_action == "CAN") && (GooleMapsPrintExpiredMessage)) {
      webpage << "\n" << "<font color=\"red\"><H1>Product has been cancelled!!!</H1></font>" << "\n"; 
    }
  }

  // PHP code used to expire a product in case a follow statemet is never issued to cancel or expire
  if(!generate_html_page) {
    if(GooleMapsPrintExpiredMessage) {
      if((!is_expired) && (vtec_action != "CAN")) {
	webpage << "\n";
	webpage << "<?php" << "\n";
	webpage << "if($ProductHasExpired == 0) {" << "\n";
	webpage << "  if($MustExpireThisPage == 1) {" << "\n";
	webpage << "    echo \"\\n<font color=\\\"red\\\"><H1>PRODUCT EXPIRED!!!</H1></font>\\n\";" << "\n";
	webpage << "  }" << "\n";
	webpage << "}" << "\n";
	webpage << "?>" << "\n";
	webpage << "\n";
      }
    }
  }

  gxString text;
  text << clear << text_product.product_hdr.pil << "\n";
  text << "\n";
  text << text_product.product_hdr.issue_office << "\n";
  text << text_product.product_hdr.name << "\n";
  text << text_product.product_hdr.issue_time.time_string << "\n";
  text << "\n";
    
  gxString ugc_segment_header = ugc_segment.ugc_segment_header;
  if(!ugc_segment_header.is_null()) {
    ugc_segment_header.TrimLeading('\n');
    ugc_segment_header.TrimTrailing('\n');
    ugc_segment_header << "\n";
    text << ugc_segment_header;
  }

  if(debug) {
    if(!vtec_message.is_null()) {
      text << "\n" << vtec_message << "\n";
    }
  }

  gxListNode<gxString> *txptr = ugc_segment.ugc_segment_text.GetHead();
  int has_highlight = 0;
  gxString highlight_color = GoogleMapsTextHighlightColor;
  if((is_expired) && (GooleMapsPrintExpiredMessage)) { 
    highlight_color = GoogleMapsTextExpiredHighlightColor;
  }

  while(txptr) {
    if(txptr->data[0] == '*') {
      if(txptr->data[1] == ' ') {
	has_highlight = 1;
	text << "<span style=\'color:" << highlight_color << ";font-weight:bold;\'>";
      }
    }
    text << txptr->data;
    if((has_highlight) && (txptr->data[0] == '\n')) {
      text << "</span>";
      has_highlight = 0;
    }
    txptr = txptr->next;
  }
  if(has_highlight) {
    // Terminate the last highlight line if needed
    text << "</span>";
    has_highlight = 0;
  }
  webpage <<  "<pre>" << text << "</pre>\n";
  
  if((is_expired) && (GooleMapsPrintExpiredMessage)) {
    webpage << "\n" << "<font color=\"red\"><H1>NOTICE: Product has expired</H1></font>" << "\n"; 
    webpage << "\n" << "<font color=\"red\"><H2>Page posted for archiving and testing only</H2></font>" << "\n"; 
  }
  else {
    if((vtec_action == "CAN") && (GooleMapsPrintExpiredMessage)) {
      webpage << "\n" << "<font color=\"red\"><H1>NOTICE: Product has been cancelled</H1></font>" << "\n"; 
    }
  }

  // PHP code used to expire a product in case a follow statemet is never issued to cancel or expire
  if(!generate_html_page) {
    if((!is_expired) && (vtec_action != "CAN")) {
      if(GooleMapsPrintExpiredMessage) {
	webpage << "\n";
	webpage << "<?php" << "\n";
	webpage << "if($ProductHasExpired == 0) {" << "\n";
	webpage << "  if($MustExpireThisPage == 1) {" << "\n";
	webpage << "    echo \"\\n<font color=\\\"red\\\"><H1>NOTICE: Product has expired</H1></font>\\n\";" << "\n";
	webpage << "    echo \"\\n<font color=\\\"red\\\"><H2>Page posted for archiving and testing only</H2></font>\\n\";" << "\n";
	webpage << "  }" << "\n";
	webpage << "}" << "\n";
	webpage << "?>" << "\n";
	webpage << "\n";
      }
    }
  }

  if(!strip_html_comments) {
    webpage << "<!-- Page Content Ends Here -->" << "\n";
  }
  webpage << "\n";

  if(generate_html_page) {
    if(debug) NT_print("Generating the HTML epilog");
    WriteHTMLTail(webpage);
  }
  else {
    inputbuf << clear << GoogleMapsPHPFooterIncludeSite;
    inputbuf.ToUpper();
    if(inputbuf != "NONE") {
      webpage << "\n";
      webpage << "<?php" << "\n";
      webpage << "include(\"" << GoogleMapsPHPFooterIncludeSite << "\");" << "\n";
      webpage << "?>" << "\n";
      webpage << "\n";
    }
    inputbuf << clear << GoogleMapsPHPFooterIncludeCMS;
    inputbuf.ToUpper();
    if(inputbuf != "NONE") {
      webpage << "\n";
      webpage << "<?php" << "\n";
      webpage << "include(\"" << GoogleMapsPHPFooterIncludeCMS << "\");" << "\n";
      webpage << "?>" << "\n";
      webpage << "\n";
    }
    if(debug) NT_print("Generating the PHP include file epilog");
    WritePHPTail(webpage);
  }
    
  return 1;
}
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
