// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- // 
// C++ Source Code File Name: m_config.cpp
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

Program configuration
*/
// ----------------------------------------------------------- // 
#include "gxdlcode.h"

int ProcessDashDashArg(gxString &arg)
{
  gxString sbuf, equal_arg;
  int has_valid_args = 0;
  
  if(arg.Find("=") != -1) {
    // Look for equal arguments
    // --config-file="/etc/vtec_util.cfg"
    equal_arg = arg;
    equal_arg.DeleteBeforeIncluding("=");
    arg.DeleteAfterIncluding("=");
    equal_arg.TrimLeading(' '); equal_arg.TrimTrailing(' ');
    equal_arg.TrimLeading('\"'); equal_arg.TrimTrailing('\"');
    equal_arg.TrimLeading('\''); equal_arg.TrimTrailing('\'');
  }

  arg.ToLower();

  // Process all -- arguments here
  if(arg == "help") {
    HelpMessage();
    return 0; // Signal program to exit
  }
  if(arg == "help-all") {
    list_all_cmd_args = 1;
    HelpMessage();
    return 0; // Signal program to exit
  }
  if(arg == "?") {
    HelpMessage();
    return 0; // Signal program to exit
  }

  if((arg == "version") || (arg == "ver")) {
    VersionMessage();
    return 0; // Signal program to exit
  }

  if(arg == "debug") {
    debug = 1;
    if(!equal_arg.is_null()) { 
      debug_level = equal_arg.Atoi(); 
    }
    has_valid_args = 1;
  }
  if(arg == "verbose") {
    verbose = 1;
    has_valid_args = 1;
  }
  if(arg == "config-file") {
    if(equal_arg.is_null()) { 
      GXSTD::cout << "Error no config file name supplied with the --config-file swtich" 
		  << "\n" << flush;
      return 0;
    }
    has_valid_args = 1;
    user_config_file = 1;
    config_file = equal_arg;
  }
  if(arg == "product-file") {
    if(equal_arg.is_null()) { 
      GXSTD::cout << "Error no product list file name supplied with the --product-file swtich" 
		  << "\n" << flush;
      return 0;
    }
    has_valid_args = 1;
    use_product_list_file = 1;
    product_list_txt = equal_arg;
  }
  if(arg == "exec-file") {
    if(equal_arg.is_null()) { 
      GXSTD::cout << "Error no product list file name supplied with the --exec-file swtich" 
		  << "\n" << flush;
      return 0;
    }
    has_valid_args = 1;
    exec_file = 1;
    exec_file_command = equal_arg;
  }

  // Start warning table input argument processing
  if(arg == "warning-table") {
    make_warning_table = 1;
    has_valid_args = 1;
  }
  if(arg == "warning-table-php") {
    make_warning_table = 1;
    generate_html_page = 0;
    has_valid_args = 1;
    if(!equal_arg.is_null()) {
      php_file_extension = equal_arg;
      php_file_extension.TrimTrailing('.');
      if(php_file_extension[0] != '.') {
	php_file_extension.InsertAt(0, ".");
      }
    }
  }

  if(arg == "data-dir") {
    if(equal_arg.is_null()) { 
      GXSTD::cout << "Error no directory name supplied with the --data-dir swtich" 
		  << "\n" << flush;
      return 0;
    }
    has_valid_args = 1;
    input_data_dir = equal_arg;    
  }
  if(arg == "get-web-products") {
    download_web_products = 1;
    has_valid_args = 1;
    if(!equal_arg.is_null()) {
      product_web_site_arg = equal_arg;
    }
  }
  if(arg == "arch-mode") {
    if(!equal_arg.is_null()) { 
      equal_arg.ToUpper();
      if(equal_arg == "TEST") {
	archive_test_mode = 1;
      }
    }
    use_archive_mode = 1;
    has_valid_args = 1;
  }
  if(arg == "cwa") {
    if(equal_arg.is_null()) { 
      GXSTD::cout << "Error no CWA name supplied with the --cwa swtich" 
		  << "\n" << flush;
      return 0;
    }
    input_cwa = equal_arg; 
    has_valid_args = 1;
  }
  if(arg == "icon-dir") {
    if(equal_arg.is_null()) { 
      GXSTD::cout << "Error no server icon DIR supplied with the --icon-dir swtich" 
		  << "\n" << flush;
      return 0;
    }
    icon_dir_on_web_server = equal_arg; 
    has_valid_args = 1;
  }

  if(arg == "google-maps-popup") {
    make_google_map_page = 1;
    use_google_map_js_popup = 1;
    has_valid_args = 1;
  }
  if(arg == "google-maps-html") {
    make_google_map_page = 1;
    use_google_map_html_format = 1;
    has_valid_args = 1;
  }
  if(arg == "google-maps-php") {
    make_google_map_page = 1;
    use_google_map_php_format = 1;
    has_valid_args = 1;
  }
  if(arg == "match-dynamic-names") {
    use_dynamic_zone_table = 1;
    match_dynamic_zone_names = 1;
    has_valid_args = 1;
  }
  if(arg == "no-dynamic-zones") {
    use_dynamic_zone_table = 0;
    match_dynamic_zone_names = 0;
    has_valid_args = 1;
  }
  if(arg == "dump-zone-table") {
    if(!equal_arg.is_null()) dynamic_zone_table_name = equal_arg;
    dump_zone_table = 1;
    has_valid_args = 1;
  }
  if(arg == "dump-trigger-list") {
    if(!equal_arg.is_null()) trigger_table_name = equal_arg;
    dump_trigger_table = 1;
    has_valid_args = 1;
  }
  if(arg == "drop-unknown-products") {
    drop_unknown_products = 1;
    has_valid_args = 1;
  }
  if(arg == "drop-routine-products") {
    drop_routine_products = 1;
    has_valid_args = 1;
  }
  if(arg == "no-overwrite") {
    overwrite_output_webpage = 0;
    has_valid_args = 1;
  }
  if(arg == "match-codes-only") {
    match_zones_numbers_only = 1;
    has_valid_args = 1;
  }
  if(arg == "no_key_table") {
    add_key_table = 0;
    add_key_table_in_page = 0;
    has_valid_args = 1;
  }
  if(arg == "key-table-in-page") {
    add_key_table = 1;
    add_key_table_in_page = 1;
    has_valid_args = 1;
  }
  if(arg == "no-ver-table") {
    add_ver_table = 0;
    add_ver_table_in_page = 0;
    has_valid_args = 1;
  }
  if(arg == "ver-table-in-page") {
    add_ver_table = 1;
    add_ver_table_in_page = 1;
    has_valid_args = 1;
  }
  if(arg == "ver-worksheets") {
    add_ver_worksheets = 1;
    has_valid_args = 1;
  }

  if(arg == "shortfused-table") {
    add_short_fused_table = 1;
    has_valid_args = 1;
    if(!equal_arg.is_null()) short_fused_table_php_file = equal_arg;
  }

  if(arg == "ver-worksheets-overwrite") {
    overwrite_output_worksheet = 1;
    add_ver_worksheets = 1;
    has_valid_args = 1;
  }
  if(arg == "worksheet-out-dir") {
    if(equal_arg.is_null()) { 
      GXSTD::cout << "Error no worksheet DIR supplied with the --worksheet-output-dir" 
		  << "\n" << flush;
      return 0;
    }
    worksheet_output_dir = equal_arg; 
    has_valid_args = 1;
  }
  if(arg == "worksheet-print-cmd") {
    if(equal_arg.is_null()) { 
      GXSTD::cout << "Error no print command supplied with the --worksheet-print-cmd" 
		  << "\n" << flush;
      return 0;
    }
    worksheet_print_cmd = equal_arg; 
    has_valid_args = 1;
  }
  if(arg == "force-worksheet-test") {
    force_worksheet_test = 1;
    has_valid_args = 1;
  }
  if(arg == "dump-html-tables") {
    dump_html_tables = 1;
    has_valid_args = 1;
  }
  if(arg == "dump-warning-table") {
    if(!equal_arg.is_null()) {
      warning_table_html_file = equal_arg;
      warning_table_js_file = warning_table_html_file;
      warning_table_js_file.DeleteAfterLastIncluding(".");
      warning_table_js_file << ".js";
    }
    dump_warning_table = 1;
    has_valid_args = 1;
  }
  if(arg == "dump-key-table") {
    if(!equal_arg.is_null()) {
      key_table_html_file = equal_arg;
    }
    dump_key_table = 1;
    has_valid_args = 1;
  }
  if(arg == "dump-ver-table") {
    if(!equal_arg.is_null()) {
      ver_table_html_file = equal_arg;
    }
    dump_ver_table = 1;
    has_valid_args = 1;
  }
  if(arg == "add-sas-refresh-tag") {
    add_sas_refresh_tag = 1;
    has_valid_args = 1;
    if(!equal_arg.is_null()) {
      sbuf << clear << equal_arg;
      sas_refresh_seconds = sbuf.Atoi();
    }
  }
  if(arg == "disable-empty-tables") {
    has_valid_args = 1;
    allow_empty_tables = 0;
  }
  // End warning table input argument processing

  if(arg == "output-dir") {
    if(equal_arg.is_null()) { 
      GXSTD::cout << "Error no directory name supplied with the --output-dir swtich" 
		  << "\n" << flush;
      return 0;
    }
    has_valid_args = 1;
    output_dir = equal_arg;    
  }

  if(arg == "log-file") {
    if(equal_arg.is_null()) { 
      GXSTD::cout << "Error no log file name supplied with the --log-file swtich" 
		  << "\n" << flush;
      return 0;
    }
    has_valid_args = 1;
    use_logfile = 1;
    logfile_name = equal_arg;
  }
  if(arg == "log-file-clear") {
    clear_logfile = 1;
    has_valid_args = 1;
    if(!equal_arg.is_null()) { 
      use_logfile = 1;
      logfile_name = equal_arg;
    }
  }

  if(arg == "parse-product") {
    parse_product = 1;
    has_valid_args = 1;
    return has_valid_args;
  }
  if(arg == "read-product") {
    read_product = 1;
    has_valid_args = 1;
    if(!equal_arg.is_null()) { 
      read_product_formatting = 1;
      read_product_config_file = equal_arg;
    }
    return has_valid_args;
  }

  if(arg == "rss") {
    rss_feed = 1;
    has_valid_args = 1;
    if(!equal_arg.is_null()) { 
      rss_file_ext = equal_arg;
    }
    return has_valid_args;
  }

  if(arg == "rss-outdir") {
    if(equal_arg.is_null()) { 
      GXSTD::cout << "Error no directory name supplied with the --rss-outdir swtich" 
		  << "\n" << flush;
      return 0;
    }
    has_valid_args = 1;
    rss_output_dir = equal_arg;    
  }

  if(arg == "rss-product") {
    rss_product = 1;
    has_valid_args = 1;
    if(!equal_arg.is_null()) { 
      read_product_formatting = 1;
      read_product_config_file = equal_arg;
    }
    return has_valid_args;
  }

  if(arg == "google-maps") {
    make_google_map_page = 1;
    has_valid_args = 1;
  }
  if(arg == "gen-js-files") {
    gen_js_files = 1;
    has_valid_args = 1;
  }

  if(arg == "php") {
    generate_html_page = 0;
    has_valid_args = 1;
    if(!equal_arg.is_null()) {
      php_file_extension = equal_arg;
      php_file_extension.TrimTrailing('.');
      if(php_file_extension[0] != '.') {
	php_file_extension.InsertAt(0, ".");
      }
    }
  }

  if(arg == "html") {
    generate_html_page = 1;
    has_valid_args = 1;
    if(!equal_arg.is_null()) {
      html_file_extension = equal_arg;
      html_file_extension.TrimTrailing('.');
      if(html_file_extension[0] != '.') {
	html_file_extension.InsertAt(0, ".");
      }
    }
  }

  if(arg == "html-include") {
    generate_html_page = 1;
    generate_page_headers = 0;
    has_valid_args = 1;
    if(!equal_arg.is_null()) {
      html_file_extension = equal_arg;
      html_file_extension.TrimTrailing('.');
      if(html_file_extension[0] != '.') {
	html_file_extension.InsertAt(0, ".");
      }
    }
  }

  if(arg == "js-ext") {
    if(equal_arg.is_null()) { 
      GXSTD::cout << "Error no DOT extension supplied with --js-ext switch" 
		  << "\n" << flush;
      return 0;
    }
    has_valid_args = 1;
    js_file_extension = equal_arg;
    js_file_extension.TrimTrailing('.');
    if(js_file_extension[0] != '.') {
      js_file_extension.InsertAt(0, ".");
    }
  }

  // 05/13/2009: Added switch to set primary and backup product download sites from the command line
  if(arg == "download-site") {
    if(equal_arg.is_null()) { 
      GXSTD::cout << "Error no site name supplied with the --download-site swtich" 
		  << "\n" << flush;
      return 0;
    }
    has_valid_args = 1;
    product_web_site_arg = equal_arg;
  }
  if(arg == "download-site2") {
    if(equal_arg.is_null()) { 
      GXSTD::cout << "Error no backup no site name supplied with the --download-site2 swtich" 
		  << "\n" << flush;
      return 0;
    }
    has_valid_args = 1;
    product_web_site_arg_backup = equal_arg;
  }

  // 05/13/2009: Added Web product download to specified file option to allow
  // 05/13/2009: program to request a text product if the product in the LDM
  // 05/13/2009: database is expired or does not exist.
  if(arg == "product-download") {
    if(equal_arg.is_null()) { 
      GXSTD::cout << "Error no CCCNNN product name supplied with the --product-download swtich" 
		  << "\n" << flush;
      return 0;
    }
    download_web_product_and_exit = 1;
    download_web_product_name = equal_arg;
    has_valid_args = 1;
  }
  if(arg == "download-fname") {
    if(equal_arg.is_null()) { 
      GXSTD::cout << "Error no file name supplied with the --download-fname swtich" 
		  << "\n" << flush;
      return 0;
    }
    download_web_product_file_name = equal_arg;
    has_valid_args = 1;
  }

  if(arg == "download-retries") {
    if(equal_arg.is_null()) { 
      GXSTD::cout << "Error no value supplied with the --download-retries swtich" 
		  << "\n" << flush;
      return 0;
    }
    download_retries = equal_arg.Atoi();
    if(download_retries <= 0) download_retries = 5;
    has_valid_args = 1;
  }
  if(arg == "download-pause") {
    if(equal_arg.is_null()) { 
      GXSTD::cout << "Error no value supplied with the --download-pause swtich" 
		  << "\n" << flush;
      return 0;
    }
    download_retry_pause = equal_arg.Atoi();
    if(download_retry_pause <= 0) download_retry_pause = 5;
    has_valid_args = 1;
  }

  if(!has_valid_args) {
    GXSTD::cout << "Error unknown command line switch --" << arg << "\n" << flush;
  }

  arg.Clear();
  return has_valid_args;
}

int ProcessArgs(int argc, char *argv[])
{
  // process the program's argument list
  int i;
  gxString sbuf;
  num_command_line_args = 0;

  for(i = 1; i < argc; i++ ) {
    if(*argv[i] == '-') {
      char sw = *(argv[i] +1);
      switch(sw) {
	case '?':
	  HelpMessage();
	  return 0; // Signal program to exit

	case '-':
	  sbuf = &argv[i][2]; 
	  // Add all -- prepend filters here
	  sbuf.TrimLeading('-');
	  if(!ProcessDashDashArg(sbuf)) return 0;
	  break;


	default:
	  GXSTD::cout << "Error unknown command line switch " << argv[i] << "\n" << flush;
	  return 0;
      }
      num_command_line_args++;
    }
  }
  
  return 1; // All command line arguments were valid
}

void CfgFixTrueFalseString(gxString &out, char *in)
{
  out.Clear();
  if(!in) return;
  out = in;
  out.ToUpper();
  out.TrimLeadingSpaces();
  out.TrimTrailingSpaces();
}

int LoadOrBuildConfigFile()
{
  gxConfig CfgData;
  gxString sbuf;

  NT_print("Loading config file", config_file.c_str());
  int status = CfgData.Load(config_file.c_str());

  if(!status) {
    if(user_config_file) {
      NT_print("User specified config file not found");
    }
    else {
      NT_print("Config file not found");
    }
    NT_print("Building default configuration file");

    DiskFileB dfile(config_file.c_str(), DiskFileB::df_READWRITE,
		    DiskFileB::df_CREATE); 
    if(!dfile) {
      NT_print("Cannot write to config file", config_file.c_str());
      return 0;
    }
    dfile << "#  " << ProgramDescription << " " << VersionString << " configuration file" << "\n";
    dfile << "#" << "\n";
    dfile << "\n";
    dfile << "# Program configuration" << "\n";
    dfile << "spool_dir = " << spool_dir.c_str() << "\n";
    dfile << "service_name = " << service_name.c_str() << "\n";
    dfile << "\n";
    dfile << "\n";
    dfile << "# File system configuration" << "\n";
    sbuf << clear << "max_file_open_tries = " << max_file_open_tries;
    dfile << sbuf.c_str() << "\n";
    sbuf << clear << "file_open_wait_time = " << file_open_wait_time;
    dfile << sbuf.c_str() << "\n";
    dfile << "\n";
    dfile << "\n";
    dfile << "# Comms configuration" << "\n";
    dfile << "# NOTE: The settings below are global and will affect all retry parameters" << "\n";
    dfile << "##retries = 5" << "\n";
    dfile << "##retry_pause = 5"  << "\n";
    dfile << "# Set our download retry parameters" << "\n";
    sbuf << clear << "download_retries = " << download_retries;
    dfile << sbuf.c_str() << "\n";
    sbuf << clear << "download_retry_pause = " << download_retry_pause;
    dfile << sbuf.c_str() << "\n";
    dfile << "\n";
    dfile << "\n";
    dfile << "# User defined table files" << "\n";
    dfile << "area_table_file = " << area_table_file.c_str() << "\n";
    dfile << "zone_table_file = " << zone_table_file.c_str() << "\n";
    dfile << "product_table_file = " << product_table_file.c_str() << "\n";
    dfile << "\n";
    dfile << "\n";
    dfile << "# Timezone configuration" << "\n";
    dfile << "# Timezone table CSV syntax = ZZZ(offset),ZZZ(offset)" << "\n";
    dfile << "# ZZZ = timezone name" << "\n";
    dfile << "# offset = number of hours to offset from GMT" << "\n";
    dfile << "Timezone_codes = " << Timezone_codes.c_str() << "\n";
    dfile << "\n";
    dfile << "\n";
    dfile << "# Follow statement configuration" << "\n";
    dfile << "# Followup PIL CSV syntax = AAA,AAA,AAA" << "\n";
    dfile << "Followup_statement_pils = " << Followup_statement_pils.c_str() << "\n";
    dfile << "# Phenomena code to pil CSV syntax = PP AAA,PP AAA" << "\n";
    dfile << "# PP = 2 letter VTEC phenomena code" << "\n";
    dfile << "# AAA = 3 letter product PIL" << "\n";
    dfile << "Followup_phenomena_code_to_pil = " << Followup_phenomena_code_to_pil.c_str() << "\n";
    dfile << "\n";
    dfile << "\n";
    dfile << "# VTEC tables" << "\n";
    dfile << "# Status code CSV syntax = Code Description,Code Description,..." << "\n";
    dfile << "PVTEC_status_codes = " << PVTEC_status_codes.c_str() << "\n";
    dfile << "\n";
    dfile << "\n";
    dfile << "# Action code CSV syntax = Code Description,Code Description,..." << "\n";
    dfile << "PVTEC_action_codes = " << PVTEC_action_codes.c_str() << "\n";
    dfile << "\n";
    dfile << "\n";
    dfile << "# Significance code CSV syntax = Code Description,Code Description,..." << "\n";
    dfile << "PVTEC_significance_codes = " << PVTEC_significance_codes.c_str() << "\n";
    dfile << "\n";
    dfile << "\n";
    dfile << "# Phenomena code CSV syntax = Code Description,Code Description,..." << "\n";
    dfile << "PVTEC_phenomena_codes = " << PVTEC_phenomena_codes.c_str() << "\n";
    dfile << "\n";
    dfile << "\n";
    dfile << "# Flood severity code CSV syntax = Code Description,Code Description,..." << "\n";
    dfile << "HVTEC_flood_severity_codes = " << HVTEC_flood_severity_codes.c_str() << "\n";
    dfile << "\n";
    dfile << "\n";
    dfile << "# Immediate cause code CSV syntax = Code Description,Code Description,..." << "\n";
    dfile << "HVTEC_immediate_cause_codes = " << HVTEC_immediate_cause_codes.c_str() << "\n";
    dfile << "\n";
    dfile << "\n";
    dfile << "# Flood record code CSV syntax = Code Description,Code Description,..." << "\n";
    dfile << "HVTEC_flood_record_codes = " << HVTEC_flood_record_codes.c_str() << "\n";
    dfile << "\n";
    dfile << "\n";
    dfile << "# Global Web page configuration" << "\n";
    dfile << "WebpageOutputDirectory = " << WebpageOutputDirectory.c_str() << "\n";
    dfile << "\n";
    dfile << "\n";
    dfile << "####################################" << "\n";
    dfile << "# Google maps Web page configuration" << "\n";
    dfile << "#" << "\n";
    dfile << "# Set to the virtual Web root location on the Web server" << "\n";
    dfile << "GoogleMapsWebServerLocation = " << GoogleMapsWebServerLocation.c_str() << "\n";
    dfile << "#" << "\n";
    dfile << "GoogleMaps3URL = " << GoogleMaps3URL.c_str() << "\n";
    dfile << "GoogleMaps3Key = " << GoogleMaps3Key.c_str() << "\n";
    dfile << "GoogleMapsVersion = " << GoogleMapsVersion.c_str() << "\n";
    dfile << "GoogleMapsChannel = " << GoogleMapsChannel.c_str() << "\n";
    dfile << "GoogleMapsClientName = " << GoogleMapsClientName.c_str() << "\n";
    dfile << "GoogleMapsWMSTransparentLayersScript = " << GoogleMapsWMSTransparentLayersScript.c_str() << "\n";
    dfile << "GoogleMapsWMSGAL = " << GoogleMapsWMSGAL.c_str() << "\n";
    dfile << "GoogleMapsRadarOverlay = " << GoogleMapsRadarOverlay.c_str() << "\n";
    dfile << "GoogleMapsGIcon = " << GoogleMapsGIcon.c_str() << "\n";
    dfile << "GoogleMapsWebPageName = " << GoogleMapsWebPageName.c_str() << "\n";
    dfile << "GoogleMapsDefaultTextColor = " << GoogleMapsDefaultTextColor.c_str() << "\n";
    dfile << "GoogleMapsTextHighlightColor = " << GoogleMapsTextHighlightColor.c_str() << "\n";
    dfile << "GoogleMapsTextExpiredHighlightColor = " << GoogleMapsTextExpiredHighlightColor.c_str() << "\n";
    dfile << "GoogleMapsDefaultZoom = " << GoogleMapsDefaultZoom.c_str() << "\n";
    dfile << "GoogleMapsTextColorByProduct = " << GoogleMapsTextColorByProduct.c_str() << "\n";
    dfile << "GoogleMapsZoomByProduct = " << GoogleMapsZoomByProduct.c_str() << "\n";
    sbuf << clear << "GooleMapsPrintExpiredMessage = " << GooleMapsPrintExpiredMessage;
    dfile << sbuf.c_str() << "\n";
    sbuf << clear << "GoogleMapsTextOverlay = " << GoogleMapsTextOverlay;
    dfile << sbuf.c_str() << "\n";
    sbuf << clear << "GoogleMapsWidth = " << GoogleMapsWidth;
    dfile << sbuf.c_str() << "\n";
    sbuf << clear << "GoogleMapsHeight = " << GoogleMapsHeight;
    dfile << sbuf.c_str() << "\n";
    dfile << "#" << "\n";
    dfile << "# Version 2 legacy code" << "\n";                                                     
    dfile << "#NOTE: As of May 2013 Google maps V2 will no longer work" << "\n";                                                         
    dfile << "GoogleMapsURL = " << GoogleMapsURL.c_str() << "\n";
    dfile << "GoogleMapsKey = " << GoogleMapsKey.c_str() << "\n";
    dfile << "GoogleMapsWMSSpec = " << GoogleMapsWMSSpec.c_str() << "\n";
    dfile << "GoogleMapsWMSOverlaySpec = " << GoogleMapsWMSOverlaySpec.c_str() << "\n";
    dfile << "#" << "\n";
    dfile << "# Set to the absolute UNIX path where the PHP include file is located" << "\n";
    dfile << "GoogleMapsPHPHeaderIncludeCMS = " << GoogleMapsPHPHeaderIncludeCMS.c_str() << "\n";
    dfile << "GoogleMapsPHPHeaderIncludeSite = " << GoogleMapsPHPHeaderIncludeSite.c_str() << "\n";
    dfile << "GoogleMapsPHPLeftIncludeCMS = " << GoogleMapsPHPLeftIncludeCMS.c_str() << "\n";
    dfile << "GoogleMapsPHPLeftIncludeSite = " << GoogleMapsPHPLeftIncludeSite.c_str() << "\n";
    dfile << "GoogleMapsPHPFooterIncludeSite = " << GoogleMapsPHPFooterIncludeSite.c_str() << "\n";
    dfile << "GoogleMapsPHPFooterIncludeCMS = " << GoogleMapsPHPFooterIncludeCMS.c_str() << "\n";
    dfile << "#" << "\n";
    dfile << "GoogleMapsDisplayDisclaimer = " << GoogleMapsDisplayDisclaimer.c_str() << "\n";
    dfile << "GoogleMapsDisclaimer = " << GoogleMapsDisclaimer.c_str() << "\n";
    dfile << "#" << "\n";
    dfile << "# NEW CMS settings that allow you to call a product request script for Google map pages." << "\n";
    dfile << "# This way the Google map files can be written as HTML files with a .txt extension to" << "\n";
    dfile << "# the site's /www/apps-data/localwfo/xxx/google_maps/wwa directory. The product request" << "\n";
    dfile << "# PHP script in the site's /www/html/xxx Web root will load the Google map HTML as an" << "\n";
    dfile << "# include text file." << "\n";
    dfile << "#" << "\n";
    dfile << "GoogleMapsWebServerRequestScript = " << GoogleMapsWebServerRequestScript.c_str() << "\n";
    dfile << "GoogleMapsWebServerRequestVar = " << GoogleMapsWebServerRequestVar.c_str() << "\n";
    dfile << "GoogleMapsWebServerTitleVar = " << GoogleMapsWebServerTitleVar.c_str() << "\n";
    dfile << "####################################" << "\n";
    dfile << "\n";
    dfile << "\n";
    dfile << "######################################" << "\n";
    dfile << "# Warning table Web page configuration" << "\n";
    dfile << "WarningTableCWA = " << WarningTableCWA.c_str() << "\n";
    dfile << "WarningTableWebPageName = " << WarningTableWebPageName.c_str() << "\n";
    dfile << "WarningTableTitle = " << WarningTableTitle.c_str() << "\n";
    dfile << "WarningTableIcons = " << WarningTableIcons.c_str() << "\n";
    dfile << "WarningTableIconWidth = " << WarningTableIconWidth.c_str() << "\n";
    dfile << "WarningTableIconHeight = " << WarningTableIconHeight.c_str() << "\n";
    dfile << "WarningTableData = " << WarningTableData.c_str() << "\n";
    dfile << "WarningTableHeaderLink = " << WarningTableHeaderLink.c_str() << "\n";
    dfile << "WarningTablePrintUpdateTime = " << WarningTablePrintUpdateTime.c_str() << "\n";
    dfile << "WarningTableTimezone = " << WarningTableTimezone.c_str() << "\n";
    dfile << "WarningTableArchiveMode = " << WarningTableArchiveMode.c_str() << "\n";
    dfile << "WarningTableWidth = " << WarningTableWidth.c_str() << "\n";
    dfile << "WarningTableMainTableAttributes = " << WarningTableMainTableAttributes.c_str() << "\n";
    dfile << "# Subtable 1 = Area title" << "\n";
    dfile << "WarningSubTable1Width = " << WarningSubTable1Width.c_str() << "\n";
    dfile << "# Subtable2 = Area name table" << "\n";
    dfile << "WarningSubTable2Width = " << WarningSubTable2Width.c_str() << "\n";
    dfile << "# Subtable 3 = product icons" << "\n";
    dfile << "WarningSubTable3Width = " << WarningSubTable3Width.c_str() << "\n";
    dfile << "WarningZoneNameTableDataWidth = " << WarningZoneNameTableDataWidth.c_str() << "\n";
    dfile << "WarningTableRows = " << WarningTableRows.c_str() << "\n";
    dfile << "WarningTableBorderTest = " << WarningTableBorderTest.c_str() << "\n";
    dfile << "WarningTableTitleBGColor = " << WarningTableTitleBGColor.c_str() << "\n";
    dfile << "WarningTableTitleFontSize = " << WarningTableTitleFontSize.c_str() << "\n";
    dfile << "WarningTableTitleFontColor = " << WarningTableTitleFontColor.c_str() << "\n";
    dfile << "WarningTableAreaTitleBGColor = " << WarningTableAreaTitleBGColor.c_str() << "\n";
    dfile << "WarningTableAreaTitleFontSize = " << WarningTableAreaTitleFontSize.c_str() << "\n";
    dfile << "WarningTableAreaTitleFontColor = " << WarningTableAreaTitleFontColor.c_str() << "\n";
    dfile << "WarningTableZoneNameBGColor = " << WarningTableZoneNameBGColor.c_str() << "\n";
    dfile << "WarningTableZoneNameFontSize = " << WarningTableZoneNameFontSize.c_str() << "\n";
    dfile << "WarningTableZoneNameFontColor = " << WarningTableZoneNameFontColor.c_str() << "\n";
    dfile << "WarningTableZoneNameMouseOver = " << WarningTableZoneNameMouseOver.c_str() << "\n";
    dfile << "WarningTableProductBGColor = " << WarningTableProductBGColor.c_str() << "\n";
    dfile << "WarningTableProductFontSize = " << WarningTableProductFontSize.c_str() << "\n";
    dfile << "WarningTableProductFontColor = " << WarningTableProductFontColor.c_str() << "\n";
    dfile << "WarningTableJStoolbar = " << WarningTableJStoolbar.c_str() << "\n";
    dfile << "WarningTableJSscrollbars = " << WarningTableJSscrollbars.c_str() << "\n";
    dfile << "WarningTableJSlocation = " << WarningTableJSlocation.c_str() << "\n";
    dfile << "WarningTableJSstatusbar = " << WarningTableJSstatusbar.c_str() << "\n";
    dfile << "WarningTableJSmenubar = " << WarningTableJSmenubar.c_str() << "\n";
    dfile << "WarningTableJSresizable = " << WarningTableJSresizable.c_str() << "\n";
    dfile << "WarningTableJSwidth = " << WarningTableJSwidth.c_str() << "\n";
    dfile << "WarningTableJSheight = " << WarningTableJSheight.c_str() << "\n";
    dfile << "WarningTableJSleft = " << WarningTableJSleft.c_str() << "\n";
    dfile << "WarningTableJStop = " << WarningTableJStop.c_str() << "\n";
    dfile << "\n";
    dfile << "# Optional setup for Web product download configuration"  << "\n";   
    // 05/13/2009: Switched primary download site to forecast.weather.gov
    dfile << "# Primary Website for product download"  << "\n";   
    dfile << "WarningTableProductWebsite = " << WarningTableProductWebsite.c_str()  << "\n";
    dfile << "WarningTableProductWebsiteScript = " << WarningTableProductWebsiteScript.c_str() << "\n";
    dfile << "WarningTableProductWebsiteQuery = " << WarningTableProductWebsiteQuery.c_str() << "\n";
    // 05/13/2009: Added SRH as a backup site
    dfile << "# Backup Website for product download"  << "\n";   
    dfile << "WarningTableProductEnableWebsiteBackup = " << WarningTableProductEnableWebsiteBackup.c_str() << "\n"; 
    dfile << "WarningTableProductWebsiteBackup = " << WarningTableProductWebsiteBackup.c_str()  << "\n";
    dfile << "WarningTableProductWebsiteScriptBackup = " << WarningTableProductWebsiteScriptBackup.c_str() << "\n";
    dfile << "WarningTableProductWebsiteQueryBackup = " << WarningTableProductWebsiteQueryBackup.c_str() << "\n";

    // 07/10/2008: Added help bullets
    dfile << "\n";
    dfile << "# Optional help bullets at the bottom of the warning table" << "\n";
    dfile << "WarningTableHelpBullet1 = " << WarningTableHelpBullet1.c_str() << "\n";
    dfile << "WarningTableHelpBullet2 = " << WarningTableHelpBullet2.c_str() << "\n";
    dfile << "WarningTableHelpBullet3 = " << WarningTableHelpBullet3.c_str() << "\n";
    dfile << "WarningTableHelpBullet4 = " << WarningTableHelpBullet4.c_str() << "\n";
    dfile << "WarningTableHelpBullet5 = " << WarningTableHelpBullet5.c_str() << "\n";

    // 06/23/2009: Added product pre and post processing options
    dfile << "\n";
    dfile << "# Product pre-processing options"  << "\n";   
    dfile << "WarningTableSortOrderByHighestTimeStamp = " << WarningTableSortOrderByHighestTimeStamp.c_str() << "\n"; 
    dfile << "WarningTableSkipDuplicateUGCHeaders = " << WarningTableSkipDuplicateUGCHeaders.c_str() << "\n";
    dfile << "WarningTableSkipExpiredUGCHeaders = " << WarningTableSkipExpiredUGCHeaders.c_str() << "\n";
    dfile << "\n";
    dfile << "# Product post-processing options"  << "\n";   
    dfile << "WarningTableMatchAllZones = " << WarningTableMatchAllZones.c_str() << "\n";

    dfile << "######################################" << "\n";
    dfile << "\n";
    dfile << "\n";
    dfile << "######################################" << "\n";
    dfile << "# Key table configuration" << "\n";
    dfile << "KeyTableLinkName = " << KeyTableLinkName.c_str() << "\n";
    dfile << "KeyTableLinkBorder = " << KeyTableLinkBorder.c_str() << "\n";
    dfile << "KeyTableWarningTitle = " << KeyTableWarningTitle.c_str() << "\n";
    dfile << "KeyTableWatchTitle = " << KeyTableWatchTitle.c_str() << "\n";
    dfile << "KeyTableAdvisoryTitle = " << KeyTableAdvisoryTitle.c_str() << "\n";
    dfile << "KeyTableWidth = " << KeyTableWidth.c_str() << "\n";
    dfile << "KeyTableIconWidth = " << KeyTableIconWidth.c_str() << "\n";
    dfile << "KeyTableIconHeight = " << KeyTableIconHeight.c_str() << "\n";
    dfile << "KeyTableBGColor = " << KeyTableBGColor.c_str() << "\n";
    dfile << "KeyTableTitleBGColor = " << KeyTableTitleBGColor.c_str() << "\n";
    dfile << "KeyTableTitleFontSize = " << KeyTableTitleFontSize.c_str() << "\n";
    dfile << "KeyTableTitleFontColor = " << KeyTableTitleFontColor.c_str() << "\n";
    dfile << "KeyTableIconNameFontSize = " << KeyTableIconNameFontSize.c_str() << "\n";
    dfile << "KeyTableIconNameFontColor = " << KeyTableIconNameFontColor.c_str() << "\n";
    dfile << "KeyTableTDWidth = " << KeyTableTDWidth.c_str() << "\n";
    dfile << "KeyTableJStoolbar = " << KeyTableJStoolbar.c_str() << "\n";
    dfile << "KeyTableJSscrollbars = " << KeyTableJSscrollbars.c_str() << "\n";
    dfile << "KeyTableJSlocation = " << KeyTableJSlocation.c_str() << "\n";
    dfile << "KeyTableJSstatusbar = " << KeyTableJSstatusbar.c_str() << "\n";
    dfile << "KeyTableJSmenubar = " << KeyTableJSmenubar.c_str() << "\n";
    dfile << "KeyTableJSresizable = " << KeyTableJSresizable.c_str() << "\n";
    dfile << "KeyTableJSwidth = " << KeyTableJSwidth.c_str() << "\n";
    dfile << "KeyTableJSheight = " << KeyTableJSheight.c_str() << "\n";
    dfile << "KeyTableJSleft = " << KeyTableJSleft.c_str() << "\n";
    dfile << "KeyTableJStop = " << KeyTableJStop.c_str() << "\n";
    dfile << "######################################" << "\n";
    dfile << "\n";
    dfile << "\n";
    dfile << "######################################" << "\n";
    dfile << "# Verification table configuration" << "\n";
    dfile << "VerTableLinkName = " << VerTableLinkName.c_str() << "\n";
    dfile << "VerTableLinkBorder = " << VerTableLinkBorder.c_str() << "\n";
    dfile << "VerTableTitle = " << VerTableTitle.c_str() << "\n";
    dfile << "VerTablePrintUpdateTime = " << VerTablePrintUpdateTime.c_str() << "\n";
    dfile << "VerTableTitleBGColor = " << VerTableTitleBGColor.c_str() << "\n";
    dfile << "VerTableTitleFontColor = " << VerTableTitleFontColor.c_str() << "\n";
    dfile << "VerTableTitleFontSize = " << VerTableTitleFontSize.c_str() << "\n";
    dfile << "VerTableHeaderLink = " << VerTableHeaderLink.c_str() << "\n";
    dfile << "VerTableWidth = " << VerTableWidth.c_str() << "\n";
    dfile << "VerTableBorder = " << VerTableBorder.c_str() << "\n";
    dfile << "VerTableZoneNameTDWidth = " << VerTableZoneNameTDWidth.c_str() << "\n";
    dfile << "VerTableZoneNameBGColor = " << VerTableZoneNameBGColor.c_str() << "\n";
    dfile << "VerTableZoneNameMouseOver = " << VerTableZoneNameMouseOver.c_str() << "\n";
    dfile << "VerTableZoneNameFontSize = " << VerTableZoneNameFontSize.c_str() << "\n";
    dfile << "VerTableZoneNameFontColor = " << VerTableZoneNameFontColor.c_str() << "\n";
    dfile << "VerTableProductTDWidth = " << VerTableProductTDWidth.c_str() << "\n";
    dfile << "VerTableProductFontColor = " << VerTableProductFontColor.c_str() << "\n";
    dfile << "VerTableProductFontSize = " << VerTableProductFontSize.c_str() << "\n";
    dfile << "VerTableProductBGColor = " << VerTableProductBGColor.c_str() << "\n";
    dfile << "VerTableJStoolbar = " << VerTableJStoolbar.c_str() << "\n";
    dfile << "VerTableJSscrollbars = " << VerTableJSscrollbars.c_str() << "\n";
    dfile << "VerTableJSlocation = " << VerTableJSlocation.c_str() << "\n";
    dfile << "VerTableJSstatusbar = " << VerTableJSstatusbar.c_str() << "\n";
    dfile << "VerTableJSmenubar = " << VerTableJSmenubar.c_str() << "\n";
    dfile << "VerTableJSresizable = " << VerTableJSresizable.c_str() << "\n";
    dfile << "VerTableJSwidth = " << VerTableJSwidth.c_str() << "\n";
    dfile << "VerTableJSheight = " << VerTableJSheight.c_str() << "\n";
    dfile << "VerTableJSleft = " << VerTableJSleft.c_str() << "\n";
    dfile << "VerTableJStop = " << VerTableJStop.c_str() << "\n";
    dfile << "######################################" << "\n";
    dfile << "\n";
    dfile << "\n";
    dfile << "######################################" << "\n";
    dfile << "# Short fused warning table configuration" << "\n";
    dfile << "ShortFusedTableLinkName = " << ShortFusedTableLinkName.c_str() << "\n";
    dfile << "ShortFusedTableLinkBorder = " << ShortFusedTableLinkBorder.c_str() << "\n";
    dfile << "ShortFusedTableTitle = " << ShortFusedTableTitle.c_str() << "\n";
    dfile << "ShortFusedTablePrintUpdateTime = " << ShortFusedTablePrintUpdateTime.c_str() << "\n";
    dfile << "ShortFusedTableTitleBGColor = " << ShortFusedTableTitleBGColor.c_str() << "\n";
    dfile << "ShortFusedTableTitleFontColor = " << ShortFusedTableTitleFontColor.c_str() << "\n";
    dfile << "ShortFusedTableTitleFontSize = " << ShortFusedTableTitleFontSize.c_str() << "\n";
    dfile << "ShortFusedTableHeaderLink = " << ShortFusedTableHeaderLink.c_str() << "\n";
    dfile << "ShortFusedTableWidth = " << ShortFusedTableWidth.c_str() << "\n";
    dfile << "ShortFusedTableBorder = " << ShortFusedTableBorder.c_str() << "\n";
    dfile << "ShortFusedTableZoneNameTDWidth = " << ShortFusedTableZoneNameTDWidth.c_str() << "\n";
    dfile << "ShortFusedTableZoneNameBGColor = " << ShortFusedTableZoneNameBGColor.c_str() << "\n";
    dfile << "ShortFusedTableZoneNameMouseOver = " << ShortFusedTableZoneNameMouseOver.c_str() << "\n";
    dfile << "ShortFusedTableZoneNameFontSize = " << ShortFusedTableZoneNameFontSize.c_str() << "\n";
    dfile << "ShortFusedTableZoneNameFontColor = " << ShortFusedTableZoneNameFontColor.c_str() << "\n";
    dfile << "ShortFusedTableProductTDWidth = " << ShortFusedTableProductTDWidth.c_str() << "\n";
    dfile << "ShortFusedTableProductFontColor = " << ShortFusedTableProductFontColor.c_str() << "\n";
    dfile << "ShortFusedTableProductFontSize = " << ShortFusedTableProductFontSize.c_str() << "\n";
    dfile << "ShortFusedTableProductBGColor = " << ShortFusedTableProductBGColor.c_str() << "\n";
    dfile << "ShortFusedTableLinkToWarningTable = " << ShortFusedTableLinkToWarningTable.c_str() << "\n";
    dfile << "ShortFusedTableLinkToWarningTableTitle = " << ShortFusedTableLinkToWarningTableTitle.c_str() << "\n";
    dfile << "######################################" << "\n";
    dfile << "\n";
    dfile << "######################################" << "\n";
    dfile << "# RSS configuration for warning table" << "\n";
    dfile << "# The parameters below are only use in RSS mode" << "\n";
    dfile << "CustomProductName = " << CustomProductName.c_str() << "\n";
    dfile << "CustomProductTitle = " << CustomProductTitle.c_str() << "\n";
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
    dfile << "######################################" << "\n";
    dfile << "\n";
    dfile << "\n";
    dfile << "# End of configuration file" << "\n";
    dfile << "\n";
    dfile.df_Close();
    return 1;
  }

  // Load the configuration 
  char *str = 0;
  int ival = 0;

  // Load string values from the configuration file
  str = CfgData.GetStrValue("spool_dir");
  if(!str) {
    if(debug) NT_print("spool_dir parameter missing from config file");
  }
  else {
    spool_dir = str;
  }
  str = CfgData.GetStrValue("service_name");
  if(!str) {
    if(debug) NT_print("service_name parameter missing from config file");
  }
  else {
    service_name = str;
  }
  str = CfgData.GetStrValue("area_table_file");
  if(!str) {
    if(debug) NT_print("area_table_file parameter missing from config file");
  }
  else {
    area_table_file = str;
  }
  str = CfgData.GetStrValue("zone_table_file");
  if(!str) {
    if(debug) NT_print("zone_table_file parameter missing from config file");
  }
  else {
    zone_table_file = str;
  }
  str = CfgData.GetStrValue("product_table_file");
  if(!str) {
    if(debug) NT_print("product_table_file parameter missing from config file");
  }
  else {
    product_table_file = str;
  }
  str = CfgData.GetStrValue("Timezone_codes");
  if(!str) {
    if(debug) NT_print("Timezone_codes parameter missing from config file");
  }
  else {
    Timezone_codes = str;
  }
  str = CfgData.GetStrValue("Followup_statement_pils");
  if(!str) {
    if(debug) NT_print("Followup_statement_pils parameter missing from config file");
  }
  else {
    Followup_statement_pils = str;
  }
  str = CfgData.GetStrValue("Followup_phenomena_code_to_pil");
  if(!str) {
    if(debug) NT_print("Followup_phenomena_code_to_pil parameter missing from config file");
  }
  else {
    Followup_phenomena_code_to_pil = str;
  }
  str = CfgData.GetStrValue("PVTEC_status_codes");
  if(!str) {
    if(debug) NT_print("PVTEC_status_codes parameter missing from config file");
  }
  else {
    PVTEC_status_codes = str;
  }
  str = CfgData.GetStrValue("PVTEC_action_codes");
  if(!str) {
    if(debug) NT_print("PVTEC_action_codes parameter missing from config file");
  }
  else {
    PVTEC_action_codes = str;
  }
  str = CfgData.GetStrValue("PVTEC_significance_codes");
  if(!str) {
    if(debug) NT_print("PVTEC_significance_codes parameter missing from config file");
  }
  else {
    PVTEC_significance_codes = str;
  }
  str = CfgData.GetStrValue("PVTEC_phenomena_codes");
  if(!str) {
    if(debug) NT_print("PVTEC_phenomena_codes parameter missing from config file");
  }
  else {
    PVTEC_phenomena_codes = str;
  }
  str = CfgData.GetStrValue("HVTEC_flood_severity_codes");
  if(!str) {
    if(debug) NT_print("HVTEC_flood_severity_codes parameter missing from config file");
  }
  else {
    HVTEC_flood_severity_codes = str;
  }
  str = CfgData.GetStrValue("HVTEC_immediate_cause_codes");
  if(!str) {
    if(debug) NT_print("HVTEC_immediate_cause_codes parameter missing from config file");
  }
  else {
    HVTEC_immediate_cause_codes = str;
  }
  str = CfgData.GetStrValue("HVTEC_flood_record_codes");
  if(!str) {
    if(debug) NT_print("HVTEC_flood_record_codes parameter missing from config file");
  }
  else {
    HVTEC_flood_record_codes = str;
  }
  str = CfgData.GetStrValue("GoogleMapsURL");
  if(!str) {
    if(debug) NT_print("GoogleMapsURL parameter missing from config file");
  }
  else {
    GoogleMapsURL = str;
  }
  str = CfgData.GetStrValue("GoogleMapsKey");
  if(!str) {
    if(debug) NT_print("GoogleMapsKey parameter missing from config file");
  }
  else {
    GoogleMapsKey = str;
  }
  str = CfgData.GetStrValue("GoogleMapsClientName");
  if(!str) {
    if(debug) NT_print("GoogleMapsClientName parameter missing from config file");
  }
  else {
    GoogleMapsClientName = str;
  }
  str = CfgData.GetStrValue("GoogleMapsWMSTransparentLayersScript");
  if(!str) {
    if(debug) NT_print("GoogleMapsWMSTransparentLayersScript parameter missing from config file");
  }
  else {
    GoogleMapsWMSTransparentLayersScript = str;
  }
  str = CfgData.GetStrValue("GoogleMapsWMSGAL");
  if(!str) {
    if(debug) NT_print("GoogleMapsWMSGAL parameter missing from config file");
  }
  else {
    GoogleMapsWMSGAL = str;
  }
  str = CfgData.GetStrValue("GoogleMapsWMSSpec");
  if(!str) {
    if(debug) NT_print("GoogleMapsWMSSpec parameter missing from config file");
  }
  else {
    GoogleMapsWMSSpec = str;
  }
  str = CfgData.GetStrValue("GoogleMapsWMSOverlaySpec");
  if(!str) {
    if(debug) NT_print("GoogleMapsWMSOverlaySpec parameter missing from config file");
  }
  else {
    GoogleMapsWMSOverlaySpec = str;
  }
  str = CfgData.GetStrValue("GoogleMapsGIcon");
  if(!str) {
    if(debug) NT_print("GoogleMapsGIcon parameter missing from config file");
  }
  else {
    GoogleMapsGIcon = str;
  }
  str = CfgData.GetStrValue("WebpageOutputDirectory");
  if(!str) {
    if(debug) NT_print("WebpageOutputDirectory parameter missing from config file");
  }
  else {
    WebpageOutputDirectory = str;
  }
  str = CfgData.GetStrValue("GoogleMapsWebPageName");
  if(!str) {
    if(debug) NT_print("GoogleMapsWebPageName parameter missing from config file");
  }
  else {
    GoogleMapsWebPageName = str;
  }
  str = CfgData.GetStrValue("GoogleMapsDefaultTextColor");
  if(!str) {
    if(debug) NT_print("GoogleMapsDefaultTextColor parameter missing from config file");
  }
  else {
    GoogleMapsDefaultTextColor = str;
  }
  str = CfgData.GetStrValue("GoogleMapsTextHighlightColor");
  if(!str) {
    if(debug) NT_print("GoogleMapsTextHighlightColor parameter missing from config file");
  }
  else {
    GoogleMapsTextHighlightColor = str;
  }
  str = CfgData.GetStrValue("GoogleMapsTextExpiredHighlightColor");
  if(!str) {
    if(debug) NT_print("GoogleMapsTextExpiredHighlightColor parameter missing from config file");
  }
  else {
    GoogleMapsTextExpiredHighlightColor = str;
  }
  str = CfgData.GetStrValue("GoogleMapsDefaultZoom");
  if(!str) {
    if(debug) NT_print("GoogleMapsDefaultZoom parameter missing from config file");
  }
  else {
    GoogleMapsDefaultZoom = str;
  }
  str = CfgData.GetStrValue("GoogleMapsTextColorByProduct");
  if(!str) {
    if(debug) NT_print("GoogleMapsTextColorByProduct parameter missing from config file");
  }
  else {
    GoogleMapsTextColorByProduct = str;
  }
  str = CfgData.GetStrValue("GoogleMapsZoomByProduct");
  if(!str) {
    if(debug) NT_print("GoogleMapsZoomByProduct parameter missing from config file");
  }
  else {
    GoogleMapsZoomByProduct = str;
  }
  
  
  // Load int values from the configuration file
  str = CfgData.GetStrValue("download_retries");
  if(!str) {
    if(debug) NT_print("download_retries parameter missing from config file");
  }
  else {
    ival = atoi(str);
    if(ival <= 0) {
      NT_print("Invalid download_retries setting in config file");
      NT_print("Using default value");
    }
    else {
      download_retries = ival;
    }
  }
  str = CfgData.GetStrValue("download_retry_pause");
  if(!str) {
    if(debug) NT_print("download_retry_pause parameter missing from config file");
  }
  else {
    ival = atoi(str);
    if(ival <= 0) {
      NT_print("Invalid download_retry_pause setting in config file");
      NT_print("Using default value");
    }
    else {
      download_retry_pause = ival;
    }
  }
  
  // 05/13/2009: Changed retries and retry_pause to global config values and are now optional
  // 05/13/2009: The download variables have been changed to download_retries and download_retry_pause 
  str = CfgData.GetStrValue("retries");
  if(str) {
    ival = atoi(str);
    if(ival <= 0) {
      NT_print("Invalid retries setting in config file");
      NT_print("Using default value");
    }
    else {
      // TODO: retires is now global. Add all affected varibles here
      download_retries = ival;
    }
  }
  str = CfgData.GetStrValue("retry_pause");
  if(str) {
    ival = atoi(str);
    if(ival <= 0) {
      NT_print("Invalid retry_pause setting in config file");
      NT_print("Using default value");
    }
    else {
      // TODO: retry_pause is now global. Add all affected varibles here
      download_retry_pause = ival;
    }
  }

  str = CfgData.GetStrValue("GoogleMapsWidth");
  if(!str) {
    if(debug) NT_print("GoogleMapsWidth parameter missing from config file");
  }
  else {
    ival = atoi(str);
    if(ival <= 0) {
      NT_print("Invalid GoogleMapsWidth setting in config file");
      NT_print("Using default value");
    }
    else {
      GoogleMapsWidth = ival;
    }
  }
  str = CfgData.GetStrValue("GoogleMapsHeight");
  if(!str) {
    if(debug) NT_print("GoogleMapsHeight parameter missing from config file");
  }
  else {
    ival = atoi(str);
    if(ival <= 0) {
      NT_print("Invalid GoogleMapsHeight setting in config file");
      NT_print("Using default value");
    }
    else {
      GoogleMapsHeight = ival;
    }
  }

  // Load true false values from the configuration file
  str = CfgData.GetStrValue("GooleMapsPrintExpiredMessage");
  if(!str) {
    if(debug) NT_print("GooleMapsPrintExpiredMessage parameter missing from config file");
  }
  else {
    sbuf = str;
    sbuf.ToLower();
    if((sbuf == "1") || (sbuf == "true")) {
      GooleMapsPrintExpiredMessage = 1;
    }
    else if((sbuf == "0") || (sbuf == "false")) {
      GooleMapsPrintExpiredMessage = 0;
    }
    else {
      NT_print("Invalid GooleMapsPrintExpiredMessage setting in config file");
      NT_print("Using default value");
    }
  }
  str = CfgData.GetStrValue("GoogleMapsTextOverlay");
  if(!str) {
    if(debug) NT_print("GoogleMapsTextOverlay parameter missing from config file");
  }
  else {
    sbuf = str;
    sbuf.ToLower();
    if((sbuf == "1") || (sbuf == "true")) {
      GoogleMapsTextOverlay = 1;
    }
    else if((sbuf == "0") || (sbuf == "false")) {
      GoogleMapsTextOverlay = 0;
    }
    else {
      NT_print("Invalid GoogleMapsTextOverlay setting in config file");
      NT_print("Using default value");
    }
  }

  str = CfgData.GetStrValue("GoogleMapsWebServerRequestScript");
  if(!str) {
    if(debug) NT_print("GoogleMapsWebServerRequestScript parameter missing from config file");
  }
  else {
    GoogleMapsWebServerRequestScript = str;
    GoogleMapsWebServerRequestScript.TrimLeadingSpaces();
    GoogleMapsWebServerRequestScript.TrimTrailingSpaces();
  }

  str = CfgData.GetStrValue("GoogleMapsWebServerRequestVar");
  if(!str) {
    if(debug) NT_print("GoogleMapsWebServerRequestVar parameter missing from config file");
  }
  else {
    GoogleMapsWebServerRequestVar = str;
    GoogleMapsWebServerRequestVar.TrimLeadingSpaces();
    GoogleMapsWebServerRequestVar.TrimTrailingSpaces();
  }

  str = CfgData.GetStrValue("GoogleMapsWebServerTitleVar");
  if(!str) {
    if(debug) NT_print("GoogleMapsWebServerTitleVar parameter missing from config file");
  }
  else {
    GoogleMapsWebServerTitleVar = str;
    GoogleMapsWebServerTitleVar.TrimLeadingSpaces();
    GoogleMapsWebServerTitleVar.TrimTrailingSpaces();
  }

  // Warning table configuration
  str = CfgData.GetStrValue("WarningTableCWA");
  if(!str) {
    if(debug) NT_print("WarningTableCWA parameter missing from config file");
  }
  else {
    WarningTableCWA = str;
  }
  str = CfgData.GetStrValue("WarningTableWebPageName");
  if(!str) {
    if(debug) NT_print("WarningTableWebPageName parameter missing from config file");
  }
  else {
    WarningTableWebPageName = str;
  }
  str = CfgData.GetStrValue("WarningTableTitle");
  if(!str) {
    if(debug) NT_print("WarningTableTitle parameter missing from config file");
  }
  else {
    WarningTableTitle = str;
  }
  str = CfgData.GetStrValue("WarningTableIcons");
  if(!str) {
    if(debug) NT_print("WarningTableIcons parameter missing from config file");
  }
  else {
    WarningTableIcons = str;
  }
  str = CfgData.GetStrValue("WarningTableIconWidth");
  if(!str) {
    if(debug) NT_print("WarningTableIconWidth parameter missing from config file");
  }
  else {
    WarningTableIconWidth = str;
  }
  str = CfgData.GetStrValue("WarningTableIconHeight");
  if(!str) {
    if(debug) NT_print("WarningTableIconHeight parameter missing from config file");
  }
  else {
    WarningTableIconHeight = str;
  }
  str = CfgData.GetStrValue("WarningTableData");
  if(!str) {
    if(debug) NT_print("WarningTableData parameter missing from config file");
  }
  else {
    WarningTableData = str;
  }
  str = CfgData.GetStrValue("WarningTableHeaderLink");
  if(!str) {
    if(debug) NT_print("WarningTableHeaderLink parameter missing from config file");
  }
  else {
    WarningTableHeaderLink = str;
  }
  str = CfgData.GetStrValue("WarningTablePrintUpdateTime");
  if(!str) {
    if(debug) NT_print("WarningTablePrintUpdateTime parameter missing from config file");
  }
  else {
    WarningTablePrintUpdateTime = str;
  }
  str = CfgData.GetStrValue("WarningTableTimezone");
  if(!str) {
    if(debug) NT_print("WarningTableTimezone parameter missing from config file");
  }
  else {
    WarningTableTimezone = str;
  }
  str = CfgData.GetStrValue("WarningTableArchiveMode");
  if(!str) {
    if(debug) NT_print("WarningTableArchiveMode parameter missing from config file");
  }
  else {
    WarningTableArchiveMode = str;
  }
  str = CfgData.GetStrValue("WarningTableWidth");
  if(!str) {
    if(debug) NT_print("WarningTableWidth parameter missing from config file");
  }
  else {
    WarningTableWidth = str;
  }
  str = CfgData.GetStrValue("WarningTableMainTableAttributes");
  if(!str) {
    if(debug) NT_print("WarningTableMainTableAttributes parameter missing from config file");
  }
  else {
    WarningTableMainTableAttributes = str;
  }
  str = CfgData.GetStrValue("WarningSubTable1Width");
  if(!str) {
    if(debug) NT_print("WarningSubTable1Width parameter missing from config file");
  }
  else {
    WarningSubTable1Width = str;
  }
  str = CfgData.GetStrValue("WarningSubTable2Width");
  if(!str) {
    if(debug) NT_print("WarningSubTable2Width parameter missing from config file");
  }
  else {
    WarningSubTable2Width = str;
  }
  str = CfgData.GetStrValue("WarningSubTable3Width");
  if(!str) {
    if(debug) NT_print("WarningSubTable3Width parameter missing from config file");
  }
  else {
    WarningSubTable3Width = str;
  }
  str = CfgData.GetStrValue("WarningZoneNameTableDataWidth");
  if(!str) {
    if(debug) NT_print("WarningZoneNameTableDataWidth parameter missing from config file");
  }
  else {
    WarningZoneNameTableDataWidth = str;
  }
  str = CfgData.GetStrValue("WarningTableRows");
  if(!str) {
    if(debug) NT_print("WarningTableRows parameter missing from config file");
  }
  else {
    WarningTableRows = str;
  }
  str = CfgData.GetStrValue("WarningTableBorderTest");
  if(!str) {
    if(debug) NT_print("WarningTableBorderTest parameter missing from config file");
  }
  else {
    WarningTableBorderTest = str;
  }
  str = CfgData.GetStrValue("WarningTableTitleBGColor");
  if(!str) {
    if(debug) NT_print("WarningTableTitleBGColor parameter missing from config file");
  }
  else {
    WarningTableTitleBGColor = str;
  }
  str = CfgData.GetStrValue("WarningTableTitleFontSize");
  if(!str) {
    if(debug) NT_print("WarningTableTitleFontSize parameter missing from config file");
  }
  else {
    WarningTableTitleFontSize = str;
  }
  str = CfgData.GetStrValue("WarningTableTitleFontColor");
  if(!str) {
    if(debug) NT_print("WarningTableTitleFontColor parameter missing from config file");
  }
  else {
    WarningTableTitleFontColor = str;
  }
  str = CfgData.GetStrValue("WarningTableAreaTitleBGColor");
  if(!str) {
    if(debug) NT_print("WarningTableAreaTitleBGColor parameter missing from config file");
  }
  else {
    WarningTableAreaTitleBGColor = str;
  }
  str = CfgData.GetStrValue("WarningTableAreaTitleFontSize");
  if(!str) {
    if(debug) NT_print("WarningTableAreaTitleFontSize parameter missing from config file");
  }
  else {
    WarningTableAreaTitleFontSize = str;
  }
  str = CfgData.GetStrValue("WarningTableAreaTitleFontColor");
  if(!str) {
    if(debug) NT_print("WarningTableAreaTitleFontColor parameter missing from config file");
  }
  else {
    WarningTableAreaTitleFontColor = str;
  }
  str = CfgData.GetStrValue("WarningTableZoneNameBGColor");
  if(!str) {
    if(debug) NT_print("WarningTableZoneNameBGColor parameter missing from config file");
  }
  else {
    WarningTableZoneNameBGColor = str;
  }
  str = CfgData.GetStrValue("WarningTableZoneNameFontSize");
  if(!str) {
    if(debug) NT_print("WarningTableZoneNameFontSize parameter missing from config file");
  }
  else {
    WarningTableZoneNameFontSize = str;
  }
  str = CfgData.GetStrValue("WarningTableZoneNameFontColor");
  if(!str) {
    if(debug) NT_print("WarningTableZoneNameFontColor parameter missing from config file");
  }
  else {
    WarningTableZoneNameFontColor = str;
  }
  str = CfgData.GetStrValue("WarningTableZoneNameMouseOver");
  if(!str) {
    if(debug) NT_print("WarningTableZoneNameMouseOver parameter missing from config file");
  }
  else {
    WarningTableZoneNameMouseOver = str;
  }
  str = CfgData.GetStrValue("WarningTableProductBGColor");
  if(!str) {
    if(debug) NT_print("WarningTableProductBGColor parameter missing from config file");
  }
  else {
    WarningTableProductBGColor = str;
  }
  str = CfgData.GetStrValue("WarningTableProductFontSize");
  if(!str) {
    if(debug) NT_print("WarningTableProductFontSize parameter missing from config file");
  }
  else {
    WarningTableProductFontSize = str;
  }
  str = CfgData.GetStrValue("WarningTableProductFontColor");
  if(!str) {
    if(debug) NT_print("WarningTableProductFontColor parameter missing from config file");
  }
  else {
    WarningTableProductFontColor = str;
  }
  str = CfgData.GetStrValue("WarningTableJStoolbar");
  if(!str) {
    if(debug) NT_print("WarningTableJStoolbar parameter missing from config file");
  }
  else {
    WarningTableJStoolbar = str;
  }
  str = CfgData.GetStrValue("WarningTableJSscrollbars");
  if(!str) {
    if(debug) NT_print("WarningTableJSscrollbars parameter missing from config file");
  }
  else {
    WarningTableJSscrollbars = str;
  }
  str = CfgData.GetStrValue("WarningTableJSlocation");
  if(!str) {
    if(debug) NT_print("WarningTableJSlocation parameter missing from config file");
  }
  else {
    WarningTableJSlocation = str;
  }
  str = CfgData.GetStrValue("WarningTableJSstatusbar");
  if(!str) {
    if(debug) NT_print("WarningTableJSstatusbar parameter missing from config file");
  }
  else {
    WarningTableJSstatusbar = str;
  }
  str = CfgData.GetStrValue("WarningTableJSmenubar");
  if(!str) {
    if(debug) NT_print("WarningTableJSmenubar parameter missing from config file");
  }
  else {
    WarningTableJSmenubar = str;
  }
  str = CfgData.GetStrValue("WarningTableJSresizable");
  if(!str) {
    if(debug) NT_print("WarningTableJSresizable parameter missing from config file");
  }
  else {
    WarningTableJSresizable = str;
  }
  str = CfgData.GetStrValue("WarningTableJSwidth");
  if(!str) {
    if(debug) NT_print("WarningTableJSwidth parameter missing from config file");
  }
  else {
    WarningTableJSwidth = str;
  }
  str = CfgData.GetStrValue("WarningTableJSheight");
  if(!str) {
    if(debug) NT_print("WarningTableJSheight parameter missing from config file");
  }
  else {
    WarningTableJSheight = str;
  }
  str = CfgData.GetStrValue("WarningTableJSleft");
  if(!str) {
    if(debug) NT_print("WarningTableJSleft parameter missing from config file");
  }
  else {
    WarningTableJSleft = str;
  }
  str = CfgData.GetStrValue("WarningTableJStop");
  if(!str) {
    if(debug) NT_print("WarningTableJStop parameter missing from config file");
  }
  else {
    WarningTableJStop = str;
  }

  // NOTE: Check the backup site first so the old config files will now default to FORECAST.WEATHER.GOV
  str = CfgData.GetStrValue("WarningTableProductEnableWebsiteBackup");
  if(!str) {
    if(debug) NT_print("WarningTableProductEnableWebsiteBackup parameter missing from config file");
  }
  else {
   WarningTableProductEnableWebsiteBackup = str;
   WarningTableProductEnableWebsiteBackup.ToUpper();
  }
  // NOTE: Check the backup site first so the old config files will now default to FORECAST.WEATHER.GOV
  str = CfgData.GetStrValue("WarningTableProductWebsiteBackup");
  if(!str) {
    if(debug) NT_print("WarningTableProductWebsiteBackup parameter missing from config file");
    WarningTableProductWebsiteBackup = WarningTableProductWebsite; 
  }
  else {
   WarningTableProductWebsiteBackup = str;
  }
  // NOTE: Check the backup site first so the old config files will now default to FORECAST.WEATHER.GOV
  str = CfgData.GetStrValue("WarningTableProductWebsiteScriptBackup");
  if(!str) {
    if(debug) NT_print("WarningTableProductWebsiteScriptBackup parameter missing from config file");
    WarningTableProductWebsiteScriptBackup = WarningTableProductWebsiteScript; 
  }
  else {
    WarningTableProductWebsiteScriptBackup = str;
  }
  // NOTE: Check the backup site first so the old config files will now default to FORECAST.WEATHER.GOV
  str = CfgData.GetStrValue("WarningTableProductWebsiteQueryBackup");
  if(!str) {
    if(debug) NT_print("WarningTableProductWebsiteQueryBackup parameter missing from config file");
    WarningTableProductWebsiteQueryBackup = WarningTableProductWebsiteQuery; 
  }
  else {
   WarningTableProductWebsiteQueryBackup = str;
  }

  str = CfgData.GetStrValue("WarningTableProductWebsite");
  if(!str) {
    if(debug) NT_print("WarningTableProductWebsite parameter missing from config file");
  }
  else {
   WarningTableProductWebsite = str;
  }
  str = CfgData.GetStrValue("WarningTableProductWebsiteScript");
  if(!str) {
    if(debug) NT_print("WarningTableProductWebsiteScript parameter missing from config file");
  }
  else {
    WarningTableProductWebsiteScript = str;
  }
  str = CfgData.GetStrValue("WarningTableProductWebsiteQuery");
  if(!str) {
    if(debug) NT_print("WarningTableProductWebsiteQuery parameter missing from config file");
  }
  else {
   WarningTableProductWebsiteQuery = str;
  }

  // Load file system setup from the configuration file
  str = CfgData.GetStrValue("max_file_open_tries");
  if(!str) {
    if(debug) NT_print("max_file_open_tries parameter missing from config file");
  }
  else {
    ival = atoi(str);
    if(ival <= 0) {
      NT_print("Invalid max_file_open_tries setting in config file");
      NT_print("Using default value");
    }
    else {
      max_file_open_tries = ival;
    }
  }

  str = CfgData.GetStrValue("file_open_wait_time");
  if(!str) {
    if(debug) NT_print("file_open_wait_time parameter missing from config file");
  }
  else {
    ival = atoi(str);
    if(ival <= 0) {
      NT_print("Invalid file_open_wait_time setting in config file");
      NT_print("Using default value");
    }
    else {
      file_open_wait_time = ival;
    }
  }

  str = CfgData.GetStrValue("KeyTableLinkName");
  if(!str) {
    if(debug) NT_print("KeyTableLinkName parameter missing from config file");
  }
  else {
   KeyTableLinkName = str;
  }

  str = CfgData.GetStrValue("KeyTableLinkBorder");
  if(!str) {
    if(debug) NT_print("KeyTableLinkBorder parameter missing from config file");
  }
  else {
    KeyTableLinkBorder = str;
  }
  str = CfgData.GetStrValue("KeyTableWarningTitle");
  if(!str) {
    if(debug) NT_print("KeyTableWarningTitle parameter missing from config file");
  }
  else {
    KeyTableWarningTitle = str;
  }
  str = CfgData.GetStrValue("KeyTableWatchTitle");
  if(!str) {
    if(debug) NT_print("KeyTableWatchTitle parameter missing from config file");
  }
  else {
    KeyTableWatchTitle = str;
  }
  str = CfgData.GetStrValue("KeyTableAdvisoryTitle");
  if(!str) {
    if(debug) NT_print("KeyTableAdvisoryTitle parameter missing from config file");
  }
  else {
    KeyTableAdvisoryTitle = str;
  }
  str = CfgData.GetStrValue("KeyTableWidth");
  if(!str) {
    if(debug) NT_print("KeyTableWidth parameter missing from config file");
  }
  else {
    KeyTableWidth = str;
  }
  str = CfgData.GetStrValue("KeyTableIconWidth");
  if(!str) {
    if(debug) NT_print("KeyTableIconWidth parameter missing from config file");
  }
  else {
    KeyTableIconWidth = str;
  }
  str = CfgData.GetStrValue("KeyTableIconHeight");
  if(!str) {
    if(debug) NT_print("KeyTableIconHeight parameter missing from config file");
  }
  else {
    KeyTableIconHeight = str;
  }
  str = CfgData.GetStrValue("KeyTableBGColor");
  if(!str) {
    if(debug) NT_print("KeyTableBGColor parameter missing from config file");
  }
  else {
    KeyTableBGColor = str;
  }
  str = CfgData.GetStrValue("KeyTableTitleBGColor");
  if(!str) {
    if(debug) NT_print("KeyTableTitleBGColor parameter missing from config file");
  }
  else {
    KeyTableTitleBGColor = str;
  }
  str = CfgData.GetStrValue("KeyTableTitleFontSize");
  if(!str) {
    if(debug) NT_print("KeyTableTitleFontSize parameter missing from config file");
  }
  else {
    KeyTableTitleFontSize = str;
  }
  str = CfgData.GetStrValue("KeyTableTitleFontColor");
  if(!str) {
    if(debug) NT_print("KeyTableTitleFontColor parameter missing from config file");
  }
  else {
    KeyTableTitleFontColor = str;
  }
  str = CfgData.GetStrValue("KeyTableIconNameFontSize");
  if(!str) {
    if(debug) NT_print("KeyTableIconNameFontSize parameter missing from config file");
  }
  else {
    KeyTableIconNameFontSize = str;
  }
  str = CfgData.GetStrValue("KeyTableIconNameFontColor");
  if(!str) {
    if(debug) NT_print("KeyTableIconNameFontColor parameter missing from config file");
  }
  else {
    KeyTableIconNameFontColor = str;
  }
  str = CfgData.GetStrValue("KeyTableTDWidth");
  if(!str) {
    if(debug) NT_print("KeyTableTDWidth parameter missing from config file");
  }
  else {
    KeyTableTDWidth = str;
  }
  str = CfgData.GetStrValue("KeyTableJStoolbar");
  if(!str) {
    if(debug) NT_print("KeyTableJStoolbar parameter missing from config file");
  }
  else {
    KeyTableJStoolbar = str;
  }
  str = CfgData.GetStrValue("KeyTableJSscrollbars");
  if(!str) {
    if(debug) NT_print("KeyTableJSscrollbars parameter missing from config file");
  }
  else {
    KeyTableJSscrollbars = str;
  }
  str = CfgData.GetStrValue("KeyTableJSlocation");
  if(!str) {
    if(debug) NT_print("KeyTableJSlocation parameter missing from config file");
  }
  else {
    KeyTableJSlocation = str;
  }
  str = CfgData.GetStrValue("KeyTableJSstatusbar");
  if(!str) {
    if(debug) NT_print("KeyTableJSstatusbar parameter missing from config file");
  }
  else {
    KeyTableJSstatusbar = str;
  }
  str = CfgData.GetStrValue("KeyTableJSmenubar");
  if(!str) {
    if(debug) NT_print("KeyTableJSmenubar parameter missing from config file");
  }
  else {
    KeyTableJSmenubar = str;
  }
  str = CfgData.GetStrValue("KeyTableJSresizable");
  if(!str) {
    if(debug) NT_print("KeyTableJSresizable parameter missing from config file");
  }
  else {
    KeyTableJSresizable = str;
  }
  str = CfgData.GetStrValue("KeyTableJSwidth");
  if(!str) {
    if(debug) NT_print("KeyTableJSwidth parameter missing from config file");
  }
  else {
    KeyTableJSwidth = str;
  }
  str = CfgData.GetStrValue("KeyTableJSheight");
  if(!str) {
    if(debug) NT_print("KeyTableJSheight parameter missing from config file");
  }
  else {
    KeyTableJSheight = str;
  }
  str = CfgData.GetStrValue("KeyTableJSleft");
  if(!str) {
    if(debug) NT_print("KeyTableJSleft parameter missing from config file");
  }
  else {
    KeyTableJSleft = str;
  }
  str = CfgData.GetStrValue("KeyTableJStop");
  if(!str) {
    if(debug) NT_print("KeyTableJStop parameter missing from config file");
  }
  else {
    KeyTableJStop = str;
  }
  str = CfgData.GetStrValue("VerTableLinkName");
  if(!str) {
    if(debug) NT_print("VerTableLinkName parameter missing from config file");
  }
  else {
    VerTableLinkName = str;
  }
  str = CfgData.GetStrValue("VerTableLinkBorder");
  if(!str) {
    if(debug) NT_print("VerTableLinkBorder parameter missing from config file");
  }
  else {
    VerTableLinkBorder = str;
  }
  str = CfgData.GetStrValue("VerTableTitle");
  if(!str) {
    if(debug) NT_print("VerTableTitle parameter missing from config file");
  }
  else {
    VerTableTitle = str;
  }
  str = CfgData.GetStrValue("VerTablePrintUpdateTime");
  if(!str) {
    if(debug) NT_print("VerTablePrintUpdateTime parameter missing from config file");
  }
  else {
    VerTablePrintUpdateTime = str;
  }
  str = CfgData.GetStrValue("VerTableTitleBGColor");
  if(!str) {
    if(debug) NT_print("VerTableTitleBGColor parameter missing from config file");
  }
  else {
    VerTableTitleBGColor = str;
  }
  str = CfgData.GetStrValue("VerTableTitleFontColor");
  if(!str) {
    if(debug) NT_print("VerTableTitleFontColor parameter missing from config file");
  }
  else {
    VerTableTitleFontColor = str;
  }
  str = CfgData.GetStrValue("VerTableTitleFontSize");
  if(!str) {
    if(debug) NT_print("VerTableTitleFontSize parameter missing from config file");
  }
  else {
    VerTableTitleFontSize = str;
  }
  str = CfgData.GetStrValue("VerTableHeaderLink");
  if(!str) {
    if(debug) NT_print("VerTableHeaderLink parameter missing from config file");
  }
  else {
    VerTableHeaderLink = str;
  }
  str = CfgData.GetStrValue("VerTableWidth");
  if(!str) {
    if(debug) NT_print("VerTableWidth parameter missing from config file");
  }
  else {
    VerTableWidth = str;
  }
  str = CfgData.GetStrValue("VerTableBorder");
  if(!str) {
    if(debug) NT_print("VerTableBorder parameter missing from config file");
  }
  else {
    VerTableBorder = str;
  }
  str = CfgData.GetStrValue("VerTableZoneNameTDWidth");
  if(!str) {
    if(debug) NT_print("VerTableZoneNameTDWidth parameter missing from config file");
  }
  else {
    VerTableZoneNameTDWidth = str;
  }
  str = CfgData.GetStrValue("VerTableZoneNameBGColor");
  if(!str) {
    if(debug) NT_print("VerTableZoneNameBGColor parameter missing from config file");
  }
  else {
    VerTableZoneNameBGColor = str;
  }
  str = CfgData.GetStrValue("VerTableZoneNameMouseOver");
  if(!str) {
    if(debug) NT_print("VerTableZoneNameMouseOver parameter missing from config file");
  }
  else {
    VerTableZoneNameMouseOver = str;
  }
  str = CfgData.GetStrValue("VerTableZoneNameFontSize");
  if(!str) {
    if(debug) NT_print("VerTableZoneNameFontSize parameter missing from config file");
  }
  else {
    VerTableZoneNameFontSize = str;
  }
  str = CfgData.GetStrValue("VerTableZoneNameFontColor");
  if(!str) {
    if(debug) NT_print("VerTableZoneNameFontColor parameter missing from config file");
  }
  else {
    VerTableZoneNameFontColor = str;
  }
  str = CfgData.GetStrValue("VerTableProductTDWidth");
  if(!str) {
    if(debug) NT_print("VerTableProductTDWidth parameter missing from config file");
  }
  else {
    VerTableProductTDWidth = str;
  }
  str = CfgData.GetStrValue("VerTableProductFontColor");
  if(!str) {
    if(debug) NT_print("VerTableProductFontColor parameter missing from config file");
  }
  else {
    VerTableProductFontColor = str;
  }
  str = CfgData.GetStrValue("VerTableProductFontSize");
  if(!str) {
    if(debug) NT_print("VerTableProductFontSize parameter missing from config file");
  }
  else {
    VerTableProductFontSize = str;
  }
  str = CfgData.GetStrValue("VerTableProductBGColor");
  if(!str) {
    if(debug) NT_print("VerTableProductBGColor parameter missing from config file");
  }
  else {
    VerTableProductBGColor = str;
  }
  str = CfgData.GetStrValue("VerTableJStoolbar");
  if(!str) {
    if(debug) NT_print("VerTableJStoolbar parameter missing from config file");
  }
  else {
    VerTableJStoolbar = str;
  }
  str = CfgData.GetStrValue("VerTableJSscrollbars");
  if(!str) {
    if(debug) NT_print("VerTableJSscrollbars parameter missing from config file");
  }
  else {
    VerTableJSscrollbars = str;
  }
  str = CfgData.GetStrValue("VerTableJSlocation");
  if(!str) {
    if(debug) NT_print("VerTableJSlocation parameter missing from config file");
  }
  else {
    VerTableJSlocation = str;
  }
  str = CfgData.GetStrValue("VerTableJSstatusbar");
  if(!str) {
    if(debug) NT_print("VerTableJSstatusbar parameter missing from config file");
  }
  else {
    VerTableJSstatusbar = str;
  }
  str = CfgData.GetStrValue("VerTableJSmenubar");
  if(!str) {
    if(debug) NT_print("VerTableJSmenubar parameter missing from config file");
  }
  else {
    VerTableJSmenubar = str;
  }
  str = CfgData.GetStrValue("VerTableJSresizable");
  if(!str) {
    if(debug) NT_print("VerTableJSresizable parameter missing from config file");
  }
  else {
    VerTableJSresizable = str;
  }
  str = CfgData.GetStrValue("VerTableJSwidth");
  if(!str) {
    if(debug) NT_print("VerTableJSwidth parameter missing from config file");
  }
  else {
    VerTableJSwidth = str;
  }
  str = CfgData.GetStrValue("VerTableJSheight");
  if(!str) {
    if(debug) NT_print("VerTableJSheight parameter missing from config file");
  }
  else {
    VerTableJSheight = str;
  }
  str = CfgData.GetStrValue("VerTableJSleft");
  if(!str) {
    if(debug) NT_print("VerTableJSleft parameter missing from config file");
  }
  else {
    VerTableJSleft = str;
  }
  str = CfgData.GetStrValue("VerTableJStop");
  if(!str) {
    if(debug) NT_print("VerTableJStop parameter missing from config file");
  }
  else {
    VerTableJStop = str;
  }

  // 05/23/2008: Added for Google map pages
  str = CfgData.GetStrValue("GoogleMapsPHPHeaderIncludeCMS");
  if(!str) {
    if(debug) NT_print("GoogleMapsPHPHeaderIncludeCMS parameter missing from config file");
  }
  else {
    GoogleMapsPHPHeaderIncludeCMS = str;
  }
  str = CfgData.GetStrValue("GoogleMapsPHPHeaderIncludeSite");
  if(!str) {
    if(debug) NT_print("GoogleMapsPHPHeaderIncludeSite parameter missing from config file");
  }
  else {
    GoogleMapsPHPHeaderIncludeSite = str;
  }
  str = CfgData.GetStrValue("GoogleMapsPHPLeftIncludeCMS");
  if(!str) {
    if(debug) NT_print("GoogleMapsPHPLeftIncludeCMS parameter missing from config file");
  }
  else {
    GoogleMapsPHPLeftIncludeCMS = str;
  }
  str = CfgData.GetStrValue("GoogleMapsPHPLeftIncludeSite");
  if(!str) {
    if(debug) NT_print("GoogleMapsPHPLeftIncludeSite parameter missing from config file");
  }
  else {
    GoogleMapsPHPLeftIncludeSite = str;
  }
  str = CfgData.GetStrValue("GoogleMapsPHPFooterIncludeSite");
  if(!str) {
    if(debug) NT_print("GoogleMapsPHPFooterIncludeSite parameter missing from config file");
  }
  else {
    GoogleMapsPHPFooterIncludeSite = str;
  }
  str = CfgData.GetStrValue("GoogleMapsPHPFooterIncludeCMS");
  if(!str) {
    if(debug) NT_print("GoogleMapsPHPFooterIncludeCMS parameter missing from config file");
  }
  else {
    GoogleMapsPHPFooterIncludeCMS = str;
  }

  // 06/10/2008: Make Google maps disclaimer optional
  str = CfgData.GetStrValue("GoogleMapsDisplayDisclaimer");
  if(!str) {
    if(debug) NT_print("GoogleMapsDisplayDisclaimer parameter missing from config file");
  }
  else {
    CfgFixTrueFalseString(GoogleMapsDisplayDisclaimer, str);
  }
  str = CfgData.GetStrValue("GoogleMapsDisclaimer");
  if(!str) {
    if(debug) NT_print("GoogleMapsDisclaimer parameter missing from config file");
  }
  else {
    GoogleMapsDisclaimer = str;
  }

  // 12/04/2008: Allow Radar overlay to be optional
  str = CfgData.GetStrValue("GoogleMapsRadarOverlay");
  if(!str) {
    if(debug) NT_print("GoogleMapsRadarOverlay parameter missing from config file");
  }
  else {
    CfgFixTrueFalseString(GoogleMapsRadarOverlay, str);
  }

  // 05/23/2008: Added for short fused table
  str = CfgData.GetStrValue("ShortFusedTableLinkName");
  if(!str) {
    if(debug) NT_print("ShortFusedTableLinkName parameter missing from config file");
    }
  else {
    ShortFusedTableLinkName = str;
  }

  str = CfgData.GetStrValue("ShortFusedTableLinkBorder");
  if(!str) {
    if(debug) NT_print("ShortFusedTableLinkBorder parameter missing from config file");
    }
  else {
    ShortFusedTableLinkBorder = str;
  }

  str = CfgData.GetStrValue("ShortFusedTableTitle");
  if(!str) {
    if(debug) NT_print("ShortFusedTableTitle parameter missing from config file");
    }
  else {
    ShortFusedTableTitle = str;
  }

  str = CfgData.GetStrValue("ShortFusedTablePrintUpdateTime");
  if(!str) {
    if(debug) NT_print("ShortFusedTablePrintUpdateTime parameter missing from config file");
    }
  else {
    ShortFusedTablePrintUpdateTime = str;
  }

  str = CfgData.GetStrValue("ShortFusedTableTitleBGColor");
  if(!str) {
    if(debug) NT_print("ShortFusedTableTitleBGColor parameter missing from config file");
    }
  else {
    ShortFusedTableTitleBGColor = str;
  }

  str = CfgData.GetStrValue("ShortFusedTableTitleFontColor");
  if(!str) {
    if(debug) NT_print("ShortFusedTableTitleFontColor parameter missing from config file");
    }
  else {
    ShortFusedTableTitleFontColor = str;
  }

  str = CfgData.GetStrValue("ShortFusedTableTitleFontSize");
  if(!str) {
    if(debug) NT_print("ShortFusedTableTitleFontSize parameter missing from config file");
    }
  else {
    ShortFusedTableTitleFontSize = str;
  }

  str = CfgData.GetStrValue("ShortFusedTableHeaderLink");
  if(!str) {
    if(debug) NT_print("ShortFusedTableHeaderLink parameter missing from config file");
    }
  else {
    ShortFusedTableHeaderLink = str;
  }
  str = CfgData.GetStrValue("ShortFusedTableWidth");
  if(!str) {
    if(debug) NT_print("ShortFusedTableWidth parameter missing from config file");
    }
  else {
    ShortFusedTableWidth = str;
  }

  str = CfgData.GetStrValue("ShortFusedTableBorder");
  if(!str) {
    if(debug) NT_print("ShortFusedTableBorder parameter missing from config file");
    }
  else {
    ShortFusedTableBorder = str;
  }

  str = CfgData.GetStrValue("ShortFusedTableZoneNameTDWidth");
  if(!str) {
    if(debug) NT_print("ShortFusedTableZoneNameTDWidth parameter missing from config file");
    }
  else {
    ShortFusedTableZoneNameTDWidth = str;
  }

  str = CfgData.GetStrValue("ShortFusedTableZoneNameBGColor");
  if(!str) {
    if(debug) NT_print("ShortFusedTableZoneNameBGColor parameter missing from config file");
    }
  else {
    ShortFusedTableZoneNameBGColor = str;
  }
  str = CfgData.GetStrValue("ShortFusedTableZoneNameMouseOver");
  if(!str) {
    if(debug) NT_print("ShortFusedTableZoneNameMouseOver parameter missing from config file");
    }
  else {
    ShortFusedTableZoneNameMouseOver = str;
  }

  str = CfgData.GetStrValue("ShortFusedTableZoneNameFontSize");
  if(!str) {
    if(debug) NT_print("ShortFusedTableZoneNameFontSize parameter missing from config file");
    }
  else {
    ShortFusedTableZoneNameFontSize = str;
  }
  str = CfgData.GetStrValue("ShortFusedTableZoneNameFontColor");
  if(!str) {
    if(debug) NT_print("ShortFusedTableZoneNameFontColor parameter missing from config file");
    }
  else {
    ShortFusedTableZoneNameFontColor = str;
  }
  str = CfgData.GetStrValue("ShortFusedTableProductTDWidth");
  if(!str) {
    if(debug) NT_print("ShortFusedTableProductTDWidth parameter missing from config file");
    }
  else {
    ShortFusedTableProductTDWidth = str;
  }
  str = CfgData.GetStrValue("ShortFusedTableProductFontColor");
  if(!str) {
    if(debug) NT_print("ShortFusedTableProductFontColor parameter missing from config file");
    }
  else {
    ShortFusedTableProductFontColor = str;
  }
  str = CfgData.GetStrValue("ShortFusedTableProductFontSize");
  if(!str) {
    if(debug) NT_print("ShortFusedTableProductFontSize parameter missing from config file");
    }
  else {
    ShortFusedTableProductFontSize = str;
  }
  str = CfgData.GetStrValue("ShortFusedTableProductBGColor");
  if(!str) {
    if(debug) NT_print("ShortFusedTableProductBGColor parameter missing from config file");
    }
  else {
    ShortFusedTableProductBGColor = str;
  }
  str = CfgData.GetStrValue("ShortFusedTableLinkToWarningTable");
  if(!str) {
    if(debug) NT_print("ShortFusedTableLinkToWarningTable parameter missing from config file");
    }
  else {
    ShortFusedTableLinkToWarningTable = str;
  }
  str = CfgData.GetStrValue("ShortFusedTableLinkToWarningTableTitle");
  if(!str) {
    if(debug) NT_print("ShortFusedTableLinkToWarningTableTitle parameter missing from config file");
    }
  else {
    ShortFusedTableLinkToWarningTableTitle = str;
  }

  // 07/10/2008: Added help bullets
  str = CfgData.GetStrValue("WarningTableHelpBullet1");
  if(!str) {
    if(debug) NT_print("WarningTableHelpBullet1 parameter missing from config file");
    }
  else {
    WarningTableHelpBullet1 = str;
  }
  str = CfgData.GetStrValue("WarningTableHelpBullet2");
  if(!str) {
    if(debug) NT_print("WarningTableHelpBullet2 parameter missing from config file");
    }
  else {
    WarningTableHelpBullet2 = str;
  }
  str = CfgData.GetStrValue("WarningTableHelpBullet3");
  if(!str) {
    if(debug) NT_print("WarningTableHelpBullet3 parameter missing from config file");
    }
  else {
    WarningTableHelpBullet3 = str;
  }
  str = CfgData.GetStrValue("WarningTableHelpBullet4");
  if(!str) {
    if(debug) NT_print("WarningTableHelpBullet4 parameter missing from config file");
    }
  else {
    WarningTableHelpBullet4 = str;
  }
  str = CfgData.GetStrValue("WarningTableHelpBullet5");
  if(!str) {
    if(debug) NT_print("WarningTableHelpBullet5 parameter missing from config file");
    }
  else {
    WarningTableHelpBullet5 = str;
  }

  // 07/16/2008: Added a path for Google map page on the web server
  str = CfgData.GetStrValue("GoogleMapsWebServerLocation");
  if(!str) {
    if(debug) NT_print("GoogleMapsWebServerLocation parameter missing from config file");
    }
  else {
    GoogleMapsWebServerLocation = str;
  }

  // 06/18/2009: Addedd RSS configuration for WWA feeds
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

  // 06/23/2009: Product pre and post processing options 
  str = CfgData.GetStrValue("WarningTableSkipDuplicateUGCHeaders");
  if(!str) {
    if(debug) NT_print("WarningTableSkipDuplicateUGCHeaders parameter missing from config file");
  }
  else {
    RPCfgFixTrueFalseString(WarningTableSkipDuplicateUGCHeaders, str);
  }
  str = CfgData.GetStrValue("WarningTableSkipExpiredUGCHeaders");
  if(!str) {
    if(debug) NT_print("WarningTableSkipExpiredUGCHeaders parameter missing from config file");
  }
  else {
    RPCfgFixTrueFalseString(WarningTableSkipExpiredUGCHeaders, str);
  }

  str = CfgData.GetStrValue("WarningTableMatchAllZones");
  if(!str) {
    if(debug) NT_print("WarningTableMatchAllZones parameter missing from config file");
  }
  else {
    RPCfgFixTrueFalseString(WarningTableMatchAllZones, str);
  }

  str = CfgData.GetStrValue("WarningTableSortOrderByHighestTimeStamp");
  if(!str) {
    if(debug) NT_print("WarningTableSortOrderByHighestTimeStamp parameter missing from config file");
  }
  else {
    RPCfgFixTrueFalseString(WarningTableSortOrderByHighestTimeStamp, str);
  }

  // 04/23/2013: Google maps 3 config
  str = CfgData.GetStrValue("GoogleMaps3URL");
  if(!str) {
    if(debug) NT_print("GoogleMaps3URL parameter missing from config file");
  }
  else {
    GoogleMaps3URL = str;
  }
  str = CfgData.GetStrValue("GoogleMaps3Key");
  if(!str) {
    if(debug) NT_print("GoogleMaps3Key parameter missing from config file");
  }
  else {
    GoogleMaps3Key = str;
  }
  str = CfgData.GetStrValue("GoogleMapsVersion");
  if(!str) {
    if(debug) NT_print("GoogleMapsVersion parameter missing from config file");
  }
  else {
    GoogleMapsVersion = str;
  }
  str = CfgData.GetStrValue("GoogleMapsChannel");
  if(!str) {
    if(debug) NT_print("GoogleMapsChannel parameter missing from config file");
  }
  else {
    GoogleMapsChannel = str;
  }

  return 1;
}
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
