/******************************************************************
*                xml.hpp 
*   Wed Dec  1 16:47:18 2004
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


#ifndef PICDB_XML_HPP
#define PICDB_XML_HPP
#include "picdb.hpp"
#include "logger.hpp"
#include "exception.hpp"
namespace Picdb
{
  namespace XML
  {
    class XML;
  }
}
class Picdb::XML::XML
{
public:
  XML ();
  virtual ~XML (); 
protected:
  static int smCount;
};
#endif
