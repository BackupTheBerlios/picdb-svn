/******************************************************************
*                backend.cc 
*   Thu Dec 30 16:50:32 2004
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


#include "../include/backend.hpp"
Picdb::Backend::Backend (){}
Picdb::Backend::~Backend (){}

#define exc(f) throw new ::Picdb::Exception (Errors::NotImplementedError, Log_Error, __FILE__, __LINE__, "Backend", "Function %s not implemented", f)

Picdb::FileList *Picdb::Backend::GetFileList(){exc("GetFileList");}
Picdb::FileData *Picdb::Backend::GetFileData(char *fmd5){exc("GetFileData");}
Picdb::Media *Picdb::Backend::GetMedia(char *fmd5){exc("GetMedia");}
Picdb::Media *Picdb::Backend::GetThumbnail(char *fmd5){exc("GetThumbnail");}
bool Picdb::Backend::AddFile(char *fmd5, Picdb::FileData *fdta, Picdb::Media *med, Picdb::Media *thumb){exc("AddFile");}
bool Picdb::Backend::ChangeFileData(char *fmd5, Picdb::FileData *fdta){exc("ChangeFileData");}
bool Picdb::Backend::ChangeDataKey(char *fmd5, char *key, char *val){exc("ChangeDataKey");}
bool Picdb::Backend::ChangeFileMedia(char *fmd5, Picdb::Media *med){exc("ChangeFileMedia");}
bool Picdb::Backend::ChangeFileThumbnail(char *fmd5, Picdb::Media *thumb){exc("ChangeFileThumbnail");}

//ypedef (Backend *)(*)(char *, int, char **);
  typedef Picdb::Backend* (*Factory_t) (char*, int argc, char **argv);
  typedef void (*Destructor_t) (Picdb::Backend*, char*, int argc, char **argv);


Picdb::Backend *Picdb::LoadBackend (char *libname, char *sig, int argc, char* argv[])
{
  void *libhdl=NULL;
  Factory_t funchdl=NULL;
  //(Backend *)(*funchdl)(char *, int, char **);
  Backend *be=NULL;
  char *flibname=(char*)PicdbAlloc (MManager->strlen(libname)+8);
  //flibname=strcpy (flibname, "picdbbe");
  //flibname=strcat (flibname, libname);
  libhdl=OS::Instance->OpenLibrary (libname);
  PicdbFree (flibname);
  if (!libhdl)
    PicdbError (Errors::Error, Log_Error, "Backend", "Failed to load library %s", libname);
  funchdl = (Factory_t)OS::Instance->GetLibrarySymbol (libhdl, "LoadBackend");
  if (!funchdl)
    PicdbError (Errors::Error, Log_Error, "Backend", "Failed to find symbol LoadBackend in %s", libname);
  be = (*funchdl)(sig, argc, argv);
  if (!be)
    PicdbError (Errors::Error, Log_Error, "Backend", "Failed to create backend of type %s with library %s", sig, libname);
  return be;
}
