// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- // 
// C++ Source Code File Name: m_util.cpp
// Compiler Used: MSVC, GCC
// Produced By: Douglas.Gaer@noaa.gov
// File Creation Date: 01/25/2008
// Date Last Modified: 07/16/2009
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

Utility classed and functions
*/
// ----------------------------------------------------------- // 
#include "gxdlcode.h"

FileListNode &FileListNode::operator=(const FileListNode &ob)
{
  if(&ob != this) Copy(ob);
  return *this;
}

void FileListNode::Copy(const FileListNode &ob)
{
  fname = ob.fname;
  mod_time = ob.mod_time;
}

int operator==(const FileListNode &a, const FileListNode &b)
{
  if((a.mod_time == b.mod_time) && (a.fname == b.fname)) return 1;
  return 0;
}

int operator<(const FileListNode &a, const FileListNode &b)
{
  if(a.mod_time < b.mod_time) return 1;
  if(a.mod_time == b.mod_time) {
    if(a.fname < b.fname) return 1;
  }
  return 0;
}

void FileListNode::Clear()
{
  mod_time = (time_t)0;
  fname.Clear();
}

int FileListNode::Set(const char *f)
{
  Clear();
  if(!f) return 0;
  
  fname = f;
  time_t access_time = (time_t)0; 
  time_t create_time = (time_t)0;

  if(futils_timestamp(fname.c_str(), access_time, mod_time, create_time) != 0) {
    time(&mod_time);
    return 0;
  }
  
  return 1;
}

void api_Sleep(unsigned long seconds)
// Make the process sleep for a specified number of seconds. 
{
#if defined (__WIN32__)
  int i = seconds * 1000; // Convert milliseconds to seconds
  Sleep((DWORD)i);
#elif defined (__UNIX__)
  sleep(seconds);
#else // No native sleep functions are defined
#error You must define a native API: __WIN32__ or __UNIX__
#endif
}

int DownloadProduct(const char *URL, const char *URL_backup, gxString &fname)
{
  NT_print("Attempting to download Web product from primary and backup site if needed");
  gxsHTTPHeader hdr;
  int rv = DownloadProduct(URL, hdr, fname);
  if(!rv) {
    NT_print("Primary download site failed");
    NT_print("Attempting to download from backup site");
    rv = DownloadProduct(URL_backup, hdr, fname);
  }
  return rv;
}

int DownloadProduct(const char *URL, gxString &fname)
{
  NT_print("Attempting to download Web product from primary site only");
  gxsHTTPHeader hdr;
  return DownloadProduct(URL, hdr, fname);
}

int DownloadProduct(const char *URL, gxsHTTPHeader &hdr, gxString &fname)
{
  gxsHTTPClient client;
  gxsURL url;
  gxsURLInfo u;
  fname.Clear();
  gxString sbuf;

  // Put this in an unecoded format
  gxString uri = URL;
  decode_uri(uri);
    
  NT_print("Downloading product");
  if(!URL) {
    NT_print("Fatal error parsing null URL string");
    return 0;
  }

  NT_print("URL:", uri.c_str());

  if(!url.ParseURL(uri.c_str(), u)) {
    NT_print("Fatal error parsing url:", URL);
    return 0;
  }

  if(u.is_dynamic) {
    fname << clear << u.local_query_file.c_str();
  }
  else {
    if((u.file == "?") || (u.file.is_null())) {
      fname << clear << u.path;
      fname.DeleteBeforeLastIncluding("/");
    }
    else {
      fname << clear << u.file;
    }
  }
  fname.TrimTrailing('?');

  int r = download_retries;
  int err = 0;
  MemoryBuffer mbuf;
  while(r--) {
    if(client.Request(u, hdr, mbuf) != 0) {
      NT_print("Error connecting to Web site");
      NT_print("Cannot download:", uri.c_str());
      NT_print(client.SocketExceptionMessage());
      sbuf << clear << "Attemping download again in " 
	   << download_retry_pause << " seconds, retires " << r;
      NT_print(sbuf.c_str());
      api_Sleep(download_retry_pause);
      err = 1;
      continue;
    }
    else {
      err = 0;
      break;
    }
  }

  if(err > 0) {
    NT_print("Cannot download product");
    return 0;
  }

  gxString product;
  product.SetString((const char *)mbuf.m_buf(), mbuf.length());

  // 05/12/2009: Filter PRE tag with class or other attributes
  if(product.Find("<pre ") != -1) {
    product.DeleteBeforeIncluding("<pre ");
    product.DeleteBeforeIncluding("\">");
  }
  product.DeleteBeforeIncluding("<PRE>");
  product.DeleteBeforeIncluding("<pre>");
  product.DeleteAfterIncluding("</PRE>");
  product.DeleteAfterIncluding("</pre>");

  if(!write_file(fname, spool_dir, product)) {
    NT_print("Product download failed");
    return 0;
  }

  NT_print("Product download complete");
  return 1;
}

int NT_print(const char *mesg1, const char *mesg2, const char *mesg3)
// Non-Thread safe print function used to print and log a console message
{
  return NT_printblock(mesg1, mesg2, mesg3, 0, 0, 1, 0, use_logfile, verbose, &logfile);
}

int NT_printlines(const char *mesg1, const char *mesg2, const char *mesg3)
// Non-Thread safe print function used to print and log a console message
{
  return NT_printblock(mesg1, mesg2, mesg3, 1, 0, 1, 0, use_logfile, verbose, &logfile);
}

int NT_console_write(const char *mesg1, const char *mesg2, const char *mesg3)
// Non-Thread safe print function used write a console message
{
  return NT_printblock(mesg1, mesg2, mesg3, 0, 0, 0, 0, 0, 1, 0);
}

int NT_console_write_lines(const char *mesg1, const char *mesg2, const char *mesg3)
// Non-Thread safe print function used write a console message
{
  return NT_printblock(mesg1, mesg2, mesg3, 1, 0, 0, 0, 0, 1, 0);
}

int NT_console_write_err(const char *mesg1, const char *mesg2, const char *mesg3)
// Non-Thread safe print function used write a console message
{
  return NT_printblock(mesg1, mesg2, mesg3, 0, 0, 1, 0, 0, 1, 0);
}

int NT_console_write_lines_err(const char *mesg1, const char *mesg2, const char *mesg3)
// Non-Thread safe print function used write a console message
{
  return NT_printblock(mesg1, mesg2, mesg3, 1, 0, 1, 0, 0, 1, 0);
}

int NT_printblock(const char *mesg1, const char *mesg2, const char *mesg3, 
		  int lines, int blob, int standard_error, int log_formatting,
		  int log_to_file, int use_verbose_mode, LogFile *my_logfile)
  // Non-Thread safe print function used to print and log a console message
{
  if(!mesg1) return 0;

  if(!my_logfile) log_to_file = 0;

  SysTime logtime;
  gxString output_message;

  if(lines) {
    if(log_formatting) {
      if(mesg1) output_message << logtime.GetSyslogTime() 
			       << " " << service_name << ": " << mesg1 << "\n";
      if(mesg2) output_message << logtime.GetSyslogTime() 
			       << " " << service_name << ": " << mesg2 << "\n";
      if(mesg3) output_message << logtime.GetSyslogTime() 
			       << " " << service_name << ": " << mesg3 << "\n";
    }
    else {
      if(mesg1) output_message << mesg1 << "\n";
      if(mesg2) output_message << mesg2 << "\n";
      if(mesg3) output_message << mesg3 << "\n";
    }
  }
  else if(blob) {
    if(log_formatting) {
      output_message << logtime.GetSyslogTime() 
		     << " " << service_name << ": " << "Message Block <<" 
		     << "\n";
      if(mesg1) output_message << mesg1;
      if(mesg2) output_message << mesg2; 
      if(mesg3) output_message << mesg3; 
      output_message << ">>" << "\n";
    }
    else {
      if(mesg1) output_message << mesg1;
      if(mesg2) output_message << mesg2; 
      if(mesg3) output_message << mesg3; 
    }
  }
  else {
    if(log_formatting) {
      output_message << logtime.GetSyslogTime() << " " << service_name << ": ";
      if(mesg1) output_message << mesg1;
      if(mesg2) output_message << " " << mesg2;
      if(mesg3) output_message << " " << mesg3;
      output_message << "\n";
    }
    else {
      if(mesg1) output_message << mesg1;
      if(mesg2) output_message << " " << mesg2;
      if(mesg3) output_message << " " << mesg3;
      output_message << "\n";
    }
  }

  if((log_to_file) && (my_logfile)) {
    if(my_logfile->Open(logfile_name.c_str()) == 0) {
      *(my_logfile) << output_message.c_str();
      my_logfile->df_Flush();
      my_logfile->Close();
    }
    else { // Could not write to log file so echo to STDERR
      gxString error_message;
      error_message << clear << logtime.GetSyslogTime() << " " << service_name << ": " 
		    << "Error writing to log file " << logfile_name.c_str()
		    << "\n";
      error_message << output_message.c_str();
      fprintf(stderr, "%s", error_message.c_str());
      fflush(stderr);
    }
  }

  if(use_verbose_mode) {
    if(standard_error) {
      fprintf(stderr, "%s", output_message.c_str());
      fflush(stderr);
    }
    else {
      GXSTD::cout.write(output_message.c_str(), output_message.length());
      GXSTD::cout.flush();
    } 
  }

  return 1;
}

int is_file_closed(const char *fname, int how_old)
{
  time_t access_time; 
  time_t mod_time;
  time_t create_time;

  if(futils_timestamp(fname, access_time, mod_time, create_time) == 0) {
    time_t curr_time;
    time(&curr_time);
    if(curr_time > 0) {
      if(curr_time > create_time) {
	if((curr_time - create_time) < how_old) {
	  return 0; // The file is open
	}
	else {
	  return 1; // The file is closed
	}
      }
    }
  }

  // The file could not be accessed 
  return -1; 
}

int insert_time_stamp(gxString &fname) 
{
  gxString stamp;
  gen_file_date_stamp(stamp);
  int offset = 0;
  int dot_offset = 0;
  while(offset != -1) {
    offset = fname.Find(".", offset);
    if(offset == -1) break;
    dot_offset = offset;
    offset++;
  }
  if(dot_offset < 0) dot_offset = fname.length()-1;
  fname.InsertAt(dot_offset, stamp);
  return 1;
}

int WriteWebPageFile(gxString &fname, gxString &pagefilename,
		     gxString &webpage)
{
  gxString sbuf;

  // Gen the ouput file names
#if defined (__WIN32__)
  gxString  path_sep_str = "\\";
#else
  gxString path_sep_str = "/";
#endif

  fname.DeleteBeforeLastIncluding(path_sep_str);
  pagefilename = fname;
  
  if(!write_file(fname, WebpageOutputDirectory, webpage)) {
    GXSTD::cout << "Error-file-create" << GXSTD::flush;
    return 0;
  }
  
  if(exec_file) {
    if(!exec_file_command.is_null()) {
      sbuf << clear << exec_file_command;
      sbuf.ReplaceString("MYSRC", fname.c_str()); 
      sbuf.ReplaceString("MYDEST", pagefilename.c_str()); 
      if(debug) NT_print("Executing command", sbuf.c_str());
      system(sbuf.c_str());
    }
  }
  
  GXSTD::cout << fname << "\n" << GXSTD::flush;

  NT_print("Web page write complete", fname.c_str());
  return 1;
}

int gen_file_date_stamp(gxString &stamp)
{
  stamp = "_MMDDYYYY_HHMMSS_";

  time_t systime_gmt; 
  time_t systime_local;
  if(!GetSystemTime(systime_gmt, systime_local)) {
    return 0;
  }

  char s[255];
  char month[25]; char day[25]; char year[25];
  char hour[25]; char minutes[25]; char seconds[25];
  struct tm tm_local;
  struct tm *tm_ptr;
  memset(&tm_local, 0, sizeof(tm_local));

  tm_ptr = gmtime(&systime_local);
  memmove(&tm_local, tm_ptr, sizeof(tm_local));

  strftime(month, 25, "%m", &tm_local);
  strftime(day, 25, "%d", &tm_local);
  strftime(year, 25, "%Y", &tm_local);
  strftime(hour, 25, "%H", &tm_local);
  strftime(minutes, 25, "%M", &tm_local);
  strftime(seconds, 25, "%S", &tm_local);

  sprintf(s, "_%s%s%s_%s%s%s",
	  month, day, year, hour, minutes, seconds);
  stamp = s;
  
  return 1;
}

int WriteWorksheet(gxString &fname, gxString &worksheet)
{
  gxString sbuf;

  if(worksheet_output_dir == "DEFAULT") {
    worksheet_output_dir = WebpageOutputDirectory;
  } 

  NT_print("Writing worksheet file");

  if(!write_file(fname, worksheet_output_dir, worksheet)) {
    NT_print("No worksheets will be generated");
    return 0;
  }

  if(!worksheet_print_cmd.is_null()) {
    sbuf << clear << worksheet_print_cmd << " " << fname;
    NT_print("Printing worksheet", sbuf.c_str());
    system(sbuf.c_str());
  }
  
  NT_print("Worksheet write complete");
  return 1;
}

int encode_uri(const gxString &unencoded_str, gxString &encoded_str)
{
  encoded_str = unencoded_str;
  return encode_uri(encoded_str);
}

int encode_uri(gxString &uri)
{
  uri.ReplaceString("&", "&amp;");
  uri.ReplaceString("<", "&lt;");
  uri.ReplaceString(">", "&gt;");
  uri.ReplaceString("\'", "&apos;");
  uri.ReplaceString("\"", "&quot;");
  return 1;
}

int decode_uri(const gxString &encoded_str, gxString &unencoded_str)
{
  unencoded_str = encoded_str;
  return decode_uri(unencoded_str);
}

int decode_uri(gxString &uri)
{
  uri.ReplaceString("&amp;", "&");
  uri.ReplaceString("&lt;", "<");
  uri.ReplaceString("&gt;", ">");
  uri.ReplaceString("&apos;", "\'");
  uri.ReplaceString("&quot;", "\"");
  return 1;
}


int write_file(gxString &fname, const gxString &content)
{
  gxString out_dir;
  return write_file(fname, out_dir, content);
}

int write_file(gxString &fname, gxString &out_dir, const gxString &content)
{
  gxString sbuf;

  if(fname.is_null()) {
    NT_print("Error null file name", sbuf.c_str()); 
    NT_print("No file will be created");
    return 0;
  }

  gxString path_sep_str;

  // Gen the ouput file names
#if defined (__WIN32__)
  path_sep_str = "\\";
#else
  path_sep_str = "/";
#endif

  if(!out_dir.is_null()) {
    fname.DeleteBeforeLastIncluding(path_sep_str);
    fname.InsertAt(0, path_sep_str);
    fname.InsertAt(0, out_dir);
    
    // Test the output DIR
    sbuf << clear << out_dir;
#if defined (__WIN32__)
    sbuf.TrimTrailing('\\');
#else
    sbuf.TrimTrailing('/');
#endif
    if(!futils_exists(sbuf.c_str())) {
      if(futils_mkdir(sbuf.c_str()) != 0) {
	NT_print("Error making file output directory", sbuf.c_str()); 
	NT_print("No file will be created");
	return 0;
      }
    }
  }

  NT_print("Writing file:", fname.c_str());

  if(futils_exists(fname.c_str())) {
    int file_open_pass = 1;
    int file_open_try = 0; 
    while(!is_file_closed(fname.c_str(), file_open_wait_time)) {
      file_open_try++;
      if(file_open_try == 1) NT_print(fname.c_str(), "is open by another application");
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
      sbuf << clear << "Giving up after " 
	   << file_open_try << " access attempts"; 
      NT_print(sbuf.c_str());
      NT_print("Error creating file", fname.c_str());
      NT_print("Program cannot overwrite existing file");
      file_open_try = 0;
      return 0;
    }
  }
  
  DiskFileB dfile; // Output product file
  dfile.df_Create(fname.c_str());
  if(!dfile) {
    NT_print("Error creating file");
    NT_print(dfile.df_ExceptionMessage());
    dfile.df_Close();
    return 0;
  }
  if(dfile.df_Write((const char *)content.c_str(), 
		    content.length()) != 0) {
    NT_print("Error writing file");
    NT_print("Cannot write to", fname.c_str());
    NT_print(dfile.df_ExceptionMessage());
    dfile.df_Close();
    return 0;
  }
  dfile.df_Close();

  NT_print("Write complete:", fname.c_str());
  return 1;
}

// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
