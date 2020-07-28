// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ---------------------------------------------------------------- // 
// C++ Header File Name: m_warning_table.h
// C++ Compiler Used: MSVC, GCC
// Produced By: Douglas.Gaer@noaa.gov
// File Creation Date: 01/25/2008
// Date Last Modified: 06/24/2009
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

Code used to generate Watch/Warning table Web pages.
*/
// ----------------------------------------------------------- // 
#ifndef __M_WARNING_TABLE_HPP__
#define __M_WARNING_TABLE_HPP__

#include "gxdlcode.h"

// --------------------------------------------------------------
// Classes
// --------------------------------------------------------------
class IconLink
{
public:
  IconLink() { Clear(); }
  ~IconLink() { }
  IconLink(const IconLink &ob) { Copy(ob); }
  IconLink &operator=(const IconLink &ob);

public:
  void Copy(const IconLink &ob);
  void Clear();

public:
  int priority;   // Warning table priority
  gxString link;  // Hyperlink for this icon
  gxString title; // Title for this hyperlink
};

class UGCSegmentListNode
{
public:
  UGCSegmentListNode() { Clear(); }
  ~UGCSegmentListNode() { }
  UGCSegmentListNode(const UGCSegmentListNode &ob) { Copy(ob); }
  UGCSegmentListNode &operator=(const UGCSegmentListNode &ob);

public:
  int MakeTimeString(gxString &str);
  int MakeTitleString();
  void Clear();
  void Copy(const UGCSegmentListNode &ob);

public:
  gxString pil; // 3 character product PIL
  gxString etn; // 4 digit event tracking number
  gxString full_pil; // 6 or 9 character PIL
  gxString wmo_header; // WMO header
  gxString name; // Product full name
  gxString issue_office; // Office name parsed from product
  TextProduct_time_header issue_time; // Time product was issued
  time_t begin_time; // VTEC being time, UGC, or product issue time
  time_t end_time; // VTEC end time, UGC end time
  int is_expired; // True if product has expired or was canceled
  gxList<gxString> text; // UGC segment text
  gxList<gxString> zones; // List of zone numbers and county codes
  ProductTableData table_data; // Product table line for VTEC pil
  gxString jscode; // Javascript code for popup products and Google maps
  IconLink icon_link; // Hyperlink for the warning table
  gxString ugc_segment_header; // Full UGC segment header
  VTEC vtec; // VTEC code for this segment
  int segment_number;
};

class CurrentWarningZoneNode
{
public:
  CurrentWarningZoneNode() { Clear(); }
  ~CurrentWarningZoneNode() { }
  CurrentWarningZoneNode(const CurrentWarningZoneNode &ob) { Copy(ob); }
  CurrentWarningZoneNode &operator=(const CurrentWarningZoneNode &ob);

public:
  void Copy(const CurrentWarningZoneNode &ob);
  void Clear();

public:
  gxString name;
  gxList<IconLink> product_linked_icons;
  ZoneTableData table_data;
  
public:
  gxString county_code; // Added to include zones and counties as one
};

class UGCSegmentList
{
public:
  UGCSegmentList() { Clear(); }
  ~UGCSegmentList() { }
  UGCSegmentList(const UGCSegmentList &ob) { Copy(ob); }
  UGCSegmentList &operator=(const UGCSegmentList &ob);

public:
  void Clear();
  void Copy(const UGCSegmentList&ob);

public:
  int AddSegmentNode(UGCSegmentListNode &ugc_node);
  int FindSegmentNode(const UGCSegmentListNode &ugc_node, 
		      UGCSegmentListNode &ugc_node_buf,
		      int ignore_segment_number);
  int RemoveSegmentNode(const UGCSegmentListNode &ugc_node, int match_vtec_hdr);
  int RemoveUPGSegmentNode(const UGCSegmentListNode &ugc_node) {  
    return RemoveSegmentNode(ugc_node, 0);
  }
  int RemoveOLDSegmentNode(const UGCSegmentListNode &ugc_node) {
    return RemoveSegmentNode(ugc_node, 1);
  }
  int FindPil(const gxString &pil, 
	      UGCSegmentListNode &ugc_node_buf);
  int UpdateSegmentNode(const UGCSegmentListNode &ugc_node,
			int ignore_segment_number, int update_all);
  int IsSegmentListEmpty() { return ugc_segment_nodes.IsEmpty(); }
  gxListNode<UGCSegmentListNode> *GetSegmentListHead() { 
    return ugc_segment_nodes.GetHead(); 
  }
  int BuildZoneTable();
  int HasIconLink(const gxString &link, CurrentWarningZoneNode &curr_node);
  int AddOrUpdateCurrentWarningZoneNode(CurrentWarningZoneNode &node, int &exists);
  int FindCurrentWarningZoneNode(gxString &zone_name);
  int HasAllZonesNode(const UGCSegmentListNode &list_node, const UGCSegmentListNode &ugc_node);

public: // Internal processing functions
  int find_zone_in_node(const UGCSegmentListNode &node, const gxString &zone);

public:
  gxList<UGCSegmentListNode> ugc_segment_nodes;
  gxList<UGCSegmentListNode> ugc_segment_UPG_remove_list;
  gxList<UGCSegmentListNode> ugc_segment_OLD_remove_list;
  gxList<CurrentWarningZoneNode> current_warnings_by_zone;  
};
// --------------------------------------------------------------

// --------------------------------------------------------------
// Global configuration variables
// --------------------------------------------------------------
gxString WarningTableCWA = "XXX";
gxString WarningTableWebPageName = "DEFAULT";
gxString WarningTableTitle = "DEFAULT";
gxString WarningTableIcons = "DEFAULT"; 
gxString WarningTableIconWidth = "12";
gxString WarningTableIconHeight = "12";
gxString WarningTableData = "DEFAULT";
gxString WarningTableHeaderLink = "NONE";
gxString WarningTablePrintUpdateTime = "1";
gxString WarningTableTimezone = "GMT";
gxString WarningTableArchiveMode = "0";
gxString WarningTableWidth = "525";
gxString WarningTableMainTableAttributes = "NONE";
// Subtable 1 = Area title
gxString WarningSubTable1Width = "AUTO";
// Subtable2 = Area name table
gxString WarningSubTable2Width = "AUTO";
// Subtable 3 = product icons
gxString WarningSubTable3Width = "AUTO";
gxString WarningZoneNameTableDataWidth = "AUTO";
gxString WarningTableRows = "3";
gxString WarningTableBorderTest = "0";
gxString WarningTableTitleBGColor = "#00008b";
gxString WarningTableTitleFontSize = "12pt";
gxString WarningTableTitleFontColor = "white";
gxString WarningTableAreaTitleBGColor = "gray";
gxString WarningTableAreaTitleFontSize = "12pt";
gxString WarningTableAreaTitleFontColor = "white";
gxString WarningTableZoneNameBGColor = "#cccccc";
gxString WarningTableZoneNameFontSize = "10pt";
gxString WarningTableZoneNameFontColor = "black";
gxString WarningTableZoneNameMouseOver = "#ffffb4";
gxString WarningTableProductBGColor = "white";
gxString WarningTableProductFontSize = "10pt";
gxString WarningTableProductFontColor = "black";
gxString WarningTableJStoolbar = "0";
gxString WarningTableJSscrollbars = "1";
gxString WarningTableJSlocation = "0";
gxString WarningTableJSstatusbar = "0";
gxString WarningTableJSmenubar = "1";
gxString WarningTableJSresizable = "1";
gxString WarningTableJSwidth = "600";
gxString WarningTableJSheight = "350";
gxString WarningTableJSleft = "180";
gxString WarningTableJStop = "20";
gxString WarningTableProductWebsite = "http://forecast.weather.gov";
gxString WarningTableProductWebsiteScript = "/product.php";
gxString WarningTableProductWebsiteQuery = "?site=SITEVAR&issuedby=ISSUEDBYVAR&product=PRODUCTVAR&format=FORMATVAR&version=VERSIONVAR&glossary=GLOSSARYVAR";
gxString WarningTableProductEnableWebsiteBackup = "TRUE";
gxString WarningTableProductWebsiteBackup = "http://www.srh.noaa.gov";
gxString WarningTableProductWebsiteScriptBackup = "/productview.php";
gxString WarningTableProductWebsiteQueryBackup = "?pil=MYPRODUCT";
gxString WarningTableSkipDuplicateUGCHeaders = "TRUE";
gxString WarningTableSkipExpiredUGCHeaders = "TRUE";
gxString WarningTableSortOrderByHighestTimeStamp = "TRUE"; 
gxString WarningTableMatchAllZones = "TRUE";
gxString KeyTableLinkName = "Key Table";
gxString KeyTableLinkBorder = "5";
gxString KeyTableWarningTitle = "Warnings/Emergencies";
gxString KeyTableWatchTitle = "Watches/Advisories";
gxString KeyTableAdvisoryTitle = "Statements";
gxString KeyTableWidth = "525";
gxString KeyTableIconWidth = "12";
gxString KeyTableIconHeight = "12";
gxString KeyTableBGColor = "cccccc";
gxString KeyTableTitleBGColor = "#00008b";
gxString KeyTableTitleFontSize = "12pt";
gxString KeyTableTitleFontColor = "white";
gxString KeyTableIconNameFontSize = "10pt";
gxString KeyTableIconNameFontColor = "black";
gxString KeyTableTDWidth = "AUTO";
gxString KeyTableJStoolbar = "0";
gxString KeyTableJSscrollbars = "1";
gxString KeyTableJSlocation = "0";
gxString KeyTableJSstatusbar = "0";
gxString KeyTableJSmenubar = "0";
gxString KeyTableJSresizable = "1";
gxString KeyTableJSwidth = "555";
gxString KeyTableJSheight = "445";
gxString KeyTableJSleft = "180";
gxString KeyTableJStop = "20";
gxString VerTableLinkName = "Verification Table";
gxString VerTableLinkBorder = "5";
gxString VerTableTitle = "DEFAULT";
gxString VerTablePrintUpdateTime = "1";
gxString VerTableTitleBGColor = "#00008b";
gxString VerTableTitleFontColor = "white";
gxString VerTableTitleFontSize = "12pt";
gxString VerTableHeaderLink = "NONE";
gxString VerTableWidth = "AUTO";
gxString VerTableBorder = "1";
gxString VerTableZoneNameTDWidth = "100";
gxString VerTableZoneNameBGColor = "white";
gxString VerTableZoneNameMouseOver = "#ffffb4"; 
gxString VerTableZoneNameFontSize = "10pt";
gxString VerTableZoneNameFontColor = "black";
gxString VerTableProductTDWidth = "AUTO";
gxString VerTableProductFontColor = "black"; 
gxString VerTableProductFontSize = "10pt";
gxString VerTableProductBGColor = "white";
gxString VerTableJStoolbar = "0";
gxString VerTableJSscrollbars = "1";
gxString VerTableJSlocation = "0";
gxString VerTableJSstatusbar = "0";
gxString VerTableJSmenubar = "1";
gxString VerTableJSresizable = "1";
gxString VerTableJSwidth = "640";
gxString VerTableJSheight = "480";
gxString VerTableJSleft = "180";
gxString VerTableJStop = "20";
gxString ShortFusedTableLinkName = "Short Fused Warning Table";
gxString ShortFusedTableLinkBorder = "5";
gxString ShortFusedTableTitle = "DEFAULT";
gxString ShortFusedTablePrintUpdateTime = "1";
gxString ShortFusedTableTitleBGColor = "#00008b";
gxString ShortFusedTableTitleFontColor = "white";
gxString ShortFusedTableTitleFontSize = "12pt";
gxString ShortFusedTableHeaderLink = "NONE";
gxString ShortFusedTableWidth = "AUTO";
gxString ShortFusedTableBorder = "1";
gxString ShortFusedTableZoneNameTDWidth = "100";
gxString ShortFusedTableZoneNameBGColor = "white";
gxString ShortFusedTableZoneNameMouseOver = "#ffffb4"; 
gxString ShortFusedTableZoneNameFontSize = "10pt";
gxString ShortFusedTableZoneNameFontColor = "black";
gxString ShortFusedTableProductTDWidth = "AUTO";
gxString ShortFusedTableProductFontColor = "black"; 
gxString ShortFusedTableProductFontSize = "10pt";
gxString ShortFusedTableProductBGColor = "white";
gxString ShortFusedTableLinkToWarningTable = "DEFAULT";
gxString ShortFusedTableLinkToWarningTableTitle = "DEFAULT";

// 07/10/2008: Added help bullets
gxString WarningTableHelpBullet1;
gxString WarningTableHelpBullet2;
gxString WarningTableHelpBullet3;
gxString WarningTableHelpBullet4;
gxString WarningTableHelpBullet5;
// --------------------------------------------------------------

// --------------------------------------------------------------
// Default file names used to dump copy of HTML tables
// --------------------------------------------------------------
gxString warning_table_html_file = "DEFAULT";
gxString warning_table_js_file = "DEFAULT";
gxString key_table_html_file = "DEFAULT";
gxString ver_table_html_file = "DEFAULT";
gxString short_fused_table_php_file = "DEFAULT";
// --------------------------------------------------------------

// --------------------------------------------------------------
// Global table variables
// --------------------------------------------------------------
UGCSegmentList Warning_table;

// Command line args configuration overrides
gxString product_web_site_arg;
gxString product_web_site_arg_backup;
gxString icon_dir_on_web_server;
gxString input_data_dir;
gxString input_cwa;
gxString dynamic_zone_table_name;
gxString dynamic_zone_js_name;
gxString trigger_table_name;
int use_archive_mode = 0;
int use_dynamic_area_table = 0;
int use_dynamic_zone_table = 1;
int match_dynamic_zone_names = 0;
int drop_unknown_products = 0;
int dump_zone_table = 0;
int dump_trigger_table = 0;
int use_google_map_js_popup = 0;
int use_google_map_html_format = 0;
int use_google_map_php_format = 0;
int archive_test_mode = 0;
int drop_routine_products = 0;
int match_zones_numbers_only = 0;
int add_key_table = 1;
int add_key_table_in_page = 0;
int add_ver_table = 1;
int add_ver_table_in_page = 0;
int add_short_fused_table = 0;
int dump_html_tables = 0;
int dump_warning_table = 0;
int dump_key_table = 0;
int dump_ver_table = 0;
int add_sas_refresh_tag = 0;
int sas_refresh_seconds = 300;
int add_ver_worksheets = 0;
int overwrite_output_worksheet = 0;
gxString worksheet_print_cmd;
gxString worksheet_output_dir = "DEFAULT";
int force_worksheet_test = 0;
int allow_empty_tables = 1;

// 06/18/2009: Add to products RSS feeds for warning table
int rss_feed = 0;
gxString rss_file_ext = ".html";
gxString rss_output_dir = "DEFAULT";
// --------------------------------------------------------------

// Standalone table helper fucntions
int generate_warning_table_page();
int build_warning_table();
int make_warning_table_web_page();
int SetupDownloadURLS(gxString &product_url, gxString &product_url_backup);
int DownloadWarningTableWebProducts();
int gen_key_table(gxString &table);
int add_key_table_icons(gxString &table, const gxString &images_dir, const gxString priority);
int gen_ver_table(gxString &table);
int gen_ver_worksheets();
int gen_short_fused_table(gxString &table);
int gen_rss_feed_all_WWA(gxString &feed);
int gen_rss_feeds_for_all_zones();

#endif // __M_WARNING_TABLE_HPP__
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
 
