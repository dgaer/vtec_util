// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ---------------------------------------------------------------- // 
// C++ Header File Name: m_prod.h
// C++ Compiler Used: MSVC, GCC
// Produced By: Douglas.Gaer@noaa.gov
// File Creation Date: 01/25/2008
// Date Last Modified: 07/17/2009
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

Product class
*/
// ----------------------------------------------------------- // 
#ifndef __M_PRODUCT_HPP__
#define __M_PRODUCT_HPP__

#include "gxdlcode.h"

class TextProduct_header
{
public:
  TextProduct_header() { Clear(); }
  ~TextProduct_header() { }
  TextProduct_header(const TextProduct_header &ob) { Copy(ob); }
  TextProduct_header &operator=(const TextProduct_header &ob);

public:
  void Copy(const TextProduct_header &ob);
  void Clear();

public:
  gxString wmo_hdr_string; // TTAAii CCCC YYGGgg (BBB)
  gxString pil; // AAACCC examples: NPWMFL, HWOMFL, NOWMFL
  // NOTE: Can be an 9 letter pil from ZCZC headers MIANOWMFL
  // NOTE: if the product does not have a pil under the WMO header
  gxString name; // Long name of product
  gxString issue_office; // Long name of issuing office
  TextProduct_time_header issue_time; // Time product was issued
  gxList<UGC> ugc_list; // List of UGC elements found in this product
  int num_ugc_segments;
  
  // 06/01/2009: Added variable for global text that affects all ZONE in a 
  // 06/01/2009: segmented VTEC product. This includes all text in the product
  // 06/01/2009: before the first VTEC segment starts.
  gxList<gxString> global_text;
};

class TextProduct
{
public:
  TextProduct() { }
  ~TextProduct() { }
  TextProduct(const TextProduct &ob) { Copy(ob); }
  TextProduct &operator=(const TextProduct &ob);

public:
  void Copy(const TextProduct &ob);
  void Clear();

public: // Main user interface
  int ParseProduct(const char *fname);

public: // Load and parsing functions
  int LoadProduct(const char *fname);
  int ParseProduct();
  int ParseWMOHeader();
  int ParseProductHeader();
  int ParseUGCSegments();
  int TestProductTime(const char *linebuf);

public: // Test and formatting functions
  void PrintProductInfo();
  void PrintTimeHeader(TextProduct_time_header &th);

public: // Output functions
  int ReadProduct();
  int RSSProduct();

private: // Internal processing functions
  int parse_product_time_header(TextProduct_time_header &th);
  int grep_text(gxList<gxString> &text_segment, gxString &stext_buf,
		const gxString &start_str_in, const gxString &end_str_in,
		const gxString &keep_start_str, const gxString &keep_end_str,
		const gxString &multi_line, const gxString &get_all,
		const gxString &filter_str);

public:
  TextProduct_header product_hdr; // Parsed product elements
  gxList<gxString> product; // In-memory copy of the product (unchanged)
  gxString error_string; // Error sting
};

#endif // __M_PRODUCT_HPP__
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
 
