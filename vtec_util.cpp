// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- // 
// C++ Source Code File Name: vtec_util.cpp
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

Utility used to read and decode VTEC product headers.
*/
// ----------------------------------------------------------- // 
#include "gxdlcode.h"

#if defined (__USE_ANSI_CPP__) // Use the ANSI Standard C++ library
#include <iostream>
using namespace std; // Use unqualified names for Standard C++ library
#else // Use the old iostream library by default
#include <iostream.h>
#endif // __USE_ANSI_CPP__

// 3plibs include files
#include "gxstring.h"
#include "dfileb.h"
#include "futils.h"
#include "gxconfig.h"
#include "systime.h"
#include "gxlist.h"
#include "gxshttp.h"
#include "gxshttpc.h"
#include "logfile.h"
#include "gxbstree.h"
#include "bstreei.h"
#include "gxsrss.h"

// Windows include files
#ifdef __MSVC_DEBUG__
#include "leaktest.h"
#endif

// Linux include files
#if defined (__LINUX__)
#include <sys/signal.h>
#ifndef __USE_SIGNAL_IO__
#define __USE_SIGNAL_IO__
#endif
#endif

// STD lib include files
#include <math.h>

// Forward class names
class UGCSegmentListNode;

// Project include files
#include "m_tables.h"
#include "m_poly.h"
#include "m_time.h"
#include "m_help.h"
#include "m_config.h"
#include "m_vtec.h"
#include "m_ugc.h"
#include "m_prod.h"
#include "m_util.h"
#include "m_google.h"
#include "m_java.h"
#include "m_javascript.h"
#include "m_warning_table.h"
#include "m_html.h"
#include "m_php.h"
#include "m_xml.h"
#include "m_rss.h"
#include "m_read_product.h"

// Version number and program name
const char *VersionString = "3.45";
const char *ProgramDescription = "VTEC text product utility";
const char *ProducedBy = "NOAA/NWS/SRH\n\
Program Support:    Douglas.Gaer@noaa.gov, Matthew.Duplantis@noaa.gov\n\
Algorithm Support:  Pablo.Santos@noaa.gov, Scott.Carroll@noaa.gov\n\
Google/GIS Support: Keith.Stellman@noaa.gov, Ira.Graffman@noaa.gov";

gxString ProgramName = "vtec_util"; // Default program name

// Constant global variables 
#if defined (__WIN32__)
const gxString path_sep = "\\";
#else
const gxString path_sep = "/";
#endif

// Global variables 
int user_config_file = 0; 
int num_command_line_args = 0;
int num_files = 0;
int list_all_cmd_args = 0;
gxList<gxString> product_list;
LogFile logfile; // Log file object

// Global configuration variables 
gxString config_file = "vtec_util.cfg";
gxString product_list_txt = "product_list.txt";
gxString WebpageOutputDirectory = "webpages";
int overwrite_output_webpage = 1;
// True if makeing static HTML or false if making PHP include file
int generate_html_page = 1; 
int generate_page_headers = 1; 
int strip_html_comments = 0;
int use_product_list_file = 0;
int download_retries = 5;
int download_retry_pause = 5;
#if defined (__WIN32__)
gxString spool_dir = "C:\\tmp\\vtec_util";
#else
gxString spool_dir = "/tmp/vtec_util";
#endif
int debug = 0;
int debug_level = 1;
int verbose = 0;
int use_logfile = 0;
int clear_logfile = 0;
int parse_product = 0;
int read_product = 0;
int rss_product = 0;
int make_google_map_page = 0;
int make_warning_table = 0;
int exec_file = 0;
int download_web_products = 0;
int max_file_open_tries = 30;
int file_open_wait_time = 10;
gxString exec_file_command;
gxString logfile_name = "vtec_util.log";
gxString service_name = ProgramName;

// 05/13/2009: Added Web product download to specified file option to allow
// 05/13/2009: program to request a text product if the product in the LDM
// 05/13/2009: database is expired or does not exist.
int download_web_product_and_exit = 0;
gxString download_web_product_name;
gxString download_web_product_file_name;

// Config file overrides
gxString output_dir;

// Function declarations
int ProcessProductList();
int LoadProductListFile();
int ExitMessage(int exit_code, const char *s1 = 0, 
		const char *s2 = 0, const char *s3 = 0);

// Program's main thread of execution.
// ----------------------------------------------------------- 
int main(int argc, char **argv)
{
#ifdef __MSVC_DEBUG__
  InitLeakTest();
#endif

  // Set this program's name
  ProgramName = argv[0];

  if(argc < 2) {
    ExitMessage(1, "\nProgram error:", "No product file or switches specified");
    VersionMessage();
    HelpMessage();
    return 1;
  }

  // Process command ling arguments and files 
  int narg;
  num_files = 0;
  char *arg = argv[narg = 1];
  gxString sbuf;

  // Load all the command line arguments
  while(narg < argc) {
    if(arg[0] != '\0') {
      if(arg[0] == '-') { // Look for command line arguments
	// Exit if argument is not valid or argument signals program to exit
	if(!ProcessArgs(argc, argv)) {
	  return 1; 
	}
      }
      else {
	sbuf = arg;
	// Add any filters to file or Web address here
	product_list.Add(sbuf);
	num_files++;
      }
    }
    arg = argv[++narg];
  }

  if(num_command_line_args == 0) {
    ExitMessage(1, "\nProgram error:", "No command line arguments specified");
    VersionMessage();
    HelpMessage();
    return 1;
  }

  // Load the product list from a text file
  if(use_product_list_file) {
    if(!LoadProductListFile()) {
      return ExitMessage(1, "Error loading product file list");
    }
  }

  // 05/13/2009: Add on demand Web product download feature
  if(download_web_product_and_exit) {
    gxString product_url;
    gxString product_url_backup;
    gxString fname;
    SetupDownloadURLS(product_url, product_url_backup);
    gxString SITEVAR = "NWS";
    gxString FORMATVAR = "TXT";
    gxString VERSIONVAR = "1";
    gxString GLOSSARYVAR = "0";
    gxString MYPRODUCT = download_web_product_name;
    gxString ISSUEDBYVAR = MYPRODUCT.Right(3);
    gxString PRODUCTVAR = MYPRODUCT.Left(3);

    gxString primary_site = product_url;
    primary_site.ReplaceString("MYPRODUCT", MYPRODUCT.c_str());
    primary_site.ReplaceString("SITEVAR", SITEVAR.c_str());
    primary_site.ReplaceString("ISSUEDBYVAR", ISSUEDBYVAR.c_str());
    primary_site.ReplaceString("PRODUCTVAR", PRODUCTVAR.c_str());
    primary_site.ReplaceString("FORMATVAR", FORMATVAR.c_str());
    primary_site.ReplaceString("VERSIONVAR", VERSIONVAR.c_str());
    primary_site.ReplaceString("GLOSSARYVAR", GLOSSARYVAR.c_str());

    if(!product_web_site_arg_backup.is_null()) {
      gxString backup_site = product_url_backup;
      backup_site.ReplaceString("MYPRODUCT", MYPRODUCT.c_str());
      backup_site.ReplaceString("SITEVAR", SITEVAR.c_str());
      backup_site.ReplaceString("ISSUEDBYVAR", ISSUEDBYVAR.c_str());
      backup_site.ReplaceString("PRODUCTVAR", PRODUCTVAR.c_str());
      backup_site.ReplaceString("FORMATVAR", FORMATVAR.c_str());
      backup_site.ReplaceString("VERSIONVAR", VERSIONVAR.c_str());
      backup_site.ReplaceString("GLOSSARYVAR", GLOSSARYVAR.c_str());
      if(!DownloadProduct(primary_site.c_str(), backup_site.c_str(), fname)) {
	return ExitMessage(1, "Error downloading product", primary_site.c_str());
      }
    }
    else {
      if(!DownloadProduct(primary_site.c_str(), fname)) {
	return ExitMessage(1, "Error downloading product", primary_site.c_str());
      }
    }
    if(download_web_product_file_name.is_null()) {
      download_web_product_file_name = download_web_product_name;
    }
    DiskFileB dfile;
    dfile.df_Open(fname.c_str());
    if(!dfile) {
      return ExitMessage(1, "Error opening file", fname.c_str());
    }
    char linebuf[1024];
    gxString outbuf;
    while(!dfile.df_EOF()) {
      // Get each line of the file and trim the line feeds
      dfile.df_GetLine(linebuf, sizeof(linebuf), '\n', 0);
      if(dfile.df_GetError() != DiskFileB::df_NO_ERROR) {
	return ExitMessage(1, "Error reading file", fname.c_str());
      }
      outbuf << linebuf;
    }
    dfile.df_Close();
    if((outbuf.IFind("<html>") != -1) || (outbuf.IFind("</html>") != -1)) {
      return ExitMessage(1, "Web product is not valid ", fname.c_str());
    }
    dfile.df_Create(download_web_product_file_name.c_str());
    if(!dfile) {
      return ExitMessage(1, "Error creating file", download_web_product_file_name.c_str());
    }
    if(dfile.df_Write((const char *)outbuf.c_str(), outbuf.length()) != 0) {
      return ExitMessage(1, "Error writing file", download_web_product_file_name.c_str());
    }
    return ExitMessage(0, "Download complete", download_web_product_file_name.c_str());
  }

  // Load the user configuration file
  if(user_config_file) {
    if(!LoadOrBuildConfigFile()) {
      return ExitMessage(1, "Error loading config file", config_file.c_str());
    }
    if((num_files == 0) && (!make_warning_table)) {
      return ExitMessage(0, "\nProgram configuration file built successfully");
    }
  }

  if((num_files == 0) && (!read_product_config_file.is_null())) {
    if(!LoadOrBuildReadProductConfigFile()) {
      return ExitMessage(1, "Error building read product formatting config file", 
			 read_product_config_file.c_str());
    }
    return ExitMessage(0, "\nRead product formatting configuration file built successfully");
  }

  // Setup and test the global variables after processing the args and CFG file
  if(WebpageOutputDirectory == "DEFAULT") {
    WebpageOutputDirectory = "webpages";
  }
  if(!output_dir.is_null()) {
    WebpageOutputDirectory = output_dir;
  }
  WebpageOutputDirectory.TrimTrailing('\\');
  WebpageOutputDirectory.TrimTrailing('/');

  if(debug) {
    NT_print("Turning on debug mode");
    if(verbose) NT_print("Turning on verbose mode");

    if(make_warning_table) {
      NT_print("Creating warning table page");
      NT_print("Warning table CWA", WarningTableCWA.c_str());
      NT_print("Data directory", WarningTableData.c_str());
      if(WarningTableArchiveMode == "1") NT_print("Archive mode is on");
    }

    if(parse_product) NT_print("Parsing product");
    if(read_product) NT_print("Reading product");
    if(rss_product) NT_print("Reading product as RSS feed");
    if(make_google_map_page) NT_print("Creating Google map page");
    if(!generate_html_page) NT_print("Generating Web pages using PHP include file format");
    if(!generate_html_page) NT_print("PHP include file using file extension", php_file_extension.c_str());
  }

  // Check the file list from the command line and/or text file input
  if((num_files == 0) && (!make_warning_table)) {
    ExitMessage(1, "\nProgram error:", "No product file or Web address specified");  
    VersionMessage();
    HelpMessage();
    return 1;
  }

  // Start logging operations here
  if(clear_logfile) {
    NT_print("Clearing log file", logfile_name.c_str());
    logfile.Create(logfile_name.c_str());
    logfile.Close();
  }

  // Load the program configuration tables here
  if(!LoadVTECTables()) {
    return ExitMessage(1, "Error loading VTEC tables");
  }
  if(!LoadTimezoneTables()) {
    return ExitMessage(1, "Error loading timezone tables");
  }

  int num_offset_hours; 
  gxString TZ;

  if(GetSystemTimeOffset(num_offset_hours, TZ)) {
    if(num_offset_hours != 0) {
      NT_print("Your system clock is not set to GMT time");
      gxString tmessage;
      tmessage << clear << "Will offset by " << num_offset_hours << " hours";
      NT_print(tmessage.c_str());
      if(!TZ.is_null()) {
	NT_print("Your system timezone is", TZ.c_str());
      }
    }
  }
  else {
    if(debug) NT_print("Error reading your system timezone offset");
  }

  if(make_warning_table) {
    if(!generate_warning_table_page()) {
      return ExitMessage(1, "Error generating the warning table");
    }
    
    if(dump_zone_table) {
      zone_table_file << clear << WarningTableCWA << "-" << "dynamic_zone_table.txt";
      if(!dynamic_zone_table_name.is_null()) {
	zone_table_file = dynamic_zone_table_name;
      }
      DumpDynamicZoneTable(zone_table_file, WarningTableCWA);

      if(dynamic_zone_js_name.is_null()) {
	dynamic_zone_js_name << clear << WarningTableCWA << "_zones.js";
	dynamic_zone_js_name.ToLower();
      }
      DumpDynamicZoneJS(dynamic_zone_js_name, WarningTableCWA);
    }

    if(dump_trigger_table) {
      gxString t_file;
      t_file << clear << WarningTableCWA << "-" << "trigger_listing.txt";
      if(!trigger_table_name.is_null()) {
	t_file = trigger_table_name;
      }
      DumpTriggerTable(t_file, WarningTableCWA);
    }

    return ExitMessage(0);
  }

  // Process the product list and preform all user defined actions
  if(!ProcessProductList()) {
    return ExitMessage(1, "Error processing product list");
  }

  if(dump_zone_table) {
    zone_table_file << clear << WarningTableCWA << "-" << "dynamic_zone_table.txt";
    if(!dynamic_zone_table_name.is_null()) {
      zone_table_file = dynamic_zone_table_name;
    }
    DumpDynamicZoneTable(zone_table_file, WarningTableCWA);
  }

  if(dump_trigger_table) {
    gxString t_file;
    t_file << clear << WarningTableCWA << "-" << "trigger_listing.txt";
    if(!trigger_table_name.is_null()) {
      t_file = trigger_table_name;
    }
    DumpTriggerTable(t_file, WarningTableCWA);
  }

  return ExitMessage(0);
}

int ProcessProductList()
{
  NT_print("Processing product list");

  if(product_list.IsEmpty()) {
    NT_print("No product files or Web addresses in product list");
    return 0;
  }
  gxListNode<gxString> *lptr = product_list.GetHead();
  gxString open_fname, sbuf;
  TextProduct text_product;
  int num_products = 0;

  while(lptr) {
    open_fname = lptr->data;

    gxString http = open_fname.Left(7);
    gxString https = open_fname.Left(8);
    http.ToLower(); https.ToLower();
    if((http == "http://") || (https == "https://")) {
      if(https == "https://") {
	NT_print("HTTPS protocol is currently not supported");
	NT_print("Skipping URL:", open_fname.c_str());
	lptr = lptr->next;
	continue;
      }     
      NT_print("Processing Web request");
      // Add any needed URL filter to the sbuf copy 
      sbuf = lptr->data;
      // NOTE: This will change open_fname to a file system name path
      if(!DownloadProduct(sbuf.c_str(), open_fname)) {
	lptr = lptr->next;
	continue;
      }
    } 
    if(open_fname.Find("://") != -1) { 
      // Check for unsupported protocols
      sbuf = lptr->data;
      sbuf.DeleteAfterIncluding("://");
      sbuf.ToUpper();
      NT_print(sbuf.c_str(), "protocol is currently not supported");
      NT_print("Skipping URL:", open_fname.c_str());
      lptr = lptr->next;
      continue;
    }

    // Process the system file name
    if(futils_isdirectory(open_fname.c_str())) {
      // Skip directory names
      NT_print(open_fname.c_str(), "is a directory name");
      NT_print("Skipping directory:", open_fname.c_str());
      open_fname.Clear(); // Reset the open file name
      lptr = lptr->next;
      continue;
    }

    if(!futils_exists(open_fname.c_str())) {
      // Skip directory names
      NT_print(open_fname.c_str(), "file does not exist");
      NT_print("Skipping:", open_fname.c_str());
      open_fname.Clear(); // Reset the open file name
      lptr = lptr->next;
      continue;
    }
	
    // Process file(s) here
    NT_print("Processing file:", open_fname.c_str());

    if(!text_product.ParseProduct(open_fname.c_str())) {
      NT_print("Error loading and parsing product file");
      NT_print(text_product.error_string.c_str());
      NT_print("Skipping file:", open_fname.c_str());
      open_fname.Clear(); // Reset the open file name
      lptr = lptr->next;
      continue;
    }

    NT_print("Processing complete");

    int num_actions = 0;

    // NOTE: Add all product actions
    if(parse_product) {
      text_product.PrintProductInfo();
      num_actions++;
    }
    if(read_product) {
      text_product.ReadProduct();
      num_actions++;
    }
    if(rss_product) {
      text_product.RSSProduct();
      num_actions++;
    }
    if(make_google_map_page) {
      generate_google_map_page(text_product);
      num_actions++;
    }

    if(num_actions == 0) {
      NT_print("No action specified");
    }

    num_products++;
    lptr = lptr->next;
  }

  NT_print("Finished processing product list");
  if(num_products == 1) {
    sbuf << clear << "Processed " << num_products << " product";
  }
  else if(num_products == 0) {
    NT_print("Did not process any products");
    if(!verbose) {
      NT_console_write_err("Error did not process any products from the product list");
      NT_console_write_err("Use --verbose argument for details");
    }
    return 0;
  }
  else {
    sbuf << clear << "Processed " << num_products << " products";
  }
  NT_print(sbuf.c_str());

  return 1;
}

int LoadProductListFile()
{
  if(!use_product_list_file) return 0;
  char sbuf[1024];
  DiskFileB dfile;

  NT_print("Loading product list file", product_list_txt.c_str());
 
  // Open the current product file
  dfile.df_Open(product_list_txt.c_str());
  if(!dfile) {
    NT_print("Cannot open product list file", product_list_txt.c_str());
    NT_print(dfile.df_ExceptionMessage());
    return 0;
  }

  while(!dfile.df_EOF()) {
    // Get each line of the file and trim the line feeds
    dfile.df_GetLine(sbuf, sizeof(sbuf), '\n');
    if(dfile.df_GetError() != DiskFileB::df_NO_ERROR) {
      NT_print("A fatal I/O error occurred while reading product list file");
      NT_print("Cannot read file", product_list_txt.c_str());
      NT_print(dfile.df_ExceptionMessage());
      break;
    }
    
    UString info_line(sbuf);
    info_line.FilterChar('\n');
    info_line.FilterChar('\r');
    info_line.TrimLeadingSpaces();
    info_line.TrimTrailingSpaces();

    if(info_line.is_null()) continue;
    
    // Skip remark lines
    if(info_line[0] == '#') continue; 
    if(info_line[0] == ';') continue; 

    if(debug) NT_print("Adding product file entry", info_line.c_str());
    product_list.Add(info_line);
    num_files++;
  }

  dfile.df_Close();

  NT_print("Product list file load complete");
  return 1;
}

int ExitMessage(int exit_code, const char *s1, const char *s2, const char *s3)
{
  if(s1) NT_console_write_err(s1, s2, s3);

  if(exit_code == 0) {
    NT_console_write_err("Exiting program...");
    return exit_code;
  }
  NT_console_write_err("Exiting program with errors!!!");
  return exit_code;
}

// Include source code files
#include "m_vtec.cpp"
#include "m_ugc.cpp"
#include "m_prod.cpp"
#include "m_config.cpp"
#include "m_util.cpp"
#include "m_tables.cpp"
#include "m_time.cpp"
#include "m_poly.cpp"
#include "m_google.cpp"
#include "m_help.cpp"
#include "m_warning_table.cpp"
#include "m_html.cpp"
#include "m_php.cpp"
#include "m_xml.cpp"
#include "m_rss.cpp"
#include "m_java.cpp"
#include "m_javascript.cpp"
#include "m_read_product.cpp"
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
