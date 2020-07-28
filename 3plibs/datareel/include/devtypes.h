// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- //
// C++ Header File Name: devtypes.h
// C++ Compiler Used: MSVC, BCC32, GCC, HPUX aCC, SOLARIS CC
// Produced By: DataReel Software Development Team
// File Creation Date: 09/20/1999
// Date Last Modified: 06/17/2016
// Copyright (c) 2001-2016 DataReel Software Development
// ----------------------------------------------------------- // 
// ---------- Include File Description and Details  ---------- // 
// ----------------------------------------------------------- // 
/*
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.
 
This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  
USA

This include file is used to define the some of the commonly
used devices types used the GX communications library.
*/
// ----------------------------------------------------------- //   
#ifndef __GX_DEVICE_TYPES_HPP__
#define __GX_DEVICE_TYPES_HPP__

#include "gxdlcode.h"

enum gxDeviceTypes {
  gxDEVICE_VOID = 0,        // Not pointing to any valid device
  gxDEVICE_CONSOLE,         // Using the console for I/O operations 
  gxDEVICE_DATAGRAM_SOCKET, // Using a datagram socket for I/O operations 
  gxDEVICE_DISK_FILE,       // Using a disk file for I/O operations
  gxDEVICE_MEMORY_BUFFER,   // Using a memory buffer for I/O operations
  gxDEVICE_NULL,            // Dump I/O into the bit bucket
  gxDEVICE_SERIAL_PORT,     // Using a serial port for I/O operations
  gxDEVICE_STREAM_SOCKET    // Using a stream socket for I/O operations
};

#endif // __GX_DEVICE_TYPES_HPP__
// ----------------------------------------------------------- // 
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
