// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- // 
// C++ Source Code File
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

Code used to generate Watch/Warning table Web pages.
*/
// ----------------------------------------------------------- // 
#include "gxdlcode.h"

IconLink &IconLink::operator=(const IconLink &ob)
{
  if(&ob != this) Copy(ob);
  return *this;
}

void IconLink::Copy(const IconLink &ob)
{
  link = ob.link;
  priority = ob.priority;
  title = ob.title;
}

void IconLink::Clear()
{
  link.Clear();
  priority = 0;
  title.Clear();
}

CurrentWarningZoneNode &CurrentWarningZoneNode::operator=(const CurrentWarningZoneNode &ob)
{
  if(&ob != this) Copy(ob);
  return *this;
}

void CurrentWarningZoneNode::Copy(const CurrentWarningZoneNode &ob)
{
  Clear();
  name = ob.name;
  table_data = ob.table_data;

  gxListNode<IconLink> *sptr;
  sptr = ob.product_linked_icons.GetHead();
  while(sptr) {
    product_linked_icons.Add(sptr->data);
    sptr = sptr->next;
  }
}

void CurrentWarningZoneNode::Clear()
{
  name.Clear();
  table_data.Clear();
  product_linked_icons.ClearList();
}

UGCSegmentList &UGCSegmentList::operator=(const UGCSegmentList &ob)
{
  if(&ob != this) Copy(ob);
  return *this;
}

void UGCSegmentList::Clear()
{
  ugc_segment_nodes.ClearList();
  ugc_segment_UPG_remove_list.ClearList();
  ugc_segment_OLD_remove_list.ClearList();
  current_warnings_by_zone.ClearList();  
}

void UGCSegmentList::Copy(const UGCSegmentList&ob)
{
  Clear();

  gxListNode<UGCSegmentListNode> *usnptr;
  usnptr = ob.ugc_segment_nodes.GetHead();
  while(usnptr) {
    ugc_segment_nodes.Add(usnptr->data);
    usnptr = usnptr->next;
  }

  gxListNode<CurrentWarningZoneNode> *zone_node;
  zone_node = ob.current_warnings_by_zone.GetHead();
  while(zone_node) {
    current_warnings_by_zone.Add(zone_node->data);
    zone_node = zone_node->next;
  }

  gxListNode<UGCSegmentListNode> *rlptr = ob.ugc_segment_UPG_remove_list.GetHead();
  while(rlptr) {
    ugc_segment_UPG_remove_list.Add(rlptr->data);
    rlptr = rlptr->next;
  }

  rlptr = ob.ugc_segment_OLD_remove_list.GetHead();
  while(rlptr) {
    ugc_segment_OLD_remove_list.Add(rlptr->data);
    rlptr = rlptr->next;
  }
}

UGCSegmentListNode &UGCSegmentListNode::operator=(const UGCSegmentListNode &ob) 
{
  if(&ob != this) Copy(ob);
  return *this;
}

void UGCSegmentListNode::Clear() 
{
  pil.Clear();
  full_pil.Clear();
  wmo_header.Clear();
  etn = "0000";
  name.Clear();
  issue_office.Clear();
  issue_time.Clear();
  begin_time = (time_t)0;
  end_time = (time_t)0;
  is_expired = 0;
  text.ClearList();
  zones.ClearList();
  table_data.Clear();
  jscode.Clear();
  icon_link.Clear();
  ugc_segment_header.Clear();
  vtec.Clear();
  segment_number = 0;
}

void UGCSegmentListNode::Copy(const UGCSegmentListNode &ob) 
{
  Clear();
  pil = ob.pil;
  full_pil = ob.full_pil;
  wmo_header = ob.wmo_header;
  etn = ob.etn;
  name = ob.name;
  issue_office = ob.issue_office;
  issue_time = ob.issue_time;
  begin_time = ob.begin_time;
  end_time = ob.end_time;
  is_expired = ob.is_expired;
  table_data = ob.table_data;
  jscode = ob.jscode;
  icon_link = ob.icon_link;
  ugc_segment_header = ob.ugc_segment_header;
  vtec = ob.vtec;
  segment_number = ob.segment_number;

  gxListNode<gxString> *sptr;
  sptr = ob.text.GetHead();
  while(sptr) {
    text.Add(sptr->data);
    sptr = sptr->next;
  }
  sptr = ob.zones.GetHead();
  while(sptr) {
    zones.Add(sptr->data);
    sptr = sptr->next;
  }
}

int UGCSegmentListNode::MakeTitleString()
{
  icon_link.title.Clear();
  gxString time_string;

  if(!MakeTimeString(time_string)) return 0;
  if(table_data.name.is_null()) return 0;
  icon_link.title << table_data.name << " " << time_string;

  return 1;
}

int UGCSegmentListNode::MakeTimeString(gxString &str)
{
  str.Clear();
  if((begin_time < 1) || (end_time < 1)) return 0;

  gxString time_string;
  time_t gmttime;
  time_t offset;
  int short_format = 1;

  if((is_expired) && (!archive_test_mode)) {
    gxString expired_keyword = "expired"; 
    if(!vtec.pvtec_hdr.action.is_null()) {
      gxString vtec_action = vtec.pvtec_hdr.action.Left(3);
      if(vtec_action == "CAN") {
	gmttime = issue_time.elapsed_time;
	offset = issue_time.gmt_offset;
	if(GetIconLinkLocalTimeString(issue_time.time_zone.c_str(), 
				      gmttime, offset, time_string, short_format)) {
	  str << "canceled" << " at " << time_string.c_str();
	  return 1;
	}
      }
    }

    short_format = 0;
    gmttime = end_time;
    offset = issue_time.gmt_offset;
    if(GetIconLinkLocalTimeString(issue_time.time_zone.c_str(), 
				  gmttime, offset, time_string, short_format)) {
      str << expired_keyword << " " << time_string.c_str();
      return 1;
    }

    gmttime = begin_time;
    offset = issue_time.gmt_offset;
    if(GetIconLinkLocalTimeString(issue_time.time_zone.c_str(), 
				  gmttime, offset, time_string, short_format)) {
      str << expired_keyword << " " << time_string.c_str();
      return 1;
    }

    str << clear << expired_keyword;
    return 1;
  }

  gxString issue_keyword = "issued";
  gxString until_keyword = "until";
  if(!vtec.pvtec_hdr.action.is_null()) {
    gxString vtec_action = vtec.pvtec_hdr.action.Left(3);
    if(vtec_action == "CAN") {
      gmttime = issue_time.elapsed_time;
      offset = issue_time.gmt_offset;
      if(GetIconLinkLocalTimeString(issue_time.time_zone.c_str(), 
				    gmttime, offset, time_string, short_format)) {
	str << "canceled" << " at " << time_string.c_str();
	return 1;
      }
    }
    if(vtec_action == "EXP") {
      issue_keyword = "issued";
      until_keyword = "expires at";
    }
    if(vtec_action == "CON") {
      gmttime = end_time;
      offset = issue_time.gmt_offset;
      if(GetIconLinkLocalTimeString(issue_time.time_zone.c_str(), 
				    gmttime, offset, time_string, short_format)) {
	str << "continued until" << " " << time_string.c_str();
	return 1;
      }
    }
    if(vtec_action == "EXT") {
      issue_keyword = "from";
      until_keyword = "to";
    }
    if(vtec_action == "COR") {
      issue_keyword = "from";
      until_keyword = "until";
    }
  }

  gmttime = begin_time;
  offset = issue_time.gmt_offset;
  if(GetIconLinkLocalTimeString(issue_time.time_zone.c_str(), 
				gmttime, offset, time_string, short_format)) {
    if(issue_keyword.is_null()) {
      str << time_string.c_str();
    }
    else {
      str << issue_keyword << " " << time_string.c_str();
    }
  }
  else {
    return 0;
  }
  
  if(begin_time == end_time) return 1;
  if(end_time < 1) return 1;

  gmttime = end_time;
  offset = issue_time.gmt_offset;
  if(GetIconLinkLocalTimeString(issue_time.time_zone.c_str(), 
				gmttime, offset, time_string, short_format)) {
    if(until_keyword.is_null()) {
      str << " " << time_string.c_str();
    }
    else {
      str << " " << until_keyword << " " << time_string.c_str();
    }
  }
  else {
    return 0;
  }

  return 1;
}

int UGCSegmentList::HasIconLink(const gxString &link, 
				CurrentWarningZoneNode &curr_node)
{
  gxListNode<IconLink> *sptr = curr_node.product_linked_icons.GetHead();
  while(sptr) {
    if(sptr->data.link == link) {
      return 1;
    }
    sptr = sptr->next;
  }

  return 0;
}

int UGCSegmentList::FindCurrentWarningZoneNode(gxString &zone_name)
{
  gxListNode<CurrentWarningZoneNode> *cwbzptr = current_warnings_by_zone.GetHead();
  while(cwbzptr) {
    if(zone_name == cwbzptr->data.name) {
      return 1;
    }
    cwbzptr = cwbzptr->next;
  }

  return 0;
}

int UGCSegmentList::AddOrUpdateCurrentWarningZoneNode(CurrentWarningZoneNode &node, int &exists)
{
  exists = 0;

  gxListNode<CurrentWarningZoneNode> *cwbzptr = current_warnings_by_zone.GetHead();
  while(cwbzptr) {
    if(node.name == cwbzptr->data.name) { // We found the node so update it
      gxListNode<IconLink> *sptr = node.product_linked_icons.GetHead();
      while(sptr) {
	if(!HasIconLink(sptr->data.link, cwbzptr->data)) {
	  int priority = sptr->data.priority;
	  if(priority == 1) {
	    cwbzptr->data.product_linked_icons.AddToFront(sptr->data);
	  }
	  else if(priority == 2) {
	    gxListNode<IconLink> *np = cwbzptr->data.product_linked_icons.GetHead();
	    while(np) {
	      if(np->data.priority == 1) {
		np = np->next;
		continue;
	      }
	      else {
		break;
	      }
	      np = np->next;
	    }
	    if(!np) { // At the end of the list so add to the end
	      cwbzptr->data.product_linked_icons.AddToBack(sptr->data);
	    }
	    else {
	      // Add after the last pri 1
	      if(np->data.priority == 1) {
		cwbzptr->data.product_linked_icons.AddAfter(np, sptr->data);
	      }
	      else { // Add before pri 2 or 3 
		cwbzptr->data.product_linked_icons.AddBefore(np, sptr->data);
	      }
	    }
	  }
	  else {
	    cwbzptr->data.product_linked_icons.AddToBack(sptr->data);
	  }
	}
	sptr = sptr->next;
      }
      exists = 1;
      return 1;
    }
    cwbzptr = cwbzptr->next;
  }

  // The node does not exist so add the node and return
  exists = 0;
  current_warnings_by_zone.Add(node);
  return 1;
}

int UGCSegmentList::BuildZoneTable()
{
  current_warnings_by_zone.Clear();

  // All products has expired
  if(ugc_segment_nodes.IsEmpty()) {
    NT_print("No watches or warnings for any zone or county");
  }

  // Check for upgraded products to remove
  gxListNode<UGCSegmentListNode> *rlptr = ugc_segment_UPG_remove_list.GetHead();
  while(rlptr) {
    if(RemoveUPGSegmentNode(rlptr->data)) {
      NT_print("Dropping previous product on UPG", rlptr->data.pil.c_str(), rlptr->data.etn.c_str());
    }
    rlptr = rlptr->next;
  }

  // Check for exipred products to remove
  rlptr = ugc_segment_OLD_remove_list.GetHead();
  while(rlptr) {
    if(RemoveOLDSegmentNode(rlptr->data)) {
      NT_print("Dropping old product", rlptr->data.pil.c_str(), rlptr->data.etn.c_str());
    }
    rlptr = rlptr->next;
  }

  ZoneTableData table_data;
  gxListNode<gxString> *sptr;
  int exists = 0;

  // Build a list of current zones in order of the zone table
  // Including zones with no wataches or warnings 
  gxListNode<ZoneTableData> *zptr = Zone_table.GetHead();
  while(zptr) {
    CurrentWarningZoneNode zone_node;
    zone_node.name = zptr->data.zone_name;
    zone_node.table_data = zptr->data;
    AddOrUpdateCurrentWarningZoneNode(zone_node, exists);
    if(match_zones_numbers_only) {
      if(exists) {
	if(debug) NT_print("A duplicate zone or county name for", zptr->data.zone_name.c_str());
	if(debug) NT_print(zptr->data.zone_name.c_str(), "will be processed as one zone");
      }
    }
    zptr = zptr->next;
  }
  
  // Update all the currnent watchs and warnings for each zone
  gxListNode<UGCSegmentListNode> *usnptr = ugc_segment_nodes.GetHead();
  while(usnptr) {
    // List of zones and products
    sptr = usnptr->data.zones.GetHead();
    while(sptr) {
      ZoneTableData table_data;
      if(match_zones_numbers_only) {
	if(FindZoneNumber(sptr->data, table_data)) {
	  CurrentWarningZoneNode zone_node;
	  zone_node.name = table_data.zone_name;
	  zone_node.table_data = table_data;
	  zone_node.product_linked_icons.Add(usnptr->data.icon_link);
	  AddOrUpdateCurrentWarningZoneNode(zone_node, exists);
	}
      }
      else {
	gxListNode<ZoneTableData> *zptr2 = Zone_table.GetHead();
	while(zptr2) {
	  if(zptr2->data.zone_number == sptr->data) {
	    table_data = zptr2->data; 
	    CurrentWarningZoneNode zone_node;
	    zone_node.name = table_data.zone_name;
	    zone_node.table_data = table_data;
	    zone_node.product_linked_icons.Add(usnptr->data.icon_link);
	    AddOrUpdateCurrentWarningZoneNode(zone_node, exists);
	  }
	  zptr2 = zptr2->next;
	}
      }
      sptr = sptr->next;
    }
    usnptr = usnptr->next;
  }

  gxListNode<CurrentWarningZoneNode> *cwbzptr = current_warnings_by_zone.GetHead();

  while(cwbzptr) {
    if(cwbzptr->data.product_linked_icons.IsEmpty()) {
      NT_print("No watches or warnings for", cwbzptr->data.name.c_str());
    }
    cwbzptr = cwbzptr->next;
  }
  
  return 1;
}

int UGCSegmentList::AddSegmentNode(UGCSegmentListNode &ugc_node)
{
  gxListNode<UGCSegmentListNode> *usnptr = ugc_segment_nodes.GetHead();

  while(usnptr) {
    if((usnptr->data.pil == ugc_node.pil) && (usnptr->data.etn == ugc_node.etn)) {
      // This product is already in the list so use the more current
      if(ugc_node.begin_time > usnptr->data.begin_time) {
	// This is a new version so update the existing node
	usnptr->data = ugc_node;
	return 1;
      }
      return 0; // We already have the most current product
    }

    usnptr = usnptr->next;
  }

  // We didn't find the product so add the callers object and return success
  ugc_segment_nodes.Add(ugc_node);
  return 1;
}

int UGCSegmentList::UpdateSegmentNode(const UGCSegmentListNode &ugc_node,
				      int ignore_segment_number, int update_all)
{
  gxListNode<UGCSegmentListNode> *usnptr = ugc_segment_nodes.GetHead();

  int num_updates = 0;

  while(usnptr) {
    if(usnptr->data.pil == ugc_node.pil) { // We have the same PIL

      int has_affected_zones = 0;
      gxString org_list_etn = usnptr->data.etn.Left(4);
      gxString org_node_etn = ugc_node.etn.Left(4);

      // Check the ETNs
      if(org_list_etn == org_node_etn) {

	// We have the same ETN so look for matching zones
	gxListNode<gxString> *list_node_zptr = usnptr->data.zones.GetHead();
	while(list_node_zptr) {
	  gxListNode<gxString> *node_zptr = ugc_node.zones.GetHead();
	  while(node_zptr) {
	    if(list_node_zptr->data == node_zptr->data) {
	      has_affected_zones++;
	      break;
	    }
	    node_zptr = node_zptr->next;
	  }
	  
	  list_node_zptr = list_node_zptr->next;
	}
	if(has_affected_zones > 0) { // We have the same zones
	  ignore_segment_number = 1;
	  update_all = 1;
	}
	else { // We do not have any of the same zones
	  ignore_segment_number = 0;
	  update_all = 0;
	}
      }
      if(ignore_segment_number) {
	if(org_list_etn == org_node_etn) {
	  usnptr->data = ugc_node;
	  num_updates++;
	  if(!update_all) return 1;
	}
      }
      else {
        if(usnptr->data.etn == ugc_node.etn) {
	  usnptr->data = ugc_node;
	  num_updates++;
	  if(!update_all) return 1;
	}
      }
    }
   usnptr = usnptr->next;
  }

  return num_updates;
}

int UGCSegmentList::FindPil(const gxString &pil, 
			    UGCSegmentListNode &ugc_node_buf)
{
  ugc_node_buf.Clear();

  gxListNode<UGCSegmentListNode> *usnptr = ugc_segment_nodes.GetHead();

  while(usnptr) {
    if(usnptr->data.pil == pil) {
      ugc_node_buf = usnptr->data;
      return 1;
    }
    usnptr = usnptr->next;
  }

  return 0;
}

int UGCSegmentList::find_zone_in_node(const UGCSegmentListNode &node, const gxString &zone)
{
  gxListNode<gxString> *node_zptr = node.zones.GetHead();
  while(node_zptr) {
    if(node_zptr->data == zone) {
      return 1;
    }
    node_zptr = node_zptr->next;
  }
  return 0;
}

int UGCSegmentList::HasAllZonesNode(const UGCSegmentListNode &list_node, 
				    const UGCSegmentListNode &ugc_node)
{
  int num_zones = 0;
  int has_zones = 0;

  gxListNode<gxString> *list_node_zptr = list_node.zones.GetHead();
  while(list_node_zptr) {
    if(find_zone_in_node(ugc_node, list_node_zptr->data)) {
      has_zones++;
    }
    num_zones++;
    list_node_zptr = list_node_zptr->next;
  }

  if(num_zones != has_zones) return 0;

  num_zones = 0;
  has_zones = 0;

  gxListNode<gxString> *node_zptr = ugc_node.zones.GetHead();
  while(node_zptr) {
    if(find_zone_in_node(list_node, node_zptr->data)) {
      has_zones++;
    }
    num_zones++;
    
    node_zptr = node_zptr->next;
  }
 
  return num_zones == has_zones;
}

int UGCSegmentList::FindSegmentNode(const UGCSegmentListNode &ugc_node, 
				    UGCSegmentListNode &ugc_node_buf, 
				    int ignore_segment_number) 
{
  ugc_node_buf.Clear();

  gxListNode<UGCSegmentListNode> *usnptr = ugc_segment_nodes.GetHead();

  while(usnptr) {
    if(usnptr->data.pil == ugc_node.pil) {
      if(ignore_segment_number) {
	gxString org_list_etn = usnptr->data.etn.Left(4);
	gxString org_node_etn = ugc_node.etn.Left(4);
	if(org_list_etn == org_node_etn) {
	  ugc_node_buf = usnptr->data;
	  return 1;
	}
      }
      else {
	if(usnptr->data.etn == ugc_node.etn) {
	  ugc_node_buf = usnptr->data;
	  return 1;
	}
      }
    }

    usnptr = usnptr->next;
  }

  return 0;
}
  
int UGCSegmentList::RemoveSegmentNode(const UGCSegmentListNode &ugc_node, int match_vtec_hdr)
{
  gxListNode<UGCSegmentListNode> *usnptr = ugc_segment_nodes.GetHead();

  while(usnptr) {
    if(usnptr->data.pil == ugc_node.pil) {
      gxString org_list_etn = usnptr->data.etn.Left(4);
      gxString org_node_etn = ugc_node.etn.Left(4); 
      int has_affected_zones = 0;

      if(org_list_etn == org_node_etn) {

	// 06/23/3009: Added option to match all zones before removing a node
	if(WarningTableMatchAllZones == "TRUE") {
	  has_affected_zones = HasAllZonesNode(usnptr->data, ugc_node);
	}
	else {
	  // Must check the zone numbers on all products 
	  gxListNode<gxString> *list_node_zptr = usnptr->data.zones.GetHead();
	  while(list_node_zptr) {
	    gxListNode<gxString> *node_zptr = ugc_node.zones.GetHead();
	    while(node_zptr) {
	      if(list_node_zptr->data == node_zptr->data) {
		has_affected_zones++;
		break;
	      }
	      node_zptr = node_zptr->next;
	    }
	    list_node_zptr = list_node_zptr->next;
	  }
	}

	if(has_affected_zones > 0) { // We have the same zones
	  if(match_vtec_hdr == 1) { // 05/15/2008: Added to drop expired products
	    if(usnptr->data.vtec.pvtec_string != ugc_node.vtec.pvtec_string) { 
	      return 0;
	    }
	  }
	  if(ugc_segment_nodes.Remove(usnptr)) {
	    return 1;
	  }
	}
      }
    }
    usnptr = usnptr->next;
  }

  return 0;
}

int generate_warning_table_page()
{
  gxString sbuf;

  // Check for command line overrides
  if(use_archive_mode) {
    WarningTableArchiveMode = "1";
  }

  if(!input_cwa.is_null()) {
    WarningTableCWA = input_cwa;
  }

  WarningTableCWA.ToUpper();

  if(WarningTableCWA == "XXX") {
    NT_print("No CWA name supplied for the warning table");
    NT_print("You must supply a CWA name in the CFG file or --cwa argument");
    if(!verbose) {
      NT_console_write_err("No CWA name supplied for the warning table");
      NT_console_write_err("You must supply a CWA name in the CFG file or --cwa argument");
    }
    return 0;
  }

  if(WarningTableTitle == "DEFAULT") {
    WarningTableTitle << clear << WarningTableCWA << " " 
		      << "Watches, Warnings, and Advisories";
  }

  if(VerTableTitle == "DEFAULT") {
    VerTableTitle << clear << WarningTableCWA << " " 
		  << "Watches, Warnings, and Advisories";
  }

  if(area_table_file == "DEFAULT") {
    area_table_file << clear << WarningTableCWA << "-" << "area_table.txt";
  }
  if(zone_table_file == "DEFAULT") {
    zone_table_file << clear << WarningTableCWA << "-" << "zone_table.txt";
  }
  if(product_table_file == "DEFAULT") {
    product_table_file << clear << WarningTableCWA << "-" << "product_table.txt";
  }

  if(dump_html_tables) {
    dump_warning_table = 1;
    dump_key_table = 1;
    dump_ver_table = 1;
  }

  // Check for the warning tables file needed to make Web based table 
  if(!futils_exists(area_table_file.c_str())) {
    if(!BuildDefaultAreaTable(area_table_file, WarningTableCWA)) {
      if(!verbose) {
	NT_console_write_err("Error building default area table", area_table_file.c_str());
      }
      return 0;
    }
  }

  if(!futils_exists(zone_table_file.c_str())) {
    if(!BuildDefaultZoneTable(zone_table_file, WarningTableCWA)) {
      if(!verbose) {
	NT_console_write_err("Error building default zone table", zone_table_file.c_str());
      }
      return 0;
    }
  }

  if(!futils_exists(product_table_file.c_str())) {
    if(!BuildDefaultProductTable(product_table_file, WarningTableCWA)) {
      if(!verbose) {
	NT_console_write_err("Error building default product table", product_table_file.c_str());
      }
      return 0;
    }
  }

  if(!LoadAreaTable()) {
    if(!verbose) {
      NT_console_write_err("Error loading area table", area_table_file.c_str());
    }
    return 0;
  }

  if(!LoadZoneTable()) {
    if(!verbose) {
      NT_console_write_err("Error loading area table", zone_table_file.c_str());
    }
    return 0;
  }

  if(!LoadProductTable()) {
    if(!verbose) {
      NT_console_write_err("Error loading product table", area_table_file.c_str());
    }
    return 0;
  }

  if(Area_table.IsEmpty()) {
    NT_print("Warning Your area table is empty", area_table_file.c_str());
    NT_print("Warning Using dynamic area table");
    use_dynamic_area_table = 1;
    WarningTableRows = "1";
    AreaTableData table_data;
    table_data.area_number = "0";
    table_data.area_name << clear << WarningTableCWA << " Zones and Counties";
    Area_table.Add(table_data);
  }
  if(Zone_table.IsEmpty()) {
    NT_print("Warning Your zone table is empty", zone_table_file.c_str());
    NT_print("Warning Using dynamic zone table");
    use_dynamic_zone_table = 1;
  }

  if(Product_table.IsEmpty()) {
    NT_print("Your product table is empty", product_table_file.c_str());
    if(!verbose) {
      NT_console_write_err("Your product table is empty", product_table_file.c_str());
    }
    return 0;
  }

  if(WarningTableData == "DEFAULT") {
#if defined (__WIN32__)
    WarningTableData = "C:\\tmp\\hazards";
#else
    WarningTableData = "/tmp/hazards";
#endif
    
    if(!input_data_dir.is_null()) {
      WarningTableData = input_data_dir;
    }

    WarningTableData.TrimTrailing('/');
    WarningTableData.TrimTrailing('\\');

    NT_print("Using default data directory", WarningTableData.c_str());

    if(!futils_exists(WarningTableData.c_str())) {
      if(futils_mkdir(WarningTableData.c_str()) != 0) {
	if(!verbose) {
	  NT_console_write_err("Error making default data directory", WarningTableData.c_str()); 
	}
   	NT_print("Error making default data directory", WarningTableData.c_str()); 
	return 0;
      }
    }
  }

#if defined (__WIN32__)
  WarningTableData.TrimTrailing('\\');
#else
  WarningTableData.TrimTrailing('/');
#endif

  if(!futils_exists(WarningTableData.c_str())) {
    NT_print(WarningTableData.c_str(), "does not exist");
    NT_print("Cannot open your product directory", WarningTableData.c_str());
    if(!verbose) {
      NT_console_write_err(WarningTableData.c_str(), "does not exist");
      NT_console_write_err("Cannot open your product directory", WarningTableData.c_str());
    }
    return 0;
  }

  // Open the directory and scan products
  if(!futils_isdirectory(WarningTableData.c_str())) {
    if(!futils_islink(WarningTableData.c_str())) {
      // Skip directory names
      NT_print(WarningTableData.c_str(), "is a file name");
      NT_print("Cannot open your product directory", WarningTableData.c_str());
      if(!verbose) {
	NT_console_write_err(WarningTableData.c_str(), "is a file name");
	NT_console_write_err("Cannot open your product directory", WarningTableData.c_str());
      }
      return 0;
    }
  }

  if(download_web_products) {
    if(!DownloadWarningTableWebProducts()) {
    }
  }

  NT_print("Opening product directory", WarningTableData.c_str());

  DIR* dirp;
  dirent* direntp;
  int num_files = 0;

  char pwd[futils_MAX_DIR_LENGTH];
  if(futils_getcwd(pwd, futils_MAX_DIR_LENGTH) != 0) {
    NT_print("File system error reading present working directory name");
    if(!verbose) {
      NT_console_write_err("File system error reading present working directory name");
    }
    return 0;
  }

  dirp = futils_opendir(WarningTableData.c_str());
  if(dirp == NULL) {
    NT_print("File system error reading product directory");
    NT_print("Cannot open your product directory", 
	     WarningTableData.c_str());
    if(!verbose) {
      NT_console_write_err("File system error reading product directory");
      NT_console_write_err("Cannot open your product directory", 
	     WarningTableData.c_str());
    }
    return 0;
  }
 
  gxBStree<FileListNode> sorted_file_list;

  for(;;) {
    dirent entry;
    direntp = futils_readdir(dirp, &entry);
    if(direntp == NULL) break;
    sbuf << clear << direntp->d_name;
    if((sbuf == ".") || (sbuf == "..")) continue;
    
#if defined (__WIN32__)
    sbuf.InsertAt(0, "\\");
#else
    sbuf.InsertAt(0, "/");
#endif
    sbuf.InsertAt(0, WarningTableData);
    
    FileListNode flnode(sbuf.c_str());
    int exists = 0;
    if(!sorted_file_list.Insert(flnode, &exists)) {
      NT_print("Error adding product to list", sbuf.c_str());
      if(exists) {
	NT_print("Duplicate file in product directory");
      }
    } 
  }
 
  // 06/24/2009: Sort order the file list by file time stamps, highest to lowest epoch time
  FileListNode n;
  while(!sorted_file_list.IsEmpty()) {
    // NOTE: The tree will be empty when the loop is complete
    if(WarningTableSortOrderByHighestTimeStamp == "FALSE") {
      // User request to sort lowest to highest
      sorted_file_list.ExtractFirst(n);
    }
    else {
      // Use highest to lowest epoch time by default
      sorted_file_list.ExtractLast(n);
    }

    if(debug) NT_print("Adding product list entry", n.fname.c_str());
    product_list.Add(n.fname);
    num_files++;
    if((debug) && (debug_level >= 3)) {
      sbuf << clear <<  "[" << num_files << "]file: " << n.fname.c_str() << " :[" <<  n.mod_time << "]";
      NT_console_write(sbuf.c_str());
    }
  }

  if(futils_chdir(pwd) != 0) {
    NT_print("File system error changing directory back to", pwd);
    if(!verbose) {
      NT_console_write_err("File system error changing directory back to", pwd);
    }
    return 0;
  }

  if(num_files == 0) {
    NT_print("No text product files found in data directory", WarningTableData.c_str());
  }

  if(!build_warning_table()) {
    if(!verbose) {
      NT_console_write_err("Error building the warning table product list");
      NT_console_write_err("Use --verbose argument for details");
    }
    return 0;
  }

  if(!make_warning_table_web_page()) {
    if(!verbose) {
      NT_console_write_err("Error building the warning table Web page");
      NT_console_write_err("Use --verbose argument for details");
    }
    return 0;
  }

  return 1;
}

int make_warning_table_web_page()
{
  if(Warning_table.IsSegmentListEmpty()) {
    NT_print("The warning table product list is empty");
    NT_print("A blank table Web page will be generated");
  }

  gxListNode<UGCSegmentListNode> *usnptr;

  gxString sbuf;
  gxString webpage;
  gxString fname = WarningTableWebPageName;
  
  if(fname == "DEFAULT") {
    fname << clear << WarningTableCWA << "-" << "warning_table";
    if(generate_html_page) { // Static HTML page extention
      fname << html_file_extension;
    }
    else { // PHP include file extension
      fname << php_file_extension;
    }
  }

  if(!overwrite_output_webpage) insert_time_stamp(fname);

  gxString pagefilename = fname;
  gxString title = WarningTableTitle;
  gxString description;

  webpage.Clear();
  description << clear << title;
  pagefilename.DeleteBeforeLastIncluding("/");
  pagefilename.DeleteBeforeLastIncluding("\\");

  if(generate_html_page) {
    if(debug) NT_print("Generating the HTML prolog");
    WriteHTMLHeader(pagefilename, title, description, webpage);
  }
  else { // Default to PHP include file
    if(debug) NT_print("Generating the PHP include file prolog");
    WritePHPIncludeHeader(pagefilename, description, webpage);
  }

  webpage << "\n";

  gxString jscode;
  if(!Warning_table.IsSegmentListEmpty()) {
    webpage << "<script language=\"javascript\">" << "\n";
    webpage << "<!--" << "\n";
    usnptr = Warning_table.GetSegmentListHead();
    while(usnptr) {
      if(!usnptr->data.jscode.is_null()) {
	if(dump_warning_table) {
	  jscode << usnptr->data.jscode << "\n";
	}
	webpage << usnptr->data.jscode << "\n";
      }
      usnptr = usnptr->next;
    }
    webpage << "// -->" << "\n";
    webpage << "</script>" << "\n";
    webpage << "\n";
  }


  gxString key_table_link, key_table, key_table_jscode;
  NT_print("Generating the key table");
  gen_key_table(key_table);
  if((add_key_table) && (!add_key_table_in_page)) { 
    NT_print("Generating the key table popup window");
    MakeJSPopupKeyTable(key_table_jscode, key_table_link, key_table);
    webpage << "\n";
    webpage << "<script language=\"javascript\">" << "\n";
    webpage << "<!--" << "\n";
    webpage << key_table_jscode << "\n";
    webpage << "// -->" << "\n";
    webpage << "</script>" << "\n";
    webpage << "\n";
  }

  gxString ver_table_link, ver_table, ver_table_jscode;;
  gen_ver_table(ver_table);
  NT_print("Generating the verification table");
  if((add_ver_table) && (!add_ver_table_in_page)) { 
    NT_print("Generating the verification table popup window");
    MakeJSPopupVerTable(ver_table_jscode, ver_table_link, ver_table);
    webpage << "\n";
    webpage << "<script language=\"javascript\">" << "\n";
    webpage << "<!--" << "\n";
    webpage << ver_table_jscode << "\n";
    webpage << "// -->" << "\n";
    webpage << "</script>" << "\n";
    webpage << "\n";
  }

  gxString curr_time_string;
  if(WarningTablePrintUpdateTime == "1") {
    if(!MakeWarningTableTimeString(curr_time_string)) {
      NT_print("Error getting local system time");
      if(debug) curr_time_string = "System time error";
    }
  }
  if(curr_time_string.is_null()) {
    if(WarningTablePrintUpdateTime == 1) {
      NT_print("Error NULL system time");
      if(debug) {
	curr_time_string = "NULL system time";
      }
    }
  }

  // Start of main table
  gxString warning_table;
  if(WarningTableMainTableAttributes == "NONE") {
    warning_table << "<table border=\"" << WarningTableBorderTest << "\" cellpadding=\"0\" cellspacing=\"0\" width=\"" 
		  << WarningTableWidth << "\">" << "\n";
  }
  else {
    warning_table << "<table border=\"" << WarningTableBorderTest << "\" cellpadding=\"0\" cellspacing=\"0\" width=\"" 
		  << WarningTableWidth << "\" " << WarningTableMainTableAttributes << " >" << "\n";
  }

  warning_table << "<tr valign=\"top\">" << "\n";
  warning_table << "<td bgcolor=\"" << WarningTableTitleBGColor << "\" colspan=\"" 
		<< WarningTableRows << "\" width=\"" 
		<< WarningTableWidth << "\" align=\"center\" style=\"color:" 
		<< WarningTableTitleFontColor << "\">" << "\n";
  warning_table << "<font style=\"font-size: " << WarningTableTitleFontSize << "\">" << "\n"
		<< "&nbsp;" << WarningTableTitle << "&nbsp;" << "\n";
  
  if(!curr_time_string.is_null()) {
    warning_table << "&nbsp;" << curr_time_string << "&nbsp;"<< "\n";
  }
  warning_table << "</font>" << "\n";
  
  if(!WarningTableHeaderLink.is_null()) {
    if(WarningTableHeaderLink != "NONE") {
      warning_table << "&nbsp;" << WarningTableHeaderLink << "&nbsp;" << "\n";
    }
  }
  warning_table << "</td>" << "\n";
  warning_table << "</tr>" << "\n";

  // Make all the subtable calculations here
  gxString subtable1_width = "175";
  gxString subtable2_width = "175";
  gxString subtable3_width = "175";
  gxString  td_width1 = "82";
  
  // Subtable 1 = Area title
  if(WarningSubTable1Width == "AUTO") {
    if(WarningTableWidth[WarningTableWidth.length()-1] == '%') {
      subtable1_width = WarningTableWidth; 
    }
    else {
      if(WarningTableRows.Atoi() <= 0) WarningTableRows = 3;
      int ibuf = WarningTableWidth.Atoi() / WarningTableRows.Atoi();
      if(ibuf < 150) ibuf = 150;
      subtable1_width << clear << ibuf;
    }
  }
  else {
    subtable1_width = WarningSubTable1Width;
  }
  if(WarningZoneNameTableDataWidth == "AUTO") {
    if(WarningTableWidth[WarningTableWidth.length()-1] == '%') {
      WarningZoneNameTableDataWidth = WarningTableWidth;
    }
    else{
      int ibuf = subtable1_width.Atoi() / 2;
      if(ibuf < 82) ibuf = 82;
      td_width1 << clear << ibuf;
    }
  }
  else {
    td_width1 = WarningZoneNameTableDataWidth;
  }

  // Subtable2 = Area name table
  if(WarningSubTable2Width == "AUTO") {
    subtable2_width << clear << subtable1_width; 
  }
  else {
    subtable2_width << clear << WarningSubTable2Width;
  }

  // Subtable 3 = product icons
  if(WarningSubTable3Width != "AUTO") {
    subtable3_width << clear << WarningSubTable3Width;
  }

  // Start of main table data
  warning_table << "<tr valign=\"top\">" << "\n";


  gxListNode<AreaTableData> *aptr = Area_table.GetHead();
  gxListNode<CurrentWarningZoneNode> *zone_node = Warning_table.current_warnings_by_zone.GetHead();
  // Check the node before processing 
  while(zone_node) {
    if(!FindAreaNumber(zone_node->data.table_data.area_number)) {
      if(!zone_node->data.product_linked_icons.IsEmpty()) {
	NT_print("Error cannot match zone number for", zone_node->data.name.c_str(),
		 zone_node->data.table_data.area_number.c_str());
	if(aptr) {
	  NT_print(zone_node->data.name.c_str(), 
		   "has active data and will be placed under", 
		   aptr->data.area_name.c_str());
	  zone_node->data.table_data.area_number = aptr->data.area_number;
	}
      }
    }
    zone_node = zone_node->next;
  }

  int max_rows = WarningTableRows.Atoi();
  int num_rows = 0;

  while(aptr) {
    if((aptr->data.area_name == "NONE") || (aptr->data.area_name == "none")) {
      aptr = aptr->next;
      continue;
    }

    if(max_rows == 0) {
      // End the previous row
      warning_table << "</tr>" << "\n";
      // Start a new row
      warning_table << "<tr valign=\"top\">" << "\n";
      max_rows = WarningTableRows.Atoi();
      num_rows++;
    }
    max_rows--;

    warning_table << "<td>" << "\n";

    // Subtable 1 = Area title
    if(WarningSubTable1Width == "NONE") {
      warning_table << "<table cellspacing=\"0\" cellpadding=\"0\" border=\"" 
		    << WarningTableBorderTest
		    << " valign=\"top\">" 
		    << "\n";
    }
    else {
      warning_table << "<table cellspacing=\"0\" cellpadding=\"0\" border=\"" 
		    << WarningTableBorderTest << "\" width=\"" 
		    << subtable1_width << "\" valign=\"top\">" 
		    << "\n";
    }
    warning_table << "<tr>" << "\n";
    warning_table << "<td bgcolor=\"" << WarningTableAreaTitleBGColor << "\">" << "\n";

    if((aptr->data.area_name == "BLANK") || (aptr->data.area_name == "blank")) {
      warning_table << "<font color=\"" << WarningTableAreaTitleFontColor << "\" style=\"font-size: " 
		    << WarningTableAreaTitleFontSize << "\">"
		    << "&nbsp;"
		    << "</font>"
		    << "</td>" << "\n";
    }
    else {
      warning_table << "<font color=\"" << WarningTableAreaTitleFontColor << "\" style=\"font-size: " 
		    << WarningTableAreaTitleFontSize << "\">"
		    << aptr->data.area_name
		    << "</font>"
		    << "</td>" << "\n";
    }
    warning_table << "</tr>" << "\n";

    warning_table << "<tr>" << "\n";
    warning_table << "<td>" << "\n";

    // Start of icon product table
    // Subtable2 = Area name table
    if(WarningSubTable2Width == "NONE") {
      warning_table << "<table cellspacing=\"0\" cellpadding=\"0\" border=\""<< WarningTableBorderTest 
		    << "\" valign=\"top\">" 
		    << "\n";
    }
    else {
      warning_table << "<table cellspacing=\"0\" cellpadding=\"0\" border=\""<< WarningTableBorderTest 
		    << "\" width=\"" << subtable2_width << "\" valign=\"top\">" 
		    << "\n";
    }
    
    zone_node = Warning_table.current_warnings_by_zone.GetHead();
    while(zone_node) {
      if(zone_node->data.table_data.area_number == aptr->data.area_number) {
	warning_table << "<tr>" << "\n";
	
	gxString is_blank_zone = zone_node->data.name.Left(5); 
	if((is_blank_zone == "BLANK") || (is_blank_zone == "blank")) {
	  if(WarningZoneNameTableDataWidth == "NONE") {
	    warning_table << "<td>&nbsp;</td>" << "\n";
	  }
	  else {
	    warning_table << "<td width=\"" << td_width1 << "\">&nbsp;</td>" << "\n";
	  }
	}
	else {
	  gxString myzone_name = zone_node->data.name;

	  if(!zone_node->data.table_data.zone_url.is_null()) {
	    if(zone_node->data.table_data.zone_title.is_null()) {
	      zone_node->data.table_data.zone_title = zone_node->data.name; 
	    }
	    myzone_name << clear << "<a href=\"" << zone_node->data.table_data.zone_url
			<< "\" title=\"" << zone_node->data.table_data.zone_title
			<< "\" target=\"_blank\">" << zone_node->data.name << "</a>";
	  }

	  if(WarningZoneNameTableDataWidth == "NONE") {
	    warning_table << "<td bgcolor=\"" << WarningTableZoneNameBGColor
			  << "\" style=\"padding-left:2px;border-top:solid 1px;\""
			  << " onMouseOver=\"this.bgColor = \'" << WarningTableZoneNameMouseOver 
			  << "\'\" onMouseOut = \"this.bgColor = \'" 
			  << WarningTableZoneNameBGColor << "\'\"" << ">"
			  << "\n"
			  << "<font color=\"" << WarningTableZoneNameFontColor 
			  << "\" style=\"font-size: " << WarningTableZoneNameFontSize << "\">" << "\n"
			  <<  myzone_name << "\n"
			  << "</font>" << "\n"
			  << "</td>" << "\n";
	  }
	  else {
	    warning_table << "<td bgcolor=\"" << WarningTableZoneNameBGColor << "\" width=\"" << td_width1 
			  << "\" style=\"padding-left:2px;border-top:solid 1px " 
			  << WarningTableZoneNameFontColor << ";\""
			  << " onMouseOver=\"this.bgColor = \'" << WarningTableZoneNameMouseOver 
			  << "\'\" onMouseOut = \"this.bgColor = \'" 
			  << WarningTableZoneNameBGColor << "\'\"" << ">"
			  << "\n"
			  << "<font style=\"font-size: " << WarningTableZoneNameFontSize << "\">" << "\n"
			  <<  myzone_name << "\n"
			  << "</font>" << "\n"
			  << "</td>" << "\n";
	  }

	  warning_table << "<td>" << "\n";
	  // Subtable 3 = product icons
	  if(WarningSubTable3Width == "AUTO") {
	    warning_table << "<table cellspacing=\"1\" cellpadding=\"1\" border=\"" 
			  << WarningTableBorderTest << "\" valign=\"top\" align=\"left\">" 
			  << "\n";
	  }
	  else {
	    warning_table << "<table cellspacing=\"1\" cellpadding=\"1\" border=\"" 
			  << WarningTableBorderTest << "\" valign=\"top\" width=\""
			  << subtable3_width << "\" align=\"left\">" 
			  << "\n";
	  }
	  warning_table << "<tr>" << "\n";
	  if(zone_node->data.product_linked_icons.IsEmpty()) {
	    warning_table << "<td bgcolor=" << WarningTableProductBGColor << ">"
			  << "<font color=\"" << WarningTableProductFontColor 
			  << "\" style=\"font-size: " << WarningTableProductFontSize << "\">"
			  << "None"
			  << "</font>"
			  << "</td>" << "\n" ;
	  }
	  else {
	    gxListNode<IconLink> *ilptr = zone_node->data.product_linked_icons.GetHead();
	    while(ilptr) {
	      warning_table << "<td bgcolor=" << WarningTableProductBGColor << ">"
			    << "<font color=\"" << WarningTableProductFontColor 
			    << "\" style=\"font-size: " << WarningTableProductFontSize << "\">"; 
	      if(!ilptr->data.link.is_null()) {
		warning_table << ilptr->data.link;
	      }
	      else {
		warning_table << "&nbsp;?";
	      }
	      warning_table << "</font>"
			    << "</td>" << "\n";
	      ilptr = ilptr->next;
	    }
	  }
	  warning_table << "</tr>" << "\n";
	  warning_table << "</table>" << "\n";
	  warning_table << "</td>" << "\n";
	}
	warning_table << "</tr>" << "\n";
      }
      zone_node = zone_node->next;
    }
    warning_table << "</table>" << "\n";
    // End of icon product table

    // End of area name table
    warning_table << "</td>" << "\n";
    warning_table << "</tr>" << "\n";
    warning_table << "</table>" << "\n";

    // End of main table data
    warning_table << "</td>" << "\n";

    aptr = aptr->next;    
  }

  // End of main table row
  warning_table << "</tr>" << "\n";

  // End of main table
  warning_table << "</table>" << "\n";

  webpage << warning_table;

  // 07/10/2008: Put the key table link and warning table link in a table
  if(((add_key_table) && (!add_key_table_in_page)) || ((add_ver_table) && (!add_ver_table_in_page))) {
    webpage << "\n";
    webpage << "<table border=\"0\" cellpadding=\"5\" cellspacing=\"5\" >" << "\n";
    webpage << "<tr>" << "\n";
  }

  if((add_key_table) && (!add_key_table_in_page)) {
    webpage << "<td>" << "\n"; // 07/10/2008: Put the key table link and warning table link in a table
    webpage << "\n";
    webpage << "<table border=\"" << KeyTableLinkBorder 
	    <<"\" cellpadding=\"0\" cellspacing=\"0\" >" << "\n";
    webpage << "<tr>" << "\n";
    webpage << "<td bgcolor=\"" << WarningTableZoneNameBGColor
	    << "\" style=\"padding-left:2px;padding-right:2px;\""
	    << " onMouseOver=\"this.bgColor = \'" << WarningTableZoneNameMouseOver 
	    << "\'\" onMouseOut = \"this.bgColor = \'" 
	    << WarningTableZoneNameBGColor << "\'\"" << ">"
	    << "\n"
	    << "<font color=\"" << WarningTableZoneNameFontColor 
	    << "\" style=\"font-size: " << WarningTableZoneNameFontSize << "\">" << "\n";
    webpage << key_table_link;
    webpage << "</font>" << "\n";
    webpage << "</td> </tr> </table>" << "\n";
    webpage << "</td>" << "\n"; // 07/10/2008: Put the key table link and warning table link in a table
    webpage << "\n";
  }

  if((add_ver_table) && (!add_ver_table_in_page)) {
    webpage << "<td>" << "\n"; // 07/10/2008: Put the key table link and warning table link in a table
    webpage << "<table border=\"" << VerTableLinkBorder 
	    << "\" cellpadding=\"0\" cellspacing=\"0\" >" << "\n";
    webpage << "<tr>" << "\n";
    webpage << "<td bgcolor=\"" << WarningTableZoneNameBGColor
	    << "\" style=\"padding-left:2px;padding-right:2px;\""
	    << " onMouseOver=\"this.bgColor = \'" << WarningTableZoneNameMouseOver 
	    << "\'\" onMouseOut = \"this.bgColor = \'" 
	    << WarningTableZoneNameBGColor << "\'\"" << ">"
	    << "\n"
	    << "<font color=\"" << WarningTableZoneNameFontColor 
	    << "\" style=\"font-size: " << WarningTableZoneNameFontSize << "\">" << "\n";
    webpage << ver_table_link;
    webpage << "</font>" << "\n";
    webpage << "</td> </tr> </table>" << "\n";
    webpage << "</td>" << "\n"; // 07/10/2008: Put the key table link and warning table link in a table
    webpage << "\n";
  }

  // 07/10/2008: Put the key table link and warning table link in a table
  if(((add_key_table) && (!add_key_table_in_page)) || ((add_ver_table) && (!add_ver_table_in_page))) {
    webpage << "</tr>" << "\n";
    webpage << "</table>" << "\n";    
  }

  // 07/10/2008: Added warning table help bullets
  if((!WarningTableHelpBullet1.is_null()) || (!WarningTableHelpBullet2.is_null()) || 
     (!WarningTableHelpBullet3.is_null()) || (!WarningTableHelpBullet4.is_null()) ||
     (!WarningTableHelpBullet5.is_null())) {
    webpage << "\n";
    webpage << "<ul>" << "\n";
    if(!WarningTableHelpBullet1.is_null()) {
      webpage << "<li>" << WarningTableHelpBullet1 << "</li>" << "\n";
    }
    if(!WarningTableHelpBullet2.is_null()) {
      webpage << "<li>" << WarningTableHelpBullet2 << "</li>" << "\n";
    }
    if(!WarningTableHelpBullet3.is_null()) {
      webpage << "<li>" << WarningTableHelpBullet3 << "</li>" << "\n";
    }
    if(!WarningTableHelpBullet4.is_null()) {
      webpage << "<li>" << WarningTableHelpBullet4 << "</li>" << "\n";
    }
    if(!WarningTableHelpBullet5.is_null()) {
      webpage << "<li>" << WarningTableHelpBullet5 << "</li>" << "\n";
    }
    webpage << "</ul>" << "\n";
  } 

  if((add_ver_table) && (add_ver_table_in_page)) {
    webpage << "\n";
    webpage << "<br>" << "\n";
    webpage << ver_table;
    webpage << "\n";
  }

  if((add_key_table) && (add_key_table_in_page)) {
    webpage << "\n";
    webpage << "<br>" << "\n";
    webpage << key_table;
    webpage << "\n";
  }

  if(generate_html_page) {
    if(debug) NT_print("Generating the HTML epilog");
    WriteHTMLTail(webpage);
  }
  else {
    if(debug) NT_print("Generating the PHP include file epilog");
    WritePHPTail(webpage);
  }
  
  gxString warning_table_fname = fname;
  int rv = WriteWebPageFile(fname, pagefilename, webpage);

  if(dump_warning_table) {
    fname = warning_table_html_file;
    if(fname == "DEFAULT") {
      fname << clear << WarningTableCWA << "-" << "warning_table_html_code" << html_file_extension;
    }
    if(!overwrite_output_webpage) insert_time_stamp(fname);
    pagefilename = fname;
    WriteWebPageFile(fname, pagefilename, warning_table);

    fname = warning_table_js_file;
    if(fname == "DEFAULT") {
      fname << clear << WarningTableCWA << "-" << "warning_table_js_code" << js_file_extension;
    }
    if(!overwrite_output_webpage) insert_time_stamp(fname);
    pagefilename = fname;
    WriteWebPageFile(fname, pagefilename, jscode);
  }
  if(dump_key_table) {
    fname = key_table_html_file;
    if(fname == "DEFAULT") {
      fname << clear << WarningTableCWA << "-" << "key_table_html_code" << html_file_extension;
    }
    if(!overwrite_output_webpage) insert_time_stamp(fname);
    pagefilename = fname;
    WriteWebPageFile(fname, pagefilename, key_table);
  }
  if(dump_ver_table) {
    fname = ver_table_html_file;
    if(fname == "DEFAULT") {
      fname << clear << WarningTableCWA << "-" << "ver_table_html_code" << html_file_extension;
    }
    if(!overwrite_output_webpage) insert_time_stamp(fname);
    pagefilename = fname;
    WriteWebPageFile(fname, pagefilename, ver_table);
  }

  if(add_short_fused_table) {
    if(ShortFusedTableTitle == "DEFAULT") {
      ShortFusedTableTitle << clear << WarningTableCWA << " " 
			   << "Short Fused Warnings";
    }
    if(ShortFusedTableLinkToWarningTable == "DEFAULT") {
      ShortFusedTableLinkToWarningTable << clear << warning_table_fname;
    }
    if(ShortFusedTableLinkToWarningTableTitle == "DEFAULT") {
      ShortFusedTableLinkToWarningTableTitle << clear 
					     << "Click here for current Watch, Warnings, and Advisories table";
    }

    gxString short_fused_table;
    gen_short_fused_table(short_fused_table);
    NT_print("Generating short fused warning table");

    fname = short_fused_table_php_file;
    if(fname == "DEFAULT") {
      fname << clear << WarningTableCWA << "-" << "short_fused_warning_table";
      if(generate_html_page) { // Static HTML page extention
	fname << html_file_extension;
      }
      else { // PHP include file extension
	fname << php_file_extension;
      }
    }

    if(!overwrite_output_webpage) insert_time_stamp(fname);
    pagefilename = fname;
    WriteWebPageFile(fname, pagefilename, short_fused_table);
  }

  if(rss_feed) {
    gxString rss_feed;
    NT_print("Writing WWA RSS feeds");
    fname << clear << WarningTableCWA << "_" << "warnings" << rss_file_ext;
    fname.ToLower();
    if(rss_output_dir == "DEFAULT") {
      rss_output_dir = WebpageOutputDirectory;
    }
    if(!overwrite_output_webpage) insert_time_stamp(fname);

    gen_rss_feed_all_WWA(rss_feed);
    if(!write_file(fname, rss_output_dir, rss_feed)) {
      NT_print("Error writing all WWA table feed");
    }

    // Write a feed for each ZONE number
    gen_rss_feeds_for_all_zones();
  }


  if(add_ver_worksheets) {
    gen_ver_worksheets();
  }

  return rv;
}

int SetupDownloadURLS(gxString &product_url, gxString &product_url_backup)
{
  product_url.Clear();
  product_url_backup.Clear();

  if(product_web_site_arg.is_null()) {
    product_url << clear << WarningTableProductWebsite 
		<< WarningTableProductWebsiteScript 
		<< WarningTableProductWebsiteQuery;
  }
  else {
    product_url << clear << product_web_site_arg;
  }

  if(product_web_site_arg_backup.is_null()) {
    product_url_backup << clear << WarningTableProductWebsiteBackup 
		       << WarningTableProductWebsiteScriptBackup 
		       << WarningTableProductWebsiteQueryBackup;
  }
  else {
    product_url_backup << clear << product_web_site_arg_backup;
  }

  product_url.ReplaceString("&", "&amp;");
  product_url_backup.ReplaceString("&", "&amp;");

  return 1;
} 

int DownloadWarningTableWebProducts()
{
  gxString product_url;
  gxString product_url_backup;
  gxString fname;
  gxString product;

  SetupDownloadURLS(product_url, product_url_backup);

  // 05/12/2009
  // Updated code to use the following site:
  // http://forecast.weather.gov/product.php?site=NWS&issuedby=MFL&product=AFD&format=TXT&version=1&glossary=0
  //
  // Our config file string will look the following:
  // http://forecast.weather.gov/product.php?site=SITEVAR&issuedby=ISSUEDBYVAR&product=PRODUCTVAR&format=FORMATVAR&version=VERSIONVAR&glossary=GLOSSARYVAR
  //
  // NOTE: PRE tag is: <pre class="glossaryProduct">
  // NOTE: Change code to: "<pre "

  gxString SITEVAR = "NWS";
  gxString FORMATVAR = "TXT";
  gxString VERSIONVAR = "1";
  gxString GLOSSARYVAR = "0";

  // Build the download list in assending sort order and elminate any duplicate products in the download list
  gxBStree<gxString> download_list;
  gxListNode<ProductTableData> *ptptr = Product_table.GetHead();
  while(ptptr) {
    product << clear << ptptr->data.pil <<  ptptr->data.office;
    download_list.Insert(product);
    ptptr = ptptr->next;
  }

  // Download the products
  gxBStreeNode<gxString> *node = download_list.GetRoot();
  gxBStreeIterator tree_iterator(node);
  while((node = (gxBStreeNode<gxString> *)tree_iterator.GetNext()) != 0) {
    gxString MYPRODUCT = node->data.c_str();
    gxString ISSUEDBYVAR = node->data.Right(3);
    gxString PRODUCTVAR = node->data.Left(3);

    gxString primary_site = product_url;
    primary_site.ReplaceString("MYPRODUCT", MYPRODUCT.c_str());
    primary_site.ReplaceString("SITEVAR", SITEVAR.c_str());
    primary_site.ReplaceString("ISSUEDBYVAR", ISSUEDBYVAR.c_str());
    primary_site.ReplaceString("PRODUCTVAR", PRODUCTVAR.c_str());
    primary_site.ReplaceString("FORMATVAR", FORMATVAR.c_str());
    primary_site.ReplaceString("VERSIONVAR", VERSIONVAR.c_str());
    primary_site.ReplaceString("GLOSSARYVAR", GLOSSARYVAR.c_str());

    if(WarningTableProductEnableWebsiteBackup == "TRUE") {
      gxString backup_site = product_url_backup;
      backup_site.ReplaceString("MYPRODUCT", MYPRODUCT.c_str());
      backup_site.ReplaceString("SITEVAR", SITEVAR.c_str());
      backup_site.ReplaceString("ISSUEDBYVAR", ISSUEDBYVAR.c_str());
      backup_site.ReplaceString("PRODUCTVAR", PRODUCTVAR.c_str());
      backup_site.ReplaceString("FORMATVAR", FORMATVAR.c_str());
      backup_site.ReplaceString("VERSIONVAR", VERSIONVAR.c_str());
      backup_site.ReplaceString("GLOSSARYVAR", GLOSSARYVAR.c_str());
      if(DownloadProduct(primary_site.c_str(), backup_site.c_str(), fname)) {
	product_list.Add(fname);
      }
    }
    else {
      if(DownloadProduct(primary_site.c_str(), fname)) {
	product_list.Add(fname);
      }
    }
  }

  return 1;
}

int build_warning_table()
// Function used to read the products and build the table.
// Returns true if sucessful or false if an error occurs. 
{
  NT_print("Building warning table from product list");

  if(product_list.IsEmpty()) {
    if(allow_empty_tables) {
      NT_print("Warning the product list is empty");
      NT_print("Building empty table");
    }
    else {
      NT_print("Error the product list is empty");
      NT_print("Program will not build an empty table");
      return 0;
    }
  }

  gxString open_fname, sbuf;
  TextProduct text_product;
  UGCSegmentListNode ugc_node;
  gxListNode<gxString> *sptr;
  gxListNode<gxString> *segptr;
  ProductTableData product_table_data;
  gxListNode<gxString> *lptr = product_list.GetHead();
  gxList<UGC> ugc_product_list;
  

  while(lptr) {
    open_fname = lptr->data;

    // Process the system file name
    if(futils_isdirectory(open_fname.c_str())) {
      // Skip directory names
      NT_print(open_fname.c_str(), "is a directory name");
      NT_print("Skipping directory", open_fname.c_str());
      open_fname.Clear(); // Reset the open file name
      lptr = lptr->next;
      continue;
    }
    
    if(!futils_exists(open_fname.c_str())) {
      // Skip directory names
      NT_print(open_fname.c_str(), "file does not exist");
      NT_print("Skipping", open_fname.c_str());
      open_fname.Clear(); // Reset the open file name
      lptr = lptr->next;
      continue;
    }
    
    // Process file(s) here
    if(debug) NT_print("Processing file", open_fname.c_str());

    int file_open_try = 0;
    int file_open_pass = 1;
    while(!is_file_closed(open_fname.c_str(), file_open_wait_time)) {
      file_open_try++;
      if(file_open_try == 1) NT_print(open_fname.c_str(), 
				      "is open by another application");
      if(file_open_try < max_file_open_tries) {
	sbuf << clear << "Waiting " << file_open_wait_time << " seconds"; 
	NT_print(sbuf.c_str());
	api_Sleep(file_open_wait_time);
	continue;
      }
      file_open_pass = 0;
      break; // Exit the is open loop and signal to continue on fail
    }
    if(!file_open_pass) {
      sbuf << clear << "Skipping " << open_fname << " after " 
	   << file_open_try << " attempts"; 
      NT_print(sbuf.c_str());
      open_fname.Clear(); // Reset the open file name
      file_open_try = 0;
      lptr = lptr->next;
      continue;
    }

    if(!text_product.LoadProduct(open_fname.c_str())) {
      NT_print("Error loading parsing product file");
      NT_print(text_product.error_string.c_str());
      NT_print("Skipping file", open_fname.c_str());
      open_fname.Clear(); // Reset the open file name
      lptr = lptr->next;
      continue;
    }

    if(!text_product.ParseWMOHeader()) {
      NT_print("Error loading and parsing product file");
      NT_print(text_product.error_string.c_str());
      NT_print("Skipping file", open_fname.c_str());
      open_fname.Clear(); // Reset the open file name
      lptr = lptr->next;
      continue;
    }

    if(!FindPIL(text_product.product_hdr.pil, sbuf)) {
      if(debug){
	NT_print("PIL not found in product list", sbuf.c_str());
	NT_print("PIL in file is", text_product.product_hdr.pil.c_str());
	NT_print("Skipping file", open_fname.c_str());
      }
      open_fname.Clear(); // Reset the open file name
      lptr = lptr->next;
      continue;
    }

    NT_print("Processing", sbuf.c_str(), open_fname.c_str());
    if(debug) NT_print("PIL in file is", text_product.product_hdr.pil.c_str());

    if(!text_product.ParseProduct()) {
      NT_print("Error parsing product file");
      NT_print(text_product.error_string.c_str());
      NT_print("Skipping file", open_fname.c_str());
      open_fname.Clear(); // Reset the open file name
      lptr = lptr->next;
      continue;
    }

    // Loop through the UGC segments for this product file
    gxListNode<UGC> *uptr = text_product.product_hdr.ugc_list.GetHead();

    gxString product = text_product.product_hdr.pil;
    if(product.length() == 9) product.DeleteAt(0, 3);
    if(product.length() == 6) product.DeleteAt(3, 3);
    
    int segment_number = 0;

    while(uptr) {
      // Update the current segment number
      segment_number++;
      int dropnode = 0;
      int update_node = 0;

      // Clear the object with each read loop
      ugc_node.Clear();
      product_table_data.Clear();

      // Set the default values
      ugc_node.pil = product;
      ugc_node.etn = "0000"; // Dummy ETN for non-vtec products
      ugc_node.full_pil << clear << text_product.product_hdr.pil << " SEGMENT " << segment_number;
      ugc_node.full_pil.TrimLeading('\n'); ugc_node.full_pil.TrimLeading(' ');
      ugc_node.full_pil.TrimTrailing('\n'); ugc_node.full_pil.TrimTrailing(' ');
      ugc_node.wmo_header = text_product.product_hdr.wmo_hdr_string;
      ugc_node.wmo_header.TrimLeading('\n'); ugc_node.wmo_header.TrimLeading(' ');
      ugc_node.wmo_header.TrimTrailing('\n'); ugc_node.wmo_header.TrimTrailing(' ');
      ugc_node.name = text_product.product_hdr.name;
      ugc_node.issue_office = text_product.product_hdr.issue_office;
      ugc_node.issue_time = text_product.product_hdr.issue_time;
      ugc_node.begin_time = text_product.product_hdr.issue_time.elapsed_time;
      ugc_node.end_time = uptr->data.ugc_hdr.ugc_time.elapsed_time;
      ugc_node.is_expired = uptr->data.ugc_hdr.is_expired;
      ugc_node.ugc_segment_header = uptr->data.ugc_segment_header;
      ugc_node.ugc_segment_header.TrimLeading('\n');
      ugc_node.ugc_segment_header.TrimTrailing('\n');
      ugc_node.ugc_segment_header.InsertAt(0, "\n");
      ugc_node.ugc_segment_header << "\n";
      ugc_node.segment_number = segment_number;

      if(WarningTableSkipDuplicateUGCHeaders == "TRUE") {
	// 06/23/3009: Look for duplicate porducts
	int has_dup_prod = 0;
	if(debug) {
	  NT_print("Checking UGC list for duplicate", ugc_node.full_pil.c_str());
	}
	gxListNode<UGC> *uptr_dup = ugc_product_list.GetHead();
	while(uptr_dup) {
	  if(uptr_dup->data ==  uptr->data) {
	    has_dup_prod = 1;
	    if((debug) && (debug_level >= 2)) {
	      NT_console_write_lines("Printing duplicate UGC headers:", 
				     uptr_dup->data.ugc_segment_header.c_str(), 
				     uptr->data.ugc_segment_header.c_str()); 
	    }
	    break;
	  }
	  uptr_dup = uptr_dup->next;
	}
	
	if(has_dup_prod) {
	  NT_print(ugc_node.full_pil.c_str(), "is already in the product list and will not be included");
	  uptr = uptr->next;
	  continue;
	}
	// Keep a list of the products we read so far
	ugc_product_list.Add(uptr->data);
      }

      if(WarningTableSkipExpiredUGCHeaders == "TRUE") {	
	// 06/23/2009: Drop expired products here
	// Advance to the next UGC segment
	if(debug) {
	  NT_print("Checking", ugc_node.full_pil.c_str(), "for expired UGC header");
	  if(WarningTableArchiveMode == "1") {
	    NT_print("We are in archive mode");
	  }
	}
	if((uptr->data.ugc_hdr.is_expired) && (WarningTableArchiveMode == "0")) { 
	  NT_print(ugc_node.full_pil.c_str(), "has expired and will not be included");
	  if((debug) && (debug_level >= 2)) {
	    NT_console_write_lines("Printing expired UGC header:", 
				   uptr->data.ugc_segment_header.c_str()); 
	  }
	  uptr = uptr->next;
	  continue;
	}
      }

      if((debug) && (debug_level >= 2)) {
	NT_console_write("Processing ", ugc_node.full_pil.c_str());
	NT_console_write_lines("Printing UGC header:", 
			       uptr->data.ugc_segment_header.c_str()); 
      }
      
      if(!uptr->data.vtec_list.IsEmpty()) { // This is a VTEC product
	
	// Process all the VTEC headers in this UGG segment
        gxListNode<VTEC> *vptr = uptr->data.vtec_list.GetHead();
	while(vptr) { // Loop through all the VTEC headers
	  if(vptr->data.IsHydro()) {
	    if(debug) {
	      NT_print("Skipping H-VTEC header", product.c_str());
	    }
	    vptr = vptr->next;    
	    continue;
	  }

	  // Keep a record of this VTEC entry
	  ugc_node.vtec = vptr->data;
	  
	  // Read the VTEC codes here
	  gxString vtec_action = vptr->data.pvtec_hdr.action.Left(3);
	  gxString vtec_pp = vptr->data.pvtec_hdr.phenomena.Left(2);
	  gxString vtec_sig = vptr->data.pvtec_hdr.significance.Left(1);
	  gxString vtec_etn = vptr->data.EventTrackingNumber(); 
	  gxString vtec_pil;
	  vtec_pil << clear << vtec_pp << "." << vtec_sig;

	  if((vtec_action == "ROU") && (drop_routine_products)) {
	    NT_print("Skipping routine product", product.c_str(), vtec_pil.c_str());
	    vptr = vptr->next;    
	    continue;
	  }

	  if(vtec_action == "UPG") {
	    gxListNode<VTEC> *next_node = vptr->next;
	    if(next_node) {
	      gxString vtec_action_next = next_node->data.pvtec_hdr.action.Left(3);
	      gxString vtec_pp_next = next_node->data.pvtec_hdr.phenomena.Left(2);
	      gxString vtec_sig_next = next_node->data.pvtec_hdr.significance.Left(1);

	      if(vtec_pp == vtec_pp_next) { 
		// 05/13/2008: Added EXA and EXB
		if((vtec_action_next == "NEW") || (vtec_action_next == "EXA") || (vtec_action_next == "EXB")) { 

		  // This watch has been upgraded
		  NT_print(product.c_str(), vtec_pil.c_str(), "has been upgraded");

		  // We must drop the previous version by adding a dummy node to the remove list
		  UGCSegmentListNode ugc_node_dummy = ugc_node; 
		  ugc_node_dummy.etn = vtec_etn;
		  ugc_node_dummy.pil << clear << vtec_pp << vtec_sig;
		  sptr = uptr->data.ugc_hdr.zone_list.GetHead();
		  while(sptr) {
		    ugc_node_dummy.zones.Add(sptr->data);
		    sptr = sptr->next;	  
		  }
		  Warning_table.ugc_segment_UPG_remove_list.Add(ugc_node_dummy);
		  vptr = vptr->next;
		  continue;
		}
	      }
	    }
	  }

	  if(uptr->data.ResetIsExpired(vptr->data)) {
	    ugc_node.is_expired = uptr->data.ugc_hdr.is_expired;
	  }

	  if(!FindProductTableProduct(product, vtec_pil, product_table_data)) {
	    NT_print("Product table missing", product.c_str(), vtec_pil.c_str());
	    if(drop_unknown_products) {
	      NT_print("Warning zone for this product will report none");
	      vptr = vptr->next;    
	      continue;
	    }
	    NT_print("Will try and match to VTEC header");
	    if(!FindProductTableVTECPil(vtec_pil, product_table_data)) {
	      NT_print("Product table missing VTEC product", product.c_str(), vtec_pil.c_str());
	      NT_print("Searching the VTEC table");
	      gxString title;
	      if(FindInVTECTable(vptr->data.Phenomena(), vptr->data.Significance(), title)) {
		NT_print("Found in VTEC table", title.c_str());
		product_table_data.pil = product;
		product_table_data.vtec_pil = vtec_pil;
		product_table_data.office = WarningTableCWA;
		if(vptr->data.Significance() == "W") {
		  // Watch
		  product_table_data.replace_pil = "WWW";
		  product_table_data.fuse = "0"; 
		  product_table_data.priority = "1"; 
		}
		else if(vptr->data.Significance() == "A") {
		  // Warning
		  product_table_data.replace_pil = "AAA";
		  product_table_data.fuse = "1"; 
		  product_table_data.priority = "2"; 
		}
		else {
		  // Advisory
		  product_table_data.replace_pil = "YYY";
		  product_table_data.fuse = "1"; 
		  product_table_data.priority = "3"; 
		}
		product_table_data.sas = "1"; 
		product_table_data.name = title;
	      }
	      else {
		NT_print("Warning zone for this product will report none");
		vptr = vptr->next;    
		continue;
	      }
	    }
	  }

	  ugc_node.table_data = product_table_data; 
	  ugc_node.icon_link.priority = product_table_data.priority.Atoi();

	  ugc_node.pil = product_table_data.replace_pil; 


	  ugc_node.etn = vptr->data.EventTrackingNumber(); 
 
	  segptr = uptr->data.ugc_segment_text.GetHead();
	  while(segptr) {
	    ugc_node.text.Add(segptr->data);
	    segptr = segptr->next;
	  }
	  sptr = uptr->data.ugc_hdr.zone_list.GetHead();
	  while(sptr) {
	    ugc_node.zones.Add(sptr->data);
	    sptr = sptr->next;	  
	  }
	  if(vptr->data.BeginTime() > 1) {
	    ugc_node.begin_time = vptr->data.BeginTime();
	  }
	  if(vtec_action == "CAN") {
	    ugc_node.end_time = uptr->data.ugc_hdr.ugc_time.elapsed_time;
	  }
	  else {
	    if(vptr->data.EndTime() > 1) {
	      ugc_node.end_time = vptr->data.EndTime();
	    }
	  }

	  // 08/18/2008: Check the current table
	  // 08/19/2008: Find and existing products that are new or old
	  UGCSegmentListNode ugc_node_buf_y;
	  int ignore_segment_number_y = 1;
	  if(Warning_table.FindSegmentNode(ugc_node, ugc_node_buf_y, ignore_segment_number_y)) {
	    gxString piletn;
	    if(ugc_node.issue_time.elapsed_time > ugc_node_buf_y.issue_time.elapsed_time) {
	      // This is the most current product so remove the one in the list
	      piletn << clear << ugc_node_buf_y.pil << ugc_node_buf_y.etn;
	      NT_print(piletn.c_str(), "found update to product already in table");
	      Warning_table.ugc_segment_OLD_remove_list.Add(ugc_node_buf_y);
	      // TODO: Remove 06/23/3009 after testing
	      // vptr = vptr->next;
	      // continue;
	    }
	    else if(ugc_node_buf_y.issue_time.elapsed_time > ugc_node.issue_time.elapsed_time) {
	      piletn << clear << ugc_node.pil << ugc_node.etn;
	      NT_print(piletn.c_str(), "found product already in table");
	      vptr = vptr->next;
	      continue;
	    }
	    else {
	      // Continue processing
	    } 
	  }
	
	  // 08/19/2008: Remove CAN products
	  if(((ugc_node.is_expired) && (WarningTableArchiveMode == "0")) || 
	     ((vtec_action == "CAN") && (WarningTableArchiveMode == "0"))) {
	    gxString piletn;
	    // 08/17/2008: Check the current table
	    UGCSegmentListNode ugc_node_buf_x;
	    int ignore_segment_number_x = 1;
	    if(Warning_table.FindSegmentNode(ugc_node, ugc_node_buf_x, ignore_segment_number_x)) {
	      if(!ugc_node_buf_x.is_expired) {
		piletn << clear << ugc_node.pil << ugc_node.etn;
		NT_print(piletn.c_str(), "found more currnet product already in table");
		vptr = vptr->next;
		continue;
	      }
	    }

	    // 05/15/2008: Remove any old products from the list
	    piletn << clear << ugc_node.pil << ugc_node.etn << "-" << segment_number;
	    NT_print(piletn.c_str(), "has expired and will not be included");
	    Warning_table.ugc_segment_OLD_remove_list.Add(ugc_node);
	    vptr = vptr->next;
	    continue;
	  }

	  // 06/10/2008: Skip any product that was previously canceled, expired, or removed
	  int skip_prod = 0;
	  gxListNode<UGCSegmentListNode> *rlptr_ck = Warning_table.ugc_segment_OLD_remove_list.GetHead();
	  while(rlptr_ck) {
	    if(ugc_node.pil == rlptr_ck->data.pil) {
	      if(ugc_node.etn == rlptr_ck->data.etn) {
		skip_prod = 1;
		// 08/17/2008: Check to see if this is a newer version
		if(!ugc_node.is_expired) {
		  // 08/19/2008: Check the time
		  if(ugc_node.issue_time.elapsed_time >= rlptr_ck->data.issue_time.elapsed_time) {
		    gxString piletn;
		    piletn << clear << ugc_node.pil << ugc_node.etn;
		    NT_print(piletn.c_str(), "found expired version and replacing with correct version");
		    skip_prod = 0;
		    int has_bad_prod = 1;
		    Warning_table.ugc_segment_OLD_remove_list.Remove(rlptr_ck);
		    while(has_bad_prod) {
		      rlptr_ck = Warning_table.ugc_segment_OLD_remove_list.GetHead();
		      int found_bad_prod = 0;
		      while(rlptr_ck) {
			if(ugc_node.pil == rlptr_ck->data.pil) {
			  if(ugc_node.etn == rlptr_ck->data.etn) {
			    Warning_table.ugc_segment_OLD_remove_list.Remove(rlptr_ck);
			    found_bad_prod = 1;
			    NT_print(piletn.c_str(), "removing from expired list");
			    break;
			  }
			}
		      rlptr_ck = rlptr_ck->next;
		      }
		      if(!found_bad_prod) has_bad_prod = 0;
		    }
		  }
		}
		break;
	      }
	    }
	    rlptr_ck = rlptr_ck->next;
	  }
	  if(skip_prod) {
	    gxString piletn;
	    piletn << clear << ugc_node.pil << ugc_node.etn << "-" << segment_number;
	    NT_print(piletn.c_str(), "was previously canceled or has expired and will not be included");
	    vptr = vptr->next;
	    continue;
	  }

	  // Create a unique ETN for products with multiple segments
	  // Updates can also be issued in other products
	  ugc_node.etn << "S" << segment_number;

	  UGCSegmentListNode ugc_node_buf;

	  int ignore_segment_number = 0;
	  int update_all = 1;


	  if((vtec_action == "EXA") || (vtec_action == "EXB")) { 
	    sbuf << clear << "Extending areas for " << ugc_node.pil << " " << vptr->data.EventTrackingNumber()
		 << " " << vtec_action;
	    NT_print(sbuf.c_str());
	  }
	  else {
	    if(Warning_table.FindSegmentNode(ugc_node, ugc_node_buf, 1)) {

	      gxString org_list_etn = ugc_node_buf.etn.Left(4);
	      gxString org_node_etn = ugc_node.etn.Left(4);
	      
	      if(org_list_etn == org_node_etn) {
		
		gxString vtec_action_list_node = ugc_node_buf.vtec.pvtec_hdr.action.Left(3);
		
		gxListNode<gxString> *list_node_zptr = ugc_node_buf.zones.GetHead();
		int has_affected_zones = 0;
		while(list_node_zptr) {
		  gxListNode<gxString> *node_zptr = ugc_node.zones.GetHead();
		  while(node_zptr) {
		    if(list_node_zptr->data == node_zptr->data) {
		      has_affected_zones++;
		      break;
		    }
		    node_zptr = node_zptr->next;
		  }
		  list_node_zptr = list_node_zptr->next;
		}
		
		if(has_affected_zones > 0) { 
		  // Ignore the segment number
		  ugc_node.etn = ugc_node_buf.etn;
		  ignore_segment_number = 1;
		}
	      } 
	    } 
	  }

	  if(Warning_table.FindSegmentNode(ugc_node, ugc_node_buf, ignore_segment_number)) {
	    // We already have this VTEC etn so find the current product
	    gxString currnode_vtec_action = ugc_node_buf.vtec.pvtec_hdr.action.Left(3);
	    dropnode = 0;
	    update_node = 0;

	    // We must check the VTEC times and action codes here
	    if(vtec_action == "NEW") { // Keep the updated action in the list
	      dropnode = 1;
	    }
	    if((currnode_vtec_action == "CAN") || (currnode_vtec_action == "EXP")) {
	      // We already have canceled or expired product in the list
	      dropnode = 1; 
	    }
	    if(ugc_node_buf.begin_time > ugc_node.begin_time) {
	      dropnode = 1;
	    }
	    if(dropnode) { // Signaled not to process this node
	      sbuf << clear << "Have current " << ugc_node.pil << " " << vptr->data.EventTrackingNumber() 
		   << " dropping " << vtec_action << " keeping " << currnode_vtec_action;
	      NT_print(sbuf.c_str());
	      
	      vptr = vptr->next;    
	      continue;
	    }

	    if(currnode_vtec_action == "NEW") { 
	      update_node = 1;
	    }
	    if(vtec_action == "CON") { // 05/13/2008: Get the last segment
	      update_node = 1;
	    }
	    if((vtec_action == "CAN") || (vtec_action == "EXP") || (vtec_action == "EXT")) {
	      update_node = 1;
	    }
	    if(ugc_node.begin_time > ugc_node_buf.begin_time) {
	      update_node = 1;
	    }
	    if(update_node) {
	      sbuf << clear << "Have current " << ugc_node.pil << " " << vptr->data.EventTrackingNumber() 
		   << " updating " << currnode_vtec_action << " to " << vtec_action;
	      NT_print(sbuf.c_str());
	    }
	  } 

	  int has_google_map_popup = 0;
	  if(make_google_map_page) {
	    if(uptr->data.polygon_warning.polygon_coords.IsEmpty()) {
	      if(debug) {
		// Not all VTEC products have polygons
		NT_print("UGC segment does not contain any polygon coordinates");
		sbuf << clear << "Skipping Google map page for UGC segment " 
		     << segment_number << " " << ugc_node.pil <<  ugc_node.etn;
		NT_print(sbuf.c_str());
	      }
	    }
	    else { // Use JavaScript popup windows for Google earth maps
	      if(use_google_map_js_popup) {
		has_google_map_popup = MakeJSGoogleMapPopupProduct(ugc_node.jscode, text_product,
								   uptr->data, segment_number,
								   ugc_node.pil, ugc_node.etn, 
								   ugc_node.name);
		if(has_google_map_popup) {
		  if(debug) {
		    sbuf << clear << " " << ugc_node.pil <<  ugc_node.etn; 
		    NT_print("Making Google map JavaScript popup", sbuf.c_str());
		  }
		  ugc_node.MakeTitleString();
		  MakeJSProductIconLink(&ugc_node);
		}
	      }
	      else { // Write a Web page for each google map page
		gxString fname, pagefilename;
		has_google_map_popup = MakeJSGoogleMapFile(text_product, uptr->data, 
							   segment_number, pagefilename, 
							   fname);
		if(has_google_map_popup) {
		  if(debug) {
		    NT_print("Linking Google map to URL", pagefilename.c_str());
		  }
		  ugc_node.MakeTitleString();

		  // 01/15/2009: Add a page title to a page request string
		  gxString title_request_var = ugc_node.icon_link.title;
		  title_request_var.DeleteAfter("warning");
		  title_request_var.DeleteAfter("Warning");
		  title_request_var.TrimTrailingSpaces();
		  title_request_var.ReplaceString(" ", "%20");

		  // 07/16/2008: Update the link to include the virtual Web server root path
		  if(!GoogleMapsWebServerLocation.is_null()) {
		    gxString new_pagefilename;
		    gxString old_pagefilename = pagefilename;
		    new_pagefilename << clear << GoogleMapsWebServerLocation;
		    new_pagefilename.TrimTrailing('/');
		    old_pagefilename.DeleteBeforeLastIncluding("/");
		    // 01/14/2009: CMS product request script
		    if(!GoogleMapsWebServerRequestScript.is_null()) {
		      gxString reqscript = GoogleMapsWebServerRequestScript;
		      reqscript.TrimLeading('/');
		      reqscript.TrimTrailing('/');
		      reqscript.DeleteBeforeLastIncluding("/");
		      old_pagefilename.DeleteAfterLastIncluding(".");
		      reqscript << "?" << GoogleMapsWebServerRequestVar << "=" << old_pagefilename;
		      if(!GoogleMapsWebServerTitleVar.is_null()) {
			if((GoogleMapsWebServerTitleVar != "NONE") || (GoogleMapsWebServerTitleVar != "none")) {
			  reqscript << "&" << GoogleMapsWebServerTitleVar << "=" << title_request_var;
			}
		      }
		      old_pagefilename = reqscript; 
		    }
		    new_pagefilename << "/" << old_pagefilename;
		    MakeJSFileIconLink(ugc_node.icon_link.link, ugc_node.pil, new_pagefilename, 
				       ugc_node.icon_link.title);
		  }
		  else if(!GoogleMapsWebServerRequestScript.is_null()) {
		    // 01/14/2009: CMS product request script
		    gxString new_pagefilename;
		    gxString old_pagefilename = pagefilename;
		    old_pagefilename.DeleteBeforeLastIncluding("/");
		    gxString reqscript = GoogleMapsWebServerRequestScript;
		    reqscript.TrimTrailing('/');
		    old_pagefilename.DeleteAfterLastIncluding(".");
		    reqscript << "?" << GoogleMapsWebServerRequestVar << "=" << old_pagefilename;
		    if(!GoogleMapsWebServerTitleVar.is_null()) {
		      if((GoogleMapsWebServerTitleVar != "NONE") || (GoogleMapsWebServerTitleVar != "none")) {
			reqscript << "&" << GoogleMapsWebServerTitleVar << "=" << title_request_var;
		      }
		    }
		    new_pagefilename << reqscript; 
		    MakeJSFileIconLink(ugc_node.icon_link.link, ugc_node.pil, new_pagefilename, 
				       ugc_node.icon_link.title);
		  }
		  else {
		    MakeJSFileIconLink(ugc_node.icon_link.link, ugc_node.pil, pagefilename, 
				       ugc_node.icon_link.title);
		  }
		}
	      }
	    }
	  }

	  if(!has_google_map_popup) {
	    if(debug) {
	      sbuf << clear << " " << ugc_node.pil <<  ugc_node.etn; 
	      NT_print("Making JavaScript text product popup", sbuf.c_str());
	    }
	    MakeJSPopupProduct(&ugc_node);
	  }

	  if(update_node) {
	    Warning_table.UpdateSegmentNode(ugc_node, ignore_segment_number, update_all);
	  }
	  else {
	    Warning_table.AddSegmentNode(ugc_node);
	  }

	  vptr = vptr->next;
	} // At the end of the VTEC list

	// Break after all VTEC segments have been processed
	uptr = uptr->next;
	continue;
      } 

      // Process the non-VTEC product here
      gxString dummy_vtec_pil = product;
      dummy_vtec_pil.InsertAt(2, ".");

      NT_print("Non-VTEC product", product.c_str());
      NT_print("Will search product table for dummy VTEC pil", dummy_vtec_pil.c_str());
      if(!FindProductTableProduct(product, dummy_vtec_pil, product_table_data)) {
	NT_print("Product table missing", product.c_str(), dummy_vtec_pil.c_str());
	if(drop_unknown_products) {
	  NT_print("Warning zone for this product will report none");
	  uptr = uptr->next;
	  continue;
	}
	NT_print("Will try and match to dummy VTEC header only");
	if(!FindProductTableVTECPil(dummy_vtec_pil, product_table_data)) {
	  NT_print("Product table missing dummy VTEC product", product.c_str(), dummy_vtec_pil.c_str());
	  NT_print("Searching the VTEC table");
	  gxString title;
	  gxString pp_code = dummy_vtec_pil.Left(2);
	  gxString sig_code = dummy_vtec_pil.Right(1);
	  if(FindInVTECTable(pp_code, sig_code, title)) {
	    NT_print("Found in VTEC table", title.c_str());
	    product_table_data.pil = product;
	    product_table_data.vtec_pil = dummy_vtec_pil;
	    product_table_data.office = WarningTableCWA;
	    if(sig_code == "W") {
	      // Watch
	      product_table_data.replace_pil = "WWW";
	      product_table_data.fuse = "0"; 
	      product_table_data.priority = "1"; 
	    }
	    else if(sig_code == "A") {
	      // Warning
	      product_table_data.replace_pil = "AAA";
	      product_table_data.fuse = "1"; 
	      product_table_data.priority = "2"; 
	    }
	    else {
	      // Advisory
	      product_table_data.replace_pil = "YYY";
	      product_table_data.fuse = "1"; 
	      product_table_data.priority = "3"; 
	    }
	    product_table_data.sas = "1"; 
	    product_table_data.name = title;
	  }
	  else {
	    NT_print("Warning zone for this product will report none");
	    uptr = uptr->next;
	    continue;
	  }
	}
      }

      ugc_node.table_data = product_table_data; 
      ugc_node.icon_link.priority = product_table_data.priority.Atoi();
      ugc_node.pil = product_table_data.pil; 
      
      segptr = uptr->data.ugc_segment_text.GetHead();
      while(segptr) {
	ugc_node.text.Add(segptr->data);
	segptr = segptr->next;
      }
      sptr = uptr->data.ugc_hdr.zone_list.GetHead();
      while(sptr) {
	ugc_node.zones.Add(sptr->data);
	sptr = sptr->next;	  
      }

      if((ugc_node.is_expired) && (WarningTableArchiveMode == "0")) {
	gxString piletn;
	piletn << clear << ugc_node.pil << ugc_node.etn << "-" << segment_number;
	NT_print(piletn.c_str(), "has expired and will not be included");
	uptr = uptr->next;
	continue;
      }

      // 06/09/2008: Added tracking feature for non-vtec products
      // 06/09/2008: All non-vtec products use a dummy ETN so we must look for duplicate products
      // 06/09/2008: and with a new popup with incremented dummy ETN. 
      gxListNode<UGCSegmentListNode> *usnptr_table = Warning_table.ugc_segment_nodes.GetHead();
      while(usnptr_table) {
	if(usnptr_table->data.pil == ugc_node.pil) {
	  gxListNode<gxString> *seg_text_ptr;
	  gxListNode<gxString> *seg_zone_ptr;
	  gxString list_etn = usnptr_table->data.etn.Left(4);
	  gxString zones_list;
	  seg_zone_ptr = usnptr_table->data.zones.GetHead();
	  while(seg_zone_ptr) {
	    zones_list << seg_zone_ptr->data << " ";
	    seg_zone_ptr = seg_zone_ptr->next;
	  }
	  gxString zones_node;
	  seg_zone_ptr = ugc_node.zones.GetHead();
	  while(seg_zone_ptr) {
	    zones_node << seg_zone_ptr->data << " ";
	    seg_zone_ptr = seg_zone_ptr->next;
	  }

	  if(zones_list == zones_node) {
	    // This is a follow-up to the same PIL so
	    // use the latest one issued
	    ugc_node.etn << clear << list_etn;
	    usnptr_table = usnptr_table->next;
	    break;
	  }

	  int curr_etn = list_etn.Atoi();
	  gxString plist_text;
	  seg_text_ptr = usnptr_table->data.text.GetHead();
	  while(seg_text_ptr) {
	    plist_text << seg_text_ptr->data << "\n";
	    seg_text_ptr = seg_text_ptr->next;
	  }
	  gxString p_text;
	  seg_text_ptr = ugc_node.text.GetHead();
	  while(seg_text_ptr) {
	    p_text << seg_text_ptr->data << "\n";
	    seg_text_ptr = seg_text_ptr->next;
	  }

	  if(plist_text != p_text) {
	    curr_etn++;
	    gxString etn_buf;
	    etn_buf.Clear();
	    etn_buf.SetFill('0');
	    etn_buf.SetWidth(4);
	    etn_buf << curr_etn;
	    ugc_node.etn << clear << etn_buf;
	  }
	}
	usnptr_table = usnptr_table->next;
      }

      ugc_node.etn << "S" << segment_number;
      MakeJSPopupProduct(&ugc_node);

      Warning_table.AddSegmentNode(ugc_node);

      // Advance to the next UGC segment
      uptr = uptr->next;
    }

    if(debug) NT_print("Processing complete");
    lptr = lptr->next;
  }
  
  if(!Warning_table.BuildZoneTable()) {
    NT_print("Error building the warning table");
    NT_print("Could not build warning lising by zone and county codes");
    return 0;
  }

  if(debug) NT_print("Building the warning table complete");
  return 1;
}

int add_key_table_icons(gxString &table, const gxString &images_dir, const gxString priority)
{
  gxListNode<ProductTableData> *ptptr = Product_table.GetHead();
  gxString imgtag;
  while(ptptr) {
    if(ptptr->data.priority == priority) {

      gxListNode<ProductTableData> *prev_ptptr = ptptr->prev;
      if(prev_ptptr) {
	if(ptptr->data.replace_pil == prev_ptptr->data.replace_pil) {
	  if(debug) NT_print("Skipping duplicate key table entry", ptptr->data.replace_pil.c_str()); 
	  ptptr = ptptr->next;
	  continue;
	}
      }
      imgtag << clear << "<img src=\"" << images_dir << "/" << ptptr->data.replace_pil
	     << ".gif" << "\" alt=\"" << ptptr->data.replace_pil << "\""
	     << " border=\"0\" align=\"absmiddle\" height=\"" 
	     << KeyTableIconHeight 
	     << "\" width=\""<< KeyTableIconWidth << "\" hspace=\"0\"" 
	     << " title=\"" << ptptr->data.replace_pil << "\">";
      table << imgtag << "&nbsp;" << ptptr->data.name << "<br>" << "\n";
    }
    ptptr = ptptr->next;
  }
  return 1;
}

int gen_key_table(gxString &table)
{
  table.Clear();

  gxString sbuf, images_dir, priority;

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

  int td_width_int = KeyTableWidth.Atoi() / 3;
  gxString td_width;
  td_width << clear << td_width_int; 
  if(KeyTableTDWidth != "AUTO") {
    td_width = KeyTableTDWidth;     
  }

  table << "<table width=\"" 
	<< KeyTableWidth 
	<< "\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\" bgcolor=\"" 
	<< KeyTableBGColor 
	<< "\">"
	<< "\n";

  table << "<tr align=\"center\">" << "\n";
  table << "<td width=\"" << td_width << "\" bgcolor=\"" 
	<< KeyTableTitleBGColor << "\" style=\"color:" << KeyTableTitleFontColor 
	<< "\"><font style=\"font-size: " << KeyTableTitleFontSize << "\">" 
	<< KeyTableWarningTitle << "</font></td>" << "\n";

  table << "<td width=\"" << td_width << "\" bgcolor=\"" 
	<< KeyTableTitleBGColor << "\" style=\"color:" << KeyTableTitleFontColor 
	<< "\"><font style=\"font-size: " << KeyTableTitleFontSize << "\">" 
	<< KeyTableWatchTitle << "</font></td>" << "\n";

  table << "<td width=\"" << td_width << "\" bgcolor=\"" 
	<< KeyTableTitleBGColor << "\" style=\"color:" << KeyTableTitleFontColor 
	<< "\"><font style=\"font-size: " << KeyTableTitleFontSize << "\">" 
	<< KeyTableAdvisoryTitle << "</font></td>" << "\n";
  table << "</tr>" << "\n";

  table << "<tr valign=\"top\">" << "\n";

  table << "<td style=\"font-size:" << KeyTableIconNameFontSize << "; padding:2px; style=\"color:" 
	<< KeyTableIconNameFontColor << "; \" width=\"" << td_width << "\">" << "\n";
  priority = "1";
  add_key_table_icons(table, images_dir, priority);
  table << "</td>" << "\n";

  table << "<td style=\"font-size:" << KeyTableIconNameFontSize << "; padding:2px; style=\"color:" 
	<< KeyTableIconNameFontColor << "; \" width=\"" << td_width << "\">" << "\n";
  priority = "2";
  add_key_table_icons(table, images_dir, priority);
  table << "</td>" << "\n";

  table << "<td style=\"font-size:" << KeyTableIconNameFontSize << "; padding:2px; style=\"color:" 
	<< KeyTableIconNameFontColor << "; \" width=\"" << td_width << "\">" << "\n";
  priority = "3";
  add_key_table_icons(table, images_dir, priority);
  table << "</td>" << "\n";

  table << "</tr>" << "\n";
  table << "</table>" << "\n";

  return 1;
}

int gen_ver_table(gxString &table)
// Function used to generate a verification table.
// Table is passed back in the table string argument.
// Returns 1 if successful. Returns 0 if an error occurs.
{
  table.Clear();
  gxString sbuf;

  if(VerTableWidth == "AUTO") {
    table << "<table cellspacing=\"0\" cellpadding=\"0\" border=\""<< VerTableBorder 
	  << "\" valign=\"top\">" 
	  << "\n";
  }
  else {
    table << "<table cellspacing=\"0\" cellpadding=\"0\" border=\""<< VerTableBorder 
	  << "\" width=\"" << VerTableWidth << "\" valign=\"top\">" 
	  << "\n";
  }

  gxString curr_time_string;
  if(VerTablePrintUpdateTime == "1") {
    if(!MakeWarningTableTimeString(curr_time_string)) {
      NT_print("Error getting local system time");
      if(debug) curr_time_string = "System time error";
    }
  }
  if(curr_time_string.is_null()) {
    if(VerTablePrintUpdateTime == 1) {
      NT_print("Error NULL system time");
      if(debug) {
	curr_time_string = "NULL system time";
      }
    }
  }

  table << "<tr valign=\"top\">" << "\n";
  table << "<td bgcolor=\"" << VerTableTitleBGColor 
	  << "\" colspan=\"2\" align=\"center\" style=\"color:" 
	  << VerTableTitleFontColor << "\">" << "\n";
  table << "<font style=\"font-size: " << VerTableTitleFontSize << "\">" << "\n"
	  << "&nbsp;" << VerTableTitle << "&nbsp;" << "\n";
  
  if(!curr_time_string.is_null()) {
    table << "&nbsp;" << curr_time_string << "&nbsp;"<< "\n";
  }
  table << "</font>" << "\n";
  
  if(!VerTableHeaderLink.is_null()) {
    if(VerTableHeaderLink != "NONE") {
      table << "&nbsp;" << VerTableHeaderLink << "&nbsp;" << "\n";
    }
  }
  table << "</td>" << "\n";
  table << "</tr>" << "\n";

  gxListNode<CurrentWarningZoneNode> *zone_node = Warning_table.current_warnings_by_zone.GetHead();
  while(zone_node) {
    if(!zone_node->data.product_linked_icons.IsEmpty()) {
      table << "<tr>" << "\n";

      table << "<td bgcolor=\"" << VerTableZoneNameBGColor 
	    << "\" style=\"padding-left:1px;style=\"padding-right:1px;border-top:solid 1px;\""
    	    << " onMouseOver=\"this.bgColor = \'" << VerTableZoneNameMouseOver 
	    << "\'\" onMouseOut = \"this.bgColor = \'" 
	    << VerTableZoneNameBGColor << "\'\""; 
	
      if(VerTableZoneNameTDWidth == "AUTO") {
	table << ">";
      }
      else {
	table << " width=\"" << VerTableZoneNameTDWidth <<"\">";
      }

      gxString myzone_name = zone_node->data.name;

      if(!zone_node->data.table_data.zone_url.is_null()) {
	if(zone_node->data.table_data.zone_title.is_null()) {
	  zone_node->data.table_data.zone_title = zone_node->data.name; 
	}
	myzone_name << clear << "<a href=\"" << zone_node->data.table_data.zone_url
		    << "\" title=\"" << zone_node->data.table_data.zone_title
		    << "\" target=\"_blank\">" << zone_node->data.name << "</a>";
      }

      table << "\n";
      table << "<font style=\"font-size: " << VerTableZoneNameFontSize 
	    << "\" color=\"" << VerTableZoneNameFontColor << "\">" << "\n"
	    <<  myzone_name << "\n"
	    << "</font>" << "\n"
	    << "</td>" << "\n";
      table << "<td bgcolor=\"" << VerTableProductBGColor 
	    << "\" style=\"padding-left:2px;style=\"padding-right:2px;\"";
      if(VerTableProductTDWidth == "AUTO") {
	table << ">";
      }
      else {
	table << " width=\"" << VerTableProductTDWidth <<"\">";
      }
      table << "\n";
      gxListNode<IconLink> *ilptr = zone_node->data.product_linked_icons.GetHead();
      while(ilptr) {
	table 	<< "<font color=\"" << VerTableProductFontColor 
		<< "\" style=\"font-size: " << VerTableProductFontSize << "\"" 
		<< ">";
	sbuf << clear << ilptr->data.link;

	if(!add_ver_table_in_page) { // Cannot have links in POPUP JS Window
	  sbuf.DeleteBeforeIncluding(">\n"); // Dump the <a href="..."> tag
	  sbuf.FilterString("</a>");
	  sbuf.DeleteAfterIncluding(" title=");
	  sbuf << ">";
	}

	table << sbuf << "&nbsp;-&nbsp;" << ilptr->data.title << "<br>";
	table << "</font>";
 	table << "\n";
	ilptr = ilptr->next;
      }
      table << "</td>" << "\n";

      table << "</tr>" << "\n";
    }
    zone_node = zone_node->next;
  }

  // End of area name table
  table << "</table>" << "\n";

  return 1;
}

int gen_ver_worksheets()
// Function used to generate a verification worksheets for warnings.
// Table is passed back in the table string argument.
// Returns 1 if successful. Returns 0 if an error occurs.
{
  gxString sheet;
  gxString fname;

  gxListNode<UGCSegmentListNode> *usnptr = Warning_table.ugc_segment_nodes.GetHead();

  while(usnptr) {

    int is_short_fused = 0;
    gxListNode<ProductTableData> *ptptr = Product_table.GetHead();
    while(ptptr) {
      if(ptptr->data.replace_pil == usnptr->data.pil) {
	if(ptptr->data.fuse == "0") {
	  gxString vtec_sig = usnptr->data.vtec.pvtec_hdr.significance.Left(1);
	  if(vtec_sig == "W") {
	    is_short_fused = 1;
	    break;
	  }
	}
      }
      ptptr = ptptr->next;
    }

    if(!is_short_fused) {
      usnptr = usnptr->next;
      continue;
    }

    if(usnptr->data.is_expired) {
      if(force_worksheet_test) {
	NT_print("Writing worksheet file for expired product", usnptr->data.pil.c_str()); 
      }
      else {
	usnptr = usnptr->next;
	continue;
      }
    }

    fname << clear << usnptr->data.pil << WarningTableCWA << "-worksheet.txt";
    if(!overwrite_output_worksheet) insert_time_stamp(fname);

    sheet.Clear();
    sheet << "Warning Type" << "\n";
    sheet << "------------" << "\n";
    sheet << usnptr->data.pil << "\n";
    sheet << "\n";
    sheet << "Valid Time" << "\n"; 
    sheet << "----------" << "\n";
    sheet << usnptr->data.icon_link.title << "\n";
    sheet << "\n";
    sheet << "County(ies)" << "\n";
    sheet << "-----------" << "\n";

    gxListNode<gxString> *sptr;
    sptr = usnptr->data.zones.GetHead();
    int count = 0;
    int max_count = 0;
    while(sptr) {
      gxListNode<ZoneTableData> *zptr = Zone_table.GetHead();
      while(zptr) {
	if(zptr->data.zone_number == sptr->data) {
	  count++;
	  sheet << "(" << count << ".) " << zptr->data.zone_name << " ";
	  max_count++;
	  if(max_count >= 4) {
	    max_count = 0;
	    sheet << "\n";
	  }
	}
	zptr = zptr->next;
      }
      sptr = sptr->next;
    }
    if(sheet[sheet.length()-1] != '\n') sheet << "\n";
    sheet << "\n";

    sheet << "Verification" << "\n";
    sheet << "------------" << "\n";
    sheet << "      Damage                                                           Contacts" << "\n";
    sheet << "      ------                                                           --------" << "\n";
    sheet << "\n";
    sheet << "1. ________________________________________________________________    _______________________________" << "\n";
    sheet << "\n";
    sheet << "   ________________________________________________________________    _______________________________" << "\n";
    sheet << "\n";
    sheet << "   ________________________________________________________________    _______________________________" << "\n";
    sheet << "\n";
    sheet << "2. ________________________________________________________________    _______________________________" << "\n";
    sheet << "\n";
    sheet << "   ________________________________________________________________    _______________________________" << "\n";
    sheet << "\n";
    sheet << "   ________________________________________________________________    _______________________________" << "\n";
    sheet << "\n";
    sheet << "3. ________________________________________________________________    _______________________________" << "\n";
    sheet << "\n";
    sheet << "   ________________________________________________________________    _______________________________" << "\n";
    sheet << "\n";
    sheet << "   ________________________________________________________________    _______________________________" << "\n";
    sheet << "\n";
    sheet << "\n";
    sheet << "Verified: YES / NO     By: _______________________     LSR Issued: YES / NO" << "\n";
    sheet << "\n";
    sheet << "\n";
    sheet << "* Use reverse side for further details" << "\n";
    sheet << "* Attach related statements" << "\n";
    sheet << "\n";

    if(!WriteWorksheet(fname, sheet)) return 0;

    usnptr = usnptr->next;
  }

  return 1;
}

int gen_short_fused_table(gxString &table)
// Function used to generate a short fused warning table.
// Table is passed back in the table string argument.
// Returns 1 if successful. Returns 0 if an error occurs.
{
  table.Clear();
  gxString sbuf;

  int num_warnings = 0;
  int total_num_polygons_links = 0;

  gxListNode<UGCSegmentListNode> *usnptr;
  usnptr = Warning_table.ugc_segment_nodes.GetHead();
  while(usnptr) {
    gxListNode<ProductTableData> *ptptr = Product_table.GetHead();
    while(ptptr) {
      if(ptptr->data.replace_pil == usnptr->data.pil) {
	if(ptptr->data.fuse == "0") {
	  gxString vtec_sig = usnptr->data.vtec.pvtec_hdr.significance.Left(1);
	  if(vtec_sig == "W") {
	    num_warnings++;
	    break;
	  }
	}
      }
      ptptr = ptptr->next;
    }
    usnptr = usnptr->next;
  }


  if(ShortFusedTableWidth == "AUTO") {
    table << "<table cellspacing=\"0\" cellpadding=\"0\" border=\""<< ShortFusedTableBorder 
	  << "\" valign=\"top\">" 
	  << "\n";
  }
  else {
    table << "<table cellspacing=\"0\" cellpadding=\"0\" border=\""<< ShortFusedTableBorder 
	  << "\" width=\"" << ShortFusedTableWidth << "\" valign=\"top\">" 
	  << "\n";
  }

  gxString curr_time_string;
  if(ShortFusedTablePrintUpdateTime == "1") {
    if(!MakeWarningTableTimeString(curr_time_string)) {
      NT_print("Error getting local system time");
      if(debug) curr_time_string = "System time error";
    }
  }
  if(curr_time_string.is_null()) {
    if(ShortFusedTablePrintUpdateTime == 1) {
      NT_print("Error NULL system time");
      if(debug) {
	curr_time_string = "NULL system time";
      }
    }
  }

  table << "<tr valign=\"top\">" << "\n";
  table << "<td bgcolor=\"" << ShortFusedTableTitleBGColor 
	  << "\" colspan=\"2\" align=\"center\" style=\"color:" 
	  << ShortFusedTableTitleFontColor << "\">" << "\n";
  table << "<font style=\"font-size: " << ShortFusedTableTitleFontSize << "\">" << "\n"
	  << "&nbsp;" << ShortFusedTableTitle << "&nbsp;" << "\n";
  
  if(!curr_time_string.is_null()) {
    table << "&nbsp;" << curr_time_string << "&nbsp;"<< "\n";
  }
  table << "</font>" << "\n";
  
  if(!ShortFusedTableHeaderLink.is_null()) {
    if(ShortFusedTableHeaderLink != "NONE") {
      table << "&nbsp;" << ShortFusedTableHeaderLink << "&nbsp;" << "\n";
    }
  }
  table << "</td>" << "\n";
  table << "</tr>" << "\n";


  gxListNode<CurrentWarningZoneNode> *zone_node = Warning_table.current_warnings_by_zone.GetHead();
  while(zone_node) {
    if(!zone_node->data.product_linked_icons.IsEmpty()) {
      gxString tbuf;
      int has_polygon_page = 0;

      gxListNode<IconLink> *ilptr = zone_node->data.product_linked_icons.GetHead();
      int num_polygons_pages = 0;
      while(ilptr) {
	if(ilptr->data.link.IFind("<a href=\"#\" onClick=") == -1) {
	  num_polygons_pages++;
	  total_num_polygons_links++;
	}
	ilptr = ilptr->next;
      }

      if(num_polygons_pages == 0) {
	zone_node = zone_node->next;
	continue;
      }

      tbuf << clear << "<tr>" << "\n";
      
      tbuf << "<td bgcolor=\"" << ShortFusedTableZoneNameBGColor 
	    << "\" style=\"padding-left:1px;style=\"padding-right:1px;border-top:solid 1px;\""
	    << " onMouseOver=\"this.bgColor = \'" << ShortFusedTableZoneNameMouseOver 
	    << "\'\" onMouseOut = \"this.bgColor = \'" 
	    << ShortFusedTableZoneNameBGColor << "\'\""; 
      
      if(ShortFusedTableZoneNameTDWidth == "AUTO") {
	tbuf << ">";
      }
      else {
	tbuf << " width=\"" << ShortFusedTableZoneNameTDWidth <<"\">";
      }
      
      gxString myzone_name = zone_node->data.name;

      if(!zone_node->data.table_data.zone_url.is_null()) {
	if(zone_node->data.table_data.zone_title.is_null()) {
	  zone_node->data.table_data.zone_title = zone_node->data.name; 
	}
	myzone_name << clear << "<a href=\"" << zone_node->data.table_data.zone_url
		    << "\" title=\"" << zone_node->data.table_data.zone_title
		    << "\" target=\"_blank\">" << zone_node->data.name << "</a>";
      }

      tbuf << "\n";
      tbuf << "<font style=\"font-size: " << ShortFusedTableZoneNameFontSize 
	    << "\" color=\"" << ShortFusedTableZoneNameFontColor << "\">" << "\n"
	    <<  myzone_name << "\n"
	    << "</font>" << "\n"
	    << "</td>" << "\n";

      tbuf << "<td bgcolor=\"" << ShortFusedTableProductBGColor 
	   << "\" style=\"padding-left:2px;style=\"padding-right:2px;\"";
      if(ShortFusedTableProductTDWidth == "AUTO") {
	tbuf << ">";
      }
      else {
	tbuf << " width=\"" << ShortFusedTableProductTDWidth <<"\">";
      }

      ilptr = zone_node->data.product_linked_icons.GetHead();
      while(ilptr) {
	if(ilptr->data.link.IFind("<a href=\"#\" onClick=") == -1) {

	  tbuf << "\n";
	  tbuf << "<font color=\"" << ShortFusedTableProductFontColor 
	       << "\" style=\"font-size: " << ShortFusedTableProductFontSize << "\"" 
	       << ">";
	  sbuf << clear << ilptr->data.link;
	  has_polygon_page = 1;
	  tbuf << sbuf << "&nbsp;-&nbsp;" << ilptr->data.title << "<br>";
	  tbuf << "</font>";
	  tbuf << "\n";
	}

	ilptr = ilptr->next;
      }
      tbuf << "</td>" << "\n";
      tbuf << "</tr>" << "\n";

      if(has_polygon_page) {
	table << tbuf;
      } 
    }

    zone_node = zone_node->next;
  }

  table << "<tr>" << "\n";
  table << "<td bgcolor=\"" << ShortFusedTableZoneNameBGColor 
	<< "\" style=\"padding-left:1px;style=\"padding-right:1px;border-top:solid 1px;\""
	<< " onMouseOver=\"this.bgColor = \'" << ShortFusedTableZoneNameMouseOver 
	<< "\'\" onMouseOut = \"this.bgColor = \'" 
	<< ShortFusedTableZoneNameBGColor << "\'\"" << " colspan=\"2\">";
  table << "<a href=\"" << ShortFusedTableLinkToWarningTable << "\">"
	<< ShortFusedTableLinkToWarningTableTitle << "</a>" << "\n";
  table << "</td>" << "\n";
  table << "</tr>" << "\n";

  // End of area name table
  table << "</table>" << "\n";

  table << "<!-- Total short fused warnings = " << num_warnings << " -->" 
	<< "\n";
  table << "<!-- Total number of polygon table links = " << total_num_polygons_links << " -->" 
	<< "\n";


  return 1;
}

int gen_rss_feed_all_WWA(gxString &feed)
// Function used to generate a RSS feeds for all WWAs.
// Returns 1 if successful. Returns 0 if an error occurs.
{
  feed.Clear();
  gxRSSfeed rss;
  gxString sbuf;

  if(CustomProductTitle == "NONE") CustomProductTitle = WarningTableTitle;
  if(CustomProductName == "NONE") CustomProductName = WarningTableWebPageName;
  
  if(CustomProductURL == "NONE") CustomProductURL = RSSDefaultURL;
  if(CustomProductDescription == "NONE") CustomProductDescription = WarningTableWebPageName;
  if(!RSSWebmaster.is_null()) rss.channel.webmaster = RSSWebmaster;
  if(!RSSManagingEditor.is_null()) rss.channel.managingeditor = RSSManagingEditor;
  
  if((!RSSImageURL.is_null()) && (!RSSImageLink.is_null()) &&
     (!RSSImageTitle.is_null()) && (!RSSImageDescription.is_null())) {
    rss.channel.image.url = RSSImageURL; 
    rss.channel.image.link = RSSImageLink;
    rss.channel.image.title = RSSImageTitle;
    rss.channel.image.description = RSSImageDescription;
  }

  if((!RSSTextInputTitle.is_null()) && (!RSSTextInputDescription.is_null()) &&
     (!RSSTextInputName.is_null()) && (!RSSTextInputLink.is_null())) {
    rss.channel.textinput.title = RSSTextInputTitle;
    rss.channel.textinput.description = RSSTextInputDescription;
    rss.channel.textinput.name = RSSTextInputName;
    rss.channel.textinput.link = RSSTextInputLink;
  }

  rss.channel.title = CustomProductTitle;
  rss.channel.link = CustomProductURL;
  rss.channel.description = CustomProductDescription;

  SysTime systime;
  gxString issue_time_string = systime.GetGMTDateTime();
  rss.channel.lastbuilddate = issue_time_string;
  rss.channel.pubdate = issue_time_string;
  rss.channel.language = "en-us";
  rss.channel.generator << clear << ProgramDescription << " " << VersionString;
  rss.channel.copyright = "None";

  int guid = 0;

  gxListNode<CurrentWarningZoneNode> *zone_node = Warning_table.current_warnings_by_zone.GetHead();
  while(zone_node) {
    if(!zone_node->data.product_linked_icons.IsEmpty()) {
      gxRSSitem rss_item;
      guid++;
      rss_item.title = zone_node->data.name;

      if(!zone_node->data.table_data.zone_url.is_null()) {
	rss_item.link = zone_node->data.table_data.zone_url;
      }
      else {
	rss_item.link = CustomProductURL;
      }

      rss.channel.lastbuilddate = issue_time_string;
      rss_item.pubdate = issue_time_string;
      rss_item.guid << clear << rss_item.link << "#" << guid;

      rss_item.description << clear << zone_node->data.name << "<br /><br />";
      gxListNode<IconLink> *ilptr = zone_node->data.product_linked_icons.GetHead();
      while(ilptr) {
	gxString linked_icon;
	if(ilptr->data.link.IFind("onClick") != -1) {
	  // This is JavaScript POPUP
	  sbuf << clear << ilptr->data.link;
	  sbuf.DeleteBeforeIncluding(">\n"); // Dump the <a href="..."> tags with JavaScript code
	  sbuf.FilterString("</a>");
	  sbuf.DeleteAfterIncluding(" title=");
	  sbuf << ">";
	  linked_icon << clear << "<a href=\"" << CustomProductURL << "\">";
	  linked_icon << sbuf << "</a>";
	}
	else {
	  if(ilptr->data.link.IFind("http://") == -1) {
	    // This is a link without the full URL
	    sbuf << clear << ilptr->data.link;
	    sbuf.DeleteBeforeIncluding(">\n"); // Dump the <a href="..."> tags unless we know the entire URL
	    sbuf.FilterString("</a>");
	    sbuf.DeleteAfterIncluding(" title=");
	    sbuf << ">";
	    linked_icon << clear << "<a href=\"" << CustomProductURL << "\">";
	    linked_icon << sbuf << "</a>";
	  }
	  else {
	    linked_icon << clear << ilptr->data.link;
	  }
	}
	linked_icon << "&nbsp;-&nbsp;" << ilptr->data.title << "<br>";
	rss_item.description << linked_icon;
	ilptr = ilptr->next;
      }

      rss.channel.items.Add(rss_item);
    }
    zone_node = zone_node->next;
  }

  rss.WriteFeed(feed);

  return 1;
}

int gen_rss_feeds_for_all_zones()
// Function used to generate a RSS feeds for all zones.
// Returns 1 if successful. Returns 0 if an error occurs.
{

  NT_print("Writing WWA RSS feeds for all zones");
  if(rss_output_dir == "DEFAULT") {
    rss_output_dir = WebpageOutputDirectory;
  }

  gxString sbuf, fname;
  int guid = 0;

  if(CustomProductURL == "NONE") CustomProductURL = RSSDefaultURL;

  gxListNode<CurrentWarningZoneNode> *zone_node = Warning_table.current_warnings_by_zone.GetHead();
  while(zone_node) {
    // 06/22/2009: Add the area_number to the file name to handle zone numbers masked to the same area name
    fname << clear << WarningTableCWA << "_" << zone_node->data.table_data.zone_number << "_" << zone_node->data.table_data.area_number << rss_file_ext;
    fname.ToLower();
    if(!overwrite_output_webpage) insert_time_stamp(fname);

    gxString blank_zone = fname.Mid(4, 3);
    blank_zone.ToUpper();
    if(blank_zone == "BBB") {
      zone_node = zone_node->next;
      continue;
    }

    gxRSSfeed rss;
    gxString feed;

    if(!RSSWebmaster.is_null()) rss.channel.webmaster = RSSWebmaster;
    if(!RSSManagingEditor.is_null()) rss.channel.managingeditor = RSSManagingEditor;
    
    if((!RSSImageURL.is_null()) && (!RSSImageLink.is_null()) &&
       (!RSSImageTitle.is_null()) && (!RSSImageDescription.is_null())) {
      rss.channel.image.url = RSSImageURL; 
      rss.channel.image.link = RSSImageLink;
      rss.channel.image.title = RSSImageTitle;
      rss.channel.image.description = RSSImageDescription;
    }

    if((!RSSTextInputTitle.is_null()) && (!RSSTextInputDescription.is_null()) &&
       (!RSSTextInputName.is_null()) && (!RSSTextInputLink.is_null())) {
      rss.channel.textinput.title = RSSTextInputTitle;
      rss.channel.textinput.description = RSSTextInputDescription;
      rss.channel.textinput.name = RSSTextInputName;
      rss.channel.textinput.link = RSSTextInputLink;
    }
    
    rss.channel.title << clear << zone_node->data.name << " Watches, Warnings, &amp; Advisories";
    rss.channel.link = CustomProductURL;
    rss.channel.description = rss.channel.title; 
    
    SysTime systime;
    gxString issue_time_string = systime.GetGMTDateTime();
    rss.channel.lastbuilddate = issue_time_string;
    rss.channel.pubdate = issue_time_string;
    rss.channel.language = "en-us";
    rss.channel.generator << clear << ProgramDescription << " " << VersionString;
    rss.channel.copyright = "None";

    guid++;
    gxRSSitem rss_item;
    rss_item.title = zone_node->data.name;
    rss.channel.lastbuilddate = issue_time_string;
    rss_item.pubdate = issue_time_string;
    rss_item.description << clear << zone_node->data.name << "<br /><br />";
    
    if(!zone_node->data.product_linked_icons.IsEmpty()) {
      if(!zone_node->data.table_data.zone_url.is_null()) {
	rss_item.link = zone_node->data.table_data.zone_url;
      }
      else {
	rss_item.link = CustomProductURL;
      }
      
      gxListNode<IconLink> *ilptr = zone_node->data.product_linked_icons.GetHead();
      while(ilptr) {
	gxString linked_icon;
	if(ilptr->data.link.IFind("onClick") != -1) {
	  // This is JavaScript POPUP
	  sbuf << clear << ilptr->data.link;
	  sbuf.DeleteBeforeIncluding(">\n"); // Dump the <a href="..."> tags with JavaScript code
	  sbuf.FilterString("</a>");
	  sbuf.DeleteAfterIncluding(" title=");
	  sbuf << ">";
	  linked_icon << clear << "<a href=\"" << CustomProductURL << "\">";
	  linked_icon << sbuf << "</a>";
	}
	else {
	  if(ilptr->data.link.IFind("http://") == -1) {
	    // This is a link without the full URL
	    sbuf << clear << ilptr->data.link;
	    sbuf.DeleteBeforeIncluding(">\n"); // Dump the <a href="..."> tags unless we know the entire URL
	    sbuf.FilterString("</a>");
	    sbuf.DeleteAfterIncluding(" title=");
	    sbuf << ">";
	    linked_icon << clear << "<a href=\"" << CustomProductURL << "\">";
	    linked_icon << sbuf << "</a>";
	  }
	  else {
	    linked_icon << clear << ilptr->data.link;
	  }
	}
	linked_icon << "&nbsp;-&nbsp;" << ilptr->data.title << "<br>";
	rss_item.description << linked_icon;
	ilptr = ilptr->next;
      }
    
    }
    else {
      gxString linked_icon;
      linked_icon << clear << "<a href=\"" << CustomProductURL << "\">";
      linked_icon << "None reported this post"  << "</a>";
      rss_item.description << linked_icon;
    }

    rss_item.guid << clear << rss_item.link << "#" << guid;
    rss.channel.items.Add(rss_item);
    rss.WriteFeed(feed);

    if(!write_file(fname, rss_output_dir, feed)) {
      NT_print("Error writing RSS feed for", zone_node->data.table_data.zone_number.c_str());
    }

    zone_node = zone_node->next;
  }

  return 1;
}
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //

