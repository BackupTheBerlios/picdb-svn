/******************************************************************
*                unixos.hpp 
*   Thu Dec 30 13:32:29 2004
*   Copyright 2004 Jens Krafczyk
*   segv@gmx.de
******************************************************************/

/******************************************************************
* 
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation; either version 2 of the License, or
*   (at your option) any later version.
* 
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU Library General Public License for more details.
* 
*   You should have received a copy of the GNU General Public License
*   along with this program; if not, write to the Free Software
*   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
* 
******************************************************************/


#ifndef PICDB_UNIXOS_HPP
#define PICDB_UNIXOS_HPP
#include "os.hpp"
#include "memorymanager.hpp"
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>

namespace Picdb
{
  class UnixOS;
}
class Picdb::UnixOS: public Picdb::OS
{
public:
  //UnixOS ();
  //virtual ~UnixOS (); 
  virtual void *OpenLibrary (const char *libname);
  virtual void CloseLibrary (void *libhdl);
  virtual void *GetLibrarySymbol (void *libhdl, const char *symname);
  virtual void *Fopen (const char *filename, const char *perm);
  virtual size_t Fread (void *handle, size_t numbytes, void *buffer);  //if numbytes==0 and the buffer was allocated using MManager, the complete file is read
  virtual size_t Fwrite (void *handle, size_t numbytes, void *buffer); //if numbytes==0 and the buffer was allocated using MManager, the complete buffer is written
  virtual void Fclose (void *handle);
  virtual void *Diropen (const char *dirname);
  virtual int ReadDirEntry (void *handle, char *entryname, int *ftype); //ftype=0 for directorries and ftype=1 for files, 2 for unknown.
  virtual void Dirclose (void *handle);
};
#endif
