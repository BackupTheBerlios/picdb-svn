/******************************************************************
*                unixos.cc 
*   Thu Dec 30 13:32:28 2004
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


#include "../include/unixos.hpp"
/*Picdb::UnixOS::UnixOS ()
{}
Picdb::UnixOS::~UnixOS ()
{}*/

void *Picdb::UnixOS::OpenLibrary (const char *libname)
{
  LogMessage (Log_Info, "OS/Unix", "Loading library %s", libname);
  void *hdl=dlopen (libname, RTLD_NOW);
  if (hdl)
    {
      LogMessage (Log_Info, "OS/Unix", "Ok:%p", hdl);
      return hdl;
    }
  char *xdlname=(char*)PicdbAlloc (MManager->strlen(libname)+7);
  xdlname=MManager->strcpy (xdlname, "lib");
  xdlname=MManager->strcat (xdlname, libname);
  xdlname=MManager->strcat (xdlname, ".so");
  LogMessage (Log_Info, "OS/Unix", "Trying %s instead.", xdlname);
  hdl=dlopen (libname, RTLD_NOW);
  PicdbFree (xdlname);
  if (hdl)
    {
      LogMessage (Log_Info, "OS/Unix", "Ok:%p", hdl);
      return hdl;
    }
  else
    {
      PicdbWarning (Errors::Error, "OS/Unix", "Library %s not found.", libname);
      return NULL;
    }
}

void *Picdb::UnixOS::GetLibrarySymbol (void *libhdl, const char *symname)
{
  LogMessage (Log_Info, "OS/Unix", "Getting symbol %s from library %p", symname, libhdl);
  void *s=dlsym (libhdl, symname);
  if (s)
    LogMessage (Log_Info, "OS/Unix", "Ok");
  else
    PicdbWarning (Errors::Error, "OS/Unix", "Symbol %s not found.", symname);
  return s;
}

void Picdb::UnixOS::CloseLibrary (void *libhdl)
{
  LogMessage (Log_Info, "OS/Unix", "Closing library %p", libhdl);
  dlclose (libhdl);
}

void *Picdb::UnixOS::Fopen (const char *filename, const char *perm)
{
  return fopen (filename, perm);
}

size_t Picdb::UnixOS::Fread (void *handle, size_t numbytes, void *buffer)
{
  return fread (buffer, 1, numbytes, (FILE*)handle);
}

size_t Picdb::UnixOS::Fwrite (void *handle, size_t numbytes, void *buffer)
{
  return fwrite (buffer, 1, numbytes, (FILE*)handle);
}

void Picdb::UnixOS::Fclose (void *handle)
{
  fclose ((FILE*)handle);
}

void *Picdb::UnixOS::Diropen (const char *dirname)
{
  return opendir (dirname);
}

int Picdb::UnixOS::ReadDirEntry (void *handle, char *entryname, int *ftype)
{
  struct dirent *ent=readdir ((DIR*)handle);
  if (!ent)
    return 1; //ERROR
  strcpy (entryname, ent->d_name);
  *ftype=2;
  return 0;
}

void Picdb::UnixOS::Dirclose (void *handle)
{
  closedir ((DIR*)handle);
}
