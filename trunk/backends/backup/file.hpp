/******************************************************************
*                file.hpp 
*   Sun Nov 21 15:47:29 2004
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


#ifndef PICDB_FILE_HPP
#define PICDB_FILE_HPP
#include "../include/picdb.hpp"
#include "../include/backend.hpp"
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include "../include/logger.hpp"
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "../include/warning.hpp"
#include "../include/filedata.hpp"
#include "../include/memorymanager.hpp"

namespace Picdb
{
  class File;
}
class Picdb::File:public Picdb::Backend
{
public:
  File (char *basedir);
  virtual ~File (); 
  virtual Filelist *GetFiles();
  virtual FileData *GetFileData(char *fmd5);
  virtual Image *GetImage (char *fmd5);
  virtual Image *GetThumbnail (char *fmd5);
protected:
  char *mBasedir;
};
extern "C" Picdb::Backend *LoadBackend(char *name, int argc, char **argv);
extern "C" void UnloadBackend (Picdb::Backend *b, char *name, int argc, char **argv);
#endif
