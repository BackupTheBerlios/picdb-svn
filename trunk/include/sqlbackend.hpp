/******************************************************************
*                sqlbackend.hpp 
*   Sat Dec  4 14:53:57 2004
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


#ifndef PICDB_SQLBACKEND_HPP
#define PICDB_SQLBACKEND_HPP
#include "picdb.hpp"
#include "backend.hpp"
#include <list>
namespace Picdb
{
  class SqlBackend;
}
class Picdb::SqlBackend
{
  //public:
protected:
  typedef std::list<char*> list_t;
  /*SqlBackend ();
    virtual ~SqlBackend ()=0;*/
  virtual int Connect()=0;
  virtual int Exec()=0;
  virtual int Close()=0;
  virtual int RowsLeft()=0;
  virtual int GetNextRow(list_t *rownames, list_t *rowvalues)=0;
};
#endif
