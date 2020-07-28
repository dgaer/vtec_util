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

RSS helper code
*/
// ----------------------------------------------------------- // 
#include "gxdlcode.h"

int TextProduct::RSSProduct()
{
  error_string.Clear();
  gxRSSfeed rss;
  gxString global_text;
  gxString segment_text;
  gxString rp_out;

  gxListNode<gxString> *sptr;
  int html = 0;

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

  
  if(ReadVerbose == "TRUE") {
    rp_out << "\n";
    if(html) rp_out << "<p>";
    rp_out << "Reading product: " << product_hdr.pil.c_str() << "\n";
    if(html) rp_out << "</p>";
    rp_out << "\n";
  }

  // All text products should use the following site:
  // http://forecast.weather.gov/product.php?site=NWS&issuedby=MFL&product=AFD&format=TXT&version=1&glossary=0
  // See notes in m_warning_table.cpp file too.
  // Our config file string will look the following:
  // http://forecast.weather.gov/product.php?site=SITEVAR&amp;issuedby=ISSUEDBYVAR&amp;product=PRODUCTVAR&amp;format=FORMATVAR&amp;version=VERSIONVAR&amp;glossary=GLOSSARYVAR
  gxString MYPRODUCT = product_hdr.pil;
  gxString ISSUEDBYVAR = product_hdr.pil.Right(3);
  gxString PRODUCTVAR = product_hdr.pil.Left(3);
  gxString SITEVAR = "NWS";
  gxString FORMATVAR = "TXT";
  gxString VERSIONVAR = "1";
  gxString GLOSSARYVAR = "0";
  gxString product_url;
  gxString product_url_backup;
  SetupDownloadURLS(product_url, product_url_backup);

  product_url.ReplaceString("MYPRODUCT", MYPRODUCT.c_str());
  product_url.ReplaceString("SITEVAR", SITEVAR.c_str());
  product_url.ReplaceString("ISSUEDBYVAR", ISSUEDBYVAR.c_str());
  product_url.ReplaceString("PRODUCTVAR", PRODUCTVAR.c_str());
  product_url.ReplaceString("FORMATVAR", FORMATVAR.c_str());
  product_url.ReplaceString("VERSIONVAR", VERSIONVAR.c_str());
  product_url.ReplaceString("GLOSSARYVAR", GLOSSARYVAR.c_str());

  rss.encoding = "utf-8";

  rss.rss_version = RSS_version;
  rss.xml_version = XML_version;

  if(CustomProductTitle == "NONE") CustomProductTitle = product_hdr.name;
  if(CustomProductName == "NONE") CustomProductName = product_hdr.name;
  
  if(CustomProductURL == "NONE") CustomProductURL = product_url;
  if(CustomProductDescription == "NONE") CustomProductDescription = product_hdr.name;
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
  gxString issue_time_string = systime.MakeGMTDateTime(product_hdr.issue_time.elapsed_time);
  rss.channel.lastbuilddate = issue_time_string;
  rss.channel.pubdate = issue_time_string;
  rss.channel.language = "en-us";
  rss.channel.generator << clear << ProgramDescription << " " << VersionString;
  rss.channel.copyright = "None";
 
  gxString gtext_buf1;
  gxString gtext_buf2;
  if((!product_hdr.global_text.IsEmpty()) && (ReadGlobalText == "TRUE")) {
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
    // Put all the global text reads together
    global_text = gtext_buf1;
    if((!gtext_buf1.is_null()) && (!gtext_buf1.is_null())) {
      global_text << "\n\n";
    }
    global_text << gtext_buf2;
  }

  if(!product_hdr.ugc_list.IsEmpty()) {
    gxListNode<UGC> *uptr = product_hdr.ugc_list.GetHead();
    while(uptr) {
      gxRSSitem rss_item;
      
      rss_item.title = CustomProductTitle;
      rss_item.link = CustomProductURL;
      gxString ugc_time_string = systime.MakeGMTDateTime(uptr->data.ugc_segment_time.elapsed_time);
      rss.channel.lastbuilddate = issue_time_string;

      rss_item.pubdate = ugc_time_string;
      rss_item.guid = CustomProductURL;

      rss_item.description << clear << product_hdr.name << "<br /><br />";

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
	  rss_item.description << anames << "<br />";
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
	  rss_item.description << cnames << "<br />";
	}
      }

      if((!StartProductText.is_null()) && (!EndProductText.is_null())) {
	gxString stext_buf;
	grep_text(uptr->data.ugc_segment_text, stext_buf,
		  StartProductText, EndProductText,
		  KeepStartProductText, KeepEndProductText,
		  MultiLineText, GetAllProductTextBlocks,
		  FilterStringFromText);
	segment_text << stext_buf;
      }

      if((!global_text.is_null()) || (!segment_text.is_null())) {
	rss_item.description << "<pre>";
      }
      if(!global_text.is_null()) {
	rss_item.description << global_text << "\n";
      }
      if(!segment_text.is_null()) {
	rss_item.description << segment_text;
      }
      if((!global_text.is_null()) || (!segment_text.is_null())) {
	rss_item.description << "</pre>";
      }

      rss.channel.items.Add(rss_item);
      uptr = uptr->next;
    }
  }
  
  rss.WriteFeed(rp_out);

  GXSTD::cout << rp_out.c_str();

  return 1;
}
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
