/******************************************************************
*                logger.cc 
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


#include "../include/logger.hpp"

Picdb::LogLevel Picdb::Log_Debug (0, "Debug", 15);
Picdb::LogLevel Picdb::Log_Info (5, "Info", 12);
Picdb::LogLevel Picdb::Log_Warning (10, "Warning", 4);
Picdb::LogLevel Picdb::Log_Error (15, "Error", 1);
Picdb::LogLevel Picdb::Log_Critical (20, "Critical", 11);
Picdb::Logger Picdb::Log;


////////////////////Levels//////////////////////////////////
Picdb::LogLevel::LogLevel (int lvl, char *lname, int clr)
{
  if (lname)
    name = strdup (lname);
  else
    name=NULL;
  level=lvl;
  if (!clr)
    color=level%10;
  else
    color=clr;
}

Picdb::LogLevel::~LogLevel ()
{
  if (name)
    {
      free (name);
      name=NULL;
    }
}

int Picdb::LogLevel::operator == (LogLevel &l)
{
  return l.level == level;
}

Picdb::LogLevel & Picdb::LogLevel::operator = (LogLevel &l)
{
  if (name)
    {
      free (name);
      name=NULL;
    }
  if (l.name)
    name = strdup (l.name);
  level=l.level;
  return *this;
}

int Picdb::LogLevel::operator < (LogLevel &l)
{
  return level < l.level;
}

int Picdb::LogLevel::operator > (LogLevel &l)
{
  return level > l.level;
}

int Picdb::LogLevel::operator >= (LogLevel &l)
{
  return level >= l.level;
}

int Picdb::LogLevel::operator <= (LogLevel &l)
{
  return level <= l.level;
}

////////////////////Streams/////////////////////////////////
Picdb::LogStream::LogStream(){}
Picdb::LogStream::~LogStream(){}

void Picdb::LogStream::Write (Picdb::LogLevel &lvl, char *subsystem, char *txt)
{
  Write (lvl, NULL, 0, subsystem, txt);
}

void Picdb::LogStream::Write (Picdb::LogLevel &lvl, char *file, unsigned long line, char *subsystem, char *txt)
{
  time_t T;
  time(&T);
  char *ct;
  ct=ctime(&T);// Flawfinder: ignore
  ct[strlen(ct)-1]=0;
  printf ("%s <%s> <%s:%li> %s %s\n", ct, subsystem, file?file:"?", line?line:0, lvl.GetName()?lvl.GetName():"?", txt);
}

void Picdb::TerminalLogStream::Write (Picdb::LogLevel &lvl, char *file, unsigned long line, char *subsystem, char *txt)
{
  time_t T;
  time(&T);
  char *ct;
  ct=ctime(&T);// Flawfinder: ignore
  ct[strlen(ct)-1]=0;
  printf ("\033[37;1m%s\033[0m <\033[34;1m%s\033[0m> <\033[34;1m%s:%li\033[0m> \033[3%i;1m%s\033[0m %s\n", ct, subsystem, file?file:"?", line?line:0, lvl.GetColor()%10, lvl.GetName()?lvl.GetName():"?", txt);
}

void Picdb::TerminalLogStream::Write (Picdb::LogLevel &lvl, char *subsystem, char *txt)
{
  Write (lvl, NULL, 0, subsystem, txt);
}
////////////////////Loggers/////////////////////////////////

Picdb::Logger::Logger ()
{
}

Picdb::Logger::~Logger ()
{
  Log (Picdb::Log_Info, __FILE__, __LINE__, "Logger", "Shutting down Logger %p", this);
  std::list <Picdb::LogStream*>::iterator iter;
  for (iter = streams.begin();iter!=streams.end();iter++)
    {
      delete (*iter);
      *iter=NULL;
    }
}

void Picdb::Logger::AddStream (Picdb::LogStream *s)
{
  if (s)
    streams.push_back (s);
}
void Picdb::Logger::Log (Picdb::LogLevel &lvl, char *subsystem, char *fmt, ...)
{
  //printf ("Message of level %i, minlvl is %i", lvl.GetLevel(), mask.GetLevel());
  if (lvl < mask)
    return;
  char *buffer;
  std::list<Picdb::LogStream*>::iterator iter;
  va_list ap;
  va_start (ap, fmt);
  vasprintf (&buffer, fmt, ap);
  va_end(ap);
  for (iter=streams.begin();iter!=streams.end();iter++)
    {
      (*iter)->Write (lvl, subsystem, buffer);
    }
  free (buffer);
}

void Picdb::Logger::Log (Picdb::LogLevel &lvl, char *file, unsigned long line, char *subsystem, char *fmt, ...)
{
  
  if (lvl < mask)
    return;
  char *buffer;
  std::list<Picdb::LogStream*>::iterator iter;
  va_list ap;
  va_start (ap, fmt);
  ::vasprintf (&buffer, fmt, ap);
  va_end(ap);
  for (iter=streams.begin();iter!=streams.end();iter++)
    {
      (*iter)->Write (lvl, file, line, subsystem, buffer);
    }
  //free (buffer);
}

void Picdb::Logger::LogSimple (Picdb::LogLevel &lvl, char *file, unsigned long line, char *subsystem, char *msg)
{
  if (lvl<mask);
  for (std::list<Picdb::LogStream*>::iterator iter=streams.begin(); iter!=streams.end();iter++)
    (*iter)->Write (lvl, file, line, subsystem, msg);
}

void Picdb::Logger::LogSimple (Picdb::LogLevel &lvl, char *subsystem, char *msg)
{
  if (lvl<mask);
  for (std::list<Picdb::LogStream*>::iterator iter=streams.begin(); iter!=streams.end();iter++)
    (*iter)->Write (lvl, subsystem, msg);
}
