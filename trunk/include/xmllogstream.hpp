/******************************************************************
*                xmllogstream.hpp 
*   Wed Dec  1 14:44:11 2004
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


#ifndef PICDB_XMLLOGSTREAM_HPP
#define PICDB_XMLLOGSTREAM_HPP
#include <map.h>
#include "logger.hpp"
#include "picdb.hpp"
#include <stdio.h>
#include <time.h>
namespace Picdb
{
  class XmlLogStream;
  class XhtmlLogStream;
}
class Picdb::XmlLogStream: public Picdb::LogStream
{
public:
  XmlLogStream (FILE *fp);
  XmlLogStream (char *filename);
  virtual void Write (Picdb::LogLevel &lvl, char *subsystem, char *txt);
  virtual void Write (Picdb::LogLevel &lvl, char *file, unsigned long line, char *subsystem, char *txt);
  virtual ~XmlLogStream (); 
protected:
  XmlLogStream(){}
  virtual void StartDocument();
  char *GetDate();
  char *GetTime();
  FILE *stream;
};
class Picdb::XhtmlLogStream: public Picdb::XmlLogStream
{
public:
  XhtmlLogStream (FILE *fp);
  XhtmlLogStream (char *filename);
  virtual void Write (Picdb::LogLevel &lvl, char *subsystem, char *txt);
  virtual void Write (Picdb::LogLevel &lvl, char *file, unsigned long line, char *subsystem, char *txt);
  virtual  ~XhtmlLogStream();
protected:
  virtual void StartDocument();
  virtual void InitColorNames();
  static std::map <const int, char *> mColorNames;
};
#endif
