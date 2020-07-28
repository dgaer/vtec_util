// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- // 
// C++ Source Code File Name: m_tables.cpp
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

Table classes and functions
*/
// ----------------------------------------------------------- // 
#include "gxdlcode.h"

void FixPipeLine(gxString &line)
{
  line.TrimLeading('|');
  line.TrimTrailing('|');
  line.ReplaceString("||", "|");
}

void FixCSVLine(gxString &line)
{
  line.TrimLeading(',');
  line.TrimTrailing(',');
  line.ReplaceString(",,", ",");
}

void FilterStringField(gxString &s)
{
  s.TrimLeadingSpaces();
  s.TrimTrailingSpaces();
  s.TrimLeading('"');
  s.TrimTrailing('"');
}

void FilterStringValue(gxString &s)
{
  s.TrimLeadingSpaces();
  s.TrimTrailingSpaces();
  s.FilterChar('\n');
  s.FilterChar('\r');
}

int LoadVTECTables()
{
  char words[MAXWORDS][MAXWORDLENGTH];
  int numwords;
  int i;
  gxString sbuf;

  FixCSVLine(PVTEC_status_codes);
  if(parse(PVTEC_status_codes.c_str(), words, &numwords, ',') == 1) {
    NT_print("Error parsing PVTEC status table");
    return 0;
  }
  for(i = 0; i < numwords; i ++) {
    if(*words[i] != 0) {
      sbuf = words[i];
      PVTEC_status_table.Add(sbuf);
    }
  }
  FixCSVLine(PVTEC_action_codes);
  if(parse(PVTEC_action_codes.c_str(), words, &numwords, ',') == 1) {
    NT_print("Error parsing PVTEC action table");
    return 0;
  }
  for(i = 0; i < numwords; i ++) {
    if(*words[i] != 0) {
      sbuf = words[i];
      PVTEC_action_table.Add(sbuf);
    }
  }
  FixCSVLine(PVTEC_significance_codes);
  if(parse(PVTEC_significance_codes.c_str(), words, &numwords, ',') == 1) {
    NT_print("Error parsing PVTEC significance table");
    return 0;
  }
  for(i = 0; i < numwords; i ++) {
    if(*words[i] != 0) {
      sbuf = words[i];
      PVTEC_significance_table.Add(sbuf);
    }
  }
  FixCSVLine(PVTEC_phenomena_codes);
  if(parse(PVTEC_phenomena_codes.c_str(), words, &numwords, ',') == 1) {
    NT_print("Error parsing PVTEC phenomena table");
    return 0;
  }
  for(i = 0; i < numwords; i ++) {
    if(*words[i] != 0) {
      sbuf = words[i];
      PVTEC_phenomena_table.Add(sbuf);
    }
  }
  FixCSVLine(HVTEC_flood_severity_codes);
  if(parse(HVTEC_flood_severity_codes.c_str(), words, &numwords, ',') == 1) {
    NT_print("Error parsing HVTEC flood severity table");
    return 0;
  }
  for(i = 0; i < numwords; i ++) {
    if(*words[i] != 0) {
      sbuf = words[i];
      HVTEC_flood_severity_table.Add(sbuf);
    }
  }
  FixCSVLine(HVTEC_immediate_cause_codes);
  if(parse(HVTEC_immediate_cause_codes.c_str(), words, &numwords, ',') == 1) {
    NT_print("Error parsing HTVEC immediate cause table");
    return 0;
  }
  for(i = 0; i < numwords; i ++) {
    if(*words[i] != 0) {
      sbuf = words[i];
      HVTEC_immediate_cause_table.Add(sbuf);
    }
  }
  FixCSVLine(HVTEC_flood_record_codes);
  if(parse(HVTEC_flood_record_codes.c_str(), words, &numwords, ',') == 1) {
    NT_print("Error parsing HTVEC flood record table");
    return 0;
  }
  for(i = 0; i < numwords; i ++) {
    if(*words[i] != 0) {
      sbuf = words[i];
      HVTEC_flood_record_table.Add(sbuf);
    }
  }

  FixCSVLine(Followup_statement_pils);
  if(parse(Followup_statement_pils.c_str(), words, &numwords, ',') == 1) {
    NT_print("Error parsing followup statement pils");
    return 0;
  }
  for(i = 0; i < numwords; i ++) {
    if(*words[i] != 0) {
      sbuf = words[i];
      Followup_statement_pils_table.Add(sbuf);
    }
  }

  FixCSVLine(Followup_phenomena_code_to_pil);
  if(parse(Followup_phenomena_code_to_pil.c_str(), words, &numwords, ',') == 1) {
    NT_print("Error parsing followup phenomena code to pil table");
    return 0;
  }
  for(i = 0; i < numwords; i ++) {
    if(*words[i] != 0) {
      sbuf = words[i];
      Followup_phenomena_code_to_pil_table.Add(sbuf);
    }
  }

  return 1;
}

int LoadTimezoneTables()
{
  char words[MAXWORDS][MAXWORDLENGTH];
  int numwords;
  int i;
  gxString sbuf;

  FixCSVLine(Timezone_codes);

  if(parse(Timezone_codes.c_str(), words, &numwords, ',') == 1) {
    NT_print("Error loading timezone table codes");
    NT_print(Timezone_codes.c_str());
    return 0;
  }

  Timezone_table.ClearList();
  for(i = 0; i < numwords; i++) {
    if(*words[i] != 0) {
      sbuf = words[i];
      Timezone_table.Add(sbuf);
    }
  }

  if(Timezone_table.IsEmpty()) {
    NT_print("Error bad timezone table");
    NT_print(Timezone_codes.c_str());
    return 0;
  }

  return 1;
}

int LoadAreaTable()
{
  char sbuf[1024];
  DiskFileB dfile;
  int line_number = 0;
  gxString mesg;
  Area_table.ClearList();

  NT_print("Loading area table file", area_table_file.c_str());
 
  // Open the current product file
  dfile.df_Open(area_table_file.c_str());
  if(!dfile) {
    NT_print("Cannot open area table file", area_table_file.c_str());
    NT_print(dfile.df_ExceptionMessage());
    return 0;
  }

  while(!dfile.df_EOF()) {
    // Get each line of the file and trim the line feeds
    dfile.df_GetLine(sbuf, sizeof(sbuf), '\n');
    if(dfile.df_GetError() != DiskFileB::df_NO_ERROR) {
      NT_print("A fatal I/O error occurred while reading area table");
      NT_print("Cannot read file", area_table_file.c_str());
      NT_print(dfile.df_ExceptionMessage());
      dfile.df_Close();
      return 0;
    }
    line_number++;

    UString info_line(sbuf);
    info_line.FilterChar('\n');
    info_line.FilterChar('\r');
    info_line.TrimLeadingSpaces();
    info_line.TrimTrailingSpaces();

    if(info_line.is_null()) continue;
    
    // Skip remark lines
    if(info_line[0] == '#') continue; 
    if(info_line[0] == ';') continue; 

    AreaTableData table_data;
    if(!ParseLine(info_line, table_data)) {
      mesg << clear << "Error in area table on line number " << line_number;
      NT_print(mesg.c_str());
      dfile.df_Close();
      return 0;
    }

    Area_table.Add(table_data);
  }

  dfile.df_Close();

  return 1;
}

int ParseLine(gxString &info_line, AreaTableData &table_data)
{
  char words[MAXWORDS][MAXWORDLENGTH];
  int numwords;
  table_data.Clear();

  if(parse(info_line.c_str(), words, &numwords, '|') == 1) {
    NT_printlines("Error parsing line", info_line.c_str());
    return 0;
  }

  if(numwords < 2) {
    NT_printlines("Error parsing area table line", info_line.c_str(), "Missing fields");
    return 0;
  }
  if(numwords > 2) {
    NT_printlines("Error parsing area table line", info_line.c_str(), "Extra fields");
    return 0;
  }

  if(*words[0] != 0) {
    table_data.area_number = words[0];
    FilterStringField(table_data.area_number);
  }
  if(*words[1] != 0) {
    table_data.area_name = words[1];
    FilterStringField(table_data.area_name);
  }

  if(table_data.area_number.is_null()) {
    NT_printlines("Bad area number", "Error parsing area table line", info_line.c_str());
    return 0;
  }
  if(table_data.area_name.is_null()) {
    NT_printlines("Bad area name", "Error parsing area table line", info_line.c_str());
    return 0;
  }

  return 1;
}

AreaTableData &AreaTableData::operator=(const AreaTableData &ob)
{
  if(&ob != this) Copy(ob);
  return *this;
}

void AreaTableData::Copy(const AreaTableData &ob)
{
  area_number = ob.area_number;
  area_name = ob.area_name;
}

void AreaTableData::Clear() 
{
  area_number.Clear();
  area_name.Clear();
}

int AreaTableData::operator==(AreaTableData &ob) 
{
  if(area_number == ob.area_number) {
    // Check for duplicate area numbers
    return area_name == ob.area_name;
  }
  return area_name == ob.area_name;
}

ZoneTableData &ZoneTableData::operator=(const ZoneTableData &ob)
{
  if(&ob != this) Copy(ob);
  return *this;
}

void ZoneTableData::Copy(const ZoneTableData &ob)
{
  zone_number = ob.zone_number;
  zone_name = ob.zone_name;
  area_number = ob.area_number;
  area_name = ob.area_name;
  zone_title = ob.zone_title;
  zone_url = ob.zone_url;
}

void ZoneTableData::Clear() 
{
  zone_number.Clear();
  zone_name.Clear();
  area_number.Clear();
  area_name.Clear();
  zone_title.Clear();
  zone_url.Clear();
}

ProductTableData &ProductTableData::operator=(const ProductTableData &ob)
{
  if(&ob != this) Copy(ob);
  return *this;
}

void ProductTableData::Clear()
{
  pil.Clear();
  vtec_pil.Clear();
  replace_pil.Clear();
  office.Clear();
  priority.Clear();
  fuse.Clear(); 
  sas.Clear(); 
  name.Clear();
}

void ProductTableData::Copy(const ProductTableData &ob)
{
  pil = ob.pil;
  vtec_pil = ob.vtec_pil;
  replace_pil = ob.replace_pil;
  office = ob.office;
  priority = ob.priority; 
  fuse = ob.fuse; 
  sas = ob.sas; 
  name = ob.name;
}

int LoadProductTable()
{
  char sbuf[1024];
  DiskFileB dfile;
  int line_number = 0;
  gxString mesg;
  Product_table.ClearList();

  NT_print("Loading product table file", product_table_file.c_str());
 
  // Open the current product file
  dfile.df_Open(product_table_file.c_str());
  if(!dfile) {
    NT_print("Cannot open product table file", product_table_file.c_str());
    NT_print(dfile.df_ExceptionMessage());
    return 0;
  }

  while(!dfile.df_EOF()) {
    // Get each line of the file and trim the line feeds
    dfile.df_GetLine(sbuf, sizeof(sbuf), '\n');
    if(dfile.df_GetError() != DiskFileB::df_NO_ERROR) {
      NT_print("A fatal I/O error occurred while reading product table");
      NT_print("Cannot read file", product_table_file.c_str());
      NT_print(dfile.df_ExceptionMessage());
      dfile.df_Close();
      return 0;
    }
    line_number++;
    
    UString info_line(sbuf);
    info_line.FilterChar('\n');
    info_line.FilterChar('\r');
    info_line.TrimLeadingSpaces();
    info_line.TrimTrailingSpaces();

    if(info_line.is_null()) continue;
    
    // Skip remark lines
    if(info_line[0] == '#') continue; 
    if(info_line[0] == ';') continue; 

    ProductTableData table_data;
    if(!ParseLine(info_line, table_data)) {
      mesg << clear << "Error in product table on line number " << line_number;
      NT_print(mesg.c_str());
      dfile.df_Close();
      return 0;
    }

    Product_table.Add(table_data);
  }

  dfile.df_Close();

  return 1;
}

int ParseLine(gxString &info_line, ProductTableData &table_data)
{
  char words[MAXWORDS][MAXWORDLENGTH];
  int numwords;
  table_data.Clear();

  if(parse(info_line.c_str(), words, &numwords, '|') == 1) {
    NT_printlines("Error parsing product table line", info_line.c_str());
    return 0;
  }

  if(numwords < 8) {
    NT_printlines("Error parsing product table line", info_line.c_str(), "Missing fields");
    return 0;
  }
  if(numwords > 8) {
    NT_printlines("Error parsing product table line", info_line.c_str(), "Extra fields");
    return 0;
  }

  if(*words[0] != 0) {
    table_data.pil = words[0];
    FilterStringField(table_data.pil);
  }
  if(*words[1] != 0) {
    table_data.vtec_pil = words[1];
    FilterStringField(table_data.vtec_pil);
  }
  if(*words[2] != 0) {
    table_data.replace_pil = words[2];
    FilterStringField(table_data.replace_pil);
  }
  if(*words[3] != 0) {
    table_data.office = words[3];
    FilterStringField(table_data.office);
  }
  if(*words[4] != 0) {
    table_data.priority = words[4];
    FilterStringField(table_data.priority);
  }
  if(*words[5] != 0) {
    table_data.fuse = words[5];
    FilterStringField(table_data.fuse);
  }
  if(*words[6] != 0) {
    table_data.sas = words[6];
    FilterStringField(table_data.sas);
  }
  if(*words[7] != 0) {
    table_data.name = words[7];
    FilterStringField(table_data.name);
  }

  if(table_data.pil.is_null()) {
    NT_printlines("Bad product table PIL", "Error parsing line", info_line.c_str());
    return 0;
  }
  if(table_data.vtec_pil.is_null()) {
    NT_printlines("Bad product table VTEC PIL", "Error parsing line", info_line.c_str());
    return 0;
  }
  if(table_data.replace_pil.is_null()) {
    NT_printlines("Bad product table ReplacePIL", "Error parsing line", info_line.c_str());
    return 0;
  }
  if(table_data.office.is_null()) {
    NT_printlines("Bad product table Office name", "Error parsing line", info_line.c_str());
    return 0;
  }
  if(table_data.priority.is_null()) {
    NT_printlines("Bad product table Priority code", "Error parsing line", info_line.c_str());
    return 0;
  }
  if(table_data.fuse.is_null()) {
    NT_printlines("Bad product table Fuse code", "Error parsing line", info_line.c_str());
    return 0;
  }
  if(table_data.sas.is_null()) {
    NT_printlines("Bad product table SAS code", "Error parsing line", info_line.c_str());
    return 0;
  }
  if(table_data.name.is_null()) {
    NT_printlines("Bad product table Name", "Error parsing line", info_line.c_str());
    return 0;
  }

  return 1;
}

int ParseLine(gxString &info_line, ZoneTableData &table_data)
{
  char words[MAXWORDS][MAXWORDLENGTH];
  int numwords;
  table_data.Clear();

  if(parse(info_line.c_str(), words, &numwords, '|') == 1) {
    NT_printlines("Error parsing line", info_line.c_str());
    return 0;
  }

  if(numwords < 3) {
    NT_printlines("Error parsing zone table line", info_line.c_str(), "Missing fields");
    return 0;
  }
  if(numwords > 5) {
    NT_printlines("Error parsing zone table line", info_line.c_str(), "Extra fields");
    return 0;
  }

  if(*words[0] != 0) {
    table_data.zone_number = words[0];
    FilterStringField(table_data.zone_number);
  }
  if(*words[1] != 0) {
    table_data.zone_name = words[1];
    FilterStringField(table_data.zone_name);
  }
  if(*words[2] != 0) {
    table_data.area_number = words[2];
    FilterStringField(table_data.area_number);
  }

  if(numwords == 3) return 1;

  if(*words[3] != 0) {
    table_data.zone_title = words[3];
    FilterStringField(table_data.zone_title);
  }

  if(numwords == 4) return 1;

  if(*words[4] != 0) {
    table_data.zone_url = words[4];
    FilterStringField(table_data.zone_url);
    encode_uri(table_data.zone_url);
  }

  if(table_data.zone_number.is_null()) {
    NT_printlines("Bad zone number", "Error parsing zone table line", info_line.c_str());
    return 0;
  }
  if(table_data.zone_name.is_null()) {
    NT_printlines("Bad zone name", "Error parsing zone table line", info_line.c_str());
    return 0;
  }
  if(table_data.area_number.is_null()) {
    NT_printlines("Bad area number", "Error parsing zone table line", info_line.c_str());
    return 0;
  }

  if(debug) {
    if(table_data.zone_title.is_null()) {
      NT_printlines("Warning parsing zone table zone title is not set", info_line.c_str());
    }
    if(table_data.zone_url.is_null()) {
      NT_printlines("Warning parsing zone table zone URL is not set", info_line.c_str());
    }
  }

  return 1;
}

int LoadZoneTable()
{
  char sbuf[1024];
  DiskFileB dfile;
  int line_number = 0;
  gxString mesg;
  Zone_table.ClearList();

  NT_print("Loading zone table file", zone_table_file.c_str());
 
  // Open the current product file
  dfile.df_Open(zone_table_file.c_str());
  if(!dfile) {
    NT_print("Cannot open zone table file", zone_table_file.c_str());
    NT_print(dfile.df_ExceptionMessage());
    return 0;
  }

  while(!dfile.df_EOF()) {
    // Get each line of the file and trim the line feeds
    dfile.df_GetLine(sbuf, sizeof(sbuf), '\n');
    if(dfile.df_GetError() != DiskFileB::df_NO_ERROR) {
      NT_print("A fatal I/O error occurred while reading zone table");
      NT_print("Cannot read file", zone_table_file.c_str());
      NT_print(dfile.df_ExceptionMessage());
      dfile.df_Close();
      return 0;
    }
    line_number++;

    UString info_line(sbuf);
    info_line.FilterChar('\n');
    info_line.FilterChar('\r');
    info_line.TrimLeadingSpaces();
    info_line.TrimTrailingSpaces();

    if(info_line.is_null()) continue;
    
    // Skip remark lines
    if(info_line[0] == '#') continue; 
    if(info_line[0] == ';') continue; 

    ZoneTableData table_data;
    if(!ParseLine(info_line, table_data)) {
      mesg << clear << "Error in zone table on line number " << line_number;
      NT_print(mesg.c_str());
      dfile.df_Close();
      return 0;
    }
    if((debug) && (debug_level >= 2)) table_data.zone_name << " - " << table_data.zone_number;     

    Zone_table.Add(table_data);
  }

  dfile.df_Close();


  gxListNode<ZoneTableData> *zptr = Zone_table.GetHead();
  if(Area_table.IsEmpty()) {
    while(zptr) {
      zptr->data.area_number = "0";
      zptr->data.area_name << clear << WarningTableCWA << " Zones and Counties";
      zptr = zptr->next;
    }
    return 1;
  }

  while(zptr) {
    gxString name;
    if(FindAreaName(zptr->data.area_number, name)) {
      zptr->data.area_name = name;
    }
    else {
      mesg << clear << "Zone " << zptr->data.area_number << " not found in area table";
      NT_print(mesg.c_str());
      NT_print("Assigning default value");
      zptr->data.area_number = "0";
      zptr->data.area_name << clear << WarningTableCWA << " Zones and Counties";
    }
    zptr = zptr->next;
  }
  
  return 1;
}

int FindProductTableProduct(const gxString &product, const gxString &vtec_pil, ProductTableData &table_data)
{
  table_data.Clear();

  gxListNode<ProductTableData> *ptptr = Product_table.GetHead();
  while(ptptr) {
    if((ptptr->data.pil == product) && (ptptr->data.vtec_pil == vtec_pil)) {
      table_data = ptptr->data; 
      return 1;
    }
    ptptr = ptptr->next;
  }

  return 0;
}

int FindProductTablePil(const gxString &product, ProductTableData &table_data)
{
  table_data.Clear();

  gxListNode<ProductTableData> *ptptr = Product_table.GetHead();
  while(ptptr) {
    if(ptptr->data.pil == product) {
      table_data = ptptr->data; 
      return 1;
    }
    ptptr = ptptr->next;
  }
  return 0;
}

int FindProductTableVTECPil(const gxString &vtec_pil, ProductTableData &table_data)
{
  table_data.Clear();

  gxListNode<ProductTableData> *ptptr = Product_table.GetHead();
  while(ptptr) {
    if(ptptr->data.vtec_pil == vtec_pil) {
      table_data = ptptr->data;
      return 1;
    }
    ptptr = ptptr->next;
  }
  return 0;
}

int FindProductListFile(const gxString &product_name)
{
  gxListNode<gxString> *lptr = product_list.GetHead();
  while(lptr) {
    if(lptr->data == product_name) {
      return 1;
    }
    lptr = lptr->next;
  }
  
  return 0;
}

int FindAreaNumber(const gxString &area_number)
{
  gxListNode<AreaTableData> *aptr = Area_table.GetHead();
  while(aptr) {
    if(aptr->data.area_number == area_number) {
      return 1;
    }
    aptr = aptr->next;
  }
  return 0;
}

int FindAreaName(const gxString &area_number, gxString &name) 
{
  name.Clear();

  gxListNode<AreaTableData> *aptr = Area_table.GetHead();
  while(aptr) {
    if(aptr->data.area_number == area_number) {
      name = aptr->data.area_name;
      return 1;
    }
    aptr = aptr->next;
  }

  return 0;
}

int FindPIL(const gxString &PIL, gxString &pbuf)
{
  gxString pil3 = PIL;
  gxString office = PIL;

  if(PIL.length() == 6) {
    pil3.DeleteAt(3, 3);
    office.DeleteAt(0, 3);
  }
  else if(PIL.length() == 9) {
    pil3.DeleteAt(0, 3);
    pil3.DeleteAt(3, 3);
    office.DeleteAt(0, 6);
  }
  else {
    if(debug) NT_print("Error finding PIL", PIL.c_str());
    return 0;
  }

  pbuf << clear << pil3 << office;

  gxListNode<ProductTableData> *ptptr = Product_table.GetHead();
  while(ptptr) {
    if((ptptr->data.pil == pil3) && (ptptr->data.office == office)) {
      return 1;
    }
    ptptr = ptptr->next;
  }
  
  return 0;
}

int UpdateZoneName(const gxString &zone_number, ZoneTableData &zone_table_data)
{
  gxListNode<ZoneTableData> *zptr = Zone_table.GetHead();
  while(zptr) {
    if(zptr->data.zone_number == zone_number) {
      zptr->data.zone_number = zone_table_data.zone_number;
      zptr->data.zone_name = zone_table_data.zone_name;
      zptr->data.zone_title = zone_table_data.zone_title;
      zptr->data.zone_url = zone_table_data.zone_url;
      zptr->data.area_number = zone_table_data.area_number;
      zptr->data.area_name = zone_table_data.area_name;
      return 1;
    }
    zptr = zptr->next;
  }
  return 0;
}

int FindZoneName(const gxString &zone_number, gxString &name)
{
  name.Clear();

  gxListNode<ZoneTableData> *zptr = Zone_table.GetHead();
  while(zptr) {
    if(zptr->data.zone_number == zone_number) {
      name = zptr->data.zone_name;
      return 1;
    }
    zptr = zptr->next;
  }
  return 0;
}

int FindZoneByName(const gxString &zone_name)
{
  gxListNode<ZoneTableData> *zptr = Zone_table.GetHead();
  while(zptr) {
    if(zptr->data.zone_name == zone_name) {
      return 1;
    }

    zptr = zptr->next;
  }
  return 0;
}

int FindZoneByName(const gxString &zone_name, ZoneTableData &table_data)
{
  table_data.Clear();

  gxListNode<ZoneTableData> *zptr = Zone_table.GetHead();
  while(zptr) {
    if(zptr->data.zone_name == zone_name) {
      table_data = zptr->data; 
      return 1;
    }

    zptr = zptr->next;
  }
  return 0;
}

int FindZoneNumber(const gxString &zone_number)
{
  gxListNode<ZoneTableData> *zptr = Zone_table.GetHead();
  while(zptr) {
    if(zptr->data.zone_number == zone_number) {
      return 1;
    }

    zptr = zptr->next;
  }
  return 0;
}

int FindZoneNumber(const gxString &zone_number, ZoneTableData &table_data)
{
  table_data.Clear();

  gxListNode<ZoneTableData> *zptr = Zone_table.GetHead();
  while(zptr) {
    if(zptr->data.zone_number == zone_number) {
      table_data = zptr->data;
      return 1;
    }
    zptr = zptr->next;
  }

  return 0;
}

int FindInVTECTable(const gxString &phenomena, const gxString &Significance, gxString &title)
{

  gxListNode<gxString> *vtablep;
  gxString sbuf;
  title.Clear();
  gxString vtec_message;

  // Parse pp table
  vtablep = PVTEC_phenomena_table.GetHead();
  while(vtablep) {
    sbuf = vtablep->data;
    gxString pp = sbuf.Left(2);
    if(pp == phenomena) {
      sbuf.DeleteBeforeIncluding(pp);
      sbuf.TrimLeadingSpaces();
      sbuf.TrimTrailingSpaces();
      vtec_message << clear << sbuf;
    }
    vtablep = vtablep->next;
  }
  // End parse

  vtablep = PVTEC_significance_table.GetHead();
  while(vtablep) {
    sbuf = vtablep->data;
    gxString s = sbuf.Left(1);
    if(s == Significance) {
      sbuf.DeleteBeforeIncluding(s);
      sbuf.TrimLeadingSpaces();
      sbuf.TrimTrailingSpaces();
      vtec_message << " " << sbuf;
      title = vtec_message;
      return 1;
    }
    vtablep = vtablep->next;
  }
  
  return 0;
}

int DumpTriggerTable(const gxString &fname, const gxString &MYCWA)
{
  NT_print("Creating product trigger listing for", MYCWA.c_str(), "warning table");

  // Build the download list
  gxBStree<gxString> trigger_list;
  gxListNode<ProductTableData> *ptptr = Product_table.GetHead();
  gxString product;

  while(ptptr) {
    product << clear << ptptr->data.pil <<  ptptr->data.office;
    trigger_list.Insert(product);
    ptptr = ptptr->next;
  }

  DiskFileB dfile(fname.c_str(), DiskFileB::df_READWRITE,
		  DiskFileB::df_CREATE); 
  if(!dfile) {
    NT_print("Cannot create product trigger listing", fname.c_str());
    return 0;
  }


  // Download the products
  gxBStreeNode<gxString> *node = trigger_list.GetRoot();
  gxBStreeIterator tree_iterator(node);
  dfile << "\n";
  while((node = (gxBStreeNode<gxString> *)tree_iterator.GetNext()) != 0) {
    dfile << node->data.c_str() << "\n";
  }
  dfile << "\n";

  dfile.df_Close();
  NT_print("Trigger listing build for", MYCWA.c_str(), "complete");  
  return 1;
}

int DumpDynamicZoneTable(const gxString &fname, const gxString &MYCWA)
{
  NT_print("Creating dynamic zone table for", MYCWA.c_str());

  DiskFileB dfile(fname.c_str(), DiskFileB::df_READWRITE,
		  DiskFileB::df_CREATE); 
  if(!dfile) {
    NT_print("Cannot create warning zone table", fname.c_str());
    return 0;
  }

  dfile << "#" << "\n";
  dfile << "# zone_table.txt" << "\n";
  dfile << "#" << "\n";
  dfile << "\n";
  dfile << "#" << "\n";
  dfile << "# Line syntax:" << "\n";
  dfile << "#" << "\n";
  dfile << "# ZONE | Name | Area Number" << "\n";
  dfile << "# Example: FLZ073 | Inland Miami | 100" << "\n";
  dfile << "#" << "\n";
  dfile << "# Field descriptions:" << "\n";
  dfile << "#" << "\n";
  dfile << "# ZONE = Zone number or county code in AAA000 format" << "\n";
  dfile << "# Name = name for this zone or county as it will appear in the warning table" << "\n";
  dfile << "# Area number = user-defined number used to bind the zone to a custom area" << "\n";
  dfile << "#" << "\n";
  dfile << "# NOTE: The area number is mapped to a name in the CWA-area_table.txt file" << "\n";
  dfile << "\n";
  dfile << "# Optional Line syntax:" << "\n";
  dfile << "# ZONE | Name | Area Number | Title | URL" << "\n";
  dfile << "# Example: FLZ073 | Inland Miami | 100 | Inland Miami | http://forecast.weather.gov/showsigwx.php?warncounty=FLC086&warnzone=FLZ073&local_place1=Inland%20Miami" 
	<< "\n";
  dfile << "#" << "\n";
  dfile << "# Optional field descriptions:" << "\n";
  dfile << "#" << "\n";
  dfile << "# Title = Title for this zone and/or county URL" << "\n";
  dfile << "# URL = URL for zone and/or county Web page" << "\n";
  dfile << "\n";
  dfile << "# NOTE: For blank table data separators use the keyword BLANK with a dummy zone number" << "\n";
  dfile << "# Example: BBB100 | BLANK-100 | 100" << "\n";
  dfile << "\n";
  dfile << "# Zones and counties for " << MYCWA.c_str() << "\n";
  dfile << "# Add your table entries here" << "\n";
  dfile << "\n";

  gxListNode<ZoneTableData> *zptr = Zone_table.GetHead();
  while(zptr) {
    dfile << zptr->data.zone_number.c_str() 
	  << " | " 
	  << zptr->data.zone_name.c_str() 
	  << " | " 
	  << zptr->data.area_number.c_str() << "\n";  
    zptr = zptr->next;
  }

  dfile << "\n";

  dfile.df_Close();
  NT_print("Dynamic zone table build for", MYCWA.c_str(), "complete");  
  return 1;
}

int BuildDefaultAreaTable(const gxString &fname, const gxString &MYCWA)
{
  NT_print("Creating default warning area table for", MYCWA.c_str());

  DiskFileB dfile(fname.c_str(), DiskFileB::df_READWRITE,
		  DiskFileB::df_CREATE); 
  if(!dfile) {
    NT_print("Cannot create warning area table", fname.c_str());
    return 0;
  }

  dfile << "#" << "\n";
  dfile << "# area_table.txt" << "\n";
  dfile << "#" << "\n";
  dfile << "# Area table used with the zone_table.txt file to map a zone number or" << "\n";
  dfile << "# FIPS code to a geographic area number. " << "\n";
  dfile << "#" << "\n";
  dfile << "# Line syntax:" << "\n";
  dfile << "#" << "\n";
  dfile << "# AREA NUMBER | AREA NAME " << "\n";
  dfile << "# Example: 100 | Miami-Dade" << "\n";
  dfile << "#" << "\n";
  dfile << "# Field descriptions:" << "\n";
  dfile << "#" << "\n";
  dfile << "# Area Number = User-defined number used to bind the zone to the area " << "\n";
  dfile << "# Area Name = Name used to describe the area" << "\n";
  dfile << "\n";
  dfile << "# NOTE: For blank table header separators use the keyword BLANK with the next area number" << "\n";
  dfile << "# Example: 900 | BLANK" << "\n";
  dfile << "\n";
  dfile << "# Start of table for " << MYCWA.c_str() << "\n";
  dfile << "# Add your custom table entries here" << "\n";
  dfile << "\n";
  dfile << "0 | " << WarningTableCWA.c_str() << " Zones and Counties" << "\n";
  dfile << "\n";

  dfile.df_Close();
  NT_print("Default warning area table build for", MYCWA.c_str(), "complete");  
  return 1;
}

int BuildDefaultZoneTable(const gxString &fname, const gxString &MYCWA)
{
  NT_print("Creating default warning zone table for", MYCWA.c_str());

  DiskFileB dfile(fname.c_str(), DiskFileB::df_READWRITE,
		  DiskFileB::df_CREATE); 
  if(!dfile) {
    NT_print("Cannot create warning zone table", fname.c_str());
    return 0;
  }

  dfile << "#" << "\n";
  dfile << "# zone_table.txt" << "\n";
  dfile << "#" << "\n";
  dfile << "\n";
  dfile << "#" << "\n";
  dfile << "# Line syntax:" << "\n";
  dfile << "#" << "\n";
  dfile << "# ZONE | Name | Area Number" << "\n";
  dfile << "# Example: FLZ073 | Inland Miami | 100" << "\n";
  dfile << "#" << "\n";
  dfile << "# Field descriptions:" << "\n";
  dfile << "#" << "\n";
  dfile << "# ZONE = Zone number or county code in AAA000 format" << "\n";
  dfile << "# Name = name for this zone or county as it will appear in the warning table" << "\n";
  dfile << "# Area number = user-defined number used to bind the zone to a custom area" << "\n";
  dfile << "#" << "\n";
  dfile << "# NOTE: The area number is mapped to a name in the CWA-area_table.txt file" << "\n";
  dfile << "\n";
  dfile << "# Optional Line syntax:" << "\n";
  dfile << "# ZONE | Name | Area Number | Title | URL" << "\n";
  dfile << "# Example: FLZ073 | Inland Miami | 100 | Inland Miami | http://forecast.weather.gov/showsigwx.php?warncounty=FLC086&warnzone=FLZ073&local_place1=Inland%20Miami" 
	<< "\n";
  dfile << "#" << "\n";
  dfile << "# Optional field descriptions:" << "\n";
  dfile << "#" << "\n";
  dfile << "# Title = Title for this zone and/or county URL" << "\n";
  dfile << "# URL = URL for zone and/or county Web page" << "\n";
  dfile << "\n";
  dfile << "# NOTE: For blank table data separators use the keyword BLANK with a dummy zone number" << "\n";
  dfile << "# Example: BBB100 | BLANK-100 | 100" << "\n";
  dfile << "\n";
  dfile << "# Zones and counties for " << MYCWA.c_str() << "\n";
  dfile << "# Add your table entries here" << "\n";
  dfile << "\n";

  dfile.df_Close();
  NT_print("Default warning zone table build for", MYCWA.c_str(), "complete");  
  return 1;
}

int BuildDefaultProductTable(const gxString &fname, const gxString &MYCWA)
{
  NT_print("Creating default warning product table for", MYCWA.c_str());

  DiskFileB dfile(fname.c_str(), DiskFileB::df_READWRITE,
		  DiskFileB::df_CREATE); 
  if(!dfile) {
    NT_print("Cannot create warning product table", fname.c_str());
    return 0;
  }

  dfile << "# product_table.txt" << "\n";
  dfile << "# configuration file used to generate the Web based warning table" << "\n";
  dfile << "# " << "\n";
  dfile << "# Line Syntax:" << "\n";
  dfile << "# PIL | VTEC PIL | ReplacePIL | Office | Priority | Fuse | SAS | Name " << "\n";
  dfile << "#" << "\n";
  dfile << "# Priority: 1=red 2=yellow 3=green" << "\n";
  dfile << "# Fuse:     0=short 1=long" << "\n";
  dfile << "# SAS:      0=no 1=yes" << "\n";
  dfile << "#" << "\n";
  dfile << "# SAS - Situational Awareness Display Format " << "\n";
  dfile << "\n";
  dfile << "# Start of table" << "\n";
  dfile << "#" << "\n";
  dfile << "# Priority 1" << "\n";
  dfile << "#" << "\n";
  dfile << "TOR | TO.W | TOR | " << MYCWA.c_str() << " | 1 | 0 | 1 | Tornado warning" << "\n";
  dfile << "SVS | TO.W | TOR | " << MYCWA.c_str() << " | 1 | 0 | 1 | Tornado warning" << "\n";
  dfile << "SVR | SV.W | SVR | " << MYCWA.c_str() << " | 1 | 0 | 1 | Severe thunderstorm warning" << "\n";
  dfile << "SVS | SV.W | SVR | " << MYCWA.c_str() << " | 1 | 0 | 1 | Severe thunderstorm warning" << "\n";
  dfile << "FFW | FF.W | FFW | " << MYCWA.c_str() << " | 1 | 0 | 1 | Flood warning" << "\n";
  dfile << "FFS | FF.W | FFW | " << MYCWA.c_str() << " | 1 | 0 | 1 | Flood warning" << "\n";
  dfile << "SMW | MA.W | SMW | " << MYCWA.c_str() << " | 1 | 0 | 1 | Special marine warning" << "\n";
  dfile << "MWS | MA.W | SMW | " << MYCWA.c_str() << " | 1 | 0 | 1 | Special marine warning" << "\n";
  dfile << "MWW | HU.W | HUW | " << MYCWA.c_str() << " | 1 | 0 | 1 | Hurricane warning" << "\n";
  dfile << "MWW | TR.W | TRW | " << MYCWA.c_str() << " | 1 | 0 | 1 | Tropical storm warning" << "\n";
  dfile << "NPW | HI.W | HIW | " << MYCWA.c_str() << " | 1 | 0 | 1 | Inland hurricane warning" << "\n";
  dfile << "NPW | TI.W | TIW | " << MYCWA.c_str() << " | 1 | 0 | 1 | Inland tropical storm warning" << "\n";
  dfile << "FLW | FL.W | FLW | " << MYCWA.c_str() << " | 1 | 0 | 1 | River flood warning" << "\n";
  dfile << "FLW | AF.W | AFW | " << MYCWA.c_str() << " | 1 | 0 | 1 | Areal flood warning" << "\n";
  dfile << "NPW | HW.W | HWW | " << MYCWA.c_str() << " | 1 | 0 | 1 | High wind warning" << "\n";
  dfile << "NPW | EH.W | EHW | " << MYCWA.c_str() << " | 1 | 0 | 1 | Excessive heat warning" << "\n";
  dfile << "NPW | FZ.W | FZW | " << MYCWA.c_str() << " | 1 | 0 | 1 | Freeze warning" << "\n";
  dfile << "NPW | HZ.W | HZW | " << MYCWA.c_str() << " | 1 | 0 | 1 | Hard freeze warning" << "\n";
  dfile << "WSW | WC.W | WCW | " << MYCWA.c_str() << " | 1 | 0 | 1 | Wind chill warning" << "\n";
  dfile << "WSW | WS.W | WSW | " << MYCWA.c_str() << " | 1 | 0 | 1 | Winter storm warning" << "\n";
  dfile << "WSW | HS.W | HSW | " << MYCWA.c_str() << " | 1 | 0 | 1 | Heavy snow warning" << "\n";
  dfile << "WSW | IS.W | ISW | " << MYCWA.c_str() << " | 1 | 0 | 1 | Ice storm warning" << "\n";
  dfile << "WSW | IP.W | IPW | " << MYCWA.c_str() << " | 1 | 0 | 1 | Sleet warning" << "\n";
  dfile << "WSW | BZ.W | BZW | " << MYCWA.c_str() << " | 1 | 0 | 1 | Blizzard warning" << "\n";
  dfile << "CFW | CF.W | CFW | " << MYCWA.c_str() << " | 1 | 0 | 1 | Coastal flood warning" << "\n";
  dfile << "RFW | FW.W | FWW | " << MYCWA.c_str() << " | 1 | 0 | 1 | Red flag warning" << "\n";
  dfile << "MWW | GL.W | GLW | " << MYCWA.c_str() << " | 1 | 1 | 1 | Gale warning" << "\n";
  dfile << "MWW | SR.W | SRW | " << MYCWA.c_str() << " | 1 | 1 | 1 | Storm warning" << "\n";
  dfile << "CEM | CE.M | CEM | " << MYCWA.c_str() << " | 1 | 0 | 1 | Civil emergency message" << "\n";
  dfile << "HLS | HU.W | HUW | " << MYCWA.c_str() << " | 1 | 0 | 1 | Hurricane warning" << "\n";
  dfile << "HLS | TR.W | TRW | " << MYCWA.c_str() << " | 1 | 0 | 1 | Tropical storm warning" << "\n";
  dfile << "HLS | HI.W | HIW | " << MYCWA.c_str() << " | 1 | 0 | 1 | Inland hurricane warning" << "\n";
  dfile << "HLS | TI.W | TIW | " << MYCWA.c_str() << " | 1 | 0 | 1 | Inland tropical storm warning" << "\n";
  dfile << "FLW | FA.W | FAW | " << MYCWA.c_str() << " | 1 | 0 | 1 | Flood Warning" << "\n";
  dfile << "\n";
  dfile << "#" << "\n";
  dfile << "# Priority 2" << "\n";
  dfile << "#" << "\n";
  dfile << "MWW | HU.A | HUA | " << MYCWA.c_str() << " | 2 | 1 | 1 | Hurricane watch" << "\n";
  dfile << "MWW | TR.A | TRA | " << MYCWA.c_str() << " | 2 | 1 | 1 | Tropical storm watch" << "\n";
  dfile << "NPW | HI.A | HIA | " << MYCWA.c_str() << " | 2 | 1 | 1 | Inland hurricane watch" << "\n";
  dfile << "NPW | TI.A | TIA | " << MYCWA.c_str() << " | 2 | 1 | 1 | Inland tropical storm watch" << "\n";
  dfile << "FFA | FA.A | FFA | " << MYCWA.c_str() << " | 2 | 1 | 1 | Flood watch" << "\n"; 
  dfile << "SPS | SW.A | SWA | " << MYCWA.c_str() << " | 2 | 0 | 1 | Significant wx alert" << "\n";
  dfile << "NPW | FG.Y | FGY | " << MYCWA.c_str() << " | 2 | 0 | 1 | Dense fog advisory" << "\n";
  dfile << "NPW | SM.Y | SMY | " << MYCWA.c_str() << " | 2 | 0 | 1 | Dense smoke advisory" << "\n";
  dfile << "NPW | HW.A | HWA | " << MYCWA.c_str() << " | 2 | 1 | 1 | High wind watch" << "\n";
  dfile << "NPW | EH.A | EHA | " << MYCWA.c_str() << " | 2 | 1 | 1 | Excessive heat watch" << "\n";
  dfile << "NPW | FZ.A | FZA | " << MYCWA.c_str() << " | 2 | 1 | 1 | Freeze watch" << "\n";
  dfile << "NPW | HZ.A | HZA | " << MYCWA.c_str() << " | 2 | 1 | 1 | Hard freeze watch" << "\n";
  dfile << "NPW | LW.Y | LWY | " << MYCWA.c_str() << " | 2 | 1 | 1 | Lake wind advisory" << "\n";
  dfile << "NPW | WI.Y | WIY | " << MYCWA.c_str() << " | 2 | 1 | 1 | Wind advisory" << "\n";
  dfile << "WSW | WC.A | WCA | " << MYCWA.c_str() << " | 2 | 1 | 1 | Wind chill watch" << "\n";
  dfile << "WSW | WS.A | WSA | " << MYCWA.c_str() << " | 2 | 1 | 1 | Winter storm watch" << "\n";
  dfile << "WSW | BZ.A | BZA | " << MYCWA.c_str() << " | 2 | 1 | 1 | Blizzard watch" << "\n";
  dfile << "CFW | CF.A | CFA | " << MYCWA.c_str() << " | 2 | 1 | 1 | Coastal flood watch" << "\n";
  dfile << "CFW | SU.A | SUA | " << MYCWA.c_str() << " | 2 | 1 | 1 | High surf watch" << "\n";
  dfile << "CFW | SU.Y | SUY | " << MYCWA.c_str() << " | 2 | 1 | 1 | High surf advisory" << "\n";
  dfile << "WCN | TO.A | TOA | " << MYCWA.c_str() << " | 2 | 1 | 1 | Tornado watch" << "\n";
  dfile << "WCN | SV.A | SVA | " << MYCWA.c_str() << " | 2 | 1 | 1 | Severe tstm watch" << "\n";
  dfile << "RFW | FW.A | FWA | " << MYCWA.c_str() << " | 2 | 1 | 1 | Fire wx watch" << "\n";
  dfile << "MWW | SC.Y | SCY | " << MYCWA.c_str() << " | 2 | 1 | 1 | Small craft advisory" << "\n";
  dfile << "MWW | SW.Y | SWY | " << MYCWA.c_str() << " | 2 | 1 | 1 | Small craft advisory for seas" << "\n";
  dfile << "HLS | HU.A | HUA | " << MYCWA.c_str() << " | 2 | 1 | 1 | Hurricane watch" << "\n";
  dfile << "HLS | TR.A | TRA | " << MYCWA.c_str() << " | 2 | 1 | 1 | Tropical storm watch" << "\n";
  dfile << "HLS | HI.A | HIA | " << MYCWA.c_str() << " | 2 | 1 | 1 | Inland hurricane watch" << "\n";
  dfile << "HLS | TI.A | TIA | " << MYCWA.c_str() << " | 2 | 1 | 1 | Inland tropical storm watch" << "\n";
  dfile << "\n";
  dfile << "#" << "\n";
  dfile << "# Priority 3" << "\n";
  dfile << "#" << "\n";
  dfile << "HLS | HL.S | HLS | " << MYCWA.c_str() << " | 3 | 1 | 1 | Hurricane local statement" << "\n";
  dfile << "SVS | SV.S | SVS | " << MYCWA.c_str() << " | 3 | 0 | 1 | Severe weather statement" << "\n";
  dfile << "MWS | MW.S | MWS | " << MYCWA.c_str() << " | 3 | 0 | 1 | Marine weather statement" << "\n";
  dfile << "MWS | MA.S | MWS | " << MYCWA.c_str() << " | 3 | 0 | 1 | Marine weather statement" << "\n";
  dfile << "FFS | FF.S | FFS | " << MYCWA.c_str() << " | 3 | 1 | 1 | Flood statement" << "\n";
  dfile << "SPS | SP.S | SPS | " << MYCWA.c_str() << " | 3 | 0 | 1 | Special wx statement" << "\n";
  dfile << "FLS | FA.Y | FAY | " << MYCWA.c_str() << " | 3 | 0 | 1 | Urban flood statement" << "\n";
  dfile << "FLS | UR.B | URB | " << MYCWA.c_str() << " | 3 | 0 | 1 | Urban flood advisory" << "\n";
  dfile << "FLS | FL.S | FLS | " << MYCWA.c_str() << " | 3 | 1 | 1 | River flood statement" << "\n";
  dfile << "RVS | RV.S | RVS | " << MYCWA.c_str() << " | 3 | 1 | 1 | River statement" << "\n";
  dfile << "NPW | HT.Y | HTY | " << MYCWA.c_str() << " | 3 | 1 | 1 | Heat advisory" << "\n";
  dfile << "WSW | WC.Y | WCY | " << MYCWA.c_str() << " | 3 | 1 | 1 | Wind chill advisory" << "\n";
  dfile << "WSW | WW.Y | WWY | " << MYCWA.c_str() << " | 3 | 1 | 1 | Winter wx advisory" << "\n";
  dfile << "WSW | SN.Y | SNY | " << MYCWA.c_str() << " | 3 | 1 | 1 | Snow advisory" << "\n";
  dfile << "WSW | ZR.Y | ZRY | " << MYCWA.c_str() << " | 3 | 1 | 1 | Freezing rain advisory" << "\n";
  dfile << "WSW | BZ.Y | BZY | " << MYCWA.c_str() << " | 3 | 1 | 1 | Blizzard advisory" << "\n";
  dfile << "CFW | CF.S | CFS | " << MYCWA.c_str() << " | 3 | 1 | 1 | Coastal hazard message" << "\n";
  dfile << "NOW | NO.W | NOW | " << MYCWA.c_str() << " | 3 | 0 | 1 | Short term forecast" << "\n";
  dfile << "HWO | HW.O | HWO | " << MYCWA.c_str() << " | 3 | 0 | 1 | Hazarous weather outlook" << "\n";
  dfile << "PNS | PN.S | PNS | " << MYCWA.c_str() << " | 3 | 1 | 1 | Public info statement" << "\n";
  dfile << "HLS | HU.S | HLS | " << MYCWA.c_str() << " | 3 | 1 | 1 | Hurricane statement" << "\n";
  dfile << "# CWF is no longer a VTEC product" << "\n";
  dfile << "# CWF | CW.F | CWF | " << MYCWA.c_str() << " | 3 | 1 | 1 | Coastal waters forecast" << "\n";
  dfile << "\n";

  dfile.df_Close();
  NT_print("Default warning product table build for", MYCWA.c_str(), "complete");  
  return 1;
}

int DumpDynamicZoneJS(const gxString &fname, const gxString &MYCWA)
{
  NT_print("Creating dynamic zone JavaScript file for", MYCWA.c_str());

  DiskFileB dfile(fname.c_str(), DiskFileB::df_READWRITE,
		  DiskFileB::df_CREATE); 
  if(!dfile) {
    NT_print("Cannot create dynamic zone JavaScript file ", fname.c_str());
    return 0;
  }

  dfile << "// ------------------------------- //" << "\n";
  dfile << "// -------- Start of File -------- //" << "\n";
  dfile << "// ------------------------------- //" << "\n";
  dfile << "// ----------------------------------------------------------- // " << "\n";
  dfile << "// JavaScript File" << "\n";
  dfile << "// Browser support: Internet Explorer, Netscape, Firefox, Chrome " << "\n";
  dfile << "// Produced By: " << ProgramDescription << " version " << VersionString << "\n";
  SysTime systime;
  gxString year = systime.GetStrTime(SysTime::Year);
  gxString month = systime.GetStrTime(SysTime::Month);
  gxString day = systime.GetStrTime(SysTime::DayOfTheMonth);
  dfile << "// File Creation " << "Date: " << month.c_str() << "/" << day.c_str() << "/" << year.c_str() << "\n";
  dfile << "// Date Last " << "Modified: " << month.c_str() << "/" << day.c_str() << "/" << year.c_str() << "\n";
  dfile << "//" << "\n";
  dfile << "// Version control: " << VersionString << "\n";
  dfile << "//" << "\n";
  dfile << "// Support Team:" << "\n";
  dfile << "//" << "\n";
  dfile << "// Contributors:" << "\n";
  dfile << "// ----------------------------------------------------------- // " << "\n";
  dfile << "// ------------- Program Description and Details ------------- // " << "\n";
  dfile << "// ----------------------------------------------------------- // " << "\n";
  dfile << "//" << "\n";
  dfile << "// Zone function calls for decoded segmented VTEC products." << "\n";
  dfile << "// The caller must provide a CaptionOn() function that displays" << "\n";
  dfile << "// the text segment for the matching zone" << "\n";
  dfile << "//" << "\n";
  dfile << "// AWIPS SITE: " << MYCWA.c_str() << "\n";
  dfile << "// ----------------------------------------------------------- // " << "\n";

  gxListNode<ZoneTableData> *zptr = Zone_table.GetHead();
  while(zptr) {

    dfile << "\n";
    dfile << "function " << zptr->data.zone_number.c_str() << "caption(evt, title) {" << "\n";
    dfile << "  CaptionOn(evt, 1, title, \'" << zptr->data.zone_number.c_str() << "\', \'" << zptr->data.zone_name.c_str() << "\');" << "\n";
    dfile << "}" << "\n";
    zptr = zptr->next;
  }

  dfile << "\n";
  dfile << "// ----------------------------------------------------------- //" << "\n";
  dfile << "// ------------------------------- //" << "\n";
  dfile << "// --------- End of File --------- //" << "\n";
  dfile << "// ------------------------------- //" << "\n";

  dfile.df_Close();
  NT_print("Dynamic zone table JavaScript file for", MYCWA.c_str(), "complete");  
  return 1;
}
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
