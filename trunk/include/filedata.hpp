/******************************************************************
*                filedata.hpp 
*   Thu Dec 30 16:52:01 2004
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


#ifndef PICDB_FILEDATA_HPP
#define PICDB_FILEDATA_HPP
#include "memorymanager.hpp"
#include "picdb.hpp"
#include <list>
#include <hash_map.h>
namespace Picdb
{
  class FileData;
}
class Picdb::FileData
{
public:
  FileData (const char *lines);
  void ParseLine (const char *line);
  char *&operator[](const char *idx);
  char *Repr();
  virtual ~FileData (); 
protected:
  struct eqstr
  {
    bool operator()(const char* s1, const char* s2) const
    {
      return strcmp(s1, s2) == 0;
    }
  };
  size_t blocksize;
  size_t size_used;
  hash_map<const char*, char *, hash<const char*>, eqstr> mData;
  //std::list<char*> mKeys;
};
#endif
