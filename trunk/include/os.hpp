/******************************************************************
*                os.hpp 
*   Tue Dec  7 15:48:11 2004
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


#ifndef PICDB_OS_HPP
#define PICDB_OS_HPP
#include "picdb.hpp"
namespace Picdb
{
  class OS; //should be a singleton or even only define static functions.
}
class Picdb::OS 
{
public:
  OS ();
  virtual void *OpenLibrary (const char *libname)=0;
  virtual void CloseLibrary (void *libhdl)=0;
  virtual void *GetLibrarySymbol (void *libhdl, const char *symname)=0;
  virtual void *Fopen (const char *filename, const char *perm)=0;
  virtual size_t Fread (void *handle, size_t numbytes, void *buffer)=0;  //if numbytes==0 and the buffer was allocated using MManager, the complete file is read
  virtual size_t Fwrite (void *handle, size_t numbytes, void *buffer)=0; //if numbytes==0 and the buffer was allocated using MManager, the complete buffer is written
  virtual void Fclose (void *handle)=0;
  virtual void *Diropen (const char *dirname)=0;
  virtual int ReadDirEntry (void *handle, char *entryname, int *ftype)=0; //ftype=0 for directorries and ftype=1 for files, 2 for unknown.
  virtual void Dirclose (void *handle)=0;
  virtual ~OS (); 
  //protected:
  static OS* Instance;
};
#endif
