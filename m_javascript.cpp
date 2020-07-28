// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- // 
// C++ Source Code File Name: m_javascript.cpp
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

JavaScript helper code
*/
// ----------------------------------------------------------- // 
#include "gxdlcode.h"

int MakeJSPopupProduct(UGCSegmentListNode *ugc_node)
{
  if(!ugc_node) return 0;

  ugc_node->jscode.Clear();
  gxString sbuf;

  if(ugc_node->text.IsEmpty()) {
    return 0;
  }

  gxListNode<gxString> *segptr = ugc_node->text.GetHead();
  gxString piletn;
  gxString productfile;
  gxString product = ugc_node->pil;
  if(product.length() == 9) product.DeleteAt(0, 3);
  if(product.length() == 6) product.DeleteAt(3, 3);

  piletn << clear << product << ugc_node->etn;;

  productfile.Clear();
  productfile << ugc_node->wmo_header 
	      << "\n" 
	      << ugc_node->full_pil 
	      << "\n" 
	      << "\n";
  productfile << ugc_node->issue_office 
	      << "\n" 
	      << ugc_node->name 
	      << "\n" 
	      << ugc_node->issue_time.time_string
    	      << "\n"; 
  if(segptr) {
    if(segptr->data != "\n") productfile << "\n";
  }

  if(!ugc_node->ugc_segment_header.is_null()) {
    productfile << ugc_node->ugc_segment_header;
  }

  // Write the text from the UGC segment
  while(segptr) {
    productfile << segptr->data;
    segptr = segptr->next;
  }

  ugc_node->jscode << "function popUp" << piletn << "() {" << "\n";

  ugc_node->jscode << piletn << "win = window.open(\'\', \'" << piletn 
		   << "\', 'toolbar=" << WarningTableJStoolbar 
		   << ",scrollbars=" << WarningTableJSscrollbars 
		   << ",location=" << WarningTableJSlocation 
		   << ",statusbar=" << WarningTableJSstatusbar
		   << ",menubar=" << WarningTableJSmenubar
		   << ",resizable=" << WarningTableJSresizable
		   << ",width=" << WarningTableJSwidth
		   << ",height=" << WarningTableJSheight
		   << ",left=" << WarningTableJSleft
		   << ",top=" << WarningTableJStop
		   << "');" << "\n";
  

  JSdocumentWriteLine(ugc_node->jscode, piletn, "<html>");
  JSdocumentWriteLine(ugc_node->jscode, piletn, "<head>");

  // Write the meta tags needed for text pages
  JSdocumentWriteLine(ugc_node->jscode, piletn, 
		      "<meta http-equiv=\\\"content-type\\\" content=\\\"text/html; charset=utf-8\\\"/>");

  // Write the page title
  sbuf << clear << "<title>" << ugc_node->name << "</title>";
  JSdocumentWriteLine(ugc_node->jscode, piletn, sbuf.c_str());
  JSdocumentWriteLine(ugc_node->jscode, piletn, "</head>");

  JSdocumentWriteLine(ugc_node->jscode, piletn, 
		      "<body bgcolor=\\\"#ffffff\\\" link=\\\"#0000ff\\\" vlink=\\\"#800080\\\">");

  JSdocumentWrite(ugc_node->jscode, piletn, "<pre>");

  // Write the main bode of code
  JSdocumentWriteFileLines(ugc_node->jscode, piletn, productfile); 

  JSdocumentWriteLine(ugc_node->jscode, piletn, "</pre>");

  // Close the HTML document here
  JSdocumentWriteLine(ugc_node->jscode, piletn, "</body>");
  JSdocumentWriteLine(ugc_node->jscode, piletn, "</html>");

  ugc_node->jscode << piletn << "win.document.close();" << "\n";
  ugc_node->jscode << "}" << "\n";

  

  return MakeJSProductIconLink(ugc_node);
}

int MakeJSProductIconLink(UGCSegmentListNode *ugc_node)
{
  // Initalize the icon_link object
  ugc_node->icon_link.link.Clear();
  ugc_node->MakeTitleString();

  gxString sbuf, images_dir;
  gxString piletn;
  gxString product = ugc_node->pil;
  gxString imgtag, ahreftab;
  if(product.length() == 9) product.DeleteAt(0, 3);
  if(product.length() == 6) product.DeleteAt(3, 3);

  piletn << clear << product << ugc_node->etn;

  if(!icon_dir_on_web_server.is_null()) {
    images_dir = icon_dir_on_web_server;
  }
  else {
    images_dir = WarningTableIcons; 
  }

  if(images_dir == "DEFAULT") {
    sbuf = WarningTableCWA;
    sbuf.ToLower();
    images_dir << clear << "/images/" << sbuf << "/hazards";
  }

  images_dir.TrimTrailing('/');

  ahreftab << clear << "<a href=\"#\" onClick=\"popUp" << piletn 
	   << "();return false;\">";
  imgtag << clear << "<img src=\"" << images_dir << "/" << ugc_node->pil 
	 << ".gif" << "\" alt=\"" << ugc_node->pil << "\""
	 << " border=\"0\" align=\"absmiddle\" height=\"" 
	 << WarningTableIconHeight 
	 << "\" width=\""<< WarningTableIconWidth << "\" hspace=\"0\"" 
	 << " title=\"" << ugc_node->icon_link.title << "\""<< ">";
  ugc_node->icon_link.link << ahreftab << "\n" << imgtag << "</a>";
  return 1;
}

int MakeJSGoogleMapPopupProduct(gxString &jscode, TextProduct &text_product, UGC &ugc_segment, 
				int current_ugc_segment, const gxString pil, const gxString &etn, 
				const gxString &product_name)
{
  jscode.Clear();
  gxString sbuf;

  gxString piletn;
  gxString product = pil;
  if(product.length() == 9) product.DeleteAt(0, 3);
  if(product.length() == 6) product.DeleteAt(3, 3);
  piletn << clear << product << etn;

  gxString webpage;
  gxString pagefilename; 
  gxString fname;
  
  // Get the orginal global configuation values
  int generate_html_page_org = generate_html_page;
  int generate_page_headers_org = generate_page_headers; 
  int strip_html_comments_org = strip_html_comments;

  generate_html_page = 1;
  generate_page_headers = 0;
  strip_html_comments = 1;

  if(debug) strip_html_comments = 0;

  gxString jsfile;
  int rv = generate_google_map_page(text_product, ugc_segment, current_ugc_segment, 
				    webpage, pagefilename, fname, jsfile);

  // Restore the orginal global configuation values
  generate_html_page = generate_html_page_org;
  generate_page_headers = generate_page_headers_org;
  strip_html_comments = strip_html_comments_org;

  if(!rv) {
    // No Google map page was generated
    return 0;
  }

  jscode << "function popUp" << piletn << "() {" << "\n";

  // Strip out the Google map JavaScript code so it can be 
  // reinserted in the document <head> </head>
  gxString jsincludes;
  int offset1 = 0;
  int offset2 = 0;
  offset1 = webpage.Find("<!-- BEGIN BLOCK1 -->"); 
  if(offset1 != -1) offset2 = webpage.Find("<!-- END BLOCK1 -->", offset1); 
  if((offset1 != -1) && (offset2 > 0)) {
    gxString tmpbuf1(webpage, offset1, (offset2-offset1));
    jsincludes = tmpbuf1;
    jsincludes.FilterString("<!-- BEGIN BLOCK1 -->");
    jsincludes.FilterString("<!-- END BLOCK1 -->");
    webpage.DeleteAt(offset1, (offset2-offset1));
    webpage.FilterString("<!-- END BLOCK1 -->");
  }

  jscode << piletn << "win = window.open(\'\', \'" << piletn 
	 << "\', 'toolbar=" << WarningTableJStoolbar 
	 << ",scrollbars=" << WarningTableJSscrollbars 
	 << ",location=" << WarningTableJSlocation 
    	 << ",statusbar=" << WarningTableJSstatusbar
    	 << ",menubar=" << WarningTableJSmenubar
	 << ",resizable=" << WarningTableJSresizable
	 << ",width=" << WarningTableJSwidth
	 << ",height=" << WarningTableJSheight
	 << ",left=" << WarningTableJSleft
	 << ",top=" << WarningTableJStop
	 << "');" << "\n";

  // Write the document types need for the map pages
  JSdocumentWriteLine(jscode, piletn, 
		      "<!DOCTYPE html PUBLIC \\\"-//W3C//DTD XHTML 1.0 Strict//EN\\\" \\\"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\\\">");
  JSdocumentWriteLine(jscode, piletn, 
		       "<html xmlns=\\\"http://www.w3.org/1999/xhtml\\\" xmlns:v=\\\"urn:schemas-microsoft-com:vml\\\">");

  JSdocumentWriteLine(jscode, piletn, "<html>");
  JSdocumentWriteLine(jscode, piletn, "<head>");

  // Write the meta tags needed for map page
  JSdocumentWriteLine(jscode, piletn, 
		      "<meta http-equiv=\\\"content-type\\\" content=\\\"text/html; charset=utf-8\\\"/>");

  // Write the JAVA script code
  JSdocumentWriteLine(jscode, piletn, "<!-- BEGIN BLOCK1 -->");
  JSdocumentWriteFileLines(jscode, piletn, jsincludes); 
  JSdocumentWriteLine(jscode, piletn, "<!-- END BLOCK1 -->");

  // Write the page title
  sbuf << clear << "<title>" << product_name << "</title>";
  JSdocumentWriteLine(jscode, piletn, sbuf.c_str());
  JSdocumentWriteLine(jscode, piletn, "</head>");

  JSdocumentWriteLine(jscode, piletn, 
		      "<body onload=\\\"load()\\\" onunload=\\\"GUnload()\\\" bgcolor=\\\"#ffffff\\\" link=\\\"#0000ff\\\" vlink=\\\"#800080\\\">");

  // Write the main bode of code
  JSdocumentWriteFileLines(jscode, piletn, webpage); 

  // Close the HTML document here
  JSdocumentWriteLine(jscode, piletn, "</body>");
  JSdocumentWriteLine(jscode, piletn, "</html>");

  jscode << piletn << "win.focus();" << "\n";

  // NOTE: We cannot close until the Google and SRH JS code loads
  // NOTE: Must time the JavaScript code load before we can close this
  // NOTE: document window. This applies to FireFox only.
  // jscode << piletn << "win.document.close();" << "\n";

  jscode << "}" << "\n";

  return 1;
}

void JSdocumentWriteLine(gxString &jscode, const gxString &piletn, const char *s)
{
  gxString sbuf;
  sbuf << clear << s << "\\n";
  JSdocumentWrite(jscode, piletn, sbuf.c_str());
}

void JSdocumentWrite(gxString &jscode, const gxString &piletn, const char *s) 
{
  jscode << piletn << "win.document.write(\"" << s << "\");" << "\n";
}

void JSdocumentWriteFileLines(gxString &jscode, const gxString &piletn, const gxString &filelines) 
{
  gxString sbuf;
  gxString linebuffer = filelines;

  // Write the JavaScript code to load when called here
  // Ensure we have a single leading and terminating linefeed
  linebuffer.TrimLeading('\n');
  linebuffer.TrimTrailing('\n');
  linebuffer.InsertAt(0, "\n");
  linebuffer << "\n";
  int offset1 = 0;
  int offset2 = 0;
  while(offset1 != -1 && offset2 != -1) {
    offset1 = linebuffer.Find("\n", offset1);
    if(offset1 == -1) break;
    offset2 = linebuffer.Find("\n", offset1+1);
    if(offset2 == -1) break;
    gxString line(linebuffer, offset1, (offset2-(offset1 - 1)));
    // Filter-out all the linefeeds
    line.TrimLeading('\n'); line.TrimTrailing('\n');
    line.ReplaceString("\"", "\\\""); // Fix the quotation marks 

    if(line.is_null()) {
      JSdocumentWrite(jscode, piletn, "\\n");
    }
    else {
      if(line.Find("</script>") != -1) {
	sbuf << clear << line;
	line.DeleteAfterIncluding("</script>");
	sbuf.DeleteBeforeIncluding("</script>");
	if(!line.is_null()) {
	  JSdocumentWrite(jscode, piletn, line.c_str());
	}
	else {
	  JSdocumentWrite(jscode, piletn, "\\n");
	}
	JSdocumentWrite(jscode, piletn, "</");
	JSdocumentWrite(jscode, piletn, "script");
	JSdocumentWrite(jscode, piletn, ">");
	if(!sbuf.is_null()) {
	  JSdocumentWriteLine(jscode, piletn, sbuf.c_str());
	}
	else {
	  JSdocumentWrite(jscode, piletn, "\\n");
	}
      }
      else {
	JSdocumentWriteLine(jscode, piletn, line.c_str());
      }
    }
    offset1++;
  }
}

int MakeJSGoogleMapFile(TextProduct &text_product, UGC &ugc_segment, 
			int current_ugc_segment, gxString &pagefilename,
			gxString &fname)
{
  pagefilename.Clear(); 
  fname.Clear();

  gxString sbuf;
  gxString webpage;

  int generate_html_page_org = generate_html_page;

  if(use_google_map_html_format) {
    // Force to HTML format
    generate_html_page = 1;
  }
  if(use_google_map_php_format) {
    // Force to PHP format
    generate_html_page = 0;
  }

  gxString js_file;
  int rv = generate_google_map_page(text_product, ugc_segment, current_ugc_segment,
				    webpage, pagefilename, fname, js_file);

  // Restore the orginal global configuation values
  generate_html_page = generate_html_page_org;

  if(!rv) {
    // No Google map page was generated
    return 0;
  }

  rv = WriteWebPageFile(fname, pagefilename, webpage);

  if(gen_js_files) {
    gxString js_file_name = fname;
    js_file_name.DeleteAfterLastIncluding(".");
    gxString js_pagefilename = pagefilename;
    js_pagefilename.DeleteAfterLastIncluding(".");
    js_file_name << ".js";
    js_pagefilename << ".js";
    WriteWebPageFile(js_file_name, js_pagefilename, js_file);
  }

  return rv;
}

int MakeJSFileIconLink(gxString &jscode, const gxString &pil, 
		       gxString &pagefilename, const gxString &title)
{
  jscode.Clear();

  gxString sbuf, images_dir;
  gxString imgtag, ahreftab;

  if(!icon_dir_on_web_server.is_null()) {
    images_dir = icon_dir_on_web_server;
  }
  else {
    images_dir = WarningTableIcons; 
  }

  if(images_dir == "DEFAULT") {
    sbuf = WarningTableCWA;
    sbuf.ToLower();
    images_dir << clear << "/images/" << sbuf << "/hazards";
  }

  images_dir.TrimTrailing('/');

  ahreftab << clear << "<a href=\"" << pagefilename << "\" target=\"_blank\">";

  imgtag << clear << "<img src=\"" << images_dir << "/" << pil 
	 << ".gif" << "\" alt=\"" << pil << "\""
	 << " border=\"0\" align=\"absmiddle\" height=\"" 
	 << WarningTableIconHeight 
	 << "\" width=\""<< WarningTableIconWidth << "\" hspace=\"0\"" 
	 << " title=\"" << title << "\""<< ">";
  jscode << ahreftab << "\n" << imgtag << "</a>";
  return 1;
}

int MakeJSPopupKeyTable(gxString &jscode, gxString &ahreftab, const gxString &table)
{
  jscode.Clear();
  ahreftab.Clear();

  if(table.is_null()) return 0;

  gxString sbuf;
  gxString piletn = "KeyTable";
  gxString product = table;

  jscode << "function popUp" << piletn << "() {" << "\n";

  jscode << piletn << "win = window.open(\'\', \'" << piletn 
		   << "\', 'toolbar=" << KeyTableJStoolbar 
		   << ",scrollbars=" << KeyTableJSscrollbars 
		   << ",location=" << KeyTableJSlocation 
		   << ",statusbar=" << KeyTableJSstatusbar
		   << ",menubar=" << KeyTableJSmenubar
		   << ",resizable=" << KeyTableJSresizable
		   << ",width=" << KeyTableJSwidth
		   << ",height=" << KeyTableJSheight
		   << ",left=" << KeyTableJSleft
		   << ",top=" << KeyTableJStop
		   << "');" << "\n";
  

  JSdocumentWriteLine(jscode, piletn, "<html>");
  JSdocumentWriteLine(jscode, piletn, "<head>");

  // Write the meta tags needed for text pages
  JSdocumentWriteLine(jscode, piletn, 
		      "<meta http-equiv=\\\"content-type\\\" content=\\\"text/html; charset=utf-8\\\"/>");

  // Write the page title
  sbuf << clear << "<title>" << "Key Table" << "</title>";
  JSdocumentWriteLine(jscode, piletn, sbuf.c_str());
  JSdocumentWriteLine(jscode, piletn, "</head>");

  JSdocumentWriteLine(jscode, piletn, 
		      "<body bgcolor=\\\"#ffffff\\\" link=\\\"#0000ff\\\" vlink=\\\"#800080\\\">");

  // Write the main bode of code
  JSdocumentWriteFileLines(jscode, piletn, product); 

  // Close the HTML document here
  JSdocumentWriteLine(jscode, piletn, "</body>");
  JSdocumentWriteLine(jscode, piletn, "</html>");

  jscode << piletn << "win.document.close();" << "\n";
  jscode << "}" << "\n";

  ahreftab << clear << "<a href=\"#\" onClick=\"popUp" << piletn 
	   << "();return false;\">" << KeyTableLinkName << "</a>";

  return 1;
}

int MakeJSPopupVerTable(gxString &jscode, gxString &ahreftab, const gxString &table)
{
  jscode.Clear();
  ahreftab.Clear();

  if(table.is_null()) return 0;

  gxString sbuf;
  gxString piletn = "VerTable";
  gxString product = table;

  jscode << "function popUp" << piletn << "() {" << "\n";

  jscode << piletn << "win = window.open(\'\', \'" << piletn 
		   << "\', 'toolbar=" << VerTableJStoolbar 
		   << ",scrollbars=" << VerTableJSscrollbars 
		   << ",location=" << VerTableJSlocation 
		   << ",statusbar=" << VerTableJSstatusbar
		   << ",menubar=" << VerTableJSmenubar
		   << ",resizable=" << VerTableJSresizable
		   << ",width=" << VerTableJSwidth
		   << ",height=" << VerTableJSheight
		   << ",left=" << VerTableJSleft
		   << ",top=" << VerTableJStop
		   << "');" << "\n";
  

  JSdocumentWriteLine(jscode, piletn, "<html>");
  JSdocumentWriteLine(jscode, piletn, "<head>");

  // Write the meta tags needed for text pages
  JSdocumentWriteLine(jscode, piletn, 
		      "<meta http-equiv=\\\"content-type\\\" content=\\\"text/html; charset=utf-8\\\"/>");

  // Write the page title
  sbuf << clear << "<title>" << "Verification Table" << "</title>";
  JSdocumentWriteLine(jscode, piletn, sbuf.c_str());
  JSdocumentWriteLine(jscode, piletn, "</head>");

  JSdocumentWriteLine(jscode, piletn, 
		      "<body bgcolor=\\\"#ffffff\\\" link=\\\"#0000ff\\\" vlink=\\\"#800080\\\">");

  // Write the main bode of code
  JSdocumentWriteFileLines(jscode, piletn, product); 

  // Close the HTML document here
  JSdocumentWriteLine(jscode, piletn, "</body>");
  JSdocumentWriteLine(jscode, piletn, "</html>");

  jscode << piletn << "win.document.close();" << "\n";
  jscode << "}" << "\n";

  ahreftab << clear << "<a href=\"#\" onClick=\"popUp" << piletn 
	   << "();return false;\">" << VerTableLinkName << "</a>";

  return 1;
}

int WriteJavaScriptTail(gxString &jscode)
{
  jscode << "\n";
  jscode << "// ----------------------------------------------------------- //" << "\n";
  jscode << "// ------------------------------- //" << "\n";
  jscode << "// --------- End of File --------- //" << "\n";
  jscode << "// ------------------------------- //" << "\n";
  jscode << "\n";
  return 1;
}

int WriteJavaScriptHeader(const gxString &fname,  
			  const gxString &description, gxString &jscode)
{
  gxString sbuf;

  jscode.Clear();
  jscode << "// ------------------------------- //" << "\n";
  jscode << "// -------- Start of File -------- //" << "\n";
  jscode << "// ------------------------------- //" << "\n";
  jscode << "// ----------------------------------------------------------- // " << "\n";
  sbuf = fname;
  sbuf.DeleteBeforeIncluding("\\");
  sbuf.DeleteBeforeIncluding("/");
  jscode << "// JavaScript File Name: " << sbuf << "\n";
  jscode << "// Browser support: Internet Explorer, Netscape, Firefox" << "\n";
  jscode << "// Produced By: " << ProgramDescription << " version " << VersionString << "\n";
  SysTime systime;
  gxString year = systime.GetStrTime(SysTime::Year);
  gxString month = systime.GetStrTime(SysTime::Month);
  gxString day = systime.GetStrTime(SysTime::DayOfTheMonth);
  jscode << "// File Creation " << "Date: " << month << "/" << day << "/" << year << "\n";
  jscode << "// Date Last " << "Modified: " << month << "/" << day << "/" << year << "\n";
  jscode << "// ----------------------------------------------------------- // " << "\n";
  jscode << "// ------------- Program Description and Details ------------- // " << "\n";
  jscode << "// ----------------------------------------------------------- // " << "\n";
  jscode << "//" << "\n";
  jscode << "// " << description << "\n";
  jscode << "//" << "\n";
  jscode << "// ----------------------------------------------------------- // " << "\n";
  jscode << "\n";
  return 1;
}

int WriteJavaScriptFile(const gxString &fname, const gxString &description, const gxString &jscode)
{
  gxString jsbuf;
  
  WriteJavaScriptHeader(fname, description, jsbuf);
  jsbuf << jscode;
  WriteJavaScriptTail(jsbuf);

  DiskFileB dfile;
  dfile.df_Create(fname.c_str());
  if(!dfile) {
    NT_print("Error creating JavaScript file");
    NT_print("Cannot create", fname.c_str());
    NT_print(dfile.df_ExceptionMessage());
    GXSTD::cout << "Error-jsfile-create" << GXSTD::flush;
    dfile.df_Close();
    return 0;
  }
  if(dfile.df_Write((const char *)jsbuf.c_str(), 
		    jsbuf.length()) != 0) {
    NT_print("Error writing JavaScript file");
    NT_print("Cannot write to", fname.c_str());
    NT_print(dfile.df_ExceptionMessage());
    GXSTD::cout << "Error-jsfile-write" << GXSTD::flush;
    dfile.df_Close();
    return 0;
  }
  dfile.df_Close();
  
  return 1;
}
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //

