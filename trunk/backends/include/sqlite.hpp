/******************************************************************
*                sqlite.hpp 
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


#ifndef PICDB_SQLITE_HPP
#define PICDB_SQLITE_HPP
#include "../include/picdb.hpp"
#include "../include/sqlbackend.hpp"
namespace Picdb
{
  class Sqlite;
}
class Picdb::Sqlite: public Picdb::SqlBackend
{
public:
  Sqlite ();
  virtual ~Sqlite (); 
};
#endif
