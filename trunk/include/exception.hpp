/******************************************************************
*                exception.hpp 
*   Sun Nov 21 17:04:17 2004
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


#ifndef PICDB_EXCEPTION_HPP
#define PICDB_EXCEPTION_HPP
#include "logger.hpp"
#include "picdb.hpp"
namespace Picdb
{
  class Exception;
  class ErrorType;
  class LogLevel;
  namespace Errors
  {
    extern ErrorType Error;
    extern ErrorType NotImplementedError;
    extern ErrorType FileNotFoundError;
    extern ErrorType ParseError;
    extern ErrorType VersionError;
    extern ErrorType RegexpError;
    extern ErrorType XmlError;
  }
}

class Picdb::ErrorType
{
public:
  ErrorType(char *exc_type="Error"){name=strdup(exc_type);}
  ~ErrorType(){free(name);}
  const char *GetErrorType (){return name;}
  int operator ==(ErrorType &e){return strcmp (name, e.name)==0;}
  int operator !=(ErrorType &e){return strcmp (name, e.name)!=0;}
  ErrorType &operator =(ErrorType &e){free(name); name=strdup(e.name); return *this;}
protected:
  char *name;
};

class Picdb::Exception
{
public:
  Exception (Picdb::ErrorType &e, Picdb::LogLevel &lvl, char *f, unsigned long l, char *s, char *fmt, ...);
  LogLevel &GetLevel(){return level;}
  char *GetFile (){return file;}
  unsigned long GetLine(){return line;}
  char *GetSubsystem(){return subsystem;}
  char *GetMessage(){return msg;}
  ErrorType &GetError(){return exc_type;}
  virtual ~Exception ();
protected:
  void Log (Picdb::LogLevel &lvl, char *f, unsigned long l, char *s, char *m);
  LogLevel level;
  unsigned long line;
  char *subsystem;
  char *msg;
  char *file;
  ErrorType exc_type;
};
#define PicdbError(etype, lvl, subsys, fmt, ...) throw new ::Picdb::Exception(etype, lvl, __FILE__, __LINE__, subsys, fmt, ## __VA_ARGS__)
#endif
