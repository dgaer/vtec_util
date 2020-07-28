// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- // 
// C++ Source Code File Name: m_help.cpp
// Compiler Used: MSVC, GCC
// Produced By: Douglas.Gaer@noaa.gov
// File Creation Date: 01/25/2008
// Date Last Modified: 06/19/2009
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

Program help code for electronic documentation
*/
// ----------------------------------------------------------- // 
#include "gxdlcode.h"

void HelpMessage()
{
  GXSTD::cout << "Usage: " << ProgramName << " [args] filename" << "\n" << flush;
  GXSTD::cout << "filename: " << "Text file containing the product" << "\n" << flush;
  GXSTD::cout << "args: " << "\n" << flush;
  GXSTD::cout << "     --help                      Print help message and exit" << "\n" << flush;
  GXSTD::cout << "     --version                   Print version number and exit" << "\n" << flush;
  GXSTD::cout << "     --debug                     Enable debug mode default level --debug=1" << "\n" << flush;
  GXSTD::cout << "     --verbose                   Turn on verbose messaging to stderr" << "\n" << flush;
  GXSTD::cout << "     --config-file=\"config.cfg\"  Specify configuration CFG file location" << "\n" << flush;
  GXSTD::cout << "     --log-file=\"mylogfile.log\"  Use log file to log program output" << "\n" << flush;
  GXSTD::cout << "     --log-file-clear            Clear the log file when program starts" << "\n" << flush;
  GXSTD::cout << "     --product-file=\"list.txt\"   List of file names or URLs to load" << "\n" << flush;
  GXSTD::cout << "     --parse-product             Parse and print product elements" << "\n" << flush;
  GXSTD::cout << "     --read-product              Read and print formatted elements" << "\n" << flush;
  GXSTD::cout << "     --read-product=\"rpconf.cfg\" Use formatting in configuration file" << "\n" << flush;
  GXSTD::cout << "     --rss-product               Read and output as RSS feed" << "\n" << flush;
  GXSTD::cout << "     --rss-product=\"rpconf.cfg\"  Use formatting in configuration file" << "\n" << flush;
  GXSTD::cout << "     --google-maps               Make Google map for product or product list" << "\n" << flush;
  GXSTD::cout << "     --warning-table             Make a warning table Web page for your CWA" << "\n" << flush;
  GXSTD::cout << "     --product-download=\"CCCNNN\" Download Web product to working DIR" << "\n" << flush;

  if(list_all_cmd_args) {
    GXSTD::cout << "     --debug=#level              Set the debug level (1, 2, 3): --debug=1" << "\n" << flush;
    GXSTD::cout << "     --log-file-clear\"file\"      Specify log file name to open and clear" << "\n" << flush;
    GXSTD::cout << "     --download-fname=\"name\"     Use specifed file name for Web product" << "\n" << flush;
    GXSTD::cout << "     --download-site=\"URL\"       Specify Website for product download" << "\n" << flush;
    GXSTD::cout << "     --download-site2=\"URL\"      Backup Website for product download" << "\n" << flush;
    GXSTD::cout << "     --download-retries=\"num\"    Specify number of download retries" << "\n" << flush;
    GXSTD::cout << "     --download-pause=\"secs\"     Number of seconds between retries" << "\n" << flush;
    GXSTD::cout << "     --html                      Use HTML format for Web pages" << "\n" << flush;
    GXSTD::cout << "     --html=\".txt\"               Specify dot extension for HTML files" << "\n" << flush;
    GXSTD::cout << "     --html-include              Use HTML include format for Web pages" << "\n" << flush;
    GXSTD::cout << "     --html-include=\".txt\"       Specify dot extension for HTML includes" << "\n" << flush;
    GXSTD::cout << "     --php                       Use PHP include format for Web pages" << "\n" << flush;
    GXSTD::cout << "     --php=\".txt\"                Specify dot extension for PHP includes" << "\n" << flush;
    GXSTD::cout << "     --js-ext=\".txt\"             Specify dot extension for JS files" << "\n" << flush;
    GXSTD::cout << "     --rss                       Produce RSS feeds with warning table" << "\n" << flush;
    GXSTD::cout << "     --rss=\".txt\"                Specify dot extension for RSS files" << "\n" << flush;
    GXSTD::cout << "     --rss-outdir=\"directory\"    Specify output DIR for RSS files" << "\n" << flush;
    GXSTD::cout << "     --exec-file=\"command\"       Execute command after file is created" << "\n" << flush;
    GXSTD::cout << "     --cwa=\"CWA\"                 Specify warning table CWA if not in CFG file" << "\n" << flush;
    GXSTD::cout << "     --data-dir=\"directory\"      Specify input data DIR if not in CFG file" << "\n" << flush;
    GXSTD::cout << "     --arch-mode                 Read old products for archiving and testing" << "\n" << flush;
    GXSTD::cout << "     --arch-mode=TEST            Do not expire old products when testing" << "\n" << flush;
    GXSTD::cout << "     --get-web-products          Get warning products from Website in CFG file " << "\n" << flush;
    GXSTD::cout << "     --get-web-products=\"site\"   Specify Website to download products from" << "\n" << flush;
    GXSTD::cout << "     --icon-dir=\"directory\"      Specify Web icon DIR if not in CFG file" << "\n" << flush;
    GXSTD::cout << "     --output-dir=\"directory\"    Specify Web output DIR if not in CFG file" << "\n" << flush;
    GXSTD::cout << "     --no-dynamic-zones          Do not add any UGC zones to your zone table" << "\n" << flush;
    GXSTD::cout << "     --match-dynamic-names       Match UGC header names to unknown zone numbers" << "\n" << flush;
    GXSTD::cout << "     --dump-zone-table           Dump a copy of your current zone table" << "\n" << flush;
    GXSTD::cout << "     --dump-zone-table=\"file\"    Specify file name to dump zone table in" << "\n" << flush;
    GXSTD::cout << "     --drop-unknown-products     Drop if not listed in product table" << "\n" << flush;
    GXSTD::cout << "     --drop-routine-products     Drop all routine products" << "\n" << flush;
    GXSTD::cout << "     --no-overwrite              Do not overwrite output Web pages" << "\n" << flush;
    GXSTD::cout << "     --dump-trigger-list         Dump trigger listing for WWA products" << "\n" << flush;
    GXSTD::cout << "     --dump-trigger-list=\"file\"  Specify file name for trigger listing" << "\n" << flush;
    GXSTD::cout << "     --match-codes-only          Do not allow duplicate numbers in zone table" << "\n" << flush;
    GXSTD::cout << "     --gen-js-files              Write Google map external javascript files" << "\n" << flush;
    GXSTD::cout << "     --no-key-table              Do not write a key table" << "\n" << flush;
    GXSTD::cout << "     --key-table-in-page         Write the key table in the HTML page" << "\n" << flush;
    GXSTD::cout << "     --no-ver-table              Do not write a verification table " << "\n" << flush;
    GXSTD::cout << "     --ver-table-in-page         Write the verification table in the HTML page " << "\n" << flush;
    GXSTD::cout << "     --dump-html-tables          Write a copy of all the HTML tables" << "\n" << flush;
    GXSTD::cout << "     --dump-warning-table        Write a copy of the warning table HTML code" << "\n" << flush;
    GXSTD::cout << "     --dump-warning-table=\"file\" Specify name for table file" << "\n" << flush;
    GXSTD::cout << "     --dump-key-table            Write a copy of the key table HTML code" << "\n" << flush;
    GXSTD::cout << "     --dump-key-table=\"file\"     Specify name for table file" << "\n" << flush;
    GXSTD::cout << "     --dump-ver-table            Write a copy of the verification HTML code" << "\n" << flush;
    GXSTD::cout << "     --dump-ver-table=\"file\"     Specify name for table file" << "\n" << flush;
    GXSTD::cout << "     --add-sas-refresh-tag       Use local Web page SAS table refresh" << "\n" << flush;
    GXSTD::cout << "     --add-sas-refresh-tag=secs  Local SAS table refresh num seconds" << "\n" << flush;
    GXSTD::cout << "     --ver-worksheets            Write new worksheet for short fused products" << "\n" << flush;
    GXSTD::cout << "     --ver-worksheets-overwrite  Write a worksheet for short fused products" << "\n" << flush;
    GXSTD::cout << "     --worksheet-out-dir=\"DIR\"   Output DIR for worksheet files" << "\n" << flush;
    GXSTD::cout << "     --worksheet-print-cmd=\"CMD\" Command used to print worksheet" << "\n" << flush;
    GXSTD::cout << "     --force-worksheet-test      Write worksheet for expired products" << "\n" << flush;
    GXSTD::cout << "     --disable-empty-tables      Do not build table with empty product list" << "\n" << flush; 
    GXSTD::cout << "     --shortfused-table          Build short fused include with warning table" << "\n" << flush;
    GXSTD::cout << "     --shortfused-table=\"file\"   Specify file name for short fused include" << "\n" << flush;
  }

  GXSTD::cout << "\n" << flush;
  if(list_all_cmd_args) {
    GXSTD::cout << "Short list of command line options:" << "\n" << flush;
    GXSTD::cout << "     --help                      Print short list of args" << "\n" << flush;
  }
  else {
    GXSTD::cout << "List all command line options:" << "\n" << flush;
    GXSTD::cout << "     --help-all                  Print all command line args" << "\n" << flush;
  }
  GXSTD::cout << "\n" << flush;
  GXSTD::cout << "Extended help topics:" << "\n" << flush;
  GXSTD::cout << "     --warning-table --help      Print extended help for Warning tables" << "\n" << flush;
  GXSTD::cout << "     --google-maps --help        Print extended help for Google maps" << "\n" << flush;

  if(make_warning_table) {
    GXSTD::cout << "\n" << flush;
    GXSTD::cout << "The --warning-table option is used to generate a table of all current"
		<< "\n"
		<< "watch, warnings, and advisories for a given CWA."
		<< "\n";
    GXSTD::cout << "\n" << flush;

    GXSTD::cout << "Examples:" << "\n" << flush; 
    GXSTD::cout << ProgramName << " --warning-table --cwa=MFL --config-file=MFL.cfg --data-dir=/data/MFL" 
		<< "\n" << flush;
    GXSTD::cout << ProgramName << " --warning-table --cwa=JAX --config-file=JAX.cfg --get-web-products" 
		<< "\n" << flush;
    GXSTD::cout << ProgramName << " --warning-table --cwa=MRX --config-file=MRX.cfg --get-web-products --verbose" 
		<< "\n" << flush;
    GXSTD::cout << "\n" << flush;
    GXSTD::cout << "The --warning-table option can also be used to create polygon warning maps"
		<< "\n";
    GXSTD::cout << "\n" << flush;
    GXSTD::cout << "Examples:" << "\n" << flush; 
    GXSTD::cout << ProgramName << " --warning-table --cwa=MFL --config-file=MFL.cfg --google-maps --data-dir=/data/MFL" 
		<< "\n" << flush;
    GXSTD::cout << "\n" << flush;
    GXSTD::cout << "The example above will link to the table to polygon warning HTML files" << "\n" << flush; 
    GXSTD::cout << "To change the formatting of the of the Google map pages you can use:" << "\n" << flush; 
    GXSTD::cout << " --google-maps-popup (Use JavaScript popups NOTE: FireFox only)" << "\n" << flush; 
    GXSTD::cout << " --google-maps-html (Always create and link to HTML map pages)" << "\n" << flush; 
    GXSTD::cout << " --google-maps-php (Generate map pages as PHP include file)" << "\n" << flush; 
    GXSTD::cout << "\n" << flush;
    GXSTD::cout << "When the program is ran for the first time it will create the CFG file"
		<< "\n"
		<< "and Zone, Area, and Product table files that can be customized for your CWA."
		<< "\n";
    GXSTD::cout << "\n" << flush;
    GXSTD::cout << "The --warning-table output can be produced as an PHP include file." << "\n";
    GXSTD::cout << "Examples below produce PHP include files that can be instered into other pages:" << "\n";
    GXSTD::cout << "\n" << flush;
    GXSTD::cout << ProgramName << " --warning-table-php --cwa=MFL --config-file=MFL.cfg --data-dir=/data/MFL" 
		<< "\n" << flush;
    GXSTD::cout << ProgramName << " --warning-table-php=.txt --cwa=MFL --config-file=MFL.cfg --data-dir=/data/MFL" 
		<< "\n" << flush;
    GXSTD::cout << ProgramName << " --warning-table-php --cwa=MFL --config-file=MFL.cfg --google-maps-html --data-dir=/data/MFL" 
		<< "\n" << flush;
    GXSTD::cout << "\n" << flush;
    GXSTD::cout << "A short fused include table can be produced with the icon based warning table." << "\n";
    GXSTD::cout << "Example below produces short fused PHP include:" << "\n";
    GXSTD::cout << "\n" << flush;
    GXSTD::cout << ProgramName << "--php --warning-table --cwa=MFL --config-file=MFL.cfg --google-maps --shortfused-table" 
		<< "\n" << flush;
    GXSTD::cout << "\n" << flush;
    GXSTD::cout << "A warning table with RSS feeds." << "\n";
    GXSTD::cout << "Example below produces a warning table and RSS feeds as TXT include files " << "\n";
    GXSTD::cout << "\n" << flush;
    GXSTD::cout << ProgramName << "--html-include=.txt --warning-table --google-maps --cwa=MFL --config-file=MFL.cfg --rss=.txt --rss-outdir=/data/rss" 
		<< "\n" << flush;
    GXSTD::cout << "\n" << flush;
   }

  if(make_google_map_page) {
    GXSTD::cout << "\n" << flush;
    GXSTD::cout << "The --google-maps option is used to generate polygon based warnings on"
		<< "\n"
		<< "a map downloaded from the Google Earth server"
		<< "\n";
    GXSTD::cout << "\n" << flush;

    GXSTD::cout << "Examples:" << "\n" << flush; 
    GXSTD::cout << ProgramName << " --google-maps TORMFL.txt" 
		<< "\n" << flush;
    GXSTD::cout << ProgramName << " --google-maps http://www.srh.noaa.gov/productview.php?pil=TORMFL" 
		<< "\n" << flush;
    GXSTD::cout << ProgramName << " --google-maps --exec-file=\"scp MYSRC user@host:/mfl/poly/MYDEST\" SVRMFL.txt" 
		<< "\n" << flush;
    GXSTD::cout << "\n" << flush;
    GXSTD::cout << "The --google-maps output can be produced as an PHP include file." << "\n";
    GXSTD::cout << "Examples below produce PHP include files that can be instered into other pages:" << "\n";
    GXSTD::cout << "\n" << flush;
    GXSTD::cout << ProgramName << " --google-maps-php http://www.srh.noaa.gov/productview.php?pil=TORMFL" 
		<< "\n" << flush;
    GXSTD::cout << ProgramName << " --google-maps-php=.txt http://www.srh.noaa.gov/productview.php?pil=TORMFL" 
		<< "\n" << flush;
    GXSTD::cout << "\n" << flush;
  }

  GXSTD::cout << "\n" << flush;
  return;
}

void VersionMessage()
{
  GXSTD::cout << "\n" << flush;
  GXSTD::cout << ProgramDescription << " version " << VersionString  << "\n" << flush;
  GXSTD::cout << "Produced by: " << ProducedBy << "\n" << flush;
  GXSTD::cout << "\n" << flush;
}
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
