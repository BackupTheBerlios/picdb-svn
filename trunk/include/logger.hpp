/******************************************************************
*                logger.hpp 
*   Sun Nov 21 12:43:41 2004
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


#ifndef PICDB_LOGGER_HPP
#define PICDB_LOGGER_HPP
#include <stdlib.h>
#include <string.h>
#include <list>
#include <time.h>
#include <stdarg.h>
#include "picdb.hpp"

namespace Picdb
{
  class Logger;
  class LogStream;
  class LogLevel;
  class TerminalLogStream;
  extern LogLevel Log_Debug;
  extern LogLevel Log_Info;
  extern LogLevel Log_Warning;
  extern LogLevel Log_Error;
  extern LogLevel Log_Critical;
  extern Logger Log;
}

class Picdb::LogLevel
{
public:
  LogLevel (int lvl=0, char *lname=NULL, int clr=0);
  ~LogLevel ();
  int operator ==(LogLevel&);
  LogLevel &operator =(LogLevel&);
  int operator <(LogLevel&);
  int operator >(LogLevel&);
  int operator >=(LogLevel&);
  int operator <=(LogLevel&);
  char *GetName(){return name;}
  int GetLevel(){return level;}
  int GetColor(){return color;}
protected:
  int level;
  int color;
  char *name;
};

class Picdb::LogStream //Default implementation just writest to stdout
{
public:
  LogStream();
  virtual ~LogStream();
  virtual void Write (Picdb::LogLevel &lvl, char *subsystem, char *txt);
  virtual void Write (Picdb::LogLevel &lvl, char *file, unsigned long line, char *subsystem, char *txt);
private:
};

class Picdb::TerminalLogStream : public Picdb::LogStream //behaves exactly like the default implementation, but colorises the output
{
public:
  virtual void Write (Picdb::LogLevel &lvl, char *subsystem, char *txt);
  virtual void Write (Picdb::LogLevel &lvl, char *file, unsigned long line, char *subsystem, char *txt);
};

class Picdb::Logger
{
public:
  Logger ();
  ~Logger (); 
  void Log (Picdb::LogLevel &lvl, char *subsystem, char *fmt,...);
  void Log (Picdb::LogLevel &lvl, char *file, unsigned long line, char *subsytem, char *fmt, ...);
  void LogSimple (Picdb::LogLevel &lvl, char *subsytem, char *msg);
  void LogSimple (Picdb::LogLevel &lvl, char *file, unsigned long line, char *subsytem, char *msg);
  void SetMask (Picdb::LogLevel &lvl) {mask=lvl;}
  LogLevel &GetMask (){return mask;}
  void AddStream (Picdb::LogStream *s);
  //void AddStream (Picdb::LogStream &s);
protected:
  Picdb::LogLevel mask;
  std::list<LogStream*> streams;
};

#define LogMessage(lvl, subsys, format, ...) ::Picdb::Log.Log (lvl, __FILE__, __LINE__, subsys, format, ## __VA_ARGS__)
#endif
