/******************************************************************
*                filelist.hpp 
*   Sun Dec 19 13:21:22 2004
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


#ifndef PICDB_FILELIST_HPP
#define PICDB_FILELIST_HPP
#include "memorymanager.hpp"
#include <list>
namespace Picdb
{
  class FileList;
}

class Picdb::FileList
{
public:
  FileList ();
  FileList (FileList &);
  FileList &operator -- ();
  FileList &operator ++ ();
  FileList &operator -- (int);
  FileList &operator ++ (int);
  FileList &operator =(FileList &);
  int IsEnd ();
  int IsBegin ();
  void ResetBegin ();
  void ResetEnd();
  char *operator *();
  //pushes back a *duplicate* of the given string !
  void PushBack(const char *); 
  unsigned long int Size();
  void Dump();
  void Clear();
  void Sort();
  virtual ~FileList (); 
protected: 
  std::list<char*>::iterator i;
  std::list<char*> l;
};
#endif
