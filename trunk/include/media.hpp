/******************************************************************
*                media.hpp 
*   Tue Jan  4 00:47:42 2005
*   Copyright 2005 Jens Krafczyk
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


#ifndef PICDB_MEDIA_HPP
#define PICDB_MEDIA_HPP
#include "picdb.hpp"
#include "os.hpp"

namespace Picdb
{
  class Media;
}
class Picdb::Media
{
public:
  Media (char *filename, char *mtype);
  Media (void *data, size_t lenght, char *mtype);
  void Reset();
  size_t Seek(size_t); //returns current position from beginning of file
  size_t Tell();
  size_t Read (size_t);
  size_t Read ();
  bool Eof (); 
  void Thumbnail();    //creates a thumbnail or preview version of the file, if possible (only images)
  char *GetMtype();
  virtual ~Media (); 
protected:
  char *MType;
};
#endif
