/******************************************************************
*                exception.cc 
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


#include "../include/exception.hpp"

Picdb::ErrorType Picdb::Errors::Error("Error");
Picdb::ErrorType Picdb::Errors::NotImplementedError("NotImplementedError");
Picdb::ErrorType Picdb::Errors::FileNotFoundError("FileNotFoundError");
Picdb::ErrorType Picdb::Errors::ParseError("ParseError");
Picdb::ErrorType Picdb::Errors::VersionError("VersionError");
Picdb::ErrorType Picdb::Errors::RegexpError ("RegexpError");
Picdb::ErrorType Picdb::Errors::XmlError("XmlError");

Picdb::Exception::Exception (Picdb::ErrorType &et, Picdb::LogLevel &lvl, char *f, unsigned long l, char *s, char *fmt, ...)
{
  file=strdup(f);
  subsystem=strdup (s);
  level=lvl;
  msg=NULL;
  line=l;
  exc_type = et;
  va_list ap;
  va_start(ap,fmt);
  vasprintf (&msg, fmt, ap);
  va_end(ap);
  Log (lvl, f, l, s, msg);
}

void Picdb::Exception::Log (Picdb::LogLevel &lvl, char *f, unsigned long l, char *s, char *m)
{
  char *xsubsystem;
  /*if (file)
    free(file);
  if (subsystem)
    free(subsystem);
  if (msg)
    free (msg);
  file=strdup (f);
  subsystem=strdup (s);
  msg=strdup(m);
  level=lvl;
  line=l; */
  xsubsystem = (char*)(malloc (strlen (subsystem) + strlen(exc_type.GetErrorType()) +5));
  strcpy (xsubsystem, subsystem);
  strcat (xsubsystem, " -> ");
  strcat (xsubsystem, exc_type.GetErrorType());
  Picdb::Log.LogSimple (level, file, line, xsubsystem, msg);
  free (xsubsystem);
}

Picdb::Exception::~Exception ()
{
  if (file)
    free(file);
  file=NULL;
  if (subsystem)
    free(subsystem);
  subsystem=NULL;
  if (msg)
    free (msg);
  msg=NULL;
}
