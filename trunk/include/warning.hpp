/******************************************************************
*                warning.hpp 
*   Tue Nov 23 20:13:21 2004
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


#ifndef PICDB_WARNING_HPP
#define PICDB_WARNING_HPP
#include "exception.hpp"
#include "picdb.hpp"
namespace Picdb
{
  class Warning;
  class ErrorType;
}
class Picdb::Warning
{
public:
  Warning (Picdb::ErrorType &e, char *f, unsigned long l, char *s, char *fmt, ...);
  virtual ~Warning ();  
};
#define PicdbWarning(etype, subsys, fmt, ...) new ::Picdb::Warning(etype, __FILE__, __LINE__, subsys, fmt, ## __VA_ARGS__)
#endif
