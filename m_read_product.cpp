// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- // 
// C++ Source Code File Name: m_read_product.cpp
// Compiler Used: MSVC, GCC
// Produced By: Douglas.Gaer@noaa.gov
// File Creation Date: 01/25/2008
// Date Last Modified: 07/17/2009
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

int TextProduct::ReadProduct() 
{
  error_string.Clear();
  gxString sbuf;
  gxString rp_out;
  int html = 0;
  gxString jscode;
  gxString jscode_global_text;
  gxString jsbuf;
  gxString js_segment_text = "var VTECSegmentText = new Array();\n";
  gxString js_segment_zones = "var VTECZoneListSegment = new Array();\n";
  gxString js_get_text_function = "\
function VTECGetText(zonenumber) { \n\
  HasCaptionMessage = false;\n\
  CaptionMessage = DefaultCaptionMessage;\n\
\n\
  var text = \"\";\n\
  var numzonelists = VTECZoneListSegment.length;\n\
\n\
  var i, j;\n\
  for(i = 0; i < numzonelists; i++) {\n\
     var n = VTECZoneListSegment[i].length;\n\
     for(j = 0; j < n; j++) {\n\
      if(VTECZoneListSegment[i][j] == zonenumber) {\n\
        HasCaptionMessage = true;\n\
	text += VTECSegmentText[i];\n\
	break;\n\
      }\n\
    }\n\
  }\n\
\n\
  if(HasCaptionMessage) CaptionMessage = text;\n\
\n\
}\n\
";

  gxString js_get_text_function_blank = "\
function VTECGetText(zonenumber) { \n\
  HasCaptionMessage = false;\n\
  CaptionMessage = DefaultCaptionMessage;\n\
}\n\
";

  if(!read_product_config_file.is_null()) {
    if(!LoadOrBuildReadProductConfigFile()) {
      error_string << clear << "Text product error: " 
		   << "Error building read product formatting config file"
		   << read_product_config_file;  
      return 0;
    }
  }

  if(product.IsEmpty()) {
    error_string << clear << "Text product error: " << "No product is loaded";
    return 0;
  }

  if(OutputInHTMLFormat == "TRUE") html = 1;
  
  if(verbose) {
    NT_console_write_err("Reading product:", product_hdr.pil.c_str());
  }

  if(html) rp_out << "<strong>" << "\n";
  if(CustomProductName != "NONE") {
    if(html) rp_out << "<p>" << "\n";
    rp_out << CustomProductName;
    if(html) rp_out << "<br>";
    rp_out << "\n";
  }
  if(CustomProductTitle != "NONE") {
    rp_out << CustomProductTitle;
    if(html) rp_out << "<br>";
    rp_out << "\n";
    if(html) rp_out << "</p>" << "\n";
  }

  if((ReadIssueOffice == "TRUE") && (product_hdr.ugc_list.IsEmpty())) {
    if(HasIssuedTitle == "NONE") {
      rp_out << product_hdr.issue_office.c_str() << " " 
	     << product_hdr.name.c_str() << "\n";
      if(html) rp_out << "<br>" << "\n";
    }
    else {
      rp_out << product_hdr.issue_office.c_str() << " " << HasIssuedTitle << " " 
	     << product_hdr.name.c_str() << "\n";
      if(html) rp_out << "<br>" << "\n";
    }
  } 
  if(html) rp_out << "</strong>" << "\n";

  if(html) rp_out << "<hr>" << "\n";

  
  int segment_index = 0;
  int num_segment_text_read = 0;
  gxString segment_text_str;
  gxListNode<gxString> *sptr;

  // 06/01/2009: Added support to parse text after the product header but before the first
  // 06/01/2009: VTEC segment as global text.
  gxString gtext_buf1;
  gxString gtext_buf2;
  gxString gp_out;

  if((!product_hdr.global_text.IsEmpty()) && (ReadGlobalText == "TRUE")) {

    if(verbose) {
      NT_console_write_err("START GLOBAL TEXT READ");
    }

    grep_text(product_hdr.global_text, gtext_buf1,
	      StartGlobalText, EndGlobalText,
	      KeepStartGlobalText, KeepEndGlobalText,
	      MultiLineGlobalText, GetAllProductGlobalTextBlocks,
	      FilterStringFromGlobalText);

    if((!StartProductText.is_null()) && (GlobalParseSkipsStartProductText == "FALSE")) {

      grep_text(product_hdr.global_text, gtext_buf2,
		StartProductText, EndProductText,
		KeepStartProductText, KeepEndProductText,
		MultiLineText, GetAllProductTextBlocks,
		FilterStringFromText);
    }

    if(verbose) {
      NT_console_write_err("END GLOBAL TEXT READ");
    }

    // Put all the global text reads together
    gxString gtext_buf = gtext_buf1;
    if((!gtext_buf1.is_null()) && (!gtext_buf1.is_null())) {
      gtext_buf << "\n\n";
    }
    gtext_buf << gtext_buf2;

    // Process the global text
    if(!gtext_buf.is_null()) {
      gp_out.Clear();
      if(html) 	gp_out << "<!-- START OF OVERVIEW -->" << "\n";

      if(WriteJavaScriptCode == "TRUE") {
	jscode_global_text << "<!-- BEGIN BLOCK2 -->" << "\n";
	jscode_global_text << "<script language=\"javascript\" type=\"text/javascript\">" << "\n";
	jscode_global_text << "<!--" << "\n";
	gxString gtext_buf3 = gtext_buf;
	gtext_buf3.ReplaceString("\"", "\\\"");
	gtext_buf3.ReplaceString("\'", "\\\'");
	gtext_buf3.FilterChar('\r');
	gtext_buf3.ReplaceString("\n", "\\n\\\n");
	jscode_global_text << "var AllZonesCaptionMessage = \'\\" << "\n";
	jscode_global_text << gtext_buf3 << "\';" << "\n"; 
	jscode_global_text << "\n" << "DefaultCaptionMessage = AllZonesCaptionMessage;" << "\n";
	jscode_global_text << "HasAllZonesMessage = true;" << "\n";
	jscode_global_text << "// -->" << "\n";
	jscode_global_text << "</script>" << "\n";
	jscode_global_text << "\n" << "<!-- END BLOCK2 -->" << "\n";
      }

      if(html) gp_out << "<strong>" << "\n";
      if(CustomGlobalName != "NONE") {
	if(html) gp_out << "<p>" << "\n";
	gp_out << CustomGlobalName;
	if(html) gp_out << "<br>";
	gp_out << "\n";
      }
      if(CustomGlobalTitle != "NONE") {
	gp_out << CustomGlobalTitle;
	if(html) gp_out << "<br>";
	gp_out << "\n";
	if(html) gp_out << "</p>" << "\n";
      }
      if(html) gp_out << "</strong>" << "\n";
      if(html) gp_out << "<a name=\"AllZones\" id=\"AllZones\"></a>"; 
      if(html) gp_out << "<font color=\"" << TextBodyColor.c_str() << "\">" << "\n";
      if(html) gp_out << "<pre>";

      gp_out << gtext_buf;

      if(html) gp_out << "</pre>" << "\n";
      if(html) gp_out << "</font>" << "\n";
      if(html) gp_out << "<hr>" << "\n";
      if(html) 	gp_out << "<!-- END OF OVERVIEW -->" << "\n";
      if(ReadAllUpper == "TRUE") gp_out.ToUpper();
    }
  }

  if(ParseFromGlobalTextOnly == "TRUE") {
    GXSTD::cout << gp_out.c_str() << GXSTD::flush;    
    return 1;
  }

  if(!product_hdr.ugc_list.IsEmpty()) {
    gxListNode<UGC> *uptr = product_hdr.ugc_list.GetHead();
    while(uptr) {

      // 06/06/2008: Skip if set to ignore empty text segements and if the segment has no text 
      if((SkipEmptySegments == "TRUE") && (uptr->data.ugc_segment_text.IsEmpty())) {
	uptr = uptr->next;
	continue;
      }
      
      // 06/05/2008: Track empty text segments with using a text filter
      // 06/06/2008: Move the text segment read to the top of segment read loop
      segment_text_str.Clear();

      if(!uptr->data.ugc_segment_text.IsEmpty()) {
	gxString stext_buf;
	grep_text(uptr->data.ugc_segment_text, stext_buf,
		  StartProductText, EndProductText,
		  KeepStartProductText, KeepEndProductText,
		  MultiLineText, GetAllProductTextBlocks,
		  FilterStringFromText);

	// 06/05/2008: Look for empty text segments
	if((stext_buf.is_null()) || (stext_buf == "\n")) {
	  stext_buf << clear << "NONE REPORTED IN THIS UPDATE";
	  if(SkipEmptySegments == "TRUE") {
	    uptr = uptr->next;
	    continue;
	  }
	}
	else {
	  num_segment_text_read++;
	}

	segment_text_str << "\n" << stext_buf.c_str() << "\n";

	// Write the JS code array
	stext_buf.ReplaceString("\"", "\\\"");
	stext_buf.ReplaceString("\'", "\\\'");
	stext_buf.FilterChar('\r');
	stext_buf.ReplaceString("\n", "\\n\\\n");
	jscode << clear << "VTECSegmentText[" << segment_index << "] = Array(\'\\" << "\n";
	jscode << stext_buf << "\');" << "\n"; 
	js_segment_text << jscode;
      }
      else {
	segment_text_str << "\n" << "NONE REPORTED IN THIS UPDATE" << "\n";

	// Write the JS code array
	jscode << clear << "VTECSegmentText[" << segment_index << "] = Array(\'"
	       << "NONE REPORTED IN THIS UPDATE\\n" << "\');" << "\n";
	js_segment_text << jscode;
      }

      if(html) rp_out << "<strong>" << "\n";
      if(!uptr->data.vtec_list.IsEmpty()) {
	if(ReadIssueOffice == "TRUE") {
	  if(HasIssuedTitle == "NONE") {
	    rp_out << product_hdr.issue_office.c_str() << " "
		   << product_hdr.name.c_str() << "\n";
	    if(html) rp_out << "<br>" << "\n";
	  }
	  else {
	    rp_out << product_hdr.issue_office.c_str() << " " << HasIssuedTitle << " " 
		   << product_hdr.name.c_str() << "\n";
	    if(html) rp_out << "<br>" << "\n";
	  }
	}
	gxListNode<VTEC> *vptr = uptr->data.vtec_list.GetHead();
	while(vptr) {
	  gxListNode<gxString> *vtablep;
	  gxString vtec_message;
	  gxString vtec_time;
	  gxString vtec_action;

	  vtec_message << clear;
	  vtec_time << clear;
	  vtec_action = vptr->data.Action();

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
	      if(!sbuf.is_null()) vtec_message << " ";
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
	      vtec_message << sbuf;
	      if(!sbuf.is_null()) vtec_message << " ";
	    }
	    vtablep = vtablep->next;
	  }

	  vtablep = PVTEC_action_table.GetHead();
	  while(vtablep) {
	    sbuf = vtablep->data;
	    gxString aaa = sbuf.Left(3);
	    if(aaa == vptr->data.Action()) {
	      sbuf.DeleteBeforeIncluding(aaa);
	      sbuf.TrimLeadingSpaces();
	      sbuf.TrimTrailingSpaces();
	      vtec_message << sbuf;
	    }
	    vtablep = vtablep->next;
	  }

	  if(vptr->data.BeginTime() > 1) {
	    gxString time_string;
	    time_t gmttime = vptr->data.BeginTime();
	    time_t offset = product_hdr.issue_time.gmt_offset;
	    if(GetLocalTimeString(product_hdr.issue_time.time_zone.c_str(), 
				  gmttime, offset, time_string)) {

	      if(vtec_action == "CAN") { 
		vtec_time << CanceledTitle << " " << time_string.c_str();
	      }
	      else if(vtec_action == "EXP") {
		vtec_time << ExpiresTitle << " " << time_string.c_str();
	      }
	      else {
		vtec_time << IssuedFromTitle << " " << time_string.c_str();
	      }
	    }
	  }
	  if(vptr->data.EndTime() > 1) {
	    gxString time_string;
	    time_t gmttime = vptr->data.EndTime();
	    time_t offset = product_hdr.issue_time.gmt_offset;
	    if(GetLocalTimeString(product_hdr.issue_time.time_zone.c_str(), 
				  gmttime, offset, time_string)) {
	      if(vtec_action == "CAN") { 
		// No end time 
	      }
	      else if(vtec_action == "EXP") {
		vtec_time << ExpiresAtTitle << " " << time_string.c_str();
	      }
	      else {
		vtec_time << " " << IssuedUntilTitle << " " << time_string.c_str(); 
	      }
	    }
	  }
	  if(ReadVTECMessage == "TRUE") {
	    rp_out << vtec_message.c_str() << "\n";
	    if(html) rp_out << "<br>" << "\n";
	    rp_out << vtec_time.c_str() << "\n";
	    if(html) rp_out << "<br>" << "\n";
	  }

	  vptr = vptr->next;
	}
      }

      gxListNode<gxString> *zlptr;
      gxString zones, fips;

      if(ZoneListingTitle == "NONE") {
	zones << clear;
      }
      else {
	zones << clear << ZoneListingTitle << " ";
      }
      if(CountyListingTitle == "NONE") {
	fips << clear;
      }
      else {
	fips << clear << CountyListingTitle << " ";
      }

      if(!uptr->data.ugc_hdr.zone_list.IsEmpty()) {
	zlptr = uptr->data.ugc_hdr.zone_list.GetHead();
	int num_segment_zones = 0;
	jsbuf << clear << "VTECZoneListSegment[" << segment_index << "] = new Array(";
	while(zlptr) {
	  if(zlptr->data[2] == 'Z') {
	    zones << zlptr->data << ReadProductDelimter << " ";
	    jsbuf << "\"" << zlptr->data << "\"";
	    num_segment_zones++;
	  }
 	  if(zlptr->data[2] == 'C') {
	    fips << zlptr->data << ReadProductDelimter << " ";
	    jsbuf << "\"" << zlptr->data << "\"";
	    num_segment_zones++;
	  }
	  if(num_segment_zones > 0 ) {
	    if(zlptr->next) {
	      jsbuf << ", ";
	    }
	    else {
	      jsbuf << ");" << "\n";
	    }
	  } 
	  zlptr = zlptr->next;
	}
	js_segment_zones << jsbuf;

	zones.TrimTrailingSpaces();
	zones.TrimTrailing(ReadProductDelimter[0]);
	fips.TrimTrailingSpaces();
	fips.TrimTrailing(ReadProductDelimter[0]);
      }

      if(ReadZoneNumbers == "TRUE") {
	rp_out << zones << "\n";
	if(html) rp_out << "<br>" << "\n";
      }
      if(ReadCountyCodes == "TRUE") {
	rp_out << fips << "\n";
	if(html) rp_out << "<br>" << "\n";
      }

      if(ReadAreaNames == "TRUE") {
	gxString anames;
	if(!uptr->data.zones_names.IsEmpty()) {
	  if(AreaListingTitle == "NONE") {
	    anames << clear;
	  }
	  else {
	    anames << clear << AreaListingTitle << " ";
	  }

	  sptr = uptr->data.zones_names.GetHead();
	  while(sptr) {
	    anames << sptr->data.c_str();
	    sptr = sptr->next;
	    if(sptr) {
	      anames << ReadProductDelimter << " ";
	    }
	  }
	  // 03/17/2009: Added to insert zone number Anchor tag references
	  if(html) {
	    zlptr = uptr->data.ugc_hdr.zone_list.GetHead();
	    while(zlptr) {
	      // 04/24/2009: Modified to fix formatting issue when A tag underlines
	      rp_out << "<a name=\"" << zlptr->data  << "\" id=\"" << zlptr->data  << "\"></a>"; 
	      zlptr = zlptr->next;
	    }
	  }
	  rp_out << anames << "\n";;
	}
      }

      if(ReadIncludingAreaNames == "TRUE") {
	if(!uptr->data.include_city_names.IsEmpty()) {
	  gxString cnames;
	  if(IncludingAreaListingTitle == "NONE") {
	    cnames << clear;
	  }
	  else {
	    cnames << clear << IncludingAreaListingTitle << " ";
	  }

	  sptr = uptr->data.include_city_names.GetHead();
	  while(sptr) {
	    cnames << sptr->data.c_str();
	    sptr = sptr->next;
	    if(sptr) {
	      cnames << ReadProductDelimter << " ";
	    }
	  }
	  rp_out << cnames;
	  if(html) rp_out << "<br>";
	  rp_out << "\n";
	}
      }

      if(ReadPolygon == "TRUE") {
	rp_out << "\n";
	if(PolygonListingTitle != "NONE") {
	  rp_out << PolygonListingTitle << "\n";
	  if(html) rp_out << "<br>" << "\n";
	}
	if(!uptr->data.polygon_warning.lat_lon_string.is_null()) {
	  rp_out << uptr->data.polygon_warning.lat_lon_string.c_str() << "\n";
	  if(html) rp_out << "<br>" << "\n";
	  rp_out << uptr->data.polygon_warning.time_mot_loc_string.c_str() << "\n";
	  if(html) rp_out << "<br>" << "\n";
	}
	else {
	  rp_out << "NONE" << "\n";
	  if(html) rp_out << "<br>" << "\n";
	  rp_out << "NONE" << "\n";
	  if(html) rp_out << "<br>" << "\n";
	}
      }

      if(html) rp_out << "</strong>" << "\n";

      if(html) rp_out << "<font color=\"" << TextBodyColor.c_str() << "\">" << "\n";
      if(html) rp_out << "<pre>";

      // 06/06/2008: Insert the segment text here
      rp_out << segment_text_str;

      if(html) rp_out << "</pre>" << "\n";
      if(html) rp_out << "</font>" << "\n";

      if(html) {
	if(uptr->next) rp_out << "<hr>" << "\n";
      }

      segment_index++;
      uptr = uptr->next;
    }
  }

  rp_out << "\n";

  if(ReadAllUpper == "TRUE") rp_out.ToUpper();
  
  jscode << clear << "\n" << js_segment_text << "\n"
	 << js_segment_zones << "\n" << js_get_text_function << "\n";

  if(WriteJavaScriptCode == "TRUE") {
    if((SkipEmptySegments == "TRUE") && (num_segment_text_read == 0)) {
      jscode << clear << "\n" << js_get_text_function_blank << "\n";
    }

    if((JavaScriptIncludeFile == "NONE") || (JavaScriptIncludeFile == "DEFAULT")) {
      jsbuf << clear << "\n" << "<!-- BEGIN BLOCK1 -->" << "\n";
      jsbuf << "<script language=\"javascript\" type=\"text/javascript\">" << "\n";
      jsbuf << "<!--" << "\n";
      jsbuf << "\n" << "var HasAllZonesMessage = false;" << "\n";
      jsbuf << jscode;
      jsbuf << "// -->" << "\n";
      jsbuf << "</script>" << "\n";
      jsbuf << "\n" << "<!-- END BLOCK1 -->" << "\n";
      GXSTD::cout << jsbuf.c_str() << GXSTD::flush;
    }
    else { // Write to a JavaScript file 
      sbuf << clear << "VTEC utility JavaScript product decode for " << product_hdr.pil;
      WriteJavaScriptFile(JavaScriptIncludeFile, sbuf, jscode);
    }
  }

  if((SkipGlobalTextIfEmptySegments == "TRUE") && (num_segment_text_read == 0)) {
    if(verbose) {
      NT_console_write_err("Skipping global text with empty segments");
    }
  }
  else {
    if((!gp_out.is_null()) && (ReadGlobalText == "TRUE")) {
      
      if(WriteJavaScriptCode == "TRUE") {
	GXSTD::cout << "\n" << jscode_global_text.c_str() << "\n" << GXSTD::flush;
      }
      GXSTD::cout << gp_out.c_str() << GXSTD::flush;
    }
  }

  if((SkipEmptySegments == "TRUE") && (num_segment_text_read == 0)) {
    if(verbose) {
      NT_console_write_err("Skipping empty segments");
    }
    return 1;    
  }

  GXSTD::cout << rp_out.c_str() << GXSTD::flush;

  return 1;
}

void RPCfgFixTrueFalseString(gxString &out, char *in)
{
  out.Clear();
  if(!in) return;
  out = in;
  out.ToUpper();
  out.TrimLeadingSpaces();
  out.TrimTrailingSpaces();
}
 
int LoadOrBuildReadProductConfigFile()
{
  gxConfig CfgData;
  gxString sbuf;

  NT_print("Loading read product config file", read_product_config_file.c_str());

  int status = CfgData.Load(read_product_config_file.c_str());
  if(!status) {
    NT_print("User specified config file not found");
    NT_print("Building default configuration file");

    DiskFileB dfile(read_product_config_file.c_str(), DiskFileB::df_READWRITE,
		    DiskFileB::df_CREATE); 
    if(!dfile) {
      NT_print("Cannot write to config file", read_product_config_file.c_str());
      return 0;
    }
    dfile << "#  " << ProgramDescription << " " << VersionString << " configuration file" << "\n";
    dfile << "#" << "\n";
    dfile << "\n";
    dfile << "# Read product configuration" << "\n";
    dfile << "\n";
    dfile << "# Output formatting " << "\n";
    dfile << "ReadVerbose = " << ReadVerbose.c_str() << "\n";
    dfile << "ReadAllUpper = " << ReadAllUpper.c_str() << "\n";
    dfile << "ReadProductDelimter = " << ReadProductDelimter.c_str() << "\n";
    dfile << "CustomProductName = " << CustomProductName.c_str() << "\n";
    dfile << "CustomProductTitle = " << CustomProductTitle.c_str() << "\n";
    dfile << "OutputInHTMLFormat = " << OutputInHTMLFormat.c_str() << "\n";
    dfile << "WriteJavaScriptCode = " << WriteJavaScriptCode.c_str() << "\n";
    dfile << "JavaScriptIncludeFile = " << JavaScriptIncludeFile.c_str() << "\n";
    dfile << "TextBodyColor = " << TextBodyColor.c_str() << "\n";
    dfile << "\n";
    dfile << "# Text body read parser elements" << "\n";
    dfile << "StartProductText = " << "\n";
    dfile << "EndProductText = " << "\n";
    dfile << "KeepStartProductText = " << KeepStartProductText.c_str() << "\n";
    dfile << "KeepEndProductText = " << KeepEndProductText.c_str() << "\n";
    dfile << "MultiLineText = " << MultiLineText.c_str() << "\n";
    dfile << "GetAllProductTextBlocks = " << GetAllProductTextBlocks.c_str() << "\n";
    dfile << "FilterStringFromText = " << "\n";
    dfile << "SkipEmptySegments = " << SkipEmptySegments.c_str() << "\n";
    dfile << "\n";
    dfile << "# Text header read configuration" << "\n";
    dfile << "ReadIssueOffice = " << ReadIssueOffice.c_str() << "\n";
    dfile << "ReadVTECMessage = " << ReadVTECMessage.c_str() << "\n";
    dfile << "ReadZoneNumbers = " << ReadZoneNumbers.c_str() << "\n";
    dfile << "ReadCountyCodes = " << ReadCountyCodes.c_str() << "\n";
    dfile << "ReadAreaNames = " << ReadAreaNames.c_str() << "\n";
    dfile << "ReadIncludingAreaNames = " << ReadIncludingAreaNames.c_str() << "\n";
    dfile << "\n";
    dfile << "# Text body read configuration" << "\n";
    dfile << "ReadPolygon = " << ReadPolygon.c_str() << "\n";
    dfile << "\n";
    dfile << "# Section titles" << "\n";
    dfile << "HasIssuedTitle = " << HasIssuedTitle.c_str() << "\n";
    dfile << "IssuedFromTitle = " << IssuedFromTitle.c_str() << "\n";
    dfile << "IssuedUntilTitle = " << IssuedUntilTitle.c_str() << "\n";
    dfile << "ZoneListingTitle = " << ZoneListingTitle.c_str() << "\n";
    dfile << "CountyListingTitle = " << CountyListingTitle.c_str() << "\n";
    dfile << "AreaListingTitle = " << AreaListingTitle.c_str() << "\n";
    dfile << "IncludingAreaListingTitle = " << IncludingAreaListingTitle.c_str() << "\n";
    dfile << "PolygonListingTitle = " << PolygonListingTitle.c_str() << "\n";
    dfile << "CanceledTitle = " << CanceledTitle.c_str() << "\n";
    dfile << "ExpiresTitle = " << ExpiresTitle.c_str() << "\n";
    dfile << "ExpiresAtTitle = " << ExpiresAtTitle.c_str() << "\n";
    dfile << "\n";
    dfile << "# Global text after the UGC header but before the first segment" << "\n";
    dfile << "# NOTE: Global text affects all ZONES in the CWA" << "\n";
    dfile << "ReadGlobalText = " << ReadGlobalText.c_str() << "\n";
    dfile << "ParseFromGlobalTextOnly = " << ParseFromGlobalTextOnly.c_str() << "\n";
    dfile << "CustomGlobalName = " << CustomGlobalName.c_str() << "\n";
    dfile << "CustomGlobalTitle = " << CustomGlobalTitle.c_str() << "\n";
    dfile << "KeepStartGlobalText = " << KeepStartGlobalText.c_str() << "\n";
    dfile << "KeepEndGlobalText = " << KeepEndGlobalText.c_str() << "\n";
    dfile << "StartGlobalText = " << StartGlobalText.c_str() << "\n";
    dfile << "EndGlobalText = " << EndGlobalText.c_str() << "\n";
    dfile << "GlobalParseSkipsStartProductText = " << GlobalParseSkipsStartProductText.c_str() << "\n";
    dfile << "MultiLineGlobalText = " << MultiLineGlobalText.c_str() << "\n";
    dfile << "GetAllProductGlobalTextBlocks = " << GetAllProductGlobalTextBlocks.c_str() << "\n";
    dfile << "FilterStringFromGlobalText = " << FilterStringFromGlobalText.c_str() << "\n";
    dfile << "SkipGlobalTextIfEmptySegments = " << SkipGlobalTextIfEmptySegments.c_str() << "\n";
    dfile << "\n";
    dfile << "# Setting for all text parsing" << "\n";
    dfile << "RegexOR = " << RegexOR.c_str() << "\n";
    dfile << "\n";
    dfile << "# RSS product section" << "\n";
    dfile << "# The parameters below are only use in RSS mode" << "\n";
    dfile << "CustomProductURL = " << CustomProductURL.c_str() << "\n";
    dfile << "CustomProductDescription = " << CustomProductDescription.c_str() << "\n";
    dfile << "RSSDefaultURL = " << RSSDefaultURL.c_str() << "\n";
    dfile << "RSSWebmaster = " << RSSWebmaster.c_str() << "\n";
    dfile << "RSS_version = " << RSS_version.c_str() << "\n";
    dfile << "XML_version = " << XML_version.c_str() << "\n";
    dfile << "RSSManagingEditor = " << RSSManagingEditor.c_str() << "\n";
    dfile << "RSSImageURL = " << RSSImageURL.c_str() << "\n";
    dfile << "RSSImageLink = " << RSSImageLink.c_str() << "\n";
    dfile << "RSSImageTitle = " << RSSImageTitle.c_str() << "\n";
    dfile << "RSSImageDescription = " << RSSImageDescription.c_str() << "\n";
    dfile << "RSSTextInputTitle = " << RSSTextInputTitle.c_str() << "\n";
    dfile << "RSSTextInputDescription = " << RSSTextInputDescription.c_str() << "\n";
    dfile << "RSSTextInputName = " << RSSTextInputName.c_str() << "\n";
    dfile << "RSSTextInputLink = " << RSSTextInputLink.c_str() << "\n";
    dfile << "RSSTextInputTitle = " << RSSTextInputTitle.c_str() << "\n";
    dfile << "# End of configuration file" << "\n";
    dfile << "\n";
    dfile.df_Close();
    return 1;
  }

  // Load the configuration 
  char *str = 0;

  // Load string values from the configuration file
  str = CfgData.GetStrValue("StartProductText");
  if(!str) {
    if(debug) NT_print("StartProductText parameter missing from config file");
  }
  else {
    StartProductText = str;
    StartProductText.ReplaceString("\\n", "\n");
    StartProductText.ReplaceString("\\r", "\r");
    StartProductText.ReplaceString("\\t", "\t");
  }

  str = CfgData.GetStrValue("EndProductText");
  if(!str) {
    if(debug) NT_print("EndProductText parameter missing from config file");
  }
  else {
    EndProductText = str;
    EndProductText.ReplaceString("\\n", "\n");
    EndProductText.ReplaceString("\\r", "\r");
    EndProductText.ReplaceString("\\t", "\t");
  }

  str = CfgData.GetStrValue("FilterStringFromText");
  if(!str) {
    if(debug) NT_print("FilterStringFromText parameter missing from config file");
  }
  else {
    FilterStringFromText = str;
    FilterStringFromText.ReplaceString("\\n", "\n");
    FilterStringFromText.ReplaceString("\\r", "\r");
    FilterStringFromText.ReplaceString("\\t", "\t");
  }


  str = CfgData.GetStrValue("ReadVerbose");
  if(!str) {
    if(debug) NT_print("ReadVerbose parameter missing from config file");
  }
  else {
    RPCfgFixTrueFalseString(ReadVerbose, str);
  }
  
  str = CfgData.GetStrValue("ReadAllUpper");
  if(!str) {
    if(debug) NT_print("ReadAllUpper parameter missing from config file");
  }
  else {
    RPCfgFixTrueFalseString(ReadAllUpper, str);
  }
  
  str = CfgData.GetStrValue("ReadIssueOffice");
  if(!str) {
    if(debug) NT_print("ReadIssueOffice parameter missing from config file");
  }
  else {
    RPCfgFixTrueFalseString(ReadIssueOffice, str);
  }
  
  str = CfgData.GetStrValue("ReadVTECMessage");
  if(!str) {
    if(debug) NT_print("ReadVTECMessage parameter missing from config file");
  }
  else {
    RPCfgFixTrueFalseString(ReadVTECMessage, str);
  }
  
  str = CfgData.GetStrValue("ReadZoneNumbers");
  if(!str) {
    if(debug) NT_print("ReadZoneNumbers parameter missing from config file");
  }
  else {
    RPCfgFixTrueFalseString(ReadZoneNumbers, str);
  }
  
  str = CfgData.GetStrValue("ReadCountyCodes");
  if(!str) {
    if(debug) NT_print("ReadCountyCodes parameter missing from config file");
  }
  else {
    RPCfgFixTrueFalseString(ReadCountyCodes, str);
  }
  
  str = CfgData.GetStrValue("ReadAreaNames");
  if(!str) {
    if(debug) NT_print("ReadAreaNames parameter missing from config file");
  }
  else {
    RPCfgFixTrueFalseString(ReadAreaNames, str);
  }
  
  str = CfgData.GetStrValue("ReadIncludingAreaNames");
  if(!str) {
    if(debug) NT_print("ReadIncludingAreaNames parameter missing from config file");
  }
  else {
    RPCfgFixTrueFalseString(ReadIncludingAreaNames, str);
  }
  
  str = CfgData.GetStrValue("ReadPolygon");
  if(!str) {
    if(debug) NT_print("ReadPolygon parameter missing from config file");
  }
  else {
    RPCfgFixTrueFalseString(ReadPolygon, str);
  }
  
  str = CfgData.GetStrValue("HasIssuedTitle");
  if(!str) {
    if(debug) NT_print("HasIssuedTitle parameter missing from config file");
  }
  else {
    HasIssuedTitle = str;
  }
  
  str = CfgData.GetStrValue("IssuedFromTitle");
  if(!str) {
    if(debug) NT_print("IssuedFromTitle parameter missing from config file");
  }
  else {
    IssuedFromTitle = str;
  }
  
  str = CfgData.GetStrValue("IssuedUntilTitle");
  if(!str) {
    if(debug) NT_print("IssuedUntilTitle parameter missing from config file");
  }
  else {
    IssuedUntilTitle = str;
  }
  
  str = CfgData.GetStrValue("ZoneListingTitle");
  if(!str) {
    if(debug) NT_print("ZoneListingTitle parameter missing from config file");
  }
  else {
    ZoneListingTitle = str;
  }

  str = CfgData.GetStrValue("CountyListingTitle");
  if(!str) {
    if(debug) NT_print("CountyListingTitle parameter missing from config file");
  }
  else {
    CountyListingTitle = str;
  }
  
  str = CfgData.GetStrValue("AreaListingTitle");
  if(!str) {
    if(debug) NT_print("AreaListingTitle parameter missing from config file");
  }
  else {
    AreaListingTitle = str;
  }
  
  str = CfgData.GetStrValue("IncludingAreaListingTitle");
  if(!str) {
    if(debug) NT_print("IncludingAreaListingTitle parameter missing from config file");
  }
  else {
    IncludingAreaListingTitle = str;
  }

  str = CfgData.GetStrValue("PolygonListingTitle");
  if(!str) {
    if(debug) NT_print("PolygonListingTitle parameter missing from config file");
  }
  else {
    PolygonListingTitle = str;
  }

  str = CfgData.GetStrValue("CanceledTitle");
  if(!str) {
    if(debug) NT_print("CanceledTitle parameter missing from config file");
  }
  else {
    CanceledTitle = str;
  }

  str = CfgData.GetStrValue("ExpiresTitle");
  if(!str) {
    if(debug) NT_print("ExpiresTitle parameter missing from config file");
  }
  else {
    ExpiresTitle = str;
  }

  str = CfgData.GetStrValue("ExpiresAtTitle");
  if(!str) {
    if(debug) NT_print("ExpiresAtTitle parameter missing from config file");
  }
  else {
    ExpiresAtTitle = str;
  }

  str = CfgData.GetStrValue("ReadProductDelimter");
  if(!str) {
    if(debug) NT_print("ReadProductDelimter parameter missing from config file");
  }
  else {
    ReadProductDelimter = str;
    ReadProductDelimter.TrimLeadingSpaces();
    ReadProductDelimter.TrimTrailingSpaces();
  }

  str = CfgData.GetStrValue("KeepStartProductText");
  if(!str) {
    if(debug) NT_print("KeepStartProductText parameter missing from config file");
  }
  else {
    RPCfgFixTrueFalseString(KeepStartProductText, str);
  }

  str = CfgData.GetStrValue("KeepEndProductText");
  if(!str) {
    if(debug) NT_print("KeepEndProductText parameter missing from config file");
  }
  else {
    RPCfgFixTrueFalseString(KeepEndProductText, str);
  }

  str = CfgData.GetStrValue("GetAllProductTextBlocks");
  if(!str) {
    if(debug) NT_print("GetAllProductTextBlocks parameter missing from config file");
  }
  else {
    RPCfgFixTrueFalseString(GetAllProductTextBlocks, str);
  }

  str = CfgData.GetStrValue("MultiLineText");
  if(!str) {
    if(debug) NT_print("MultiLineText parameter missing from config file");
  }
  else {
    RPCfgFixTrueFalseString(MultiLineText, str);
  }

  str = CfgData.GetStrValue("OutputInHTMLFormat");
  if(!str) {
    if(debug) NT_print("OutputInHTMLFormat parameter missing from config file");
  }
  else {
    RPCfgFixTrueFalseString(OutputInHTMLFormat, str);
  }

  str = CfgData.GetStrValue("CustomProductName");
  if(!str) {
    if(debug) NT_print("CustomProductName parameter missing from config file");
  }
  else {
    CustomProductName = str;
  }

  str = CfgData.GetStrValue("CustomProductTitle");
  if(!str) {
    if(debug) NT_print("CustomProductTitle parameter missing from config file");
  }
  else {
    CustomProductTitle = str;
  }

  str = CfgData.GetStrValue("WriteJavaScriptCode");
  if(!str) {
    if(debug) NT_print("WriteJavaScriptCode parameter missing from config file");
  }
  else {
    RPCfgFixTrueFalseString(WriteJavaScriptCode, str);
  }
  str = CfgData.GetStrValue("JavaScriptIncludeFile");
  if(!str) {
    if(debug) NT_print("JavaScriptIncludeFile parameter missing from config file");
  }
  else {
    JavaScriptIncludeFile = str;
  }
  str = CfgData.GetStrValue("SkipEmptySegments");
  if(!str) {
    if(debug) NT_print("SkipEmptySegments parameter missing from config file");
  }
  else {
    RPCfgFixTrueFalseString(SkipEmptySegments, str);
  }

  // 05/18/2009: Added parameter to set the text body color in HTML documents
  str = CfgData.GetStrValue("TextBodyColor");
  if(!str) {
    if(debug) NT_print("TextBodyColor parameter missing from config file");
  }
  else {
    TextBodyColor = str;
  }


  // 06/01/2009: Added to read and parse global text after the product header
  // 06/01/2009: but before the first segment
  // Load string values from the configuration file
  str = CfgData.GetStrValue("StartGlobalText");
  if(!str) {
    if(debug) NT_print("StartGlobalText parameter missing from config file");
  }
  else {
    StartGlobalText = str;
    StartGlobalText.ReplaceString("\\n", "\n");
    StartGlobalText.ReplaceString("\\r", "\r");
    StartGlobalText.ReplaceString("\\t", "\t");
  }

  str = CfgData.GetStrValue("EndGlobalText");
  if(!str) {
    if(debug) NT_print("EndGlobalText parameter missing from config file");
  }
  else {
    EndGlobalText = str;
    EndGlobalText.ReplaceString("\\n", "\n");
    EndGlobalText.ReplaceString("\\r", "\r");
    EndGlobalText.ReplaceString("\\t", "\t");
  }
  str = CfgData.GetStrValue("ReadGlobalText");
  if(!str) {
    if(debug) NT_print("ReadGlobalText parameter missing from config file");
  }
  else {
    RPCfgFixTrueFalseString(ReadGlobalText, str);
  }
  str = CfgData.GetStrValue("ParseFromGlobalTextOnly");
  if(!str) {
    if(debug) NT_print("ParseFromGlobalTextOnly parameter missing from config file");
  }
  else {
    RPCfgFixTrueFalseString(ParseFromGlobalTextOnly, str);
  }
  str = CfgData.GetStrValue("KeepStartGlobalText");
  if(!str) {
    if(debug) NT_print("KeepStartGlobalText parameter missing from config file");
  }
  else {
    RPCfgFixTrueFalseString(KeepStartGlobalText, str);
  }
  str = CfgData.GetStrValue("KeepEndGlobalText");
  if(!str) {
    if(debug) NT_print("KeepEndGlobalText parameter missing from config file");
  }
  else {
    RPCfgFixTrueFalseString(KeepEndGlobalText, str);
  }
  str = CfgData.GetStrValue("CustomGlobalName");
  if(!str) {
    if(debug) NT_print("CustomGlobalName parameter missing from config file");
  }
  else {
    CustomGlobalName = str;
  }
  str = CfgData.GetStrValue("CustomGlobalTitle");
  if(!str) {
    if(debug) NT_print("CustomGlobalTitle parameter missing from config file");
  }
  else {
    CustomGlobalTitle = str;
  }
  str = CfgData.GetStrValue("GlobalParseSkipsStartProductText");
  if(!str) {
    if(debug) NT_print("GlobalParseSkipsStartProductText parameter missing from config file");
  }
  else {
    RPCfgFixTrueFalseString(GlobalParseSkipsStartProductText, str);
  }
  str = CfgData.GetStrValue("MultiLineGlobalText");
  if(!str) {
    if(debug) NT_print("MultiLineGlobalText parameter missing from config file");
  }
  else {
    RPCfgFixTrueFalseString(MultiLineGlobalText, str);
  }
  str = CfgData.GetStrValue("GetAllProductGlobalTextBlocks");
  if(!str) {
    if(debug) NT_print("GetAllProductGlobalTextBlocks parameter missing from config file");
  }
  else {
    RPCfgFixTrueFalseString(GetAllProductGlobalTextBlocks, str);
  }
  str = CfgData.GetStrValue("FilterStringFromGlobalText");
  if(!str) {
    if(debug) NT_print("FilterStringFromGlobalText parameter missing from config file");
  }
  else {
    FilterStringFromGlobalText = str;
    FilterStringFromGlobalText.ReplaceString("\\n", "\n");
    FilterStringFromGlobalText.ReplaceString("\\r", "\r");
    FilterStringFromGlobalText.ReplaceString("\\t", "\t");
  }

  str = CfgData.GetStrValue("CustomGlobalName");
  if(!str) {
    if(debug) NT_print("CustomGlobalName parameter missing from config file");
  }
  else {
    CustomGlobalName = str;
  }
  str = CfgData.GetStrValue("SkipGlobalTextIfEmptySegments");
  if(!str) {
    if(debug) NT_print("SkipGlobalTextIfEmptySegments parameter missing from config file");
  }
  else {
    RPCfgFixTrueFalseString(SkipGlobalTextIfEmptySegments, str);
  }
  
  // 06/17/2009: Added code for RSS feed
  str = CfgData.GetStrValue("CustomProductURL");
  if(!str) {
    if(debug) NT_print("CustomProductURL parameter missing from config file");
  }
  else {
    CustomProductURL = str;
  }
  str = CfgData.GetStrValue("CustomProductDescription");
  if(!str) {
    if(debug) NT_print("CustomProductDescription parameter missing from config file");
  }
  else {
    CustomProductDescription = str;
  }
  str = CfgData.GetStrValue("RSSWebmaster");
  if(!str) {
    if(debug) NT_print("RSSWebmaster parameter missing from config file");
  }
  else {
    RSSWebmaster = str;
  }
  str = CfgData.GetStrValue("RSS_version");
  if(!str) {
    if(debug) NT_print("RSS_version parameter missing from config file");
  }
  else {
    RSS_version = str;
  }
  str = CfgData.GetStrValue("XML_version");
  if(!str) {
    if(debug) NT_print("XML_version parameter missing from config file");
  }
  else {
    XML_version = str;
  }
  str = CfgData.GetStrValue("RSSManagingEditor");
  if(!str) {
    if(debug) NT_print("RSSManagingEditor parameter missing from config file");
  }
  else {
    RSSManagingEditor = str;
  }
  str = CfgData.GetStrValue("RSSImageURL");
  if(!str) {
    if(debug) NT_print("RSSImageURL parameter missing from config file");
  }
  else {
    RSSImageURL = str;
  }
  str = CfgData.GetStrValue("RSSImageLink");
  if(!str) {
    if(debug) NT_print("RSSImageLink parameter missing from config file");
  }
  else {
    RSSImageLink = str;
  }
  str = CfgData.GetStrValue("RSSImageTitle");
  if(!str) {
    if(debug) NT_print("RSSImageTitle parameter missing from config file");
  }
  else {
    RSSImageTitle = str;
  }
  str = CfgData.GetStrValue("RSSImageDescription");
  if(!str) {
    if(debug) NT_print("RSSImageDescription parameter missing from config file");
  }
  else {
    RSSImageDescription = str;
  }
  str = CfgData.GetStrValue("RSSTextInputTitle");
  if(!str) {
    if(debug) NT_print("RSSTextInputTitle parameter missing from config file");
  }
  else {
    RSSTextInputTitle = str;
  }
  str = CfgData.GetStrValue("RSSTextInputDescription");
  if(!str) {
    if(debug) NT_print("RSSTextInputDescription parameter missing from config file");
  }
  else {
    RSSTextInputDescription = str;
  }
  str = CfgData.GetStrValue("RSSTextInputName");
  if(!str) {
    if(debug) NT_print("RSSTextInputName parameter missing from config file");
  }
  else {
    RSSTextInputName = str;
  }
  str = CfgData.GetStrValue("RSSTextInputLink");
  if(!str) {
    if(debug) NT_print("RSSTextInputLink parameter missing from config file");
  }
  else {
    RSSTextInputLink = str;
  }
  str = CfgData.GetStrValue("RSSTextInputTitle");
  if(!str) {
    if(debug) NT_print("RSSTextInputTitle parameter missing from config file");
  }
  else {
    RSSTextInputTitle = str;
  }
  str = CfgData.GetStrValue("RSSDefaultURL");
  if(!str) {
    if(debug) NT_print("RSSDefaultURL parameter missing from config file");
  }
  else {
    RSSDefaultURL = str;
  }

  str = CfgData.GetStrValue("RegexOR");
  if(!str) {
    if(debug) NT_print("RegexOR parameter missing from config file");
  }
  else {
    RegexOR = str;
  }

  return 1;
}

int TextProduct::grep_text(gxList<gxString> &text_segment, gxString &stext_buf,
			   const gxString &start_str_in, const gxString &end_str_in,
			   const gxString &keep_start_str, const gxString &keep_end_str,
			   const gxString &multi_line, const gxString &get_all,
			   const gxString &filter_str)
{
  stext_buf.Clear();
  if(text_segment.IsEmpty()) return 0;

  gxListNode<gxString> *sptr;

  stext_buf << "\n";
  int write_text = 0;
  int num_writes = 0;
  unsigned i;

  // Set delimiter for our || array
  gxString delimiter = RegexOR;

  unsigned num_start_arr = 0;
  gxString *start_arr = ParseStrings(start_str_in, delimiter, num_start_arr, 1, 1);
  if(!start_arr) start_arr = new gxString[1];
  if(num_start_arr < 1) {
    delete[] start_arr;
    start_arr = new gxString[1];
    start_arr[0] = start_str_in;
  }

  unsigned num_end_arr = 0;
  gxString *end_arr = ParseStrings(end_str_in, delimiter, num_end_arr, 1, 1);
  if(!end_arr) end_arr = new gxString[1];
  if(num_end_arr < 1) {
    delete[] end_arr;
    end_arr = new gxString[1];
    end_arr[0] = end_str_in;
  }
    
  sptr = text_segment.GetHead();
  while(sptr) {
    if(!start_str_in.is_null()) {
      int has_start = 0;
      gxString start_str;
      for(i = 0; i < num_start_arr; i++) {
	gxString stbuf = sptr->data.Left(start_arr[i].length());
	if(start_arr[i] == stbuf) {
	  has_start = 1;
	  start_str = start_arr[i];
	  break;
	}
      }
      if(has_start) {
	write_text = 1;
	// 06/15/2009: We are parsing the same line
	if(multi_line == "FALSE") {
	  int has_end = 0;
	  gxString end_str;
	  for(i = 0; i < num_end_arr; i++) {
	    gxString etbuf = sptr->data.Right(end_arr[i].length());
	    if(end_arr[i] == etbuf) {
	      has_end = 1;
	      end_str = end_arr[i];
	      break;
	    }
	  }
	  if(has_end) {
	    gxString lbuf = sptr->data.c_str();
	    if(keep_start_str == "FALSE") lbuf.DeleteAt(0, start_str.length());
	    if(keep_end_str == "FALSE") lbuf.DeleteAt((lbuf.length()-end_str.length()), end_str.length());
	    if(!filter_str.is_null()) {
	      lbuf.FilterString(filter_str);
	    }
	    stext_buf << lbuf;
	    if(get_all == "FALSE") break;
	  }
	  sptr = sptr->next;
	  continue;
	}
      }

      if(multi_line == "FALSE") {
	sptr = sptr->next;
	continue;
      }

      // We are pasring multiple lines
      gxString end_str;
      for(i = 0; i < num_end_arr; i++) {
	end_str = end_arr[i];
	if((!end_str.is_null()) && (num_writes > 0)) {
	  gxString end_str_buf = end_str; 

	  // The default is to look for the ending expression at the beginning the line
	  gxString etbuf = sptr->data.Left(end_str_buf.length());
	  int match_end_before_new_line = 0;
	  int match_end = 0;
	  int match_end_after_new_line = 0;
	  int write_last_line = 0;
	  
	  // Look for any \x regular expressions here 
	  if(end_str.length() > 2) {
	    if((end_str[end_str.length()-1] == 'Z') && (end_str[end_str.length()-2] == '\\')) {
	      // \Z regular expression 
	      match_end_before_new_line = 1;
	      write_last_line = 1;
	      end_str_buf.DeleteAt((end_str.length()-2), 2);
	    }
	    if((end_str[end_str.length()-1] == 'z') && (end_str[end_str.length()-2] == '\\')) {
	      // \z regular expression 
	      match_end = 1;
	      end_str_buf.DeleteAt((end_str.length()-2), 2);
	      write_last_line = 1;
	    }
	    if((end_str[end_str.length()-1] == 'A') && (end_str[end_str.length()-2] == '\\')) {
	      // \A regular expression 
	      match_end_after_new_line = 1;
	      end_str_buf.DeleteAt((end_str.length()-2), 2);
	    }
	  }
	  
	  if(match_end_after_new_line == 1) {
	    gxString etbuf_tmp1 = sptr->data.Left(end_str_buf.length());
	    etbuf = etbuf_tmp1;
	    if(!sptr->prev) {
	      etbuf.Clear();
	    }
	    else {
	      if(sptr->prev->data[0] != '\n') etbuf.Clear();
	    }
	  }
	  
	  if(match_end_before_new_line == 1) {
	    gxString etbuf_tmp1 = sptr->data.Right(end_str_buf.length());
	    etbuf = etbuf_tmp1;
	    if(!sptr->next) {
	      etbuf.Clear();
	    }
	    else {
	      if(sptr->next->data[0] != '\n') etbuf.Clear();
	    }
	  }

	  if(match_end == 1) {
	    gxString lbuf = sptr->data;
	    lbuf.FilterString("\n");
	    lbuf.FilterString("\r");
	    gxString etbuf_tmp1 = lbuf.Right(end_str_buf.length());
	    etbuf = etbuf_tmp1;
	  }
	  
	  if(etbuf == end_str_buf) {
	    write_text = 0;
	    if(write_last_line == 1) {
	      gxString lbuf = sptr->data.c_str();
	      if(match_end == 1) {
		lbuf.FilterString("\n");
		lbuf.FilterString("\r");
	      }
	      if(keep_end_str == "FALSE") lbuf.DeleteAt((lbuf.length()-end_str_buf.length()), end_str_buf.length()); 
	      stext_buf << lbuf;
	    }
	    if(keep_end_str == "TRUE") {
	      stext_buf << end_str_buf;
	    }

	    if(get_all == "FALSE") {
	      break;
	    }
	    num_writes = 0;
	  }
  
	} // End of multi-line parse
      }

      if(write_text) {
	gxString lbuf = sptr->data.c_str();
	if(num_writes == 0) {
	  if(keep_start_str == "FALSE") lbuf.DeleteAt(0, start_str.length()); 
	}
	// 06/15/2009: Filter string from the parsed line
	if(!filter_str.is_null()) {
	  lbuf.FilterString(filter_str);
	}

	stext_buf << lbuf;
	num_writes++;
      }
    }
    else {
      stext_buf << sptr->data.c_str();
    }
    sptr = sptr->next;
  }
  
  stext_buf.TrimLeading('\n');
  stext_buf.TrimLeadingSpaces();
  stext_buf.TrimTrailing('\n');
  stext_buf.TrimTrailingSpaces();

  delete[] start_arr;
  delete[] end_arr;

  return 1;
}
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
