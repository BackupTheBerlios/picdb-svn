/******************************************************************
*                backend.hpp 
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


#ifndef PICDB_BACKEND_HPP
#define PICDB_BACKEND_HPP
#include "filelist.hpp"
#include "media.hpp"
#include "filedata.hpp"
namespace Picdb
{
  class Backend;
  Backend *LoadBackend (char *libname, char *sig, int argc, char* argv[]);
  void UnloadBackend (char *libname, char *sig, Backend *be, int argc, char *argv[]);
}
class Picdb::Backend
{
public:
  Backend ();
  virtual FileList *GetFileList ();
  virtual FileData *GetFileData (char *fmd5);
  virtual Media *GetMedia (char *fmd5);
  virtual Media *GetThumbnail (char *fmd5);
  virtual bool AddFile (char *fmd5, FileData *fdta, Media *med, Media *thumb=NULL);
  virtual bool ChangeFileData (char *fmd5, FileData *fdta);
  virtual bool ChangeDataKey  (char *fmd5, char *key, char *val);
  virtual bool ChangeFileMedia (char *fmd5, Media *med);
  virtual bool ChangeFileThumbnail (char *fmd5, Media *thumb);
  virtual ~Backend (); 
};
#endif
