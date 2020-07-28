// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ---------------------------------------------------------------- // 
// C++ Header File Name: m_util.h
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

Utility classed and functions
*/
// ----------------------------------------------------------- // 
#ifndef __M_UTIL_HPP__
#define __M_UTIL_HPP__

#include "gxdlcode.h"

struct FileListNode
{
  FileListNode() { Clear(); }
  FileListNode(const char *f) { Set(f); }
  ~FileListNode() { }
  FileListNode(const FileListNode &ob) { Copy(ob); }
  FileListNode &operator=(const FileListNode &ob);

  void Clear();
  int Set(const char *f);
  void Copy(const FileListNode &ob);
  friend int operator==(const FileListNode &a, const FileListNode &b);
  friend int operator<(const FileListNode &a, const FileListNode &b);

  gxString fname;
  time_t mod_time;
};

// Non-thread safe sleep function
void api_Sleep(unsigned long seconds);

// Non-thread safe console logging functions, with logging to log file
int NT_print(const char *mesg1, const char *mesg2 = 0, const char *mesg3 = 0);
int NT_printlines(const char *mesg1, const char *mesg2 = 0, 
		  const char *mesg3 = 0);
int NT_printblock(const char *mesg1, const char *mesg2 = 0, 
		  const char *mesg3 = 0, int lines = 0, int blob = 0,
		  int standard_error = 0, int log_formatting = 1,
		  int log_to_file = 1, int use_verbose_mode = 0,
		  LogFile *my_logfile = 0);

// Non-thread safe console only write functions, without logging
int NT_console_write(const char *mesg1, const char *mesg2 = 0, const char *mesg3 = 0);
int NT_console_write_lines(const char *mesg1, const char *mesg2 = 0, 
			   const char *mesg3 = 0);
int NT_console_write_err(const char *mesg1, const char *mesg2 = 0, const char *mesg3 = 0);
int NT_console_write_lines_err(const char *mesg1, const char *mesg2 = 0, 
			       const char *mesg3 = 0);


// Web download utility functions
int DownloadProduct(const char *URL, const char *URL_backup, gxString &fname);
int DownloadProduct(const char *URL, gxString &fname);
int DownloadProduct(const char *URL, gxsHTTPHeader &hdr, gxString &fname);

// File utilities
int is_file_closed(const char *fname, int how_old);
int gen_file_date_stamp(gxString &stamp);
int insert_time_stamp(gxString &fname);
int WriteWebPageFile(gxString &fname, gxString &pagefilename, gxString &webpage);
int WriteWorksheet(gxString &fname, gxString &worksheet);
int write_file(gxString &fname, const gxString &content);
int write_file(gxString &fname, gxString &out_dir, const gxString &content);

// Formatting utilities
int encode_uri(const gxString &unencoded_str, gxString &encoded_str);
int encode_uri(gxString &uri);
int decode_uri(const gxString &encoded_str, gxString &unencoded_str);
int decode_uri(gxString &uri);

#endif // __M_UTIL_HPP__
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
 
