// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ---------------------------------------------------------------- // 
// C++ Header File Name: m_tables.h
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

Table classes and functions
*/
// ----------------------------------------------------------- // 
#ifndef __M_TABLES_HPP__
#define __M_TABLES_HPP__

#include "gxdlcode.h"

// --------------------------------------------------------------
// Data structures
// --------------------------------------------------------------
struct AreaTableData
{
public:
  AreaTableData() { Clear(); }
  ~AreaTableData() { }
  AreaTableData(const AreaTableData &ob) { Copy(ob); }
  AreaTableData &operator=(const AreaTableData &ob);

public:
  void Copy(const AreaTableData &ob); 
  void Clear();

public:
  int operator==(AreaTableData &ob);
  int operator>(AreaTableData &ob) { 
    return area_name > ob.area_name;
  }
  int operator<(AreaTableData &ob) {
    return area_name < ob.area_name;
  }

public:
  gxString area_number;
  gxString area_name;

  // Table example
  // AREA NUMBER | AREA NAME
  // 0 | NONE
  // 1 | South Florida Land Areas
  // 2 | Atlantic Waters
  // 3 | Gulf Waters
};

class ZoneTableData
{
public:
  ZoneTableData() { Clear(); }
  ~ZoneTableData() { }
  ZoneTableData(const ZoneTableData &ob) { Copy(ob); }
  ZoneTableData &operator=(const ZoneTableData &ob);

public:
  void Copy(const ZoneTableData &ob);
  void Clear();

public:
  gxString zone_number;
  gxString zone_name;
  gxString area_number;
  gxString zone_title;
  gxString zone_url;

  // Loaded from area table
  gxString area_name;
};

class ProductTableData
{
public:
  ProductTableData() { Clear(); }
  ~ProductTableData() { }
  ProductTableData(const ProductTableData &ob) { Copy(ob); }
  ProductTableData &operator=(const ProductTableData &ob);

public:
  void Clear();
  void Copy(const ProductTableData &ob);

public:
  gxString pil;
  gxString vtec_pil;
  gxString replace_pil;
  gxString office;
  gxString priority; 
  gxString fuse; 
  gxString sas; 
  gxString name;
};
// --------------------------------------------------------------

// --------------------------------------------------------------
// Global configuration file names
// --------------------------------------------------------------
// Set the default file names
gxString area_table_file = "DEFAULT";
gxString zone_table_file = "DEFAULT";
gxString product_table_file = "DEFAULT";
// --------------------------------------------------------------

// --------------------------------------------------------------
// Global configuration code tables
// --------------------------------------------------------------
gxList<AreaTableData> Area_table;
gxList<ZoneTableData> Zone_table;
gxList<ProductTableData> Product_table;
gxList<gxString> Timezone_table;
gxList<gxString> PVTEC_status_table;
gxList<gxString> PVTEC_action_table;
gxList<gxString> PVTEC_significance_table;
gxList<gxString> PVTEC_phenomena_table;
gxList<gxString> HVTEC_flood_severity_table;
gxList<gxString> HVTEC_immediate_cause_table;
gxList<gxString> HVTEC_flood_record_table;
gxList<gxString> Followup_phenomena_code_to_pil_table;
gxList<gxString> Followup_statement_pils_table;
// --------------------------------------------------------------

// --------------------------------------------------------------
// Global configuration default codes used to initalize config tables
// --------------------------------------------------------------
gxString Timezone_codes = "\
ATL(4),\
AST(4),\
EDT(4),\
EST(5),\
CDT(5),\
CST(6),\
MDT(6),\
MST(7),\
PDT(7),\
PST(8),\
ALA(9),\
HAW(10),\
GMT(0),\
UTC(0),\
JST(-9),\
MET(-1)\
";

gxString Followup_statement_pils = "SVS,FFS,MWS";

gxString Followup_phenomena_code_to_pil = "\
EW EWW,\
FF FFW,\
MA SMW,\
SV SVR,\
TO TOR\
";

gxString PVTEC_status_codes = "\
0 Operational product,\
T Test product,\
E Experimental product,\
X Experimental VTEC in Operational product\
";

gxString PVTEC_action_codes = "\
NEW In Affect,\
CON Continued,\
EXA Extended Area,\
EXT Extended Time,\
EXB Extended Time and Area,\
UPG Upgraded,\
CAN Cancelled,\
EXP Expired,\
ROU Routine,\
COR Corrected\
";

gxString PVTEC_significance_codes = "\
W Warning,\
A Watch,\
Y Advisory,\
S Statement,\
F Forecast,\
O Outlook,\
N Synopsis\
";

gxString PVTEC_phenomena_codes = "\
AF Volcanic Ashfall,\
AS Air Stagnation,\
AV Avalanche,\
BS Blowing Drifting Snow,\
BW Brisk Wind,\
BZ Blizzard,\
CF Coastal Flood,\
DS Dust Storm,\
DU Blowing Dust,\
EC Extreme Cold,\
EH Excessive Heat,\
EW Extreme Wind,\
FA Areal Flood,\
FF Flash Flood,\
FG Dense Fog,\
FL Flood,\
FR Frost,\
FW Fire Weather,\
FZ Freeze,\
GL Gale,\
HF Hurricane Force Wind,\
HI Inland Hurricane,\
HP Heavy Sleet,\
HS Heavy Snow,\
HT Heat,\
HU Hurricane,\
HW High Wind,\
HY Hydrologic,\
IP Sleet,\
IS Ice Storm,\
LB Lake Effect Snow and Blowing Snow,\
LE Lake Effect Snow,\
LO Low Water,\
LS Lakeshore Flood,\
LW Lake Wind,\
MA Marine,\
RH Radiological Hazard,\
SB Snow and Blowing Snow,\
SC Small Craft,\
SM Dense Smoke,\
SN Snow,\
SN Storm,\
SU High Surf,\
SV Severe Thunderstorm,\
SW Small Craft for Seas,\
TI Inland Tropical Storm,\
TO Tornado,\
TR Tropical Storm,\
TS Tsumnami,\
TY Typhoon,\
UP Ice Accretion,\
VO Volcano,\
WC Wind Chill,\
WI Wind,\
WS Winter Storm,\
WW Winter Weather,\
ZF Freezing Fog,\
ZR Freezing Rain\
";

gxString HVTEC_flood_severity_codes = "\
N None,\
0 For Flash Flood and Areal Flood Warnings,\
1 Minor,\
2 Moderate,\
3 Major,\
U Unknown\
";

gxString HVTEC_immediate_cause_codes = "\
ER Excessive Rainfall,\
SM Snowmelt,\
RS Rain and Snowmelt,\
DM Dam Levee or Failure,\
IJ Ice Jam,\
GO Glacier-Dammed Lake Outburst,\
IC Rain and or Snowmelt and or Ice Jam,\
FS Upstream Flooding plus Storm Surge,\
FT Upstream Flooding plus Tidal Effects,\
ET Elevated Upstram Flow plus Tidal Effects,\
WT Wind and or Tidal Effects,\
DR Upstream Dam or Reservoir Relase,\
MC Other Multiple Causes,\
OT Other Effects,\
UU Unknown\
";

gxString HVTEC_flood_record_codes = "\
NO A record flood is not expected,\
NR A near record or record flood is expected,\
UU Flood without a period of record to compare,\
OO For areal flood warnings or areal flash flood products\
";
// --------------------------------------------------------------


// --------------------------------------------------------------
// Time tables
// --------------------------------------------------------------
// Tables used by UNIX system time functions and product time functions
gxString System_weekdays[7] = { "SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT" }; 

gxString System_months[12] = { "JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", \
				"AUG", "SEP", "OCT", "NOV", "DEC" };
// --------------------------------------------------------------

// Table functions
int LoadVTECTables();
int LoadTimezoneTables();
int LoadAreaTable();
int LoadZoneTable();
int LoadProductTable();
int BuildDefaultProductTable(const gxString &fname, const gxString &MYCWA);
int BuildDefaultAreaTable(const gxString &fname, const gxString &MYCWA);
int BuildDefaultZoneTable(const gxString &fname, const gxString &MYCWA);
int ParseLine(gxString &info_line, AreaTableData &table_data);
int ParseLine(gxString &info_line, ZoneTableData &table_data);
int ParseLine(gxString &info_line, ProductTableData &table_data);
int DumpDynamicZoneTable(const gxString &fname, const gxString &MYCWA);
int DumpDynamicZoneJS(const gxString &fname, const gxString &MYCWA);
int DumpTriggerTable(const gxString &fname, const gxString &MYCWA);

// Search functions and replace
int FindProductListFile(const gxString &product_name);
int FindProductTableProduct(const gxString &product, const gxString &vtec_pil, 
			    ProductTableData &table_data);
int FindProductTablePil(const gxString &product, ProductTableData &table_data);
int FindProductTableVTECPil(const gxString &vtec_pil, ProductTableData &table_data);
int FindAreaName(const gxString &area_number, gxString &name);
int FindAreaNumber(const gxString &area_number);
int FindPIL(const gxString &PIL, gxString &pbuf);
int FindZoneNumber(const gxString &zone_number, ZoneTableData &table_data);
int FindZoneNumber(const gxString &zone_number);
int FindZoneName(const gxString &zone_number, gxString &name);
int FindZoneByName(const gxString &zone_name);
int FindZoneByName(const gxString &zone_name, ZoneTableData &table_data);
int UpdateZoneName(const gxString &zone_number, ZoneTableData &zone_table_data);
int FindInVTECTable(const gxString &phenomena, const gxString &Significance, gxString &title);

// String utilites
void FilterStringField(gxString &s);
void FixCSVLine(gxString &line);
void FilterStringValue(gxString &s);
void FixPipeLine(gxString &line);

#endif // __M_TABLES_HPP__
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
 
